CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g
CPPFLAGS := -Iinc
NAME := webserv
SRCS := main.cpp \
		utils.cpp \
		ConfigParser.cpp \
		Request.cpp \
		Response.cpp \
		Server.cpp \
		Socket.cpp \
		webserver.cpp
VPATH = src/
OBJ_DIR := obj/
DEP_DIR := dep/
OBJS := $(SRCS:%.cpp=$(OBJ_DIR)%.o)
DEPS := $(SRCS:%.cpp=$(DEP_DIR)%.d)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d

all: $(NAME)

$(DEP_DIR): ; mkdir -p $@
$(OBJ_DIR): ; mkdir -p $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OBJS) -o $@

$(OBJ_DIR)%.o : %.cpp $(DEP_DIR)%.d Makefile | $(OBJ_DIR) $(DEP_DIR)
	$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

$(DEPS): Makefile
-include $(DEPS)

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)

fclean: clean
	$(RM) $(NAME)

resetclean: fclean clean
	$(RM) -r $(OBJ_DIR) $(DEP_DIR)

re: fclean all

vm: # compiles and runs the app in a container
	docker run -it --rm -p 8080:8080 --name webserv webserv

vm_build: # build the app in a container
	docker build --no-cache -t webserv .

vm_re: vm_build vm # re-builds and runs the app container

vm_clean: # stops docker and deletes all images, and processes
	docker stop $$(docker ps -qa); \
	docker rm $$(docker ps -qa); \
	docker rmi -f $$(docker images -qa);

test_unit: # compiles and runs all unit tests
	$(MAKE) -C test/unit

# To run individual unit tests from root:
#			`make socket -C test/unit`
#			`make parser -C test/unit`
# 		etc..

test_e2e: # compiles and runs end-to-end tests in the app container
# @docker-compose --profile test up --build --abort-on-container-exit

test:
	cd test/e2e && npm install

.PHONY: all clean fclean resetclean re test
