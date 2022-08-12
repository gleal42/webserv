# https://hub.docker.com/_/gcc
FROM debian:buster

RUN apt-get update
RUN apt-get upgrade -y

RUN apt-get install -y vim
RUN apt-get install -y wget
RUN apt-get install -y unzip
RUN apt-get install -y curl
RUN apt-get install -y gcc

RUN apt-get install -y build-essential
RUN apt-get install -y zlib1g
RUN apt-get install -y zlib1g-dev
RUN apt-get install -y libpcre3
RUN apt-get install -y libpcre3-dev

# RUN apt-get install -y nginx
# RUN apt-get install -y nginx-extras

RUN apt-get -y install php7.3
RUN apt-get -y install php7.3-fpm

COPY nginx/test.conf /tmp/test.conf

RUN rm -rf /etc/nginx/sites-enabled/default
RUN rm -f var/www/html/index.nginx-debian.html
RUN rm -f var/www/html/index.html

COPY public /var/www/
COPY www /var/www/

EXPOSE 80

COPY nginx/*.sh .
CMD bash start.sh
