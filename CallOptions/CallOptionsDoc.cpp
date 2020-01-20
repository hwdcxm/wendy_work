// CallOptionsDoc.cpp : implementation of the CCallOptionsDoc class
//

#include "stdafx.h"
#include "CallOptions.h"

#include "CallOptionsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCallOptionsDoc

IMPLEMENT_DYNCREATE(CCallOptionsDoc, CDocument)

BEGIN_MESSAGE_MAP(CCallOptionsDoc, CDocument)
	//{{AFX_MSG_MAP(CCallOptionsDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CCallOptionsDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CCallOptionsDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ICallOptions to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {3B94ACBA-97C8-4858-B4C6-02A6BA54B509}
static const IID IID_ICallOptions =
{ 0x3b94acba, 0x97c8, 0x4858, { 0xb4, 0xc6, 0x2, 0xa6, 0xba, 0x54, 0xb5, 0x9 } };

BEGIN_INTERFACE_MAP(CCallOptionsDoc, CDocument)
	INTERFACE_PART(CCallOptionsDoc, IID_ICallOptions, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCallOptionsDoc construction/destruction

CCallOptionsDoc::CCallOptionsDoc()
{
	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
}

CCallOptionsDoc::~CCallOptionsDoc()
{
	AfxOleUnlockApp();
}

BOOL CCallOptionsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCallOptionsDoc serialization

void CCallOptionsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCallOptionsDoc diagnostics

#ifdef _DEBUG
void CCallOptionsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCallOptionsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCallOptionsDoc commands
