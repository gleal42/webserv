#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

#include <iostream>
#include <string>

# define LOG(x) std::cout << x << std::endl
# define ERROR(x) std::cerr << x << std::endl

int webserver(char *config);

#include "Server.hpp"

#endif
