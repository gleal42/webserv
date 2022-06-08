/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 22:25:02 by gleal             #+#    #+#             */
/*   Updated: 2022/06/08 01:48:29 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConf.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

ServerConf::ServerConf()
{
	// default confs
    port = 8080;
    header = "Hello from server";
}

ServerConf::ServerConf(const char *config)
{
	// parse conf file/analyze
	(void)config;
    port = 8080;
    header = "HTTP/1.1 200 OK\n";
	header += "Server: Hello\n";

	std::ifstream body("index.html");
	if ( (body.rdstate() & std::ifstream::failbit ) != 0
    || (body.rdstate() & std::ifstream::badbit ) != 0 )
    {
        std::cerr << "error opening " << "index.html" << std::endl;
        return ;
    }
	std::stringstream body_str;
	body_str << body.rdbuf();
	header += "Content-Length: ";
	std::stringstream len;
	len << body_str.str().size();
	header += len.str();
	header += "\nContent-Type: text/html\n\n";
	header += body_str.str();
}

ServerConf::ServerConf(const ServerConf &conf)
{
	*this = conf;
}

ServerConf::~ServerConf()
{
}

ServerConf &ServerConf::operator=(const ServerConf &conf)
{
	header = conf.header;
	port = conf.port;
	return (*this);
}

const std::string ServerConf::getHeader() const
{
	return (header);
}

int	ServerConf::getPort()
{
	return (port);
}
