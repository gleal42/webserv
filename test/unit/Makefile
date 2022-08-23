CXX := c++
CXXFLAGS := -std=c++98
INC := -I../../inc
CPPFLAGS := $(INC)
COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)


all:	socket parser status file_handler server


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
STATUS := HTTPStatus.test.cpp ../../src/BaseStatus.cpp ../../src/utils.cpp

status: HTTPStatus # run HTTPStatus class tests
	./HTTPStatus

# compile HTTPStatus class tests
HTTPStatus:	$(STATUS) ../../inc/HTTPStatus.hpp
	$(COMPILE) $(STATUS) -o $@


##---  Handler  ---##
HANDLER := Handler.test.cpp ../../src/Handler.cpp \
	../../src/BaseStatus.cpp ../../src/utils.cpp ../../src/url_utils.cpp \
	../../src/Request.cpp ../../src/Socket.cpp ../../src/Response.cpp

handler: Handler # run Handler class tests
	./Handler

# compile Handler class tests
Handler:	$(HANDLER) ../../inc/Handler.hpp
	$(COMPILE) $(HANDLER) -o $@


##---  FileHandler  ---##
FILE_HANDLER := FileHandler.test.cpp ../../src/FileHandler.cpp \
	../../src/BaseStatus.cpp ../../src/utils.cpp ../../src/url_utils.cpp \
	../../src/Handler.cpp ../../src/Request.cpp ../../src/Socket.cpp \
	../../src/Response.cpp

file_handler: FileHandler # run FileHandler class tests
	./FileHandler

# compile FileHandler class tests
FileHandler:	$(FILE_HANDLER) ../../inc/FileHandler.hpp
	$(COMPILE) $(FILE_HANDLER) -o $@


##---  Server  ---##
SERVER := Server.test.cpp ../../src/Server.cpp \
	../../src/BaseStatus.cpp ../../src/utils.cpp ../../src/Connection.cpp \
	../../src/Handler.cpp ../../src/FileHandler.cpp ../../src/ConfigParser.cpp \
	../../src/Request.cpp ../../src/Listener.cpp ../../src/Response.cpp \
	../../src/url_utils.cpp ../../src/Socket.cpp

server: Server # run Server class tests
	./Server

# compile Server class tests
Server:	$(SERVER) ../../inc/Server.hpp
	$(COMPILE) $(SERVER) -o $@


.PHONY: all socket parser status file_handler server
