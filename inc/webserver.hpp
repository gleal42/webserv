/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:09:11 by msousa            #+#    #+#             */
/*   Updated: 2022/06/25 19:51:05 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <string>
# include <cstdlib> // For EXIT_FAILURE, etc, macros
# include <sstream>

# include "macros.hpp"
// Didn't work to include all header files here, if someone wants to give it
// a shot whilst still being able to `make`

// Functions
int 		webserver(std::string config);
std::string	to_string(int number);

#endif
