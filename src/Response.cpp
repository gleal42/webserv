/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 01:05:43 by gleal             #+#    #+#             */
/*   Updated: 2022/07/22 18:26:46 by gleal            ###   ########.fr       */
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
	_status = rhs._status;
	_body = rhs._status;
	for (ResponseHeaders::iterator it = _headers.begin(); it != _headers.end(); it++) {
		_headers = rhs._headers;
	}
	return *this;
}

Response::~Response( void ) { /* no-op */ }

// need to incorporate HTTPS Statuses here
std::string Response::start_line(int status)
{
	std::string http_version = "HTTP/1.1";

	std::stringstream nbr;
	nbr << status;
	std::string status_str = nbr.str();
	std::string status_message = http_phrase(status);
	return(http_version + " " + status_str + " " + status_message + "\n");
}

void	Response::send_response(Socket const & socket)
{
	if (_message.empty())
	{
		_message = start_line(_status);
		for (attributes_iterator it = _headers.begin(); it != _headers.end(); ++it) {
			_message += it->first + ": " + it->second + CRLF;
		}
		_message += CRLF + _body;
	}
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

void	Response::set_content_length(int length)
{
	_content_length = length;
}
void	Response::set_content_type(std::string const & type)
{
	_content_type = type;
}

void	Response::set_body(std::string const & body)
{
	_body = body;
}
