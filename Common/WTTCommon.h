/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Jan 21 15:13:05 2020
 */
/* Compiler settings for E:\wendy_work\Common\WTTCommon.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
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

#ifndef __WTTCommon_h__
#define __WTTCommon_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ITTObject_FWD_DEFINED__
#define __ITTObject_FWD_DEFINED__
typedef interface ITTObject ITTObject;
#endif 	/* __ITTObject_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_WTTCommon_0000 */
/* [local] */ 

//+-------------------------------------------------------------------------
//
//  Eagleye Project
//  Copyright 1999 - 2000 Tele-Trend Ltd.
//  Author: 
//  Date: 
//
//--------------------------------------------------------------------------
#if ( _MSC_VER >= 800 )
#pragma warning(disable:4201)    /* Nameless struct/union */
#endif
#define TTPID_SystemObject		1000
#define TTPID_Container			1001
#define TTPID_RootStorage		1002
#define TTPID_ContainerStorage	1003
#define TTPID_Storage			1004
#define TTPID_Stream				1005
#define TTPID_Exception			1006
#define TTPID_DataProxy			1007
#define TTPID_MainFrame			1008
#define TTPID_SystemDefault		1009
#define TTPID_UserStg			1010
#define TTPID_DataFrameMAS       1011
#define TTPID_EventLog			1012
#define TTPID_GroupsMng			1013
#define TTPID_DlgObj				1100
#define TTPID_StatObj			1101
#define TTPID_GBBIG5Obj			1102
#define TTPID_CSS				1103
#define TTPID_RadarAlert			1104
#define TTPID_RecalcObj			1105
#define TTPID_ISO				1106
#define TTPID_ChartDoProcess		1043
#define TTPID_MinKeepHour		1110
#define TTPID_ControlStorage		1014
#define TTPID_HtmlStream		1015
#define TTPID_IsDelayRelease		1016
#define TTPID_MainCommand		1017
#define TTPID_PrivateStorage		1019
#define TTPID_ItemCodeWillChg	1020
#define TTPID_GiveMeUTitle		1021
#define TTPID_SetMeTitle			1024
#define TTPID_DecompressByteSum	1025
#define TTPID_FrameSum			1026
#define TTPID_CompressFrameSum	1027
#define TTPID_ErrorFrame			1028
#define TTPID_CompressByteSum	1029
#define TTPID_ByteSum			1030
#define TTPID_MasWriteLen		1031
#define TTPID_ErrorByte			1032
#define TTPID_MasUseSize			1033
#define TTPID_ReplyIStream		1034
#define TTPID_DataExporter		1035
#define TTPID_OnlineUpdate		1036
#define TTPID_TradeEngine		1040
#define TTPID_ObjGlobalID		2000
#define TTPID_FireBroker			2002
#define TTPID_FireNoBroker		2003
#define TTPID_MultiKbdObject		2004
#define TTPID_KeyEvent			2006
#define TTPID_DInputMainHwnd		2008
#define TTPID_SetBrokerFocus		2012
#define TTPID_ControlID			2024
#define TTPID_ResHinstance		2026
#define TTPID_ResToolBarID		2028
#define TTPID_ClkMDIToolBar		2030
#define TTPID_GETMDIToolBarState		2031
#define TTPID_SystemDate			2040
#define TTPID_ItemCode			2050
#define TTPID_ItemCodeA			2060
#define TTPID_ItemCodeEx			2070
#define TTPID_ItemCodeOrder			2080
#define TTPID_GetCtrlItemCode	2061
#define TTPID_GetCtrlGroupCode	2063
#define TTPID_ItemCodePrint		2064
#define TTPID_GetDeviceStatus	3060
#define TTPID_GetDeviceStatusString	3061
#define TTPID_MessageWindow		3063
#define TTPID_FileReload			3065
#define TTPID_ES_DO_START			3066
#define TTPID_ES_DO_STOP			3067
#define TTPID_ES_AUTO_CHECKHISTDATA	3068
#define TTPID_HouseKeeping			4000
#define TTPID_NewsAccount			4003
#define TTPID_FunctionControl		4004
#define TTPID_HELP					4005
#define TTPID_GroupAccount			4006
#define TTPID_SystemAccount			4009
#define TTPID_WTVersion				4100
#define TTPID_W2TVersion				4110
#define TTPID_TELE_DSpreadBid			2208
#define TTPID_TELE_DSpreadAsk			2218
#define TTPID_TELE_DNominalRise			2228
#define TTPID_TELE_DNominalFall			2238
#define TTPID_TELE_DNominalUnChg			2248
#define TTPID_TELE_DTradeTickers			2258
#define TTPID_TELE_DHSIndex				2268
#define TTPID_TELE_DHSFutures			2278
#define TTPID_TELE_LHSIndex				2308
#define TTPID_TELE_LHSFutures			2318
#define TTPID_TELE_LNominal				2328
#define TTPID_TELE_DLargestTrans			2338
#define TTPID_TELE_DFreeText				2348
#define TTPID_TELE_LBid					2358
#define TTPID_TELE_LAsk					2368
#define TTPID_TELE_LWar					2808
#define TTPID_TELE_DWarItem				2828
#define TTPID_TELE_DWarSelected			2838
#define TTPID_TELE_DBidFore				2508
#define TTPID_TELE_DAskFore				2528
#define TTPID_UserName					2708
#define TTPID_Password					2728
#define TTPID_NewPassword				2738
#define TTPID_UserID						3108
#define TTPID_SystemID					3118
#define TTPID_ExtSystemID				3128
#define TTPID_ExtSystemIDMask			3138
#define TTPID_Frame247					3148
#define TTPID_SystemFinger				3158
#define TTPID_DataManger					3168
#define TTPID_DataUpdateFile				3178
#define TTPID_MaxInstance				3188
#define TTPID_ToolBarTip					3248
#define TTPID_LangType					3249
#define TTPID_ToolBarUI					3250
#define TTPID_DataObjectDate				5000
#define TTPID_DataSendCritical			5100
#define TTPID_DP_TRANS					2601
#define TTPID_ConfigCenter				3300
#define TTPID_TradeInfo					3301
#define TTPID_SendTradeInfo				3302
#define TTPID_RECAL						3304
#define TTPID_ErrorLog						3305
#define TTPID_SETSTS						3306
#define TTPID_DELAYSETTING				3307
#define TTPID_CLEANALLDATA				3308
#define TTPID_StrCommand						39355
#define TTPID_ALARM_START				3400
#define TTPID_ALARM_STOP				3401
#define TTPID_ALARM_SETUP				3402
#define TTPID_ALARM_MONITOR				3403
#define TTPID_DataFrame_DOReady				3404
#define TTPID_W2TE_DO_RELOAD				3405
#define TTPID_PORTFOLIO_CFG_CHG				3406
#define TTPID_PORTFOLIO_CFG_CHG_COLOR		3407
#define TTPID_CFG_CHANGE				3408
#define TTPID_MsgShowInfo			3409
#define TTPID_MsgMine_Change			3410
#define TTPID_MsgMine_Setting		3411
#define TTPID_ISO_ConType			3412
#define TTPID_ISO_ConStatus			3413
#define TTPID_W2T_CONNSTATUS			6000
#define TTPID_W2T_CONNSTATUSPROXY	6011
#define TTPID_W2T_SERVERMSG			6100
#define TTPID_ES_HisDataSync			6101
#define TTPID_W2T_SERVERNAME			6110
#define TTPID_WorkingPath			7000
#define TTPID_NotifyExtDataFileChg	7001
#define TTPID_Window_TYPE			7010
#define TTPID_IS_SYSTEM_DESKTOP		7050
#define TTPID_OptionsFiles			7060
#define TTPID_Rerfresh			7061
#define TTPID_CloseLoginWindow			7062
#define TTPID_CloseOrderWindow			7063



extern RPC_IF_HANDLE __MIDL_itf_WTTCommon_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_WTTCommon_0000_v0_0_s_ifspec;

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


/* interface __MIDL_itf_WTTCommon_0094 */
/* [local] */ 

#if ( _MSC_VER >= 800 )
#pragma warning(default:4201) /* Nameless struct/union */
#endif


extern RPC_IF_HANDLE __MIDL_itf_WTTCommon_0094_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_WTTCommon_0094_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
