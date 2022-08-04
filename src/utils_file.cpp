/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_file.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 19:27:09 by gleal             #+#    #+#             */
/*   Updated: 2022/08/04 20:21:38 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils_file.hpp"

// Maybe better to be part of Response 

namespace file
{
	typedef std::map<std::string, std::string> MimeTypes;

	std::string const get_content_type(std::string const path)
	{
		std::string extension = get_extension(path);

		// Turn this into a separate mime_type function with access to static map
		MimeTypes mime_types;

		mime_types[".css"] = "text/css";
		mime_types[".csv"] = "text/csv";
		mime_types[".gif"] = "image/gif";
		mime_types[".htm"] = "text/html";
		mime_types[".html"] = "text/html";
		mime_types[".ico"] = "image/x-icon";
		mime_types[".jpeg"] = "image/jpeg";
		mime_types[".jpg"] = "image/jpeg";
		mime_types[".js"] = "application/javascript";
		mime_types[".json"] = "application/json";
		mime_types[".png"] = "image/png";
		mime_types[".pdf"] = "application/pdf";
		mime_types[".svg"] = "image/svg+xml";
		mime_types[".txt"] = "text/plain";

		if (extension.size() && mime_types.find(extension) != mime_types.end()) {
			return mime_types[extension];
		}
		return "application/octet-stream";
	}

	void	build_error_page( const BaseStatus &error_status, Response &res )
	{
		std::string error_str = to_string(error_status.code);
    	error_str = "www/error_pages/" + error_str + ".html";
        res.set_with_file(error_str);
        res.build_message(error_status);
	}
}
