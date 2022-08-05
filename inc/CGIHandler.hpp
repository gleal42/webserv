/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 14:45:30 by gleal             #+#    #+#             */
/*   Updated: 2022/08/05 01:04:23 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CGI_HANDLER_H__
# define __CGI_HANDLER_H__

# include <unistd.h>
# include <algorithm>

# include "Request.hpp"
# include "Response.hpp"
# include "HTTPStatus.hpp"
# include "Handler.hpp"
# include "webserver.hpp"
# include "utils_file.hpp"

typedef std::map<std::string, std::string> CGIExtInterpreter;

class CGIHandler : public Handler {

public:

	CGIHandler( const std::string &uri );
	CGIHandler( CGIHandler const & src );
	~CGIHandler( void );
	CGIHandler &	operator = ( CGIHandler const & rhs );
	void				do_GET( Request & req, Response & res );
	void				do_POST( Request & req, Response & res );
	void				do_DELETE( Request & req, Response & res );
	static bool			extension_is_implemented( const std::string &extension );
	BaseStatus 			script_status( void );

private:

	static CGIExtInterpreter create_extension_pairs( void );
	static CGIExtInterpreter extension_interpreter;

	std::string path;
	std::string query_string;
	std::string extension;
	std::string interpreter;
	int			status_code;

	CGIHandler( void );
	void			set_response( std::string bdy, Response &res );
	void			execute_cgi_script( Request & req, Response & res  );
	std::vector<std::vector <char> >	environment_variables( Request & req );
	void			setenv( std::vector<std::vector <char> > &buf,  const char * var, const char * value);
	void			convert_to_charptr(std::vector<std::vector <char> > &vec_of_vec, std::vector<char *> &vec_of_charptr);
};

#endif /* __CGI_HANDLER_H__ */
