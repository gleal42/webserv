#ifndef __KQUEUE_H__
# define __KQUEUE_H__

#include <stdexcept>
#include <sys/event.h>
#include "Server.hpp"

class Kqueue 
{
	public:
		struct CreateError : public std::runtime_error {
							CreateError( void );
		};
		Kqueue();
		// Getters
		int					fd() const;
		// Member Functions
		void				update_event(int ident, short filter, u_short flags);
		int					wait_for_events();
		ConnectionsIter		find_existing_connection( Cluster cluster, int event_fd );
		void				read_connection( Socket *connection );
		void				write_to_connection( Socket *connection );
		void				close_connection( int connection_fd );
		void				run( Cluster cluster );
    	struct kevent 		ListQueue[10];
	private:
		int					_fd;
};

#endif
