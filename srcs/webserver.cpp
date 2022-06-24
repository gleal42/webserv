/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:43:25 by gleal             #+#    #+#             */
/*   Updated: 2022/06/23 19:36:21 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

#include <algorithm>
#include <arpa/inet.h>

/*
** Creates webserver from configuration (that can handle multiple requests)
** @param:	- [string] argv[1]
** @return:	[int] 0 == SUCCESS
** Line-by-line comments:
** @1	Create Kqueue - Will allow us to identify events and handle
*/

void updateEvent(int kqfd, int ident, short filter, u_short flags, u_int fflags, int data, void *udata)
{
    struct kevent kev;
	EV_SET(&kev, ident, filter, flags, fflags, data, udata);
	kevent(kqfd, &kev, 1, NULL, 0, NULL);
}

int    accept_client(int kq, Server &serv)
{
    // // Setup new client with prelim address info
	// sockaddr_in clientAddr;
	// int clientAddrLen = sizeof(clientAddr);
	// int clfd = -1;

	// // Accept the pending connection and retrive the client descriptor
	// clfd = accept(serv.getFd(), (sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
	// if (clfd == -1)
	// 	return -1;

	// // Set socket as non blocking
	// fcntl(clfd, F_SETFL, O_NONBLOCK);

	// // Add kqueue event to track the new client socket for READ and WRITE events
	// updateEvent(kq, clfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	// updateEvent(kq, clfd, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, NULL); // Disabled initially

	// // Print the client's IP on connect
	// std::cout << "[" << clfd << "] connected" << std::endl;

    SocketAddress clientAddr;

    int clientAddrLen = sizeof(clientAddr);
	int req_fd = -1;

    req_fd = accept(serv.getFd(), (sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
    if (req_fd == -1)
    {
        perror("Failed because");
        throw("Accept error");
    }
    fcntl(req_fd, F_SETFL, O_NONBLOCK);
    updateEvent(kq, req_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    updateEvent(kq, req_fd, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, NULL);
    return (req_fd);
}

int webserver(std::string input)
{
    ServerConfig config(input);
    Server sv(config);
    int kq = kqueue();
    if (kq < 0)
    {
        ERROR("In Kqueue: ");
        exit(EXIT_FAILURE);
    }
    updateEvent(kq, sv.getFd(), EVFILT_READ, EV_ADD, 0, 0, NULL);
    // Setting Server Event Kqueue
    struct kevent ListQueue[10];
    int n = 0;
    unsigned int fd = -1;
    try
    {
        std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
        while (1)
        {
            struct timespec kqTimeout = {2, 0};
            (void)kqTimeout;
            n = kevent(kq, NULL, 0, ListQueue, 10, NULL);
            if (n <= 0)
                continue;
            for (int i = 0; i < n; i++)
            {
                if (ListQueue[i].ident == (unsigned int)sv.getFd())
                {
                    fd = accept_client(kq, sv);
                    std::cout << "CLIENT NEW: (" << fd << ") ID: " << i << std::endl;
                }
                else
                {
                    if (ListQueue[i].flags & EV_EOF)
                    {
                        struct kevent new_event;
                        EV_SET(&new_event, ListQueue[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                        kevent(kq, &new_event, 1, NULL, 0, NULL);
                        close(ListQueue[i].ident);
                        continue ;
                    }
                    if (ListQueue[i].filter == EVFILT_READ)
                    {
                        Request request(config, ListQueue[i].ident, &sv.getAddress()); // Client file descriptor management part
                        request.parse(ListQueue[i].ident);
                        Response response(config, request);
                        response.send(ListQueue[i].ident);
                    }
                }
            }
        }
    }
    catch (const char *str)
    {
        std::cerr << str << std::endl;
        return (1);
    }
    return (0);
}

// for (int i = 0; i < n; i++)
// {
//     if (ListQueue[i].ident == (unsigned int)sv.getFd()) // process the received event
//         accept_client(kq, sv.getFd());
// 	// std::cout << "Connections detected: " << n << std::endl;
// 	// std::cout << "server fd: " << sv.getFd() << std::endl;
//     // std::cout << "identifier: " << ListQueue[i].ident << std::endl;
//     // if (ListQueue[i].ident == (unsigned int)sv.getFd()) // process the received event
//     // {
//     //     std::cout << "DOESNT EXIST" << std::endl;
// 	// 	struct kevent new_client;

//     //     sockaddr_in client_address;
//     //     socklen_t client_addr_len = sizeof(client_address);
//     //     if ((req_fd = accept(sv.getFd(), (struct sockaddr *)&client_address, &client_addr_len) < 0))
//     //     {
//     //         perror("Failed because");
//     //         throw("Accept error");
//     //     }
// 	// 	std::cout << "receive fd: " << req_fd << std::endl;
//     //     if (fcntl(req_fd, F_SETFL, O_NONBLOCK) < 0)
//     //     {
//     //         ERROR("In socket flags\n");
//     //         exit(EXIT_FAILURE);
//     //     }
//     //     EV_SET(&new_client, req_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
//     //     if (kevent(kq, &new_client, 1, NULL, 0, NULL) < 0)
// 	// 		perror("Kevent");
//     // }
//     else
//     {
//         std::cout << "ALREADY EXISTS" << std::endl;
//         if (ListQueue[i].flags & EV_EOF)
//         {
//             struct kevent new_event;
//             EV_SET(&new_event, ListQueue[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
//             kevent(kq, &new_event, 1, NULL, 0, NULL);
//             close(ListQueue[i].ident);
//             continue ;
//         }
//         if (ListQueue[i].filter == EVFILT_READ)
//         {
//             Request request(ListQueue[i].ident, &sv.getAddress()); // Client file descriptor management part
//             request.parse(ListQueue[i].ident);
//             Response response(config);
//             response.send(request._socket);
//         }
//     }
// }
