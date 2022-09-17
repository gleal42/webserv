/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 19:38:07 by msousa            #+#    #+#             */
/*   Updated: 2022/09/17 21:17:39 by fmeira           ###   ########.fr       */
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

std::string processed_root( const ServerConfig & server_conf, const LocationConfig &location )
{
	std::string root = priority_directive(server_conf.get_root(), location.get_root());
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
	for (ErrorPage_cit it_dest_err = dest_list.begin();
		it_dest_err != dest_list.end();
		it_dest_err++)
	{
		std::vector<unsigned short>::const_iterator it_del = std::find(it_dest_err->second.begin(), it_dest_err->second.end(), code);
		if (it_del != it_dest_err->second.end())
			return(it_dest_err->first);
	}
    return (std::string());
}

Location_cit	path_resolve( URI & uri, const ServerConfig & server_conf)
{
	Location_cit location_inside_server = location_resolve(server_conf, uri.path);
	LocationConfig location;
	if (location_inside_server != server_conf.get_locations().end())
		location = location_inside_server->second;
	std::string root ("public" + processed_root( server_conf, location ));
	if (uri.path.compare(0, 6 , "public/") == 0)
		uri.path = uri.path.substr(7);
	clean_path(uri, root);
	std::string root_path = root + uri.path;
	if (is_directory(root_path))
	{
		if (root_path[root_path.length()-1] != '/')
			uri.redirect_confirmed = true;
		else
			directory_indexing_resolve( uri, root_path, server_conf, location);
	}
	if (*uri.path.begin() != '/')
		uri.path.insert(uri.path.begin(), '/');
	root_path = root + uri.path;
	Location_cit redir_locations = location_resolve(server_conf, uri.path);

	if ( redir_locations != server_conf.get_locations().end() && redir_locations->first.size() > location_inside_server->first.size())
		return (path_resolve(uri, server_conf));
	else {
		uri.path = root_path;
		return location_inside_server;
	}
}

Location_cit      location_resolve(const ServerConfig &server_block, const std::string & path)
{
	std::string location_path = path;
	if ( location_path.size() > 0 && *(location_path.end()-1) != '/')
		location_path.push_back('/');
	const Locations &locations = server_block.get_locations();
	if (locations.size() == 0)
		return (locations.end());
	while (location_path.empty() == false)
	{
		for (Location_cit it = locations.begin();
			it != locations.end();
			it++)
			{
				if ((it->first) == location_path)
					return (it);
			}
		location_path.erase(--location_path.end());
	}
	return (locations.end());
	// throw HTTPStatus<404>(); // may need to add default / location to match nginx behaviour
}

void	reset_path(std::string &path, std::string &extra_path)
{
	if (extra_path.empty() == false) {
		path = path + extra_path;
		extra_path.clear();
	}
}

void			clean_path( URI & uri, const std::string &root)
{
	reset_path(uri.path, uri.extra_path);

	if ( is_file(root + uri.path) == false && is_directory(root + uri.path) == false)
	{
		size_t query_string_start = uri.path.rfind("?");
		while (query_string_start != std::string::npos)
		{
			reset_path(uri.path, uri.extra_path);
			uri.query = uri.path.substr(query_string_start);
			uri.path = uri.path.substr(0, query_string_start);
			if ( is_file(root + uri.path) == true || is_directory(root + uri.path) == true )
				break ;
			size_t extra_path_start = uri.path.rfind("/");
			while (extra_path_start != std::string::npos)
			{
				uri.extra_path = uri.path.substr(extra_path_start) + uri.extra_path;
				uri.path = uri.path.substr(0, extra_path_start);
				if ( is_file(root + uri.path) == true || is_directory(root + uri.path) == true )
					break ;
				extra_path_start = uri.path.rfind("/");
			}
			query_string_start = uri.path.rfind("?");
		}
	}
	if (uri.query.size() > 0)
		uri.query = uri.query.substr(1);
}

// ~INDEX PRIORITY:
//		1) matching [locationpath].html assigned to location by index directive on LocationConfig
// 		2) matching [locationpath].html set on location's ServerConfig index directive
//		3) index.html inside location's directory (in case there is no index on ServerConfig)
//		4) autoindex directive on
// ~AFAIK, the only way to define an index for root is through location /
void			directory_indexing_resolve( URI & uri, const std::string &root, const ServerConfig &server_conf, const LocationConfig & location)
{
	Indexes indexes;

	if (!location.is_empty())
		indexes = location.get_indexes();

	if (indexes.empty())
	{
		indexes = server_conf.get_indexes();
		if (indexes.empty())
		{
			if (is_file(root + "index.html"))
			{
				uri.path = root.substr(7) + "index.html";
				return ;
			}
			else if (location.get_autoindex() == AUTOINDEX_ON
			|| (server_conf.get_autoindex()== AUTOINDEX_ON && location.get_autoindex()== AUTOINDEX_UNSET))
			{
				uri.autoindex_confirmed = true;
				return ;
			}
			throw HTTPStatus<403>();
		}
		Indexes_cit index = file::find_valid_index(root, indexes);
		if (index == indexes.end())
		{
			if (is_file(root + "index.html"))
			{
				uri.path = root.substr(7) + "index.html";
				return ;
			}
			if (location.get_autoindex() == AUTOINDEX_ON
			|| (server_conf.get_autoindex()== AUTOINDEX_ON && location.get_autoindex()== AUTOINDEX_UNSET))
			{
				uri.autoindex_confirmed = true;
				return ;
			}
			throw HTTPStatus<403>();
		}
		uri.path = uri.path + (*index);
		return ;
	}

	Indexes_cit index = file::find_valid_index(root, indexes);
	if (index == indexes.end()){
		if (location.get_autoindex() == AUTOINDEX_ON
		|| (server_conf.get_autoindex()== AUTOINDEX_ON && location.get_autoindex()== AUTOINDEX_UNSET))
		{
			uri.autoindex_confirmed = true;
			return ;
		}
		throw HTTPStatus<403>();
	}
	uri.path = uri.path + (*index);
}

std::string set_time(struct tm *tm_time){
	return (to_string(tm_time->tm_mday)
			+ '-' + resolve_month(tm_time->tm_mon)
			+ '-' + to_string(tm_time->tm_year + 1900) + ' '
			+ (tm_time->tm_hour < 10 ? "0" : "") + to_string(tm_time->tm_hour)
			+ ':' + (tm_time->tm_min < 10 ? "0" : "")
			+ to_string(tm_time->tm_min));
}

std::string resolve_month(int i)
{
	const std::string months[12] = {"Jan",  "Feb",  "Mar",  "Apr", "May", "Jun",
									"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	return (months[i]);
}

std::string insert_whitespace(size_t len, size_t spaces){
	std::string whitespaces;

	for (size_t i = 0; i < spaces - len; i++)
		whitespaces.push_back(' ');
	return (whitespaces);
}


BaseStatus get_httpstatus(int code)
{
	if (code == 200)
		return (HTTPStatus<200>());
	if (code == 302)
		return (HTTPStatus<302>());
	if (code == 400)
		return (HTTPStatus<400>());
	if (code == 511)
		return (HTTPStatus<511>());
    if (code == 100)
        return (HTTPStatus<100>());
    if (code == 101)
        return (HTTPStatus<101>());
    if (code == 201)
        return (HTTPStatus<201>());
    if (code == 202)
        return (HTTPStatus<202>());
    if (code == 203)
        return (HTTPStatus<203>());
    if (code == 204)
        return (HTTPStatus<204>());
    if (code == 205)
        return (HTTPStatus<205>());
    if (code == 206)
        return (HTTPStatus<206>());
    if (code == 207)
        return (HTTPStatus<207>());
    if (code == 300)
		return (HTTPStatus<300>());
	if (code == 301)
		return (HTTPStatus<301>());
	if (code == 303)
		return (HTTPStatus<303>());
	if (code == 304)
		return (HTTPStatus<304>());
	if (code == 305)
		return (HTTPStatus<305>());
	if (code == 307)
		return (HTTPStatus<307>());
	if (code == 401)
		return (HTTPStatus<401>());
	if (code == 402)
		return (HTTPStatus<402>());
	if (code == 403)
		return (HTTPStatus<403>());
	if (code == 404)
		return (HTTPStatus<404>());
	if (code == 405)
		return (HTTPStatus<405>());
	if (code == 406)
		return (HTTPStatus<406>());
	if (code == 407)
		return (HTTPStatus<407>());
	if (code == 408)
		return (HTTPStatus<408>());
	if (code == 409)
		return (HTTPStatus<409>());
	if (code == 410)
		return (HTTPStatus<410>());
	if (code == 411)
		return (HTTPStatus<411>());
	if (code == 412)
		return (HTTPStatus<412>());
	if (code == 413)
		return (HTTPStatus<413>());
	if (code == 414)
		return (HTTPStatus<414>());
	if (code == 415)
		return (HTTPStatus<415>());
	if (code == 416)
		return (HTTPStatus<416>());
	if (code == 417)
		return (HTTPStatus<417>());
	if (code == 422)
		return (HTTPStatus<422>());
	if (code == 423)
		return (HTTPStatus<423>());
	if (code == 424)
		return (HTTPStatus<424>());
	if (code == 426)
		return (HTTPStatus<426>());
	if (code == 428)
		return (HTTPStatus<428>());
	if (code == 429)
		return (HTTPStatus<429>());
	if (code == 431)
		return (HTTPStatus<431>());
	if (code == 451)
		return (HTTPStatus<451>());
	if (code == 500)
		return (HTTPStatus<500>());
	if (code == 501)
		return (HTTPStatus<501>());
	if (code == 502)
		return (HTTPStatus<502>());
	if (code == 503)
		return (HTTPStatus<503>());
	if (code == 504)
		return (HTTPStatus<504>());
	if (code == 505)
		return (HTTPStatus<505>());
	if (code == 507)
		return (HTTPStatus<507>());
	return (HTTPStatus<500>());
}
