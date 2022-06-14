/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 01:05:43 by gleal             #+#    #+#             */
/*   Updated: 2022/06/14 01:29:10 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(void)
{
	_start_line =  "HTTP/1.1 200 OK";
	_fields.push_back("Server: Hello");

	std::ifstream body("index.html");
	if ( (body.rdstate() & std::ifstream::failbit ) != 0
    || (body.rdstate() & std::ifstream::badbit ) != 0 )
    {
        std::cerr << "error opening " << "index.html" << std::endl;
        return ;
    }
	std::stringstream body_str;
	body_str << body.rdbuf();
	_body = body_str.str();
	
	std::string temp = "Content-Length: ";
	std::stringstream len;
	len << body_str.str().size();
	temp += len.str();
	_fields.push_back(temp);
	temp = "Content-Type: text/html";
	_fields.push_back(temp);
}