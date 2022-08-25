/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 00:20:17 by msousa            #+#    #+#             */
/*   Updated: 2022/08/25 17:35:30 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_H__
# define __SERVER_H__

#include <stdexcept>
#include "Listener.hpp"
#include "Connection.hpp"
#include "ConfigParser.hpp"

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
	Listeners		listeners( void ) const;
	Connections		connections( void ) const;
	size_t			listeners_amount( void ) const;

	// TODO: check if can be private
	void			start( void );
	int				wait_for_events();
	void			event_update(int ident, short filter, u_short flags);
	void			new_connection( Listener * listener );
	void			read_connection( Connection *connection , EVENT const & event );
	void			write_to_connection( Connection *connection );
	void			service(Request & req, Response & res);
	void			close_connection( int connection_fd );
	void			close_listener( int listener_fd );
   	EVENT 			events[EVENTS_SIZE];

private:

	Server( void );
	Server( Server const & src );
	Server &	operator=( Server const & src );
	int				_queue_fd;
	Listeners		_listeners;
	Connections		_connections;
	size_t			_listeners_amount;

};

#endif
