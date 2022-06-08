/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 21:16:23 by gleal             #+#    #+#             */
/*   Updated: 2022/06/07 22:56:17 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONF_HPP
# define SERVERCONF_HPP

#include <string>

class ServerConf
{
	private:
		std::string			header;
		int					port;
	public:
		ServerConf();
		ServerConf(const char *config);
		ServerConf(const ServerConf &conf);
		~ServerConf();
		ServerConf &operator=(const ServerConf &conf);
		const std::string getHeader() const;
		int	getPort();
};

#endif
