/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 22:26:21 by msousa            #+#    #+#             */
/*   Updated: 2022/08/31 17:41:42 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileHandler.hpp"

/* Constructors */
FileHandler::FileHandler( void ) { /* no-op */ }
FileHandler::FileHandler( FileHandler const & src ): Handler() { *this = src; }

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

void	FileHandler::do_GET( Request & req, Response & res )
{
	if (req._path == "/")
		res._uri = "public/index.html";
	else
		res._uri = std::string("public/") + (req.request_uri.path.c_str() + 1);

	if (req._path.size() > 100) {
		throw HTTPStatus<400>(); // Example
	}
	if (req._path.find("..") != std::string::npos) {
		throw HTTPStatus<404>();
	}
	res.set_with_file(res._uri);
}

std::string const FileHandler::get_content_type(std::string const path)
{
	std::string extension = get_extension(path);

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

void	FileHandler::do_POST( Request & req, Response & res )
{
	if (req.get_form_type() == "multipart/form-data") {
		post_multi_type_form(req);
	}
	else if (req.get_form_type() == "application/x-www-form-urlencoded")
		post_form_urlencoded(req);
	else
		throw HTTPStatus<415>();
	res.set_default_page(); // temporary
}

/*
	Multi part forms separate each part using a delimiter (sent in Request)
	Last delimiter is followed by "--" so I use this information in the parsing.

	To check if it is file, we can see that after the delimiter there is
	a field called "filename", which non-file inputs don't have.

	Each delimiter has an extra "--" before the delimiter itself
	After delimiter, the extra form headers end when double CRLF is found
*/

void	FileHandler::post_multi_type_form( Request & req )
{
	// std::string::size_type start_multi_form = req._raw_body.find("\r\n\r\n") + 4;
	// std::string multi_form = req._raw_body.substr(start_multi_form);
	std::string multi_form = req._raw_body;
	std::string section_body;
	std::string delimiter = req.get_delimiter();

	size_t next_delimiter = multi_form.find(delimiter);
	if (next_delimiter == std::string::npos)
		throw HTTPStatus<400>();
	size_t last_delimiter = multi_form.find(delimiter + "--");
	if (last_delimiter == std::string::npos)
		throw HTTPStatus<400>();

	std::string::size_type start_file;
	std::string::size_type end_file;
	while (1)
	{
		size_t form_header_pos = next_delimiter + delimiter.size() + 2; // \r\n
		multi_form = multi_form.substr(form_header_pos);
		next_delimiter = multi_form.find(delimiter);
		if (next_delimiter == std::string::npos)
			throw HTTPStatus<400>();
		last_delimiter = multi_form.find(delimiter + "--");
		if (last_delimiter == std::string::npos)
			throw HTTPStatus<400>();
		std::string filename = parse_from_multipart_form("filename=", multi_form, next_delimiter);
	
		start_file = multi_form.find(D_CRLF) + 4;
		if (start_file == std::string::npos)
			throw HTTPStatus<400>();
		section_body = multi_form.substr(start_file);
		end_file = section_body.find(delimiter) - 4; // -4 => "--" + "\r\n"
		section_body = section_body.substr(0, end_file);
		std::cout << "Form part body has size: [" << section_body.size() << "]" << std::endl;

		if (filename.empty())
		{
			std::string paramater_name = parse_from_multipart_form("name=", multi_form, next_delimiter);
			params[paramater_name] = section_body; // Replace with insert and add BadRequest in case not unique?
		}
		else
		{
			file::save(section_body, filename);
			filename.clear();
		}
		if (next_delimiter == last_delimiter)
			return ;
	}
	// OPTIONAL: Default Servicing for params.
}

// Used to parse data like "name" and "filename" from form part headers.

std::string		FileHandler::parse_from_multipart_form( const std::string parameter, const std::string &body, size_t next_delimiter )
{
	std::string filename;
	size_t param_pos = body.find(parameter.c_str());

	if (param_pos == std::string::npos || param_pos > next_delimiter)
		return filename;
	size_t start_filename = body.find(parameter.c_str()) + parameter.length() + 1;
	filename = body.substr(start_filename);
	size_t end_filename = filename.find("\"");
	filename = filename.substr(0, end_filename);
	std::cout << parameter << " is: [" << filename << "]" << std::endl;
	return (filename);
}

/*
	Simply parses data from "application/x-www-form-urlencoded" form

	Saves variables and values.
	However no action is defined yet for these parsed values
*/

void	FileHandler::post_form_urlencoded( Request & req )
{
	size_t ending_char = req._raw_body.rfind('\0');
	if (ending_char == std::string::npos)
		throw HTTPStatus<400>();
	std::string single_form = req._raw_body.substr(0, ending_char);
	while (1)
	{
		size_t separator = single_form.find('&');
		std::string var_val = single_form.substr(0, separator);
		url::decode(var_val);
		size_t equal_pos = single_form.find('=');
		if (equal_pos == std::string::npos)
			throw HTTPStatus<400>();
		params[var_val.substr(0, equal_pos)] = var_val.substr(equal_pos + 1); // Replace with insert and add BadRequest in case not unique?
		if (separator == std::string::npos)
			return ;
		single_form = single_form.substr(separator+1);
	}
}

/*
	removes file (also does some extensions validations)
	In future PR validations will be simply if there is .. because
	root will start in public folder
*/

void	FileHandler::do_DELETE( Request & req , Response & res )
{
	std::string str(req._path.c_str() + 1);
	file::remove(std::string(req._path.c_str() + 1));
	res.set_default_page(); // temporary
}

// Added a protection to prevent us from deleting a repository code or other testing data

void	FileHandler::delete_file( std::string filename )
{
	static char const * temp_ext[8] = {
    [0] = ".cpp",
    [1] = ".hpp",
    [2] = ".html",
    [3] = ".ico",
    [4] = ".ts",
    [5] = ".rb",
    [6] = ".sh",
    [7] = ".h",
	};
	const Extensions	forbidden_extensions(temp_ext, temp_ext + sizeof(temp_ext) / sizeof(char const *));

	std::string file_extension = get_extension(filename);
	if (file_extension.empty() || forbidden_extensions.count(file_extension)) {
		throw HTTPStatus<405>();
	}
	if (filename.substr(0, 13) != "post/uploads/") {// Temporary
		throw HTTPStatus<405>();
	}
	std::cout << "Extension is [" << file_extension << "]" << std::endl;
	filename = "public/" + filename;
	std::cout << "Filename is [" << filename << "]" << std::endl;
	if (std::remove (filename.c_str()) != 0)
		throw HTTPStatus<404>();
}

