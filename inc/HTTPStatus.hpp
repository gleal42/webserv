/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 22:26:01 by msousa            #+#    #+#             */
/*   Updated: 2022/07/04 18:18:26 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __HTTP_STATUS_H__
# define __HTTP_STATUS_H__

# include <iostream>
# include <map>

typedef std::map<int, std::string> HTTPStatuses;

enum HTTPStatusGroup {
	STATUS_INVALID,
	STATUS_INFO,
	STATUS_SUCCESS,
	STATUS_REDIRECT,
	STATUS_CLIENT_ERROR,
	STATUS_SERVER_ERROR,
};

std::string const	http_phrase( int code )
{
    static HTTPStatuses	statuses;

	if (!statuses.empty()) {
		return statuses[code];
	}

    statuses[100] = "Continue";
    statuses[101] = "Switching Protocols";
    statuses[200] = "OK";
    statuses[201] = "Created";
    statuses[202] = "Accepted";
    statuses[203] = "Non-Authoritative Information";
    statuses[204] = "No Content";
    statuses[205] = "Reset Content";
    statuses[206] = "Partial Content";
    statuses[207] = "Multi-Status";
    statuses[300] = "Multiple Choices";
    statuses[301] = "Moved Permanently";
    statuses[302] = "Found";
    statuses[303] = "See Other";
    statuses[304] = "Not Modified";
    statuses[305] = "Use Proxy";
    statuses[307] = "Temporary Redirect";
    statuses[400] = "Bad Request";
    statuses[401] = "Unauthorized";
    statuses[402] = "Payment Required";
    statuses[403] = "Forbidden";
    statuses[404] = "Not Found";
    statuses[405] = "Method Not Allowed";
    statuses[406] = "Not Acceptable";
    statuses[407] = "Proxy Authentication Required";
    statuses[408] = "Request Timeout";
    statuses[409] = "Conflict";
    statuses[410] = "Gone";
    statuses[411] = "Length Required";
    statuses[412] = "Precondition Failed";
    statuses[413] = "Request Entity Too Large";
    statuses[414] = "Request-URI Too Large";
    statuses[415] = "Unsupported Media Type";
    statuses[416] = "Request Range Not Satisfiable";
    statuses[417] = "Expectation Failed";
    statuses[422] = "Unprocessable Entity";
    statuses[423] = "Locked";
    statuses[424] = "Failed Dependency";
    statuses[426] = "Upgrade Required";
    statuses[428] = "Precondition Required";
    statuses[429] = "Too Many Requests";
    statuses[431] = "Request Header Fields Too Large";
    statuses[451] = "Unavailable For Legal Reasons";
    statuses[500] = "Internal Server Error";
    statuses[501] = "Not Implemented";
    statuses[502] = "Bad Gateway";
    statuses[503] = "Service Unavailable";
    statuses[504] = "Gateway Timeout";
    statuses[505] = "HTTP Version Not Supported";
    statuses[507] = "Insufficient Storage";
    statuses[511] = "Network Authentication Required";
    return statuses[code];
}

HTTPStatusGroup	http_group( int code )
{
	switch (code)
	{
	case 100 ... 199: return STATUS_INFO; break;
	case 200 ... 299: return STATUS_SUCCESS; break;
	case 300 ... 399: return STATUS_REDIRECT; break;
	case 400 ... 499: return STATUS_CLIENT_ERROR; break;
	case 500 ... 599: return STATUS_SERVER_ERROR; break;
	default: return STATUS_INVALID; break;
	};
}

class BaseStatus: virtual public std::exception {
public:

	// https://stackoverflow.com/questions/121162/what-does-the-explicit-keyword-mean
	explicit BaseStatus( int code,
						 std::string reason_phrase,
						 HTTPStatusGroup group )
						 : code(code), reason_phrase(reason_phrase), group(group) {}
	virtual ~BaseStatus( void ) throw () {} // To be able to subclass

	// Variables
	int					code;
	std::string			reason_phrase;
	HTTPStatusGroup		group;

	// Methods
	virtual const char*		what( void ) const throw () {
		return reason_phrase.c_str();
	}
	bool	is_status_info( void ) 			{ return http_group(code) == STATUS_INFO; }
	bool	is_status_success( void )		{ return http_group(code) == STATUS_SUCCESS; }
	bool	is_status_redirect( void )		{ return http_group(code) == STATUS_REDIRECT; }
	bool	is_status_error( void ) 		{ return code >= 400; }
	bool	is_status_client_error( void )	{ return http_group(code) == STATUS_CLIENT_ERROR; }
	bool	is_status_server_error( void )	{ return http_group(code) == STATUS_SERVER_ERROR; }
};

// ************************************************************************** //
//                            HTTPStatus Template                          	  //
// ************************************************************************** //

template <int Code>
class HTTPStatus: virtual public BaseStatus {
public:
	explicit HTTPStatus( void ) : BaseStatus(Code, http_phrase(Code), http_group(Code)) {}
};

// Output Stream override
template <int Code>
std::ostream &	operator << ( std::ostream & o, HTTPStatus<Code> const & i )
{
	o << i.code << ": " << i.reason_phrase;
	return o;
}

#endif /* __HTTP_STATUS_H__ */
