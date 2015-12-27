# php-go

php-go allows to call Go code from PHP.

This is a work in progress.

## Goals:

- Allow to _export_ Go functions and Go constants from Go to PHP
- Be reliable and always safe
- Build PHP extension once, use many independantly built Go modules; re-building a Go module makes new code immediately available in PHP

## TODO:

- [ ] Support exporting constants
- [ ] Support all scalar types
- [ ] Support slices, maps (copying)
- [ ] Support objects (proxying)

## Install

#### PHP ext:

    cd ext
    phpize && ./configure && make && sudo make install
    
Then add ``extension=phpgo.so`` to your php.ini, or call php with ``-dextension=phpgo.so``

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
  "toUpper": strings.ToLower,
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

