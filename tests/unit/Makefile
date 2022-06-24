CXX := c++
CXXFLAGS := -std=c++98
INC := -I../../inc
CPPFLAGS := $(INC)
COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)

all:	socket parser

##---  Socket  ---##
SOCKET := Socket.test.cpp ../../srcs/Socket.cpp

socket: Socket # run Socket class tests
	./Socket

# compile Socket class tests
Socket:	$(SOCKET)
	$(COMPILE) $(SOCKET) -o $@

##---  ConfigParser  ---##
PARSER := ConfigParser.test.cpp ../../srcs/ConfigParser.cpp

parser: ConfigParser # run ConfigParser class tests
	./ConfigParser

# compile ConfigParser class tests
ConfigParser:	$(PARSER)
	$(COMPILE) $(PARSER) -o $@

.PHONY: all socket parser
