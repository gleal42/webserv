/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 20:30:18 by msousa            #+#    #+#             */
/*   Updated: 2022/07/08 03:21:50 by gleal            ###   ########.fr       */
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
	_raw_request_line = param._raw_request_line;
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
void	Request::read_request_line(std::vector<char> &_unparsed_request){
	std::string						str;
	int								i = 0;
	int								j = 0;
	std::string						buf (std::string(_unparsed_request.data()));
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

	_raw_request_line = buf.substr(0, i + j + 2);
	std::cout << "Request line is :" << _raw_request_line << std::endl;
	str = buf.substr(++j + ++i);
	_unparsed_request.clear();
	_unparsed_request = std::vector<char>(str.begin(), str.end()); // in order to include the null characterq
	_unparsed_request.push_back('\0');
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


void	Request::read_header(std::vector<char> &_unparsed_request)
{
	std::string				key;
	std::string				value;
	std::string				strptr(_unparsed_request.data());
	size_t					key_start = 0;
	size_t					separator = 0;
	size_t					value_start;
	size_t					end;

	size_t	body_start = strptr.find("\r\n\r\n");
	if (body_start == std::string::npos)
		return ;
	_raw_headers += strptr.substr(0, body_start + 4);
	std::cout << "Headers are :" << _raw_headers << std::endl;
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
	std::string remaining = strptr.substr(body_start + 4);
	std::vector<char> a(remaining.begin(), remaining.end() + 1);
	_unparsed_request = a;
}

void	Request::read_body(std::vector<char> &_unparsed_request)
{
	join_char_vectors(_raw_body, _unparsed_request);
	_unparsed_request.clear();
}

void	Request::parse(Socket & socket, struct kevent const & Event )
{
	socket.receive(Event.data);
	if (socket._buffer.empty() || socket.bytes() < 0)
		throw std::exception(); // TODO: decide what error this is

	join_char_vectors(_raw_request, socket._buffer); // Maybe unnecessary
	std::cout << "We have received [" << _raw_request.size() << "] bytes in total." << std::endl;
	join_char_vectors(_unparsed_request, socket._buffer);

	if (_raw_request_line.empty() || _raw_request_line.find('\n') == std::string::npos)
		read_request_line(_unparsed_request);
	if (_raw_headers.empty() || _raw_headers.find("\r\n\r\n") == std::string::npos)
		read_header(_unparsed_request);
	if (_attributes.count("Content-Length"))
		read_body(_unparsed_request);

	// set accept_* values
	// setup_forwarded_info();
	// request_uri = parse_uri(unparsed_uri);
}

void	Request::join_char_vectors(std::vector<char> &original, std::vector<char> &to_add)
{
	if (!original.empty())
		original.pop_back();
	original.insert(original.end(), to_add.begin(), to_add.end()); 
}

void	Request::clear()
{
	_raw_request.clear();
	_unparsed_request.clear();
	_raw_request_line.clear();
	_raw_headers.clear();
	_raw_body.clear();
	_unparsed_uri.clear();
	_attributes.clear();
}