#include "Request.hpp"

Request::Request(int socket, sockaddr_in *sockaddr)
{
	(void)socket;
	(void)sockaddr;
}
Request::Request(const Request& param) {
	// TODO (copy constructor)
	(void)param;
}

Request::~Request() {
	std::cout << "Request" << " destroyed" << std::endl;
	close(_socket);
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
    return (buf);
}

int	Request::parse(int read_socket){
 {
	char		buffer[30000] = {0};

	long		valread;
	memset(buffer, '\0', sizeof(buffer));
	if ((valread = recv(read_socket, buffer, 30000, 0)) < 1)
	{
		close(read_socket);
		perror("Failed because ");
		throw("Error opening file");
	}
	if (!*buffer)
		throw("Http header empty");
	std::string header(buffer);
	header = read_request_line(buffer);
	read_header(header);
	// set accept_* values
	// setup_forwarded_info();
	// request_uri = parse_uri(unparsed_uri);
	}
    return (EXIT_SUCCESS);
}
