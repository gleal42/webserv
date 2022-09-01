# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/31 15:23:05 by gleal             #+#    #+#              #
#    Updated: 2022/07/31 15:25:13 by gleal            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98
NAME := hello_html.cgi
SRCS := hello_html.cpp
OBJS := $(SRCS:.cpp=.o)
all: $(NAME)
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@
%.o: %.cpp Makefile
	$(CXX) $(CXXFLAGS) $< -c -o $@
clean:
	$(RM) *.o
fclean: clean
	$(RM) $(NAME)
re: fclean all

.PHONY: all clean fclean re