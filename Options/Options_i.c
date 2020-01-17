/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Jan 17 10:51:42 2020
 */
/* Compiler settings for E:\wendy_work\Options\Options.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_ITTOptions = {0xA2A4F837,0x24CC,0x4CA8,{0xB3,0x6F,0xAC,0xF7,0xD0,0x40,0xD1,0x01}};


const IID LIBID_OPTIONSLib = {0x024401C5,0x443E,0x4C48,{0x87,0x82,0x0C,0x25,0xFC,0xD3,0xAD,0x24}};


const CLSID CLSID_TTOptions = {0x492E31D4,0xB80D,0x11D3,{0xAF,0x24,0x00,0xA0,0xCC,0x23,0xE6,0x98}};


#ifdef __cplusplus
}
#endif

