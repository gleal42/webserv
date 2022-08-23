/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 09:45:56 by msousa            #+#    #+#             */
/*   Updated: 2022/08/23 03:38:24 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "FileHandler.hpp"
#include "CGIHandler.hpp"
#include <iostream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <dirent.h>


Server::CreateError::CreateError( void )
: std::runtime_error("Failed to create Kernel Queue.") { /* No-op */ }

Server::Server() // private
{
    throw std::runtime_error("Please use non-default constructor");
    // _fd = kqueue();
    // if (_fd < 0)
    //     throw CreateError();
}

Server::Server(const ConfigParser &parser)
{
    this->_fd = epoll_create(1);
    if (_fd < 0){
        throw CreateError();}
	this->_listeners_amount = parser.server_configs.size(); //TODO: should be nbr of listens in config
	Listener		*new_listener;
	for (size_t i = 0; i < _listeners_amount; ++i)
    {
        // std::cout << "Listener Number " << i + 1 << std::endl;
		// Initialize each new Listener with a config from the parser
        ServerConfig	config(parser.config(i));
		new_listener = new Listener(config);
		add_event(new_listener->fd(), (EPOLLIN | EPOLLET));
		_cluster[new_listener->fd()] = new_listener;
	}
}

int	Server::fd() const { return(_fd); }

void Server::add_event(int ident, uint32_t events)
{
    struct epoll_event ev;
   	ev.events = events;
    ev.data.fd = ident; //this was this->_fd
	if (epoll_ctl(this->_fd, EPOLL_CTL_ADD, ident, &ev) == -1){
        throw CreateError();}
}

void Server::switch_event_to(int ident, uint32_t events)
{
    struct epoll_event ev;
   	ev.events = events;
	epoll_ctl(this->_fd, EPOLL_CTL_MOD, ident, &ev);
}


void Server::delete_event(int ident)
{
    struct epoll_event ev;
   	ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
	epoll_ctl(this->_fd, EPOLL_CTL_DEL, ident, &ev);
}

/*
 * File descriptors open are:
 * 3 - Server fd
 * 4 - Server
 * 5 - Accept client request
 * 6 - favicon.ico https://stackoverflow.com/questions/41686296/why-does-node-hear-two-requests-favicon
 *
 * Lines:
 * 18-19 has_active_connections only temporary for now for clean shutdown
*/

void	Server::start( void )
{
	int nbr_events = 0;
    while (1)
    {
		nbr_events = wait_for_events();
		std::cout << "Number of events recorded: " << nbr_events << std::endl;
        if (nbr_events <= 0)
            continue;
        for (int i = 0; i < nbr_events; i++)
        {
            ClusterIter event_fd = _cluster.find(ListQueue[i].data.fd);
            if (event_fd != _cluster.end())
                new_connection(event_fd->second); // New event for non-existent file descriptor
            else
            {
				ConnectionsIter connection_it = _connections.find(ListQueue[i].data.fd);
                if (ListQueue[i].events & EPOLLRDHUP)
                {
                    close_connection(ListQueue[i].data.fd); // If there are no more connections open in any server do cleanup(return)
                    if (_connections.size() == 0)
                        return ;
                }
                else if (ListQueue[i].events == EPOLLIN)
                    read_connection(connection_it->second, ListQueue[i]);
                else if (ListQueue[i].events == EPOLLOUT)
                {
					write_to_connection(connection_it->second);
                    if (connection_it->second->response.is_empty())
                    {
                        connection_it->second->request.clear();
                        connection_it->second->response.clear();
                    }
                }
            }
        }
    }
}

int	Server::wait_for_events()
{
	std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
    struct timespec kqTimeout = {2, 0};
    (void)kqTimeout;
    return (epoll_wait(this->fd(), this->ListQueue, 10, -1));
}

void	Server::new_connection( Listener * listener )
{
	// check if can still add
	Connection * connection  = new Connection(listener->socket());
	int client_fd = connection->fd();
	_connections[client_fd] = connection;
	std::cout << "CLIENT NEW: (" << client_fd << ")" << std::endl;
	add_event(client_fd, (EPOLLIN | EPOLLOUT | EPOLLET));
}

/**
 * The reason why parse is done here is so that we know when we should
 * stop receiving.
 * When we have a big body we need to know both the Content-Length as well
 * as when the body starts and when it ends.
 * This causes the HTTPStatus try catch process duplicated but this analysis
 * needs to be done. Otherwise we might risk stopping a request mid sending.
 **/
void	Server::read_connection( Connection *connection, struct epoll_event const & Event )
{
    std::cout << "\t\t\t\t000000000000000000000\n";
    std::cout << "About to read the file descriptor: " << connection->fd() << std::endl;
    std::cout << "Incoming data has size of: " << Event.data.fd << std::endl;
    connection->request.parse(*connection->socket(), Event);
    if (connection->request._headers.count("Content-Length"))
    {
        std::cout << "Analyzing if whole body was transferred: " << std::endl;
        std::stringstream content_length(connection->request._headers["Content-Length"]);
        size_t value = 0;
        content_length >> value;
        if (connection->request._raw_body.size() < value + 1)
        {
            std::cout << "Body total received :" << connection->request._raw_body.size() << std::endl;
            std::cout << "Content Length :" << value << std::endl;
            std::cout << "Remaining :" << value - connection->request._raw_body.size() << std::endl;
            return ;
        }
    }

    if (connection->request._raw_body.size())
    {
        std::cout << "Final Body size :" << connection->request._raw_body.size() << std::endl;
        std::cout << "Body :" << connection->request._raw_body.data() << std::endl;
    }
    this->switch_event_to(connection->fd(), EPOLLOUT | EPOLLET);
}

void	Server::write_to_connection( Connection *connection )
{
	std::cout << "About to write to file descriptor: " << connection->fd() << std::endl;
	std::cout << "The socket has the following size to write " << ListQueue[0].data.fd << std::endl;
    // Could use for better size efficiency
    if (connection->response.is_empty())
        service(connection->request, connection->response);
    connection->response.send_response(*connection->socket());
    if (connection->response.is_empty())
    {
        std::cout << "Connection was empty after sending" << std::endl;
        this->switch_event_to(connection->fd(), EPOLLIN | EPOLLET);
    }
}

/*
** Uses appropriate handler to service the request, creating the response
** @param:	- [Request] request that has been received
**			- [Response] response that will be sent
** Line-by-line comments:
** @1-3	FileHandler handler or CGIHandler handler
*/

// first listener should be default since fds are usually increasing in number

void	Server::service(Request & req, Response & res)
{
    bool    autoindex_confirmed = false;
    try
    {
        url::decode(req.request_uri.path); // Interpret url as extended ASCII
		std::cout << "\n\tHOST CENAS: " << req.request_uri.host << "\n";
		if (req.request_uri.host.empty())
        {
			std::cout << "EMPTY HOST\n";
            throw HTTPStatus<400>();
        }
        ServerConfig config_to_use = find_config_to_use(req);
        Locations::iterator location_to_use = find_location_to_use(config_to_use, req.request_uri.path);

        resolve_path(req.request_uri.path, config_to_use, location_to_use, autoindex_confirmed);
        if (autoindex_confirmed)
        {
            do_autoindex(req.request_uri.path, res);
            res.build_message(HTTPStatus<200>());
        }
        else if (CGIHandler::extension_is_implemented(get_extension(req.request_uri.path)))
        {
            CGIHandler handler(req.request_uri.path); // probably needs config for root path etc
            handler.service(req, res);
            res.build_message(handler.script_status());
        }
        else
        {
            FileHandler handler; // probably needs config for root path etc
            handler.service(req, res);
            res.build_message(HTTPStatus<200>());
        }
    } catch (BaseStatus &error_status) {
        file::build_error_page(error_status, res);
    }
}

ServerConfig	Server::find_config_to_use(const Request & req)
{
    ServerConfig config_to_use;
    struct addrinfo *host = get_host(req.request_uri.host);

    for (ClusterIter it = _cluster.begin(); it != _cluster.end(); it++)
    {
        std::vector<Listen> listens_vec = it->second->_config.get_listens();
        for(std::vector<Listen>::iterator it_l = listens_vec.begin(); it_l != listens_vec.end(); it_l++){
            if (is_address_being_listened(it_l->ip, (const struct sockaddr_in *)host->ai_addr)
            && it_l->port == req.request_uri.port){
                // if (it->second.is_default())
                if (config_to_use.is_empty())
                    config_to_use = it->second->_config;
                std::vector<std::string> server_names = it->second->_config.get_server_names();
                for (std::vector<std::string>::iterator it_s = server_names.begin(); it_s != server_names.end(); it_s++){
                    if (*it_s == req.request_uri.host)
                        config_to_use = it->second->_config;
                }
            }
        }
    }
    freeaddrinfo(host);
    if (config_to_use.get_server_names().size())
        std::cout << "We will use config with server_name " << config_to_use.get_server_names()[0] << std::endl;
    return (config_to_use);
}

Locations::iterator	Server::find_location_to_use(ServerConfig &server_block, const std::string & path)
{
    std::string path_directory = path;
    if (path_directory[path_directory.size() - 1] != '/')
       path_directory.push_back('/');
	Locations &locations = server_block.get_locations();
    while (path_directory.empty() == false)
    {
        for (Locations::iterator it = locations.begin();
            it != locations.end();
            it++)
            {
                if ((it->first) == path_directory)
                    return (it);
            }
        path_directory.erase(path_directory.end());
        // remove_directory(path_directory);
    }
    // locations.insert("/", LocationConfig());
    // return locations["/"];
    throw HTTPStatus<404>(); // may need to add default / location to match nginx behaviour
}

// Create URL object

void    Server::resolve_path(std::string & path, const ServerConfig & server_conf, Locations::iterator locations, bool & autoindex_confirmed)
{
    std::string root = locations->second.get_root();
    if (root.empty())
    {
		root = server_conf.get_root();
        if (root.empty())
            root = "public";
    }
	if (root[root.size() - 1] == '/')
		root.erase(root.end());
    std::string location_name = locations->first;
	std::string temp_path = root + path;
    if (is_file(temp_path))
    {
        path = temp_path;
        return ;
    }
    if (is_directory(temp_path))
    {
        root = temp_path;
        if (root[root.size() - 1] != '/')
            root.push_back('/');
        std::vector<std::string> indexes;
        indexes = locations->second.get_indexes();
        if (indexes.empty())
        {
            indexes = server_conf.get_indexes();
            if (indexes.empty())
            {
                if (is_file(root + "index.html"))
                {
                    path = root + "index.html";
                    return ;
                }
                throw HTTPStatus<404>();
            }
            std::vector<std::string>::const_iterator index = file::find_valid_index(root, indexes);
            if (index == indexes.end())
            {
                if ((locations->second).get_autoindex() == AUTOINDEX_OFF)
                    throw HTTPStatus<501>(); // Not implemented yet
                autoindex_confirmed = true;
                return ;
                // throw HTTPStatus<403>();
            }
            path = root + (*index);
            return ;
        }
        std::vector<std::string>::const_iterator index = file::find_valid_index(root, indexes);
        if (index == indexes.end())
            throw HTTPStatus<404>();
        path = root + (*index);
    }
    throw HTTPStatus<404>();
}

void				Server::do_autoindex(std::string & path, Response & res){
	std::ifstream	icon;
	DIR	*			dir;
	struct dirent *	de;
    struct stat		st;
    struct tm		tm_time;
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
        std::string time(tmp_time.substr(0, tmp_time.length() - 1));

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
	res.set_header("Content-Length", to_string(html_content.length()));
    res.set_body(html_content);
}


Server::~Server()
{
	for (ClusterIter it = _cluster.begin(); it != _cluster.end(); ++it) {
        close_listener(it->first);
	}
	for (ConnectionsIter it = _connections.begin(); it != _connections.end(); it++) {
        close_connection(it->first);
	}
    close(this->_fd);
}

void	Server::close_listener( int listener_fd )
{
    std::cout << "Closing Listener with fd: " << listener_fd << std::endl;
    this->delete_event(listener_fd);
    delete _cluster[listener_fd];
    _connections.erase(listener_fd);
}

void	Server::close_connection( int connection_fd )
{
    std::cout << "Closing Connection for client: " << connection_fd << std::endl;
    this->delete_event(connection_fd);
    delete _connections[connection_fd];
    _connections.erase(connection_fd);
}
