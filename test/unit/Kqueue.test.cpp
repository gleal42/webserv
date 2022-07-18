// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Kqueue.hpp"

TEST_CASE("Kqueue constructor") {

	SUBCASE("Sets fd") {
		Kqueue a;
		LOG("Kqueee fd is :" << a.fd());
		CHECK(a.fd() >= 0);
    }
}
