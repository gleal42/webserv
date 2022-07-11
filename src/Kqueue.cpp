/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kqueue.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 19:11:20 by gleal             #+#    #+#             */
/*   Updated: 2022/07/11 22:37:56 by gleal            ###   ########.fr       */
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

int	Kqueue::fd() const { return(_fd); }

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
            ClusterIter event_fd = cluster.find(ListQueue[i].ident);
            if (event_fd != cluster.end()) // New event for non-existent file descriptor
                event_fd->second->accept_client(*this);
            else
            {
				ConnectionsIter connection_it = find_existing_connection(cluster, ListQueue[i].ident);
                if (ListQueue[i].flags & EV_EOF) {
                    close_connection(connection_it->second->parent(), ListQueue[i].ident); // If there are no more connections open in any server do cleanup(return)
                    if (!has_active_connections(cluster))
                        return ;
                }
                else if (ListQueue[i].filter == EVFILT_READ)
                    read_connection(connection_it->second, ListQueue[i]);
                else if (ListQueue[i].filter == EVFILT_WRITE)
                {
					write_to_connection(connection_it->second);
                    if (connection_it->second->response.is_empty())
                        connection_it->second->request.clear();
                }
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
    for (ClusterIter listener = cluster.begin(); listener != cluster.end(); listener++)
    {
        connection_it = listener->second->_connections.find(event_fd);
        if (connection_it != listener->second->_connections.end())
            return (connection_it);
    }
    return (connection_it);
}

void	Kqueue::close_connection( Listener *listener, int connection_fd)
{
    std::cout << "Closing Connection for client: " << connection_fd << std::endl;
    this->update_event(connection_fd, EVFILT_READ, EV_DELETE);
    this->update_event(connection_fd, EVFILT_WRITE, EV_DELETE);
    close(connection_fd);
    // p connection_fd
    // p listener._connections[connection_fd]
    delete listener->_connections[connection_fd];
    listener->_connections.erase(connection_fd);
}

void	Kqueue::read_connection( Socket *connection, struct kevent const & Event )
{
    std::cout << "About to read the file descriptor: " << connection->fd() << std::endl;
    std::cout << "Incoming data has size of: " << Event.data << std::endl;
    connection->request.parse(*connection, Event);
    if (connection->request._attributes.count("Content-Length"))
    {
        std::cout << "Analyzing if whole body was transferred: " << std::endl;
        std::stringstream content_length(connection->request._attributes["Content-Length"]);
        size_t value = 0;
        content_length >> value;
        if (connection->request._raw_body.size() < value + 1)
        {
            std::cout << "Body total received :" << connection->request._raw_body.size() << std::endl;
            std::cout << "Content Length :" << value << std::endl;
            std::cout << "Remaining :" << value - connection->request._raw_body.size() << std::endl;
            return ;
        }
    }
    std::cout << "Final Body size :" << connection->request._raw_body.size() << std::endl;

    this->update_event(connection->fd(), EVFILT_READ, EV_DISABLE);
    this->update_event(connection->fd(), EVFILT_WRITE, EV_ENABLE);
}

void	Kqueue::write_to_connection( Socket *connection )
{
	std::cout << "About to write to file descriptor: " << connection->fd() << std::endl;
	std::cout << "The socket has the following size to write " << ListQueue[0].data << std::endl; // Could use for better size efficiency
    if (connection->response.is_empty())
        connection->response.prepare_response(connection->request, connection->parent()->_config);
    connection->response.send_response(*connection);
    if (connection->response.is_empty())
    {
        std::cout << "Connection was empty after sending" << std::endl;
        this->update_event(connection->fd(), EVFILT_READ, EV_ENABLE);
        this->update_event(connection->fd(), EVFILT_WRITE, EV_DISABLE);
    }
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
