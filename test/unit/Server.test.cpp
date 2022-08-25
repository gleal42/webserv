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
	parser.call();

	SUBCASE("starts the server") {
		Server	server(parser);

		/*
			Find way to test a running process
		*/
		// CHECK_NOTHROW(server.start());
    }
}

// int	Server::wait_for_events()
// {
// 	std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
//     struct timespec kqTimeout = {2, 0};
//     (void)kqTimeout;
//     return (kevent(this->fd(), NULL, 0, ListQueue, 10, NULL));
// }

TEST_CASE("Server `wait_for_events` method") {
	ConfigParser parser(CONFIG_FILE);
	parser.call();

	SUBCASE("waits for events") {
		Server	server(parser);

		/*
			Find way to test a running process
		*/
		// CHECK_NOTHROW(server.wait_for_events());
    }
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

// void	Server::new_connection( Listener * listener )
// {
// 	// check if can still add
// 	Connection * connection  = new Connection(listener->socket());
// 	int client_fd = connection->fd();
// 	_connections[client_fd] = connection;
// 	std::cout << "CLIENT NEW: (" << client_fd << ")" << std::endl;
// 	update_event(client_fd, EVFILT_READ, EV_ADD | EV_ENABLE);
// 	// Will be used later in case we can't send the whole message
// 	update_event(client_fd, EVFILT_WRITE, EV_ADD | EV_DISABLE);
// }

TEST_CASE("Server `new_connection` method") {
	ConfigParser parser(CONFIG_FILE);
	parser.call();

	SUBCASE("add new connection to connections") {
		Server	server(parser);

		REQUIRE(server.cluster().begin()->first != FD_UNSET);

		// Cluster		cluster = server.cluster();
		// Cluster_it	it = cluster.begin();
		// // Listener	listener(*it->second);
		// int			listener_fd = it->first;

		// listener->listen();

		// CHECK_NOTHROW(server.new_connection(server.cluster().begin()->second));
    }
}

/*
	emulate parseable request to do this
*/
TEST_CASE("Server `read_connection` method") {
	ConfigParser parser(CONFIG_FILE);
}



// void	Server::write_to_connection( Connection *connection )
// {
// 	std::cout << "About to write to file descriptor: " << connection->fd() << std::endl;
// 	std::cout << "The socket has the following size to write " << ListQueue[0].data << std::endl; // Could use for better size efficiency
//     if (connection->response.is_empty())
//         service(connection->request, connection->response);
//     connection->response.send_response(*connection->socket());
//     if (connection->response.is_empty())
//     {
//         std::cout << "Connection was empty after sending" << std::endl;
//         this->update_event(connection->fd(), EVFILT_READ, EV_ENABLE);
//         this->update_event(connection->fd(), EVFILT_WRITE, EV_DISABLE);
//     }
// }

TEST_CASE("Server `write_to_connection` method") {
	ConfigParser parser(CONFIG_FILE);
}

/* Helpers */

bool	check_fd_unset(int	fd) { return fcntl(fd, F_GETFD) == FD_UNSET; }

