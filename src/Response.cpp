/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 01:05:43 by gleal             #+#    #+#             */
/*   Updated: 2022/07/11 16:28:08 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response( void ) { /* no-op */ }

Response::Response(const ServerConfig & config){
	// TODO (implement constructor)
	// set member vars from config
	(void)config;
}

Response::~Response( void ) { /* no-op */ }

std::string Response::start_line(int status)
{
	std::string http_version = "HTTP/1.1";

	std::stringstream nbr;
	nbr << status;
	std::string status_str = nbr.str();
	std::string status_message = "OK"; // Find matching message to status in map
	return(http_version + " " + status_str + " " + status_message + CRLF);
}

void	Response::send_response(Socket & socket)
{
	(void)socket;
	std::string	message = start_line(200);
	std::stringstream len;
	len << _content_length;
	// use header object for this
	message += "Server: Hello\r\n";
	message += "Content-Type: " + _content_type + CRLF;
	// message += "Connection: keep-alive\r\n";
	message += CRLF;
	// We can't be reading the whole file into memory before sending
	message += body + CRLF;
	socket.send(message);
	printf("\n------------------ message sent -------------------\n");
}
// Note that the last += statement sends \r\n. This has the effect of transmitting
// a blank line. This blank line is used by the client to delineate the HTTP
// header from the beginning of the HTTP body.

void	Response::set_content_length(int length) { _content_length = length; }
void	Response::set_content_type(std::string type) { _content_type = type; }

