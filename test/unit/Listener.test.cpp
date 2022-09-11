// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Listener.hpp"

// Constants
#define PORT 8080
#define PORT_STR "8080"
#define FD 3 // if no other file descriptors open
#define BUFFER_SIZE 5000
#define MAX_CONNECTIONS 10

TEST_CASE("Listener constructors") {
	ServerConfig	config;

	SUBCASE("copy and assignment set the same `fd`") {
		Listener	a(config);
		Listener	b(a);

		CHECK(a.fd() == b.fd());

		a.close();
		b.close();
    }

	SUBCASE("with `config` argument, sets `fd` and `port`") {
		Listener c(config);

		CHECK(c.fd() != FD_UNSET);
		CHECK(c.port() != PORT_UNSET);

		c.close();
    }
}

TEST_CASE("Listener `close` method") {
	ServerConfig	config;
	config.set_listen(false, PORT_STR);

	SUBCASE("closes file descriptor and frees fd") {
		Listener		a(config);
		a.close();
		Listener		b(config);

		CHECK(b.fd() == FD);
		CHECK(b.port() == PORT);

		b.close();
    }

	SUBCASE("is called on Socket destructor") {
		Listener	*b = new Listener(config);

		CHECK(b->fd() == FD);

		delete b;
		Listener c(config);

		CHECK(c.fd() == FD);

		close(FD);
		c.close();
    }
}

TEST_CASE("Listener `listen` method") {
	ServerConfig	config;

	SUBCASE("accepts max connections argument") {
		Listener		a(config);

		CHECK_NOTHROW(a.listen(MAX_CONNECTIONS));

		a.close();
    }
}

TEST_CASE("Listener `send` method") {
  	std::string response = "Good talking to you\n";

	SUBCASE("accepts response argument") {
		ServerConfig	config;
		Listener 		a(config);

		CHECK_NOTHROW(a.send(response));
		// TODO: more tests

		a.close();
    }
}

TEST_CASE("Listener `receive` method") {
	ServerConfig	config;
	Listener 		a(config);

	SUBCASE("throws error when buffer size doesnt match read bytes") {
		CHECK_THROWS(a.receive(BUFFER_SIZE));

		a.close();
    }
}

TEST_CASE("Listener `accept` method") {
	ServerConfig	config;
	Listener 		server(config);
	server.listen(MAX_CONNECTIONS);

	// TODO: when non-blocking
	// SUBCASE("returns new listener connected to client") {
	// 	Listener *client;
	// 	client = server.accept();

	// 	// CHECK(client->fd() != FD_UNSET);
	// 	CHECK(std::string(strerror(errno)) == "Resource temporarily unavailable");

	// 	if (client) {
	// 		client->close();
	// 		delete client;
	// 	}
    // }

	server.close();
}
