/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 19:38:07 by msousa            #+#    #+#             */
/*   Updated: 2022/08/26 00:30:48 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserver.hpp"

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
