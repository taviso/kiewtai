//
//    Hiew External Module include
//

#ifndef _HEM_H_
#define _HEM_H_

////////////////////////////////////////////////////////////
// HEM SDK version, Major version will be compatible!

#define HEM_SDK_VERSION_MAJOR         0
#define HEM_SDK_VERSION_MINOR         53

////////////////////////////////////////////////////////////
// Force BYTE alignment of all structures

#include <pshpack1.h>

////////////////////////////////////////////////////////////
// Open namespace for cpp

#if __cplusplus
 extern "C"
 {
#endif

////////////////////////////////////////////////////////////
// Define of export function

#define HEM_EXPORT            __declspec( dllexport ) _cdecl

////////////////////////////////////////////////////////////
// Define type of the hem interface functions

#define HEM_API                       _cdecl

////////////////////////////////////////////////////////////
// Defines of hem types

typedef unsigned char                 HEM_BYTE;
typedef unsigned short                HEM_WORD;
typedef unsigned int                  HEM_UINT;
typedef unsigned long                 HEM_DWORD;
typedef unsigned __int64              HEM_QWORD;
typedef enum HIEWGATEID_T             HIEWGATE_ID;

#ifndef NULL
#ifdef __cplusplus
#define NULL                  0
#else
#define NULL                  ((void *)0)
#endif
#endif

////////////////////////////////////////////////////////////
// HEM keys line
//
//  Active x12  | Caption 6x12
// "123456789ABC|F1____F2____F3____F4____F5____F6____F7____F8____F9____F10___F11___F12___"
//
#define HEM_FNKEY_DELIMITER           '|'

typedef struct{
  HEM_BYTE         *main,         // main   keys line, "" for none
                   *alt,          // alt-   keys line, "" for none
                   *ctrl,         // ctrl-  keys line, "" for none
                   *shift;        // shift- keys line, "" for none
  }HEM_FNKEYS;

#define HEM_FNKEY_F1                  0xFF3B
#define HEM_FNKEY_F2                  0xFF3C
#define HEM_FNKEY_F3                  0xFF3D
#define HEM_FNKEY_F4                  0xFF3E
#define HEM_FNKEY_F5                  0xFF3F
#define HEM_FNKEY_F6                  0xFF40
#define HEM_FNKEY_F7                  0xFF41
#define HEM_FNKEY_F8                  0xFF42
#define HEM_FNKEY_F9                  0xFF43
#define HEM_FNKEY_F10                 0xFF44
#define HEM_FNKEY_F11                 0xFF85
#define HEM_FNKEY_F12                 0xFF86

#define HEM_FNKEY_ALTF1               0xFF68
#define HEM_FNKEY_ALTF2               0xFF69
#define HEM_FNKEY_ALTF3               0xFF6A
#define HEM_FNKEY_ALTF4               0xFF6B
#define HEM_FNKEY_ALTF5               0xFF6C
#define HEM_FNKEY_ALTF6               0xFF6D
#define HEM_FNKEY_ALTF7               0xFF6E
#define HEM_FNKEY_ALTF8               0xFF6F
#define HEM_FNKEY_ALTF9               0xFF70
#define HEM_FNKEY_ALTF10              0xFF71
#define HEM_FNKEY_ALTF11              0xFF8B
#define HEM_FNKEY_ALTF12              0xFF8C

#define HEM_FNKEY_CTRLF1              0xFF5E
#define HEM_FNKEY_CTRLF2              0xFF5F
#define HEM_FNKEY_CTRLF3              0xFF60
#define HEM_FNKEY_CTRLF4              0xFF61
#define HEM_FNKEY_CTRLF5              0xFF62
#define HEM_FNKEY_CTRLF6              0xFF63
#define HEM_FNKEY_CTRLF7              0xFF64
#define HEM_FNKEY_CTRLF8              0xFF65
#define HEM_FNKEY_CTRLF9              0xFF66
#define HEM_FNKEY_CTRLF10             0xFF67
#define HEM_FNKEY_CTRLF11             0xFF89
#define HEM_FNKEY_CTRLF12             0xFF8A

#define HEM_FNKEY_SHIFTF1             0xFF54
#define HEM_FNKEY_SHIFTF2             0xFF55
#define HEM_FNKEY_SHIFTF3             0xFF56
#define HEM_FNKEY_SHIFTF4             0xFF57
#define HEM_FNKEY_SHIFTF5             0xFF58
#define HEM_FNKEY_SHIFTF6             0xFF59
#define HEM_FNKEY_SHIFTF7             0xFF5A
#define HEM_FNKEY_SHIFTF8             0xFF5B
#define HEM_FNKEY_SHIFTF9             0xFF5C
#define HEM_FNKEY_SHIFTF10            0xFF5D
#define HEM_FNKEY_SHIFTF11            0xFF87
#define HEM_FNKEY_SHIFTF12            0xFF88

////////////////////////////////////////////////////////////
// Length of strings

#define HEM_FILENAME_MAXLEN           260
#define HEM_SHORTNAME_SIZE            16
#define HEM_NAME_SIZE                 60
#define HEM_ABOUT_SIZE                48

////////////////////////////////////////////////////////////
// Bits of the hemFlag

#define HEM_FLAG_MARKEDBLOCK          0x80000000

#define HEM_FLAG_FILEMASK             0x0003FFD8
#define HEM_FLAG_TE64                 0x00020000  // 0.53
#define HEM_FLAG_TE                   0x00010000  // 0.52
#define HEM_FLAG_MACHO64              0x00008000  // 0.50
#define HEM_FLAG_MACHO                0x00004000  // 0.50
#define HEM_FLAG_ELF64                0x00002000
#define HEM_FLAG_PE64                 0x00001000
#define HEM_FLAG_ELF                  0x00000800
#define HEM_FLAG_NLM                  0x00000400
#define HEM_FLAG_PE                   0x00000200
#define HEM_FLAG_LX                   0x00000100
#define HEM_FLAG_LE                   0x00000080
#define HEM_FLAG_NE                   0x00000040
#define HEM_FLAG_FILE                 0x00000010
#define HEM_FLAG_DISK                 0x00000008

#define HEM_FLAG_MODEMASK             0x00000007
#define HEM_FLAG_CODE                 0x00000004
#define HEM_FLAG_HEX                  0x00000002
#define HEM_FLAG_TEXT                 0x00000001

////////////////////////////////////////////////////////////
// Bits of the returnActionFlag

#define HEM_RETURN_SETOFFSET          0x00000001
#define HEM_RETURN_FILERELOAD         0x00000002
#define HEM_RETURN_SETMODE            0x00000004

#define HEM_RETURN_MODE_TEXT          1
#define HEM_RETURN_MODE_HEX           2
#define HEM_RETURN_MODE_CODE          3

////////////////////////////////////////////////////////////
// Bits of the Find()

#define HEM_FIND_NEXT                 0x00000001
#define HEM_FIND_BACKWARD             0x00000002
#define HEM_FIND_CASESENSITIVE        0x00000004
#define HEM_FIND_INMARK               0x00000008
#define HEM_FIND_USEMASK              0x00000010

////////////////////////////////////////////////////////////
// Structure of the hiew-call, get from hiew at hem calling

typedef struct{
  int                     cbSize;
  HEM_DWORD               hemFlag;
  HEM_DWORD               returnActionFlag;
  HEM_QWORD               returnOffset;
  HEM_BYTE                returnMode;
  HEM_BYTE                reserved1[ 3 ];
  HEM_DWORD               winColMax;      // 0.32
  HEM_DWORD               winRowMax;      // 0.32
  HEM_DWORD               filenameHash;   // 0.40
  HEM_BYTE                reserved2[ 4*26 ];
  }HEMCALL_TAG;

////////////////////////////////////////////////////////////
// Structure of the hem-info, return to hiew at hem loading

typedef struct{
  int                     cbSize;
  HEM_WORD                sizeOfInt;
  HEM_WORD                reserved1;
  HEM_BYTE                sdkVerMajor, sdkVerMinor;
  HEM_BYTE                hemVerMajor, hemVerMinor;
  HEM_DWORD               hemFlag;
  HEM_DWORD               reserved2;
  int                    (HEM_API *EntryPoint)( HEMCALL_TAG *hemCall );
  int                    (HEM_API *Unload)( void );
  int                    (HEM_API *Hem2HemGate)( void * );
  int                     reserved3[ 4 ];
  HEM_BYTE                shortName[ HEM_SHORTNAME_SIZE ];
  HEM_BYTE                name[  HEM_NAME_SIZE ];
  HEM_BYTE                about1[ HEM_ABOUT_SIZE ];
  HEM_BYTE                about2[ HEM_ABOUT_SIZE ];
  HEM_BYTE                about3[ HEM_ABOUT_SIZE ];
  }HEMINFO_TAG;

////////////////////////////////////////////////////////////
// Structure of the hiew-info, get from hiew at hem loading

typedef struct{
  int                     cbSize;
  HEM_BYTE                sdkVerMajor,  sdkVerMinor;
  HEM_BYTE                hiewVerMajor, hiewVerMinor;
  int                    (HEM_API *HiewGate)( void * );
  int                     hemHandle;
  HEM_BYTE                hemFile[ HEM_FILENAME_MAXLEN ];
  HEMINFO_TAG            *hemInfo;
  int                     reserved[ 4 ];
  }HIEWINFO_TAG;

////////////////////////////////////////////////////////////
// Return code of the hem interface

#define HEM_ERR_INTERNAL                    (-21)
#define HEM_ERR_INVALID_ARGUMENT            (-20)
#define HEM_ERR_HIEW_VERSION_INVALID        (-19)
#define HEM_ERR_FNKEYS_INVALID              (-18)
#define HEM_ERR_READONLYFILE                (-17)
#define HEM_ERR_POINTER_IS_NULL             (-16)
#define HEM_ERR_HIEWDATA_SIZE_MISMATCH      (-15)
#define HEM_ERR_HEMINFO_SIZE_MISMATCH       (-14)
#define HEM_ERR_HEMINFO_IS_NULL             (-13)
#define HEM_ERR_HEM2HEMGATE_IS_NULL         (-12)
#define HEM_ERR_HEM_NOTFOUND                (-11)
#define HEM_ERR_HIEWGATE_PARM_INVALID       (-10)
#define HEM_ERR_HIEWGATE_ID_INVALID         (-9)
#define HEM_ERR_HANDLE_INVALID              (-8)
#define HEM_ERR_NOADDRESS_HIEWGATE          (-7)
#define HEM_ERR_NOENTRYPOINT                (-6)
#define HEM_ERR_UNLOADED                    (-5)
#define HEM_ERR_SDKVER_INCOMPATIBLE         (-4)
#define HEM_ERR_NOADDRESS_LOAD              (-3)
#define HEM_ERR_LOADDLL                     (-2)
#define HEM_ERROR                           (-1)
#define HEM_OK                                0

#define HEM_INPUT_ESC                         0
#define HEM_INPUT_CR                          1
#define HEM_KEYBREAK                          2
#define HEM_OFFSET_NOT_FOUND                  (HEM_QWORD)(-1)

/////////////////////////////////////////////////////////////
// Identificators of the hiew-gate calls

enum HIEWGATEID_T{
        HIEWGATE_ID_NULL                    = 0,
        HIEWGATE_ID_GETDATA,
        HIEWGATE_ID_GETHEM2HEMGATE,
        HIEWGATE_ID_GETMEMORY,
        HIEWGATE_ID_FREEMEMORY,
        HIEWGATE_ID_FILEOPENFORWRITE,
        HIEWGATE_ID_FILEREAD,
        HIEWGATE_ID_FILEWRITE,
        HIEWGATE_ID_MESSAGE,
        HIEWGATE_ID_WINDOW,
        HIEWGATE_ID_MENU,
        HIEWGATE_ID_GETSTRING,
        HIEWGATE_ID_MESSAGEWAITOPEN,        // 0.30
        HIEWGATE_ID_MESSAGEWAITCLOSE,       // 0.30
        HIEWGATE_ID_ISKEYBREAK,             // 0.30
        HIEWGATE_ID_SETERRORMSG,            // 0.35
        HIEWGATE_ID_GETSTRINGDUAL,          // 0.35
        HIEWGATE_ID_GETFILENAME,            // 0.40
        HIEWGATE_ID_REALLOCMEMORY,          // 0.40
        HIEWGATE_ID_MARKBLOCK,              // 0.40
        HIEWGATE_ID_NAMES,                  // 0.42
        HIEWGATE_ID_GLOBAL2LOCAL,           // 0.46
        HIEWGATE_ID_LOCAL2GLOBAL,           // 0.46
        HIEWGATE_ID_FIND,                   // 0.42
        HIEWGATE_ID_COLORMARKER,            // 0.48
        HIEWGATE_ID_MAX };

////////////////////////////////////////////////////////////
// Structures of the hiew-gate calls

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_NULL
  int                     hemHandle;
  }HIEWGATE_NULL;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_GETDATA
  int                     hemHandle;
  HEM_BYTE                filename[ HEM_FILENAME_MAXLEN ];
  HEM_QWORD               filelength;
  HEM_QWORD               offsetCurrent;
  HEM_QWORD               offsetMark1;
  HEM_QWORD               offsetMark2;
  HEM_QWORD               sizeMark;
  }HIEWGATE_GETDATA;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_GETHEM2HEMGATE
  int                     hemHandle;
  HEM_BYTE                shortName[ HEM_SHORTNAME_SIZE ];
  int                    (HEM_API *Hem2HemGate)( void * );
  }HIEWGATE_GETHEM2HEMGATE;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_GETMEMORY
  int                     hemHandle;
  HEM_UINT                bytes;
  HEM_BYTE               *retPmem;
  }HIEWGATE_GETMEMORY;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_FREEMEMORY
  int                     hemHandle;
  HEM_BYTE               *pMem;
  }HIEWGATE_FREEMEMORY;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_FILEOPENFORWRITE
  int                     hemHandle;
  }HIEWGATE_FILEOPENFORWRITE;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_FILEREAD
  int                     hemHandle;
  HEM_QWORD               offset;
  HEM_BYTE               *buffer;
  HEM_UINT                bytes;
  }HIEWGATE_FILEREAD;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_FILEWRITE
  int                     hemHandle;
  HEM_QWORD               offset;
  HEM_BYTE               *buffer;
  HEM_UINT                bytes;
  }HIEWGATE_FILEWRITE;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_MESSAGE
  int                     hemHandle;
  HEM_BYTE               *title;
  HEM_BYTE               *msg;
  }HIEWGATE_MESSAGE;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_WINDOW
  int                     hemHandle;
  HEM_BYTE               *title;
  HEM_BYTE              **lines;
  int                     linesCount;
  int                     width;
  int                     dummy;
  HEM_FNKEYS              fnKeys;
  HEM_UINT                returnFnKey;
  }HIEWGATE_WINDOW;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_MENU
  int                     hemHandle;
  HEM_BYTE               *title;
  HEM_BYTE              **lines;
  int                     linesCount;
  int                     width;
  int                     startItem;        // [ 1 ... linesCount ]
  HEM_FNKEYS              fnKeys;
  HEM_UINT                returnFnKey;
  HEM_BYTE            * (*CallbackLine)( int lineNumber, void *pData );  // 0.40
  void                   *pData;                                         // 0.40
  }HIEWGATE_MENU;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_GETSTRING
  int                     hemHandle;
  HEM_BYTE               *title;
  HEM_BYTE               *string;
  int                     stringLen;
  }HIEWGATE_GETSTRING;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_MESSAGEWAITOPEN
  int                     hemHandle;
  HEM_BYTE               *msg;
  }HIEWGATE_MESSAGEWAITOPEN;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_MESSAGEWAITCLOSE
  int                     hemHandle;
  }HIEWGATE_MESSAGEWAITCLOSE;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_ISKEYBREAK
  int                     hemHandle;
  }HIEWGATE_ISKEYBREAK;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_SETERRORMSG
  int                     hemHandle;
  HEM_BYTE               *errorMsg;
  }HIEWGATE_SETERRORMSG;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_GETSTRINGDUAL
  int                     hemHandle;
  HEM_BYTE               *title;
  HEM_BYTE               *string;
  int                     stringLenMax;     // <= 20 !
  int                     stringLen;
  int                     bOnHexLine;
  }HIEWGATE_GETSTRINGDUAL;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_GETSFILENAME
  int                     hemHandle;
  HEM_BYTE               *title;
  HEM_BYTE               *filename;         // [ HEM_FILENAME_MAXLEN ]
  }HIEWGATE_GETFILENAME;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_REALLOCMEMORY
  int                     hemHandle;
  HEM_BYTE               *pMem;
  HEM_UINT                newSize;
  HEM_BYTE               *retPmem;
  }HIEWGATE_REALLOCMEMORY;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_MARKBLOCK
  int                     hemHandle;
  HEM_QWORD               offset1;
  HEM_QWORD               offset2;
  }HIEWGATE_MARKBLOCK;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_NAMES
  int                     hemHandle;
  int                     subfunction;      // see enum below
  HEM_QWORD               offset;
  struct{            int  bLocal   :1,
                          bComment :1;
        };
  HEM_BYTE               *name;
  int                     nameBufferLength;
  HEM_DWORD               r4;
  HEM_DWORD               r3;
  HEM_DWORD               r2;
  HEM_DWORD               r1;
  }HIEWGATE_NAMES;

enum{  HEM_NAMES_             = 0,
       HEM_NAMES_ADD_GLOBAL,
       HEM_NAMES_ADD_LOCAL,
       HEM_NAMES_DEL_LOCAL,
       HEM_NAMES_DEL_GLOBAL,
       HEM_NAMES_DEL_NAME,
       HEM_NAMES_CLEAR        = 32,
       HEM_NAMES_COUNT_LOCAL,
       HEM_NAMES_COUNT_GLOBAL,
       HEM_NAMES_COUNT_NAME,
       HEM_NAMES_GET_LOCAL,
       HEM_NAMES_GET_GLOBAL,
       HEM_NAMES_FIND_NAME,
  };

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_GLOBAL2LOCAL
  int                     hemHandle;
  HEM_QWORD               offsetGlobal;
  HEM_QWORD               offsetLocal;
  }HIEWGATE_GLOBAL2LOCAL;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_LOCAL2GLOBAL
  int                     hemHandle;
  HEM_QWORD               offsetLocal;
  HEM_QWORD               offsetGlobal;
  }HIEWGATE_LOCAL2GLOBAL;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_FIND
  int                     hemHandle;
  int                     flags;
  HEM_QWORD               offset;
  HEM_BYTE               *pData;            // [ dataLength+1 ]
  int                     dataLength;       // <= 20 !
  HEM_BYTE               *pMask;            // [ dataLength+1 ]
  HEM_QWORD               retOffset;
  }HIEWGATE_FIND;

typedef struct{
  int                     cbSize;
  HIEWGATE_ID             callId;           // HIEWGATE_ID_COLORMARKER
  int                     hemHandle;
  HEM_QWORD               offset;
  HEM_DWORD               length;           // <= 0xFFFFFF
  HEM_BYTE                color;
  }HIEWGATE_COLORMARKER;

////////////////////////////////////////////////////////////
// The single exported function of the hem

int HEM_EXPORT  Hem_Load( HIEWINFO_TAG *hiewInfo );

////////////////////////////////////////////////////////////
// Prototypes of high-level hiew-gate interface

int        HiewGate_GetLastResult( void );

int        HiewGate_Null( void );
int        HiewGate_Set( HIEWINFO_TAG *hiewInfo );
int        HiewGate_GetData( HIEWGATE_GETDATA *hiewData );
int        HiewGate_GetHem2HemGate( HIEWGATE_GETHEM2HEMGATE *tag, HEM_BYTE *shortName );

HEM_BYTE  *HiewGate_GetMemory( HEM_UINT bytes );
HEM_BYTE  *HiewGate_ReallocMemory( HEM_BYTE *pMem, HEM_UINT bytes );
int        HiewGate_FreeMemory( HEM_BYTE *pMem );

int        HiewGate_FileOpenForWrite( void );
int        HiewGate_FileRead( HEM_QWORD offset, HEM_UINT bytes, HEM_BYTE *buffer );
int        HiewGate_FileWrite( HEM_QWORD offset, HEM_UINT bytes, HEM_BYTE *buffer );

int        HiewGate_Message( HEM_BYTE *title, HEM_BYTE *msg );
int        HiewGate_MessageWaitOpen( HEM_BYTE *msg );
int        HiewGate_MessageWaitClose( void );

int        HiewGate_Window( HEM_BYTE *title, HEM_BYTE **lines, int linesCount, int width, HEM_FNKEYS *fnKeys, HEM_UINT *returnFnKey );
int        HiewGate_Menu( HEM_BYTE *title, HEM_BYTE **lines, int linesCount, int width, int startItem, HEM_FNKEYS *fnKeys, HEM_UINT *returnFnKey, HEM_BYTE * (*CallbackLine)( int, void * ), void *pData );
int        HiewGate_IsKeyBreak( void );

int        HiewGate_GetString( HEM_BYTE *title, HEM_BYTE *string, int stringLen );
int        HiewGate_SetErrorMsg( HEM_BYTE *errorMsg );
int        HiewGate_GetStringDual( HEM_BYTE *title, HEM_BYTE *string, int stringLenMax /* <= 20 */, int stringLen, int *bOnHexLine );
int        HiewGate_GetFilename( HEM_BYTE *title, HEM_BYTE *filename );

int        HiewGate_MarkBlock( HEM_QWORD offset1, HEM_QWORD offset2 );
int        HiewGate_UnmarkBlock( void );

int        HiewGate_Names_Clear( void );
int        HiewGate_Names_AddLocal( HEM_QWORD offset, HEM_BYTE *name );
int        HiewGate_Names_AddGlobal( HEM_QWORD offset, HEM_BYTE *name );
int        HiewGate_Names_DelLocal( HEM_QWORD offset );
int        HiewGate_Names_DelGlobal( HEM_QWORD offset );
int        HiewGate_Names_DelName( HEM_BYTE *name );
HEM_BYTE  *HiewGate_Names_GetLocal( HEM_QWORD offset, HEM_BYTE *retname, int retnameBufferLength );
HEM_BYTE  *HiewGate_Names_GetGlobal( HEM_QWORD offset, HEM_BYTE *retname, int retnameBufferLength );
HEM_QWORD  HiewGate_Names_FindName( HEM_BYTE *name, int *bLocal );
int        HiewGate_Names_AddLocalComment( HEM_QWORD offset, HEM_BYTE *comment );
int        HiewGate_Names_AddGlobalComment( HEM_QWORD offset, HEM_BYTE *comment );
int        HiewGate_Names_DelLocalComment( HEM_QWORD offset );
int        HiewGate_Names_DelGlobalComment( HEM_QWORD offset );
HEM_BYTE  *HiewGate_Names_GetLocalComment( HEM_QWORD offset, HEM_BYTE *retname, int retnameBufferLength );
HEM_BYTE  *HiewGate_Names_GetGlobalComment( HEM_QWORD offset, HEM_BYTE *retname, int retnameBufferLength );
int        HiewGate_Names_CountLocal( void );
int        HiewGate_Names_CountGlobal( void );
int        HiewGate_Names_CountName( void );

HEM_QWORD  HiewGate_Global2Local( HEM_QWORD offsetGlobal );
HEM_QWORD  HiewGate_Local2Global( HEM_QWORD offsetLocal );

HEM_QWORD  HiewGate_Find( int flags, HEM_QWORD offset, HEM_BYTE *pData, int dataLength /* <= 20 */, HEM_BYTE *pMask );
HEM_QWORD  HiewGate_FindNext( void );

int        HiewGate_ColorMarker( HEM_QWORD offset, HEM_DWORD length /* <= 0xFFFFFF, 0 - for delete */, HEM_BYTE color );

////////////////////////////////////////////////////////////
// Close namespace for cpp

#if __cplusplus
 };
#endif

////////////////////////////////////////////////////////////
// Restore default alignment of all structures

#include <poppack.h>

#endif /* _HEM_H_ */
/// End of the include /////////////////////////////////////
