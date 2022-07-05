CXX := c++
CXXFLAGS := -std=c++98
INC := -I../../inc
CPPFLAGS := $(INC)
COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)


all:	socket parser


##---  Socket  ---##
SOCKET := Socket.test.cpp ../../src/utils.cpp ../../src/Socket.cpp

socket: Socket # run Socket class tests
	./Socket

# compile Socket class tests
Socket:	$(SOCKET) ../../inc/Socket.hpp
	$(COMPILE) $(SOCKET) -o $@


##---  ConfigParser  ---##
PARSER := ConfigParser.test.cpp ../../src/ConfigParser.cpp

parser: ConfigParser # run ConfigParser class tests
	./ConfigParser

# compile ConfigParser class tests
ConfigParser:	$(PARSER) ../../inc/ConfigParser.hpp
	$(COMPILE) $(PARSER) -o $@


##---  HTTPStatus  ---##
STATUS := HTTPStatus.test.cpp

status: HTTPStatus # run HTTPStatus class tests
	./HTTPStatus

# compile HTTPStatus class tests
HTTPStatus:	$(STATUS) ../../inc/HTTPStatus.hpp
	$(COMPILE) $(STATUS) -o $@


##---  FileHandler  ---##
FILE_HANDLER := FileHandler.test.cpp ../../src/FileHandler.cpp

file_handler: FileHandler # run FileHandler class tests
	./FileHandler

# compile FileHandler class tests
FileHandler:	$(FILE_HANDLER) ../../inc/FileHandler.hpp
	$(COMPILE) $(FILE_HANDLER) -o $@


.PHONY: all socket parser status file_handler
