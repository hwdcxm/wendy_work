// CallOptionsFView.cpp : implementation of the CCallOptionsFormView class
//

#include "stdafx.h"
#include "CallOptions.h"

#include "CallOptionsDoc.h"
#include "CallOptionsFView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCallOptionsFormView

IMPLEMENT_DYNCREATE(CCallOptionsFormView, CFormView)

BEGIN_MESSAGE_MAP(CCallOptionsFormView, CFormView)
	//{{AFX_MSG_MAP(CCallOptionsFormView)
	ON_BN_CLICKED(IDC_BTN_CALL, OnBtnCall)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCallOptionsFormView construction/destruction

CCallOptionsFormView::CCallOptionsFormView()
	: CFormView(CCallOptionsFormView::IDD)
{
	//{{AFX_DATA_INIT(CCallOptionsFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CCallOptionsFormView::~CCallOptionsFormView()
{
}

void CCallOptionsFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallOptionsFormView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CCallOptionsFormView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CCallOptionsFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CCallOptionsFormView printing

BOOL CCallOptionsFormView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCallOptionsFormView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCallOptionsFormView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CCallOptionsFormView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CCallOptionsFormView diagnostics

#ifdef _DEBUG
void CCallOptionsFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCallOptionsFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCallOptionsDoc* CCallOptionsFormView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCallOptionsDoc)));
	return (CCallOptionsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCallOptionsFormView message handlers

#import "G:\Tele-Trend\bin\Options.dll" no_namespace

void CCallOptionsFormView::OnBtnCall() 
{
	// TODO: Add your control notification handler code here
	CLSID clsid;
	
	HRESULT hr ;
	// Looks up a CLSID in the registry, given a ProgID.
	hr = CLSIDFromProgID(OLESTR("Options.TTOptions.1"),&clsid);
	
	if(S_OK !=hr )
		return;
	
	ITTObject* pObject = NULL; //Interface Pointer 
	
	//	hr = CoCreateInstance(clsid,
	//		NULL,
	//		CLSCTX_INPROC_SERVER,
	//		__uuidof(ITTObject),
	//		(LPVOID*)&pObject);
	
	m_Wnd.CreateControl(_T("Options.TTOptions.1") ,NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,200,200), this, 1 );  //XX.XX.1
	
	IUnknown* pIUnknown = m_Wnd.GetControlUnknown();
	
	HRESULT hr2 =pIUnknown->QueryInterface(__uuidof(ITTObject),(void**)&pObject);
	
	int nID = 0;
	VARIANT mVar;
	VARIANT * pvar = &mVar;

	int iactive = 1;


//[11:21:12.152]:---STDMETHODIMP CTTOptions::PutByID(nID=1000,VARIANT* pVar) b 
	nID = 1000;
	pObject->PutByID(nID,&mVar);

//[11:21:12.152]:---STDMETHODIMP CTTOptions::PutByID(nID=3301,VARIANT* pVar) b
	nID = 3301;
	pObject->PutByID(nID,&mVar);

//[11:21:12.152]:---STDMETHODIMP CTTOptions::PutByID(nID=3188,VARIANT* pVar) b 
		nID = 3188;
	pObject->PutByID(nID,&mVar);

//[11:21:12.152]:---STDMETHODIMP CTTOptions::PutByID(nID=3409,VARIANT* pVar) b
		nID = 3409;
	pObject->PutByID(nID,&mVar);

//[11:21:12.152]:---STDMETHODIMP CTTOptions::PutByID(nID=2024,VARIANT* pVar) b
		nID = 2024;
	pObject->PutByID(nID,&mVar);

//[11:21:12.153]:---STDMETHODIMP CTTOptions::PutByID(nID=4004,VARIANT* pVar) b 
		nID = 4004;
	pObject->PutByID(nID,&mVar);

//[11:21:12.153]:---STDMETHODIMP CTTOptions::PutByID(nID=2060,VARIANT* pVar) b 
		nID = 2060;
	pObject->PutByID(nID,&mVar);

//[11:21:12.153]:---STDMETHODIMP CTTOptions::InitObject() b 
	pObject->InitObject();

//[11:21:12.153]:---STDMETHODIMP CTTOptions::GetByID(nID=2026,VARIANT** ppVar) b 
	nID = 2026;
	pObject->GetByID(nID,&pvar);

//[11:21:12.153]:---STDMETHODIMP CTTOptions::GetByID(nID=2028,VARIANT** ppVar) b 
		nID = 2028;
	pObject->GetByID(nID,&pvar);

//[11:21:12.153]:---STDMETHODIMP CTTOptions::Activate(nState=1) b 
		iactive = 1;
	pObject->Activate(iactive);

//[11:21:12.153]:---STDMETHODIMP CTTOptions::PutByID(nID=1017,VARIANT* pVar) b 
			nID = 1017;
	pObject->PutByID(nID,&mVar);

//[11:21:12.153]:---STDMETHODIMP CTTOptions::GetByID(nID=1021,VARIANT** ppVar) b 
			nID = 1021;
	pObject->GetByID(nID,&pvar);

//[11:21:12.154]:---STDMETHODIMP CTTOptions::PutByID(nID=1110,VARIANT* pVar) b 
				nID = 1110;
	pObject->PutByID(nID,&mVar);

//[11:21:12.154]:---STDMETHODIMP CTTOptions::PutByID(nID=3306,VARIANT* pVar) b 
				nID = 3306;
	pObject->PutByID(nID,&mVar);

//[11:21:12.154]:---STDMETHODIMP CTTOptions::PutByID(nID=3307,VARIANT* pVar) b 
				nID = 3307;
	pObject->PutByID(nID,&mVar);

//[11:21:12.155]:---CTTOptions::SetObjectRects() b 
//[11:21:12.157]:---STDMETHODIMP CTTOptions::GetByID(nID=7010,VARIANT** ppVar) b 
				nID = 7070;
	pObject->GetByID(nID,&pvar);

//[11:21:12.157]:---STDMETHODIMP CTTOptions::GetByID(nID=7010,VARIANT** ppVar) b 
					nID = 7070;
	pObject->GetByID(nID,&pvar);



}
