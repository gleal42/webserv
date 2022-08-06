// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Handler.hpp"

TEST_CASE("Handler constructors") {

	SUBCASE("default") {
		CHECK_NOTHROW(Handler a);
    }

	SUBCASE("copy") {
		Handler	a;

		CHECK_NOTHROW(Handler b(a));
    }

	SUBCASE("assignment") {
		Handler	a;

		CHECK_NOTHROW(Handler b = a);
    }
}

TEST_CASE("Handler `service` method") {
	Handler 	handler;
	Request		req;
	Response	res;

	SUBCASE("accepts Request and Response and services based on HTTP method") {
		CHECK_NOTHROW(handler.service(req, res));
    }

	SUBCASE("when HTTP method GET") {
		req.request_method = GET;

		CHECK_THROWS(handler.service(req, res));

		try {
			handler.service(req, res);
		}
		catch(std::exception & e) {
			CHECK(std::string(e.what()) == "You need to implement `do_GET` when inheriting from Handler!");
		}
    }

	SUBCASE("when HTTP method POST") {
		req.request_method = POST;

		CHECK_THROWS(handler.service(req, res));

		try {
			handler.service(req, res);
		}
		catch(std::exception & e) {
			CHECK(std::string(e.what()) == "You need to implement `do_POST` when inheriting from Handler!");
		}
    }

	SUBCASE("when HTTP method DELETE") {
		req.request_method = DELETE;

		CHECK_THROWS(handler.service(req, res));

		try {
			handler.service(req, res);
		}
		catch(std::exception & e) {
			CHECK(std::string(e.what()) == "You need to implement `do_DELETE` when inheriting from Handler!");
		}
    }
}
