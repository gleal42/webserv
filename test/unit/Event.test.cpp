// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Event.hpp"

TEST_CASE("Event constructor") {

	SUBCASE("accepts OS specific event") {
		CHECK_NOTHROW(Event event(EVENT event_os));
    }

	SUBCASE("default initiates empty OS specific event") {
		CHECK_NOTHROW(Event event);
    }
}

TEST_CASE("Event `fd` method") {
	EVENT event_os;
	event_os.ident = 3;
	Event event(event_os);

	SUBCASE("gets specific event fd") {
		CHECK(event.fd() == event_os.ident);
    }
}

// TEST_CASE("Event `flags` method") {

// 	SUBCASE("gets specific event flags") {

//     }
// }

// TEST_CASE("Event `filter` method") {

// 	SUBCASE("gets specific event filter") {

//     }
// }

