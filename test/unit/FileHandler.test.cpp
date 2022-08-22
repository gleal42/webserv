// Doctest tutorial
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "FileHandler.hpp"

/* Helpers */
void	file_create(std::string filename);
void	file_delete(std::string filename);

TEST_CASE("FileHandler constructors") {

	SUBCASE("default") {
		CHECK_NOTHROW(FileHandler a);
    }

	SUBCASE("copy") {
		FileHandler	a;

		CHECK_NOTHROW(FileHandler b(a));
    }

	SUBCASE("assignment") {
		FileHandler	a;

		CHECK_NOTHROW(FileHandler b = a);
    }
}

TEST_CASE("FileHandler `service` method") {
	FileHandler handler;
	Request		req;
	Response	res;

	SUBCASE("accepts Request and Response and services based on HTTP method") {
		CHECK_NOTHROW(handler.service(req, res));
    }
}


TEST_CASE("FileHandler `do_GET` method") {
	FileHandler handler;
	Request		req;
	Response	res;
	req.request_method = GET;

	SUBCASE("when path is empty raises 400") {
		CHECK_THROWS(handler.service(req, res));

		try {
			handler.service(req, res);
		}
		catch(std::exception & e) {
			CHECK(std::string(e.what()) == "Not Found");
		}
    }

	SUBCASE("when path ends with '/' searches for index.html") {
		req._path = "/";
		file_create("index.html");
		std::string	result("HTTP/1.1 200 OK" +
							std::string(CRLF) +
							"Content-Length: 13" +
							std::string(CRLF) +
							"Content-Type: text/html" +
							std::string(CRLF) + std::string(CRLF) +
							"my text here!");

		CHECK_NOTHROW(handler.service(req, res));
		CHECK(res._uri == "index.html");
		CHECK(res.message() == result);

		file_delete("index.html");
    }

	SUBCASE("searches for requested file") {
		req._path = "/random.html";
		file_create("random.html");
		std::string	result("HTTP/1.1 200 OK" +
							std::string(CRLF) +
							"Content-Length: 13" +
							std::string(CRLF) +
							"Content-Type: text/html" +
							std::string(CRLF) + std::string(CRLF) +
							"my text here!");

		CHECK_NOTHROW(handler.service(req, res));
		CHECK(res._uri == "random.html");
		CHECK(res.message() == result);

		file_delete("random.html");
    }

	SUBCASE("when path is longer then 100 raises 400") {
		req._path = "ahjkdflasdlsdjldksjaflsdlashdglasjdfalsdjasldhajksdhajskdasjdlasjkdaskjdasjdkfhaskjdgfalshdgkajsdhfds";

		CHECK_THROWS(handler.service(req, res));

		try {
			handler.service(req, res);
		}
		catch(std::exception & e) {
			CHECK(std::string(e.what()) == "Bad Request");
		}
    }

	SUBCASE("when path includes `..` raises 404") {
		req._path = "../random.html";

		CHECK_THROWS(handler.service(req, res));

		try {
			handler.service(req, res);
		}
		catch(std::exception & e) {
			LOG(e.what());
			CHECK(std::string(e.what()) == "Not Found");
		}
    }

	SUBCASE("when path is to an invalid file raises 404") {
		req._path = "invalid.html";

		CHECK_THROWS(handler.service(req, res));

		try {
			handler.service(req, res);
		}
		catch(std::exception & e) {
			CHECK(std::string(e.what()) == "Not Found");
		}
    }
}

// TEST_CASE("FileHandler `do_POST` method") {
// 	FileHandler handler;
// 	Request		req;
// 	Response	res;
// 	req.request_method = POST;

// 	SUBCASE("when HTTP method POST") {
// 		CHECK_NOTHROW(handler.service(req, res));

//     }
// }

// TEST_CASE("FileHandler `do_DELETE` method") {
// 	FileHandler handler;
// 	Request		req;
// 	Response	res;
// 	req.request_method = DELETE;

// 	SUBCASE("when HTTP method DELETE") {
// 		CHECK_NOTHROW(handler.service(req, res));

//     }
// }

/* Helpers */

void	file_create(std::string filename) {
	std::ofstream	outfile(filename);
	outfile << "my text here!";
	outfile.close();
}

void	file_delete(std::string filename) {
	remove(filename.c_str());
}
