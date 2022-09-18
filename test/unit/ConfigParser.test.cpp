// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "ConfigParser.hpp"

#define CONFIG_FILE "../../default.conf"

TEST_CASE("ConfigParser constructor") {

	SUBCASE("accepts config_file") {

		CHECK_NOTHROW(ConfigParser a(CONFIG_FILE));
    }

	SUBCASE("sets private ServerConfig amount and array") {
		ConfigParser a(CONFIG_FILE);

		CHECK(a.configs_amount() == 0);
    }
}

TEST_CASE("ConfigParser `call` method") {

	SUBCASE("sets private ServerConfig amount and vector") {
		ConfigParser a(CONFIG_FILE);
		a.call();

		CHECK(a.configs_amount() == 1);
    }
}

