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
VPATH = srcs/
OBJ_DIR := objs/
DEP_DIR := deps/
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

vm_re: # re-builds and runs the app container
	@docker build --no-cache -t webserv . && \
	docker run -it --rm -p 8080:8080 --name webserv webserv

vm_clean: # stops docker and deletes all app container images, and processes
	@docker stop $$(docker ps -qa); \
	docker rm $$(docker ps -qa); \
	docker rmi -f $$(docker images -qa);

tests_unit: # compiles and runs all unit tests
	$(MAKE) -C tests/unit

# To run individual unit tests from root:
#			`make socket -C tests/unit`
#			`make parser -C tests/unit`
# 		etc..

tests_e2e: # compiles and runs end-to-end tests in the app container
# @docker-compose --profile test up --build --abort-on-container-exit

.PHONY: all clean fclean resetclean re
