/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 22:25:56 by msousa            #+#    #+#             */
/*   Updated: 2022/07/22 23:08:34 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FILE_HANDLER_H__
# define __FILE_HANDLER_H__

# include "Request.hpp"
# include "Response.hpp"
# include "HTTPStatus.hpp"
# include <stdexcept>
# include <set>
# include <map>
# include <cstdio>
# include <algorithm>


typedef std::set<std::string>	Extensions;
typedef std::map<std::string, std::string>	Parameters;

// ************************************************************************** //
//                               FileHandler Class                            //
// ************************************************************************** //

class FileHandler {

public:

	FileHandler( void );
	FileHandler( FileHandler const & src );
	~FileHandler( void );
	FileHandler &	operator = ( FileHandler const & rhs );

	void				service_client_download( Request & req, Response & res );
	void				service_multi_type_form( Request & req );
	void				service_form_urlencoded( Request & req );
	void				decode_url( std::string & single_form);
	void				delete_file( std::string filename );

private:

	std::string const	get_content_type( std::string const path );
	std::streampos		file_size( std::string full_path );

	// POST

	std::string			parse_file_name( const std::string &body, size_t next_delimiter );
	std::string			parse_param_name( const std::string &body, size_t next_delimiter );
	std::string			parse_from_multipart_form( const std::string parameter, const std::string &body, size_t next_delimiter );
	void				save_file( std::string &body, std::string filename );
	std::string			multipart_form_body( const std::string &multi_form );

	Parameters params;
};

#endif /* __FILE_HANDLER_H__ */
