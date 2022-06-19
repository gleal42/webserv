/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 01:05:43 by gleal             #+#    #+#             */
/*   Updated: 2022/06/18 00:47:37 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(const ServerConfig &config, const Request &request)
{
	(void)config;
	(void)request;
}

std::string Response::start_line(int status)
{
	std::string http_version = "HTTP/1.1";

	std::stringstream nbr;
	nbr << status;
	std::string status_str = nbr.str();
	std::string status_message = "OK"; // Find matching message to status in map
	return(http_version + " " + status_str + " " + status_message + "\n");
}

void Response::send(int socketfd)
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
	write(socketfd, message.c_str() , message.size());
    printf("\n------------------Hello message sent-------------------\n");
}

