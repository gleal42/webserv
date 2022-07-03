/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:05:52 by gleal             #+#    #+#             */
/*   Updated: 2022/07/04 00:05:24 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_H__
# define __SERVER_H__

# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <sys/event.h>
# include <map>

// Apagar depois
# include <errno.h>

# include "ServerConfig.hpp"
# include "Response.hpp"

typedef sockaddr_in SocketAddress;

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
class	Kqueue;
class	Server;
typedef std::map<int, Server*> Cluster;
typedef Cluster::iterator ClusterIter;

typedef std::map< int, Socket * > Connections;
typedef Connections::iterator ConnectionsIter;
typedef std::pair< int, Socket * > Connection;

class Server {

public:
	Server( Server const & src );
	Server( ServerConfig const & config );
	~Server( void );
	Server &	operator = ( Server const & rhs );

	// Server Starts listening on creation
	void		accept_client( Kqueue &kq );
	void		stop( void );
	void		shutdown( void );
	int			fd();
	Socket *	socket();
	static bool	has_connections(Cluster cluster);
	Connections		_connections;

private:

	Server( void );
	ServerConfig	_config;
	Socket *		_socket;
	size_t			_max_connections;
};

#endif /* __SERVER_H__ */
