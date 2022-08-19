/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 14:45:30 by gleal             #+#    #+#             */
/*   Updated: 2022/08/19 22:42:11 by gleal            ###   ########.fr       */
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
# include "file.hpp"
# include "print.hpp"

typedef std::map<std::string, std::string> CGIExtInterpreter;

class CGIHandler : public Handler {

public:

	CGIHandler( const URI &uri );
	CGIHandler( CGIHandler const & src );
	~CGIHandler( void );
	CGIHandler &	operator = ( CGIHandler const & rhs );
	void				do_GET( Request & req, Response & res );
	void				do_POST( Request & req, Response & res );
	void				do_DELETE( Request & req, Response & res );
	static bool			extension_is_implemented( const std::string &extension );


private:

	static CGIExtInterpreter create_extension_pairs( void );
	static CGIExtInterpreter extension_interpreter;

	std::string path;
	std::string query_string;
	std::string extension;
	std::string interpreter;


	CGIHandler( void );
	void			set_response( std::string body, Response &res );
	void			execute_cgi_script( Request & req, Response & res  );
	std::vector< std::vector<char> >	environment_variables( Request & req );
	void			set_env( std::vector< std::vector<char> > &buf, const std::string &var, const std::string & value);
	void			convert_to_charptr_vec(std::vector< std::vector<char> > &vec_of_vec, std::vector<char *> &vec_of_charptr);
};

#endif /* __CGI_HANDLER_H__ */
