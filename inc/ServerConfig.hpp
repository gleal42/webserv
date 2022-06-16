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

class ServerConfig
{
	private:
		std::string			_name;
		int					_port;
		int					_return;
	public:
		ServerConfig();
		ServerConfig(std::string &config);
		ServerConfig(const ServerConfig &config);
		~ServerConfig();
		ServerConfig &operator=(const ServerConfig &config);
		const std::string getName() const;
		int	getPort();
		int	getReturn();
};

#endif
