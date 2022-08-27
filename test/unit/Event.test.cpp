// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Event.hpp"

TEST_CASE("Event constructor") {

	SUBCASE("accepts OS specific event") {
		CHECK_NOTHROW(Event event(EVENT event_os));
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

TEST_CASE("Event `is_read` method") {
	EVENT event_os;
	Event event(event_os);

	SUBCASE("returns true if event is Read") {
		event_os.filter = EVFILT_READ;

		CHECK(event.is_read() == true);
    }

	SUBCASE("returns false if event is not Read") {
		event_os.filter = EVFILT_WRITE;

		CHECK(event.is_read() == false);
    }
}

TEST_CASE("Event `is_write` method") {
	EVENT event_os;
	Event event(event_os);

	SUBCASE("returns true if event is Write") {
		event_os.filter = EVFILT_WRITE;

		CHECK(event.is_write() == true);
    }

	SUBCASE("returns false if event is not Write") {
		event_os.filter = EVFILT_READ;

		CHECK(event.is_write() == false);
    }
}

