/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:05:52 by gleal             #+#    #+#             */
/*   Updated: 2022/06/07 22:42:46 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#include "macros.hpp"
# include "ServerConf.hpp"

class Server{
private:
	ServerConf conf;
	struct sockaddr_in address;
	int fd;
    Server();
	void	init_addr();
public:
    Server(const ServerConf &config);
    Server(const Server &server);
    ~Server();
    Server &operator=(const Server &server);
	int		rcv_msg(void) const;
};

#endif
