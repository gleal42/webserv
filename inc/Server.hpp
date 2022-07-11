/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:05:52 by gleal             #+#    #+#             */
/*   Updated: 2022/07/11 19:23:44 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_H__
# define __SERVER_H__

# include <iostream>
# include <sys/event.h>

# include "ServerConfig.hpp"
# include "Socket.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "FileHandler.hpp"

// ************************************************************************** //
//                               Server Class                             	  //
// ************************************************************************** //

/*
A class to represent a single threaded HTTP Server

Server will have multiple accepted sockets so maybe a vector / map
is more adequate

Needs to be able to:

	- initialize a server socket for the given configuration
	- start (Starts accepting connections from clients)
	- stop (Stops the server from accepting new connections)
	- shutdown (Shuts down the server and all accepted clients)

*/

// https://en.wikipedia.org/wiki/List_of_HTTP_header_fields

struct 	ServerConfig;
class	Socket;

typedef std::map< int, Socket * > Connections;
typedef Connections::iterator ConnectionsIter;
typedef std::pair< int, Socket * > Connection;

class Server {

public:

	Server( void );
	Server( Server const & src );
	Server( ServerConfig const & config );
	~Server( void );
	Server &	operator = ( Server const & rhs );

	void		start( void );
	void		stop( void );
	void		shutdown( void );

private:

	ServerConfig	_config;
	int				_kq;
	Socket *		_socket;
	Connections		_connections;
	size_t			_max_connections;
	struct kevent 	_ListQueue[10];

	void			run(Socket & socket);
	void			service(Request & req, Response & res);

	Socket *		get_client( int fd );
	void			drop_client( ConnectionsIter & it );
	int				wait_on_clients( void );
};

#endif /* __SERVER_H__ */
