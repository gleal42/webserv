// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "HTTPStatus.hpp"
#include "macros.hpp"

TEST_CASE("HTTPStatus") {

	SUBCASE("has a public `code`, `reason_phrase` and `group` member variable") {
		HTTPStatus<404> e;

		CHECK(e.code == 404);
		CHECK(e.reason_phrase == "Not Found");
		CHECK(e.reason_phrase == http_phrase(404));
		CHECK(e.group == STATUS_CLIENT_ERROR);
    }

	SUBCASE("can be thrown and catched and returns `reason_phrase` on `what()`") {
		try {
			throw HTTPStatus<400>();
		}
		catch(HTTPStatus<400>& e) {
			std::string	message(e.what());

			CHECK(message == "Bad Request");
			CHECK(message == http_phrase(400));
		}

		try {
			throw HTTPStatus<403>();
		}
		catch(HTTPStatus<403>& e) {
			std::string	message(e.what());

			CHECK(message == "Forbidden");
			CHECK(message == http_phrase(403));
		}
		catch(BaseStatus& e) {
			LOG("Shouldn't log this.");
		}
    }

	SUBCASE("can be catched as base class") {
		try {
			throw HTTPStatus<500>();
		}
		catch(BaseStatus& e) {
			std::string	message(e.what());

			CHECK(message == "Internal Server Error");
			CHECK(message == http_phrase(500));
		}
    }

	SUBCASE("has methods to check `group`") {
		HTTPStatus<100> info;
		HTTPStatus<400> client_error;
		HTTPStatus<500> server_error;

		CHECK(info.is_status_info() == true);
		CHECK(info.is_status_success() == false);
		CHECK(info.is_status_redirect() == false);
		CHECK(info.is_status_error() == false);
		CHECK(info.is_status_client_error() == false);
		CHECK(info.is_status_server_error() == false);

		CHECK(client_error.is_status_info() == false);
		CHECK(client_error.is_status_success() == false);
		CHECK(client_error.is_status_redirect() == false);
		CHECK(client_error.is_status_error() == true);
		CHECK(client_error.is_status_client_error() == true);
		CHECK(client_error.is_status_server_error() == false);

		CHECK(server_error.is_status_info() == false);
		CHECK(server_error.is_status_success() == false);
		CHECK(server_error.is_status_redirect() == false);
		CHECK(server_error.is_status_error() == true);
		CHECK(server_error.is_status_client_error() == false);
		CHECK(server_error.is_status_server_error() == true);
    }

}


