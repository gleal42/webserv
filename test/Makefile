CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98
NAME := client
SRCS := client.cpp
INC := 
OBJS := $(SRCS:.cpp=.o)
all: $(NAME)
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@
%.o: %.cpp $(INC) Makefile
	$(CXX) $(CXXFLAGS) $< -c -o $@
clean:
	$(RM) *.o
fclean: clean
	$(RM) $(NAME)
re: fclean all

.PHONY: all clean fclean re
