/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 15:26:40 by gleal             #+#    #+#             */
/*   Updated: 2022/08/19 00:30:03 by gleal            ###   ########.fr       */
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
    _fd = kqueue();
    if (_fd < 0)
        throw CreateError();
	_listeners_amount = parser.configs_amount();
	Listener		*new_listener;
	for (size_t i = 0; i < _listeners_amount; ++i)
    {
        std::cout << "Listener Number " << i + 1 << std::endl;
		// Initialize each new Listener with a config from the parser
		ServerConfig	config(parser.config(i));
		new_listener = new Listener(config);
		update_event(new_listener->fd(), EVFILT_READ, EV_ADD);
		_cluster[new_listener->fd()] = new_listener;
	}
}

int	Server::fd() const { return(_fd); }

void Server::update_event(int ident, short filter, u_short flags)
{
    struct kevent kev;
	EV_SET(&kev, ident, filter, flags, 0, 0, NULL);
	kevent(this->_fd, &kev, 1, NULL, 0, NULL);
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
            ClusterIter event_fd = _cluster.find(ListQueue[i].ident);
            if (event_fd != _cluster.end()) // New event for non-existent file descriptor
                new_connection(event_fd->second);
            else
            {
				ConnectionsIter connection_it = _connections.find(ListQueue[i].ident);
                if (ListQueue[i].flags & EV_EOF)
                {
                    close_connection(ListQueue[i].ident); // If there are no more connections open in any server do cleanup(return)
                    if (_connections.size() == 0)
                        return ;
                }
                else if (ListQueue[i].filter == EVFILT_READ)
                    read_connection(connection_it->second, ListQueue[i]);
                else if (ListQueue[i].filter == EVFILT_WRITE)
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
    return (kevent(this->fd(), NULL, 0, ListQueue, 10, NULL));
}

void	Server::new_connection( Listener * listener )
{
	// check if can still add
	Connection * connection  = new Connection(listener->socket());
	int client_fd = connection->fd();
	_connections[client_fd] = connection;
	std::cout << "CLIENT NEW: (" << client_fd << ")" << std::endl;
	update_event(client_fd, EVFILT_READ, EV_ADD | EV_ENABLE);
	update_event(client_fd, EVFILT_WRITE, EV_ADD | EV_DISABLE); // Will be used later in case we can't send the whole message
}

/**
 * The reason why parse is done here is so that we know when we should
 * stop receiving.
 * When we have a big body we need to know both the Content-Length as well
 * as when the body starts and when it ends.
 * This causes the HTTPStatus try catch process duplicated but this analysis
 * needs to be done. Otherwise we might risk stopping a request mid sending.
 **/
void	Server::read_connection( Connection *connection, struct kevent const & Event )
{
    std::cout << "About to read the file descriptor: " << connection->fd() << std::endl;
    std::cout << "Incoming data has size of: " << Event.data << std::endl;
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
    this->update_event(connection->fd(), EVFILT_READ, EV_DISABLE);
    this->update_event(connection->fd(), EVFILT_WRITE, EV_ENABLE);
}

void	Server::write_to_connection( Connection *connection )
{
	std::cout << "About to write to file descriptor: " << connection->fd() << std::endl;
	std::cout << "The socket has the following size to write " << ListQueue[0].data << std::endl; // Could use for better size efficiency
    if (connection->response.is_empty())
        service(connection->request, connection->response);
    connection->response.send_response(*connection->socket());
    if (connection->response.is_empty())
    {
        std::cout << "Connection was empty after sending" << std::endl;
        this->update_event(connection->fd(), EVFILT_READ, EV_ENABLE);
        this->update_event(connection->fd(), EVFILT_WRITE, EV_DISABLE);
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
    try
    {
        url::decode(req.request_uri.path); // Interpret url as extended ASCII
		if (req.request_uri.host.empty())
			throw HTTPStatus<400>();
        ServerConfig config_to_use = find_config_to_use(req);
        
        Locations::const_iterator location_to_use = find_location_to_use(config_to_use, req.request_uri.path);
        
        resolve_path(req.request_uri.path, config_to_use, location_to_use);
        std::string extension = get_extension(req.request_uri.path);
        if (CGIHandler::extension_is_implemented(extension))
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
    ServerConfig to_use;
    struct addrinfo *host = get_host(req.request_uri.host);

    for (ClusterIter it = _cluster.begin(); it != _cluster.end(); it++)
    {
        if (is_address_being_listened(it->second->_config.get_ip(), (const struct sockaddr_in *)host->ai_addr)
            && it->second->_config.get_port() == req.request_uri.port)
        {
            // if (it->second.is_default())
            if (to_use.is_empty())
                to_use = it->second->_config;
            std::vector<std::string> server_names = it->second->_config.get_server_name();
            for (std::vector<std::string>::iterator it_s = server_names.begin();
                it_s != server_names.end();
                it_s++)
            {
                if (*it_s == req.request_uri.host)
                    to_use = it->second->_config;
            }
        }
    }
    freeaddrinfo(host);
    if (to_use.get_server_name().size())
        std::cout << "We will use config with server_name " << to_use.get_server_name()[0] << std::endl;
    return (to_use);
}

Locations::const_iterator	Server::find_location_to_use(const ServerConfig &server_block, const std::string & path)
{
    std::string path_directory = path;
    if (path_directory.back() != '/')
       path_directory.push_back('/');
	const Locations &locations = server_block.get_locations();
    while (path_directory.empty() == false)
    {
        for (Locations::const_iterator it = locations.begin();
            it != locations.end();
            it++)
            {
                if ((it->first) == path_directory)
                    return (it);
            }
        path_directory.pop_back();
        // remove_directory(path_directory);
    }
    // locations.insert("/", LocationConfig());
    // return locations["/"];
    throw HTTPStatus<404>(); // may need to add default / location to match nginx behaviour
}

// Create URL object

void    Server::resolve_path(std::string & path, const ServerConfig & server_conf, Locations::const_iterator locations)
{
    std::string root = locations->second.get_root();
    if (root.empty())
    {
		root = server_conf.get_root();
        if (root.empty())
            root = "public";
    }
	if (root.back() == '/')
		root.pop_back();
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
        if (root.back() != '/')
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
                if ((locations->second).get_autoindex() == on)
                    throw HTTPStatus<501>(); // Not implemented yet
                throw HTTPStatus<403>();
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
    this->update_event(listener_fd, EVFILT_READ, EV_DELETE);
    delete _cluster[listener_fd];
    _connections.erase(listener_fd);
}

void	Server::close_connection( int connection_fd )
{
    std::cout << "Closing Connection for client: " << connection_fd << std::endl;
    this->update_event(connection_fd, EVFILT_READ, EV_DELETE);
    this->update_event(connection_fd, EVFILT_WRITE, EV_DELETE);
    delete _connections[connection_fd];
    _connections.erase(connection_fd);
}
