#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

#include "types.hpp"
#include "webserver.hpp"

class LocationConfig : public BaseConfig {

public:
	LocationConfig( void );
	~LocationConfig( void );
	LocationConfig( const LocationConfig & );
	LocationConfig & operator= ( const LocationConfig & );

	bool	is_empty( void );

	// Setters
	int		find_directive(const std::string &directive);
	void	set_directive(int directive, const std::string &content);
	void	set_cgi(bool has_separators, const std::string &content);
	void	set_limit_except(const std::string &content);

	// Getters
	const CGI &				get_cgi( void ) const;
	bool					has_cgi( const std::string & ) const;
	const StringVector &	get_limit_except( void ) const;

private:
	CGI				_cgi;
	StringVector	_limit_except;

};

#endif
