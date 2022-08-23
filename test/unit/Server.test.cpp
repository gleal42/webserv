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
		CHECK(server.fd() > 0);
		CHECK(server.fd() == 3);
    }

	SUBCASE("sets listeners amount from parser") {
		CHECK(server.listeners_amount() == 1);
    }

	// cluster seems to be the wrong word for it
	// https://www.scalecomputing.com/blog/best-practice-always-3-nodes-minimum-in-a-cluster
	SUBCASE("sets `cluster` of configs amount of listeners") {
		CHECK(server.cluster().size() == 1);
    }

	SUBCASE("updates listeners to read and write") {
    }
}

// Server::~Server()
// {
// 	for (Cluster_it it = _cluster.begin(); it != _cluster.end(); ++it) {
//         close_listener(it->first);
// 	}
// 	for (Connections_it it = _connections.begin(); it != _connections.end(); it++) {
//         close_connection(it->first);
// 	}
//     close(this->_fd);
// }

// std::map<int, int> m;
// std::vector<int> key, value;
// for(std::map<int,int>::iterator it = m.begin(); it != m.end(); ++it) {
//   key.push_back(it->first);
//   value.push_back(it->second);
//   std::cout << "Key: " << it->first << std::endl();
//   std::cout << "Value: " << it->second << std::endl();
// }

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

		int				queue_fd = server->fd();
		VectorInt		connection_fds;
		VectorInt		listener_fds;

		Cluster			listeners(server->cluster());
		Connections		connections(server->connections());

		for (Cluster_it it = listeners.begin(); it != listeners.end(); ++it) {
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
}

TEST_CASE("Server `wait_for_events` method") {
	ConfigParser parser(CONFIG_FILE);
}

// void Server::update_event(int ident, short filter, u_short flags)
// {
//     	struct kevent kev;
// 		EV_SET(&kev, ident, filter, flags, 0, 0, NULL);
// 		kevent(this->_fd, &kev, 1, NULL, 0, NULL);
// }

TEST_CASE("Server `update_event` method") {
	ConfigParser parser(CONFIG_FILE);
}

TEST_CASE("Server `new_connection` method") {
	ConfigParser parser(CONFIG_FILE);
}

TEST_CASE("Server `read_connection` method") {
	ConfigParser parser(CONFIG_FILE);
}

TEST_CASE("Server `write_to_connection` method") {
	ConfigParser parser(CONFIG_FILE);
}

TEST_CASE("Server `close_connection` method") {
	ConfigParser parser(CONFIG_FILE);
}

TEST_CASE("Server `close_listener` method") {
	ConfigParser parser(CONFIG_FILE);
}

/* Helpers */

bool	check_fd_unset(int	fd) { return fcntl(fd, F_GETFD) == FD_UNSET; }

