/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:10:11 by msousa            #+#    #+#             */
/*   Updated: 2022/06/20 18:38:09 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SOCKET_H__
# define __SOCKET_H__

# include <iostream>
# include <sys/socket.h> // For socket functions
# include <netinet/in.h> // For sockaddr_in

# include "macros.hpp"

// ************************************************************************** //
//                               Socket Class                             	  //
// ************************************************************************** //

/*
A class to encapsulate the Socket C methods, turning it into a OOP piece

Needs to be able to:

	- Create the socket
	- Identify the socket
	- On the server, wait for an incoming connection
	- Send and receive messages
	- Close the socket

Implement wrappers for the following C functions

- int socket(domain, type, protocol);
- int bind(int socket, const struct sockaddr *address, socklen_t address_len);
- int listen(int socket, int backlog);
- int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict
	address_len);
- read / write should be send / recv

*/



class Socket {

public:

	struct Error : public std::exception {
		virtual const char*	what( void ) const throw() = 0;
	};

	struct CreateError : public Error {
		virtual const char*	what( void ) const throw();
	};

	Socket( void ); // address, port
	Socket( Socket const & src );
	~Socket( void );
	Socket &	operator = ( Socket const & rhs );

	int		fd( void );

private:

	// Should be private to avoid being set to a wrong value
	int		_fd;

};

std::ostream &	operator << ( std::ostream & o, Socket const & i );

#endif /* __SOCKET_H__ */
