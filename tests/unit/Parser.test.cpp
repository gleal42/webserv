// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Parser.hpp"

#define CONFIG_FILE "config_file"

// TODO: Boiler Plate, complete later
TEST_CASE("Parser constructor") {

	SUBCASE("accepts config_file") {

		CHECK_NOTHROW(Parser a(CONFIG_FILE));
    }

	SUBCASE("sets private ServerConfig amount and array") {
		Parser a(CONFIG_FILE);

		CHECK(a.configs_amount() == 0);
    }
}

TEST_CASE("Parser `call` method") {

	SUBCASE("sets private ServerConfig amount and vector") {
		Parser a(CONFIG_FILE);
		a.call();

		CHECK(a.configs_amount() == 1);
    }
}

