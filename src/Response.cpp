/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 01:05:43 by gleal             #+#    #+#             */
/*   Updated: 2022/07/23 17:09:48 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Socket.hpp"
#include "HTTPStatus.hpp"

Response::Response( void ) { /* no-op */ }

Response::Response( Response const & src ){
	// TODO (implement constructor)
	*this = src;
}

Response &	Response::operator = ( Response const & rhs )
{
	_uri = rhs._uri;
	_message = rhs._message;
	_body = rhs._body;
	_headers.clear();
	for (ResponseHeaders::iterator it = _headers.begin(); it != _headers.end(); it++) {
		_headers = rhs._headers;
	}
	return *this;
}

Response::~Response( void ) { /* no-op */ }

// need to incorporate HTTPS Statuses here
std::string Response::start_line( BaseStatus &status )
{
	std::string http_version = "HTTP/1.1";

	std::stringstream nbr;
	nbr << status.code;
	std::string status_str = nbr.str();
	std::string status_message = status.reason_phrase;
	return(http_version + " " + status_str + " " + status_message + CRLF);
}

void	Response::build_message( BaseStatus status )
{
	if (_message.empty())
	{
		_message = start_line(status);
		for (attributes_iterator it = _headers.begin(); it != _headers.end(); ++it) {
			_message += it->first + ": " + it->second + CRLF;
		}
		_message += CRLF + _body;
	}
}

void	Response::send_response( Socket const & socket )
{
	int sent_chars = socket.send(_message);
	int msg_size = _message.size();
	std::cout << "Message size before is: " << msg_size << std::endl;
	std::cout << "Sent size before is: "<< sent_chars << std::endl;
	_message = _message.substr(sent_chars, msg_size - sent_chars);
	std::cout << "Message size after is: " << _message.size() << std::endl;
    printf("\n------------------Hello message sent-------------------\n");
}
// Note that the last += statement sends \r\n. This has the effect of transmitting
// a blank line. This blank line is used by the client to delineate the HTTP
// header from the beginning of the HTTP body.

void	Response::set_attribute(std::string name, std::string value)
{
	_headers[name] = value;
}

bool	Response::is_empty()
{
	return (_message.empty());
}

void	Response::set_body(std::string const & body)
{
	_body = body;
}

void	Response::set_default_body( void )
{
	this->set_attribute("Content-Type", "text/html");
	std::string body("<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n</head>\n<body>\n<h1>Success!! 😀</h1>\n</body>\n</html>");
	this->set_body(body.c_str());
	std::stringstream len;
	len << body.size();
	this->set_attribute("Content-Length", len.str());
}

// Duplicated code in service_client_download()

void	Response::set_error_body( int error_code )
{
	std::stringstream to_str;
	std::string error_str;
	to_str << error_code;
	to_str >> error_str;

	error_str = "www/error_pages/" + error_str + ".html";

	std::ifstream file;
	file.open(error_str.c_str(), std::ios::binary);
	if ( (file.rdstate() & std::ifstream::failbit ) != 0
		|| (file.rdstate() & std::ifstream::badbit ) != 0 )
	{
		ERROR("error opening " << error_str.c_str());
		throw std::runtime_error("Can't open default error file");
	}

	this->set_attribute("Content-Type", ".html");
	std::stringstream body;
	body << file.rdbuf();
	this->set_body(body.str());
	std::stringstream len;
	len << body.str().size();
	this->set_attribute("Content-Length", len.str());

	file.close();
}