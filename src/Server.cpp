/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 09:45:56 by msousa            #+#    #+#             */
/*   Updated: 2022/08/26 16:38:28 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
    Testes a passar:
    Display big images ()
    Várias Requests ao mesmo tempo
    EOF working
    HTML CSS priority
    Javascript (later)
 */

Server::CreateError::CreateError( void )
: std::runtime_error("Failed to create Kernel Queue.") { /* No-op */ }

// private
Server::Server() { throw std::runtime_error("Please use non-default constructor"); }

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

void Server::listener_event_read_add(int listener_fd)
{
    EVENT event;

#if defined(DARWIN)
	EV_SET(&event, listener_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	kevent(_queue_fd, &event, 1, NULL, 0, NULL);
#endif

#if defined(LINUX)
	event.events = EPOLLIN;
	epoll_ctl(_queue_fd, EPOLL_CTL_ADD, listener_fd, &event);
#endif
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
            Listener_it		it = _listeners.find(events[i].ident); // <--------

            if (it != _listeners.end()) {
				// New event for non-existent file descriptor
                connection_new(it->second); // <--------
			}
			else {
				Connections_it	connection_it = _connections.find(events[i].ident); // <--------

				if (events[i].flags & EV_EOF) { // <--------
					// If there are no more connections open in any server do cleanup(return)
                    connection_close(events[i].ident); // <--------

                    if (_connections.size() == 0) {
                        return ;
					}
                }
				else if (events[i].filter == EVFILT_READ) { // <--------
                    connection_read(connection_it->second, events[i]); // <--------
				}
				else if (events[i].filter == EVFILT_WRITE) { // <--------
					connection_write(connection_it->second);

                    if (connection_it->second->response.is_empty()) {
                        connection_it->second->request.clear();
					}
                }
            }
        }
    }
}

int	Server::events_wait( void )
{
	LOG("\n+++++++ Waiting for new connection ++++++++\n");

	int					events_ready;

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
	Connection *	connection = new Connection(listener->socket());
	int 			connection_fd = connection->fd();

	_connections[connection_fd] = connection;

	LOG("connection NEW: (" << connection_fd << ")");

	event_update(connection_fd, EVFILT_READ, EV_ADD | EV_ENABLE); // <--------
	// Will be used later in case we can't send the whole message
	event_update(connection_fd, EVFILT_WRITE, EV_ADD | EV_DISABLE); // <--------
}


// int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
// EPOLL_CTL_ADD, EPOLL_CTL_DEL, EPOLL_CTL_MOD
void Server::event_update(int fd, short filter, u_short flags)
{
    EVENT event;
	EV_SET(&event, fd, filter, flags, 0, 0, NULL);
	kevent(_queue_fd, &event, 1, NULL, 0, NULL);
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
void	Server::connection_read( Connection *connection, EVENT const & event )
{
    LOG("About to read the file descriptor: " << connection->fd());
    LOG("Incoming data has size of: " << event.data);

    connection->request.parse(*connection->socket(), event); // <--------

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

    event_update(connection->fd(), EVFILT_READ, EV_DISABLE); // <--------
    event_update(connection->fd(), EVFILT_WRITE, EV_ENABLE); // <--------
}

void	Server::connection_write( Connection *connection )
{
	LOG("About to write to file descriptor: " << connection->fd());
	LOG("The socket has the following size to write " << events[0].data); // <--------

    if (connection->response.is_empty()) {
        service(connection->request, connection->response);
	}

    connection->response.send_response(*connection->socket());

    if (connection->response.is_empty()) {
        LOG("Connection was empty after sending");

        event_update(connection->fd(), EVFILT_READ, EV_ENABLE); // <--------
        event_update(connection->fd(), EVFILT_WRITE, EV_DISABLE); // <--------
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
    FileHandler	handler;
    try {
        handler.service(req, res);
    }
	catch (BaseStatus &error_status) {
        res.set_error_body(error_status.code);
        res.build_message(error_status);
    }
}

Server::~Server()
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

    event_update(listener_fd, EVFILT_READ, EV_DELETE); // <--------

    delete _listeners[listener_fd];
	// Weird! When I change this to _listeners.erase(listener_fd),
	// which should be the correct one afaik
	// I get a segfault
    _connections.erase(listener_fd);
}

void	Server::connection_close( int connection_fd )
{
    LOG("Closing Connection: " << connection_fd);

    event_update(connection_fd, EVFILT_READ, EV_DELETE); // <--------
    event_update(connection_fd, EVFILT_WRITE, EV_DELETE); // <--------

    delete _connections[connection_fd];
    _connections.erase(connection_fd);
}
