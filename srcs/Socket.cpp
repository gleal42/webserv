/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:31:55 by msousa            #+#    #+#             */
/*   Updated: 2022/06/20 20:44:37 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

/* Exceptions */
Socket::BindError::BindError( void ) : std::runtime_error("") { /* No-op */ }

Socket::CreateError::CreateError( void )
	: std::runtime_error("Failed to create socket.") { /* No-op */ }

Socket::BindError::BindError( int port )
	: std::runtime_error("Failed to bind to port " + std::to_string(port) + ".")
	{ /* No-op */ }

/* Constructors */
Socket::Socket( void ) { _socket(); }

// TODO: will we also pass `domain`?
Socket::Socket( int port )
{
	_socket();
	bind(port);
}

Socket::Socket( Socket const & src ) { *this = src; }

/* Destructor */
Socket::~Socket( void )
{
	// close()
}

/* Assignment operator */
Socket &	Socket::operator = ( Socket const & rhs )
{
	if (this != &rhs) {
		_fd = rhs._fd;
	}
	return *this;
}

int	Socket::fd( void ) { return _fd; }
int	Socket::port( void ) { return _port; }

// C `socket` function wrapper
void	Socket::_socket( void )
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);
  	if (_fd < 0) {
		throw Socket::CreateError();
	}
}

// C `bind` function wrapper
void	Socket::bind( int port )
{
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = htonl(INADDR_ANY);
	_address.sin_port = htons(port);
	if (::bind(_fd, (SocketAddress *)&_address, sizeof(_address)) < 0) {
		throw Socket::BindError(port);
	}
	_port = port;	// only set port if did't fail `bind` call
}

/* ostream override */
std::ostream &	operator << ( std::ostream & o, Socket const & i )
{
	(void)i;
	o << "Socket";
	return o;
}
