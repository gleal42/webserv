// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "webserver.hpp"

# define CONFIG_FILE "config_file"

TEST_CASE("Compiles") {

	webserver("config_file");

}
