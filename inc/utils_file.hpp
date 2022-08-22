/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_file.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 19:21:06 by gleal             #+#    #+#             */
/*   Updated: 2022/08/21 14:38:16 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_FILE_H__
# define __UTILS_FILE_H__

# include <string>
# include <map>
# include <set>
# include "webserver.hpp"
# include "Response.hpp"

namespace file
{
	std::string const	content_type( const std::string & path );
	void				build_error_page( const BaseStatus &error_status, Response &res );
	std::streampos		size( const std::string &full_path );
	long				size( FILE *open_file );
	void				save( const std::string &file_body, std::string & filename );
	void				remove( const std::string & filename );
	std::string			get_string(FILE *file_ptr, int file_fd );
	std::vector<std::string>::const_iterator find_valid_index(const std::string &root, const std::vector<std::string> &indexes );
}


#endif /* __UTILS_FILE_H__ */
