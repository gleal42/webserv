/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 19:21:06 by gleal             #+#    #+#             */
/*   Updated: 2022/08/25 23:53:41 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FILE_H__
# define __FILE_H__

# include <string>
# include <map>
# include <set>
# include "webserver.hpp"
# include "Response.hpp"

namespace file
{
	std::string const	content_type( const std::string & path );
	std::streampos		size( const std::string &full_path );
	long				size( FILE *open_file );
	void				save( const std::string &file_body, const std::string & filename );
	void				remove( const std::string & filename );
	std::string			get_string(FILE *file_ptr, int file_fd );
}


#endif /* __FILE_H__ */
