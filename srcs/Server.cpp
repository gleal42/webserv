/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:30:33 by gleal             #+#    #+#             */
/*   Updated: 2022/06/24 17:22:03 by msousa           ###   ########.fr       */
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
		//value = rhs.value;
	}
	return *this;
}

// Starts accepting connections
void	Server::start( void )
{
	int	temp_fd; // to make it work until we have a `select` mechanism

	// check if can still add
	if (_connections.size() < _max_connections) {
		Socket *	connection = _socket->accept();
		_connections.insert(Connection(connection->fd(), connection));
		// temp
		temp_fd = connection->fd();
	} else {
		stop();
	}

	// code to select which connection to run
	run(*_connections[temp_fd]);
}

// Does necessary to service a connection
void	Server::run(Socket & socket) {
	Request 	req(_config);
	Response 	res(_config);
	try {
		// while timeout and Running
		socket.receive(_config.input_buffer_size);
		req.parse(socket);
		res.request_method = req.request_method;
		// res.request_uri = req.request_uri;
		// if (request_callback) {
		// 	request_callback(req, res);
		// }
		service(req, res);
	}
	catch (std::exception error) {
		ERROR(error.what());
	// catch (HTTPStatus error) {
		// res.set_error(error);
		// if (error.code) {
		// 	res.status = error.code;
		// }
	}
	if (req.request_line != "") {
		res.send_response(socket);
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

	(void)req;
	(void)res;
	// handler.service(req, res);
}

// Stops accepting connections
void	Server::stop( void )
{
	// TODO:
}

void	Server::shutdown( void )
{
	// TODO:
}
