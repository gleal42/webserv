/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:30:33 by gleal             #+#    #+#             */
/*   Updated: 2022/06/17 23:13:28 by gleal            ###   ########.fr       */
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
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == 0)
    {
        ERROR("In socket\n");
        exit(EXIT_FAILURE);
    }
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

int	Server::getFd()
{
    return (_fd);
}