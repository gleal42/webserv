/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 19:38:07 by msousa            #+#    #+#             */
/*   Updated: 2022/08/26 01:38:35 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserver.hpp"
# include <unistd.h>
# include "HTTPStatus.hpp"
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

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

std::string	get_extension( const std::string &filename )
{
    std::string extension;
    size_t ext_position = filename.find_last_of('.');
    if (ext_position == std::string::npos )
        return (extension);
    return (filename.substr(ext_position));
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

struct addrinfo *get_host(const std::string &hostname )
{
	struct addrinfo *host;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	int resolve_req_host = getaddrinfo(hostname.c_str(), NULL, &hints, &host);
	if (resolve_req_host != 0)
	{
        if (resolve_req_host == EAI_NONAME)
            return NULL;
        throw HTTPStatus<500>();
	}
    if (host->ai_next != NULL)
        throw HTTPStatus<500>();
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
