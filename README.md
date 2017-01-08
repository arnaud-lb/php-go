# php-go

![Supported PHP versions: 7.x](https://img.shields.io/badge/php-7.x-blue.svg) [![Build Status](https://travis-ci.org/arnaud-lb/php-go.svg)](https://travis-ci.org/arnaud-lb/php-go)

php-go allows to call Go code from PHP, with minimal code boilerplate.

## Goals:

- Allow to _export_ Go functions and Go constants from Go to PHP
- Be reliable and always safe
- Deploy Go code without re-building the PHP extension

## TODO:

- [x] Support exporting functions
- [x] Support all scalar types in arguments and return values
- [ ] Support exporting constants
- [ ] Support slices, maps (copying)
- [ ] Support objects (proxying)

## Install

#### PHP ext:

    cd ext
    phpize && ./configure && make && sudo make install

Then add ``extension=phpgo.so`` to your php.ini, or call php with ``-dextension=phpgo.so``

Note: php-go supports PHP 7 (non-ZTS). For PHP 5, use the php5 branch.

#### Go module:

    go get github.com/arnaud-lb/php-go/php-go
  
## Usage

#### Exporting Go functions

``` go
package main

import (
  "strings"
  "github.com/arnaud-lb/php-go/php-go"
)

// call php.Export() for its side effects
var _ = php.Export("example", map[string]interface{}{
  "toUpper": strings.ToUpper,
  "takeOverTheWorld": TakeOverTheWorld,
}

func TakeOverTheWorld() {
}

func main() {
}
```

The module can then be compiled as a shared library using `-buildmode c-shared`:

    go build -o example.so -buildmode c-shared .

Note: Go **requires** that the module be a _main_ package with a _main_ function in this mode.

#### Using the module in PHP

``` php
// Create a class from the Go module, and return an instance of it
$module = phpgo_load("/path/to/example.so", "example");

// Display the methods defined by the class
ReflectionClass::export($module);

// Call some method
$module->toUpper("foo");
```

