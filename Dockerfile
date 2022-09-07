FROM debian:buster

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get -y install php7.3 build-essential

# Create app directory
RUN mkdir -p /usr/src/webserv
WORKDIR /usr/src/webserv

# Copy source files
COPY . /usr/src/webserv

# Compile
RUN make

# Env
ENV CONFIG_FILE ""

# Run
CMD ./scripts/start.sh ${CONFIG_FILE}



