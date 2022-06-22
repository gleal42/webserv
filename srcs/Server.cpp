/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:30:33 by gleal             #+#    #+#             */
/*   Updated: 2022/06/21 23:44:22 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::init_addr()
{
    memset(&_address, 0, sizeof(struct sockaddr_in));
    _address.sin_family = AF_INET;
    _address.sin_port = htons(_config.getPort());
    _address.sin_addr.s_addr = INADDR_ANY;
}

Server::Server()
{
}

Server::Server(const ServerConfig &config)
: _config(config)
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "server fd: " << _fd << std::endl;
    if (_fd == 0)
    {
        ERROR("In socket\n");
        exit(EXIT_FAILURE);
    }
    // Setting socket to NONBLOCKING before binding
    init_addr();
    if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
    {
        ERROR("In socket flags\n");
        exit(EXIT_FAILURE);
    }
    // int val = 1;
    // // Allowing the same port to be bound multiple times
	// if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) < 0)
    // {
    //     ERROR("In set socket options\n");
    //     exit(EXIT_FAILURE);
    // }
    if (bind(_fd, (struct sockaddr *)&_address, sizeof(_address))<0)
    {
        ERROR("In bind\n");
        exit(EXIT_FAILURE);
    }
    if (listen(_fd, 128) < 0)
    {
        ERROR("In listen\n");
        exit(EXIT_FAILURE);
    }
}

Server::~Server()
{
	close(_fd);
}

Server::SocketAddress	&Server::getAddress()
{
    return (_address);
}

int Server::getFd()
{
    return (_fd);
}
