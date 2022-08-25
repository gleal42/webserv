// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Server.hpp"

#define CONFIG_FILE "config_file" // Temporary

/* Helpers */
bool	check_fd_unset(int	fd);

TEST_CASE("Server constructors") {
	ConfigParser parser(CONFIG_FILE);
	parser.call();

	SUBCASE("only accepts ConfigParser") {
		CHECK_NOTHROW(Server server(parser));
    }

	Server	server(parser);

	SUBCASE("sets file descriptor for kernel queue") {
		CHECK(server.queue_fd() > 0);
		CHECK(server.queue_fd() == 3);
    }

	SUBCASE("sets listeners amount from parser") {
		CHECK(server.listeners_amount() == 1);
    }

	// cluster seems to be the wrong word for it
	// https://www.scalecomputing.com/blog/best-practice-always-3-nodes-minimum-in-a-cluster
	SUBCASE("sets `cluster` of configs amount of listeners") {
		CHECK(server.listeners().size() == 1);
    }

	SUBCASE("updates listeners to read and write") {
    }
}

TEST_CASE("Server destructor") {
	ConfigParser parser(CONFIG_FILE);
	parser.call();

	SUBCASE("deletes everything") {
		Server	*server = new Server(parser);

		CHECK_NOTHROW(delete server);
    }

	SUBCASE("closes connections, listeners and queue fd") {
		typedef std::vector<int>			VectorInt;
		typedef std::vector<int>::iterator	VectorInt_it;

		Server			*server = new Server(parser);

		int				queue_fd = server->queue_fd();
		VectorInt		connection_fds;
		VectorInt		listener_fds;

		Listeners			listeners(server->listeners());
		Connections		connections(server->connections());

		for (Listener_it it = listeners.begin(); it != listeners.end(); ++it) {
			listener_fds.push_back(it->first);
		}
		for (Connections_it it = connections.begin(); it != connections.end(); it++) {
			connection_fds.push_back(it->first);
		}

		delete server;

		for (VectorInt_it it = listener_fds.begin(); it != listener_fds.end(); ++it) {
			CHECK(check_fd_unset(*it));
		}
		for (VectorInt_it it = connection_fds.begin(); it != connection_fds.end(); ++it) {
			CHECK(check_fd_unset(*it));
		}
		CHECK(check_fd_unset(queue_fd));
    }
}

TEST_CASE("Server `start` method") {
	ConfigParser parser(CONFIG_FILE);
	parser.call();

	SUBCASE("starts the server") {
		Server	server(parser);

		/*
			Find way to test a running process
		*/
		// CHECK_NOTHROW(server.start());
    }
}

TEST_CASE("Server `events_wait` method") {
	ConfigParser parser(CONFIG_FILE);
	parser.call();

	SUBCASE("waits for events") {
		Server	server(parser);

		/*
			Find way to test a running process
		*/
		// CHECK_NOTHROW(server.events_wait());
    }
}

TEST_CASE("Server `event_update` method") {
	ConfigParser parser(CONFIG_FILE);
}

TEST_CASE("Server `connection_new` method") {
	ConfigParser parser(CONFIG_FILE);
	parser.call();

	SUBCASE("add new connection to connections") {
		Server	server(parser);

		REQUIRE(server.listeners().begin()->first != FD_UNSET);

		// Listeners		cluster = server.listeners();
		// Listener_it	it = cluster.begin();
		// // Listener	listener(*it->second);
		// int			listener_fd = it->first;

		// listener->listen();

		// CHECK_NOTHROW(server.connection_new(server.listeners().begin()->second));
    }
}

/*
	emulate parseable request to do this
*/
TEST_CASE("Server `connection_read` method") {
	ConfigParser parser(CONFIG_FILE);
}

TEST_CASE("Server `connection_write` method") {
	ConfigParser parser(CONFIG_FILE);
}

/* Helpers */

bool	check_fd_unset(int	fd) { return fcntl(fd, F_GETFD) == FD_UNSET; }

