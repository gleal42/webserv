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

struct ServerConfig
{
	std::string		name;
	std::string		bind_address; // following pseudo-code
	int				port;
	int				max_clients;
	int				request_timeout;
	std::string		root;
	int				client_max_body_size;
};

#endif
