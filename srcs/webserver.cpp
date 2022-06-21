/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:43:25 by gleal             #+#    #+#             */
/*   Updated: 2022/06/21 01:09:31 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"




/*
** Creates webserver from configuration (that can handle multiple requests)
** @param:	- [string] argv[1]
** @return:	[int] 0 == SUCCESS
** Line-by-line comments:
** @1	Create Kqueue - Will allow us to identify events and handle
*/

int webserver(std::string input)
{
    int kq = kqueue();
    if (kq < 0)
    {
        ERROR("In Kqueue: ");
        exit(EXIT_FAILURE);
    }
    ServerConfig config(input);
    Server sv(config, kq);
    struct kevent eventList[10];
    try
    {
        while (1)
        {
            std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
            int n = kevent(kq, NULL, 0, eventList, 10, NULL);
            for (int i = 0; i < n; i++)
            {
                if (n <= 0)
                {
                    ERROR("In Kevent waiting\n");
                    exit(EXIT_FAILURE); 
                }
                if (eventList[0].filter == EVFILT_READ) // process the received event
                {
                    Request request(sv.getListeners().begin()->fd, &sv.getAddress()); // Client file descriptor management part
                    request.parse();
                    Response response(config);
                    response.send(request._socket);
                }
            }
        }
    }
    catch (const char *str)
    {
        std::cerr << str << std::endl;
    }

    close(sv.getListeners().begin()->fd);
    close(kq);
    return 0;
}