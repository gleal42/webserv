UNAME			:= $(shell uname)
CXX 			:= c++
CXXFLAGS 	:= -std=c++98
ifeq ($(UNAME), Linux)
		CXXFLAGS 	+= -DLINUX
endif
ifeq ($(UNAME), Darwin)
		CXXFLAGS 	+= -DDARWIN
endif
INC				:= -I../../inc
CPPFLAGS	:= $(INC)
COMPILE		:= $(CXX) $(CXXFLAGS) $(CPPFLAGS)


all:	socket listener connection parser status handler file_handler server event


##---  Socket  ---##
SOCKET := Socket.test.cpp ../../src/utils.cpp ../../src/Socket.cpp ../../src/print.cpp ../../src/BaseStatus.cpp

socket: Socket # run Socket class tests
	./Socket

# compile Socket class tests
Socket:	$(SOCKET) ../../inc/Socket.hpp
	$(COMPILE) $(SOCKET) -o $@


##---  Listener  ---##
LISTENER := Listener.test.cpp ../../src/utils.cpp ../../src/Listener.cpp \
	../../src/Socket.cpp ../../src/print.cpp ../../src/BaseStatus.cpp

listener: Listener # run Listener class tests
	./Listener

# compile Listener class tests
Listener:	$(LISTENER) ../../inc/Listener.hpp
	$(COMPILE) $(LISTENER) -o $@


##---  Connection  ---##
CONNECTION := Connection.test.cpp ../../src/utils.cpp ../../src/Connection.cpp \
	../../src/Socket.cpp ../../src/Listener.cpp ../../src/Request.cpp \
	../../src/Response.cpp ../../src/BaseStatus.cpp ../../src/print.cpp \
	../../src/file.cpp

connection: Connection # run Connection class tests
	./Connection

# compile Connection class tests
Connection:	$(CONNECTION) ../../inc/Connection.hpp
	$(COMPILE) $(CONNECTION) -o $@


##---  ConfigParser  ---##
PARSER := ConfigParser.test.cpp ../../src/ConfigParser.cpp

parser: ConfigParser # run ConfigParser class tests
	./ConfigParser

# compile ConfigParser class tests
ConfigParser:	$(PARSER) ../../inc/ConfigParser.hpp
	$(COMPILE) $(PARSER) -o $@


##---  HTTPStatus  ---##
STATUS := HTTPStatus.test.cpp ../../src/BaseStatus.cpp \
../../src/utils.cpp ../../src/print.cpp

status: HTTPStatus # run HTTPStatus class tests
	./HTTPStatus

# compile HTTPStatus class tests
HTTPStatus:	$(STATUS) ../../inc/HTTPStatus.hpp
	$(COMPILE) $(STATUS) -o $@


##---  Handler  ---##
HANDLER := Handler.test.cpp ../../src/Handler.cpp \
	../../src/BaseStatus.cpp ../../src/utils.cpp ../../src/url.cpp \
	../../src/Request.cpp ../../src/Socket.cpp ../../src/Response.cpp \
	../../src/print.cpp ../../src/file.cpp

handler: Handler # run Handler class tests
	./Handler

# compile Handler class tests
Handler:	$(HANDLER) ../../inc/Handler.hpp
	$(COMPILE) $(HANDLER) -o $@


##---  FileHandler  ---##
FILE_HANDLER := FileHandler.test.cpp ../../src/FileHandler.cpp \
	../../src/BaseStatus.cpp ../../src/utils.cpp ../../src/url.cpp \
	../../src/print.cpp ../../src/file.cpp ../../src/Handler.cpp ../../src/Request.cpp \
	../../src/Socket.cpp ../../src/Response.cpp

file_handler: FileHandler # run FileHandler class tests
	./FileHandler

# compile FileHandler class tests
FileHandler:	$(FILE_HANDLER) ../../inc/FileHandler.hpp
	$(COMPILE) $(FILE_HANDLER) -o $@


##---  Server  ---##
SERVER := Server.test.cpp ../../src/Server.cpp \
	../../src/BaseStatus.cpp ../../src/utils.cpp ../../src/Connection.cpp \
	../../src/Handler.cpp ../../src/FileHandler.cpp ../../src/CGIHandler.cpp\
	../../src/ConfigParser.cpp ../../src/Request.cpp ../../src/Listener.cpp \
	../../src/Response.cpp ../../src/url.cpp ../../src/Socket.cpp \
	../../src/Event.cpp ../../src/print.cpp ../../src/file.cpp

server: Server # run Server class tests
	./Server

# compile Server class tests
Server:	$(SERVER) ../../inc/Server.hpp
	$(COMPILE) $(SERVER) -o $@


##---  Event  ---##
EVENT := Event.test.cpp ../../src/Event.cpp

event: Event # run Event class tests
	./Event

# compile Event class tests
Event:	$(EVENT) ../../inc/Event.hpp
	$(COMPILE) $(EVENT) -o $@


.PHONY: all socket listener connection parser status handler file_handler \
	server event
