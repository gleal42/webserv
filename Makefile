CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g
CPPFLAGS := -Iinc
NAME := webserv
SRCS := main.cpp \
		webserver.cpp \
		Server.cpp \
		Response.cpp \
		Request.cpp \
		ServerConfig.cpp
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

tests_unit: # compiles and runs all unit tests
	$(MAKE) unit -C tests

# To run individual unit tests from root:
#			`make socket -C tests/unit`
#			`make parser -C tests/unit`
# 		etc..

tests_e2e:
	$(MAKE) e2e -C tests

.PHONY: all clean fclean resetclean re
