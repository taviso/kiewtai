#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <windows.h>
#include <malloc.h>

#include "kaitai.h"
#include "parsers.h"

//
// This file is part of Kiewtai, a port of Kaitai to the Hiew hex editor.
//
// This is just a test wrapper command to verify parsing is working.
//
// Tavis Ormandy <taviso@gmail.com>
//

VOID CALLBACK FieldNotify(PVOID UserPtr, LPCSTR Name, ULONG Start, ULONG End)
{
    printf("Field: %s Start: %lu End: %lu\n", Name, Start, End);
    return;
}

VOID CALLBACK ErrorNotify(PVOID UserPtr, LPCSTR Message)
{
    printf("Error: %s\n", Message);
    return;
}

int main(int argc, char *argv[]) {
    char *buffer = NULL;
    FILE *input = NULL;
    size_t len;

    if (argc != 3) {
        fprintf(stderr, "usage: %s [ParserName] [file]\n", argv[0]);
        goto error;
    }

    input = fopen(argv[2], "rb");

    if (input == NULL) {
        fprintf(stderr, "could not open file %s\n", argv[2]);
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
        fprintf(stderr, "failed to read file\n");
        goto error;
    }

    for (PKAITAI_PARSER Parser = &KaitaiParsers[0]; Parser->Name; Parser++) {
        if (strcmp(Parser->Name, argv[1]) == 0) {
            if (KaitaiQueryFormat(Parser,
                                  KAITAI_FLAG_EXPANDARRAYS,
                                  buffer,
                                  len,
                                  FieldNotify,
                                  ErrorNotify,
                                  NULL) == FALSE) {
                fprintf(stderr, "Parse Failure\n");
            }
            goto success;
        }
    }

    fprintf(stderr, "could not find matching parser\n");

error:
    if (buffer)
        free(buffer);
    if (input)
        fclose(input);
    return 1;

success:
    free(buffer);
    fclose(input);
    return 0;
}
