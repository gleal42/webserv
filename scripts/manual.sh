# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    manual.sh                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/16 15:22:00 by gleal             #+#    #+#              #
#    Updated: 2022/09/08 23:21:33 by gleal            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

docker build -t fred_server .
docker run --rm --security-opt seccomp=unconfined -it --name fred_server -v $(PWD):/usr/src/webserv -p 8080:8080 fred_server
# --cap-add=SYS_PTRACE