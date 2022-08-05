UNAME  :=      $(shell uname)
ifeq ($(UNAME), Darwin)
       CXX := c++
else
       CXX := g++
endif
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g
CPPFLAGS := -Iinc
NAME := webserv
SRCS := main.cpp \
		webserver.cpp \
		Server.cpp \
		ConfigParser.cpp \
		Listener.cpp \
		Connection.cpp \
		Request.cpp \
		Response.cpp \
		Socket.cpp \
		Handler.cpp \
		FileHandler.cpp \
		BaseStatus.cpp \
		url_utils.cpp \
		utils.cpp
VPATH = src/
OBJ_DIR := obj/
DEP_DIR := dep/
OBJS := $(SRCS:%.cpp=$(OBJ_DIR)%.o)
DEPS := $(SRCS:%.cpp=$(DEP_DIR)%.d)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d

all: fd_script $(NAME)

fd_script: # temporary while server doesn't clear open fd's
	@bash scripts/close_fds.sh

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

vm: # runs the app in a container
	docker run -it --rm -p 8080:8080 --name webserv webserv

vm_build: # build the app in a container
	docker build --no-cache -t webserv .

vm_re: vm_build vm # re-builds and runs the app container

vm_clean: # stops docker and deletes images, and processes
	docker stop $$(docker ps -qa); \
	docker rm $$(docker ps -qa); \
	docker rmi -f $$(docker images -qa);

test_unit: # compiles and runs unit tests
	$(MAKE) -C test/unit

# To run individual unit tests from root:
#			`make socket -C test/unit`
#			`make parser -C test/unit`
# 		etc..

# Temporary while no linux version of code
# NOTE: Have ./webserv running in separate terminal
test_e2e: # compiles and runs end-to-end tests
	cd test/e2e && npm test
#	cd test/e2e && docker-compose up --build --abort-on-container-exit

.PHONY: all clean fclean resetclean re test vm_build vm fd_script
