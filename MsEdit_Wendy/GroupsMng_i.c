/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Apr 24 14:52:52 2019
 */
/* Compiler settings for C:\Users\Administrator\Desktop\w2t\GroupsMng\GroupsMng.idl:
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

const IID IID_ITTGroupsMng = {0xC0DF3111,0x8A7B,0x11D3,{0xAE,0xD6,0x00,0xA0,0xCC,0x23,0xE6,0x98}};


const IID LIBID_GROUPSMNGLib = {0x284B09A6,0x85C3,0x11D3,{0xAE,0xCF,0x00,0xA0,0xCC,0x23,0xE6,0x98}};


const CLSID CLSID_TTGroupsMng = {0xC0DF3112,0x8A7B,0x11D3,{0xAE,0xD6,0x00,0xA0,0xCC,0x23,0xE6,0x98}};


#ifdef __cplusplus
}
#endif

