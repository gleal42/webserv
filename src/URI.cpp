#include "URI.hpp"
#include "webserver.hpp"


URI::URI( void )
{
	port = -1;
    cgi_confirmed = false;
    autoindex_confirmed = false;
    redirect_confirmed = false;
}

std::string		URI::to_s( void ) {
	return std::string("http://") + host + std::string(":") + to_string(port) + path + query;
}

void			URI::clear( void )
{
	host.clear();
	port = -1;
	path.clear();
	extra_path.clear();
	query.clear();
	fragment.clear();
    cgi_confirmed = false;
    autoindex_confirmed = false;
    redirect_confirmed = false;
}