/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:10:11 by msousa            #+#    #+#             */
/*   Updated: 2022/06/22 21:34:07 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SOCKET_H__
# define __SOCKET_H__

# include <iostream>
# include <vector>
# include <sys/socket.h> // For socket functions
# include <netinet/in.h> // For sockaddr_in
# include <unistd.h> // For close()
# include <fcntl.h>

# include "macros.hpp"

# define PORT_UNSET -1
# define FD_UNSET -1

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

Implements wrappers for the following C functions

- int socket(domain, type, protocol);
- int bind(int socket, const struct sockaddr *address, socklen_t address_len);
- int listen(int socket, int backlog);
- int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict
	address_len);
- read / write should be send / recv (C++)

*/

typedef struct sockaddr_in 	SocketAddress;

class Socket {

public:

	struct CreateError : public std::runtime_error {
		CreateError( void );
	};
	struct BindError : public std::runtime_error {
		BindError( void );
		BindError( int port );
	};
	struct ListenError : public std::runtime_error {
		ListenError( void );
	};

	Socket( void );
	Socket( int port );
	Socket( Socket const & src );
	~Socket( void );
	Socket &		operator = ( Socket const & rhs );

	// Getters
	int				fd( void ) const;
	int				port( void ) const;
	// Setters
	void			set_fd( int fd );

	void 			create( void );
	void			bind( int port );
	void			close( void );
	void			listen( int max_connections );
	void			send( const std::string & response );
	int				receive( int buffer_size );
	Socket *		accept( void );
	std::string		to_s( void ) const;

private:

	// Should be private to avoid being set to a wrong value
	int					_port;
	int					_fd;
	SocketAddress		_address;
	std::vector<char>	_buffer;

};

std::ostream &	operator << ( std::ostream & o, Socket const & i );

#endif /* __SOCKET_H__ */
