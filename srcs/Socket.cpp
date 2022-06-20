/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:31:55 by msousa            #+#    #+#             */
/*   Updated: 2022/06/20 18:38:29 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

/* Exceptions */
const char*	Socket::CreateError::what( void ) const throw()
{
	return "Failed to create socket.";
}

/* Constructors */
Socket::Socket( void )
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);
  	if (_fd == -1) {
		throw Socket::CreateError();
	}
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
		if (rhs._fd == -1) {
			throw Socket::CreateError();
		}
		_fd = rhs._fd;
	}
	return *this;
}

int	Socket::fd( void ) { return _fd; }

// Socket &	Socket::operator = ( Socket const & rhs )
// {
// 	if (rhs._fhs == -1) {
// 		throw Socket::CreateError();
// 	}
// 	if (this != &rhs) {
// 		fd = rhs.fd;
// 	}
// 	return *this;
// }

/* ostream override */
std::ostream &	operator << ( std::ostream & o, Socket const & i )
{
	(void)i;
	o << "Socket";
	return o;
}
