#ifndef __URI_H__
# define __URI_H__

#include "types.hpp"

class URI {

public:

	URI( void );
	std::string		host;
	int				port;
	std::string		path;
	std::string		extra_path;
	std::string		query; // map
	std::string		fragment;
	bool			cgi_confirmed;
    bool			autoindex_confirmed;
    bool			redirect_confirmed;

	std::string		to_s( void );
	void			clear( void );
};

#endif // __URI_H__
