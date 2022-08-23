// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Server.hpp"

#define CONFIG_FILE "config_file" // Temporary

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

// void Server::update_event(int ident, short filter, u_short flags)
// {
//     	struct kevent kev;
// 		EV_SET(&kev, ident, filter, flags, 0, 0, NULL);
// 		kevent(this->_fd, &kev, 1, NULL, 0, NULL);
// }

	SUBCASE("updates listeners to read and write") {
    }
}

TEST_CASE("Server destructor") {
	ConfigParser parser(CONFIG_FILE);
}

TEST_CASE("Server `start` method") {
	ConfigParser parser(CONFIG_FILE);
}

TEST_CASE("Server `wait_for_events` method") {
	ConfigParser parser(CONFIG_FILE);
}

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

