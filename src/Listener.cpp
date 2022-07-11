/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 22:16:37 by gleal             #+#    #+#             */
/*   Updated: 2022/07/11 22:17:18 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listener.hpp"
#include "Socket.hpp"
#include "Kqueue.hpp"

/* Constructors */

Listener::Listener( void ) : _socket(NULL) { /* no-op */ }
Listener::Listener( Listener const & src ) : _socket(NULL) { *this = src; }


Listener::Listener( ServerConfig const & config ) : _config(config), _socket(NULL)
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
	// maybe move them to main Listener initialization in `webserver()`
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
Listener::~Listener( void ) { if (_socket) delete _socket; }

/* Assignment operator */
Listener &	Listener::operator = ( Listener const & rhs )
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
void	Listener::accept_client( Kqueue &kq )
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
void	Listener::stop( void )
{
	// TODO: Add Error page exception here?
	std::cout << "Max connections have been reached." << std::endl;
}

void	Listener::shutdown( void )
{
	_socket->close();
	for (ConnectionsIter it = _connections.begin(); it != _connections.end(); it++)
	{
		it->second->close();
		delete it->second;
	}
}

int Listener::fd()
{
    return (_socket->fd());
}

Socket *	Listener::socket()
{
    return (_socket);
}