/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 22:26:21 by msousa            #+#    #+#             */
/*   Updated: 2022/07/05 02:20:15 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileHandler.hpp"

/* Constructors */
FileHandler::FileHandler( void ) { /* no-op */ }
FileHandler::FileHandler( FileHandler const & src ) { *this = src; }

/* Destructor */
FileHandler::~FileHandler( void ) { /* no-op */ }

/* Assignment operator */
FileHandler &	FileHandler::operator = ( FileHandler const & rhs )
{
	if (this != &rhs) {
		//value = rhs.value;
	}
	return *this;
}

typedef std::map<std::string, std::string> MimeTypes;

std::string const FileHandler::get_content_type(std::string const path)
{
	std::string::size_type position = path.rfind('.');
	if (position == std::string::npos)
		position = 0;
	std::string extension = path.substr(position);

	// Turn this into a separate mime_type function with access to static map
	MimeTypes mime_types;

	mime_types[".css"] = "text/css";
	mime_types[".csv"] = "text/csv";
	mime_types[".gif"] = "image/gif";
	mime_types[".htm"] = "text/html";
	mime_types[".html"] = "text/html";
	mime_types[".ico"] = "image/x-icon";
	mime_types[".jpeg"] = "image/jpeg";
	mime_types[".jpg"] = "image/jpeg";
	mime_types[".js"] = "application/javascript";
	mime_types[".json"] = "application/json";
	mime_types[".png"] = "image/png";
	mime_types[".pdf"] = "application/pdf";
	mime_types[".svg"] = "image/svg+xml";
	mime_types[".txt"] = "text/plain";

	if (extension.size() && mime_types.find(extension) != mime_types.end()) {
		return mime_types[extension];
	}

	return "application/octet-stream";
}

// The service() function fills in a response with a requested resource. Our
// server expects all hosted files to be in a subdirectory called public.
// Ideally, our server should not allow access to any files outside of this
// public directory. However, enforcing this restriction may be more difficult
// than it first appears.
//
// In a production server, we would also want to log information.
// Most production servers log the date, time, request method, the client's
// user-agent string, and the response code as a minimum.
//
// Our function then normalizes the requested path. There are a few things to
// check for. First, if the path is /, then we need to serve a default file.
// There is a tradition of serving a file called index in that case, and, indeed,
// this is what our code does. This should be based on the parsed ServerConfig.
//
// Our code also checks that the path doesn't contain two consecutive dots—...
// In file paths, two dots indicate a reference to a parent directory. However,
// for security reasons, we want to allow access only into our public directory.
// We do not want to provide access to any parent directory. If we allowed paths
// with .., then a malicious client could send GET /../web_server.c HTTP/1.1 and
// gain access to our server source code!
void	FileHandler::service(Request & req, Response & res)
{
	if (req._path == "/") req._path = "/index.html";

	if (req._path.size() > 100) {
		throw HTTPStatus<400>(); // Example
	}

	if (req._path.find("..") != std::string::npos) {
		throw HTTPStatus<404>();
	}

	std::string	full_path("public" + req._path);
	std::ifstream file(full_path);

	if ( (file.rdstate() & std::ifstream::failbit ) != 0
		|| (file.rdstate() & std::ifstream::badbit ) != 0 )
	{
		ERROR("error opening " << full_path);
		throw HTTPStatus<404>();
	}

	res.set_content_length(file_size(full_path));
	res.set_content_type(get_content_type(full_path));

	std::stringstream body;
	body << file.rdbuf();
	res.body = body.str();

	file.close();
}

std::streampos	FileHandler::file_size( std::string	full_path )
{

	std::streampos	fsize = 0;
	std::ifstream	file( full_path, std::ios::binary );

	fsize = file.tellg();
	file.seekg( 0, std::ios::end );
	fsize = file.tellg() - fsize;
	file.close();

	return fsize;
}
