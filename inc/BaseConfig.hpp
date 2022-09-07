#ifndef BASECONFIG_HPP
# define BASECONFIG_HPP

#include "types.hpp"
#include "webserver.hpp"
#include "ConfigParser.hpp"

# define PORT_MAX   65535
# define PORT_MIN   1
# define CONTEXT_DIRECTIVES 8

class BaseConfig{
    public:
    // Constructor
        BaseConfig();
    // Destructor
        virtual ~BaseConfig();

    // Setters
    virtual int                     find_directive(const std::string &directive) = 0;
    virtual void                    set_directive(int directive, const std::string &content) = 0;
    void                            set_root(bool has_separators, const std::string &content);
    void                            set_autoindex(const std::string &content);
    void                            set_error_pages(const std::string &content);
    void                            set_max_body_size(bool has_separators, const std::string &content);
    void                            set_indexes(const std::string &content);
    void                            set_redirect(bool has_separators, const std::string &content);

    // Getters
    const std::string&              get_root( void ) const;
    AutoBool                        get_autoindex( void ) const;
    const ErrorPage&                get_error_pages( void ) const;
    long long                       get_max_body_size( void ) const;
    const std::vector<std::string>& get_indexes( void ) const;
    const std::vector<Redirect>&    get_redirect( void );

    protected:
        std::string                 _root;
        AutoBool                    _autoindex;
        ErrorPage                   _error_pages;
        long long                   _client_max_body_size;
        std::vector<std::string>    _indexes;
        std::vector<Redirect>       _redirect;
};

#endif
