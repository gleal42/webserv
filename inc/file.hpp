/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 19:21:06 by gleal             #+#    #+#             */
/*   Updated: 2022/09/11 18:14:55 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FILE_H__
# define __FILE_H__

# include <string>
# include <map>
# include <set>
# include "webserver.hpp"
# include "HTTPStatus.hpp"

// # include "Response.hpp"

namespace file
{
	std::string const	content_type( const std::string & path );
	std::streampos		size( const std::string & full_path );
	long				size( FILE *open_file );
	void				save( const std::string & file_body, const std::string & file_name );
	void				remove( const std::string & file_name );
	std::string			get_string( FILE *file_ptr, int file_fd );
	Indexes_cit		find_valid_index( const std::string & root, const StringVector & indexes );
}


#endif /* __FILE_H__ */
