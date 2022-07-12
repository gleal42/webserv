/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 22:19:09 by gleal             #+#    #+#             */
/*   Updated: 2022/07/12 15:26:07 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LISTENER_H__
# define __LISTENER_H__

# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <sys/event.h>
# include <map>

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
class	Server;
class	Listener;
typedef std::map<int, Listener*> Cluster;
typedef Cluster::iterator ClusterIter;

typedef std::map< int, Socket * > Connections;
typedef Connections::iterator ConnectionsIter;
typedef std::pair< int, Socket * > Connection;

class Listener {

public:
	Listener( Listener const & src );
	Listener( ServerConfig const & config );
	~Listener( void );
	Listener &	operator = ( Listener const & rhs );

	// Listener Starts listening on creation
	void		accept_client( Server &kq );
	void		stop( void );
	void		shutdown( void );
	int			fd();
	Socket *	socket();
	static bool	has_connections(Cluster cluster);
	Connections		_connections;
	ServerConfig	_config;
private:

	Listener( void );
	Socket *		_socket;
	size_t			_max_connections;
};

#endif /* __LISTENER_H__ */
