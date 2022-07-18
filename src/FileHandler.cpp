/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 22:26:21 by msousa            #+#    #+#             */
/*   Updated: 2022/07/22 18:37:21 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "FileHandler.hpp"
# include <stdexcept>

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
	if (req._path == "/")
		res._uri = "index.html";
	else
		res._uri = req._path.c_str() + 1;

	if (req._path.size() > 100) {
		throw HTTPStatus<400>(); // Example
	}

	if (req._path.find("..") != std::string::npos) {
		throw HTTPStatus<404>();
	}

	std::ifstream file(res._uri.c_str());
	if ( (file.rdstate() & std::ifstream::failbit ) != 0
		|| (file.rdstate() & std::ifstream::badbit ) != 0 )
	{
		ERROR("error opening " << res._uri.c_str());
		throw HTTPStatus<404>();
	}

	res.set_attribute("Content-Type", get_content_type(res._uri.c_str()));
	std::stringstream body;
	body << file.rdbuf();
	res.set_body(body.str());
	std::stringstream len;
	len << body.str().size();
	res.set_attribute("Content-Length", len.str());

	file.close();
}

// Perhaps it is better to just count body size?
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

void	FileHandler::save_file(Request & req, Response & res)
{
	std::string content_type(req._headers["Content-Type"]);
	std::string form_type(content_type.substr(0, content_type.find(";")));

	size_t start_filename = req._raw_body.find("filename=") + 10;
	std::string filename(req._raw_body.substr(start_filename));
	size_t end_filename = filename.find("\"");
	filename = filename.substr(0, end_filename);

	std::cout << "Filename is: [" << filename << "]" << std::endl;
	if (form_type == "multipart/form-data")
	{
		std::string delimiter = content_type.substr(content_type.find("boundary=") + 9);
		if (delimiter.empty())
			throw std::runtime_error("No delimiter");
		std::string::size_type start_file = req._raw_body.find("\r\n\r\n") + 4;
		std::string file = req._raw_body.substr(start_file);
		std::string::size_type end_file = file.find(delimiter) - 4; // -4 => "--" + "\r\n"
	
		file = file.substr(0, end_file);
		// std::cout << "Another delimiter is found at position: " << end_file << std::endl;;
		
		std::cout << "File has size: [" << file.size() << "]" << std::endl;
	
		std::ofstream outfile;
		outfile.open("post/uploads/" + filename, std::ios::binary);
		outfile.write(file.data(), file.size());
		outfile.close();

		std::ifstream infile;
	}
	res.set_attribute("Content-Type", "text/plain");
	std::string body("Good job");
	res.set_body(body.c_str());
	std::stringstream len;
	len << body.size();
	res.set_attribute("Content-Length", len.str());
}

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
