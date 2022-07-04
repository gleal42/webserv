/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:30:33 by gleal             #+#    #+#             */
/*   Updated: 2022/07/04 22:07:14 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Socket.hpp"
#include "Kqueue.hpp"

/* Constructors */

Server::Server( void ) : _socket(NULL) { /* no-op */ }
Server::Server( Server const & src ) : _socket(NULL) { *this = src; }


Server::Server( ServerConfig const & config ) : _config(config), _socket(NULL)
{
	// set some variables from the config in initialization list aswell
	// or here if they need treating first
	try {
		_socket = new Socket(config);
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

// Accepts connections
void	Server::accept_client( Kqueue &kq )
{
	// check if can still add
	if (_connections.size() < _max_connections) {
		Socket *	connection = _socket->accept();
		if (!connection)
			return ; // add error here
		connection->set_parent(this);
		int client_fd = connection->fd();
		_connections[client_fd] = connection;
		std::cout << "CLIENT NEW: (" << client_fd << ")" << std::endl;
		kq.update_event(client_fd, EVFILT_READ, EV_ADD | EV_ENABLE);
		kq.update_event(client_fd, EVFILT_WRITE, EV_ADD | EV_DISABLE); // Will be used later in case we can't send the whole message
	} else {
		stop();
	}
}
// Previous run and service logic changed to webserver.cpp and Response.cpp files


// Stops accepting connections
void	Server::stop( void )
{
	// TODO: Add Error page exception here?
	std::cout << "Max connections have been reached." << std::endl;
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

int Server::fd()
{
    return (_socket->fd());
}

Socket *	Server::socket()
{
    return (_socket);
}