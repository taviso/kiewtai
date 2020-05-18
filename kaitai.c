#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <windows.h>
#include <malloc.h>
#include <setjmp.h>
#include <assert.h>
#include "duktape.h"

#include "kaitai.h"
#include "parsers.h"

//
//
// This file is part of Kiewtai, a port of Kaitai to the Hiew hex editor.
//
// Tavis Ormandy <taviso@gmail.com
//
//

#define MAX_ERROR 256

typedef jmp_buf JMPBUF, *PJMPBUF;

typedef struct _PARSE_CONTEXT {
    ULONG Flags;
    PVOID UserPtr;
    PFIELD_CALLBACK FieldCallback;
    PERROR_CALLBACK ErrorCallback;
    JMPBUF FatalError;
} PARSE_CONTEXT, *PPARSE_CONTEXT;

// The stack is expected to have an Object and a name to dump, they
// are not consumed, you should pop them.
static void dump_debug_data(duk_context *ctx, PPARSE_CONTEXT Context)
{
    duk_idx_t top = duk_get_top(ctx);

    // Make sure we can handle this object.
    if (!duk_is_object(ctx, -1) || duk_is_string(ctx, -1))
        return;

    // OK, this is bullshit and confused the hell out of me. Kaitai "lazily"
    // defines the debug objects inside getters, so you can't enumerate them
    // until you've fetched them. I don't want the objects, but I have to get
    // everything anyway  :(
    duk_enum(ctx, -1, DUK_ENUM_INCLUDE_NONENUMERABLE);

    while (duk_next(ctx, -1, true)) {
        duk_pop_n(ctx, 2);
    }

    duk_pop(ctx);

    // Arrays dont have a _debug property themselves, only their elements.
    if (duk_is_array(ctx, -1)) {

        // Check if the user wants us to dump this.
        if (Context->Flags & KAITAI_FLAG_EXPANDARRAYS) {
            duk_enum(ctx, -1, DUK_ENUM_ARRAY_INDICES_ONLY);

            while (duk_next(ctx, -1, true)) {
                // Construct a better name, by preprending parent object name.
                duk_dup(ctx, -5);           // ParentObjectName
                duk_push_string(ctx, "[");  //
                duk_dup(ctx, -4);           // RecordNum
                duk_push_string(ctx, "]");  //
                duk_concat(ctx, 4);
                //fprintf(stderr, "about to dump %s\n", duk_get_string(ctx, -1));
                duk_replace(ctx, -3);
                // Handle this recursively
                dump_debug_data(ctx, Context);

                // [ Key, Value ]
                duk_pop_n(ctx, 2);
            }

            duk_pop(ctx); // Enumerator
        }

        assert(top == duk_get_top(ctx));
        return;
    }

    // Check if this property has data we need to enumerate.
    if (duk_get_prop_string(ctx, -1, "_debug") == false) {
        duk_pop(ctx);   // undefined
        assert(top == duk_get_top(ctx));
        return;
    }

    // Enumerate all the debug entries.
    duk_enum(ctx, -1, DUK_ENUM_OWN_PROPERTIES_ONLY);

    while (duk_next(ctx, -1, true)) {
        int base = 0;

        // This is some pseudo object?
        // > parsedElf.header.programHeaders[3].flagsObj._debug._m_maskProc
        // {}
        if (!duk_has_prop_string(ctx, -1, "start")
         || !duk_has_prop_string(ctx, -1, "end")
         || !duk_has_prop_string(ctx, -1, "ioOffset")) {
            duk_pop_n(ctx, 2);
            continue;
        }

        // Construct a better name, by preprending parent object name.
        duk_dup(ctx, -6);           // ParentObjectName
        duk_push_string(ctx, ".");  // FieldSeparator
        duk_dup(ctx, -4);           // PropName
        duk_concat(ctx, 3);
        duk_replace(ctx, -3);

        duk_get_prop_string(ctx, -1, "start");
        duk_get_prop_string(ctx, -2, "end");
        duk_get_prop_string(ctx, -3, "ioOffset");

        // The start is relative to the ioOffset.
        base = duk_get_int(ctx, -1);

        // Notify caller.
        Context->FieldCallback(Context->UserPtr,
                               duk_get_string(ctx, -5),
                               base + duk_get_int(ctx, -3),
                               base + duk_get_int(ctx, -2));

        // If there was an 'arr' property, this indicates this object is a
        // group of records.
        // TODO: should I call the callback for this?
        if (duk_get_prop_string(ctx, -4, "arr")) {
            duk_enum(ctx, -1, DUK_ENUM_ARRAY_INDICES_ONLY);

            while (duk_next(ctx, -1, true)) {
                // Construct a better name, by preprending parent object name.
                duk_dup(ctx, -9);           // ParentObjectName
                duk_push_string(ctx, "[");  //
                duk_dup(ctx, -4);           // RecordNum
                duk_push_string(ctx, "]");  //
                duk_concat(ctx, 4);
                duk_replace(ctx, -3);
                //fprintf(stderr, "Array: %s\n", duk_get_string(ctx, -2));
                duk_pop_n(ctx, 2);
            }

            // Enumerator
            duk_pop(ctx);
        }

        // [ Key, Value, Start, End, ioOffset, arr (or undefined) ]
        duk_pop_n(ctx, 6);
    }

    // [ _debug, Enumerator ]
    duk_pop_n(ctx, 2);

    // Now we need to enumerate again to see if it has any sub-objects.
    duk_enum(ctx, -1, DUK_ENUM_OWN_PROPERTIES_ONLY);

    while (duk_next(ctx, -1, true)) {
        const char *keyname = duk_get_string(ctx, -2);

        // fprintf(stderr, "examining %s...\n", keyname);

        // Ignore non-object properties, which contain extracted data.
        if (duk_is_object(ctx, -1) == false) {
            // fprintf(stderr, "%s is not a object\n", keyname);
            goto nextprop;
        }

        // Internal properties we shouldn't dump.
        if (strcmp(keyname, "_root") == 0
         || strcmp(keyname, "_parent") == 0
         || strcmp(keyname, "_debug") == 0
         || strcmp(keyname, "_io") == 0) {
            goto nextprop;
        }

        // It is an interesting object, first make a better name for it.
        duk_dup(ctx, -5);           // ParentObjectName
        duk_push_string(ctx, ".");  // FieldSeparator
        duk_dup(ctx, -4);           // PropName
        duk_concat(ctx, 3);
        duk_replace(ctx, -3);

        dump_debug_data(ctx, Context);

      nextprop:
        duk_pop_n(ctx, 2);  // [ Obj, Name ]
    }

    duk_pop(ctx); // [ Enumerator ]

    assert(top == duk_get_top(ctx));
    return;
}

static VOID DukFatalError(PPARSE_CONTEXT Context, LPCSTR Message)
{
    Context->ErrorCallback(Context->UserPtr, Message);
    longjmp(Context->FatalError, true);
}

BOOL KaitaiQueryFormat(PKAITAI_PARSER Format,
                       ULONG Flags,
                       PVOID Buffer,
                       ULONG BufSize,
                       PFIELD_CALLBACK FieldCallback,
                       PERROR_CALLBACK ErrorCallback,
                       PVOID UserPtr)
{
    CHAR ErrorBuf[MAX_ERROR];
    duk_context *ctx;
    PARSE_CONTEXT ParseContext = {
        .Flags = Flags,
        .UserPtr = UserPtr,
        .ErrorCallback = ErrorCallback,
        .FieldCallback = FieldCallback,
    };

    // Create a new parser context.
    ctx = duk_create_heap(NULL, NULL, NULL, &ParseContext, DukFatalError);

    // Setup a jmpbuf to handle fatal duktape exceptions.
    if (setjmp(ParseContext.FatalError)) {
        goto error;
    }

    // Initialize some object dependecies.
    DUK_LOAD_BINOBJ(ctx, PolyFill);
    DUK_LOAD_BINOBJ(ctx, KaitaiStream);
    DUK_LOAD_BINOBJ(ctx, pako_inflate);

    // Load the requested parser.
    duk_peval_lstring_noresult(ctx, Format->Start, Format->Size);

    // The name of the current object.
    duk_push_string(ctx, Format->Name);
    duk_peval_string(ctx, Format->Name);
    duk_peval_string(ctx, "KaitaiStream");

    memcpy(duk_push_buffer(ctx, BufSize, false), Buffer, BufSize);

    if (duk_pnew(ctx, 1) != 0) {
        snprintf(ErrorBuf,
                 MAX_ERROR,
                 "KaitaiStream: %s",
                 duk_safe_to_string(ctx, -1));
        ErrorCallback(UserPtr, ErrorBuf);
        goto error;
    }

    if (duk_pnew(ctx, 1) != 0) {
        snprintf(ErrorBuf,
                 MAX_ERROR,
                 "%s: %s",
                 Format->Name,
                 duk_safe_to_string(ctx, -1));
        ErrorCallback(UserPtr, ErrorBuf);
        goto error;
    }

    duk_dup(ctx, -1);
    duk_push_string(ctx, "_read");

    // Call _read() to initialize some properties.
    if (duk_pcall_prop(ctx, -2, 0) != DUK_EXEC_SUCCESS ) {
        snprintf(ErrorBuf,
                 MAX_ERROR,
                 "%s._read(): %s",
                 Format->Name,
                 duk_safe_to_string(ctx, -1));
        ErrorCallback(UserPtr, ErrorBuf);
        goto error;
    }

    duk_pop_n(ctx, 2);

    dump_debug_data(ctx, &ParseContext);

    duk_pop_n(ctx, 2);

    duk_destroy_heap(ctx);
    return true;

error:
    duk_destroy_heap(ctx);
    return false;
}
