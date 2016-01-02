all: test.so

test.so: test.go ../../../../php-go/*
	GOPATH=$$(pwd)/../../../../../../../../ go build -o test.so -buildmode c-shared
