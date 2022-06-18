#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

#include <iostream>
#include <string>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ServerConfig.hpp"

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
		Request(int socket, sockaddr_in *sockaddr);
		Request(const Request&);
		~Request();
		Request&	operator= (const Request&);

		int					_socket;
		// Socket				socket;
		std::string			_request_line; 		// The complete request line such as: `GET / HTTP/1.1`
		RequestMethod		_request_method;
		std::string			_unparsed_uri; 		// The unparsed URI of the request
		URI					_request_uri;		// The parsed URI of the request
		std::string			_path;
		std::string			_path_info;			// The script name (CGI variable)
		std::string			_query_string;		// The query from the URI of the request
		std::string			_raw_header;			// The raw header of the request
		RequestHeader		_header;				// The parsed header of the request
		std::string			_accept;				// The Accept header value
		std::string			_accept_charset;		// The Accept-Charset header value
		std::string			_accept_encoding;	// The Accept-Encoding header value
		std::string			_accept_language;	// The Accept-Language header value
		// SocketAddress		addr;				// The socket address of the server
		// SocketAddress		peeraddr;			// The socket address of the client
		RequestAttributes	_attributes;			// Map of request attributes
		std::string			_request_time;		// The local time this request was received

		// Parses a request from +socket+.  This is called internally by Server
		std::string	read_header(std::string buf);
		std::string	read_request_line(std::string buf);
		void		parse(void);
};

std::ostream&	operator<<(std::ostream&, const Request&);

#endif

