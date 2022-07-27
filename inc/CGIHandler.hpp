/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 14:45:30 by gleal             #+#    #+#             */
/*   Updated: 2022/07/27 19:44:19 by gleal            ###   ########.fr       */
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

	void	execute_cgi_script( Request & req, Response & res  );
	void	set_environment_variables( std::vector<char *> &buf,  Request & req );
};

#endif /* __CGI_HANDLER_H__ */
