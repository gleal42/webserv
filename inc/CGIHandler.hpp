/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 20:36:12 by gleal             #+#    #+#             */
/*   Updated: 2022/07/18 14:37:53 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CGI_HANDLER_H__
# define __CGI_HANDLER_H__

# include "Request.hpp"
# include "Response.hpp"
# include "HTTPStatus.hpp"

/* -------------------------------------------------------------------------- */
/*                              CGIHandler Class                              */
/* -------------------------------------------------------------------------- */

class CGIHandler {

public:

	CGIHandler( void );
	CGIHandler( CGIHandler const & src );
	~CGIHandler( void );
	CGIHandler &	operator = ( CGIHandler const & rhs );

	void				client_download(Request & req, Response & res);

private:

	std::string const	get_content_type(std::string const path);
	std::streampos		file_size( std::string full_path );

};

#endif /* __FILE_HANDLER_H__ */
