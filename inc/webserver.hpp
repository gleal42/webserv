/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:09:11 by msousa            #+#    #+#             */
/*   Updated: 2022/09/07 00:10:11 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <iostream>
# include <string>
# include <netdb.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <cstdlib> // For EXIT_FAILURE, etc, macros
# include <sstream>
# include <cstring>
# include <fstream>
# include <exception>
# include <map>
# include <vector>
# include <unistd.h>
# include <arpa/inet.h> // inet_ntop()
# include "macros.hpp"
# include "BaseStatus.hpp"
# include "types.hpp"
# include "print.hpp"
# include "ServerConfig.hpp"
# include "file.hpp"



// Functions
int 				webserver(const std::string &config);
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
bool				is_link(const std::string &path);
struct addrinfo *	get_host(const std::string &hostname );
bool				is_address_being_listened(const std::string & listener_address, const struct sockaddr_in *req_host);
void				remove_directory(std::string &path);
std::string			address_to_hostname(struct sockaddr *address);
std::string			b64decode(const std::string & encoded_string);
std::string			processed_root( const ServerConfig & server_conf, Location_const_it locations );
const std::string &	priority_directive( const std::string &server_directive, const std::string & location_directive );
const int &			priority_directive( const int &server_directive, const int & location_directive );
void				update_error_code(ErrorPage &dest_list, const std::string &err_path, unsigned short code);
Location_const_it	path_resolve( URI & uri, const ServerConfig & server_conf);
Location_const_it	location_resolve(const ServerConfig &server_block, const std::string & path);
void				cgi_path_resolve( URI & uri, Location_const_it locations);
void				directory_indexing_resolve( URI & uri, const std::string &root, const ServerConfig &server_conf, Location_const_it locations);

template <typename T>
T	str_to_nbr(const std::string &str)
{
	std::stringstream ss(str);
	T nbr;
	ss >> nbr;
	return (nbr);
}

struct equals
{
	equals(const std::string &ref);
	bool operator()(const std::string&val);
	std::string ref;
};

class URI {

public:
	std::string		host;
	int				port;
	std::string		path;
	std::string		extra_path;
	std::string		query; // map
	std::string		fragment;
	
	std::string		to_s( void ) {
		return std::string("http://") + host + std::string(":") + to_string(port) + path + query;
	}
};

#endif
