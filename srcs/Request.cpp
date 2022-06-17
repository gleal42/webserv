#include "Request.hpp"

Request::Request(int socket, Config config){
	if (_socket = accept(socket, (struct  sockaddr *)&address, (socklen_t *)&addressLen)) < 0)
		throw("Accept");
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

void	Request::read_request_line(Socket socket){

}
