#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

#include <iostream>

# define LOG(x) std::cout << x << std::endl
# define ERROR(x) std::cerr << x << std::endl

void	webserver(char *config);

#endif
