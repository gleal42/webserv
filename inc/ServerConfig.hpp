/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 00:50:11 by fmeira            #+#    #+#             */
/*   Updated: 2022/09/07 17:42:54 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include "types.hpp"
#include "webserver.hpp"
// #include "BaseConfig.hpp"
// #include "LocationConfig.hpp"

class ServerConfig : public BaseConfig
{
    public:
        ServerConfig();
        ~ServerConfig();
        ServerConfig(const ServerConfig&);
        ServerConfig& operator= (const ServerConfig&);

        bool                            is_empty( void );
        // Setters
        int                             find_directive(const std::string &directive);
        void                            set_directive(int directive, const std::string &content);
        void                            set_listen(bool has_separators, const std::string &content);
        void                            set_server_name(const std::string &content);
        void                            set_location(const std::string &name, const LocationConfig& location);

        // Getters
        const std::string               &get_ip( void ) const;
        int                             get_port( void ) const;
        const std::vector<Listen>&      get_listens( void ) const;
        const Locations&                      get_locations( void ) const;
        const std::vector<std::string>& get_server_names( void ) const;

        Locations                   _locations;
    private:
        std::vector<Listen>         _listens;
        std::vector<std::string>    _server_names;
};

std::ostream&	operator<<(std::ostream&, ServerConfig&);
std::ostream&	operator<<(std::ostream&, const CGI&);


#endif
