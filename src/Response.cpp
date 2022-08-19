/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 01:05:43 by gleal             #+#    #+#             */
/*   Updated: 2022/08/31 17:38:29 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Socket.hpp"
#include "HTTPStatus.hpp"
#include "file.hpp"

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
std::string Response::start_line( const BaseStatus &status )
{
	std::string http_version = "HTTP/1.1";

	std::stringstream nbr;
	nbr << status.code;
	std::string	status_str = nbr.str();
	std::string status_message = status.reason_phrase;
	return(http_version + " " + status_str + " " + status_message + CRLF);
}

void	Response::build_message( const BaseStatus &status )
{
	if (_message.empty())
	{
		_message = start_line(status);
		for (ResponseHeaders_it it = _headers.begin(); it != _headers.end(); ++it) {
			_message += it->first + ": " + it->second + CRLF;
		}
		_message += CRLF + _body;
	}
}

std::string		Response::message( void ) { return _message; }

void	Response::send_response( Socket const & socket )
{
	int sent_chars = socket.send(_message);
	int msg_size = _message.size();
	std::cout << "Message size before is: " << msg_size << std::endl;
	std::cout << "Message is: " << _message << std::endl;
	std::cout << "Sent size before is: "<< sent_chars << std::endl;
	_message = _message.substr(sent_chars, msg_size - sent_chars);
	std::cout << "Message size after is: " << _message.size() << std::endl;
    printf("\n------------------Hello message sent-------------------\n");
}
// Note that the last += statement sends \r\n. This has the effect of transmitting
// a blank line. This blank line is used by the client to delineate the HTTP
// header from the beginning of the HTTP body.

void	Response::set_header(const std::string &name, const std::string &value)
{
	_headers[name] = value;
	if (name == "Content-Length")
	{
		std::cout << "Setting Content-Length as " << value << std::endl;
	}
}

bool	Response::is_empty()
{
	return (_message.empty());
}

void	Response::set_body(std::string const & body)
{
	_body = body;
}

void	Response::set_default_page( void )
{
	this->set_header("Content-Type", "text/html");
	std::string body("<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n</head>\n<body>\n<h1>Success!! 😀</h1>\n</body>\n</html>");
	this->set_body(body.c_str());
	std::stringstream len;
	len << body.size();
	this->set_header("Content-Length", len.str());
}

void    Response::set_with_file( const std::string &filename )
{
	std::ifstream file(filename.c_str());
	if ( (file.rdstate() & std::ifstream::failbit ) != 0
		|| (file.rdstate() & std::ifstream::badbit ) != 0 )
	{
		ERROR("error opening " << filename.c_str());
		throw HTTPStatus<404>();
	}

	set_header("Content-Type", file::content_type(filename.c_str()));
	std::stringstream body;
	body << file.rdbuf();
	set_body(body.str());
	std::stringstream len;
	len << body.str().size();
	set_header("Content-Length", len.str());

	file.close();	
}

void	Response::clear( void )
{
	_uri.clear();
	_body.clear();
	_headers.clear();
	_message.clear();
}

void	Response::save_raw_headers( std::string headers )
{
	while  (1)
	{
		size_t next_header = headers.find(CRLF);
		std::string single_header = headers.substr(0, next_header);
		size_t separator = single_header.find(":");
		if (single_header.empty() == false
			&& separator == std::string::npos)
			throw HTTPStatus<400>();
		this->set_header(single_header.substr(0, separator), single_header.substr(separator+1));
		if (next_header == std::string::npos)
			break ;
		headers = headers.substr(next_header + 2);
	}
}

const std::string	Response::get_header_value(const std::string &name)
{
	if (_headers.count(name)) {
		return(_headers[name]);
	}
	return ("");
}

void	Response::delete_header( const std::string & name )
{
	std::cout << name << " header was deleted" << std::endl;
	_headers.erase(name);
}
