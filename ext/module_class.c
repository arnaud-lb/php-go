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

#include "php_phpgo.h"
#include "module.h"

typedef struct {
	zend_object  std;
	phpgo_module *module;
} module_intern;

zend_object_handlers module_object_handlers;

static zend_object_value module_new(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	module_intern *intern;

	intern = ecalloc(1, sizeof(*intern));
	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(&intern->std, (zend_objects_store_dtor_t) zend_objects_destroy_object, (zend_objects_free_object_storage_t) zend_objects_free_object_storage, NULL TSRMLS_CC);
	retval.handlers = &module_object_handlers;

	return retval;
}
/* }}} */

PHP_METHOD(PHPGo__Module, __fun)
{
	zval ***args;
	int argc;
	const char *fname;
	module_intern *intern;
	php_export **pexport;
	php_export *export;
	php_arg *ins;
	php_arg *outs;
	zend_function *active_function = EG(current_execute_data)->function_state.function;
	int i;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "*", &args, &argc) == FAILURE) {
		return;
	}

	fname = active_function->common.function_name;
	intern = (module_intern*)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_hash_find(&intern->module->exports, fname, strlen(fname)+1, (void**)&pexport) != SUCCESS) {
		php_error(E_ERROR, "Internal error: couldn't find export named `%s`", fname);
		return;
	}

	export = *pexport;

	if (argc != export->num_ins) {
		const char *class_name = active_function->common.scope ? active_function->common.scope->name : "";
		php_error(
			E_WARNING,
			"%s%s%s() expects %s %d parameter%s, %d given",
			class_name,
			class_name[0] ? "::" : "",
			fname,
			"exactly",
			(int) export->num_ins,
			export->num_ins == 1 ? "" : "s",
			argc
		);
		return;
	}

	ins = ecalloc(export->num_ins, sizeof(*ins));

	for (i = 0; i < export->num_ins; i++) {
		php_type *t = &export->ins[i];
		if (strcmp(t->kind, "int") == 0) {
			long l;
			if (zend_parse_parameter(0, i, args[i], "l", &l) != SUCCESS) {
				return;
			}
			ins[i].i = (int) l;
		} else {
			php_error(E_ERROR, "Interval error: unknown input type `%s`", t->kind);
		}
	}

	outs = intern->module->php_go_call_fun(export, ins);

	if (export->num_outs > 1) {
		array_init(return_value);
	}

	for (i = 0; i < export->num_outs; i++) {
		php_type *t = &export->outs[i];
		if (strcmp(t->kind, "int") == 0) {
			if (export->num_outs == 1) {
				RETVAL_LONG(outs[i].i);
			} else {
				add_next_index_long(return_value, outs[i].i);
			}
		} else {
			php_error(E_ERROR, "Interval error: unknown output type `%s`", t->kind);
		}
	}

	free(outs);
}

static void phpgo_add_method(zend_function_entry *fe, php_export *export)
{
	int argidx;
	zend_arg_info *args = ecalloc(export->num_ins+1, sizeof(*args));

	args[0].class_name_len = export->num_ins; // required num args

	for (argidx = 0; argidx < export->num_ins; argidx++) {
		php_type *t = &export->ins[argidx];
		char *buf;
		int len = spprintf(&buf, 0, "%s%d", t->kind, argidx);
		args[argidx+1].name = buf;
		args[argidx+1].name_len = len;
	}

	fe->fname = estrdup(export->name);
	fe->handler = ZEND_MN(PHPGo__Module___fun);
	fe->arg_info = args;
	fe->num_args = export->num_ins;
	fe->flags = ZEND_ACC_PUBLIC;
}

void phpgo_module_new_instance(zval *ret, phpgo_module *module)
{
	zend_class_entry tmpce;
	zend_class_entry *ce;
	char *class_name;
	module_intern *intern;
	zend_function_entry *module_fe;
	HashPosition pos;
	php_export **pexport;
	int fidx = -1;

	spprintf(&class_name, 0, "PHPGo\\Module\\%s_%x_%x", module->name, PHPGO_G(load_counter)++, rand());

	module_fe = ecalloc(zend_hash_num_elements(&module->exports)+1, sizeof(*module_fe));

	for (zend_hash_internal_pointer_reset_ex(&module->exports, &pos);
			zend_hash_get_current_data_ex(&module->exports, (void**)&pexport, &pos) == SUCCESS;
			zend_hash_move_forward_ex(&module->exports, &pos)) {
		fidx++;
		phpgo_add_method(&module_fe[fidx], *pexport);
	}

	INIT_CLASS_ENTRY_EX(tmpce, class_name, strlen(class_name), module_fe);

	ce = zend_register_internal_class(&tmpce TSRMLS_CC);
	ce->create_object = module_new;
	ce->ce_flags |= ZEND_ACC_FINAL_CLASS;

	object_init_ex(ret, ce);

	intern = (module_intern*)zend_object_store_get_object(ret TSRMLS_CC);
	intern->module = module;
}

void phpgo_module_class_init()
{
	memcpy(&module_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	module_object_handlers.clone_obj = NULL;
}
