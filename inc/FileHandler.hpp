/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 22:25:56 by msousa            #+#    #+#             */
/*   Updated: 2022/07/22 18:27:34 by gleal            ###   ########.fr       */
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

	void				service( Request & req, Response & res );
	void				save_file( Request & req, Response & res );

private:

	std::string const	get_content_type( std::string const path );
	std::streampos		file_size( std::string full_path );

};

#endif /* __FILE_HANDLER_H__ */
