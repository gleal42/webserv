CXX := c++
CXXFLAGS := -std=c++98
INC := -I../../inc
CPPFLAGS := $(INC)
COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)



all:	socket parser kqueue

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

KQUEUE := Kqueue.test.cpp \
		../../src/Kqueue.cpp \
		../../src/Server.cpp \
		../../src/Socket.cpp \
		../../src/Response.cpp \
		../../src/utils.cpp \
		../../src/Request.cpp

kqueue: Kqueue # run Kqueue class tests
	./Kqueue

# # compile Kqueue class tests
Kqueue:	$(KQUEUE) ../../inc/Kqueue.hpp
	$(COMPILE) $(KQUEUE) -o $@

.PHONY: all socket parser status kqueue
