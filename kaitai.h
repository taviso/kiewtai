#ifndef __KAITAI_H
#define __KAITAI_H

enum {
    KAITAI_FLAG_DEFAULT,
    KAITAI_FLAG_EXPANDARRAYS = 1 << 0,
    KAITAI_FLAG_PROCESSZLIB = 1 << 1,
};

// Convenience macro to load a binary into duktape.
#define DUK_LOAD_BINOBJ(ctx, name) do {                                 \
    extern char binary_ ## name ## _js_start[];                         \
    extern size_t binary_ ## name ## _js_size;                          \
    duk_peval_lstring_noresult(ctx,                                     \
                               binary_ ## name ## _js_start,            \
                               (ULONG) &binary_ ## name ## _js_size);   \
    } while (false)

typedef struct _KAITAI_PARSER {
     LPCSTR Name;
     PCHAR Start;
     ULONG Size;
} KAITAI_PARSER, *PKAITAI_PARSER;

typedef VOID (CALLBACK *PFIELD_CALLBACK)(PVOID UserPtr, LPCSTR Name, ULONG Start, ULONG End);
typedef VOID (CALLBACK *PERROR_CALLBACK)(PVOID UserPtr, LPCSTR ErrorString);

BOOL KaitaiQueryFormat(PKAITAI_PARSER Format,
                       ULONG Flags,
                       PVOID Buffer,
                       ULONG BufSize,
                       PFIELD_CALLBACK FieldCallback,
                       PERROR_CALLBACK ErrorCallback,
                       PVOID UserPtr);

#endif
