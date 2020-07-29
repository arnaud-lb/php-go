#!/bin/bash
NO_INTERACTION=1 make test 2>&1
if test $? -gt 0
then
  for f in /gopath/src/github.com/arnaud-lb/php-go/ext/tests/*.diff
    do
      printf "\n\n"
      printf "=====================================================================\n"
      printf "Errors in $f\n"
      printf "=====================================================================\n\n"
      printf "$(cat $f)\n"
    done
fi

