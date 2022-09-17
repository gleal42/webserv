/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 09:45:56 by msousa            #+#    #+#             */
/*   Updated: 2022/09/17 14:06:13 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "FileHandler.hpp"
#include "CGIHandler.hpp"
#include <iostream>
#include <stdexcept>
#include <dirent.h>
# define WHITESPACE_CAP 50

Server::CreateError::CreateError( void )
: std::runtime_error("Failed to create Kernel Queue.") { /* No-op */ }

// Constructors
Server::Server( void ) : _queue_fd(FD_UNSET), _sigint(false) { /* No-op */ }
Server::Server(const ConfigParser &parser) : _queue_fd(FD_UNSET), _sigint(false)
{ init(parser); }

// The size argument is an indication to the kernel about the number of file descriptors
// a process wants to monitor, which helps the kernel to decide the size of the epoll
// instance. Since Linux 2.6.8, this argument is ignored because the epoll data structure
// dynamically resizes as file descriptors are added or removed from it.

// Getters
int	Server::queue_fd( void ) const { return _queue_fd; }
Listeners	Server::listeners( void ) const { return _listeners; }
Connections	Server::connections( void ) const { return _connections; }
size_t	Server::listeners_amount( void ) const { return _listeners_amount; }
bool	Server::sigint( void ) const { return _sigint; }

// Setters
void	Server::set_sigint( bool called ) { _sigint = called; }

void	Server::init(const ConfigParser &parser)
{
	// guard from calling more then once
	if (_queue_fd != FD_UNSET) {
		return;
	}

	_queue_fd = QUEUE();
	if (_queue_fd < 0) {
		throw CreateError();
	}

	_listeners_amount = parser.configs_amount();

	Listener	*listener;
	for (size_t i = 0; i < _listeners_amount; ++i) {
		try {
			listener = new Listener(parser.config(i));
		}
		catch (SocketError& e) {
			ERROR("Failed listener new: " << e.what());
			continue ;
		}

		listener_event_read_add(listener->fd());
		_listeners[listener->fd()] = listener;
	}
}


/*
 * File descriptors open are:
 * 3 - Server fd
 * 4 - Server
 * 5 - Accept connection request
 * 6 - favicon.ico https://stackoverflow.com/questions/41686296/why-does-node-hear-two-requests-favicon
 *
 * Lines:
 * 18-19 has_active_connections only temporary for now for clean shutdown
*/

void	Server::start( void )
{
	int n = 0;

	while (1) {
		n = events_wait();

		if (sigint()) {
			LOG("\nClosing server...");
			close();
			break;
		}

		if (n == -1) {
			ERROR("Error in kernel queue polling.");
			exit(EXIT_FAILURE);
		}

		LOG("Number of events recorded: " << n);
		for (int i = 0; i < n; i++) {
			Event		event(events[i]);
			Listener_it	it = _listeners.find(event.fd());

			if (it != _listeners.end()) {
				// New event for non-existent file descriptor
				connection_new(it->second);
			}
			else {
				Connections_it	connection_it = _connections.find(event.fd());

				LOG("\n\t\t\tNEW EVENT");
				if (event.is_close()) {
					connection_close(event.fd());
					LOG("\n\t\t\tCONNECTIONS SIZE = " << _connections.size());

					// If there are no more connections open in server do cleanup(return)
					if (_connections.size() == 0) {
						LOG("\n\t\t\tLEAVING");
						return ;
					}
				}
				else if (event.is_read()) {
					LOG("\n\t\t\tDETECTED READ");
					connection_read(connection_it->second, event.read_size());
				}
				else if (event.is_write()) {
					connection_write(connection_it->second);

					if (connection_it->second->response.is_empty()) {
						connection_it->second->request.clear();
						connection_it->second->response.clear();
					}
				}
			}
		}
	}
}

int	Server::events_wait( void )
{
	int		events_ready;
	LOG("\n---------------Waiting for events---------------\n");
#if defined(DARWIN)
	events_ready = kevent(_queue_fd, NULL, 0, events, EVENTS_SIZE, TIMEOUT);
#endif
#if defined(LINUX)
	events_ready = epoll_wait(_queue_fd, events, EVENTS_SIZE, TIMEOUT);
#endif
	return events_ready;
}

void	Server::connection_new( Listener * listener )
{
	// TODO: check if can still add
	Connection *	connection;
	try {
		connection = new Connection(listener);
	}
	catch (SocketError& e) {
		ERROR("Failed connection new: " << e.what());
		return ;
	}

	int 			connection_fd = connection->fd();
	EVENT 			event;

	_connections[connection_fd] = connection;

#if defined(DARWIN)
	EV_SET(&event, connection_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	kevent(_queue_fd, &event, 1, NULL, 0, NULL);

	// Will be used later in case we can't send the whole message
	EV_SET(&event, connection_fd, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, NULL);
	kevent(_queue_fd, &event, 1, NULL, 0, NULL);
#endif
#if defined(LINUX)
	event.data.fd = connection_fd;

	event.events = EPOLLIN;
	epoll_ctl(_queue_fd, EPOLL_CTL_ADD, connection_fd, &event);
	event.events = EPOLLOUT;
	epoll_ctl(_queue_fd, EPOLL_CTL_ADD, connection_fd, &event);
#endif
}

/**
 * The reason why parse is done here is so that we know when we should
 * stop receiving.
 * When we have a big body we need to know both the Content-Length as well
 * as when the body starts and when it ends.
 * This causes the HTTPStatus try catch process duplicated but this analysis
 * needs to be done. Otherwise we might risk stopping a request mid sending.
 **/
void	Server::connection_read( Connection *connection, int read_size )
{
	LOG("About to read the file descriptor: " << connection->fd());
	LOG("Incoming data has size of: " << read_size);

	try {
		connection->request.parse(*connection, read_size);
	}
	catch (SocketError& e) {
		ERROR("Failed connection read: " << e.what());
		connection_close(connection->fd());
		return ;
	}


	if (connection->request._headers.count("Content-Length")) {
		LOG("Analyzing if whole body was transferred: ");

		std::stringstream	content_length(connection->request._headers["Content-Length"]);
		size_t				value = 0;

		content_length >> value;

		if (connection->request._raw_body.size() < value + 1) {
			LOG("Body total received :" << connection->request._raw_body.size());
			LOG("Content Length :" << value);
			LOG("Remaining :" << value - connection->request._raw_body.size());

			return ;
		}
	}

	LOG("|--- Headers ---|");
	LOG(connection->request._raw_headers);
	LOG("|--- Headers ---|");
	if (connection->request._raw_body.size()) {
		LOG("Final Body size :" << connection->request._raw_body.size());
		LOG("Body :" << connection->request._raw_body.data());
	}

	connection_event_toggle_write(connection->fd());
}

void	Server::connection_write( Connection *connection )
{
	LOG("About to write to file descriptor: " << connection->fd());

	if (connection->response.is_empty()) {
		service(connection->request, connection->response, connection->address());
	}

	try {
		connection->response.send_response(*connection);
	}
	catch (SocketError& e) {
		ERROR("Failed connection write: " << e.what());
		connection_close(connection->fd());
		return ;
	}

	if (connection->response.is_empty()) {
		LOG("Connection was empty after sending");

		connection_event_toggle_read(connection->fd());
	}
}

void	Server::connection_event_toggle_write( int connection_fd )
{
	EVENT	event;

#if defined(DARWIN)
	EV_SET(&event, connection_fd, EVFILT_READ, EV_DISABLE, 0, 0, NULL);
	kevent(_queue_fd, &event, 1, NULL, 0, NULL);
	EV_SET(&event, connection_fd, EVFILT_WRITE, EV_ENABLE, 0, 0, NULL);
	kevent(_queue_fd, &event, 1, NULL, 0, NULL);
#endif
#if defined(LINUX)
	event.events = EPOLLOUT;
	event.data.fd = connection_fd;
	epoll_ctl(_queue_fd, EPOLL_CTL_MOD, connection_fd, &event);
#endif
}

void	Server::connection_event_toggle_read( int connection_fd )
{
	EVENT	event;

#if defined(DARWIN)
	EV_SET(&event, connection_fd, EVFILT_READ, EV_ENABLE, 0, 0, NULL);
	kevent(_queue_fd, &event, 1, NULL, 0, NULL);
	EV_SET(&event, connection_fd, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);
	kevent(_queue_fd, &event, 1, NULL, 0, NULL);
#endif
#if defined(LINUX)
	event.events = EPOLLIN;
	event.data.fd = connection_fd;
	epoll_ctl(_queue_fd, EPOLL_CTL_MOD, connection_fd, &event);
#endif
}

/*
** Uses appropriate handler to service the request, creating the response
** @param:	- [Request] request that has been received
**			- [Response] response that will be sent
** Line-by-line comments:
** @1-3	FileHandler handler or CGIHandler handler
*/

// if (req.request_uri.host.empty())
// 	throw HTTPStatus<400>();

void	Server::service(Request & req, Response & res, const in_addr &connection_addr)
{
    Handler *handler = NULL;
	try {
		req.request_uri.autoindex_confirmed = false;
		req.request_uri.redirect_confirmed = false;
		request_process_config(req, res, connection_addr);
		if (req.request_uri.redirect_confirmed == true)
            do_redirect(res);
		else if (req.request_uri.autoindex_confirmed == true)
            do_autoindex(req.request_uri, res);
		else
		{
			handler = handler_resolve(req, connection_addr);
			handler->service(req, res);
			res.build_message(handler->script_status());
		}
	} catch (BaseStatus &error_status) {
		res.set_error_page(error_status);
	}
	if (handler != NULL)
		delete handler;
}

void	Server::do_redirect(Response & res)
{
	res.set_header("Location", res.redirect.new_path);
	res.set_header("Content-Length", to_string(res.get_body_size()));
	res.set_header("Content-Type", "text/html");
	res.set_header("Connection", "keep-alive");
	// if res.redirect.code isn't valid, sends 500 as code. Nginx sends code anyway. Shall we keep it like this?
	res.build_message(get_httpstatus(res.redirect.code));
}

void	Server::do_autoindex(URI & uri, Response & res)
{
	DIR	*			dir;
	struct dirent *	de;
    struct stat		st;
    struct tm		tm_time;
	std::string		time;
	std::string		path = uri.path;
	std::string		private_path(path.substr(6));

    std::string     html_content = "<html>\n<head><title>Index of " + private_path
								 + "</title></head>\n<body>\n<h1>Index of "
								 + private_path + "</h1><hr><pre>"
								 + "<div><a href=\"..\"></div>../</a>";
	std::string		dirs_buffer("");
	std::string		files_buffer("");

    dir = opendir(path.c_str());
    if (dir == NULL)
	{
		perror("opendir");
        throw HTTPStatus<404>();
	}
    while ((de = readdir(dir)) != NULL)
    {
        if (*de->d_name == 0 || *de->d_name == '.')
            continue ;

        std::string file_name(de->d_name);
		std::string file_path = path + '/' + file_name;
        if (lstat(file_path.c_str(), &st) == 0)
        {
			gmtime_r(&(st.st_mtim.tv_sec), &tm_time);
			std::string tmp_time(asctime(&tm_time));
			bool is_dir = S_ISDIR(st.st_mode);

			time = tmp_time.substr(0, tmp_time.length() - 1);
			if (is_dir)
			{
				file_name.push_back('/');
				if (file_name.length() >= WHITESPACE_CAP)
				{
					file_name = file_name.substr(0, WHITESPACE_CAP - 4).append("..> ");
					dirs_buffer += "<div><a href=\"" + std::string(de->d_name) + "\">";
				}
				else
					dirs_buffer += "<div><a href=\"" + std::string(file_name) + "\">";
				dirs_buffer += file_name + "</a>"
							+ insert_whitespace(file_name.length(), WHITESPACE_CAP)
							+ set_time(&tm_time)
							+ insert_whitespace(1, WHITESPACE_CAP/2)
							+ "-</div>";
			}
            else
            {
                size_t file_size(st.st_size);
                std::stringstream ss;
                ss << file_size;
				std::string fsize = ss.str();
				if (file_name.length() >= WHITESPACE_CAP)
				{
					file_name = file_name.substr(0, WHITESPACE_CAP - 4).append("..> ");
					files_buffer += "<div><a href=\"" + std::string(de->d_name) + "\">";
				}
				else
					files_buffer += "<div><a href=\"" + std::string(file_name) + "\">";
				files_buffer += file_name + "</a>"
							 + insert_whitespace(file_name.length(), WHITESPACE_CAP)
							 + set_time(&tm_time)
							 + insert_whitespace(fsize.length(), WHITESPACE_CAP/2)
							 + fsize + "</div>";
            }
        }
    }
    closedir(dir);
	html_content += dirs_buffer + files_buffer + "</pre><hr></body>\n</html>";

	res.set_header("Content-Type", "text/html");
	res.set_header("Content-Length", to_string(html_content.size()));
	// res.set_header("Date", time);
    res.set_body(html_content);
	res.build_message(HTTPStatus<200>());
}

void	Server::request_process_config( Request & req, Response & res, const in_addr &connection_addr)
{
	url::decode(req.request_uri.path);
	ServerConfig config_to_use = config_resolve(req, res, connection_addr);
	res.set_server_config(config_to_use);
	Location_cit location_inside_server = path_resolve(req.request_uri, config_to_use);
	LocationConfig location_to_use;
	if (location_inside_server != config_to_use.get_locations().end())
		location_to_use = location_inside_server->second;
	if (is_redirect(req, res, location_to_use))
		return ;
	res.add_error_list(config_to_use.get_error_pages(), location_to_use.get_error_pages());

	const StringVector &req_methods = location_to_use.get_limit_except();
	if (req_methods.empty() == false && std::find_if(req_methods.begin(), req_methods.end(), equals(req.method_to_str())) == req_methods.end())
		throw (HTTPStatus<403>());

	long long max_client_body_size = priority_directive(config_to_use.get_max_body_size(), location_to_use.get_max_body_size());
	if (max_client_body_size > 0 && ((long long)req._raw_body.size() > max_client_body_size))
		throw (HTTPStatus<413>());

	CGI cgi = location_to_use.get_cgi();
	if (cgi.empty())
		return ;
	size_t script_path_pos = req.request_uri.path.find(cgi.extension);
	if (script_path_pos != std::string::npos)
		req.request_uri.cgi_confirmed = true; // check
	// TODO: add some HTTPstatus error when cgi config path doesn't match CGI Handler or similar logic
}

bool	Server::is_redirect(Request & req, Response & res, LocationConfig & location)
{
	res.redirect.new_path.clear();
	if (req.request_uri.redirect_confirmed == true)
	{
		res.redirect.new_path = "http://";
		res.redirect.new_path += req._headers.at("Host");
		if (strncmp(req.request_uri.path.c_str(), "public/", 7) == 0)
			req.request_uri.path = req.request_uri.path.substr(6);
		res.redirect.new_path += req.request_uri.path + "/";
		res.redirect.code = 301;
		return (true);
	}
	if (location.get_redirects().empty() == false)
	{
		req.request_uri.redirect_confirmed = true;
		if (is_directory("public" + location.get_first_redirect().new_path) || is_file("public" + location.get_first_redirect().new_path))
			res.redirect.new_path = location.get_first_redirect().new_path;
		else
		{
			if (strncmp(location.get_first_redirect().new_path.c_str(), "http://", 7) != 0)
				res.redirect.new_path = "http://";
			res.redirect.new_path += location.get_first_redirect().new_path;
		}
		res.redirect.code = location.get_first_redirect().code;
		return (true);
	}
	return (false);
}
ServerConfig   Server::config_resolve(const Request & req, Response & res , const in_addr &connection_addr)
{
	ServerConfig to_use;
	for (Listener_it it = _listeners.begin(); it != _listeners.end(); it++)
	{
		if (is_address_being_listened(it->second->_config.get_ip(), (const sockaddr_in *)&connection_addr)
			&& it->second->_config.get_port() == req.request_uri.port)
		{
			// if (it->second.is_default())
			if (to_use.is_empty())
			{
				to_use = it->second->_config;
				const StringVector&serv_names = to_use.get_server_names();
				if (serv_names.size() > 0)
					res.set_header("Host", serv_names[0]);
			}
			StringVector potential_server_names = it->second->_config.get_server_names();
			StringVector_it potential_host = std::find_if(potential_server_names.begin(), potential_server_names.end(), equals(req.request_uri.host));
			StringVector to_use_host_names = to_use.get_server_names();
			StringVector_it to_use_host = std::find_if(to_use_host_names.begin(), to_use_host_names.end(), equals(req.request_uri.host));
			if (potential_host != potential_server_names.end() && to_use_host == to_use_host_names.end())
			{
			    to_use = it->second->_config;
				res.set_header("Host", *potential_host);
			}
		}
	}
	if (to_use.get_server_names().size())
		std::cout << "We will use config with server_name " << to_use.get_server_names()[0] << std::endl;
	return (to_use);
}

// if (location_path.back() != '/') Needs redirection to fix
// 	location_path.push_back('/');
// remove_directory(location_path); instead of location_path.erase
// locations.insert("/", LocationConfig());
// return locations["/"];

// CGI and Files have different URI
// size_t path_start = _path.find('/');
// size_t query_string_start = _path.rfind('?');
// if (path_start == std::string::npos) {
// request_uri.path = _path;
// }
// else
// request_uri.path = _path.substr(0, query_string_start);
// if (query_string_start != std::string::npos)
// request_uri.query = _path.substr(query_string_start);

Handler *Server::handler_resolve( Request & req, const in_addr &connection_addr )
{
	std::string extension = get_extension(req.request_uri.path);
	if (CGIHandler::extension_is_implemented(extension))
        return (new CGIHandler(req.request_uri, connection_addr));
    else
        return (new FileHandler());
}

// Destructor
Server::~Server( void ) { close(); }

void	Server::close( void )
{
	for (Listener_it it = _listeners.begin(); it != _listeners.end(); ++it) {
		listener_close(it->first);
	}
	for (Connections_it it = _connections.begin(); it != _connections.end(); it++) {
		connection_close(it->first);
	}
	::close(_queue_fd);
}

void	Server::listener_close( int listener_fd )
{
	LOG("Closing Listener with fd: " << listener_fd);

#if defined(DARWIN)
	EVENT event;

	EV_SET(&event, listener_fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
	kevent(_queue_fd, &event, 1, NULL, 0, NULL);
#endif
#if defined(LINUX)
	epoll_ctl(_queue_fd, EPOLL_CTL_DEL, listener_fd, NULL);
#endif

	delete _listeners[listener_fd];
	// Weird! When I change this to _listeners.erase(listener_fd),
	// which should be the correct one afaik
	// I get a segfault
	_listeners.erase(listener_fd);
}

void	Server::connection_close( int connection_fd )
{
	LOG("Closing Connection: " << connection_fd);

#if defined(DARWIN)
	EVENT event;

	EV_SET(&event, connection_fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
	kevent(_queue_fd, &event, 1, NULL, 0, NULL);
	EV_SET(&event, connection_fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
	kevent(_queue_fd, &event, 1, NULL, 0, NULL);
#endif
#if defined(LINUX)
	epoll_ctl(_queue_fd, EPOLL_CTL_DEL, connection_fd, NULL);
#endif

	delete _connections[connection_fd];
	_connections.erase(connection_fd);
}

void Server::listener_event_read_add(int listener_fd)
{
	EVENT event;

#if defined(DARWIN)
	EV_SET(&event, listener_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	kevent(_queue_fd, &event, 1, NULL, 0, NULL);
#endif
#if defined(LINUX)
	event.events = EPOLLIN;
	event.data.fd = listener_fd;
	epoll_ctl(_queue_fd, EPOLL_CTL_ADD, listener_fd, &event);
#endif
}
