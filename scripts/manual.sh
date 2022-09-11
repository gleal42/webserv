# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    manual.sh                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/16 15:22:00 by gleal             #+#    #+#              #
#    Updated: 2022/09/11 09:48:26 by msousa           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

docker build -t manual_server .
docker run --rm --security-opt seccomp=unconfined -it --name manual_server -v $(PWD):/usr/src/webserv -p 80:80 manual_server
# --cap-add=SYS_PTRACE
