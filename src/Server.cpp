/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 09:45:56 by msousa            #+#    #+#             */
/*   Updated: 2022/09/08 23:37:11 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "FileHandler.hpp"
#include "CGIHandler.hpp"
#include <iostream>
#include <stdexcept>
#include <dirent.h>

Server::CreateError::CreateError( void )
: std::runtime_error("Failed to create Kernel Queue.") { /* No-op */ }

// private
Server::Server( void ) { throw std::runtime_error("Please use non-default constructor"); }

// The size argument is an indication to the kernel about the number of file descriptors
// a process wants to monitor, which helps the kernel to decide the size of the epoll
// instance. Since Linux 2.6.8, this argument is ignored because the epoll data structure
// dynamically resizes as file descriptors are added or removed from it.

// Getters
int	Server::queue_fd( void ) const { return _queue_fd; }
Listeners	Server::listeners( void ) const { return _listeners; }
Connections	Server::connections( void ) const { return _connections; }
size_t	Server::listeners_amount( void ) const { return _listeners_amount; }

Server::Server(const ConfigParser &parser)
{
	_queue_fd = QUEUE();
	if (_queue_fd < 0) {
		throw CreateError();
	}

	_listeners_amount = parser.configs_amount();

	Listener	*listener;
	for (size_t i = 0; i < _listeners_amount; ++i)
	{
		listener = new Listener(parser.config(i));
		listener_event_read_add(listener->fd()); // TODO: method in listener class
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
		LOG("Number of events recorded: " << n);

		if (n <= 0) {
			continue;
		}

		for (int i = 0; i < n; i++) {
			Event		event(events[i]);
			Listener_it	it = _listeners.find(event.fd());

			if (it != _listeners.end()) {
				// New event for non-existent file descriptor
				connection_new(it->second);
			}
			else {
				Connections_it	connection_it = _connections.find(event.fd());

				if (event.is_close()) {
					connection_close(event.fd());

					// If there are no more connections open in server do cleanup(return)
					if (_connections.size() == 0) {
						return ;
					}
				}
				else if (event.is_read()) {
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
	Connection *	connection = new Connection(listener);
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

	connection->request.parse(*connection, read_size);

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

	connection->response.send_response(*connection);

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
		request_process_config(req, res);
		if (res.autoindex_confirmed == true)
            do_autoindex(req.request_uri.path, res);
		else {
			handler = handler_resolve(req, connection_addr);
			handler->service(req, res);
		}
		res.build_message(handler->script_status());
	} catch (BaseStatus &error_status) {
		res.set_error_page(error_status);
	}
	if (handler != NULL)
		delete handler;
}

void				Server::do_autoindex(std::string & path, Response & res){
	std::ifstream	icon;
	DIR	*			dir;
	struct dirent *	de;
    struct stat		st;
    struct tm		tm_time;
	std::string		time;
    std::string     html_content = "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n</head>\n<body>\n<h3>Index of " + path + "</h3><br>";

    dir = opendir(path.c_str());
    if (path != "")
    if (dir == NULL)
        throw HTTPStatus<404>();
    while ((de = readdir(dir)) != NULL)
    {
        if (*de->d_name == 0 || (*de->d_name == '.' && *(de->d_name + 1) == 0))
            continue ;
        std::string file_path = path + de->d_name;
        std::string file_name(de->d_name);

        gmtime_r(&(st.st_mtim.tv_sec), &tm_time);
        std::string tmp_time(asctime(&tm_time));
        time = tmp_time.substr(0, tmp_time.length() - 1);

        if (lstat(file_path.c_str(), &st) == 0)
        {
            if (S_ISDIR(st.st_mode))
                    html_content += "<div><a href=\"" + file_name + "/\">" + "&emsp;&emsp;&emsp;" + de->d_name + time + "&emsp;-" + "</div><br>";
            else
            {
                size_t file_size(st.st_size);
                std::stringstream ss;
                ss << file_size;
                html_content += "<div><a href=\"" + file_name + "\">" + "&emsp;&emsp;&emsp;" + de->d_name + time + "&emsp;" + ss.str() + "</div><br>";
            }
        }
	    html_content += "\n</body>\n</html>\n";
        closedir(dir);
    }
	res.set_header("Content-Type", "text/html");
	res.set_header("Date", time);
    res.set_body(html_content);
}

void	Server::request_process_config( Request & req, Response & res)
{
	url::decode(req.request_uri.path);
	ServerConfig config_to_use = config_resolve(req, res);
	res.set_server_config(config_to_use);
	Location_const_it location_to_use = path_resolve(req.request_uri, config_to_use);

	res.add_error_list(config_to_use.get_error_pages(), location_to_use->second.get_error_pages());
	const std::vector<std::string> &req_methods = location_to_use->second.get_limit_except();
	if (std::find_if(req_methods.begin(), req_methods.end(), equals(req.method_to_str())) == req_methods.end())
		throw (HTTPStatus<403>());

	long long max_client_body_size = priority_directive(config_to_use.get_max_body_size(), location_to_use->second.get_max_body_size());
	if (max_client_body_size > 0 && ((long long)req._raw_body.size() > max_client_body_size))
		throw (HTTPStatus<413>());
}

ServerConfig   Server::config_resolve(const Request & req, Response & res )
{
	ServerConfig to_use;
	struct addrinfo *host = get_host(req.request_uri.host);

	for (Listener_it it = _listeners.begin(); it != _listeners.end(); it++)
	{
		if (is_address_being_listened(it->second->_config.get_ip(), (const struct sockaddr_in *)host->ai_addr)
			&& it->second->_config.get_port() == req.request_uri.port)
		{
			// if (it->second.is_default())
			if (to_use.is_empty())
			{
				to_use = it->second->_config;
				res.set_header("Host", to_use.get_server_names()[0]);
			}
			std::vector<std::string> server_names = it->second->_config.get_server_names();
			for (std::vector<std::string>::iterator it_s = server_names.begin();
				it_s != server_names.end();
				it_s++)
			{
				if (*it_s == req.request_uri.host)
				{
				    to_use = it->second->_config;
					res.set_header("Host", *it_s);
				}
			}
		}
	}
	freeaddrinfo(host);
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

Server::~Server( void )
{
	for (Listener_it it = _listeners.begin(); it != _listeners.end(); ++it) {
		listener_close(it->first);
	}
	for (Connections_it it = _connections.begin(); it != _connections.end(); it++) {
		connection_close(it->first);
	}
	close(_queue_fd);
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
	_connections.erase(listener_fd);
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
