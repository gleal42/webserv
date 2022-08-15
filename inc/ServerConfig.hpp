/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 00:50:11 by fmeira            #+#    #+#             */
/*   Updated: 2022/08/15 19:52:27 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>

# define PORT_MAX   65535
# define PORT_MIN   1

// Taken from Server Config Parsing.
// Each server will have these things

enum autobool{
    off,
    on,
    unset
};

typedef std::map<std::string, std::vector<int> > errorPage;

class BaseConfig{
    public:
    // Constructor
        BaseConfig();
    // Destructor
        virtual ~BaseConfig();

    // Setters
    virtual void                    set_directive(int directive, std::string &content) = 0;
    void                            set_root(bool has_separators, std::string &content);
    void                            set_autoindex(std::string &content);
    void                            set_error_pages(std::string &content);
    void                            set_max_body_size(int has_separators,std::string &content);
    void                            set_indexes(const std::string &content);

    // Getters
    std::string                     get_root( void ) const;
    autobool                        get_autoindex( void ) const;
    errorPage                       get_error_pages( void ) const;
    int                             get_max_body_size( void ) const;
    std::vector<std::string>        get_indexes( void ) const;

    protected:
        std::string                 _root;
        autobool                    _autoindex;
        errorPage                   _error_pages;
        int                         _client_max_body_size;
        std::vector<std::string>    _indexes;

};

class LocationConfig : public BaseConfig
{
    public:
        bool                        is_empty( void );
        // Setters
        void                        set_directive(int directive, std::string &content);
        void                        set_cgi(std::string &content);
        void                        set_limit_except(const std::string &content);

        // Getters
        std::string                 get_cgi( void );
        std::vector<std::string>    get_limit_except( void );

    private:
        std::string                 _name;
        std::string                 _cgi;
        std::vector<std::string>    _limit_except;

};
typedef std::map<std::string, LocationConfig> Locations;


class ServerConfig : public BaseConfig
{
    public:
        ServerConfig();

        bool                        is_empty( void );

        // Setters
        void                        set_directive(int directive, std::string &content);
        void                        set_listen(int has_separators, const std::string &content);
        void                        set_server_name(const std::string &content);
        void                        set_location(const std::string &name, const LocationConfig& location);

        // Getters
		const std::string					&get_ip( void ) const;
		int									get_port( void ) const;
        const std::vector<std::string>		&get_server_name( void ) const;
		Locations							get_locations( void ) const;

    private:
        std::string                 _ip;
        int                         _port;
        std::vector<std::string>    _server_name;
        Locations                   _locations;
};

typedef std::vector<ServerConfig> Configs;

#endif
