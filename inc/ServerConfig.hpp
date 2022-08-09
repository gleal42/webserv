/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 00:50:11 by fmeira            #+#    #+#             */
/*   Updated: 2022/08/09 21:11:13 by fmeira           ###   ########.fr       */
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
    AUTOINDEX_OFF,
    AUTOINDEX_ON,
    AUTOINDEX_UNSET
};

typedef std::map<std::string, std::vector<int> > errorPage;

class BaseConfig{
    public:
    // Constructor
        BaseConfig();
    // Destructor
        virtual ~BaseConfig();

    // Setters
    virtual int                     find_directive(std::string &content) = 0;
    virtual void                    set_directive(int directive, std::string &content) = 0;
    void                            set_root(bool has_separators, std::string &content);
    void                            set_autoindex(std::string &content);
    void                            set_error_pages(std::string &content);
    void                            set_max_body_size(int has_separators,std::string &content);
    void                            set_indexes(std::string &content);

    // Getters
    std::string                     get_root( void );
    autobool                        get_autoindex( void );
    errorPage                       get_error_pages( void );
    int                             get_max_body_size( void ) const;
    std::vector<std::string>        get_indexes( void );

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
        LocationConfig();
        ~LocationConfig();

        bool                        is_empty( void );
        // Setters
        int                         find_directive(std::string &content);
        void                        set_directive(int directive, std::string &content);
        void                        set_cgi(std::string &content);
        void                        set_limit_except(std::string &content);

        // Getters
        std::string                 get_cgi( void );
        std::vector<std::string>    get_limit_except( void );

    private:
        std::string                 _cgi;
        std::vector<std::string>    _limit_except;

};
typedef std::map<std::string, LocationConfig> Locations;

class ServerConfig : public BaseConfig
{
    public:
        ServerConfig();
        ~ServerConfig();
        ServerConfig(const ServerConfig&);
        ServerConfig& operator= (const ServerConfig&);

        bool                        is_empty( void );

        // Setters
        int                         find_directive(std::string &content);
        void                        set_directive(int directive, std::string &content);
        void                        set_listen(int has_separators, std::string &content);
        void                        set_server_name(std::string &content);

        // Getters
        std::string                 get_ip( void );
        int                         get_port( void );
        Locations                   get_locations( void );
        std::vector<std::string>    get_server_name( void );

    private:
        std::string                 _ip;
        int                         _port;
        std::vector<std::string>    _server_name;
        Locations                   _locations;
};

typedef std::vector<ServerConfig> Configs;
std::ostream&   operator<<(std::ostream&, ServerConfig&);

#endif
