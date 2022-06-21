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
#define BUFFER_SIZE 5000
#define MAX_CONNECTIONS 10

TEST_CASE("Socket constructors") {

	SUBCASE("default does not set `fd`") {
		Socket a;

		CHECK(a.fd() == FD_UNSET);
    }

	SUBCASE("copy and assignment set the same `fd`") {
		Socket a(PORT);
		Socket b(a);

		CHECK(a.fd() == b.fd());
    }

	SUBCASE("with `int port` argument, sets `fd` and `port`") {
		Socket c(PORT);

		CHECK(c.fd() != FD_UNSET);
		CHECK(c.port() != PORT_UNSET);
    }
}

TEST_CASE("Socket `bind` method") {

	SUBCASE("doesn't get called in default constructor") {
		Socket	a;
		CHECK(a.port() == PORT_UNSET);
    }

	SUBCASE("allows setting `port` separate from constructor") {
		Socket	a;
		a.create();
		a.bind(PORT);

		CHECK(a.port() == PORT);
    }

	SUBCASE("doesnt allow setting the same `port` twice") {
		Socket	a;
		Socket	b(PORT);
		a.create();
		CHECK_THROWS(a.bind(PORT));

		try {
			a.bind(PORT);
		}
		catch(Socket::BindError& e) {
			CHECK(e.what() == "Failed to bind to port " + std::to_string(PORT) + ".");
		}
    }
}

TEST_CASE("Socket `close` method") {

	SUBCASE("closes file descriptor and frees port") {
		Socket	a(PORT);
		a.close();
		Socket	b;
		b.create();

		CHECK_NOTHROW(b.bind(PORT));
		CHECK(b.fd() == FD);
		CHECK(b.port() == PORT);
    }

	SUBCASE("is called on destructor") {
		Socket	*b = new Socket();
		b->create();

		CHECK(b->fd() == FD);

		delete b;
		Socket c;
		c.create();

		CHECK(c.fd() == FD);
    }
}

TEST_CASE("Socket `listen` method") {

	SUBCASE("accepts max connections argument") {
		Socket a(PORT);

		CHECK_NOTHROW(a.listen(MAX_CONNECTIONS));
    }

	SUBCASE("fails if port hasn't been set on the socket") {
		Socket	a;

		CHECK_THROWS(a.listen(MAX_CONNECTIONS));

		try {
			a.listen(MAX_CONNECTIONS);
		}
		catch(Socket::ListenError& e) {
			CHECK(std::string(e.what()) == "Failed to listen on socket.");
		}
    }
}

TEST_CASE("Socket `send` method") {
  	std::string response = "Good talking to you\n";

	SUBCASE("accepts response argument") {
		Socket a(PORT);

		CHECK_NOTHROW(a.send(response));
		// TODO: more tests
    }
}

TEST_CASE("Socket `receive` method") {
	Socket a(PORT);

	SUBCASE("returns read bytes") {
		int bytes;

		CHECK_NOTHROW(bytes = a.receive(BUFFER_SIZE));
		// LOG(bytes); // currently outputting -1 which is the error
    }
}

TEST_CASE("Socket `accept` method") {
	Socket server(PORT);
	server.listen(MAX_CONNECTIONS);

	SUBCASE("returns new socket connected to client") {
		Socket *client;
		client = server.accept();

		// CHECK(client->fd() != FD_UNSET);
		CHECK(std::string(strerror(errno)) == "Resource temporarily unavailable");

		if (client)
			delete client;
    }
}
