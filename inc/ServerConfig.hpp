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

// Header should be object instead of string:
// header.contentType
// header.etc..

class ServerConfig
{
	private:
		std::string			_header;
		int					_port;
	public:
		ServerConfig();
		ServerConfig(const char *config);
		ServerConfig(const ServerConfig &config);
		~ServerConfig();
		ServerConfig &operator=(const ServerConfig &config);
		const std::string getHeader() const;
		int	getPort();
};

#endif
