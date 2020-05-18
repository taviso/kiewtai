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
    char *buffer;
    FILE *input;
    size_t len;

    if (argc != 3) {
        fprintf(stderr, "usage: %s [ParserName] [file]\n", argv[0]);
        return 1;
    }

    input = fopen(argv[2], "rb");

    fseek(input, 0, SEEK_END);
    len = ftell(input);
    rewind(input);

    buffer = malloc(len);

    fread(buffer, len, 1, input);

    for (PKAITAI_PARSER Parser = &KaitaiParsers[0]; Parser->Name; Parser++) {
        if (strcmp(Parser->Name, argv[1]) == 0) {
            fprintf(stderr, "Detailed:\n");
            if (KaitaiQueryFormat(Parser,
                                  KAITAI_FLAG_EXPANDARRAYS,
                                  buffer,
                                  len,
                                  FieldNotify,
                                  ErrorNotify,
                                  NULL) == FALSE) {
                fprintf(stderr, "Parse Failure\n");
            }
            fprintf(stderr, "Simple:\n");
            if (KaitaiQueryFormat(Parser,
                                  KAITAI_FLAG_DEFAULT,
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

success:
    free(buffer);
    fclose(input);
    return 0;
}
