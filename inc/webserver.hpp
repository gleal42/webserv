/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:09:11 by msousa            #+#    #+#             */
/*   Updated: 2022/08/18 18:28:57 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <iostream>
# include <sys/socket.h>
# include <sys/time.h>
# include <cstdlib> // For EXIT_FAILURE, etc, macros
# include <sstream>

# include "macros.hpp"
# include "BaseStatus.hpp"
# include "types.hpp"

// Functions
int 				webserver(std::string config);
std::string 		get_extension( const std::string &filename );
std::string			to_string( int number );
int					str_to_hexa(std::string hexa_nbr);
bool				is_directory(std::string &path);
bool				is_file(std::string &path);

#endif
