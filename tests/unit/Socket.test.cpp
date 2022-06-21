// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

/*
Usage:
	c++ -std=c++98 -Iinc srcs/Socket.cpp tests/Socket.test.cpp -o socket-test
*/
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Socket.hpp"

// Constants
#define PORT 8080
#define FD 3 // if no other file descriptors open

TEST_CASE("Socket constructors") {

	SUBCASE("default sets `fd`") {
		Socket a;

		CHECK(a.fd() > -1);
		CHECK(a.fd() == FD);
    }

	SUBCASE("copy and assignment set the same `fd`") {
		Socket a;
		Socket b(a);

		CHECK(b.fd() > -1);
		CHECK(b.fd() == FD);
    }

	SUBCASE("with `int port` argument, sets `fd` and `port`") {
		Socket c(PORT);

		CHECK(c.fd() > -1);
		CHECK(c.port() == PORT);
    }
}

TEST_CASE("Socket `bind` method") {
	Socket	a;
	int		port = PORT;

	SUBCASE("doesn't get called in default constructor") {
		CHECK(a.port() == 0);
    }

	SUBCASE("allows setting `port` separate from constructor") {
		a.bind(port);

		CHECK(a.port() == port);
    }

	SUBCASE("doesnt allow setting the same `port` twice") {
		Socket b(port);
		CHECK_THROWS(a.bind(port));

		try {
			a.bind(port);
		}
		catch(Socket::BindError& e) {
			CHECK(e.what() == "Failed to bind to port " + std::to_string(port) + ".");
		}
    }
}

TEST_CASE("Socket `close` method") {
	Socket	a;
	int		port = PORT;

	SUBCASE("closes file descriptor") {
		a.close();
		Socket	b;

		CHECK(b.fd() == FD);
    }
}

