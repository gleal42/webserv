/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 19:30:33 by gleal             #+#    #+#             */
/*   Updated: 2022/07/01 16:35:24 by gleal            ###   ########.fr       */
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
    Request request;
    Response response;
    struct kevent ListQueue[10];
	int n = 0;
    int fd = -1;
    while (1)
    {
		std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
        struct timespec kqTimeout = {2, 0};
        (void)kqTimeout;
        n = kevent(kq.fd(), NULL, 0, ListQueue, 10, NULL);
        // std::cout << "Number of events recorded: " << n << std::endl;
        if (n <= 0)
            continue;
        for (int i = 0; i < n; i++)
        {
            // std::cout << "Event number: " << i << std::endl;
            if (ListQueue[i].ident == (unsigned int)getFd()) // New event for non-existent file descriptor
            {
                fd = accept_client(kq);
                (void)fd;
            }
            else
            {
                if (ListQueue[i].flags & EV_EOF)
                {
                    // std::cout << "Closing Connection for client: " << ListQueue[i].ident << std::endl;
                    kq.update_event(ListQueue[i].ident, EVFILT_READ, EV_DELETE);
                    kq.update_event(ListQueue[i].ident, EVFILT_WRITE, EV_DELETE);
                    close(ListQueue[i].ident);
                    // continue ;
                    return ;
                }
                if (ListQueue[i].filter == EVFILT_READ)
                {
                    std::cout << "About to read the file descriptor: " << ListQueue[i].ident << std::endl;
                    request.parse(ListQueue[i].ident);
                    kq.update_event(ListQueue[i].ident, EVFILT_READ, EV_DISABLE);
                    kq.update_event(ListQueue[i].ident, EVFILT_WRITE, EV_ENABLE);
                }
                if (ListQueue[i].filter == EVFILT_WRITE)
                {
                    Response response(_config, request);
                    response.send(ListQueue[i].ident);
                    kq.update_event(ListQueue[i].ident, EVFILT_READ, EV_ENABLE);
                    kq.update_event(ListQueue[i].ident, EVFILT_WRITE, EV_DISABLE);
                }
            }
        }
    }
}

void	Server::shutdown( void )
{
	this->_socket.close();
}

/* File descriptors open are:
 * 3 - Server fd
 * 4 - Kqueue
 * 5 - Accept client request
 * 6 - favicon.ico https://stackoverflow.com/questions/41686296/why-does-node-hear-two-requests-favicon
*/

/* 

	int	temp_fd; // to make it work until we have a `select` mechanism

	// while (1) {
	//     TODO: infinite loop here with `accept` code below
	// }

	// check if can still add
	if (_connections.size() < _max_connections) {
		Socket *	connection = _socket->accept();
		if (!connection)
			return ; // add error here
		_connections.insert(Connection(connection->fd(), connection));
		// temp
		temp_fd = connection->fd();
	} else {
		stop();
	}

 */

int    Server::accept_client( Kqueue &kq )
{
	Socket *	connection = _socket.accept();
    if (!connection)
        return (-1); // add error here
    int client_fd = connection->fd();
    _connections[client_fd] = connection;
    std::cout << "CLIENT NEW: (" << client_fd << ")" << std::endl;

    kq.update_event(client_fd, EVFILT_READ, EV_ADD | EV_ENABLE);
    kq.update_event(client_fd, EVFILT_WRITE, EV_ADD | EV_DISABLE); // Will be used later in case we can't send the whole message
    return (client_fd);
}