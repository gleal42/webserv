/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 19:38:07 by msousa            #+#    #+#             */
/*   Updated: 2022/09/10 19:59:56 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserver.hpp"
# include <unistd.h>
# include "HTTPStatus.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <algorithm>

std::string	to_string(int number)
{
	std::string			result;
	std::ostringstream	convert;
	convert << number;
	return convert.str();
}

std::string const	http_phrase( int code )
{
    static HTTPStatuses	statuses;

	if (!statuses.empty()) {
		return statuses[code];
	}

    statuses[100] = "Continue";
    statuses[101] = "Switching Protocols";
    statuses[200] = "OK";
    statuses[201] = "Created";
    statuses[202] = "Accepted";
    statuses[203] = "Non-Authoritative Information";
    statuses[204] = "No Content";
    statuses[205] = "Reset Content";
    statuses[206] = "Partial Content";
    statuses[207] = "Multi-Status";
    statuses[300] = "Multiple Choices";
    statuses[301] = "Moved Permanently";
    statuses[302] = "Found";
    statuses[303] = "See Other";
    statuses[304] = "Not Modified";
    statuses[305] = "Use Proxy";
    statuses[307] = "Temporary Redirect";
    statuses[400] = "Bad Request";
    statuses[401] = "Unauthorized";
    statuses[402] = "Payment Required";
    statuses[403] = "Forbidden";
    statuses[404] = "Not Found";
    statuses[405] = "Method Not Allowed";
    statuses[406] = "Not Acceptable";
    statuses[407] = "Proxy Authentication Required";
    statuses[408] = "Request Timeout";
    statuses[409] = "Conflict";
    statuses[410] = "Gone";
    statuses[411] = "Length Required";
    statuses[412] = "Precondition Failed";
    statuses[413] = "Request Entity Too Large";
    statuses[414] = "Request-URI Too Large";
    statuses[415] = "Unsupported Media Type";
    statuses[416] = "Request Range Not Satisfiable";
    statuses[417] = "Expectation Failed";
    statuses[422] = "Unprocessable Entity";
    statuses[423] = "Locked";
    statuses[424] = "Failed Dependency";
    statuses[426] = "Upgrade Required";
    statuses[428] = "Precondition Required";
    statuses[429] = "Too Many Requests";
    statuses[431] = "Request Header Fields Too Large";
    statuses[451] = "Unavailable For Legal Reasons";
    statuses[500] = "Internal Server Error";
    statuses[501] = "Not Implemented";
    statuses[502] = "Bad Gateway";
    statuses[503] = "Service Unavailable";
    statuses[504] = "Gateway Timeout";
    statuses[505] = "HTTP Version Not Supported";
    statuses[507] = "Insufficient Storage";
    statuses[511] = "Network Authentication Required";
    return statuses[code];
}

HTTPStatusGroup	http_group( int code )
{
	switch (code)
	{
	case 100 ... 199: return STATUS_INFO; break;
	case 200 ... 299: return STATUS_SUCCESS; break;
	case 300 ... 399: return STATUS_REDIRECT; break;
	case 400 ... 499: return STATUS_CLIENT_ERROR; break;
	case 500 ... 599: return STATUS_SERVER_ERROR; break;
	default: return STATUS_INVALID; break;
	};
}

static bool is_not_alphanum( int c )
{
    return (!std::isalnum(c));
}

std::string get_extension( const std::string &filename )
{
    std::string extension;
    size_t start_extension_pos = filename.find_last_of('.');
    if (start_extension_pos == std::string::npos )
        return (extension);
    extension = filename.substr(start_extension_pos);
    std::string::iterator end_extension_pos (std::find_if((extension.begin()+1), extension.end(), is_not_alphanum)); // Too restrictive?
    return (std::string(extension.begin(), end_extension_pos));
}

int	str_to_hexa(std::string hexa_nbr)
{
	std::stringstream ss;
	int x;
	ss << std::hex << hexa_nbr.c_str();
	ss >> x;
	return (x);
}

std::string full_path(const std::string &relative_path)
{
	char read_buf[200];
	memset(read_buf, '\0', sizeof(read_buf));
	std::string abs_path(getcwd(read_buf, 200));

    return (abs_path + '/' + relative_path);
}

std::vector<char>	convert_to_char_vector(const std::string &string)
{
	std::vector<char> char_vector(string.begin(), string.end());
	char_vector.push_back('\0');
	return (char_vector);
}

std::string	filename(const std::string &path)
{
	std::string script_filename (path.substr(path.find_last_of('/')));
	if (script_filename.empty())
		return (path);
	return (script_filename.c_str() + 1);
}

std::string remove_query_string(const std::string &uri)
{
    size_t query_string_start = uri.find('?');
    return (uri.substr(0, query_string_start));
}

std::string get_query_string(const std::string &uri)
{
    std::string query_string;

    size_t query_string_start = uri.find('?');
    if (query_string_start == std::string::npos)
        return std::string();
    return (uri.substr(query_string_start));
}

// For comparing with original files

// std::ifstream infile;
// infile.open("cute.jpeg", std::ios::binary);
// if ( (infile.rdstate() & std::ifstream::failbit ) != 0
// 	|| (infile.rdstate() & std::ifstream::badbit ) != 0 )
// {
// 	ERROR("error opening " << res._uri.c_str());
// 	throw HTTPStatus<404>();
// }
// std::stringstream temp;
// temp << infile.rdbuf();
// std::cout << "It should have size: [" << temp.str().size() << "]" << std::endl;
// infile.close();

bool is_directory(const std::string &path)
{
    struct stat s;

    if (lstat(path.c_str(), &s) == 0)
        if (S_ISDIR(s.st_mode))
            return (true);
    return (false);
}

bool is_file(const std::string &path)
{
    struct stat s;

    if (lstat(path.c_str(), &s) == 0)
        if (S_ISREG(s.st_mode))
            return (true);
    return (false);
}

bool is_link(const std::string &path)
{
    struct stat s;

    if (lstat(path.c_str(), &s) == 0)
        if (S_ISLNK(s.st_mode))
            return (true);
    return (false);
}

struct addrinfo *get_host(const std::string &hostname )
{
	struct addrinfo *host;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	int resolve_req_host = getaddrinfo(hostname.c_str(), NULL, &hints, &host);
	if (resolve_req_host != 0){
        if (resolve_req_host == EAI_NONAME)
            return NULL;
        throw HTTPStatus<500>();
    }
    if (host->ai_next != NULL){
        throw HTTPStatus<500>();
    }
	return host;
}

/**
 * Checks if interface to which Listeners is bound
 * (addresses being listened) include that of the request.
 * This is done by resolving the request hostname and checking
 * the address
 *
 * @param listener_address listen 'address':port (address part)
 * @param req_host Host: 'hostname':80 (hostname part)
 * @return Request was sent to an interface which this listener is bound to
 */

bool is_address_being_listened(const std::string & listener_address, const struct sockaddr_in *req_host)
{
	print_address("Req", (struct sockaddr *)req_host);
    struct addrinfo *listener_host = get_host(listener_address.c_str());
	print_address("Listener", listener_host->ai_addr);
	const struct sockaddr_in *listener_addr = (const struct sockaddr_in *)listener_host->ai_addr;
	if (listener_addr->sin_addr.s_addr == 0
		|| listener_addr->sin_addr.s_addr == req_host->sin_addr.s_addr)
	{
		freeaddrinfo(listener_host);
		return true;
	}
	freeaddrinfo(listener_host);
	return false;
}

void	remove_directory(std::string &path)
{
	size_t backslash_pos = path.find_last_of('/');
	if (backslash_pos == std::string::npos)
		path.clear();
	else
		path = path.substr(0, (backslash_pos+1));
}
std::string address_to_hostname(struct sockaddr *address)
{
    static char buf[NI_MAXHOST];
    std::memset(buf, '\0', NI_MAXHOST);

    getnameinfo(address, sizeof(*address), buf, sizeof(buf), NULL, 0, NI_NUMERICHOST);
    return(buf);
}

static unsigned int b64_decode_table(const unsigned char chr) {
    if		(chr >= 'A' && chr <= 'Z') return chr - 'A';
    else if (chr >= 'a' && chr <= 'z') return chr - 'a' + ('Z' - 'A')               + 1;
    else if (chr >= '0' && chr <= '9') return chr - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
    else if (chr == '+' || chr == '-') return 62;
    else if (chr == '/' || chr == '_') return 63;
    else if (chr == '=' || chr == '.') return 0;
    throw HTTPStatus<500>();
}

// Encoding and Decoding keeping the same binary representation while:
// Encoding -> Turning 3 chars into 4 chars
// Decoding -> Turning 4 chars into 3 chars
// three ASCII numbers 155, 162, and 233
// 10011011 10100010 11101001
// Encoding them would mean to do this:
// 100110 111010 001011 101001 (38 58 11 41)
// Which we can then translate to m6Lp if we use the Base64 Encoding Table
// https://www.lifewire.com/base64-encoding-overview-1166412
// In base64 chars have at most 6 bits so they have to be converted to 8 bits

std::string b64decode(const std::string& encoded_string)
{
    size_t length_of_string = encoded_string.length();
    size_t pos = 0;
    std::string ret;
    while (pos < length_of_string)
	{
		ret.push_back(
			((b64_decode_table(encoded_string[pos+0])) << 2) // Starting 6 bits taken from 1st char
		+	((b64_decode_table(encoded_string[pos+1]) & 0x30 ) >> 4)); // Last 2 bits taken from 2st char (110000)
       if (pos + 2 < length_of_string)
       {
			ret.push_back(
			((b64_decode_table(encoded_string[pos+1]) & 0x0f) << 4)	// Starting 4 bits taken from 2nd char (001111)
		+	((b64_decode_table(encoded_string[pos+2]) & 0x3c) >> 2)); // Last 4 bits taken from 3rd char (111100)
       }
	   if (pos + 3 < length_of_string)
	   {
			ret.push_back(
			((b64_decode_table(encoded_string[pos+2]) & 0x03 ) << 6 ) // Starting 2 bits taken from 3rd char (000011)
			+ b64_decode_table(encoded_string[pos+3])); // Last 6 bits taken from 4th char
	   }
       pos += 4;
    }
    return ret;
}

std::string processed_root( const ServerConfig & server_conf, Location_const_it locations )
{
	std::string root = priority_directive(server_conf.get_root(), locations->second.get_root());
	if (*(--root.end()) == '/')
		root.erase(--root.end());
    return (root);
}

const std::string &priority_directive( const std::string &server_directive, const std::string & location_directive )
{
    if (location_directive.empty() == false)
        return (location_directive);
    return (server_directive);
}

const int &priority_directive( const int &server_directive, const int & location_directive )
{
    if (location_directive >= 0)
        return (location_directive);
    return (server_directive);
}

equals::equals(const std::string &ref): ref(ref){}
bool equals::operator()(const std::string&val){ return (val == ref);}

void    update_error_code(ErrorPage &dest_list, const std::string &err_path, unsigned short code)
{
	for (ErrorPage_it it_dest_err = dest_list.begin();
		it_dest_err != dest_list.end();
		it_dest_err++)
	{
		std::vector<unsigned short>::iterator it_del = std::find(it_dest_err->second.begin(), it_dest_err->second.end(), code);
		if (it_del != it_dest_err->second.end())
			it_dest_err->second.erase(it_del);
	}
	dest_list[err_path].push_back(code);
}

std::string   get_error_path(const ErrorPage &dest_list, unsigned short code)
{
	for (ErrorPage_const_it it_dest_err = dest_list.begin();
		it_dest_err != dest_list.end();
		it_dest_err++)
	{
		std::vector<unsigned short>::const_iterator it_del = std::find(it_dest_err->second.begin(), it_dest_err->second.end(), code);
		if (it_del != it_dest_err->second.end())
			return(it_dest_err->first);
	}
    return (std::string());
}

Location_const_it	path_resolve( URI & uri, const ServerConfig & server_conf)
{
	Location_const_it locations = location_resolve(server_conf, uri.path);
	std::string root ("public" + processed_root( server_conf, locations ));
	std::string root_path = root + uri.path;
	if (is_directory(root_path)) {
		std::cout << "URI " << uri.path << " is dir\n";
		directory_indexing_resolve( uri, root_path, server_conf, locations);
		// if (uri.autoindex_confirmed == true) {
		// 	std::cout << "autoindex_confirmed for " << uri.path << "\n";
			// return locations;}
	}
	cgi_path_resolve(uri, locations);
	if (*uri.path.begin() != '/')
		uri.path.insert(uri.path.begin(), '/');
	root_path = root + uri.path;
	Location_const_it redir_locations = location_resolve(server_conf, uri.path);
	if (redir_locations->first.size() > locations->first.size())
		return (path_resolve(uri, server_conf));
	else {
		uri.path = root_path;
		return locations;
	}
}

Location_const_it      location_resolve(const ServerConfig &server_block, const std::string & path)
{
	std::string location_path = path;
	if ( location_path.size() > 0 && *(location_path.end()-1) != '/')
		location_path.push_back('/');
	const Locations &locations = server_block.get_locations();
	while (location_path.empty() == false)
	{
		for (Location_const_it it = locations.begin();
			it != locations.end();
			it++)
			{
				if ((it->first) == location_path)
					return (it);
			}
		location_path.erase(--location_path.end());
	}
	throw HTTPStatus<404>(); // may need to add default / location to match nginx behaviour
}

void			cgi_path_resolve( URI & uri, Location_const_it locations)
{
	if (uri.extra_path.empty() == false) {
		uri.path = uri.path + uri.extra_path;
		uri.extra_path.clear();
	}
	CGI cgi = locations->second.get_cgi();
	if (cgi.empty())
		return ;
	size_t script_path_pos = uri.path.find(cgi.extension);
	if (script_path_pos == std::string::npos)
		return ;
	script_path_pos = script_path_pos + cgi.extension.size();
	uri.extra_path = uri.path.substr(script_path_pos + 1);
	uri.path = uri.path.substr(0, script_path_pos);
}

// void			directory_indexing_resolve( URI & uri, const std::string &root, const ServerConfig &server_conf, Location_const_it locations)
// {
// 	Indexes indexes;
// 	indexes = locations->second.get_indexes();
// 	if (indexes.empty())
// 	{
// 		indexes = server_conf.get_indexes();
// 		if (indexes.empty())
// 		{
// 			if (is_file(root + "index.html"))
// 			{
// 			    uri.path = "index.html";
// 			    return ;
// 			}
// 			if ((locations->second).get_autoindex() == AUTOINDEX_ON)
// 				uri.autoindex_confirmed = true;
// 					return ;
// 			throw HTTPStatus<404>();
// 		}
// 		Index_const_it index = file::find_valid_index(root, indexes);
// 		if (index == indexes.end())
// 		{
// 			if ((locations->second).get_autoindex() == AUTOINDEX_ON)
// 				throw HTTPStatus<501>(); // Not implemented yet
// 			throw HTTPStatus<403>();
// 		}
// 		uri.path = (*index);
// 		return ;
// 	}
// 	Index_const_it index = file::find_valid_index(root, indexes);
// 	if (index == indexes.end())
// 		throw HTTPStatus<404>();
// 	uri.path = (*index);
// }

void			directory_indexing_resolve( URI & uri, const std::string &root, const ServerConfig &server_conf, Location_const_it locations)
{
	(void)server_conf;

	Indexes indexes;
	indexes = locations->second.get_indexes();
	if (indexes.empty())
	{
		if (is_file(root + "index.html"))
		{
		    uri.path = "index.html";
		    return ;
		}
		else if (locations->second.get_autoindex() == AUTOINDEX_ON
		|| (server_conf.get_autoindex()== AUTOINDEX_ON && locations->second.get_autoindex()== AUTOINDEX_UNSET))
		{
			uri.autoindex_confirmed = true;
			return ;
		}
		throw HTTPStatus<403>();
	}
	Index_const_it index = file::find_valid_index(root, indexes);
	if (index == indexes.end())
		throw HTTPStatus<404>();
	uri.path = uri.path + (*index);
}

std::string set_time(struct tm *tm_time){
	return (to_string(tm_time->tm_mday) + '-' + resolve_month(tm_time->tm_mon)
	+ '-' + to_string(tm_time->tm_year + 1900) + ' ' + to_string(tm_time->tm_hour)
	+ ':' + to_string(tm_time->tm_min));
}
std::string resolve_month(int i)
{
	const std::string months[12] = {"Jan",  "Feb",  "Mar",  "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	return (months[i]);
}

std::string insert_whitespace(size_t len, size_t spaces){
	std::string whitespaces;

	for (size_t i = 0; i < spaces - len; i++)
		whitespaces.push_back(' ');
	return (whitespaces);
}
