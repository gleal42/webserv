#ifndef __KQUEUE_H__
# define __KQUEUE_H__

#include <stdexcept>
#include <sys/event.h>
#include "Listener.hpp"
#include "Socket.hpp"
#include "ConfigParser.hpp"

class Server 
{
public:
	struct CreateError : public std::runtime_error {
						CreateError( void );
	};
	Server(const ConfigParser &parser);
	~Server();
	// Getters
	int					fd() const;
	// Member Functions
	void				start( void );
	int					wait_for_events();
	void				update_event(int ident, short filter, u_short flags);
	void				read_connection( Socket *connection , struct kevent const & Event );
	void				write_to_connection( Socket *connection );
	ConnectionsIter		find_existing_connection( Cluster cluster, int event_fd );
	void				service(Request & req, Response & res);
	void				close_connection( Listener *listener, int connection_fd);
	
   	struct kevent 		ListQueue[10];
private:
	Server();
	int			_fd;
	Cluster		_cluster;
	size_t		_listeners_amount;
	bool	has_active_connections(Cluster cluster);
};
#endif
