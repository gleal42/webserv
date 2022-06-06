/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:30:33 by gleal             #+#    #+#             */
/*   Updated: 2022/06/06 20:55:54 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
: header(), server_fd(0), new_socket(0), address(), addrlen (sizeof(address)), valread(0)
{

}

Server::Server(const Server &server)
{
    *this = server;    
}

Server::~Server()
{
}


Server &Server::operator=(const Server &server)
{
    header = server.header;
    server_fd = server.server_fd;
    new_socket = server.new_socket;
    address = server.address;
    addrlen = server.addrlen;
    valread = server.valread;
    return (*this);
}