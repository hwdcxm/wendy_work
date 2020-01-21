// TTOptions.h : Declaration of the CTTOptions

#ifndef __TTOPTIONS_H_
#define __TTOPTIONS_H_

#include "resource.h"       // main symbols
#include <atlctl.h>

#include "..\common\DataProxy.h" 

#include "logfile.h" // wendy add 2020.01.20 2019.12.27

/////////////////////////////////////////////////////////////////////////////
// CTTOptions
class ATL_NO_VTABLE CTTOptions : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<ITTOptions, &IID_ITTOptions, &LIBID_OPTIONSLib>,
	public IDispatchImpl<_ITTDataProxyEvents,&IID__ITTDataProxyEvents, &LIBID_DATAPROXYLib>, //
	public CComControl<CTTOptions>,
	public IPersistStreamInitImpl<CTTOptions>,
	public IOleControlImpl<CTTOptions>,
	public IOleObjectImpl<CTTOptions>,
	public IOleInPlaceActiveObjectImpl<CTTOptions>,
	public IViewObjectExImpl<CTTOptions>,
	public IOleInPlaceObjectWindowlessImpl<CTTOptions>,
	public ISupportErrorInfo,
	public IPersistStorageImpl<CTTOptions>,
	public ISpecifyPropertyPagesImpl<CTTOptions>,
	public IQuickActivateImpl<CTTOptions>,
	public IDataObjectImpl<CTTOptions>,
	public IProvideClassInfo2Impl<&CLSID_TTOptions, NULL, &LIBID_OPTIONSLib>,
	public CComCoClass<CTTOptions, &CLSID_TTOptions>,
	public ITTObject
{
public:
	CContainedWindow m_ctlEdit;

	// ITTObject相关数据.
	IUnknown*		m_pDataProxy;
	IUnknown*		m_pSystemObj;	
	IUnknown*		m_pGrpMng;
	IUnknown*		m_pMainFrm;
	int				m_nControlID;		// control 在MainFrame中的标识ID.		
	unsigned long	m_dwID;				// control 在DO中的标识ID.		
	HANDLE			m_hAdviseThread;	// advise thread hande. Warning: not CloseHandle()
	DWORD			m_dwAdvise;			// Advise返回值, 未用.
	BOOL			m_bAdvise;			// 连接命令 or 短开连接命令.		
	LONG            m_PTransdate;           //获取保存的当前Options的日期.

	CTTOptions() :	
		m_ctlEdit(_T("Edit"), this, 1)
	{
		CLogout("---CTTOptions::CTTOptions() b \r\n");
		m_bWindowOnly = TRUE;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TTOPTIONS)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTTOptions)
	COM_INTERFACE_ENTRY(ITTOptions)
	COM_INTERFACE_ENTRY2(IDispatch,ITTOptions)
	COM_INTERFACE_ENTRY(_ITTDataProxyEvents)  //
//	COM_INTERFACE_ENTRY(IDispatch) //
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY(ITTObject)
END_COM_MAP()

BEGIN_PROP_MAP(CTTOptions)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_MSG_MAP(CTTOptions)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
	CHAIN_MSG_MAP(CComControl<CTTOptions>)
ALT_MSG_MAP(1)
	// Replace this with message map entries for superclassed Edit
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	BOOL PreTranslateAccelerator(LPMSG pMsg, HRESULT& hRet)
	{
		CLogout("---CTTOptions::PreTranslateAccelerator() b \r\n");
		if(pMsg->message == WM_KEYDOWN && 
			(pMsg->wParam == VK_LEFT || 
			pMsg->wParam == VK_RIGHT ||
			pMsg->wParam == VK_UP ||
			pMsg->wParam == VK_DOWN))
		{
			hRet = S_FALSE;
			return TRUE;
		}
		//TODO: Add your additional accelerator handling code here
		return FALSE;
	}

	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CLogout("---CTTOptions::OnSetFocus() b \r\n");

		LRESULT lRes = CComControl<CTTOptions>::OnSetFocus(uMsg, wParam, lParam, bHandled);
		if (m_bInPlaceActive)
		{
			DoVerbUIActivate(&m_rcPos,  NULL);
			if(!IsChild(::GetFocus()))
				m_ctlEdit.SetFocus();
		}
		return lRes;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CLogout("---CTTOptions::OnCreate() b \r\n");

		RECT rc;
		GetWindowRect(&rc);
		rc.right -= rc.left;
		rc.bottom -= rc.top;
		rc.top = rc.left = 0;
		m_ctlEdit.Create(m_hWnd, rc);
		return 0;
	}
	STDMETHOD(SetObjectRects)(LPCRECT prcPos,LPCRECT prcClip)
	{
		CLogout("---CTTOptions::SetObjectRects() b \r\n");
		IOleInPlaceObjectWindowlessImpl<CTTOptions>::SetObjectRects(prcPos, prcClip);
		int cx, cy;
		cx = prcPos->right - prcPos->left;
		cy = prcPos->bottom - prcPos->top;
		::SetWindowPos(m_ctlEdit.m_hWnd, NULL, 0,
			0, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE);
		return S_OK;
	}

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		CLogout("---CTTOptions::InterfaceSupportsErrorInfo() b \r\n");

		static const IID* arr[] = 
		{
			&IID_ITTOptions,
		};
		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// ITTOptions
public:
	// ITTObject
	STDMETHOD(InitObject)();	
	STDMETHOD(Activate)(int nState);
	STDMETHOD(GetState)(int* pnState);
	STDMETHOD(GetByID)(int nID, VARIANT** ppVar);	
	STDMETHOD(PutByID)(int nID, VARIANT* pVar);
	STDMETHOD(GetByName)(BSTR strName, VARIANT** ppVar);
	STDMETHOD(PutByName)(BSTR strName, VARIANT* pVar);
};

#endif //__TTOPTIONS_H_
