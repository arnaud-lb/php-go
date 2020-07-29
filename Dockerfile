FROM php:7.4-cli

RUN apt-get update
RUN apt-get install -y wget

# Install Golang
RUN wget https://dl.google.com/go/go1.13.3.linux-amd64.tar.gz
RUN tar -xvf go1.13.3.linux-amd64.tar.gz -C /usr/local
ENV PATH=/usr/local/go/bin:$PATH
ENV GOROOT=/usr/local/go

# Build php-go
COPY . /gopath/src/github.com/arnaud-lb/php-go
ENV GOPATH=/gopath
WORKDIR /gopath/src/github.com/arnaud-lb/php-go/ext/tests/fixtures/go
RUN go build -o test.so -buildmode c-shared

# Install the extension
WORKDIR /gopath/src/github.com/arnaud-lb/php-go/ext
RUN phpize && ./configure && make && make install
RUN echo "extension = phpgo.so" >> /gopath/src/github.com/arnaud-lb/php-go/ext/tmp-php.ini

# Execute the test
CMD cd /gopath/src/github.com/arnaud-lb/php-go/ext && ../local-test.sh