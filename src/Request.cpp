/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 20:30:18 by msousa            #+#    #+#             */
/*   Updated: 2022/07/06 01:07:29 by gleal            ###   ########.fr       */
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
	input_buffer_size = config.input_buffer_size;
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
	input_buffer_size = param.input_buffer_size;
	_unparsed_uri = param._unparsed_uri;
	_path = param._path;
	_path_info = param._path_info;
	_query_string = param._query_string;
	_raw_header = param._raw_header;
	_header = param._header;
	_accept = param._accept;
	_accept_charset = param._accept_charset;
	_accept_encoding = param._accept_encoding;
	_accept_language = param._accept_language;
	// addr = param.addr;
	// peeraddr = param.peeraddr;
	_attributes = param._attributes;
	_request_time = param._request_time;
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
void	Request::read_request_line(std::string *strptr){
	std::string						str;
	int								i = 0;
	int								j = 0;
	std::string						buf = *strptr;
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

	str = buf.substr(++j + ++i, buf.length());
	*strptr = str;
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
void	Request::read_header(std::string *strptr){
	std::string				key;
	std::string				value;
	size_t					key_start = 0;
	size_t					separator = 0;
	size_t					value_start;
	size_t					end;
	std::string				buf = *strptr;
	size_t					body_start = buf.find("\r\n\r\n");

	for (size_t i = 0; i < body_start; i++){
		if (buf[i] == ':')
		{
			separator = i - key_start;
			key = buf.substr(key_start, separator);
			end = 0;
			i += 2;
			value_start = i;
			while (buf[i] != '\r' && buf[i] != '\n'){
				end++;
				i++;
			}
			value = buf.substr(value_start, end);
			this->_header.insert(std::pair<std::string, std::string>(key, value));
			key_start = i + 2;
		}
	}
	if (body_start + 4 != buf.size())
		this->_raw_body = buf.substr(body_start + 4);
};

void	Request::parse(Socket & socket){
	socket.receive(input_buffer_size);
	std::string		raw_request = socket.to_s();
	if (raw_request.empty() || socket.bytes() < 0)
		throw std::exception(); // TODO: decide what error this is
	this->_raw_header = raw_request.substr(0);
	read_request_line(&raw_request);
	read_header(&raw_request);

	// set accept_* values
	// setup_forwarded_info();
	// request_uri = parse_uri(unparsed_uri);
}
