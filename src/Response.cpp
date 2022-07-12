/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 01:05:43 by gleal             #+#    #+#             */
/*   Updated: 2022/07/12 20:28:29 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Socket.hpp"
#include "HTTPStatus.hpp"

Response::Response( void ) { /* no-op */ }

// void	Response::prepare_response( Request const & request , ServerConfig config )
// {
// 	_status = 200;
// 	// TODO (implement constructor)
// 	// set member vars from config
// 	std::cout << "Unparsed URI " << request._path << std::endl;
// 	if (request._path == "/")
// 		_uri = "index.html";
// 	else
// 		_uri = request._path.c_str() + 1;
// 	std::ifstream open_file(_uri.c_str());
// 	if ( (open_file.rdstate() & std::ifstream::failbit ) != 0
//     || (open_file.rdstate() & std::ifstream::badbit ) != 0 )
//     {
//         std::cerr << "error opening " << _uri << std::endl;
// 		//  send_error(404);
// 		return ;
//     }
// 	std::stringstream body_str;
// 	body_str << open_file.rdbuf();
// 	_body = body_str.str();
// 	// Add function/class/template with MACROS to deal with attributes

// 	if (!config.name.empty())
// 		set_attribute("Server", config.name);
// 	std::stringstream len;
// 	len << body_str.str().size();
// 	set_attribute("Content-Length", len.str());

// 	std::string::size_type n = _uri.find('.');
// 	if (n == std::string::npos) {
// 		throw std::runtime_error("Invalid File"); // Add better exception
// 	}
// 	std::string type = &_uri[n];
// 	if (type == ".html")
// 		set_attribute("Content-Type", "text/html");
// 	else if (type == ".jpeg")
// 		set_attribute("Content-Type", "image/jpeg");
// 	open_file.close();
// 	// save_file(request._raw_body);
// }

Response::Response( Response const & src ){
	// TODO (implement constructor)
	*this = src;
}

Response &	Response::operator = ( Response const & rhs )
{
	_status = rhs._status;
	_body = rhs._status;
	for (ResponseAttributes::iterator it = _attributes.begin(); it != _attributes.end(); it++) {
		_attributes = rhs._attributes;
	}
	return *this;
}

Response::~Response( void ) { /* no-op */ }

// need to incorporate HTTPS Statuses here
std::string Response::start_line(int status)
{
	std::string http_version = "HTTP/1.1";

	std::stringstream nbr;
	nbr << status;
	std::string status_str = nbr.str();
	std::string status_message = http_phrase(status);
	return(http_version + " " + status_str + " " + status_message + "\n");
}

void	Response::send_response(Socket const & socket)
{
	if (_message.empty())
	{
		_message = start_line(_status);
		for (attributes_iterator it = _attributes.begin(); it != _attributes.end(); ++it) {
			_message += it->first + ": " + it->second + "\n";
		}
		_message += "\n" + _body;
	}
	int sent_chars = ((Socket)socket).send(_message);
	int msg_size = _message.size();
	std::cout << "Message size before is: " << msg_size << std::endl;
	std::cout << "Sent size before is: "<< sent_chars << std::endl;
	_message = _message.substr(sent_chars, msg_size - sent_chars);
	std::cout << "Message size after is: " << _message.size() << std::endl;
    printf("\n------------------Hello message sent-------------------\n");
}

void	Response::set_attribute(std::string name, std::string value)
{
	_attributes[name] = value;
}

bool	Response::is_empty()
{
	return (_message.empty());
}

void	Response::save_file(std::vector<char> const & body)
{
	std::string full_str(body.data());
	std::string delimiter = full_str.substr(0, full_str.find("\r\n"));
	std::string::size_type start_file = full_str.find("\r\n\r\n") + 4;
	std::string::size_type end_file = full_str.rfind(delimiter.c_str());
	
	std::string file = full_str.substr(start_file, end_file - start_file - 2);
    std::ofstream outfile ("forest.jpeg");
    outfile << file.c_str();
    outfile.close();

	std::ifstream open_file("test/forest.jpeg");
	if ( (open_file.rdstate() & std::ifstream::failbit ) != 0
    || (open_file.rdstate() & std::ifstream::badbit ) != 0 )
    {
        std::cerr << "error opening " << _uri << std::endl;
		//  send_error(404);
		return ;
	}
	std::stringstream body_str;
	body_str << open_file.rdbuf();
	std::string original_image = body_str.str();
}

void	Response::set_content_length(int length)
{
	_content_length = length;
}
void	Response::set_content_type(std::string const & type)
{
	_content_type = type;
}

void	Response::set_body(std::string const & body)
{
	_body = body;
}
