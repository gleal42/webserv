/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 20:30:18 by msousa            #+#    #+#             */
/*   Updated: 2022/06/23 10:08:27 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request( void ) { /* no-op */ }

Request::Request(const ServerConfig & config){
	// TODO (implement constructor)
	// set member vars from config
	// even though we aren't using in `parse` might still be needed for counting
	// how much read and that can still read, if not remove
	input_buffer_size = config.input_buffer_size;
}

Request::Request(const Request& param) {
	// TODO (copy constructor)
	(void)param;
}

Request::~Request() {
	std::cout << "Request" << " destroyed" << std::endl;
	// TODO (destructor)
}

// might not be needed so can be private and have no implementation
Request& Request::operator= (const Request& param) {
	// TODO (Assignment operatior)
	// std::swap()
	(void)param;
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
	std::string		raw_request;

	raw_request = socket.to_s();
	if (raw_request.empty() || socket.bytes() < 0)
		throw std::exception();
	this->_raw_header = raw_request.substr(0);
	read_request_line(&raw_request);
	read_header(&raw_request);

	// set accept_* values
	// setup_forwarded_info();
	// request_uri = parse_uri(unparsed_uri);
}
