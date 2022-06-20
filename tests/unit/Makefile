CXX := c++
CXXFLAGS := -std=c++98
CPPFLAGS := -I../../inc
COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)

all: 			Socket

Socket:
	$(COMPILE) ../../srcs/Socket.cpp Socket.test.cpp -o $@ \
	&& ./Socket

.PHONY: Socket
