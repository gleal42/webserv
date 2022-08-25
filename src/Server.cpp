/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 09:45:56 by msousa            #+#    #+#             */
/*   Updated: 2022/08/25 16:07:35 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "FileHandler.hpp"
#include <iostream>
#include <stdexcept>

/*
    Testes a passar:
    Display big images ()
    Vários Servers ao mesmo tempo
    Várias Requests ao mesmo tempo
    EOF working
    HTML CSS priority
    Javascript (later)
 */

Server::CreateError::CreateError( void )
: std::runtime_error("Failed to create Kernel Queue.") { /* No-op */ }

Server::Server() // private
{
    throw std::runtime_error("Please use non-default constructor");
}

// int epoll_create(int size);
//
// The size argument is an indication to the kernel about the number of file descriptors
// a process wants to monitor, which helps the kernel to decide the size of the epoll
// instance. Since Linux 2.6.8, this argument is ignored because the epoll data structure
// dynamically resizes as file descriptors are added or removed from it.

Server::Server(const ConfigParser &parser)
{
    _fd = kqueue();
    // _fd = epoll_create(size);
    if (_fd < 0)
        throw CreateError();
	_listeners_amount = parser.configs_amount();
	Listener		*new_listener;
	for (size_t i = 0; i < _listeners_amount; ++i)
    {
		// Initialize each new Listener with a config from the parser
		ServerConfig	config(parser.config(i));
		new_listener = new Listener(config);
		// EPOLLIN, EPOLLOUT, EPOLLET ?
		update_event(new_listener->fd(), EVFILT_READ, EV_ADD);
		_cluster[new_listener->fd()] = new_listener;
	}
}

// Getters
int	Server::fd( void ) const { return _fd; }
Cluster	Server::cluster( void ) const { return _cluster; }
Connections	Server::connections( void ) const { return _connections; }
size_t	Server::listeners_amount( void ) const { return _listeners_amount; }

// int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
// EPOLL_CTL_ADD, EPOLL_CTL_DEL, EPOLL_CTL_MOD
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
            Cluster_it event_fd = _cluster.find(ListQueue[i].ident);
            if (event_fd != _cluster.end()) // New event for non-existent file descriptor
                new_connection(event_fd->second);
            else
            {
				Connections_it connection_it = _connections.find(ListQueue[i].ident);
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
                        connection_it->second->request.clear();
                }
            }
        }
    }
}

// int epoll_wait(int epfd, struct epoll_event *evlist, int maxevents, int timeout);
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
	// Will be used later in case we can't send the whole message
	update_event(client_fd, EVFILT_WRITE, EV_ADD | EV_DISABLE);
}

// Reference
// Does necessary to service a connection
// void	Server::run(Socket & socket) {
// 	Request 	req(_config);
// 	Response 	res(_config);
// 	try {
// 		// while timeout and Running
// 		req.parse(socket);
// 		res.request_method = req.request_method;
// 		// res.request_uri = req.request_uri;
// 		// if (request_callback) {
// 		// 	request_callback(req, res);
// 		// }
// 		service(req, res);
// 	}
// 	catch (BaseStatus & error) {
// 		ERROR(error.what());
// 		// res.set_error(error);
// 		if (error.code) {
// 			// res.status = error.code;
// 		}
// 	}
// 	// if (req.request_line != "") {
// 	// 	res.send_response(socket);
// 	// }

// 	res.send_response(socket);
// }

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
	LOG("|--- Headers ---|");
	LOG(connection->request._raw_headers);
	LOG("|--- Headers ---|");
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

void	Server::service(Request & req, Response & res)
{
    FileHandler handler; // probably needs config for root path etc
    try {
        handler.service(req, res);
    } catch (BaseStatus &error_status)
    {
        res.set_error_body(error_status.code);
        res.build_message(error_status);
    }
}

Server::~Server()
{
	for (Cluster_it it = _cluster.begin(); it != _cluster.end(); ++it) {
        close_listener(it->first);
	}
	for (Connections_it it = _connections.begin(); it != _connections.end(); it++) {
        close_connection(it->first);
	}
    close(this->_fd);
}

void	Server::close_listener( int listener_fd )
{
    std::cout << "Closing Listener with fd: " << listener_fd << std::endl;
    this->update_event(listener_fd, EVFILT_READ, EV_DELETE);
    delete _cluster[listener_fd];
	// Weird! When I change this to _cluster.erase(listener_fd),
	// which should be the correct one afaik
	// I get a segfault
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
