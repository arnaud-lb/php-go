# php-go

php-go allows to call Go code from PHP.

This is a work in progress.

## Goals:

- Allow to _export_ Go functions and Go constants from Go to PHP
- Be reliable and always safe
- Build PHP extension once, use many independantly built Go modules; re-building a Go module makes new code immediately available in PHP

## TODO:

- [ ] Finish PHP extension
- [ ] Support exporting constants
- [ ] Support all scalar types
- [ ] Support slices, maps (copying)
- [ ] Support objects (proxying)
