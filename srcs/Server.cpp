/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:30:33 by gleal             #+#    #+#             */
/*   Updated: 2022/06/16 05:02:36 by gleal            ###   ########.fr       */
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

Response Server::getResponse() const
{
    return (_response);
}

int			Server::receive_message() const
{
    int new_socket;
    int length = sizeof(_address);
    std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
    if ((new_socket = accept(_fd, (struct sockaddr *)&_address, (socklen_t*)&length))<0)
    {
        ERROR("In accept\n");
        exit(EXIT_FAILURE);
    }
    char buffer[30000] = {0};
    long valread = read( new_socket , buffer, 30000);
    if (valread < 0)
    {
        ERROR("Nothing read\n");
        return(EXIT_FAILURE);
    }
    std::cout << buffer;
    getResponse().send(new_socket);
    printf("\n------------------Hello message sent-------------------\n");
    close(new_socket);
    return (EXIT_SUCCESS);
}

Server::~Server()
{
    close(_fd);
}
