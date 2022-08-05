/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 23:00:39 by gleal             #+#    #+#             */
/*   Updated: 2022/08/31 16:24:10 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://www.rfc-editor.org/rfc/rfc9112.html#name-status-line

/*

Field name							Description											Example
Accept-CH							Requests HTTP Client Hints							Accept-CH: UA, Platform
Access-Control-Allow-Origin,
Access-Control-Allow-Credentials,
Access-Control-Expose-Headers,
Access-Control-Max-Age,
Access-Control-Allow-Methods,
Access-Control-Allow-Headers		Specifying which web sites can participate
									in cross-origin resource sharing.					Access-Control-Allow-Origin: *
Accept-Patch						Specifies which patch document formats
									this server supports.								Accept-Patch: text/example;charset=utf-8
Accept-Ranges						What partial content range types this server
									supports via byte serving							Accept-Ranges: bytes
Age									The age the object has been in a
									proxy cache in seconds								Age: 12
Allow								Valid methods for a specified resource.
									To be used for a *405 Method not allowed*"			Allow: GET, HEAD
Alt-Svc[47]							Indicate that resources can also be
									accessed at a different network location
									(host or port) or using a different protocol. 		Alt-Svc: http/1.1="http2.example.com:8001"; ma=7200
Cache-Control						Tells all caching mechanisms from server
									to client whether they may cache this object.
									It is measured in seconds"							Cache-Control: max-age=3600
Connection							Control options for the current connection
									and list of hop-by-hop response fields.				Connection: close
Content-Disposition					An opportunity to raise a ""File Download""
									dialogue box for a known MIME type with
									binary format or suggest a filename
									for dynamic content. Quotes are necessary
									with special characters."							Content-Disposition: attachment; filename="fname.ext"
Content-Encoding					The type of encoding used on the data.				Content-Encoding: gzip
Content-Language					The natural language or languages of the
									intended audience for the enclosed content"			Content-Language: da
Content-Length						The length of the response body in octets 			Content-Length: 348
Content-Location					An alternate location for the returned data			Content-Location: /index.htm
Content-MD5							A Base64-encoded binary MD5 sum of the content
									of the response										Content-MD5: Q2hlY2sgSW50ZWdyaXR5IQ==
Content-Range						Where in a full body message this partial
									message belongs										Content-Range: bytes 21010-47021/47022
Content-Type						The MIME type of this content						Content-Type: text/html; charset=utf-8
Date								The date and time that the message was sent			Date: Tue, 15 Nov 1994 08:12:31 GMT
Delta-Base							Specifies the delta-encoding entity tag
									of the response.									Delta-Base: "abc"
ETag								An identifier for a specific version of a
									resource, often a message digest					ETag: "737060cd8c284d8af7ad3082f209582d"
Expires								Gives the date/time after which the response
									is considered stale (in HTTP-date format)"			Expires: Thu, 01 Dec 1994 16:00:00 GMT
IM									Instance-manipulations applied to the response.		IM: feed
Last-Modified						The last modified date for the requested object
									(in HTTP-dateformat)								Last-Modified: Tue, 15 Nov 1994 12:45:26 GMT
Link								Used to express a typed relationship with
									another resource, (RFC 5988)						Link: ; rel="alternate"[52]
Location							Used in redirection, or when a new resource
									has been created.									Location: http://www.w3.org/pub/WWW/													People.html																				Location: /pub/WWW/People.html"

Pragma								Implementation-specific fields that may have
									various effects anywhere along the
									request-response chain."							Pragma: no-cache
Preference-Applied					Indicates which Prefer tokens were honored
									by the server and applied to the processing
									of the request."									Preference-Applied: return=representation
Proxy-Authenticate					Request authentication to access the proxy.			Proxy-Authenticate: Basic
Public-Key-Pins						HTTP Public Key Pinning, announces hash of
									website's authentic TLS certificate"				Public-Key-Pins: max-age=2592000;
																						pin-sha256=""E9CZ9INDbd+2eRQozYqqbQ2yXLVKB9+xcprMF+44U1g="";"
Retry-After							If an entity is temporarily unavailable,
									this instructs the client to try again later.
									Value could be a specified period of
									time (in seconds) or a HTTP-date."	"				- Example 1: Retry-After: 120
   																						- Example 2: Retry-After: Fri, 07 Nov 2014 23:59:59 GMT"
Server								A name for the server								Server: Apache/2.4.1 (Unix)
Set-Cookie							An HTTP cookie	Set-Cookie: UserID=JohnDoe;			Max-Age=3600; Version=1
Strict-Transport-Security			A HSTS Policy informing the HTTP client how
									long to cache the HTTPS only policy and
									whether this applies to subdomains.					Strict-Transport-Security: max-age=16070400; includeSubDomains
Trailer								The Trailer general field value indicates
									that the given set of header fields is present
									in the trailer of a message encoded
									with chunked transfer coding.						Trailer: Max-Forwards
Transfer-Encoding					The form of encoding used to safely transfer
									the entity to the user. Currently defined methods
									are: chunked, compress, deflate, gzip, identity.	Transfer-Encoding: chunked
Tk									Tracking Status header, value suggested
									to be sent in response to a DNT(do-not-track),
									possible values:
									""!"" — under construction
									""?"" — dynamic
									""G"" — gateway to multiple parties
									""N"" — not tracking
									""T"" — tracking
									""C"" — tracking with consent
									""P"" — tracking only if consented
									""D"" — disregarding DNT
									""U"" — updated"	Tk: ?
Upgrade								"Ask the client to upgrade to another protocol. 	Upgrade: h2c, HTTPS/1.3, IRC/6.9, RTA/x11, websocket
Vary								Tells downstream proxies how to match future		- Vary: *
									request headers to decide whether the cached		- Vary: Accept-Language"
									response can be used rather than requesting a
									fresh one from the origin server."

Via									Informs the client of proxies through
									which the response was sent.						Via: 1.0 fred, 1.1 example.com (Apache/1.1)
Warning								A general warning about possible problems			Warning: 199 Miscellaneous warning
									with the entity body.
WWW-Authenticate					Indicates the authentication scheme
									that should be used to access the requested			WWW-Authenticate: Basic
									entity.
X-Frame-Options						Clickjacking protection: deny - no rendering		X-Frame-Options: deny
									within a frame, sameorigin - no rendering if
									origin mismatch, allow-from - allow from specified
									location, allowall - non-standard,
									allow from any location"
*/

#ifndef __RESPONSE_H__
# define __RESPONSE_H__

# include <fstream>
# include <sstream>

# include "Socket.hpp"
# include "HTTPStatus.hpp"
# include "types.hpp"

// ************************************************************************** //
//                               Response Class                               //
// ************************************************************************** //

class Response {

public:

	Response( void ); // put bck private after making pointer in connection
	~Response( void );
	Response &	operator = ( Response const & rhs );

	// Variables

	RequestMethod	request_method;

	std::string 	start_line( const BaseStatus &status );
	void			build_message( const BaseStatus &status );
	void			send_response(Socket const & socket);
	void 			send_error(int socketfd);
	bool			is_empty( void );

	std::string		_uri;

	// Setters

	void			set_header(std::string name, std::string value);
	void			set_content_type( std::string const & type );
	void			set_body( std::string const &type );
	void			set_default_page( void );
	void			save_raw_headers( std::string headers );
	void    		set_with_file( const std::string &filename );

	// Getters

	std::string		message( void );

	// Getters
	const std::string	get_header_value(const std::string name);

	// Unsetters

	void			delete_header( const std::string & name );
	void			clear( void );

private:
	std::string		_body;
	ResponseHeaders	_headers;			// Map of request headers
	std::string		_message;
	Response( Response const & src );		// while not implemented
};

std::ostream &	operator << ( std::ostream & o, Response const & i );

#endif /* __RESPONSE_H__ */
