/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 Arnaud Le Blanc                              |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Arnaud Le Blanc <arnaud.lb@gmail.com>                        |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_PHPGO_H
#define PHP_PHPGO_H

extern zend_module_entry phpgo_module_entry;
#define phpext_phpgo_ptr &phpgo_module_entry

#define PHP_PHPGO_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_PHPGO_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_PHPGO_API __attribute__ ((visibility("default")))
#else
#	define PHP_PHPGO_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(phpgo);
PHP_MSHUTDOWN_FUNCTION(phpgo);
PHP_RINIT_FUNCTION(phpgo);
PHP_RSHUTDOWN_FUNCTION(phpgo);
PHP_MINFO_FUNCTION(phpgo);

PHP_FUNCTION(confirm_phpgo_compiled);	/* For testing, remove later. */

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(phpgo)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(phpgo)
*/

/* In every utility function you add that needs to use variables 
   in php_phpgo_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as PHPGO_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define PHPGO_G(v) TSRMG(phpgo_globals_id, zend_phpgo_globals *, v)
#else
#define PHPGO_G(v) (phpgo_globals.v)
#endif

#endif	/* PHP_PHPGO_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
