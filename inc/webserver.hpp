/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:09:11 by msousa            #+#    #+#             */
/*   Updated: 2022/07/28 15:49:41 by gleal            ###   ########.fr       */
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

#include "macros.hpp"

typedef std::map<int, std::string> HTTPStatuses;

enum HTTPStatusGroup {
	STATUS_INVALID,
	STATUS_INFO,
	STATUS_SUCCESS,
	STATUS_REDIRECT,
	STATUS_CLIENT_ERROR,
	STATUS_SERVER_ERROR,
};

// Didn't work to include all header files here, if someone wants to give it
// a shot whilst still being able to `make`

// Functions
int 				webserver(std::string config);
std::string const	http_phrase( int code );
HTTPStatusGroup		http_group( int code );
std::string 		get_extension( const std::string &filename );
std::string			to_string( int number );
int					str_to_hexa(std::string hexa_nbr);

template <typename T>
void	print(T val)
{
	std::cout << val << std::endl;
}

struct copy_convert_vector
{
public:
	copy_convert_vector(std::vector<char *> &empty_convert);
	void operator()(std::vector<char> word);
private:
	std::vector<char *> &converted;
};

#endif
