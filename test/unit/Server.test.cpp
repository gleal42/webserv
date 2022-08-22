// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Server.hpp"

#define CONFIG_FILE "config_file"

// struct CreateError : public std::runtime_error {
// 		CreateError( void );
// };
// Server(const ConfigParser &parser);
// ~Server();
// // Getters
// int					fd() const;
// // Member Functions
// // TODO: check if can be private
// void				start( void );
// int					wait_for_events();
// void				update_event(int ident, short filter, u_short flags);
// void				new_connection( Listener * listener );
// void				read_connection( Connection *connection , struct kevent const & Event );
// void				write_to_connection( Connection *connection );
// void				service(Request & req, Response & res);
// void				close_connection( int connection_fd );
// void				close_listener( int listener_fd );
// struct kevent 		ListQueue[10];

TEST_CASE("Server constructors") {
	ConfigParser parser(CONFIG_FILE);

	SUBCASE("only accepts ConfigParser") {
		Server	a(parser);

    }
}

