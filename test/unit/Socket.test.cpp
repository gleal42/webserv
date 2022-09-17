// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Socket.hpp"
#include "ServerConfig.hpp"

// Constants
#define PORT 8080
#define PORT_STR "8080"
#define HOSTNAME "127.0.0.1"
#define FD 3 // if no other file descriptors open
#define BUFFER_SIZE 5000
#define MAX_CONNECTIONS 10

TEST_CASE("Socket constructors") {

	SUBCASE("default does not set `fd`") {
		Socket a;

		CHECK(a.fd() == FD_UNSET);

		a.close();
    }

	ServerConfig	config;
	config.set_listen(0, PORT_STR);

	SUBCASE("copy and assignment set the same `fd`") {
		Socket 			a(config);
		Socket 			b(a);

		CHECK(a.fd() == b.fd());

		a.close();
		b.close();
    }

	SUBCASE("with `int port` argument, sets `fd` and `port`") {
		Socket c(config);

		CHECK(c.fd() != FD_UNSET);
		CHECK(c.port() != PORT_UNSET);

		c.close();
    }
}

TEST_CASE("Socket `bind` method") {

	SUBCASE("doesn't get called in default constructor") {
		Socket	a;

		CHECK(a.port() == PORT_UNSET);

		a.close();
    }

	SUBCASE("allows setting `port` and `hostname` separate from constructor") {
		Socket	a;
		a.create();
		a.bind(HOSTNAME, PORT);

		CHECK(a.port() == PORT);

		a.close();
    }

	// TODO: get this to pass somehow
	// SUBCASE("allow setting the same `port` twice") {
	// 	ServerConfig	config;

	// 	config.set_listen(0, PORT_STR);

	// 	Socket			a;
	// 	Socket			b(config);

	// 	a.create();

	// 	CHECK_NOTHROW(a.bind(HOSTNAME, PORT));

	// 	a.close();
	// 	b.close();
    // }
}

TEST_CASE("Socket `close` method") {

	SUBCASE("closes file descriptor and frees fd") {
		ServerConfig	config;

		config.set_listen(0, PORT_STR);

		Socket			a(config);
		a.close();
		Socket			b;
		b.create();

		CHECK_NOTHROW(b.bind(HOSTNAME, PORT));

		CHECK(b.fd() == FD);
		CHECK(b.port() == PORT);

		a.close();
		b.close();
    }

	SUBCASE("is called on destructor") {
		Socket	*b = new Socket();
		b->create();

		CHECK(b->fd() == FD);

		delete b;
		Socket c;
		c.create();

		CHECK(c.fd() == FD);

		close(FD);
		c.close();
    }
}

TEST_CASE("Socket `listen` method") {

	SUBCASE("accepts max connections argument") {
		ServerConfig	config;

		config.set_listen(0, PORT_STR);

		Socket 			a(config);

		CHECK_NOTHROW(a.listen(MAX_CONNECTIONS));

		a.close();
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

		a.close();
    }
}

// TEST_CASE("Socket `send` method") {
//   	std::string response = "Good talking to you\n";

// 	SUBCASE("accepts response argument") {
// 		ServerConfig	config;

// 		config.set_listen(0, PORT_STR);

// 		Socket 			a(config);

// 		CHECK_NOTHROW(a.send(response));
// 		// TODO: more tests

// 		a.close();
//     }
// }

TEST_CASE("Socket `receive` method") {
	ServerConfig	config;
	config.set_listen(0, PORT_STR);
	Socket 			a(config);

	SUBCASE("throws error when buffer size doesnt match read bytes") {
		CHECK_THROWS(a.receive(BUFFER_SIZE));

		a.close();
    }
}

TEST_CASE("Socket `accept` method") {
	ServerConfig	config;
	config.set_listen(0, PORT_STR);
	Socket 			server(config);
	server.listen(MAX_CONNECTIONS);

	// TODO: when non-blocking
	// SUBCASE("returns new socket connected to client") {
	// 	Socket *client;
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
