#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Socket.hpp"
#include "ServerConfig.hpp"

// Constants
#define PORT 8080

TEST_CASE("SocketError constructors") {

	SUBCASE("default does not set `fd`") {
		// SocketError a;

		// CHECK(a.fd() == FD_UNSET);
		CHECK_NOTHROW(SocketError a);
    }

	SUBCASE("BindError can be thrown and catched by SocketError") {
		try {
			throw Socket::BindError(PORT);
		}
		catch(SocketError& e) {
			std::string	message(e.what());

			CHECK(message == "Failed to bind to port 8080.");
		}
		catch(std::exception& e) {
			LOG("Shouldn't log this.");
		}
    }

	SUBCASE("CreateError can be thrown and catched by SocketError") {
		try {
			throw Socket::CreateError();
		}
		catch(SocketError& e) {
			std::string	message(e.what());

			CHECK(message == "Failed to create socket.");
		}
		catch(std::exception& e) {
			LOG("Shouldn't log this.");
		}
    }

	SUBCASE("ReusableAddressError can be thrown and catched by SocketError") {
		try {
			throw Socket::ReusableAddressError();
		}
		catch(SocketError& e) {
			std::string	message(e.what());

			CHECK(message == "Failed to make socket address reusable.");
		}
		catch(std::exception& e) {
			LOG("Shouldn't log this.");
		}
    }

	SUBCASE("ReusablePortError can be thrown and catched by SocketError") {
		try {
			throw Socket::ReusablePortError();
		}
		catch(SocketError& e) {
			std::string	message(e.what());

			CHECK(message == "Failed to make socket port reusable.");
		}
		catch(std::exception& e) {
			LOG("Shouldn't log this.");
		}
    }

	SUBCASE("ListenError can be thrown and catched by SocketError") {
		try {
			throw Socket::ListenError();
		}
		catch(SocketError& e) {
			std::string	message(e.what());

			CHECK(message == "Failed to listen on socket.");
		}
		catch(std::exception& e) {
			LOG("Shouldn't log this.");
		}
    }

	SUBCASE("AcceptError can be thrown and catched by SocketError") {
		try {
			throw Socket::AcceptError();
		}
		catch(SocketError& e) {
			std::string	message(e.what());

			CHECK(message == "Failed to Accept new connection.");
		}
		catch(std::exception& e) {
			LOG("Shouldn't log this.");
		}
    }
}
