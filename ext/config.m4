dnl $Id$
dnl config.m4 for extension phpgo

PHP_ARG_ENABLE(phpgo, whether to enable phpgo support,
   [  --enable-phpgo           Enable phpgo])

if test "$PHP_PHPGO" != "no"; then
  PHP_ADD_INCLUDE(../php-go)
  PHP_NEW_EXTENSION(phpgo, phpgo.c module.c module_class.c, $ext_shared)
fi
