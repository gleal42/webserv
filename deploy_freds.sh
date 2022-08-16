# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    deploy_freds.sh                                    :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/16 15:22:00 by gleal             #+#    #+#              #
#    Updated: 2022/08/16 15:24:33 by gleal            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

docker build -f Dockerfile_webserv -t fred_server .
docker run --rm -it --name fred_server -p 80:80 fred_server
