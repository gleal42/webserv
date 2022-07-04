/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 01:05:43 by gleal             #+#    #+#             */
/*   Updated: 2022/07/04 21:33:57 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Socket.hpp"
#include "HTTPStatus.hpp"

Response::Response( void ) { /* no-op */ }

Response::Response( Request const & request , ServerConfig config )
: _status(200)
{
	// TODO (implement constructor)
	// set member vars from config
	std::cout << "Unparsed URI " << request._unparsed_uri << std::endl;
	if (request._unparsed_uri == "/favicon.ico")
		return ;
	if (request._unparsed_uri == "/")
		_uri = "index.html";
	else
		_uri = request._unparsed_uri.c_str() + 1;
	std::ifstream open_file(_uri.c_str());
	if ( (open_file.rdstate() & std::ifstream::failbit ) != 0
    || (open_file.rdstate() & std::ifstream::badbit ) != 0 )
    {
        std::cerr << "error opening " << _uri << std::endl;
		//  send_error(404);
		return ;
    }
	std::stringstream body_str;
	body_str << open_file.rdbuf();
	_body = body_str.str();

	// Add function/class/template with MACROS to deal with attributes

	if (!config.name.empty())
		set_attribute("Server", config.name);
	std::stringstream len;
	len << body_str.str().size();
	set_attribute("Content-Length", len.str());

	std::string::size_type n = _uri.find('.');
	if (n == std::string::npos) {
		throw std::runtime_error("Invalid File"); // Add better exception
	}
	std::string type = &_uri[n];
	if (type == ".html")
		set_attribute("Content-Type", "text/html");
	else if (type == ".jpeg")
		set_attribute("Content-Type", "image/jpeg");
}

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
	std::string message = start_line(_status);
	for (attributes_iterator it = _attributes.begin(); it != _attributes.end(); ++it) {
		message += it->first + ": " + it->second + "\n";
	}
	message += "\n" + _body;
	((Socket)socket).send(message);
    printf("\n------------------Hello message sent-------------------\n");
}

void	Response::set_attribute(std::string name, std::string value)
{
	_attributes[name] = value;
}

/**
 * Need to finish incorporating this logic.
 * However, because reading and writing are 2 Separate Events,
 * At the moment we are passing the Request as parameter to gain
 * access to all the variables but we can change it.
 * Service FILE and GCI handler logic still doesn't exist.
**/


// // Does necessary to service a connection
// void   Server::run(Socket & socket) {
//        Request         req(_config);
//        Response        res(_config);
//        try {
//                // while timeout and Running
//                socket.receive(_config.input_buffer_size);
//                req.parse(socket);
//                res.request_method = req.request_method;
//                // res.request_uri = req.request_uri;
//                // if (request_callback) {
//                //      request_callback(req, res);
//                // }
//                service(req, res);
//        }
//        catch (std::exception & error) {
//                ERROR(error.what());
//        // catch (HTTPStatus error) {
//                // res.set_error(error);
//                // if (error.code) {
//                //      res.status = error.code;
//                // }
//        }
//        // if (req.request_line != "") {
//        //      res.send_response(socket);
//        // }

//        // Temporary
//        if (req._raw_header != "") {
//                res.send_response(socket);
//        }
// }
// //Services +req+ and fills in +res+
// void   Server::service(Request & req, Response & res) {
//        // Use factory create() method on Handler interface 
//  Abstract class
//        // Pick handler::service based on available info
//        // ..
//        // req.script_name = script_name;
//        // req.path_info = path_info;
// 
//        // FileHandler  handler;
//        // OR
//        // CGIHandler   handler;
// 
//        (void)req;
//        (void)res;
//        // handler.service(req, res);
// }