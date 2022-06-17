#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

#include <iostream>
#include <string>
#include <map>
#include <sys/socket.h>

typedef std::map<std::string, std::string> ResponseHeader;
typedef std::map<std::string, std::string> RequestHeader;
typedef std::map<std::string, std::string> RequestQuery;
typedef std::map<std::string, std::string> RequestAttributes;
typedef std::map<std::string, std::string> RequestMeta;

enum RequestMethod {
	GET,
	POST,
	DELETE,
};

class URI {
	std::string		host;
	std::string		port;
	std::string		path;
	std::string		query;

	std::string		to_s( void ) {
		return std::string("http://") + host + std::string(":") + port + path + query;
	}
};

class Request {
	public:
		Request(int);
		Request(const Request&);
		~Request();
		Request&	operator= (const Request&);

		int					_socket;
		// Socket				socket;
		std::string			request_line; 		// The complete request line such as: `GET / HTTP/1.1`
		RequestMethod		request_method;
		std::string			unparsed_uri; 		// The unparsed URI of the request
		URI					request_uri;		// The parsed URI of the request
		std::string			path;
		std::string			path_info;			// The script name (CGI variable)
		std::string			query_string;		// The query from the URI of the request
		std::string			raw_header;			// The raw header of the request
		RequestHeader		header;				// The parsed header of the request
		std::string			accept;				// The Accept header value
		std::string			accept_charset;		// The Accept-Charset header value
		std::string			accept_encoding;	// The Accept-Encoding header value
		std::string			accept_language;	// The Accept-Language header value
		// SocketAddress		addr;				// The socket address of the server
		// SocketAddress		peeraddr;			// The socket address of the client
		RequestAttributes	attributes;			// Map of request attributes
		std::string			request_time;		// The local time this request was received

		// Parses a request from +socket+.  This is called internally by Server
		void	parse(Socket socket) {
			// ...
			read_request_line(socket);
			read_header(socket);
			// set accept_* values
			// ...
			// setup_forwarded_info();
			// request_uri = parse_uri(unparsed_uri);
	}
};

std::ostream&	operator<<(std::ostream&, const Request&);

#endif

