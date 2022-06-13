/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:05:52 by gleal             #+#    #+#             */
/*   Updated: 2022/06/13 17:44:11 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "macros.hpp"
# include "ServerConfig.hpp"

class Server{
private:
	ServerConfig _config;
	typedef sockaddr_in SocketAddress;
	SocketAddress _address;
	int _fd;
    Server();
	void	init_addr();
public:
    Server(const ServerConfig &config);
    Server(const Server &server);
    ~Server();
    Server &operator=(const Server &server);
	int		receive_message(void) const;
};

#endif
