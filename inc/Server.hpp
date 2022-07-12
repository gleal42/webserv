#ifndef __KQUEUE_H__
# define __KQUEUE_H__

#include <stdexcept>
#include <sys/event.h>
#include "Listener.hpp"
#include "Connection.hpp"
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
	void				new_connection( Listener * listener );
	void				read_connection( Connection *connection , struct kevent const & Event );
	void				write_to_connection( Connection *connection );
	void				service(Request & req, Response & res);
	void				close_connection( int connection_fd );
	void				close_listener( int listener_fd );
   	struct kevent 		ListQueue[10];
private:
	Server();
	Server( const Server &src );
	Server &operator=( const Server &src );
	int					_fd;
	Cluster				_cluster;
	Connections			_connections;
	size_t				_listeners_amount;
};
#endif
