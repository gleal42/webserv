/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_file.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 19:21:06 by gleal             #+#    #+#             */
/*   Updated: 2022/08/04 20:21:47 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_FILE_H__
# define __UTILS_FILE_H__

# include <string>
# include <map>
# include "webserver.hpp"
# include "Response.hpp"

namespace file
{
	std::string const get_content_type(std::string const path);
	void	build_error_page( const BaseStatus &error_status, Response &res );
}


#endif /* __UTILS_FILE_H__ */
