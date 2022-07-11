/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:30:33 by gleal             #+#    #+#             */
/*   Updated: 2022/07/11 16:27:29 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Kqueue.hpp"

/* Constructors */
Server::Server( void ) : _socket(NULL) { /* no-op */ }
Server::Server( Server const & src ) : _socket(NULL) { *this = src; }

Server::Server( ServerConfig const & config, int kq ) : _config(config), _kq(kq) , _socket(NULL)
{
	
	// set some variables from the config in initialization list aswell
	// or here if they need treating first
	try {
		_socket = new Socket(config.port);
	}
	// TODO: do specific things
	catch(Socket::CreateError& e) { // or std::runtime_error
		// stop(); // shutdown();
		LOG(e.what());
	}
	catch(Socket::BindError& e) {
		// stop(); // shutdown();
		LOG(e.what());
	}

	// there should always be a socket at this point,
	// the catches above should stop flow
	// maybe move them to main Server initialization in `webserver()`
	try {
		_socket->listen(config.max_clients);
	}
	catch(Socket::ListenError& e) {
		// stop(); // shutdown();
		LOG(e.what());
	}
	_max_connections = config.max_clients;
	add_event(_kq, _socket->fd(), EVFILT_READ);
}

/* Destructor */
Server::~Server( void ) { if (_socket) delete _socket; }

/* Assignment operator */
Server &	Server::operator = ( Server const & rhs )
{
	if (this != &rhs) {
		_config = rhs._config;
		_socket = new Socket(*rhs._socket);
		_connections = rhs._connections;
		_max_connections = rhs._max_connections;
	}
	return *this;
}

// Starts accepting connections
void	Server::start( void )
{
	int	nbr_events = wait_on_clients();
	PRINT("NUMBER EVENTS: " << nbr_events);
	if (nbr_events <= 0)
		return ;
	for (size_t i = 0; i < (size_t)nbr_events; i++)
	{
		if (_ListQueue[i].ident == (size_t)this->_socket->fd())
		{
			Socket *	connection = _socket->accept(_config.input_buffer_size);
			if (!connection)
				return ; // catch socket error here
			add_event(_kq, connection->fd(), EVFILT_READ);
			enable_event(_kq, connection->fd(), EVFILT_READ);
			add_event(_kq, connection->fd(), EVFILT_WRITE);
			disable_event(_kq, connection->fd(), EVFILT_WRITE);
			_connections[connection->fd()] = connection;
			PRINT( "New connection fd " << connection->fd() );
		}
		else
		{
			for (ConnectionsIter it = _connections.begin(); it != _connections.end(); ++it)
			{
				if (_ListQueue[i].ident == (size_t)it->first)
				{
					PRINT("New event for non-server fd " << _ListQueue[i].ident);
					if (_config.input_buffer_size == it->second->bytes()) {
						throw HTTPStatus<400>();
					}
					if (_ListQueue[i].flags & EV_EOF)
					{
							PRINT ("Closing Connection for client: " << _ListQueue[i].ident);
							delete_event(_kq, it->first, EVFILT_READ);
							delete_event(_kq, it->first, EVFILT_WRITE);
							drop_client(it);
							if (it == _connections.end())
								break ;
					}
					else if (_ListQueue[i].filter == EVFILT_READ)
					{
						PRINT("About to read from fd " << _ListQueue[i].ident);
						try {
							it->second->receive();
						}
						catch(const std::exception& e) { // catch SocketError::Receive
							ERROR(e.what());
							drop_client(it);
						}
						disable_event(_kq, it->first, EVFILT_READ);
						enable_event(_kq, it->first, EVFILT_WRITE);
					}
					else if (_ListQueue[i].filter == EVFILT_WRITE)
					{
						PRINT("About to write on fd " << _ListQueue[i].ident);
						it->second->_buffer[it->second->bytes()] = 0;

						std::string		raw_request(it->second->to_s());
						std::string		blank_line(std::string(CRLF) + std::string(CRLF));

						if (raw_request.find(blank_line) != std::string::npos) {

							if (raw_request.substr(0, 5) != "GET /") {
								throw HTTPStatus<400>();
							}
							// check why eof with writing
							run(*it->second);
						} //if (raw_request.find
						enable_event(_kq, it->first, EVFILT_READ);
						disable_event(_kq, it->first, EVFILT_WRITE);
					} // else if (_ListQueue->filter
				} // if (_ListQueue->ident
			} // for(ConnectionsIter
		}
	} //for (size_t i = 0
}

// Does necessary to service a connection
void	Server::run(Socket & socket) {
	Request 	req(_config);
	Response 	res(_config);
	try {
		// while timeout and Running
		req.parse(socket);
		res.request_method = req.request_method;
		// res.request_uri = req.request_uri;
		// if (request_callback) {
		// 	request_callback(req, res);
		// }
		service(req, res);
	}
	catch (BaseStatus & error) {
		ERROR(error.what());
		// res.set_error(error);
		if (error.code) {
			// res.status = error.code;
		}
	}
	// if (req.request_line != "") {
	// 	res.send_response(socket);
	// }

	// Temporary
	res.send_response(socket);
}

// Services +req+ and fills in +res+
void	Server::service(Request & req, Response & res) {
	// Use factory create() method on Handler interface / Abstract class
	// Pick handler::service based on available info
	// ..
	// req.script_name = script_name;
	// req.path_info = path_info;

	// FileHandler 	handler;
	// OR
	// CGIHandler 	handler;
	FileHandler handler; // probably needs config for root path etc

	handler.service(req, res);
}

// Stops accepting connections
void	Server::stop( void )
{
	// TODO:
}

void	Server::shutdown( void )
{
	_socket->close();
	for (ConnectionsIter it = _connections.begin(); it != _connections.end(); it++)
	{
		it->second->close();
		delete it->second;
	}
}

// Our get_client() function accepts a Socket and searches through the
// map of connected clients to return the relevant Socket. If no matching
// Socket is found in the map, then a NULL nullptr is returned
Socket *	Server::get_client( int fd )
{
	if (_connections.find(fd) == _connections.end()) {
		return NULL;
	}
	return _connections[fd];
}

// The drop_client() function searches through our map of clients and removes
// a given client.
void	Server::drop_client( ConnectionsIter & it )
{
	it->second->close();
	delete it->second;
	_connections.erase(it++); // maybe log if not found for some reason
}

// Our server is capable of handling many simultaneous connections. This means
// that our server must have a way to wait for data from multiple clients at once.
// We define a function, wait_on_clients(), which blocks until an existing client
// sends data, or a new client attempts to connect. This function uses select()
int	Server::wait_on_clients( void )
{
    // struct timespec kqTimeout = {2, 0};
	std::cout << "\n+++++++ Waiting on clients ++++++++\n" << std::endl;
    return (kevent(_kq, NULL, 0, _ListQueue, 10, NULL));
}
// In the preceding code, first a new fd_set is declared and zeroed-out. The server
// socket is then added to the fd_set first. Then the code loops through the
// map of connected clients and adds the socket for each one in turn.
// A variable, max_socket, is maintained throughout this process to store the
// maximum socket number as required by select().
//
// After all the sockets are added to fd_set reads, the code calls select(), and
// select() returns when one or more of the sockets in reads is ready.
//
// The wait_on_clients() function returns reads so that the caller can see which
// socket is ready.
