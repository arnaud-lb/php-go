/*
  +----------------------------------------------------------------------+
  | php-go                                                               |
  +----------------------------------------------------------------------+
  | Copyright (c) 2015-2016 Arnaud Le Blanc                              |
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_phpgo.h"
#include "module.h"
#include "module_class.h"

ZEND_DECLARE_MODULE_GLOBALS(phpgo)

int le_phpgo;

/* {{{ phpgo_functions[]
 */
const zend_function_entry phpgo_functions[] = {
	PHP_FE(phpgo_load,	NULL)
	PHP_FE_END
};
/* }}} */

/* {{{ phpgo_module_entry
 */
zend_module_entry phpgo_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"phpgo",
	phpgo_functions,
	PHP_MINIT(phpgo),
	PHP_MSHUTDOWN(phpgo),
	PHP_RINIT(phpgo),
	PHP_RSHUTDOWN(phpgo),
	PHP_MINFO(phpgo),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_PHPGO_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PHPGO
ZEND_GET_MODULE(phpgo)
#endif

/* {{{ php_phpgo_init_globals
 */
static void php_phpgo_init_globals(zend_phpgo_globals *phpgo_globals TSRMLS_DC)
{
	phpgo_globals->load_counter = 0;
	zend_hash_init(&phpgo_globals->modules, 0, NULL, phpgo_module_dtor, 1);
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(phpgo)
{
	ZEND_INIT_MODULE_GLOBALS(phpgo, php_phpgo_init_globals, NULL);
	phpgo_module_class_init();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(phpgo)
{
	zend_hash_destroy(&PHPGO_G(modules));
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(phpgo)
{
	zend_llist_init(&PHPGO_G(classes), sizeof(zend_class_entry*), phpgo_module_class_list_dtor, 0);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(phpgo)
{
	zend_llist_destroy(&PHPGO_G(classes));
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(phpgo)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "phpgo", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ proto string phpgo_load(string path, string name)
   Loads a Go module and returns a class instance */
PHP_FUNCTION(phpgo_load)
{
	char *path;
	size_t path_len;
	char *name;
	size_t name_len;

	phpgo_module *module;
	char *err;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ps", &path, &path_len, &name, &name_len) == FAILURE) {
		return;
	}

	err = phpgo_module_load(&module, path, name TSRMLS_CC);
	if (err) {
		php_error(E_WARNING, "Failed loading %s (%s): %s", path, name, err);
		efree(err);
		RETURN_FALSE;
	}

	phpgo_module_new_instance(return_value, module TSRMLS_CC);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
