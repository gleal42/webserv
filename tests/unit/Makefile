CXX := c++
CXXFLAGS := -std=c++98
CPPFLAGS := -I../../inc
COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)

all:	Socket
	./Socket

Socket:	../../srcs/Socket.cpp ../../inc/Socket.hpp Socket.test.cpp
	$(COMPILE) ../../srcs/Socket.cpp Socket.test.cpp -o $@

.PHONY: all
