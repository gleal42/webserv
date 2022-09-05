# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    clean_containers.sh                                :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/07 20:31:30 by gleal             #+#    #+#              #
#    Updated: 2022/08/10 20:59:44 by gleal            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

docker rm $(docker container ls -qa)
docker rmi $(docker images -q -a )
docker system prune --force
