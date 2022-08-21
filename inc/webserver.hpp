/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:09:11 by msousa            #+#    #+#             */
/*   Updated: 2022/08/21 02:20:42 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

#include <iostream>
#include <string>
// #include <sys/event.h>
#include <vector>
#include <map>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <cstring>
#include <exception>

#include <fstream>
#include <cstdlib> // For EXIT_FAILURE, etc, macros
#include <sstream>

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
std::string 		full_path(const std::string &relative_path);
std::vector<char>	convert_to_char_vector(const std::string &string);
std::string			filename(const std::string &path);
std::string			remove_query_string(const std::string &uri);
std::string			get_query_string(const std::string &uri);
std::string			to_string(int number);
bool				is_directory(const std::string &path);
bool				is_file(const std::string &path);
struct addrinfo *	get_host(const std::string &hostname );
bool				is_address_being_listened(const std::string & listener_address, const struct sockaddr_in *req_host);
void				print_address(const std::string &name, struct sockaddr *address);
void				remove_directory(std::string &path);

template <typename T>
T	str_to_nbr(const std::string &str)
{
	std::stringstream ss(str);
	T nbr;
	ss >> nbr;
	return (nbr);
}

#endif
