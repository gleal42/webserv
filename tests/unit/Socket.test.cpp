// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

/*
Usage:
	c++ -std=c++98 -Iinc srcs/Socket.cpp tests/Socket.test.cpp -o socket-test
*/
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Socket.hpp"

TEST_CASE("Socket member properties") {
	Socket socket;

	CHECK(socket.fd() > -1);
	CHECK(socket.fd() == 3); // if no other file descriptors open
}

