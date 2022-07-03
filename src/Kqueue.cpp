/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kqueue.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 19:11:20 by gleal             #+#    #+#             */
/*   Updated: 2022/07/04 00:12:32 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kqueue.hpp"
#include <iostream>

/* 
    Testes a passar:
    Display big images ()
    Vários Servers ao mesmo tempo
    Várias Requests ao mesmo tempo
    EOF working
    HTML CSS priority
    Javascript (later)
 */

Kqueue::CreateError::CreateError( void )
: std::runtime_error("Failed to create Kernel Queue.") { /* No-op */ }

Kqueue::Kqueue()
{
    _fd = kqueue();
    if (_fd < 0)
        throw CreateError();
}

int	Kqueue::fd() const
{
	return(_fd);
}

void Kqueue::update_event(int ident, short filter, u_short flags)
{
    struct kevent kev;
	EV_SET(&kev, ident, filter, flags, 0, 0, NULL);
	kevent(this->_fd, &kev, 1, NULL, 0, NULL);
}

/* 
 * File descriptors open are:
 * 3 - Server fd
 * 4 - Kqueue
 * 5 - Accept client request
 * 6 - favicon.ico https://stackoverflow.com/questions/41686296/why-does-node-hear-two-requests-favicon
*/

void	Kqueue::run( Cluster cluster )
{
	int nbr_events = 0;
    while (1)
    {
		nbr_events = wait_for_events();
		std::cout << "Number of events recorded: " << nbr_events << std::endl;
        if (nbr_events <= 0)
            continue;
        for (int i = 0; i < nbr_events; i++)
        {
            // std::cout << "Event number: " << i << std::endl;
            ClusterIter event_fd = cluster.find(ListQueue[i].ident);
            if (event_fd != cluster.end()) // New event for non-existent file descriptor
                event_fd->second->accept_client(*this);
            else
            {
				ConnectionsIter connection_it = find_existing_connection(cluster, ListQueue[i].ident);
                if (ListQueue[i].flags & EV_EOF) {
                    close_connection(connection_it->second->server(), ListQueue[i].ident); // If there are no more connections open in any server do cleanup(return)
                    if (!has_active_connections(cluster))
                        return ;
                }
                else if (ListQueue[i].filter == EVFILT_READ)
                    read_connection(connection_it->second);
                else if (ListQueue[i].filter == EVFILT_WRITE)
					write_to_connection(connection_it->second);
            }
        }
    }
}

int	Kqueue::wait_for_events()
{
	std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
    struct timespec kqTimeout = {2, 0};
    (void)kqTimeout;
    return (kevent(this->fd(), NULL, 0, ListQueue, 10, NULL));
}


ConnectionsIter	Kqueue::find_existing_connection( Cluster cluster, int event_fd )
{
    ConnectionsIter connection_it;
    for (ClusterIter it_server = cluster.begin(); it_server != cluster.end(); it_server++)
    {
        connection_it = it_server->second->_connections.find(event_fd);
        if (connection_it != it_server->second->_connections.end())
            return (connection_it);
    }
    return (connection_it);
}

void	Kqueue::close_connection( Server *server, int connection_fd)
{
    std::cout << "Closing Connection for client: " << connection_fd << std::endl;
    this->update_event(connection_fd, EVFILT_READ, EV_DELETE);
    this->update_event(connection_fd, EVFILT_WRITE, EV_DELETE);
    close(connection_fd);
    // p connection_fd
    // p server._connections[connection_fd]
    delete server->_connections[connection_fd];
    server->_connections.erase(connection_fd);
}

void	Kqueue::read_connection( Socket *connection )
{
    std::cout << "About to read the file descriptor: " << connection->fd() << std::endl;
    connection->request.parse(*connection);
    this->update_event(connection->fd(), EVFILT_READ, EV_DISABLE);
    this->update_event(connection->fd(), EVFILT_WRITE, EV_ENABLE);
}

void	Kqueue::write_to_connection( Socket *connection )
{
	std::cout << "About to write to file descriptor: " << connection->fd() << std::endl;
    Response response(connection->request);
    response.send_response(*connection);
    this->update_event(connection->fd(), EVFILT_READ, EV_ENABLE);
    this->update_event(connection->fd(), EVFILT_WRITE, EV_DISABLE);
}

bool	Kqueue::has_active_connections(Cluster cluster)
{
    for (ClusterIter it = cluster.begin(); it != cluster.end(); ++it)
    {
        if (it->second->_connections.size())
            return true;
    }
    return false;
}