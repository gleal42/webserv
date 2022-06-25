/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 19:43:25 by gleal             #+#    #+#             */
/*   Updated: 2022/06/25 18:38:12 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

#include <algorithm>
#include <arpa/inet.h>

KqueueError::KqueueError( void )
: std::runtime_error("Failed to create Kernel Queue.") { /* No-op */ }

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

int    Server::accept_client( int kq )
{
    SocketAddress clientAddr;

    int clientAddrLen = sizeof(clientAddr);

    _connection.set_fd(accept(this->getFd(), (sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen));
    if (_connection.fd() == -1)
        throw Socket::AcceptError();

    fcntl(_connection.fd(), F_SETFL, O_NONBLOCK);
    updateEvent(kq, _connection.fd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    updateEvent(kq, _connection.fd(), EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, NULL); // Will be used later in case we can't send the whole message
    return (_connection.fd());
}

int webserver(std::string input)
{
    ServerConfig config(input);
    Server sv(config);
    int kq = kqueue();
    if (kq < 0)
        throw KqueueError();
    updateEvent(kq, sv.getFd(), EVFILT_READ, EV_ADD, 0, 0, NULL);
    // Setting Server Event Kqueue
    try
    {
        sv.run(kq);
    }
    catch (const char *str)
    {
        std::cerr << str << std::endl;
        return (1);
    }
    sv.shutdown();
    return (0);
}
