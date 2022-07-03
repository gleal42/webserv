/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:30:33 by gleal             #+#    #+#             */
/*   Updated: 2022/07/03 18:43:23 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Kqueue.hpp"

/* Constructors */

Server::Server( void ) : _socket(NULL){ /* no-op */ }
Server::Server( Server const & src ): _socket(NULL) { *this = src; }


Server::Server(const ServerConfig &config)
: _config(config), _socket(NULL)
{
	_socket = new Socket(config);
	_socket->listen(config.max_clients);
	_max_connections = config.max_clients;
}

/* Destructor */
Server::~Server( void ){ if (_socket) delete _socket; }

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

void	Server::accept_client( Kqueue &kq )
{
	if (_connections.size() < _max_connections) {
		Socket *	connection = _socket->accept();
		if (!connection)
			return ; // add error here
		connection->set_parent(this->_socket);
		int client_fd = connection->fd();
		_connections[client_fd] = connection;
		std::cout << "CLIENT NEW: (" << client_fd << ")" << std::endl;
		kq.update_event(client_fd, EVFILT_READ, EV_ADD | EV_ENABLE);
		kq.update_event(client_fd, EVFILT_WRITE, EV_ADD | EV_DISABLE); // Will be used later in case we can't send the whole message
	} else {
		stop();
	}
}

// Stops accepting connections
void	Server::stop( void )
{
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

void    Server::init_addr()
{
    memset(&_address, 0, sizeof(struct sockaddr_in));
    _address.sin_family = AF_INET;
    _address.sin_port = htons(_config.port);
    _address.sin_addr.s_addr = INADDR_ANY;
}


SocketAddress	&Server::getAddress()
{
    return (_address);
}

int Server::fd()
{
    return (_socket->fd());
}
