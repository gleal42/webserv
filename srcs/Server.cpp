/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:30:33 by gleal             #+#    #+#             */
/*   Updated: 2022/06/19 20:08:28 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::init_addr()
{
    _address.sin_family = AF_INET;
    _address.sin_port = htons(_config.getPort());
    _address.sin_addr.s_addr = INADDR_ANY;
    memset(_address.sin_zero, '\0', sizeof(_address.sin_zero));
}

Server::Server()
{
}

Server::Server(const ServerConfig &config)
: _config(config)
{
    init_addr();
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == 0)
    {
        ERROR("In socket\n");
        exit(EXIT_FAILURE);
    }
    // // Setting socket to NONBLOCKING before binding
    // if (fcntl(fd, F_SETFL, O_NONBLOCK) < 1)
    // {
    //     perror("Failed because");
    //     ERROR("In socket flags\n");
    //     exit(EXIT_FAILURE);
    // }
    if (bind(fd, (struct sockaddr *)&_address, sizeof(_address))<0)
    {
        ERROR("In bind\n");
        exit(EXIT_FAILURE);
    }
    if (listen(fd, 128) < 0)
    {
        ERROR("In listen\n");
        exit(EXIT_FAILURE);
    }
	Socket listener;
	listeners.push_back(listener);
	listeners.begin()->fd = fd;
}

Server::~Server()
{
	std::vector<Socket>::iterator n = listeners.end();
    for (std::vector<Socket>::iterator it = listeners.begin(); it != n; it++)
	{
		close((*it).fd);
	}
}

Server::SocketAddress	&Server::getAddress()
{
    return (_address);
}

std::vector<Socket>	&Server::getListeners()
{
    return (listeners);
}
