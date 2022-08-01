/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 00:50:11 by fmeira            #+#    #+#             */
/*   Updated: 2022/07/27 20:28:47 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

// Taken from Server Config Parsing.
// Each server will have these things

struct LocationConfig
{
    std::string                                         root;
    bool                                                autoindex;
    std::map<std::string, std::vector<int> >            error_pages;
    int                                                 client_max_body_size;
    std::vector<std::string>                            indexes;
    std::vector<std::string>                            limit_except;
    std::string                                         cgi_bin;
	std::map<std::string, std::vector<std::string> >    cgi_map;
};

typedef std::map<std::string, LocationConfig> Locations;

struct ServerConfig
{
    std::string                                 root;
    bool                                        autoindex;
    std::map<std::string, std::vector<int> >    error_pages;
    int                                         client_max_body_size;
    std::vector<std::string>                    indexes;
    std::string                                 ip;
    int                                         port;
    std::string                                 server_name;
    Locations                                   locations;
};

typedef std::vector<ServerConfig> Configs;

#endif
