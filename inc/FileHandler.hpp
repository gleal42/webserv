/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 22:25:56 by msousa            #+#    #+#             */
/*   Updated: 2022/07/22 18:39:51 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FILE_HANDLER_H__
# define __FILE_HANDLER_H__

# include "Request.hpp"
# include "Response.hpp"
# include "HTTPStatus.hpp"

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
	void				save_file( std::string &body, std::string filename );
	void				default_response( Response & res );

	std::string			get_form_type( void );


	void				set_form_type( const std::string &content_type );
	void				set_delimiter( const std::string &content_type );

private:

	std::string const	get_content_type( std::string const path );
	std::streampos		file_size( std::string full_path );

	// POST

	void				set_content_type(Request & reqo);
	std::string			parse_file_name( const std::string &body, size_t next_delimiter );

	std::string			content_type;
	std::string 		form_type;
	std::string 		delimiter;

};

#endif /* __FILE_HANDLER_H__ */
