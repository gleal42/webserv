/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 01:05:43 by gleal             #+#    #+#             */
/*   Updated: 2022/09/13 00:58:43 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Socket.hpp"
#include "HTTPStatus.hpp"
#include "file.hpp"
#include <algorithm>

Response::Response( void ) { /* no-op */ }

Response::Response( Response const & src ){
	// TODO (implement constructor)
	*this = src;
}

Response &	Response::operator = ( Response const & rhs )
{
	_uri = rhs._uri;
	_message = rhs._message;
	_body = rhs._body;
	_headers.clear();
	for (ResponseHeaders::iterator it = _headers.begin(); it != _headers.end(); it++) {
		_headers = rhs._headers;
	}
	return *this;
}

Response::~Response( void ) { /* no-op */ }

// need to incorporate HTTPS Statuses here
std::string Response::start_line( const BaseStatus &status )
{
	std::string http_version = "HTTP/1.1";

	std::stringstream nbr;
	nbr << status.code;
	std::string	status_str = nbr.str();
	std::string status_message = status.reason_phrase;
	return(http_version + " " + status_str + " " + status_message + CRLF);
}

void	Response::build_message( const BaseStatus &status )
{
	if (_message.empty())
	{
		_message = start_line(status);
		for (ResponseHeaders_it it = _headers.begin(); it != _headers.end(); ++it) {
			_message += it->first + ": " + it->second + CRLF;
		}
		_message += CRLF + _body;
	}
}

std::string		Response::message( void ) { return _message; }

void	Response::send_response( Socket const & socket )
{
	int sent_chars = socket.send(_message);
	// TODO: check -1 and 0?
	int msg_size = _message.size();
	std::cout << "Message size before is: " << msg_size << std::endl;
	std::cout << "Message is: " << _message << std::endl;
	std::cout << "Sent size before is: "<< sent_chars << std::endl;
	_message = _message.substr(sent_chars, msg_size - sent_chars);
	LOG("Message size after is: " << _message.size());
    LOG("\n------------------Message sent-------------------\n");
}
// Note that the last += statement sends \r\n. This has the effect of transmitting
// a blank line. This blank line is used by the client to delineate the HTTP
// header from the beginning of the HTTP body.

void	Response::set_header(const std::string &name, const std::string &value)
{
	_headers[name] = value;
	LOG ("Setting " << name << " as " << value);
}

bool	Response::is_empty()
{
	return (_message.empty());
}

void	Response::set_body(std::string const & body)
{
	_body = body;
}

void	Response::set_default_page( void )
{
	this->set_header("Content-Type", "text/html");
	std::string body("<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n</head>\n<body>\n<h1>Success!! 😀</h1>\n</body>\n</html>");
	this->set_body(body.c_str());
	std::stringstream len;
	len << body.size();
	this->set_header("Content-Length", len.str());
}

void	Response::set_error_page( const BaseStatus &error_status )
{
	std::string error_str = error_path(error_status.code);
	try{
		set_with_file(error_str);
	}
	catch (BaseStatus &exc)
	{
		error_str = to_string(error_status.code);
		error_str = "public/www/error_pages/" + error_str + ".html";
		set_with_file(error_str);
	}
	build_message(error_status);
}

void    Response::set_with_file( const std::string &filename )
{
	if (filename.empty())
		throw HTTPStatus<404>();
	std::ifstream file(filename.c_str());
	if ( (file.rdstate() & std::ifstream::failbit ) != 0
		|| (file.rdstate() & std::ifstream::badbit ) != 0 )
	{
		ERROR("error opening " << filename.c_str());
		throw HTTPStatus<404>();
	}

	set_header("Content-Type", file::content_type(filename.c_str()));
	std::stringstream body;
	body << file.rdbuf();
	set_body(body.str());
	std::stringstream len;
	len << body.str().size();
	set_header("Content-Length", len.str());

	file.close();
}

void	Response::clear( void )
{
	_uri.clear();
	_body.clear();
	_headers.clear();
	_message.clear();
}

void	Response::save_raw_headers( std::string headers )
{
	while  (1)
	{
		size_t next_header = headers.find(CRLF);
		std::string single_header = headers.substr(0, next_header);
		size_t separator = single_header.find(":");
		if (single_header.empty() == false
			&& separator == std::string::npos)
			throw HTTPStatus<400>();
		this->set_header(single_header.substr(0, separator), single_header.substr(separator+1));
		if (next_header == std::string::npos)
			break ;
		headers = headers.substr(next_header + 2);
	}
}

const std::string	Response::get_header_value(const std::string &name)
{
	if (_headers.count(name)) {
		return(_headers[name]);
	}
	return ("");
}

std::size_t			Response::get_body_size( void ) {return(_body.size());}


void	Response::delete_header( const std::string & name )
{
	std::cout << name << " header was deleted" << std::endl;
	_headers.erase(name);
}

void	Response::add_error_list(const ErrorPage &server_error_pages , const ErrorPage &location_error_pages)
{
	this->_available_errors = server_error_pages;
	for (ErrorPage_cit it_err = location_error_pages.begin();
			it_err != location_error_pages.end();
			it_err++)
		{

			for (std::vector<unsigned short>::const_iterator it_code = it_err->second.begin();
				it_code != it_err->second.end();
				it_code++)
				{
					update_error_code(this->_available_errors, it_err->first, *it_code);
				}
		}
}

std::string   Response::error_path(unsigned short code)
{
	for (ErrorPage_it it_dest_err = this->_available_errors.begin();
		it_dest_err != this->_available_errors.end();
		it_dest_err++)
	{
		std::vector<unsigned short>::iterator it_del = std::find(it_dest_err->second.begin(), it_dest_err->second.end(), code);
		if (it_del != it_dest_err->second.end())
		{
			URI error_uri;
			error_uri.path = it_dest_err->first;
			Location_cit location_to_use = path_resolve(error_uri, _server_conf);
			const StringVector &req_methods = location_to_use->second.get_limit_except();
			if (std::find_if(req_methods.begin(), req_methods.end(), equals("GET")) == req_methods.end())
				return std::string();
			return(error_uri.path);
		}
	}
    return (std::string());
}

void			Response::set_server_config(const ServerConfig &config_to_use)
{
	_server_conf = config_to_use;
}
