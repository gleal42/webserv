/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 16:15:31 by gleal             #+#    #+#             */
/*   Updated: 2022/06/07 18:53:48 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

int	Client::init_addr()
{
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (address.sin_addr.s_addr == (in_addr_t)(-1))
    {
		std::cerr << "\nInvalid address/ Address not supported \n";
        return (-1);
    }
    return (0);
}

Client::Client()
{
}

Client::Client(const char *config)
{
    (void)config;
    conf.port = 8080;
    conf.header = "Hello from client";
    init_addr();
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
		std::cerr << "Socket creation error \n";
        exit(EXIT_FAILURE);
    }
    if (connect(fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
		std::cerr << "\nConnection Failed \n";
        exit(EXIT_FAILURE);
    }
}

Client::Client(const Client &client)
{
    (void)client;
}

Client::~Client()
{
}

Client &Client::operator=(const Client &client)
{
    (void)client;
    return (*this);
}

int	Client::send_msg() const
{
    long valread;
    std::vector<char>buffer(1024);
    send(fd, conf.header.c_str(), strlen(conf.header.c_str()), 0 );
	std::cout << "Hello message sent\n";
    valread = recv(fd, buffer.data(), buffer.size(), 0);
    if (valread < 0)
    {
        ERROR ("Error when client read");
    }
    std::cout << buffer.data() << std::endl;
    return 0;
}