/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Jan 17 16:17:50 2020
 */
/* Compiler settings for E:\wendy_work\Options\Options.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Options_h__
#define __Options_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ITTObject_FWD_DEFINED__
#define __ITTObject_FWD_DEFINED__
typedef interface ITTObject ITTObject;
#endif 	/* __ITTObject_FWD_DEFINED__ */


#ifndef __ITTOptions_FWD_DEFINED__
#define __ITTOptions_FWD_DEFINED__
typedef interface ITTOptions ITTOptions;
#endif 	/* __ITTOptions_FWD_DEFINED__ */


#ifndef __TTOptions_FWD_DEFINED__
#define __TTOptions_FWD_DEFINED__

#ifdef __cplusplus
typedef class TTOptions TTOptions;
#else
typedef struct TTOptions TTOptions;
#endif /* __cplusplus */

#endif 	/* __TTOptions_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_Options_0000 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_Options_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Options_0000_v0_0_s_ifspec;

#ifndef __ITTObject_INTERFACE_DEFINED__
#define __ITTObject_INTERFACE_DEFINED__

/* interface ITTObject */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_ITTObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F763D0E1-6359-11d3-B24E-000000000000")
    ITTObject : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE InitObject( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Activate( 
            /* [in] */ int nState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetState( 
            /* [retval][out] */ int __RPC_FAR *pnState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetByID( 
            /* [in] */ int nID,
            /* [out][in] */ VARIANT __RPC_FAR *__RPC_FAR *ppVar) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PutByID( 
            /* [in] */ int nID,
            /* [in] */ VARIANT __RPC_FAR *pVar) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetByName( 
            /* [in] */ BSTR strName,
            /* [out][in] */ VARIANT __RPC_FAR *__RPC_FAR *ppVar) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PutByName( 
            /* [in] */ BSTR strName,
            /* [in] */ VARIANT __RPC_FAR *pVar) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITTObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITTObject __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITTObject __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITTObject __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitObject )( 
            ITTObject __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Activate )( 
            ITTObject __RPC_FAR * This,
            /* [in] */ int nState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetState )( 
            ITTObject __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *pnState);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetByID )( 
            ITTObject __RPC_FAR * This,
            /* [in] */ int nID,
            /* [out][in] */ VARIANT __RPC_FAR *__RPC_FAR *ppVar);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PutByID )( 
            ITTObject __RPC_FAR * This,
            /* [in] */ int nID,
            /* [in] */ VARIANT __RPC_FAR *pVar);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetByName )( 
            ITTObject __RPC_FAR * This,
            /* [in] */ BSTR strName,
            /* [out][in] */ VARIANT __RPC_FAR *__RPC_FAR *ppVar);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PutByName )( 
            ITTObject __RPC_FAR * This,
            /* [in] */ BSTR strName,
            /* [in] */ VARIANT __RPC_FAR *pVar);
        
        END_INTERFACE
    } ITTObjectVtbl;

    interface ITTObject
    {
        CONST_VTBL struct ITTObjectVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITTObject_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITTObject_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITTObject_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITTObject_InitObject(This)	\
    (This)->lpVtbl -> InitObject(This)

#define ITTObject_Activate(This,nState)	\
    (This)->lpVtbl -> Activate(This,nState)

#define ITTObject_GetState(This,pnState)	\
    (This)->lpVtbl -> GetState(This,pnState)

#define ITTObject_GetByID(This,nID,ppVar)	\
    (This)->lpVtbl -> GetByID(This,nID,ppVar)

#define ITTObject_PutByID(This,nID,pVar)	\
    (This)->lpVtbl -> PutByID(This,nID,pVar)

#define ITTObject_GetByName(This,strName,ppVar)	\
    (This)->lpVtbl -> GetByName(This,strName,ppVar)

#define ITTObject_PutByName(This,strName,pVar)	\
    (This)->lpVtbl -> PutByName(This,strName,pVar)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ITTObject_InitObject_Proxy( 
    ITTObject __RPC_FAR * This);


void __RPC_STUB ITTObject_InitObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITTObject_Activate_Proxy( 
    ITTObject __RPC_FAR * This,
    /* [in] */ int nState);


void __RPC_STUB ITTObject_Activate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITTObject_GetState_Proxy( 
    ITTObject __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *pnState);


void __RPC_STUB ITTObject_GetState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITTObject_GetByID_Proxy( 
    ITTObject __RPC_FAR * This,
    /* [in] */ int nID,
    /* [out][in] */ VARIANT __RPC_FAR *__RPC_FAR *ppVar);


void __RPC_STUB ITTObject_GetByID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITTObject_PutByID_Proxy( 
    ITTObject __RPC_FAR * This,
    /* [in] */ int nID,
    /* [in] */ VARIANT __RPC_FAR *pVar);


void __RPC_STUB ITTObject_PutByID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITTObject_GetByName_Proxy( 
    ITTObject __RPC_FAR * This,
    /* [in] */ BSTR strName,
    /* [out][in] */ VARIANT __RPC_FAR *__RPC_FAR *ppVar);


void __RPC_STUB ITTObject_GetByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ITTObject_PutByName_Proxy( 
    ITTObject __RPC_FAR * This,
    /* [in] */ BSTR strName,
    /* [in] */ VARIANT __RPC_FAR *pVar);


void __RPC_STUB ITTObject_PutByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITTObject_INTERFACE_DEFINED__ */


#ifndef __ITTOptions_INTERFACE_DEFINED__
#define __ITTOptions_INTERFACE_DEFINED__

/* interface ITTOptions */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITTOptions;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A2A4F837-24CC-4CA8-B36F-ACF7D040D101")
    ITTOptions : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ITTOptionsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITTOptions __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITTOptions __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITTOptions __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITTOptions __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITTOptions __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITTOptions __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITTOptions __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } ITTOptionsVtbl;

    interface ITTOptions
    {
        CONST_VTBL struct ITTOptionsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITTOptions_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITTOptions_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITTOptions_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITTOptions_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITTOptions_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITTOptions_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITTOptions_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITTOptions_INTERFACE_DEFINED__ */



#ifndef __OPTIONSLib_LIBRARY_DEFINED__
#define __OPTIONSLib_LIBRARY_DEFINED__

/* library OPTIONSLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_OPTIONSLib;

EXTERN_C const CLSID CLSID_TTOptions;

#ifdef __cplusplus

class DECLSPEC_UUID("492E31D4-B80D-11D3-AF24-00A0CC23E698")
TTOptions;
#endif
#endif /* __OPTIONSLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
