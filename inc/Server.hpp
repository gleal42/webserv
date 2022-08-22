/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 00:20:17 by msousa            #+#    #+#             */
/*   Updated: 2022/08/22 01:49:19 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_H__
# define __SERVER_H__

#include <stdexcept>
// #include <sys/event.h>
#include <sys/epoll.h>
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

class Server {

public:

	struct CreateError : public std::runtime_error {
						CreateError( void );
	};
	Server(const ConfigParser &parser);
	~Server();
	// Getters
	int					fd() const;
	// Member Functions
	// TODO: check if can be private
	void				start( void );
	int					wait_for_events();
	void				new_connection( Listener * listener );
	void				read_connection( Connection *connection , struct epoll_event const & Event );
	void				write_to_connection( Connection *connection );
	void				service(Request & req, Response & res);

	ServerConfig		find_config_to_use(const Request & req);
	Locations::iterator	find_location_to_use(ServerConfig &server_block, const std::string & path);
	void				resolve_path(std::string & path, const ServerConfig & server_conf, Locations::iterator location_conf, bool &autoindex_confirmed);
	void				do_autoindex(std::string & path,Response & res);
	void				close_connection( int connection_fd );
	void				close_listener( int listener_fd );

	void				add_event(int ident, uint32_t events);
	void				switch_event_to(int ident, uint32_t events);
	void				delete_event(int ident);

	struct epoll_event 	ListQueue[10];

private:

	Server( void );
	Server( Server const & src );
	Server &operator=( Server const & src );
	int					_fd;
	Cluster				_cluster;
	Connections			_connections;
	size_t				_listeners_amount;

};

#endif
