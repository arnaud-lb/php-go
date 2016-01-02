#include <stdlib.h>
#include <phpgo.h>

php_exports * new_php_exports(size_t num_exports) {
    php_exports * pes = malloc(sizeof(php_exports));
    pes->num_exports = 0;
    pes->exports = calloc(num_exports, sizeof(pes->exports[0]));
    return pes;
}

php_export * init_php_exports_export(php_exports * pes, size_t i, size_t num_ins, size_t num_outs) {
    php_export * pe = &pes->exports[i];
    pe->ins = calloc(num_ins, sizeof(pe->ins[0]));
    pe->outs = calloc(num_outs, sizeof(pe->outs[0]));
    return pe;
}

php_arg_desc * init_php_export_in(php_export * pe, size_t i) {
    return &pe->ins[i];
}

php_arg_desc * init_php_export_out(php_export * pe, size_t i) {
    return &pe->outs[i];
}

void free_php_exports(php_exports * pes) {
    size_t i;
    for (i = 0; i < pes->num_exports; i++) {
        php_export * pe = &pes->exports[i];
        free(pe->ins);
        free(pe->outs);
    }
    free(pes->exports);
    free(pes);
}
