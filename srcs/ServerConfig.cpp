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
}

ServerConfig::ServerConfig(const char *config)
{
	// parse conf file/analyze
	(void)config;
    _port = 8080;
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
	_port = conf._port;
	return (*this);
}

int	ServerConfig::getPort()
{
	return (_port);
}
