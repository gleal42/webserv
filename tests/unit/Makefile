CXX := c++
CXXFLAGS := -std=c++98
CPPFLAGS := -I../../inc
COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)

all:	socket

socket: Socket # run Socket class tests
	./Socket

# compile Socket class tests
Socket:	../../srcs/Socket.cpp ../../inc/Socket.hpp Socket.test.cpp
	$(COMPILE) ../../srcs/Socket.cpp Socket.test.cpp -o $@

parser: Parser # run Parser class tests
	./Parser

# compile Parser class tests
Parser:	../../srcs/Parser.cpp ../../inc/Parser.hpp Parser.test.cpp
	$(COMPILE) ../../srcs/Parser.cpp Parser.test.cpp -o $@

.PHONY: all socket parser
