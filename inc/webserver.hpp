#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

#include <iostream>
#include <string>
#include <sys/event.h>

#include "macros.hpp"
#include "Server.hpp"

int webserver(std::string config);

#endif
