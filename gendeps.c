#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <windows.h>
#include <malloc.h>

#include "duktape.h"

// This file is part of Kiewtai, a port of Kaitai to the Hiew hex editor.
//
// This is just a hack to generate dependencies for kaitai modules at build time.
//
// Tavis Ormandy <taviso@gmail.com>
//

bool find_and_resolve_module(duk_context *ctx, const char *name)
{
    char module[MAX_PATH];
    char *buffer = NULL;
    FILE *input = NULL;
    size_t len;

    snprintf(module, MAX_PATH, "%s.js", name);

    // Try various pathname combinations.
    if ((input = fopen(module, "rb")) == NULL
     && (input = fopen(name, "rb")) == NULL) {
        fprintf(stderr, "could not find module %s\n", name);
        goto error;
    }

    fseek(input, 0, SEEK_END);

    len = ftell(input);

    rewind(input);

    buffer = malloc(len);

    if (buffer == NULL) {
        fprintf(stderr, "memory allocation failed\n");
        goto error;
    }

    if (fread(buffer, 1, len, input) != len) {
        fprintf(stderr, "failed to read module\n");
        goto error;
    }

    duk_peval_lstring_noresult(ctx, buffer, len);

success:
    fwrite(buffer, 1, len, stdout);

    free(buffer);
    fclose(input);
    return true;

error:
    if (buffer)
        free(buffer);
    if (input)
        fclose(input);
    return false;
}

static duk_ret_t print_dependencies(duk_context *ctx)
{
    const char *dep = duk_to_string(ctx, 0);

    // Standard dependency, already available.
    if (strcmp(dep, "kaitai-struct/KaitaiStream") == 0)
        return 0;

    // Normalize
    dep = strrchr(dep, '/') + 1;

    duk_push_global_object(ctx);

    // Is it already defined?
    if (duk_has_prop_string(ctx, -1, dep)) {
        // Already known
        duk_pop(ctx);
        return 0;
    }

    // Define it
    duk_push_boolean(ctx, true);
    duk_put_global_string(ctx, dep);

    // And now parse and print it.
    find_and_resolve_module(ctx, dep);

    duk_pop(ctx);
    return 0;
}

int main(int argc, char *argv[])
{
    duk_context *ctx;

    if (argc != 2) {
        fprintf(stderr, "usage: %s [ParserName]\n", argv[0]);
        return 1;
    }

    ctx = duk_create_heap_default();

    duk_peval_string(ctx, "var module = { exports: true }");
    duk_push_c_function(ctx, print_dependencies, 1);
    duk_put_global_string(ctx, "require");

    // Define this module.
    duk_push_boolean(ctx, true);
    duk_put_global_string(ctx, argv[1]);

    if (find_and_resolve_module(ctx, argv[1])) {
        duk_destroy_heap(ctx);
        return 0;
    }

    fprintf(stderr, "dependency error\n");
    duk_destroy_heap(ctx);
    return 0;
}
