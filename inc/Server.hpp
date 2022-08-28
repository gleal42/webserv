/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 00:20:17 by msousa            #+#    #+#             */
/*   Updated: 2022/08/27 14:59:10 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_H__
# define __SERVER_H__

#include <stdexcept>

#include "Event.hpp"
#include "Listener.hpp"
#include "Connection.hpp"
#include "ConfigParser.hpp"
#include "FileHandler.hpp"

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

*/

# define EVENTS_SIZE 10

class Server {

public:

	struct CreateError : public std::runtime_error {
		CreateError( void );
	};

	Server(const ConfigParser &parser);
	~Server( void );

	// Getters
	int				queue_fd() const;
	Connections		connections( void ) const;
	Listeners		listeners( void ) const;
	size_t			listeners_amount( void ) const;

	// TODO: check if can be private
	void			start( void );
	void			service(Request & req, Response & res);
	int				events_wait();

	/* Connection */
	void			connection_new( Listener * listener );
	void			connection_read( Connection *connection , int read_size );
	void			connection_write( Connection *connection );
	void			connection_close( int connection_fd );
	void			connection_event_toggle_write( int connection_fd );
	void			connection_event_toggle_read( int connection_fd );

	/* Listener */
	void			listener_close( int listener_fd );
	void			listener_event_read_add( int listener_fd );


   	EVENT 			events[EVENTS_SIZE];

private:

	Server( void );
	Server( Server const & src );
	Server &		operator=( Server const & src );
	int				_queue_fd;
	Connections		_connections;
	Listeners		_listeners;
	size_t			_listeners_amount;

};

#endif
