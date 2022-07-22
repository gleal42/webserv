/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 20:30:18 by msousa            #+#    #+#             */
/*   Updated: 2022/07/22 18:35:41 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Socket.hpp"
#include "Listener.hpp"
#include "HTTPStatus.hpp"

/* Constructors */
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
void	Request::read_request_line(std::string &_unparsed_request){
	int								i = 0;
	int								j = 0;
	std::string						buf (_unparsed_request);
	std::string::iterator			iter = buf.begin();
	RequestMethods	request_methods;

	request_methods["GET"] = GET;
	request_methods["POST"] = POST;
	request_methods["DELETE"] = DELETE;

	for (; *iter != ' '; iter++)
		i++;
	_unparsed_request = buf.substr(0, i++);

	// Consider global constant map: 	RequestMethods[ _unparsed_request ]
	// typedef std::map< std::string, RequestMethod >	RequestMethods;
	// this will avoid re-doing these comparisons every time
	if (_unparsed_request.compare("GET") == 0)
		request_method = GET;
	else if(_unparsed_request.compare("POST") == 0)
		request_method = POST;
	else if(_unparsed_request.compare("DELETE") == 0)
		request_method = DELETE;
	else
		throw("No appropriate method");
		// this error should have happened at the config parsing stage and blocked the loading of the server

	for (*(iter)++; *iter != ' '; iter++)
		j++;

	_unparsed_uri = buf.substr(i, j);
	// Temporary, TODO: make proper URI instance:
	// request_uri.parse(_unparsed_uri);
	// _path = request_uri.path;
	_path = _unparsed_uri;

	for (iter++; *iter != '\n'; iter++)
		j++;

	_raw_request_line = buf.substr(0, i + j + 2);
	std::cout << "Request line is :" << _raw_request_line << std::endl;
	_unparsed_request = buf.substr(++j + ++i);
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

	size_t	body_start = _unparsed_request.find("\r\n\r\n");
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
}

void	Request::read_body(std::string &_unparsed_request)
{
	join_strings(_raw_body, _unparsed_request);
	_unparsed_request.clear();
}

void	Request::parse(Socket & socket, struct kevent const & Event )
{
	socket.receive(Event.data);
	if (socket._buffer.empty() || socket.bytes() < 0)
		throw std::exception(); // TODO: decide what error this is

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

// Replace by template?

void	Request::append_buffer(std::string &str, std::vector<char> &to_add)
{
	if (!str.empty())
		str.pop_back();
	str.append(to_add.data(), to_add.size());
}

// Replace by template?

void	Request::join_strings(std::string &str, std::string &to_add)
{
	if (!str.empty())
		str.pop_back();
	str.append(to_add.data(), to_add.size());
}

void	Request::clear()
{
	_received.clear();
	_unparsed_request.clear();
	_raw_request_line.clear();
	_raw_headers.clear();
	_raw_body.clear();
	_path.clear();
	_headers.clear();
}
