
#ifndef PHPGO_H
#define PHPGO_H

#include <stdlib.h>
#include <stdint.h>

#define PHPGO_API_VERSION 20151227

enum {
    PHPGO_KIND_BOOL = 1,
    PHPGO_KIND_INT,
    PHPGO_KIND_INT8,
    PHPGO_KIND_INT16,
    PHPGO_KIND_INT32,
    PHPGO_KIND_INT64,
    PHPGO_KIND_UINT,
    PHPGO_KIND_UINT8,
    PHPGO_KIND_UINT16,
    PHPGO_KIND_UINT32,
    PHPGO_KIND_UINT64,
    PHPGO_KIND_UINTPTR,
    PHPGO_KIND_FLOAT32,
    PHPGO_KIND_FLOAT64,
    PHPGO_KIND_STRING,
};

typedef struct {
    int kind;
    char * name;
} php_arg_desc;

typedef struct {
    char * name;
    size_t num_ins;
    size_t num_outs;
    php_arg_desc * ins;
    php_arg_desc * outs;
    void * g;
} php_export;

typedef struct {
    int32_t version;
    php_export * exports;
    size_t num_exports;
} php_exports;

typedef struct {
    char *s;
    int l;
} php_arg_string;

typedef union {
    uint8_t b;
    int64_t l;
    double d;
    php_arg_string s;
} php_arg;

typedef php_exports * (*PHPExportsFun)();
typedef php_arg * (*PHPGoCallFun)(php_export * pes, php_arg * args);

php_exports * new_php_exports(size_t num_exports);
php_export * init_php_exports_export(php_exports * pes, size_t i, size_t num_ins, size_t num_outs);
php_arg_desc * init_php_export_in(php_export * pe, size_t i);
php_arg_desc * init_php_export_out(php_export * pe, size_t i);

#endif /* PHPGO_H */

