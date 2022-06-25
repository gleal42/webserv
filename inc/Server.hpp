/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:05:52 by gleal             #+#    #+#             */
/*   Updated: 2022/06/25 18:29:10 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/event.h>

#include "macros.hpp"
# include "ServerConfig.hpp"
# include "Response.hpp"
# include "Request.hpp"
# include "Socket.hpp"

// Apagar depois
#include <errno.h>


// https://en.wikipedia.org/wiki/List_of_HTTP_header_fields

// Server will have multiple requests and responses so maybe a vector is more adequate

typedef sockaddr_in SocketAddress;

class Server
{
	private:
		Socket			_socket;
		Socket			_connection;
		ServerConfig	_config;
		SocketAddress	_address;
		Server();
		void	init_addr();
	public:
		Server(const ServerConfig &config);
		Server(const Server &server);
		~Server();
		Server &operator=(const Server &server);
		SocketAddress	&getAddress();
		int				getFd();
		void		run(int kq);
		void		shutdown( void );
		int	accept_client( int kq );
};

#endif
