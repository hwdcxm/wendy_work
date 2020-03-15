/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Apr 24 14:52:52 2019
 */
/* Compiler settings for C:\Users\Administrator\Desktop\w2t\GroupsMng\GroupsMng.idl:
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

#ifndef __GroupsMng_h__
#define __GroupsMng_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ITTGroupsMng_FWD_DEFINED__
#define __ITTGroupsMng_FWD_DEFINED__
typedef interface ITTGroupsMng ITTGroupsMng;
#endif 	/* __ITTGroupsMng_FWD_DEFINED__ */


#ifndef __TTGroupsMng_FWD_DEFINED__
#define __TTGroupsMng_FWD_DEFINED__

#ifdef __cplusplus
typedef class TTGroupsMng TTGroupsMng;
#else
typedef struct TTGroupsMng TTGroupsMng;
#endif /* __cplusplus */

#endif 	/* __TTGroupsMng_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "WTTCommon.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_GroupsMng_0000 */
/* [local] */ 

#define ItemID_ItemCode		0
#define ItemID_Lang1Descrip	1
#define ItemID_Lang2Descrip	2
#define ItemID_Lang3Descrip	3
#define ItemID_SecurityTypes	4
#define ItemID_EarnPerShare	5
#define ItemID_DividPerShare	6
#define ItemID_CorrStockCode	7
#define ItemID_SubscripUnit	8
#define ItemID_SubscripPrice 9
#define ItemID_LotSize		10
#define ItemID_CurrencyCode	11
#define ItemID_PriceType		12
#define ItemID_NonBasketWarrData		13
#define ItemID_BasketWarrData		14
#define ItemID_IsBasketWarr			15
#define ItemID_WarrItemsNumb			16
#define ItemID_CorrItemCode			17
#define ItemID_ExpiryDate	18
#define ItemID_TradeSessionNumber	19
#define ItemID_TradeSessions			20
#define ItemID_CurrencyUnit			21
#define ItemID_AccruInterest			22
#define ItemID_FixInterestCode		23
#define ItemID_SuspensionFlag		24
#define ItemID_ShortSell				25
#define ItemID_CCASS					26
#define ItemID_MessureUnit			27
#define ItemID_SpTradeSessions		28
#define ItemID_ListingDate			29
#define ItemID_HisTradeSessions		30


extern RPC_IF_HANDLE __MIDL_itf_GroupsMng_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_GroupsMng_0000_v0_0_s_ifspec;

#ifndef __ITTGroupsMng_INTERFACE_DEFINED__
#define __ITTGroupsMng_INTERFACE_DEFINED__

/* interface ITTGroupsMng */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITTGroupsMng;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C0DF3111-8A7B-11D3-AED6-00A0CC23E698")
    ITTGroupsMng : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindItem( 
            /* [in] */ BYTE cGroupCode,
            /* [size_is][in] */ BYTE __RPC_FAR *pszItemCode,
            /* [retval][out] */ long __RPC_FAR *pEchoID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindItemByID( 
            /* [in] */ BYTE cGroupCode,
            /* [in] */ int nItemCode,
            /* [retval][out] */ long __RPC_FAR *pEchoID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindFirstItem( 
            /* [in] */ BYTE cGroupCode,
            /* [retval][out] */ long __RPC_FAR *pEchoID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindLastItem( 
            /* [in] */ BYTE cGroupCode,
            /* [retval][out] */ long __RPC_FAR *pEchoID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindPreItem( 
            /* [in] */ long lItemID,
            /* [retval][out] */ long __RPC_FAR *pEchoID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindNextItem( 
            /* [in] */ long lItemID,
            /* [retval][out] */ long __RPC_FAR *pEchoID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SelectItem( 
            /* [size_is][out][in] */ BYTE __RPC_FAR *pEchoBuf) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetItemPara( 
            /* [in] */ long lItemID,
            int nParaID,
            /* [size_is][out] */ BYTE __RPC_FAR *pBuf,
            /* [in] */ int BufLen) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGroupsTotal( 
            /* [retval][out] */ long __RPC_FAR *pEchoGroupsTotal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGroupCode( 
            /* [in] */ long lGroupID,
            /* [size_is][out] */ BYTE __RPC_FAR *pBuf) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetItemsTotalOfGroup( 
            /* [in] */ BYTE cGroupCode,
            /* [retval][out] */ long __RPC_FAR *pEchoItemsTotal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindMatchItem( 
            /* [in] */ BYTE cGroupCode,
            /* [size_is][in] */ BYTE __RPC_FAR *pszItemCode,
            /* [retval][out] */ long __RPC_FAR *pEchoID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGroupName( 
            long lGroupID,
            long lLangType,
            BYTE __RPC_FAR *pBuf) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE PutItemPara( 
            /* [in] */ long lItemID,
            int nParaID,
            /* [size_is][out] */ BYTE __RPC_FAR *pBuf,
            /* [in] */ int BufLen) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE BlockMng( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CSSMng( 
            IUnknown __RPC_FAR *pCSS) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGroupArray( 
            /* [in] */ short iFlag,
            /* [out] */ BYTE __RPC_FAR *piPrivCount,
            /* [retval][out] */ BYTE __RPC_FAR *pStrArray) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetBlockArray( 
            /* [in] */ short iFlag,
            /* [in] */ BYTE __RPC_FAR *pszGroup,
            /* [retval][out] */ BYTE __RPC_FAR *pStrArray) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetItemArray( 
            /* [in] */ short iFlag,
            /* [in] */ BYTE __RPC_FAR *pszGroup,
            /* [in] */ BYTE __RPC_FAR *pszBlock,
            /* [in] */ int __RPC_FAR *pAutolinkwarrants,
            /* [retval][out] */ BYTE __RPC_FAR *pStrArray) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGroupNameByLang( 
            /* [in] */ short iFlag,
            /* [in] */ short iLangType,
            /* [in] */ BYTE __RPC_FAR *pszGroup,
            /* [retval][out] */ BYTE __RPC_FAR *pszValue) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetBlockNameByLang( 
            /* [in] */ short iFlag,
            /* [in] */ short iLangType,
            /* [in] */ BYTE __RPC_FAR *pszGroup,
            /* [in] */ BYTE __RPC_FAR *pszBlock,
            /* [retval][out] */ BYTE __RPC_FAR *pszValue) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SelectBlock( 
            /* [out] */ short __RPC_FAR *piFlag,
            /* [out] */ BYTE __RPC_FAR *pszGroup,
            /* [out] */ BYTE __RPC_FAR *pszBlock,
            /* [out] */ BYTE __RPC_FAR *pszEchoGroup,
            /* [out] */ BYTE __RPC_FAR *pszEchoBlock) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetDefaultBlock( 
            /* [out] */ BYTE __RPC_FAR *pszEchoGroup,
            /* [out] */ BYTE __RPC_FAR *pszEchoBlock) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetExtData( 
            BYTE __RPC_FAR *pszFieldName,
            BYTE __RPC_FAR *pBuf,
            int __RPC_FAR *pBufLen) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE HouseKeeping( 
            DWORD date) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITTGroupsMngVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITTGroupsMng __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITTGroupsMng __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindItem )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ BYTE cGroupCode,
            /* [size_is][in] */ BYTE __RPC_FAR *pszItemCode,
            /* [retval][out] */ long __RPC_FAR *pEchoID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindItemByID )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ BYTE cGroupCode,
            /* [in] */ int nItemCode,
            /* [retval][out] */ long __RPC_FAR *pEchoID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindFirstItem )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ BYTE cGroupCode,
            /* [retval][out] */ long __RPC_FAR *pEchoID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindLastItem )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ BYTE cGroupCode,
            /* [retval][out] */ long __RPC_FAR *pEchoID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindPreItem )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ long lItemID,
            /* [retval][out] */ long __RPC_FAR *pEchoID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindNextItem )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ long lItemID,
            /* [retval][out] */ long __RPC_FAR *pEchoID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectItem )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [size_is][out][in] */ BYTE __RPC_FAR *pEchoBuf);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetItemPara )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ long lItemID,
            int nParaID,
            /* [size_is][out] */ BYTE __RPC_FAR *pBuf,
            /* [in] */ int BufLen);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetGroupsTotal )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pEchoGroupsTotal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetGroupCode )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ long lGroupID,
            /* [size_is][out] */ BYTE __RPC_FAR *pBuf);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetItemsTotalOfGroup )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ BYTE cGroupCode,
            /* [retval][out] */ long __RPC_FAR *pEchoItemsTotal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindMatchItem )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ BYTE cGroupCode,
            /* [size_is][in] */ BYTE __RPC_FAR *pszItemCode,
            /* [retval][out] */ long __RPC_FAR *pEchoID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetGroupName )( 
            ITTGroupsMng __RPC_FAR * This,
            long lGroupID,
            long lLangType,
            BYTE __RPC_FAR *pBuf);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PutItemPara )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ long lItemID,
            int nParaID,
            /* [size_is][out] */ BYTE __RPC_FAR *pBuf,
            /* [in] */ int BufLen);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BlockMng )( 
            ITTGroupsMng __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CSSMng )( 
            ITTGroupsMng __RPC_FAR * This,
            IUnknown __RPC_FAR *pCSS);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetGroupArray )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ short iFlag,
            /* [out] */ BYTE __RPC_FAR *piPrivCount,
            /* [retval][out] */ BYTE __RPC_FAR *pStrArray);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBlockArray )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ short iFlag,
            /* [in] */ BYTE __RPC_FAR *pszGroup,
            /* [retval][out] */ BYTE __RPC_FAR *pStrArray);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetItemArray )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ short iFlag,
            /* [in] */ BYTE __RPC_FAR *pszGroup,
            /* [in] */ BYTE __RPC_FAR *pszBlock,
            /* [in] */ int __RPC_FAR *pAutolinkwarrants,
            /* [retval][out] */ BYTE __RPC_FAR *pStrArray);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetGroupNameByLang )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ short iFlag,
            /* [in] */ short iLangType,
            /* [in] */ BYTE __RPC_FAR *pszGroup,
            /* [retval][out] */ BYTE __RPC_FAR *pszValue);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBlockNameByLang )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [in] */ short iFlag,
            /* [in] */ short iLangType,
            /* [in] */ BYTE __RPC_FAR *pszGroup,
            /* [in] */ BYTE __RPC_FAR *pszBlock,
            /* [retval][out] */ BYTE __RPC_FAR *pszValue);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectBlock )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [out] */ short __RPC_FAR *piFlag,
            /* [out] */ BYTE __RPC_FAR *pszGroup,
            /* [out] */ BYTE __RPC_FAR *pszBlock,
            /* [out] */ BYTE __RPC_FAR *pszEchoGroup,
            /* [out] */ BYTE __RPC_FAR *pszEchoBlock);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDefaultBlock )( 
            ITTGroupsMng __RPC_FAR * This,
            /* [out] */ BYTE __RPC_FAR *pszEchoGroup,
            /* [out] */ BYTE __RPC_FAR *pszEchoBlock);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetExtData )( 
            ITTGroupsMng __RPC_FAR * This,
            BYTE __RPC_FAR *pszFieldName,
            BYTE __RPC_FAR *pBuf,
            int __RPC_FAR *pBufLen);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HouseKeeping )( 
            ITTGroupsMng __RPC_FAR * This,
            DWORD date);
        
        END_INTERFACE
    } ITTGroupsMngVtbl;

    interface ITTGroupsMng
    {
        CONST_VTBL struct ITTGroupsMngVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITTGroupsMng_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITTGroupsMng_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITTGroupsMng_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITTGroupsMng_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITTGroupsMng_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITTGroupsMng_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITTGroupsMng_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITTGroupsMng_FindItem(This,cGroupCode,pszItemCode,pEchoID)	\
    (This)->lpVtbl -> FindItem(This,cGroupCode,pszItemCode,pEchoID)

#define ITTGroupsMng_FindItemByID(This,cGroupCode,nItemCode,pEchoID)	\
    (This)->lpVtbl -> FindItemByID(This,cGroupCode,nItemCode,pEchoID)

#define ITTGroupsMng_FindFirstItem(This,cGroupCode,pEchoID)	\
    (This)->lpVtbl -> FindFirstItem(This,cGroupCode,pEchoID)

#define ITTGroupsMng_FindLastItem(This,cGroupCode,pEchoID)	\
    (This)->lpVtbl -> FindLastItem(This,cGroupCode,pEchoID)

#define ITTGroupsMng_FindPreItem(This,lItemID,pEchoID)	\
    (This)->lpVtbl -> FindPreItem(This,lItemID,pEchoID)

#define ITTGroupsMng_FindNextItem(This,lItemID,pEchoID)	\
    (This)->lpVtbl -> FindNextItem(This,lItemID,pEchoID)

#define ITTGroupsMng_SelectItem(This,pEchoBuf)	\
    (This)->lpVtbl -> SelectItem(This,pEchoBuf)

#define ITTGroupsMng_GetItemPara(This,lItemID,nParaID,pBuf,BufLen)	\
    (This)->lpVtbl -> GetItemPara(This,lItemID,nParaID,pBuf,BufLen)

#define ITTGroupsMng_GetGroupsTotal(This,pEchoGroupsTotal)	\
    (This)->lpVtbl -> GetGroupsTotal(This,pEchoGroupsTotal)

#define ITTGroupsMng_GetGroupCode(This,lGroupID,pBuf)	\
    (This)->lpVtbl -> GetGroupCode(This,lGroupID,pBuf)

#define ITTGroupsMng_GetItemsTotalOfGroup(This,cGroupCode,pEchoItemsTotal)	\
    (This)->lpVtbl -> GetItemsTotalOfGroup(This,cGroupCode,pEchoItemsTotal)

#define ITTGroupsMng_FindMatchItem(This,cGroupCode,pszItemCode,pEchoID)	\
    (This)->lpVtbl -> FindMatchItem(This,cGroupCode,pszItemCode,pEchoID)

#define ITTGroupsMng_GetGroupName(This,lGroupID,lLangType,pBuf)	\
    (This)->lpVtbl -> GetGroupName(This,lGroupID,lLangType,pBuf)

#define ITTGroupsMng_PutItemPara(This,lItemID,nParaID,pBuf,BufLen)	\
    (This)->lpVtbl -> PutItemPara(This,lItemID,nParaID,pBuf,BufLen)

#define ITTGroupsMng_BlockMng(This)	\
    (This)->lpVtbl -> BlockMng(This)

#define ITTGroupsMng_CSSMng(This,pCSS)	\
    (This)->lpVtbl -> CSSMng(This,pCSS)

#define ITTGroupsMng_GetGroupArray(This,iFlag,piPrivCount,pStrArray)	\
    (This)->lpVtbl -> GetGroupArray(This,iFlag,piPrivCount,pStrArray)

#define ITTGroupsMng_GetBlockArray(This,iFlag,pszGroup,pStrArray)	\
    (This)->lpVtbl -> GetBlockArray(This,iFlag,pszGroup,pStrArray)

#define ITTGroupsMng_GetItemArray(This,iFlag,pszGroup,pszBlock,pAutolinkwarrants,pStrArray)	\
    (This)->lpVtbl -> GetItemArray(This,iFlag,pszGroup,pszBlock,pAutolinkwarrants,pStrArray)

#define ITTGroupsMng_GetGroupNameByLang(This,iFlag,iLangType,pszGroup,pszValue)	\
    (This)->lpVtbl -> GetGroupNameByLang(This,iFlag,iLangType,pszGroup,pszValue)

#define ITTGroupsMng_GetBlockNameByLang(This,iFlag,iLangType,pszGroup,pszBlock,pszValue)	\
    (This)->lpVtbl -> GetBlockNameByLang(This,iFlag,iLangType,pszGroup,pszBlock,pszValue)

#define ITTGroupsMng_SelectBlock(This,piFlag,pszGroup,pszBlock,pszEchoGroup,pszEchoBlock)	\
    (This)->lpVtbl -> SelectBlock(This,piFlag,pszGroup,pszBlock,pszEchoGroup,pszEchoBlock)

#define ITTGroupsMng_GetDefaultBlock(This,pszEchoGroup,pszEchoBlock)	\
    (This)->lpVtbl -> GetDefaultBlock(This,pszEchoGroup,pszEchoBlock)

#define ITTGroupsMng_GetExtData(This,pszFieldName,pBuf,pBufLen)	\
    (This)->lpVtbl -> GetExtData(This,pszFieldName,pBuf,pBufLen)

#define ITTGroupsMng_HouseKeeping(This,date)	\
    (This)->lpVtbl -> HouseKeeping(This,date)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_FindItem_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [in] */ BYTE cGroupCode,
    /* [size_is][in] */ BYTE __RPC_FAR *pszItemCode,
    /* [retval][out] */ long __RPC_FAR *pEchoID);


void __RPC_STUB ITTGroupsMng_FindItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_FindItemByID_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [in] */ BYTE cGroupCode,
    /* [in] */ int nItemCode,
    /* [retval][out] */ long __RPC_FAR *pEchoID);


void __RPC_STUB ITTGroupsMng_FindItemByID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_FindFirstItem_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [in] */ BYTE cGroupCode,
    /* [retval][out] */ long __RPC_FAR *pEchoID);


void __RPC_STUB ITTGroupsMng_FindFirstItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_FindLastItem_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [in] */ BYTE cGroupCode,
    /* [retval][out] */ long __RPC_FAR *pEchoID);


void __RPC_STUB ITTGroupsMng_FindLastItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_FindPreItem_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [in] */ long lItemID,
    /* [retval][out] */ long __RPC_FAR *pEchoID);


void __RPC_STUB ITTGroupsMng_FindPreItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_FindNextItem_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [in] */ long lItemID,
    /* [retval][out] */ long __RPC_FAR *pEchoID);


void __RPC_STUB ITTGroupsMng_FindNextItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_SelectItem_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [size_is][out][in] */ BYTE __RPC_FAR *pEchoBuf);


void __RPC_STUB ITTGroupsMng_SelectItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_GetItemPara_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [in] */ long lItemID,
    int nParaID,
    /* [size_is][out] */ BYTE __RPC_FAR *pBuf,
    /* [in] */ int BufLen);


void __RPC_STUB ITTGroupsMng_GetItemPara_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_GetGroupsTotal_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pEchoGroupsTotal);


void __RPC_STUB ITTGroupsMng_GetGroupsTotal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_GetGroupCode_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [in] */ long lGroupID,
    /* [size_is][out] */ BYTE __RPC_FAR *pBuf);


void __RPC_STUB ITTGroupsMng_GetGroupCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_GetItemsTotalOfGroup_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [in] */ BYTE cGroupCode,
    /* [retval][out] */ long __RPC_FAR *pEchoItemsTotal);


void __RPC_STUB ITTGroupsMng_GetItemsTotalOfGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_FindMatchItem_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [in] */ BYTE cGroupCode,
    /* [size_is][in] */ BYTE __RPC_FAR *pszItemCode,
    /* [retval][out] */ long __RPC_FAR *pEchoID);


void __RPC_STUB ITTGroupsMng_FindMatchItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_GetGroupName_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    long lGroupID,
    long lLangType,
    BYTE __RPC_FAR *pBuf);


void __RPC_STUB ITTGroupsMng_GetGroupName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_PutItemPara_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [in] */ long lItemID,
    int nParaID,
    /* [size_is][out] */ BYTE __RPC_FAR *pBuf,
    /* [in] */ int BufLen);


void __RPC_STUB ITTGroupsMng_PutItemPara_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_BlockMng_Proxy( 
    ITTGroupsMng __RPC_FAR * This);


void __RPC_STUB ITTGroupsMng_BlockMng_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_CSSMng_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    IUnknown __RPC_FAR *pCSS);


void __RPC_STUB ITTGroupsMng_CSSMng_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_GetGroupArray_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [in] */ short iFlag,
    /* [out] */ BYTE __RPC_FAR *piPrivCount,
    /* [retval][out] */ BYTE __RPC_FAR *pStrArray);


void __RPC_STUB ITTGroupsMng_GetGroupArray_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_GetBlockArray_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [in] */ short iFlag,
    /* [in] */ BYTE __RPC_FAR *pszGroup,
    /* [retval][out] */ BYTE __RPC_FAR *pStrArray);


void __RPC_STUB ITTGroupsMng_GetBlockArray_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_GetItemArray_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [in] */ short iFlag,
    /* [in] */ BYTE __RPC_FAR *pszGroup,
    /* [in] */ BYTE __RPC_FAR *pszBlock,
    /* [in] */ int __RPC_FAR *pAutolinkwarrants,
    /* [retval][out] */ BYTE __RPC_FAR *pStrArray);


void __RPC_STUB ITTGroupsMng_GetItemArray_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_GetGroupNameByLang_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [in] */ short iFlag,
    /* [in] */ short iLangType,
    /* [in] */ BYTE __RPC_FAR *pszGroup,
    /* [retval][out] */ BYTE __RPC_FAR *pszValue);


void __RPC_STUB ITTGroupsMng_GetGroupNameByLang_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_GetBlockNameByLang_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [in] */ short iFlag,
    /* [in] */ short iLangType,
    /* [in] */ BYTE __RPC_FAR *pszGroup,
    /* [in] */ BYTE __RPC_FAR *pszBlock,
    /* [retval][out] */ BYTE __RPC_FAR *pszValue);


void __RPC_STUB ITTGroupsMng_GetBlockNameByLang_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_SelectBlock_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [out] */ short __RPC_FAR *piFlag,
    /* [out] */ BYTE __RPC_FAR *pszGroup,
    /* [out] */ BYTE __RPC_FAR *pszBlock,
    /* [out] */ BYTE __RPC_FAR *pszEchoGroup,
    /* [out] */ BYTE __RPC_FAR *pszEchoBlock);


void __RPC_STUB ITTGroupsMng_SelectBlock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_GetDefaultBlock_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    /* [out] */ BYTE __RPC_FAR *pszEchoGroup,
    /* [out] */ BYTE __RPC_FAR *pszEchoBlock);


void __RPC_STUB ITTGroupsMng_GetDefaultBlock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_GetExtData_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    BYTE __RPC_FAR *pszFieldName,
    BYTE __RPC_FAR *pBuf,
    int __RPC_FAR *pBufLen);


void __RPC_STUB ITTGroupsMng_GetExtData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITTGroupsMng_HouseKeeping_Proxy( 
    ITTGroupsMng __RPC_FAR * This,
    DWORD date);


void __RPC_STUB ITTGroupsMng_HouseKeeping_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITTGroupsMng_INTERFACE_DEFINED__ */



#ifndef __GROUPSMNGLib_LIBRARY_DEFINED__
#define __GROUPSMNGLib_LIBRARY_DEFINED__

/* library GROUPSMNGLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_GROUPSMNGLib;

EXTERN_C const CLSID CLSID_TTGroupsMng;

#ifdef __cplusplus

class DECLSPEC_UUID("C0DF3112-8A7B-11D3-AED6-00A0CC23E698")
TTGroupsMng;
#endif
#endif /* __GROUPSMNGLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
