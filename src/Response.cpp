/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 01:05:43 by gleal             #+#    #+#             */
/*   Updated: 2022/06/22 21:34:56 by msousa           ###   ########.fr       */
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
	return(http_version + " " + status_str + " " + status_message + "\n");
}

void	Response::send_response(Socket const & socket)
{
	std::ifstream body("index.html");
	if ( (body.rdstate() & std::ifstream::failbit ) != 0
    || (body.rdstate() & std::ifstream::badbit ) != 0 )
    {
        std::cerr << "error opening " << "index.html" << std::endl;
		// send_error(404);
		return ;
    }
	std::string message = start_line(200);
	message += "Server: Hello\n";
	std::stringstream body_str;
	body_str << body.rdbuf();
	std::stringstream len;
	len << body_str.str().size();
	message += "Content-Length: " + len.str() + "\n";
	message += "Content-Type: text/html\n\n";
	message += body_str.str();
	((Socket)socket).send(message);
    printf("\n------------------Hello message sent-------------------\n");
}

