# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    compare_with_nginx.sh                              :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/16 15:22:00 by gleal             #+#    #+#              #
#    Updated: 2022/08/16 15:23:24 by gleal            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

docker build -t nginx_server .
docker run --rm -it --name ft_server -p 80:80 nginx_server
