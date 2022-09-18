/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmeira <fmeira@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 20:30:18 by msousa            #+#    #+#             */
/*   Updated: 2022/09/18 00:45:56 by fmeira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Socket.hpp"
#include "Listener.hpp"
#include "HTTPStatus.hpp"
#include "ServerConfig.hpp"
#include <algorithm>

std::map<enum RequestMethod, std::string>Request::_method_conv = init_map();

Request::ReqMethodConversion	Request::init_map( void )
{
	ReqMethodConversion req_method;
	req_method[GET]="GET";
	req_method[POST]="POST";
	req_method[DELETE]="DELETE";
	return req_method;
}

/* Constructors */
Request::Request( void ) { /* no-op */ }

Request::Request( ServerConfig const & config )
{
	// TODO (implement constructor)
	// set member vars from config
	// even though we aren't using in `parse` might still be needed for counting
	// how much read and that can still read, if not remove
	client_max_body_size = config.get_max_body_size();
	request_uri.autoindex_confirmed = false;
	request_uri.redirect_confirmed = false;
}

Request::Request( Request const & param ) {
	// TODO initializer list?
	*this = param;
}

Request::~Request() {
	std::cout << "Request" << " destroyed" << std::endl;
	// TODO (destructor)
}

// might not be needed so can be private and have no implementation
Request& Request::operator= ( Request const & param ) {
	// TODO (Assignment operatior)
	// std::swap()
	_raw_request_line = param._raw_request_line;
	request_method = param.request_method;
	request_uri = param.request_uri;
	client_max_body_size = param.client_max_body_size;
	_path = param._path;
	_headers = param._headers;
	_raw_body = param._raw_body;
	return (*this);
}

std::ostream & operator<<(std::ostream& s, const Request& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

/* Methods */

// Reads request line, assigning the appropriate method and unparsed uri. (Will we need a parsed URI of the request?)
// Increments strptr to the beggining of the header section
void	Request::read_request_line( std::string & _unparsed_request ) {
	int								i = 0;
	int								j = 0;
	std::string						parsed_req_method;
	std::string::iterator			it = _unparsed_request.begin();
	RequestMethods	request_methods;

	request_methods["GET"] = GET;
	request_methods["POST"] = POST;
	request_methods["DELETE"] = DELETE;
	for (; *it != ' '; it++)
		i++;
	parsed_req_method = _unparsed_request.substr(0, i++);
	if (request_methods.find(parsed_req_method) == request_methods.end())
		throw HTTPStatus<405>();
	request_method = request_methods[parsed_req_method];
	for (*(it)++; *it != ' '; it++)
		j++;

	_path = _unparsed_request.substr(i, j);
	for (it++; *it != '\n'; it++)
		j++;

	_raw_request_line = _unparsed_request.substr(0, i + j + 2);
	std::cout << "Request line is :" << _raw_request_line << std::endl;
	std::cout << "Path is :" << _path << std::endl;

	if (_path.size() > 100) {
		throw HTTPStatus<414>(); // Example
	}
	if (_path.find("..") != std::string::npos) {
		throw HTTPStatus<404>();
	}
	_unparsed_request = _unparsed_request.substr(++j + ++i);

	request_uri.path = _path;
};

// 1
// Reads the header lines and inserts fields into a hash map(KEY->VALUE).
// So far, fields are parsed using the following logic:
// for every line, until the start of body section, KEY is every char that comes before ':' on that line
// and VALUE is every char the follow after ": " until the end of the line
// Later we can do further adjustments on that, if necessary.
// 2
// Some requests send data to the server in order to update it: as often the case with POST requests (containing HTML form data).
// So I made a new class variable, _raw_body, which contains data associated with the request.
// In case this data is present, read_header() parses it. We can, of course, make a custom function just for that.
// Let me know what you think.

// Check file upload


void	Request::read_header(std::string &_unparsed_request)
{
	std::string				key;
	std::string				value;
	size_t					key_start = 0;
	size_t					separator = 0;
	size_t					value_start;
	size_t					end;

	size_t	body_start = _unparsed_request.find(D_CRLF);
	if (body_start == std::string::npos)
		return ;
	_raw_headers += _unparsed_request.substr(0, body_start + 4);
	std::cout << "Headers are :" << std::endl << _raw_headers << std::endl;
	for (size_t i = 0; i < body_start; i++){
		if (_raw_headers[i] == ':')
		{
			separator = i - key_start;
			key = _raw_headers.substr(key_start, separator);
			end = 0;
			i += 2;
			value_start = i;
			while (_raw_headers[i] != '\r' && _raw_headers[i] != '\n'){
				end++;
				i++;
			}
			value = _raw_headers.substr(value_start, end);
			this->_headers.insert(std::pair<std::string, std::string>(key, value));
			key_start = i + 2;
		}
	}
	_unparsed_request = _unparsed_request.substr(body_start + 4);
	// Defining Host to be used in Service

	std::string raw_host = _headers["Host"];
	size_t port_start = raw_host.find(':');
	request_uri.host = raw_host.substr(0, port_start);
	if (port_start == std::string::npos)
		request_uri.port = 80; // default port for HTTP
	else
	{
		std::string port = raw_host.substr(port_start + 1);
		// std::cout << "PORT SHOULD BE " << port << std::endl;
		request_uri.port = str_to_nbr<int>(port);
		std::cout << "PORT IS " << request_uri.port << std::endl;
	}
}

void	Request::read_body(std::string &_unparsed_request)
{
	join_strings(_raw_body, _unparsed_request);
	_unparsed_request.clear();
}

void	Request::parse(Socket & socket, int read_size )
{
	socket.receive(read_size);
	if (socket._buffer.empty()) {
		throw std::exception(); // TODO: decide what error this is
	}
	if (socket.bytes() == 0) {
		// TODO: decide what to do here
		LOG("BYTES WAS ZERO");
		LOG("BYTES WAS ZERO");
		LOG("BYTES WAS ZERO");
		LOG("BYTES WAS ZERO");
	}

	std::cout << "We have received [" << socket._buffer.size() << "] bytes in total." << std::endl;
	append_buffer(_unparsed_request, socket._buffer);
	std::cout << "Now buffer has size [" << _unparsed_request.size() << "] bytes in total." << std::endl;

	if (_raw_request_line.empty() || _raw_request_line.find('\n') == std::string::npos)
		read_request_line(_unparsed_request);
	if (_raw_headers.empty() || _raw_headers.find("\r\n\r\n") == std::string::npos)
		read_header(_unparsed_request);
	if (_headers.count("Content-Length"))
		read_body(_unparsed_request);

	// set accept_* values
	// setup_forwarded_info();
	// request_uri = parse_uri(unparsed_uri);
}

// Replace following 2 by template?

void	Request::append_buffer(std::string &str, std::vector<char> &to_add)
{
	if (!str.empty())
		str.erase(--str.end());
	str.append(to_add.data(), to_add.size());
}

void	Request::join_strings(std::string &str, std::string &to_add)
{
	if (!str.empty())
		str.erase(--str.end());
	str.append(to_add.data(), to_add.size());
}

void	Request::clear()
{
	_unparsed_request.clear();
	_raw_request_line.clear();
	_raw_headers.clear();
	_raw_body.clear();
	_path.clear();
	request_uri.clear();
	cgi.extension.clear();
	cgi.interpreter.clear();
	client_max_body_size = -1;
	_headers.clear();
}

// Maybe these 2 could be included in the parsing?

std::string		Request::get_form_type( void )
{
    std::string content_type = _headers["Content-Type"];
	size_t form_type_pos = content_type.find(";");
	return (content_type.substr(0, form_type_pos));
}

std::string			Request::get_auth_type( void ) const
{
	std::string auth_type;
	if (_headers.count("Authorization"))
	{
		auth_type = _headers.at("Authorization");
		std::string::iterator it(std::find_if(auth_type.begin(), auth_type.end(), ::isspace));
		return(std::string(auth_type.begin(), it));
	}
	return auth_type;
}

std::string			Request::get_remote_user( void ) const
{
	if (_headers.count("Authorization"))
	{
		std::string authorization = _headers.at("Authorization");
		std::string auth_type = get_auth_type();
		std::string user_password;
		if (auth_type == "Basic")
		{
			std::string::iterator it(std::find_if(authorization.begin(), authorization.end(), ::isspace));
			while (::isspace(*it)) ++it;
			if (it == authorization.end())
				return std::string();
			user_password = std::string(it, authorization.end());
			user_password = b64decode(user_password);
			return (user_password.substr(0, user_password.find(':')));
		}
		else if (auth_type == "Digest")
		{
			size_t username = authorization.find("username=\"");
			if (username == std::string::npos)
				return std::string();
			user_password = std::string(authorization.substr(username + strlen("username=\"")));
			return (user_password.substr(0, user_password.find('"')));
		}
		throw HTTPStatus<501>();
	}
	return std::string();
}

std::string		Request::get_delimiter( void )
{
	std::string content_type = _headers["Content-Type"];
	size_t boundary_pos = content_type.find("boundary=");
	if (boundary_pos == std::string::npos)
		throw HTTPStatus<400>();
	std::string delimiter = content_type.substr(boundary_pos + 9);
	if (delimiter.empty())
		throw HTTPStatus<400>();
	return (delimiter);
}

std::string	Request::method_to_str( void )
{
	try {
		std::string method(this->_method_conv.at(request_method));
		return method;
	} catch (std::exception &err) {
		(void)err;
		throw HTTPStatus<501>();
	}
}

std::string		Request::get_hostname( void )
{
	try
	{
		std::string hostname(this->_headers.at("Host"));
		return hostname.substr(0, hostname.find(':'));
	} catch (std::exception &err)
	{
		(void)err;
		throw HTTPStatus<400>();
	}
}

const std::string	Request::get_header_value(const std::string &name)
{
	if (_headers.count(name)) {
		return(_headers[name]);
	}
	return ("");
}
