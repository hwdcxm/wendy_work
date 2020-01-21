/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Jan 21 15:13:05 2020
 */
/* Compiler settings for E:\wendy_work\Common\DataProxy.idl:
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

#ifndef __DataProxy_h__
#define __DataProxy_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ITTDataProxy_FWD_DEFINED__
#define __ITTDataProxy_FWD_DEFINED__
typedef interface ITTDataProxy ITTDataProxy;
#endif 	/* __ITTDataProxy_FWD_DEFINED__ */


#ifndef ___ITTDataProxyEvents_FWD_DEFINED__
#define ___ITTDataProxyEvents_FWD_DEFINED__
typedef interface _ITTDataProxyEvents _ITTDataProxyEvents;
#endif 	/* ___ITTDataProxyEvents_FWD_DEFINED__ */


#ifndef __TTDataProxy_FWD_DEFINED__
#define __TTDataProxy_FWD_DEFINED__

#ifdef __cplusplus
typedef class TTDataProxy TTDataProxy;
#else
typedef struct TTDataProxy TTDataProxy;
#endif /* __cplusplus */

#endif 	/* __TTDataProxy_FWD_DEFINED__ */


#ifndef __TTDataProxyProp_FWD_DEFINED__
#define __TTDataProxyProp_FWD_DEFINED__

#ifdef __cplusplus
typedef class TTDataProxyProp TTDataProxyProp;
#else
typedef struct TTDataProxyProp TTDataProxyProp;
#endif /* __cplusplus */

#endif 	/* __TTDataProxyProp_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "WTTCommon.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __ITTDataProxy_INTERFACE_DEFINED__
#define __ITTDataProxy_INTERFACE_DEFINED__

/* interface ITTDataProxy */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITTDataProxy;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8D5DC90E-641F-11D3-BE14-00A0CC23E61B")
    ITTDataProxy : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ITTDataProxyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITTDataProxy __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITTDataProxy __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITTDataProxy __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITTDataProxy __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITTDataProxy __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITTDataProxy __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITTDataProxy __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } ITTDataProxyVtbl;

    interface ITTDataProxy
    {
        CONST_VTBL struct ITTDataProxyVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITTDataProxy_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITTDataProxy_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITTDataProxy_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITTDataProxy_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITTDataProxy_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITTDataProxy_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITTDataProxy_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITTDataProxy_INTERFACE_DEFINED__ */



#ifndef __DATAPROXYLib_LIBRARY_DEFINED__
#define __DATAPROXYLib_LIBRARY_DEFINED__

/* library DATAPROXYLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DATAPROXYLib;

#ifndef ___ITTDataProxyEvents_INTERFACE_DEFINED__
#define ___ITTDataProxyEvents_INTERFACE_DEFINED__

/* interface _ITTDataProxyEvents */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID__ITTDataProxyEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8D5DC90F-641F-11D3-BE14-00A0CC23E61B")
    _ITTDataProxyEvents : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct _ITTDataProxyEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _ITTDataProxyEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _ITTDataProxyEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _ITTDataProxyEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _ITTDataProxyEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _ITTDataProxyEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _ITTDataProxyEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _ITTDataProxyEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _ITTDataProxyEventsVtbl;

    interface _ITTDataProxyEvents
    {
        CONST_VTBL struct _ITTDataProxyEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ITTDataProxyEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _ITTDataProxyEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _ITTDataProxyEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _ITTDataProxyEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _ITTDataProxyEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _ITTDataProxyEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _ITTDataProxyEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ___ITTDataProxyEvents_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_TTDataProxy;

#ifdef __cplusplus

class DECLSPEC_UUID("81F75143-6265-11D3-BE13-00A0CC23E61B")
TTDataProxy;
#endif

EXTERN_C const CLSID CLSID_TTDataProxyProp;

#ifdef __cplusplus

class DECLSPEC_UUID("8D5DC910-641F-11D3-BE14-00A0CC23E61B")
TTDataProxyProp;
#endif
#endif /* __DATAPROXYLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
