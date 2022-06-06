/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:05:52 by gleal             #+#    #+#             */
/*   Updated: 2022/06/06 20:55:09 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <string>
#include <netinet/in.h>

class Server{
private:
    std::string header;
    int server_fd;
    int new_socket;
    struct sockaddr_in address;
    int addrlen;
    long valread;
public:
    Server();
    Server(const Server &server);
    ~Server();
    Server &operator=(const Server &server);
};

#endif
