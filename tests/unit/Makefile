CXX := c++
CXXFLAGS := -std=c++98
INC := -I../../inc
CPPFLAGS := $(INC)
COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)

all:	socket parser webserv

##---  Socket  ---##
SOCKET := Socket.test.cpp ../../srcs/Socket.cpp

socket: Socket # run Socket class tests
	./Socket

# compile Socket class tests
Socket:	$(SOCKET)
	$(COMPILE) $(SOCKET) -o $@

##---  Parser  ---##
PARSER := Parser.test.cpp ../../srcs/Parser.cpp ../../srcs/ServerConfig.cpp

parser: Parser # run Parser class tests
	./Parser

# compile Parser class tests
Parser:	$(PARSER)
	$(COMPILE) $(PARSER) -o $@

##---  webserv  ---##
WEBSERV := webserver.test.cpp ../../srcs/webserver.cpp ../../srcs/Parser.cpp \
	../../srcs/ServerConfig.cpp ../../srcs/Server.cpp

webserv: Webserv # run webserver file tests
	./Webserv

# compile webserver file tests
Webserv:	$(WEBSERV)
	$(COMPILE) $(WEBSERV) -o $@

.PHONY: all socket parser webserv
