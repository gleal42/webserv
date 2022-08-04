/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:09:11 by msousa            #+#    #+#             */
/*   Updated: 2022/08/31 16:22:03 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

#include <iostream>
#include <string>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <map>
#include <vector>

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
std::string 		full_path(const std::string &relative_path);
std::vector<char>	convert_to_char_vector(const std::string &string);
std::string			filename(const std::string &path);
std::string			remove_query_string(const std::string &uri);
std::string			get_query_string(const std::string &uri);

template <typename T>
void	print_data(T val)
{
	std::cout << val.data() << std::endl;
}

template <typename T>
void	print(T val)
{
	std::cout << val.data() << std::endl;
}

#endif
