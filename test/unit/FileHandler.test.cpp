// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "FileHandler.hpp"

TEST_CASE("FileHandler `get_content_type` method") {

	SUBCASE("accepts file path and returns mime-type") {
		FileHandler h;

		// CHECK(h.get_content_type("index.html") == "text/html");
    }

}


