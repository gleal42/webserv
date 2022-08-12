# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    compare_with_nginx.sh                              :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/07 15:39:10 by gleal             #+#    #+#              #
#    Updated: 2022/08/11 21:58:51 by gleal            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

docker build --build-arg ENABLED_MODULES="ndk lua" -t ft_server .
docker run --rm -it --name ft_server -p 80:80 ft_server
