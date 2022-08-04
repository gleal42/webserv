/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 22:25:56 by msousa            #+#    #+#             */
/*   Updated: 2022/08/31 16:21:47 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FILE_HANDLER_H__
# define __FILE_HANDLER_H__

# include <stdexcept>
# include <cstdio>
# include <algorithm>
# include "utils_url.hpp"
# include "utils_file.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "HTTPStatus.hpp"
# include "Handler.hpp"
# include "HTTPStatus.hpp"
# include "types.hpp"

// ************************************************************************** //
//                               FileHandler Class                            //
// ************************************************************************** //

class FileHandler : public Handler {

public:

	FileHandler( void );
	FileHandler( FileHandler const & src );
	~FileHandler( void );
	FileHandler &	operator = ( FileHandler const & rhs );

private:

	void				do_GET( Request & req, Response & res );
	void				do_POST( Request & req, Response & res );
	void				do_DELETE( Request & req, Response & res );

	// GET
	std::string const	get_content_type( std::string const path );

	// POST
	void				post_multi_type_form( Request & req );
	void				post_form_urlencoded( Request & req );
	std::string			parse_from_multipart_form( const std::string parameter, const std::string &body, size_t next_delimiter );
	void				save_file( std::string &body, std::string filename );
	std::string			multipart_form_body( const std::string &multi_form );

	// DELETE
	void				delete_file( std::string filename );
	Parameters			params;
};

#endif /* __FILE_HANDLER_H__ */
