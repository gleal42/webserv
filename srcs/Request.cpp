/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 03:51:51 by gleal             #+#    #+#             */
/*   Updated: 2022/06/18 00:51:52 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(int fd, SocketAddress &address)
{
    int length = sizeof(address);
    std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
    if ((this->socket = accept(fd, (struct sockaddr *)&address, (socklen_t*)&length))<0)
    {
        ERROR("In accept\n");
        exit(EXIT_FAILURE);
    }
}

int	Request::getSocket()
{
    return (this->socket);
}

int	Request::receive()
{
    char buffer[30000] = {0};
    long valread = recv( this->getSocket() , buffer, 30000, 0);
    if (valread < 0)
    {
        ERROR("Nothing recved\n");
        return (EXIT_FAILURE);
    }
    if (*buffer)
    {
        std::cout << buffer << std::endl;
        // parse()...
    }
    return (EXIT_SUCCESS);
}

Request::Request()
{
}