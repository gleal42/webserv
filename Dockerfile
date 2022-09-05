# https://hub.docker.com/_/gcc
FROM gcc:latest

# Create app directory
RUN mkdir -p /usr/src/webserv
WORKDIR /usr/src/webserv

# Copy source files
COPY . /usr/src/webserv

# Compile
RUN make

# Run
CMD ["./webserv", "test/test.conf"]
