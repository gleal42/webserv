#include "Request.hpp"

Request::Request(int socket, sockaddr_in *sockaddr){
	int		addressLen = sizeof(sockaddr);
	std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";

	if ((_socket = accept(socket, (struct  sockaddr *)sockaddr, (socklen_t *)&addressLen)) < 0)
		throw("Accept error");
}
Request::Request(const Request& param) {
	// TODO (copy constructor)
	(void)param;
}

Request::~Request() {
	std::cout << "Request" << " destroyed" << std::endl;
	// TODO (destructor)
}

Request& Request::operator= (const Request& param) {
	// TODO (Assignment operatior)
	// std::swap()
	(void)param;
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const Request& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

std::string	Request::read_request_line(std::string buf){
	std::string::iterator			iter;
	std::string						str;
	int								i = 0;
	int								j = 0;

	for (iter = buf.begin(); *iter != ' '; iter++)
		i++;

	str = buf.substr(0, ++i);

	if (str.compare("GET"))
		_request_method = GET;
	else if(str.compare("POST"))
		_request_method = POST;
	else if(str.compare("DELETE"))
		_request_method = DELETE;
	else
		throw("No appropriate method");

	for (iter++; *iter != ' '; iter++)
		j++;

	_unparsed_uri = buf.substr(i, j);

	for (iter++; *iter != '\n'; iter++)
		j++;

	str = buf.substr(++j + ++i, buf.length());
	return (str);
}

std::string	Request::read_header(std::string buf){
	std::string			key;
	std::string			value;
	size_t				i = 0;
	size_t				key_start = 0;
	size_t				separator = 0;
	size_t				value_start;
	size_t				end;
	size_t				body_start = buf.find("\n\n");
	RequestAttributes	_attributes;

	for (std::string::iterator iter = buf.begin(); iter != buf.end() && i < body_start; iter++, i++){
		if (*iter == ':')
		{
			separator = i - key_start;
			key = buf.substr(key_start, separator);
			end = 0;
			i += 2;
			value_start = i;
			iter += 2;
			while (*iter != '\n'){
				end++;
				iter++;
				i++;
			}
				value = buf.substr(value_start, end);
				_attributes.insert(std::pair<std::string, std::string>(key, value));
				key_start = i + 1;
			}
	}
	if (body_start)
		return (buf.substr(body_start));
	else
		return (NULL);
}

void	Request::parse(void){
	char		buffer[30000] = {0};
	long		valread = recv(_socket, buffer, 30000, 0);

	if (!buffer || valread < 0)
		throw("Http header empty");

	std::string header(buffer);
	header = read_request_line(buffer);
	std::string body = read_header(header);
	// set accept_* values
	// setup_forwarded_info();
	// request_uri = parse_uri(unparsed_uri);
}
