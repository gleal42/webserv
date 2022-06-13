/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 22:25:02 by gleal             #+#    #+#             */
/*   Updated: 2022/06/08 01:48:29 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

ServerConfig::ServerConfig()
{
	// default confs
    _port = 8080;
    _header = "Hello from server";
}

ServerConfig::ServerConfig(const char *config)
{
	// parse conf file/analyze
	(void)config;
    _port = 8080;
    _header = "HTTP/1.1 200 OK\n";
	_header += "Server: Hello\n";

	std::ifstream body("index.html");
	if ( (body.rdstate() & std::ifstream::failbit ) != 0
    || (body.rdstate() & std::ifstream::badbit ) != 0 )
    {
        std::cerr << "error opening " << "index.html" << std::endl;
        return ;
    }
	std::stringstream body_str;
	body_str << body.rdbuf();
	_header += "Content-Length: ";
	std::stringstream len;
	len << body_str.str().size();
	_header += len.str();
	_header += "\nContent-Type: text/html\n\n";
	_header += body_str.str();
}

ServerConfig::ServerConfig(const ServerConfig &conf)
{
	*this = conf;
}

ServerConfig::~ServerConfig()
{
}

ServerConfig &ServerConfig::operator=(const ServerConfig &conf)
{
	_header = conf._header;
	_port = conf._port;
	return (*this);
}

const std::string ServerConfig::getHeader() const
{
	return (_header);
}

int	ServerConfig::getPort()
{
	return (_port);
}
