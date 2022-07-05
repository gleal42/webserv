/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:30:33 by gleal             #+#    #+#             */
/*   Updated: 2022/07/05 09:49:01 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* Constructors */
Server::Server( void ) : _socket(NULL) { /* no-op */ }
Server::Server( Server const & src ) : _socket(NULL) { *this = src; }

Server::Server( ServerConfig const & config ) : _config(config), _socket(NULL)
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
	// begin an endless loop that waits on clients. We call wait_on_clients()
	// to wait until a new client connects or an old client sends new data:
	while(1) {
		fd_set	reads;
		reads = wait_on_clients();

		// The server then detects whether a new client has connected. This case
		// is indicated by server being set in fd_set reads. We use the FD_ISSET()
		// macro to detect this condition:
		if (FD_ISSET(_socket->fd(), &reads)
			&& _connections.size() < _max_connections) {

			// Once a new client connection has been detected,
			// creates a new connection.
			//
			// The accept() socket function is used to accept the new connection.
			// The new socket returned by accept() is stored in connections.
			Socket *	connection = _socket->accept(_config.input_buffer_size);
			if (!connection)
				return ; // catch socket error here
			_connections[connection->fd()] = connection;
		}

		// Our server must then handle the case where an already connected client
		// is sending data. We first walk through the map of clients and use
		// FD_ISSET() on each client to determine which clients have data available.
		for (ConnectionsIter it = _connections.begin(); it != _connections.end(); it++)
		{
			if (FD_ISSET(it->first, &reads)) {

				// We then check that we have memory available to store more
				// received data for client. If the client's buffer is already
				// completely full, then we send a 400 error.
				if (_config.input_buffer_size == it->second->bytes()) {
					throw HTTPStatus<400>();
				}

				// Knowing that we have at least some memory left to store received data,
				// we can use recv() to store the client's data.
				try {
					it->second->receive();
				}
				catch(const std::exception& e) { // catch SocketError::Receive
					ERROR(e.what());
					drop_client(it->second);
				}

				// If the received data was written successfully, our server adds a null
				// terminating character to the end of that client's data buffer.
				// This allows us to use string find() to search the buffer, as the null
				// terminator tells find() when to stop.
				//
				// Recall that the HTTP header and body is delineated by a blank line.
				// Therefore, if find() finds a blank line (\r\n\r\n, or CRLF x2), we
				// know that the HTTP header has been received and we can begin to service it.
				it->second->_buffer[it->second->bytes()] = 0;

				std::string		raw_request(it->second->to_s());
				std::string		blank_line(std::string(CRLF) + std::string(CRLF));

				if (raw_request.find(blank_line) != std::string::npos) {

					// Temporary
					// Our server only handles GET requests. We also enforce that any
					// valid path should start with a slash character; strncmp() is
					// used to detect these two conditions in the following code:
					if (raw_request.substr(0, 5) != "GET /") {
						throw HTTPStatus<400>();
					}

					run(*it->second);
				} //if (raw_request.find
			} //if (FD_ISSET
		} //for (ConnectionsIter
	} //while(1)
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
	if (req._raw_header != "") {
		res.send_response(socket);
		drop_client(&socket);
	}
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
void	Server::drop_client( Socket * client )
{
	client->close();
	_connections.erase(client->fd()); // maybe log if not found for some reason
	delete client;
}

// Our server is capable of handling many simultaneous connections. This means
// that our server must have a way to wait for data from multiple clients at once.
// We define a function, wait_on_clients(), which blocks until an existing client
// sends data, or a new client attempts to connect. This function uses select()
fd_set	Server::wait_on_clients( void )
{
	fd_set	reads;
	FD_ZERO(&reads);
	FD_SET(_socket->fd(), &reads);
	int	max_socket = _socket->fd();

	for (ConnectionsIter it = _connections.begin(); it != _connections.end(); it++)
	{
		FD_SET(it->first, &reads);
		if (it->first > max_socket)
			max_socket = it->first;
	}

	if (::select(max_socket + 1, &reads, 0, 0, 0) < 0)
	{
		// Temporary
		ERROR("select() failed. " + std::string(strerror(errno)));
		exit(1);
	}

	return reads;
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
