#!/bin/sh

eval "$(gimme 1.5.2)"

set -xe

mkdir "$(pwd)/gopath"
export GOPATH="$(pwd)/gopath"

go env

mkdir -p gopath/src/github.com/arnaud-lb
ln -s $(pwd) gopath/src/github.com/arnaud-lb/php-go
cd gopath/src/github.com/arnaud-lb/php-go

(
    cd ext
    ./prepare-tests.sh

    echo "extension = $(pwd)/modules/phpgo.so" >> ~/.phpenv/versions/$(phpenv version-name)/etc/php.ini
    phpenv config-rm xdebug.ini

    phpize
    CFLAGS="-Wall -Werror" ./configure
    make

    PHP=$(which php)
    REPORT_EXIT_STATUS=1 TEST_PHP_EXECUTABLE="$PHP" "$PHP" run-tests.php -q --show-diff
)
