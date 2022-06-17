/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:31:55 by msousa            #+#    #+#             */
/*   Updated: 2022/06/15 19:27:44 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

/* Constructors */
Socket::Socket( void )
{
	fd = socket(AF_INET, SOCK_STREAM, 0);
  	if (fd == -1) {
		ERROR("Failed to create socket. errno: " << errno);
		exit(EXIT_FAILURE); // TODO: throw std::exception inheriting SocketError
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
		//value = rhs.value;
	}
	return *this;
}

/* ostream override */
std::ostream &	operator << ( std::ostream & o, Socket const & i )
{
	(void)i;
	o << "Socket";
	return o;
}
