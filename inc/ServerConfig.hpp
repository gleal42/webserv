/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 21:16:23 by gleal             #+#    #+#             */
/*   Updated: 2022/06/13 17:03:23 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include <string>

// Taken from Server Config Parsing.
// Each server will have these things

struct location
{
	/* data */
};


struct ServerConfig
{
	std::string					server_name;
	std::string					root;
	std::string					cgi_bin;
	std::string					error_page;
	std::vector<std::string>	allowed_methods;
	bool						auto_index;
	std::vector<std::string>	indexes;
	struct location				location;
	int							client_max_body_size;
	int							max_clients;
	int							request_timeout;
	std::string					bind_address; // following pseudo-code
};

#endif
