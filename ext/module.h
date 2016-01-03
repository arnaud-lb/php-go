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

#ifndef MODULE_H
#define MODULE_H

#include "php.h"
#include "phpgo.h"
#include "php_phpgo.h"

typedef struct {
	char *name;
	HashTable exports;
	PHPGoCallFun php_go_call_fun;
} phpgo_module;

char* phpgo_module_load(phpgo_module **module_pp, const char *path, const char *name TSRMLS_DC);
void phpgo_module_free(phpgo_module *module);
void phpgo_module_dtor(void *data);

#endif /* MODULE_H */

