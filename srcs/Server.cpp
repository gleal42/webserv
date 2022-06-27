/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:30:33 by gleal             #+#    #+#             */
/*   Updated: 2022/06/27 22:58:57 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::init_addr()
{
    memset(&_address, 0, sizeof(struct sockaddr_in));
    _address.sin_family = AF_INET;
    _address.sin_port = htons(_config.getPort());
    _address.sin_addr.s_addr = INADDR_ANY;
}

Server::Server()
{
}

Server::Server(const ServerConfig &config)
: _config(config), _socket(_config.getPort())
{
	_socket.listen(128); // Change 128 to max connections
}

Server::~Server()
{
	// close(_fd);
}

SocketAddress	&Server::getAddress()
{
    return (_address);
}

int Server::getFd()
{
    return (_socket.fd());
}

void	Server::run( Kqueue &kq )
{
    struct kevent ListQueue[10];
	int n = 0;
    int fd = -1;
    while (1)
    {
		std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
        struct timespec kqTimeout = {2, 0};
        (void)kqTimeout;
        n = kevent(kq.getFd(), NULL, 0, ListQueue, 10, NULL);
        if (n <= 0)
            continue;
        for (int i = 0; i < n; i++)
        {
            if (ListQueue[i].ident == (unsigned int)getFd()) // New event for non-existent file descriptor
            {
                fd = accept_client(kq);
                std::cout << "Kernel Event ID: " << i << std::endl;
                std::cout << "CLIENT NEW: (" << fd << ")" << std::endl;
            }
            else
            {
                if (ListQueue[i].flags & EV_EOF)
                {
                    kq.updateEvent(ListQueue[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    kq.updateEvent(ListQueue[i].ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                    close(ListQueue[i].ident);
                    continue ;
                }
                if (ListQueue[i].filter == EVFILT_READ)
                {
                    Request request(_config, ListQueue[i].ident, &getAddress()); // Client file descriptor management part
                    request.parse(ListQueue[i].ident);
                    Response response(_config, request);
                    response.send(ListQueue[i].ident);
                }
            }
        }
    }
}

void	Server::shutdown( void )
{
	this->_socket.close();
}

int    Server::accept_client( Kqueue &kq )
{
    SocketAddress clientAddr;

    int clientAddrLen = sizeof(clientAddr);

    _connection.set_fd(accept(this->getFd(), (sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen));
    if (_connection.fd() == -1)
        throw Socket::AcceptError();

    fcntl(_connection.fd(), F_SETFL, O_NONBLOCK);
    kq.updateEvent(_connection.fd(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    kq.updateEvent(_connection.fd(), EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, NULL); // Will be used later in case we can't send the whole message
    return (_connection.fd());
}