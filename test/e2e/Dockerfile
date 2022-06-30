FROM node:14.15.5

# Create test directory
RUN mkdir -p /usr/src/test
WORKDIR /usr/src/test

# Install depedencies
COPY package.json /usr/src/test
RUN npm install

# Bundle test src
COPY . /usr/src/test

CMD [ "npm" , "test" ]
