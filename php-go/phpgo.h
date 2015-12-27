
#ifndef PHPGO_H
#define PHPGO_H

#include <stdlib.h>
#include <stdint.h>

#define PHPGO_API_VERSION 20151227

typedef struct {
    char * kind;
} php_type;

typedef struct {
    char * name;
    size_t num_ins;
    size_t num_outs;
    php_type * ins;
    php_type * outs;
    void * g;
} php_export;

typedef struct {
    int32_t version;
    php_export * exports;
    size_t num_exports;
} php_exports;

typedef union {
    int i;
} php_arg;

typedef php_exports * (*PHPExportsFun)();
typedef php_arg * (*PHPGoCallFun)(php_export * pes, php_arg * args);

php_exports * new_php_exports(size_t num_exports);
php_export * init_php_exports_export(php_exports * pes, size_t i, size_t num_ins, size_t num_outs);
php_type * init_php_export_in(php_export * pe, size_t i);
php_type * init_php_export_out(php_export * pe, size_t i);

#endif /* PHPGO_H */

