/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 00:50:11 by fmeira            #+#    #+#             */
/*   Updated: 2022/07/22 00:50:13 by fmeira           ###   ########.fr       */
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

typedef struct s_locations
{
    std::string                                         _root;
    bool                                                _autoindex;
    std::map<std::string, std::vector<int> >            _error_pages;
    int                                                 _max_body_size;
    std::vector<std::string>                            _indexes;
    std::vector<std::string>                            _allowed_methods;
    std::string                                         _cgi_bin;
	std::map<std::string, std::vector<std::string> >    _cgi_map;
}       LocationConfig;

typedef std::map<std::string, LocationConfig> Locations;

typedef struct s_config
{
    std::string                                 _root;
    bool                                        _autoindex;
    std::map<std::string, std::vector<int> >    _error_pages;
    int                                         _max_body_size;
    std::vector<std::string>                    _indexes;
    std::string                                 _ip;
    int                                         _port;
    std::string                                 _server_name;
    Locations                                   _locations;
}       ServerConfig;

typedef std::vector<ServerConfig> Configs;

#endif
