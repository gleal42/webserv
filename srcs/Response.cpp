/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 01:05:43 by gleal             #+#    #+#             */
/*   Updated: 2022/06/16 04:55:47 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(void)
{
	http_version = "HTTP/1.1";
	status_code = "200";
	status_message = "OK";

	addField("Server", "Hello");
	std::ifstream body("index.html");
	if ( (body.rdstate() & std::ifstream::failbit ) != 0
    || (body.rdstate() & std::ifstream::badbit ) != 0 )
    {
        std::cerr << "error opening " << "index.html" << std::endl;
        return ;
    }
	std::stringstream body_str;
	body_str << body.rdbuf();
	setBody(body_str.str());

	std::stringstream len;
	len << body_str.str().size();
	addField("Content-Length", len.str());
	addField("Content-Type", "text/html");
}


	// _start_line =  "HTTP/1.1 200 OK";

	// _fields.push_back("Server: Hello");

	// std::ifstream body("index.html");
	// if ( (body.rdstate() & std::ifstream::failbit ) != 0
    // || (body.rdstate() & std::ifstream::badbit ) != 0 )
    // {
    //     std::cerr << "error opening " << "index.html" << std::endl;
    //     return ;
    // }
	// std::stringstream body_str;
	// body_str << body.rdbuf();
	// _body = body_str.str();
	
	// std::string temp = "Content-Length: ";
	// std::stringstream len;
	// len << body_str.str().size();
	// temp += len.str();
	// _fields.push_back(temp);
	// temp = "Content-Type: text/html";
	// _fields.push_back(temp);

const std::string Response::getStartLine() const
{
	std::string str = http_version + " " + status_code + " " + status_message;
	return str;
}

void Response::send(int socketfd)
{
	std::string str = http_version + " " + status_code + " " + status_message;
	std::map<std::string, std::string> fields = getAllFields();
	str += "\n";
	str += "Server: ";
	str += getField("Server");
	str += "\n";
	str += "Content-Length: ";
	str += getField("Content-Length");
	str += "\n";
	str += "Content-Type: ";
	str += getField("Content-Type");
	
/* 	for (std::map<std::string, std::string>::iterator it = fields.begin(); it != fields.end(); it++)
	{
		str += "\n";
		str += it->first;
		str += ": ";
		str += it->second;
	} */
	str += "\n\n";
	str += getBody();
	write(socketfd, str.c_str() , str.size());
}

