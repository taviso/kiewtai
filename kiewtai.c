#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <windows.h>
#include <malloc.h>
#include <shlwapi.h>
#include "duktape.h"

#include "hem.h"
#include "kaitai.h"
#include "parsers.h"

#pragma comment(lib, "SHLWAPI")

//
//
// This file is part of Kiewtai, a port of Kaitai to the Hiew hex editor.
//
// Tavis Ormandy <taviso@gmail.com
//
//

static HEM_API Hem_EntryPoint(HEMCALL_TAG *);
static HEM_API Hem_Unload(void);

HEMINFO_TAG KaitaiStruct = {
    .cbSize         = sizeof(KaitaiStruct),
    .sizeOfInt      = sizeof(int),
    .sdkVerMajor    = HEM_SDK_VERSION_MAJOR,
    .sdkVerMinor    = HEM_SDK_VERSION_MINOR,
    .hemFlag        = HEM_FLAG_MODEMASK | HEM_FLAG_FILEMASK,
    .EntryPoint     = Hem_EntryPoint,
    .Unload         = Hem_Unload,
    .shortName      = "Kiewtai",
    .name           = "Kiewtai: Kaitai Struct format parsers",
    .about1         = "Kaitai Struct is a declarative language used",
    .about2         = "to describe various binary data structures.",
    .about3         = "Author: Tavis Ormandy <taviso@gmail.com>",
};

enum {
    HEM_BLACK,
    HEM_BLUE,
    HEM_GREEN,
    HEM_CYAN,
    HEM_RED,
    HEM_PURPLE,
    HEM_BROWN,
    HEM_GREY,
    HEM_DARKGREY,
    HEM_LIGHTBLUE,
    HEM_LIGHTGREEN,
    HEM_LIGHTCYAN,
    HEM_LIGHTRED,
    HEM_LIGHTPURPLE,
    HEM_YELLOW,
    HEM_WHITE,
};

#define MKCOLOR(bg, fg) (((bg & 0xf) << 4) | ((fg) & 0xf))

// Set of default colors that look okay. I think just alternating
// them looks pretty good.
static HEM_BYTE ColorMarkers[] = {
    MKCOLOR(HEM_WHITE, HEM_LIGHTBLUE),
    MKCOLOR(HEM_LIGHTBLUE, HEM_WHITE),
};

static CHAR KeyLine[12+1+6*12+1];

// The format of the shortcut key layout we pass to Hiew.
static LPCSTR KeyLineFmt =
    "%012llX"
    "%c"
    "%-6.6s"
    "%-6.6s"
    "%-6.6s"
    "%-6.6s"
    "%-6.6s"
    "%-6.6s"
    "%-6.6s"
    "%-6.6s"
    "%-6.6s"
    "%-6.6s"
    "%-6.6s"
    "%-6.6s";

static BOOL OptSimpleParsing;
static BOOL OptDisableComments;
static BOOL OptDisableColors;

// Options available during parser Menu
static HEM_FNKEYS ParseOpts = {
    .main  = KeyLine,
    .alt   = "",
    .ctrl  = "",
    .shift = "",
};

#define ENABLE_FKEY(set, n) ((set) |= (1ULL << ((12 - (n)) * 4)));

VOID GenerateKeyLine(VOID)
{
    UINT64 EnabledKeysSet = 0;

    // Enable Function Keys, if you don't enable them they are silently
    // ignored by hiew with no notification.
    ENABLE_FKEY(EnabledKeysSet, 1);
    ENABLE_FKEY(EnabledKeysSet, 2);
    ENABLE_FKEY(EnabledKeysSet, 3);
    ENABLE_FKEY(EnabledKeysSet, 4);
    ENABLE_FKEY(EnabledKeysSet, 5);

    // Generate shortcut line.
    sprintf(KeyLine, KeyLineFmt, EnabledKeysSet,
                                 HEM_FNKEY_DELIMITER,
                                 "Help",
                                 OptSimpleParsing   ? "Detailed" : "Simple",
                                 OptDisableComments ? "ComsOn"   : "ComOff",
                                 OptDisableColors   ? "ColsOn"   : "ColOff",
                                 "Search", "", "", "", "", "", "", "");
    return;
}

INT ShowHelp(VOID)
{
    static PCHAR HelpText[] = {
        "This plugin parses the loaded file with Kaitai Struct, a",
        "declarative language for defining binary formats. You can",
        "read more about Kaitai here:",
        "",
        "   https://kaitai.io/",
        "",
        "Options:",
        "",
        "   F1 - Show this Help Text.",
        "   F2 - Toggle detailed/simple parsing.",
        "   F3 - Toggle adding comments.",
        "   F4 - Toggle adding color markers.",
        "   F5 - Search for a parser.",
        "",
        "The default level of details is very high, switch to simple",
        "parsing if you just want header fields and records marked.",
        "By default, color markers and comments are added, you can",
        "toggle them individually.",
        "",
        "If you are reversing a binary blob, there may be embedded",
        "resources like images you want to parse. You can do that",
        "by making an active block, then Kaitai will only parse",
        "that data.",
    };

    return HiewGate_Window("Kaitai Parser Plugin",
                           HelpText,
                           _countof(HelpText),
                           60,
                           NULL,
                           NULL);
}

int HEM_EXPORT Hem_Load(HIEWINFO_TAG *HiewInfo)
{
    HiewGate_Set(HiewInfo);
    HiewInfo->hemInfo = &KaitaiStruct;
    return HEM_OK;
}

int HEM_API Hem_Unload()
{
    return HEM_OK;
}

static PCHAR HiewGate_StringDup(LPCSTR String)
{
    PCHAR Result;
    Result = HiewGate_GetMemory(strlen(String) + 1);
    return strcpy(Result, String);
}

VOID CALLBACK KaitaiErrorNotify(PVOID UserPtr, LPCSTR Message)
{
    #pragma warning(suppress: 4090)
    HiewGate_Message("Kaitai Parser", Message);
}

VOID CALLBACK KaitaiFieldNotify(HEM_QWORD *BaseAddr, LPCSTR Name, ULONG Start, ULONG End)
{
    static ColorIndex;
    HEM_BYTE ColorMarker = ColorMarkers[ColorIndex++ % _countof(ColorMarkers)];

    if (OptDisableComments == FALSE) {
        #pragma warning(suppress: 4090)
        HiewGate_Names_AddGlobalComment(*BaseAddr + Start, Name);
    }

    if (OptDisableColors == FALSE) {
        HiewGate_ColorMarker(*BaseAddr + Start, End - Start, ColorMarker);
    }
}

int HEM_API Hem_EntryPoint(HEMCALL_TAG *HemCall)
{
    HEM_QWORD BaseAddr;
    HEM_QWORD BufferSize;
    PCHAR ParserList[_countof(KaitaiParsers)];
    DWORD ParserWidth = 0;
    DWORD ParserNum = 0;
    HEM_UINT PressedKey;
    HIEWGATE_GETDATA HiewData;
    PVOID ParseBuf;

    if (HemCall->cbSize < sizeof(HEMCALL_TAG))
        return HEM_ERROR;

    if (HiewGate_GetData(&HiewData) != HEM_OK)
        return HEM_ERROR;

    // Generate the menu
    for (DWORD Parser = 0; Parser < _countof(ParserList); Parser++) {
        ParserList[Parser] = HiewGate_StringDup(KaitaiParsers[Parser].Name);

        if (strlen(ParserList[Parser]) > ParserWidth)
            ParserWidth = strlen(ParserList[Parser]);
    }

    // Let the user pick what template they want.
    while (true) {

        // Update the option key names.
        GenerateKeyLine();

        ParserNum = HiewGate_Menu("Select Parser",
                                  ParserList,
                                  _countof(ParserList),
                                  ParserWidth,
                                  ParserNum,
                                  &ParseOpts,
                                  &PressedKey,
                                  NULL,
                                  NULL);

        if (ParserNum == HEM_INPUT_ESC) {
            break;
        }

        if (PressedKey == 0) {
            break;
        }

        switch (PressedKey) {
            case HEM_FNKEY_F1:
                ShowHelp();
                break;
            case HEM_FNKEY_F2:  // Toggle Detailed/Simple
                OptSimpleParsing = !OptSimpleParsing;
                break;
            case HEM_FNKEY_F3:  // Toggle Comments On/Off
                OptDisableComments = !OptDisableComments;
                break;
            case HEM_FNKEY_F4:  // Toggle Colors On/Off
                OptDisableColors = !OptDisableColors;
                break;
            case HEM_FNKEY_F5:  { // Search
                // Static so that the last search is remembered
                static CHAR SearchQuery[64];

                // Just a quick search because the list can be long.
                if (HiewGate_GetString("Parser Search",
                                       SearchQuery,
                                       sizeof SearchQuery) == HEM_ERROR)
                    break;

                for (DWORD i = 1; i < _countof(ParserList); i++) {
                    // The index the user is currently.
                    DWORD CurrIndex = ParserNum - 1;

                    // Add current search location.
                    CurrIndex += i;

                    // Wrap if necessary.
                    CurrIndex %= _countof(ParserList);

                    if (StrStrIA(ParserList[CurrIndex], SearchQuery)) {
                        // Found, move cursor.
                        ParserNum = CurrIndex + 1;
                        break;
                    }
                }

                break;
            }
            default:
                HiewGate_SetErrorMsg("Unexpected Function Key Pressed.");
                break;
        }
    }

    // Clean up.
    for (DWORD Parser = 0; Parser < _countof(ParserList); Parser++) {
        HiewGate_FreeMemory(ParserList[Parser]);
    }

    // Check result
    if (ParserNum == HEM_INPUT_ESC) {
        HiewGate_SetErrorMsg("Action was cancelled.");
        return HEM_OK;
    }

    // Convert Item into Index
    ParserNum--;

    // Check if there is an active block, if not use the whole file.
    if (HiewData.sizeMark) {
        BaseAddr = HiewData.offsetMark1;
        BufferSize = HiewData.sizeMark;
    } else {
        BaseAddr = 0;
        BufferSize = HiewData.filelength;
    }

    ParseBuf = HiewGate_GetMemory(BufferSize);

    if (ParseBuf == NULL) {
        HiewGate_Message("Error", "Memory Allocation Error");
        return HEM_OK;
    }

    if (HiewGate_FileRead(BaseAddr, BufferSize, ParseBuf) != BufferSize) {
        HiewGate_Message("Error", "File Read Error");
        HiewGate_FreeMemory(ParseBuf);
        return HEM_OK;
    }

    if (KaitaiQueryFormat(&KaitaiParsers[ParserNum],
                          OptSimpleParsing
                            ? KAITAI_FLAG_DEFAULT
                            : KAITAI_FLAG_EXPANDARRAYS,
                          ParseBuf,
                          BufferSize,
                          KaitaiFieldNotify,
                          KaitaiErrorNotify,
                          &BaseAddr) == FALSE) {
        HiewGate_Message("Error", "Kaitai Parse Failed");
    }

    HiewGate_FreeMemory(ParseBuf);
    return HEM_OK;
}
