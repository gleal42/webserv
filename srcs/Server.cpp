/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:30:33 by gleal             #+#    #+#             */
/*   Updated: 2022/06/07 23:31:47 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::init_addr()
{
    address.sin_family = AF_INET;
    address.sin_port = htons(conf.getPort());
    address.sin_addr.s_addr = INADDR_ANY;
    memset(address.sin_zero, '\0', sizeof(address.sin_zero));
}

Server::Server()
{
}

Server::Server(const ServerConf &config)
{
    conf = config;
    init_addr();
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == 0)
    {
        ERROR("In socket\n");
        exit(EXIT_FAILURE);
    }
    if (bind(fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        ERROR("In bind\n");
        exit(EXIT_FAILURE);
    }
    if (listen(fd, 128) < 0)
    {
        ERROR("In listen\n");
        exit(EXIT_FAILURE);
    }
}

int			Server::rcv_msg() const
{
    int new_socket;
    int addrlen = sizeof(address);
    std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
    if ((new_socket = accept(fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
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
    write(new_socket, conf.getHeader().c_str() , conf.getHeader().size());
    printf("\n------------------Hello message sent-------------------\n");
    close(new_socket);
    return (EXIT_SUCCESS);
}

Server::~Server()
{
    close(fd);
}
