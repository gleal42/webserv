/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 23:01:05 by gleal             #+#    #+#             */
/*   Updated: 2022/07/07 17:30:47 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <map>

#include "macros.hpp"
#include "ServerConfig.hpp"


// https://www.rfc-editor.org/rfc/rfc9112.html#name-request-line

/*
Name								Description																	Example

A-IM								Acceptable instance-manipulations for the request.[12]						A-IM: feed
Accept								Media type(s) that is/are acceptable for the response. 						Accept:	text/html
Accept-Charset						Character sets that are acceptable.											Accept-Charset: utf-8
Accept-Datetime						Acceptable version in time.													Accept-Datetime: Thu, 31 May 2007 20:35:00 GMT
Accept-Encoding						List of acceptable encodings. See HTTP compression.							Accept-Encoding: gzip, deflate
Accept-Language						List of acceptable human languages for response.							Accept-Language: en-US
Access-Control-Request-Method,																					Access-Control-Request-Method: GET
Access-Control-Request-Headers		Initiates a request for cross-origin resource
									sharing with Origin.
Authorization						Authentication credentials for HTTP authentication.							Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==
Cache-Control						Specify directives that *must* be obeyed by all								Cache-Control: no-cache
									caching mechanisms along the request-response chain.
Connection							Control options for the current connection and								Connection: keep-alive
									list of hop-by-hop request fields.											Connection: Upgrade
Content-Encoding					The type of encoding used on the data. 										Content-Encoding: gzip
Content-Length						The length of the request body in octets (8-bit bytes).						Content-Length: 348
Content-MD5							A Base64-encoded binary MD5 sum of the content of the request body.			Content-MD5: Q2hlY2sgSW50ZWdyaXR5IQ==
Content-Type						The Media type of the body of the request									Content-Type: application/x-www-form-urlencoded
									(used with POST and PUT requests).
Cookie								An HTTP cookie previously sent by the server with Set-Cookie.				Cookie: $Version=1; Skin=new;
Date								The date and time at which the message was originated (HTTP-date			Date: Tue, 15 Nov 1994 08:12:31 GMT
									format).
Expect								Indicates that particular server behaviors are required by the client.		Expect: 100-continue
Forwarded							Disclose original information of a client connecting to a web server 		Forwarded: for=192.0.2.60;proto=http;by=203.0.113.43
									through an HTTP proxy.														Forwarded: for=192.0.2.43, for=198.51.100.17
From								The email address of the user making the request.							From: user@example.com
Host								"The domain name of the server (for virtual hosting), and the TCP port 		Host: en.wikipedia.org:8080
									number on which the server is listening. The port number may be omitted if	Host: en.wikipedia.org
									the port is the standard port for the service requested (Mandatory).
If-Match							Only perform the action if the client supplied entity matches the same 		If-Match: "737060cd8c284d8af7ad3082f209582d"
									entity on the server. This is mainly for methods like PUT to only update a
									resource if it has not been modified since the user last updated it.
If-Modified-Since					Allows a *304 Not Modified* to be returned if content is unchanged.			If-Modified-Since: Sat, 29 Oct 1994 19:43:31 GMT
If-None-Match						Allows a *304 Not Modified* to be returned if content is unchanged.			If-None-Match: "737060cd8c284d8af7ad3082f209582d"
If-Range							If the entity is unchanged, send me the part(s) that I am missing; 			If-Range: "737060cd8c284d8af7ad3082f209582d"
									otherwise, send me the entire new entity.
If-Unmodified-Since					Only send the response if the entity has not been modified since a specific If-Unmodified-Since: Sat, 29 Oct 1994 19:43:31 GMT
									time.
Max-Forwards						Limit the number of times the message can be forwarded through proxies or 	Max-Forwards: 10
									gateways.
Origin								Initiates a request for cross-origin resource sharing (asks server for 		Origin: http://www.example-social-network.com
									Access-Control-* response fields).
Pragma								Implementation-specific fields that may have various effects anywhere along Pragma: no-cache
									the request-response chain.
Prefer								Allows client to request that certain behaviors be employed by a server		Prefer: return=representation
									while processing a request.
Proxy-Authorization					Authorization credentials for connecting to a proxy.						Proxy-Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==
Range								Request only part of an entity. Bytes are numbered from 0.					Range: bytes=500-999
Referer								This is the address of the previous web page from which a link to the 		Referer: http://en.wikipedia.org/wiki/Main_Page
									currently requested page was followed.
TE									The transfer encodings the user agent is willing to accept:					TE: trailers, deflate
									the same values as for the response header field
									Transfer-Encoding can be used, plus the ""trailers"" value
									(related to the ""chunked"" transfer method) to notify the
									server it expects to receive additional fields in the trailer after the
									last, zero-sized, chunk.
Trailer								The Trailer general field value indicates that the given set of header		Trailer: Max-Forwards
									fields is present in the trailer of a message encoded
									with chunked transfer coding."
Transfer-Encoding					The form of encoding used to safely transfer the entity to the user.		Transfer-Encoding: chunked
									Currently defined methods are:
									chunked, compress, deflate, gzip, identity.
User-Agent							The user agent string of the user agent.									User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:12.0) Gecko/20100101 Firefox/12.0"
Upgrade								Ask the server to upgrade to another protocol.								Upgrade: h2c, HTTPS/1.3, IRC/6.9, RTA/x11, websocket
Via									Informs the server of proxies through which the request was sent.			Via: 1.0 fred, 1.1 example.com (Apache/1.1)
Warning								A general warning about possible problems with the entity body.				Warning: 199 Miscellaneous warning

*/


typedef std::map<std::string, std::string> ResponseHeader;
typedef std::map<std::string, std::string> RequestHeader;
typedef std::map<std::string, std::string> RequestQuery;
typedef std::map<std::string, std::string> RequestAttributes;
typedef std::map<std::string, std::string> RequestMeta;
typedef sockaddr_in SocketAddress;
class Socket;

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
	Request( ServerConfig const & config );
	Request( Request const & src ); // while not implemented
	~Request();
	Request&	operator= ( Request const & param );

	std::vector<char>	_raw_request;			// The raw header of the request
	std::vector<char>	_unparsed_request;		// The raw header of the request
	std::string			_raw_request_line; 		// The complete request line such as: `GET / HTTP/1.1`
	std::string			_raw_headers;			// The raw header of the request
	std::vector<char>	_raw_body;

	std::string			_unparsed_uri; 				// The unparsed URI of the request
	RequestMethod		request_method;
	URI					request_uri;				// The parsed URI of the request
	int					client_max_body_size;		// Max client body size

	// some of these will be private
	RequestAttributes	_attributes;			// Map of request attributes

	// Parses a request from +socket+.  This is called internally by Server
	void				parse(Socket & socket, struct kevent const & Event );
	void				read_header(std::vector<char> &strptr);
	void				read_request_line(std::vector<char> &strptr);
	void				read_body(std::vector<char> &strptr);

private:

	Request( void );

};

std::ostream&	operator<<(std::ostream&, const Request&);

#endif
