/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:10:11 by msousa            #+#    #+#             */
/*   Updated: 2022/08/31 22:11:58 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SOCKET_H__
# define __SOCKET_H__

# include <sys/socket.h>
# include <unistd.h>
# include <fcntl.h>
# include <cstring>
# include <cerrno>

# include "macros.hpp"
# include "ServerConfig.hpp"
# include "webserver.hpp"
# include "types.hpp"

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

class Socket {

public:

	struct CreateError : public std::runtime_error {
		CreateError( void );
	};
	struct ReusableAddressError : public std::runtime_error {
		ReusableAddressError( void );
	};
	struct ReusablePortError : public std::runtime_error {
		ReusablePortError( void );
	};
	struct BindError : public std::runtime_error {
		BindError( void );
		BindError( int port );
	};
	struct ListenError : public std::runtime_error {
		ListenError( void );
	};
	struct AcceptError : public std::runtime_error {
		AcceptError( void );
	};

	Socket( void );
	Socket( ServerConfig config );
	Socket( Socket const & src );
	~Socket( void );
	Socket &		operator = ( Socket const & rhs );

	// Getters
	int				fd( void ) const;
	int				port( void ) const;
	int				bytes( void ) const;

	// Setters
	void			set_fd( int fd );

	void 			create( void );
	void 			setsockopt( int option );
	void			bind( int port );
	void			close( void );
	void			listen( int max_connections );
	int				send( const std::string & response ) const;
	void			receive( int buffer_size );
	Socket *		accept( void );
	std::string		to_s( void ) const;
	std::vector<char>	_buffer;

private:

	// Should be private to avoid being set to a wrong value
	int					_port;
	int					_fd;
	SocketAddress		_address;
	int					_bytes;

};

std::ostream &	operator << ( std::ostream & o, Socket const & i );

#endif /* __SOCKET_H__ */
