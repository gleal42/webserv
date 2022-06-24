/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:05:52 by gleal             #+#    #+#             */
/*   Updated: 2022/06/24 17:23:02 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_H__
# define __SERVER_H__

# include <iostream>

# include "ServerConfig.hpp"
# include "Socket.hpp"
# include "Request.hpp"
# include "Response.hpp"

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
	Socket *		_socket;
	Connections		_connections;
	size_t			_max_connections;

	void			run(Socket & socket);
	void			service(Request & req, Response & res);
};

#endif /* __SERVER_H__ */
