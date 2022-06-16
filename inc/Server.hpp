/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:05:52 by gleal             #+#    #+#             */
/*   Updated: 2022/06/16 04:32:20 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>


#include "macros.hpp"
# include "ServerConfig.hpp"
# include "Response.hpp"
# include "Request.hpp"

// https://en.wikipedia.org/wiki/List_of_HTTP_header_fields

// Server will have multiple requests and responses so maybe a vector is more adequate

class Server
{
	private:
		typedef sockaddr_in SocketAddress;

		ServerConfig	_config;
		SocketAddress	_address;
		Request			_request;
		Response		_response;
		int _fd;

		Server();
		void	init_addr();
	public:
		Server(const ServerConfig &config);
		Server(const Server &server);
		~Server();
		Server &operator=(const Server &server);
		int		receive_message(void) const;
		Response	getResponse() const;
};

#endif
