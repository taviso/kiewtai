//
//  High-level Hiew Gate interface
//

#include "hem.h"

/// Macro //////////////////////////////////////////////////

#define  HEMTAG( id )  HIEWGATE_##id tag = { sizeof( HIEWGATE_##id ), HIEWGATE_ID_##id }

/// Prototypes /////////////////////////////////////////////

HEM_BYTE *hemStrncpy( HEM_BYTE *t, HEM_BYTE *s, int n );
int       HiewGateHighLevel( void *tag );

/// Variables //////////////////////////////////////////////

int  (HEM_API *HiewGate)( void *tag ) = NULL; // Address of the HiewGate function, received by Hem_Load()
int  hemHandle = 0;                           // Hem handle, received by Hem_Load() 
int  lastResult;                              // for HiewGateHighLevel() calling

/// Functions //////////////////////////////////////////////
// local replace of strncpy() -  n is full length of target with last zero

HEM_BYTE *hemStrncpy( HEM_BYTE *t, HEM_BYTE *s, int n )
{
 HEM_BYTE  *r = s;
 while( n > 1 && ( *t++ = *s++ ) )
   n--;
 if( n > 0 )
   *t = 0;
 return( r );
}

////////////////////////////////////////////////////////////
// Set HiewGate and hemHandle, call from Hem_Load() 

int  HiewGate_Set( HIEWINFO_TAG *hiewInfo )
{
 HiewGate = hiewInfo->HiewGate; 
 hemHandle = hiewInfo->hemHandle;
 return( HEM_OK );
}

////////////////////////////////////////////////////////////
// Call of the HiewGate

int  HiewGateHighLevel( void *tag )
{
 ((HIEWGATE_NULL*)tag)->hemHandle = hemHandle;
 return( lastResult = HiewGate? HiewGate( tag ) : HEM_ERR_NOADDRESS_HIEWGATE );
}

////////////////////////////////////////////////////////////
// Get lastresult

int  HiewGate_GetLastResult()
{
 return( lastResult );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_ERROR          
//    HEM_OK

int  HiewGate_Null( void )
{
 HEMTAG( NULL );
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_ERR_HIEWDATA_SIZE_MISMATCH          
//    HEM_OK

int  HiewGate_GetData( HIEWGATE_GETDATA *tag )
{
 tag->cbSize = sizeof( HIEWGATE_GETDATA );
 tag->callId = HIEWGATE_ID_GETDATA;
 return( HiewGateHighLevel( tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_HEM_ERR_HEM2HEMGATE_IS_NULL
//    HEM_ERR_HEM_NOTFOUND          
//    HEM_OK

int  HiewGate_GetHem2HemGate( HIEWGATE_GETHEM2HEMGATE *tag, HEM_BYTE *shortName )
{
 tag->cbSize = sizeof( HIEWGATE_GETHEM2HEMGATE );
 tag->callId = HIEWGATE_ID_GETHEM2HEMGATE;
 hemStrncpy( tag->shortName, shortName, HEM_SHORTNAME_SIZE );
 return( HiewGateHighLevel( tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    NULL on error
//    pointer

HEM_BYTE  *HiewGate_GetMemory( HEM_UINT bytes )
{
 HEMTAG( GETMEMORY );
 tag.bytes = bytes;
 tag.retPmem = NULL;
 HiewGateHighLevel( &tag );
 return( tag.retPmem );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_OK

int  HiewGate_FreeMemory( HEM_BYTE *pMem )
{
 HEMTAG( FREEMEMORY );
 tag.pMem = pMem;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    NULL on error
//    new pointer

HEM_BYTE  *HiewGate_ReallocMemory( HEM_BYTE *pMem, HEM_UINT newSize )
{
 HEMTAG( REALLOCMEMORY );
 tag.pMem = pMem;
 tag.newSize = newSize;
 tag.retPmem = NULL;
 HiewGateHighLevel( &tag );
 return( tag.retPmem );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_ERR_READONLYFILE
//    HEM_OK

int  HiewGate_FileOpenForWrite()
{
 HEMTAG( FILEOPENFORWRITE );
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_ERR_POINTER_IS_NULL
//    HEM_ERROR
//    number of bytes read

int  HiewGate_FileRead( HEM_QWORD offset, HEM_UINT bytes, HEM_BYTE *buffer )
{
 HEMTAG( FILEREAD );
 tag.offset = offset;
 tag.bytes = bytes;
 tag.buffer = buffer;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_ERR_POINTER_IS_NULL
//    HEM_ERR_READONLYFILE
//    HEM_OK

int  HiewGate_FileWrite( HEM_QWORD offset, HEM_UINT bytes, HEM_BYTE *buffer )
{
 HEMTAG( FILEWRITE );
 tag.offset = offset;
 tag.bytes = bytes;
 tag.buffer = buffer;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    key pressed in upper case

int  HiewGate_Message( HEM_BYTE *title, HEM_BYTE *msg )
{
 HEMTAG( MESSAGE );
 tag.title = title;
 tag.msg = msg;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_ERR_FNKEYS_INVALID          
//    HEM_INPUT_ESC
//    HEM_OK
//    pressed FnKey stored into returnFnKey

int  HiewGate_Window( HEM_BYTE *title, HEM_BYTE **lines, int linesCount, int width, HEM_FNKEYS *fnKeys, HEM_UINT *returnFnKey )
{
 HEMTAG( WINDOW );
 int  rc;
 tag.title = title;
 tag.lines = lines;
 tag.linesCount = linesCount;
 tag.width = width;
 if( fnKeys ){
   tag.fnKeys.main = fnKeys->main;
   tag.fnKeys.alt = fnKeys->alt;
   tag.fnKeys.ctrl = fnKeys->ctrl;
   tag.fnKeys.shift = fnKeys->shift;
 }else{
   tag.fnKeys.main = NULL;
   tag.fnKeys.alt = NULL;
   tag.fnKeys.ctrl = NULL;
   tag.fnKeys.shift = NULL;
 }
 rc = HiewGateHighLevel( &tag );
 if( returnFnKey )
   *returnFnKey = tag.returnFnKey;
 return( rc );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_ERR_FNKEYS_INVALID          
//    HEM_INPUT_ESC
//    selected item index, starting from 1
//    pressed FnKey stored into returnFnKey

int  HiewGate_Menu( HEM_BYTE *title, HEM_BYTE **lines, int linesCount, int width, int startItem, HEM_FNKEYS *fnKeys, HEM_UINT *returnFnKey, HEM_BYTE *(*CallbackLine)( int, void * ), void *pData )
{
 HEMTAG( MENU );
 int  rc;
 tag.title = title;
 tag.lines = lines;
 tag.linesCount = linesCount;
 tag.width = width;
 tag.startItem = startItem;         // [ 1 ... linesCount ]
 tag.CallbackLine = CallbackLine;   // 0.40
 tag.pData = pData;                 // 0.40
 if( fnKeys ){
   tag.fnKeys.main = fnKeys->main;
   tag.fnKeys.alt = fnKeys->alt;
   tag.fnKeys.ctrl = fnKeys->ctrl;
   tag.fnKeys.shift = fnKeys->shift;
 }else{
   tag.fnKeys.main = NULL;
   tag.fnKeys.alt = NULL;
   tag.fnKeys.ctrl = NULL;
   tag.fnKeys.shift = NULL;
 }
 rc = HiewGateHighLevel( &tag );
 if( returnFnKey )
   *returnFnKey = tag.returnFnKey;
 return( rc );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_ERROR          
//    HEM_INPUT_ESC
//    HEM_INPUT_CR

int  HiewGate_GetString( HEM_BYTE *title, HEM_BYTE *string, int stringLen )
{
 HEMTAG( GETSTRING );
 tag.title = title;
 tag.string = string;
 tag.stringLen = stringLen;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_OK

int  HiewGate_MessageWaitOpen( HEM_BYTE *msg )
{
 HEMTAG( MESSAGEWAITOPEN );
 tag.msg = msg;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_OK

int  HiewGate_MessageWaitClose()
{
 HEMTAG( MESSAGEWAITCLOSE );
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_KEYBREAK, if ESC was pressed

int  HiewGate_IsKeyBreak()
{
 HEMTAG( ISKEYBREAK );
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_ERROR          

int HiewGate_SetErrorMsg( HEM_BYTE *errorMsg )
{
 HEMTAG( SETERRORMSG );
 tag.errorMsg = errorMsg;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_ERROR          
//    HEM_INPUT_ESC
//    bytes input

int  HiewGate_GetStringDual( HEM_BYTE *title, HEM_BYTE *string, int stringLenMax, int stringLen, int *bOnHexLine )
{
 HEMTAG( GETSTRINGDUAL );
 int  rc;
 tag.title = title;
 tag.string = string;
 tag.stringLenMax = stringLenMax;
 tag.stringLen = stringLen;
 tag.bOnHexLine = *bOnHexLine;
 rc = HiewGateHighLevel( &tag );
 *bOnHexLine = tag.bOnHexLine;
 return( rc );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_ERR_POINTER_IS_NULL
//    HEM_INPUT_ESC
//    HEM_INPUT_CR

int  HiewGate_GetFilename( HEM_BYTE *title, HEM_BYTE *filename )
{
 HEMTAG( GETFILENAME );
 tag.title = title;
 tag.filename = filename;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_OK

int  HiewGate_MarkBlock( HEM_QWORD offset1, HEM_QWORD offset2 )
{
 HEMTAG( MARKBLOCK );
 tag.offset1 = offset1;
 tag.offset2 = offset2;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_OK

int  HiewGate_UnmarkBlock()
{
 HEMTAG( MARKBLOCK );
 tag.offset1 = tag.offset2 = (HEM_QWORD)(-1);
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_OK

int  HiewGate_Names_Clear()
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_CLEAR;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_OK
//    HEM_ERROR, if offset or name already exist
//    HEM_ERR_INTERNAL

int  HiewGate_Names_AddLocal( HEM_QWORD offset, HEM_BYTE *name )
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_ADD_LOCAL;
 tag.bLocal = 1;
 tag.bComment = 0;
 tag.offset = offset;
 tag.name = name;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_OK
//    HEM_ERROR, if offset or name already exist
//    HEM_ERR_INTERNAL

int  HiewGate_Names_AddGlobal( HEM_QWORD offset, HEM_BYTE *name )
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_ADD_GLOBAL;
 tag.bLocal = 0;
 tag.bComment = 0;
 tag.offset = offset;
 tag.name = name;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_OK
//    HEM_ERROR

int  HiewGate_Names_DelLocal( HEM_QWORD offset )
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_DEL_LOCAL;
 tag.bLocal = 1;
 tag.bComment = 0;
 tag.offset = offset;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_OK
//    HEM_ERROR

int  HiewGate_Names_DelGlobal( HEM_QWORD offset )
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_DEL_GLOBAL;
 tag.bLocal = 0;
 tag.bComment = 0;
 tag.offset = offset;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_OK
//    HEM_ERROR

int  HiewGate_Names_DelName( HEM_BYTE *name )
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_DEL_NAME;
 tag.name = name;
 tag.bComment = 0;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    count
//    HEM_ERROR

int  HiewGate_Names_CountName()
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_COUNT_NAME;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    count
//    HEM_ERROR

int  HiewGate_Names_CountLocal()
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_COUNT_LOCAL;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    count
//    HEM_ERROR

int  HiewGate_Names_CountGlobal()
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_COUNT_GLOBAL;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    name
//    NULL, if not found

HEM_BYTE  *HiewGate_Names_GetLocal( HEM_QWORD offset, HEM_BYTE *name, int nameBufferLength )
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_GET_LOCAL;
 tag.bLocal = 1;
 tag.bComment = 0;
 tag.offset = offset;
 tag.name = name;
 tag.nameBufferLength = nameBufferLength;
 return( ( HiewGateHighLevel( &tag ) == HEM_OK )? name : NULL );
}

////////////////////////////////////////////////////////////
// returns: 
//    name
//    NULL, if not found

HEM_BYTE  *HiewGate_Names_GetGlobal( HEM_QWORD offset, HEM_BYTE *name, int nameBufferLength )
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_GET_GLOBAL;
 tag.bLocal = 0;
 tag.bComment = 0;
 tag.offset = offset;
 tag.name = name;
 tag.nameBufferLength = nameBufferLength;
 return( ( HiewGateHighLevel( &tag ) == HEM_OK )? name : NULL );
}

////////////////////////////////////////////////////////////
// returns: 
//    offset
//    HEM_OFFSET_NOT_FOUND

HEM_QWORD  HiewGate_Names_FindName( HEM_BYTE *name, int *bLocal )
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_FIND_NAME;
 tag.name = name;
 tag.bComment = 0;
 tag.offset = HEM_OFFSET_NOT_FOUND;
 if( HiewGateHighLevel( &tag ) == HEM_OK && bLocal )
   *bLocal = tag.bLocal;
 return( tag.offset );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_OK
//    HEM_ERROR, if offset already exist
//    HEM_ERR_INTERNAL

int  HiewGate_Names_AddLocalComment( HEM_QWORD offset, HEM_BYTE *name )
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_ADD_LOCAL;
 tag.bLocal = 1;
 tag.bComment = 1;
 tag.offset = offset;
 tag.name = name;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_OK
//    HEM_ERROR, if offset already exist
//    HEM_ERR_INTERNAL

int  HiewGate_Names_AddGlobalComment( HEM_QWORD offset, HEM_BYTE *name )
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_ADD_GLOBAL;
 tag.bLocal = 0;
 tag.bComment = 1;
 tag.offset = offset;
 tag.name = name;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_OK
//    HEM_ERROR

int  HiewGate_Names_DelLocalComment( HEM_QWORD offset )
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_DEL_LOCAL;
 tag.bLocal = 1;
 tag.bComment = 1;
 tag.offset = offset;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_OK
//    HEM_ERROR

int  HiewGate_Names_DelGlobalComment( HEM_QWORD offset )
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_DEL_GLOBAL;
 tag.bLocal = 0;
 tag.bComment = 1;
 tag.offset = offset;
 return( HiewGateHighLevel( &tag ) );
}

////////////////////////////////////////////////////////////
// returns: 
//    name
//    NULL, if not found

HEM_BYTE  *HiewGate_Names_GetLocalComment( HEM_QWORD offset, HEM_BYTE *name, int nameBufferLength )
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_GET_LOCAL;
 tag.bLocal = 1;
 tag.bComment = 1;
 tag.offset = offset;
 tag.name = name;
 tag.nameBufferLength = nameBufferLength;
 return( ( HiewGateHighLevel( &tag ) == HEM_OK )? name : NULL );
}

////////////////////////////////////////////////////////////
// returns: 
//    name
//    NULL, if not found

HEM_BYTE  *HiewGate_Names_GetGlobalComment( HEM_QWORD offset, HEM_BYTE *name, int nameBufferLength )
{
 HEMTAG( NAMES ); 
 tag.subfunction = HEM_NAMES_GET_GLOBAL;
 tag.bLocal = 0;
 tag.bComment = 1;
 tag.offset = offset;
 tag.name = name;
 tag.nameBufferLength = nameBufferLength;
 return( ( HiewGateHighLevel( &tag ) == HEM_OK )? name : NULL );
}

////////////////////////////////////////////////////////////
// returns: 
//    offsetLocal
//    HEM_OFFSET_NOT_FOUND, if conversion failed

HEM_QWORD  HiewGate_Global2Local( HEM_QWORD offsetGlobal )
{
 HEMTAG( GLOBAL2LOCAL ); 
 tag.offsetGlobal = offsetGlobal;
 tag.offsetLocal = HEM_OFFSET_NOT_FOUND;
 HiewGateHighLevel( &tag );
 return( tag.offsetLocal );
}

////////////////////////////////////////////////////////////
// returns: 
//    offsetGlobal
//    HEM_OFFSET_NOT_FOUND, if conversion failed

HEM_QWORD  HiewGate_Local2Global( HEM_QWORD offsetLocal )
{
 HEMTAG( LOCAL2GLOBAL ); 
 tag.offsetLocal = offsetLocal;
 tag.offsetGlobal = HEM_OFFSET_NOT_FOUND;
 HiewGateHighLevel( &tag );
 return( tag.offsetGlobal );
}

////////////////////////////////////////////////////////////
// returns: 
//    offset
//    HEM_OFFSET_NOT_FOUND

HEM_QWORD  HiewGate_Find( int flags, HEM_QWORD offset, HEM_BYTE *pData, int dataLength, HEM_BYTE *pMask )
{
 HEMTAG( FIND );
 tag.flags = flags;
 tag.offset = offset;
 tag.pData = pData;
 tag.dataLength = dataLength;
 tag.pMask = pMask;
 tag.retOffset = HEM_OFFSET_NOT_FOUND;
 HiewGateHighLevel( &tag );
 return( tag.retOffset );
}

////////////////////////////////////////////////////////////
// returns: 
//    offset
//    HEM_NOT_FOUND

HEM_QWORD  HiewGate_FindNext()
{
 HEMTAG( FIND );
 tag.flags = HEM_FIND_NEXT;
 tag.retOffset = HEM_OFFSET_NOT_FOUND;
 HiewGateHighLevel( &tag );
 return( tag.retOffset );
}

////////////////////////////////////////////////////////////
// returns: 
//    HEM_OK
//    HEM_ERR_INVALID_ARGUMENT

int  HiewGate_ColorMarker( HEM_QWORD offset, HEM_DWORD length /* <= 0xFFFFFF, 0 - for delete */, HEM_BYTE color )
{
 HEMTAG( COLORMARKER );
 tag.offset = offset;
 tag.length = length;
 tag.color  = color;
 return( HiewGateHighLevel( &tag ) );
}

/// Module End /////////////////////////////////////////////
