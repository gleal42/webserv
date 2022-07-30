/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 14:45:30 by gleal             #+#    #+#             */
/*   Updated: 2022/07/30 17:02:52 by gleal            ###   ########.fr       */
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

class CGIHandler : public Handler {

public:

	CGIHandler( void );
	CGIHandler( CGIHandler const & src );
	~CGIHandler( void );
	CGIHandler &	operator = ( CGIHandler const & rhs );
	void				do_GET( Request & req, Response & res );
	void				do_POST( Request & req, Response & res );
	void				do_DELETE( Request & req, Response & res );

private:

	BaseStatus		set_response( std::string bdy, Response &res );
	void	execute_cgi_script( Request & req, Response & res  );
	std::vector<std::vector <char> >	environment_variables( Request & req );
	void	setenv( std::vector<std::vector <char> > &buf,  const char * var, const char * value);
	void	convert_to_charptr(std::vector<std::vector <char> > &vec_of_vec, std::vector<char *> &vec_of_charptr);
};

#endif /* __CGI_HANDLER_H__ */
