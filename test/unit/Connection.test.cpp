// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Connection.hpp"
#include "Listener.hpp"
#include "ServerConfig.hpp"

// Constants
#define FD 3 // if no other file descriptors open
#define BUFFER_SIZE 5000

TEST_CASE("Connection constructors") {
	ServerConfig	config;
	Listener		listener(config);

	SUBCASE("with `listener` argument, sets `fd`") {
		// Connection c(&listener);

		// CHECK(c.fd() != FD_UNSET);

		// c.close();
    }
}

TEST_CASE("Connection `close` method") {
	ServerConfig	config;
	Listener		listener(config);

	SUBCASE("closes file descriptor and frees fd") {
		// Connection	a(&listener);
		// a.close();
		// Connection	b(&listener);

		// CHECK(b.fd() == FD);

		// a.close();
		// b.close();
    }

	SUBCASE("is called on Socket destructor") {
		// Connection	*b = new Connection(&listener);

		// CHECK(b->fd() == FD);

		// delete b;
		// Connection	c(&listener);

		// CHECK(c.fd() == FD);

		// close(FD);
		// c.close();
    }
}

TEST_CASE("Connection `send` method") {
	ServerConfig	config;
	Listener		listener(config);
  	std::string		response = "Good talking to you\n";

	SUBCASE("accepts response argument") {
		// Connection 		a(&listener);

		// CHECK_NOTHROW(a.send(response));
		// // TODO: more tests

		// a.close();
    }
}

TEST_CASE("Connection `receive` method") {
	ServerConfig	config;
	Listener		listener(config);
	// Connection 		a(&listener);

	SUBCASE("throws error when buffer size doesnt match read bytes") {
		// CHECK_THROWS(a.receive(BUFFER_SIZE));

		// a.close();
    }
}
