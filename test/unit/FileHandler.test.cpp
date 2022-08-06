// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "FileHandler.hpp"

// FileHandler( void );
// FileHandler( FileHandler const & src );
// ~FileHandler( void );
// FileHandler &	operator = ( FileHandler const & rhs );
// void				do_GET( Request & req, Response & res );
// void				do_POST( Request & req, Response & res );
// void				do_DELETE( Request & req, Response & res );

TEST_CASE("FileHandler `get_content_type` method") {

	SUBCASE("accepts file path and returns mime-type") {
		FileHandler h;

		// CHECK(h.get_content_type("index.html") == "text/html");
    }

}
