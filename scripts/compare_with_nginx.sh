# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    compare_with_nginx.sh                              :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/16 15:22:00 by gleal             #+#    #+#              #
#    Updated: 2022/09/11 20:27:00 by gleal            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

PORT1="-p 80:80"
# PORT1="-p 10000:10000"
# PORT2="-p 10001:10001"
# PORT3="-p 10003:10003"

docker build -t nginx_server -f Dockerfile_nginx .
docker run --rm -it --name ft_server $PORT1 $PORT2 $PORT3 nginx_server
