/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 20:30:18 by msousa            #+#    #+#             */
/*   Updated: 2022/07/07 03:16:15 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Socket.hpp"
#include "Server.hpp"

Request::Request( void ) { /* no-op */ }

Request::Request( ServerConfig const & config )
{
	// TODO (implement constructor)
	// set member vars from config
	// even though we aren't using in `parse` might still be needed for counting
	// how much read and that can still read, if not remove
	client_max_body_size = config.client_max_body_size;
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
	request_line = param.request_line;
	request_method = param.request_method;
	request_uri = param.request_uri;
	client_max_body_size = param.client_max_body_size;
	_unparsed_uri = param._unparsed_uri;
	_raw_request = param._raw_request;
	_attributes = param._attributes;
	_raw_body = param._raw_body;
	return (*this);
}

std::ostream & operator<<(std::ostream& s, const Request& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

// Reads request line, assigning the appropriate method and unparsed uri. (Will we need a parsed URI of the request?)
// Increments strptr to the beggining of the header section
void	Request::read_request_line(std::string &strptr){
	std::string						str;
	int								i = 0;
	int								j = 0;
	std::string						buf = strptr;
	std::string::iterator			iter = buf.begin();

	for (; *iter != ' '; iter++)
		i++;
	str = buf.substr(0, ++i);

	// Consider global constant map: 	RequestMethods[ str ]
	// typedef std::map< std::string, RequestMethod >	RequestMethods;
	// this will avoid re-doing these comparisons every time
	if (str.compare("GET"))
		request_method = GET;
	else if(str.compare("POST"))
		request_method = POST;
	else if(str.compare("DELETE"))
		request_method = DELETE;
	else
		throw("No appropriate method");
		// this error should have happened at the config parsing stage and blocked the loading of the server

	for (*(iter)++; *iter != ' '; iter++)
		j++;

	_unparsed_uri = buf.substr(i, j);

	for (iter++; *iter != '\n'; iter++)
		j++;

	request_line = buf.substr(0, i + j);
	str = buf.substr(++j + ++i, buf.length());
	strptr = str;
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


void	Request::read_header(std::string &strptr)
{
	std::string				key;
	std::string				value;
	size_t					key_start = 0;
	size_t					separator = 0;
	size_t					value_start;
	size_t					end;

	size_t	body_start = strptr.find("\r\n\r\n");
	if (body_start == std::string::npos)
		return ;
	_raw_headers += strptr.substr(0, body_start + 4);
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
			this->_attributes.insert(std::pair<std::string, std::string>(key, value));
			key_start = i + 2;
		}
	}
	strptr = strptr.substr(body_start + 4);
}

void	Request::read_body(std::string &strptr)
{
	this->_raw_body += strptr;
	strptr.clear();
}

void	Request::parse(Socket & socket, struct kevent const & Event )
{
	socket.receive(Event.data);
	std::string		raw_request = socket.to_s();
	if (raw_request.empty() || socket.bytes() < 0)
		throw std::exception(); // TODO: decide what error this is
	this->_raw_request += raw_request.substr(0);
	this->_unparsed_request += raw_request.substr(0);

	if (request_line.empty() || request_line.find('\n') == std::string::npos)
		read_request_line(_unparsed_request);
	if (_raw_headers.empty() || _raw_headers.find("\r\n\r\n") == std::string::npos)
		read_header(_unparsed_request);
	if (_attributes.count("Content-Length"))
		read_body(_unparsed_request);

	// set accept_* values
	// setup_forwarded_info();
	// request_uri = parse_uri(unparsed_uri);
}
