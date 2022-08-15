/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 00:50:11 by fmeira            #+#    #+#             */
/*   Updated: 2022/08/16 00:20:23 by fmeira           ###   ########.fr       */
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

enum AutoBool{
    AUTOINDEX_OFF,
    AUTOINDEX_ON,
    AUTOINDEX_UNSET
};

struct Listen{
    std::string                 ip;
    int                         port;
    bool                        is_set;
};

typedef std::map<std::string, std::vector<int> > ErrorPage;

class BaseConfig{
    public:
    // Constructor
        BaseConfig();
    // Destructor
        virtual ~BaseConfig();

    // Setters
    virtual int                     find_directive(std::string &directive) = 0;
    virtual void                    set_directive(int directive, std::string &content) = 0;
    void                            set_root(bool has_separators, std::string &content);
    void                            set_autoindex(std::string &content);
    void                            set_error_pages(std::string &content);
    void                            set_max_body_size(bool has_separators,std::string &content);
    void                            set_indexes(std::string &content);

    // Getters
    std::string&                    get_root( void );
    AutoBool                        get_autoindex( void );
    ErrorPage&                      get_error_pages( void );
    int                             get_max_body_size( void ) const;
    std::vector<std::string>&       get_indexes( void );

    protected:
        std::string                 _root;
        AutoBool                    _autoindex;
        ErrorPage                   _error_pages;
        int                         _client_max_body_size;
        std::vector<std::string>    _indexes;

};

class LocationConfig : public BaseConfig
{
    public:
        LocationConfig();
        ~LocationConfig();
        LocationConfig(const LocationConfig&);
        LocationConfig& operator= (const LocationConfig&);

        bool                        is_empty( void );
        // Setters
        int                         find_directive(std::string &directive);
        void                        set_directive(int directive, std::string &content);
        void                        set_cgi(bool has_separators, std::string &content);
        void                        set_limit_except(std::string &content);

        // Getters
        std::string&                get_cgi( void );
        std::vector<std::string>&   get_limit_except( void );

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
        int                         find_directive(std::string &directive);
        void                        set_directive(int directive, std::string &content);
        void                        set_listen(bool has_separators, std::string &content);
        void                        set_server_name(std::string &content);

        // Getters
        std::vector<Listen>&        get_listens( void );
        Locations&                  get_locations( void );
        std::vector<std::string>&   get_server_names( void );

        Locations                   _locations;
    private:
        std::vector<Listen>         _listens;
        std::vector<std::string>    _server_names;
};

typedef std::vector<ServerConfig> Configs;
std::ostream&   operator<<(std::ostream&, ServerConfig&);

typedef std::vector<ServerConfig> Configs;

#endif
