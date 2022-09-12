FROM debian:buster

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get -y install build-essential php7.3 php-cgi lldb lsof

# Create app directory
RUN mkdir -p /usr/src/webserv
WORKDIR /usr/src/webserv

# Copy source files
COPY . /usr/src/webserv

# Compile
RUN make

EXPOSE 8080

# Run
ENTRYPOINT ["./scripts/start.sh"]
