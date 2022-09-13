#include "webserver.hpp"
#include "ConfigParser.hpp"

// Constructor
LocationConfig::LocationConfig()
{};

// Destructor
LocationConfig::~LocationConfig()
{};

// Copy constructor
LocationConfig::LocationConfig(const LocationConfig& param)
{
    this->_root = param._root;
    this->_autoindex = param._autoindex;
    this->_error_pages = param._error_pages;
    this->_client_max_body_size = param._client_max_body_size;
    this->_indexes = param._indexes;
    this->_redirects = param._redirects;
    this->_cgi.extension = param._cgi.extension;
    this->_cgi.interpreter = param._cgi.interpreter;
    this->_limit_except = param._limit_except;
}

namespace {
    bool valid_method(std::string str)
    {
        return (str == "GET" || str == "POST" || str == "DELETE");}
}

//~~~~~~~~~~~~~~~~~~~~~~~~LocationConfig methods~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool	CGI::is_configured(const std::string &uri_extension) const
{
       if (uri_extension == extension)
               return true;
       return false;
}

bool	CGI::empty() const { return (extension.empty() && interpreter.empty());}

bool    LocationConfig::is_empty( void )
{
    return (this->_root.empty() && this->_autoindex == AUTOINDEX_UNSET
        && this->_error_pages.empty() && this->_client_max_body_size == -1
        && this->_indexes.empty() && this->_redirects.empty()
        && this->_limit_except.empty()
        && this->_cgi.extension.empty() && this->_cgi.interpreter.empty());
}

int     LocationConfig::find_directive(const std::string &directive)
{
    const std::string   valid_location_directives[CONTEXT_DIRECTIVES] =
    {"root", "autoindex", "error_page", "client_max_body_size", "index",
    "redirect", "cgi", "limit_except"};

    int                 i = -1;

    while (++i < CONTEXT_DIRECTIVES)
        if (directive == valid_location_directives[i])
        return (i);
    throw (ConfigurationDirectiveError(directive));
    return (0);
}


// LocationConfig setters

void LocationConfig::set_directive(int directive, const std::string& content)
{
    bool    has_separators = (content.find_first_of(SEPARATORS) != std::string::npos);
    if (content.empty())
        throw (ConfigurationSyntaxError());

    switch (directive)
    {
        case DIRECTIVE_ROOT:
            this->set_root(has_separators, content);
            break;
        case DIRECTIVE_AUTOINDEX:
            this->set_autoindex(content);
            break;
        case DIRECTIVE_ERRORPAGE:
            this->set_error_pages(content);
            break;
        case DIRECTIVE_MAXBODYSIZE:
            this->set_max_body_size(has_separators, content);
            break;
        case DIRECTIVE_INDEX:
            this->set_indexes(content);
            break;
        case DIRECTIVE_REDIRECT:
            this->set_redirect(has_separators, content);
            break;
        case DIRECTIVE_CGI:
         this->set_cgi(has_separators, content);
            break;
        case DIRECTIVE_LIMITEXCEPT:
            this->set_limit_except(content);
            break;
        default:
            break;
    }
}

void    LocationConfig::set_cgi(bool has_separators, const std::string &content)
{
    (void)has_separators;
    // if (has_separators)
	//     throw (MultipleArgumentsError(content));
    this->_cgi.extension = std::strtok(const_cast<char *>(content.c_str()), " ");
    this->_cgi.interpreter = std::strtok(NULL, " ");
    // if (!is_file(this->_cgi.interpreter) && !is_link(this->_cgi.interpreter))
    //     throw (BadFileError(this->_cgi.interpreter));
}

void    LocationConfig::set_limit_except(const std::string &content)
{
    char *token = std::strtok(const_cast<char *>(content.c_str()), " ");

    while (token)
    {
        if (!(valid_method(token)))
	        throw (ConfigurationDirectiveError(token));
        StringVector_it last = this->_limit_except.end();
        StringVector_it tmp = this->_limit_except.begin();
        for (; tmp != last; ++tmp)
            if (*tmp == token)
	            throw (ConfigurationDirectiveError(content));
        this->_limit_except.push_back(std::string(token));
        token = std::strtok(NULL, " ");
    }
}

// LocationConfig getters
const CGI&						    LocationConfig::get_cgi( void ) const {return (this->_cgi);}
bool                            	LocationConfig::has_cgi( const std::string &uri ) const
{
	return(_cgi.is_configured(get_extension(uri)));
}
const StringVector&   LocationConfig::get_limit_except( void ) const {return (this->_limit_except);}

// LocationConfig Operators

std::ostream&  operator<<(std::ostream&os, const CGI&cgi)
{
       os << "Extension [" << cgi.extension << "] Interpreter [" << cgi.interpreter << "]";
       return os;
}

LocationConfig& LocationConfig::operator= (const LocationConfig& param)
{
    if (this == &param)
        return (*this);

    this->_root = param._root;
    this->_autoindex = param._autoindex;
    this->_error_pages = param._error_pages;
    this->_client_max_body_size = param._client_max_body_size;
    this->_indexes = param._indexes;
    this->_redirects = param._redirects;
    this->_cgi = param._cgi;
    this->_limit_except = param._limit_except;

    return (*this);
}
