/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:09:11 by msousa            #+#    #+#             */
/*   Updated: 2022/06/22 21:05:17 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

#include <iostream>
#include <string>

#include "macros.hpp"
#include "ServerConfig.hpp"
#include "Socket.hpp"
#include "Parser.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"

// Functions
int webserver(std::string config);

#endif
