#ifndef __KQUEUE_H__
# define __KQUEUE_H__

#include <stdexcept>
#include <sys/event.h>
#include "Listener.hpp"
#include "Socket.hpp"

class Server 
{
public:
	struct CreateError : public std::runtime_error {
						CreateError( void );
	};
	Server();
	// Getters
	int					fd() const;
	// Member Functions
	void				update_event(int ident, short filter, u_short flags);
	int					wait_for_events();
	ConnectionsIter		find_existing_connection( Cluster cluster, int event_fd );
	void				read_connection( Socket *connection , struct kevent const & Event );
	void				write_to_connection( Socket *connection );
	void				close_connection( Listener *listener, int connection_fd);
	void				run( Cluster cluster );
	void				service(Request & req, Response & res);
	
   	struct kevent 		ListQueue[10];
private:
	int		_fd;
	bool	has_active_connections(Cluster cluster);
};
#endif
