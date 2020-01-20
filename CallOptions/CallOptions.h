// CallOptions.h : main header file for the CALLOPTIONS application
//

#if !defined(AFX_CALLOPTIONS_H__7A313012_2592_4488_9437_66407D83FC11__INCLUDED_)
#define AFX_CALLOPTIONS_H__7A313012_2592_4488_9437_66407D83FC11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCallOptionsApp:
// See CallOptions.cpp for the implementation of this class
//

class CCallOptionsApp : public CWinApp
{
public:
	CCallOptionsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCallOptionsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CCallOptionsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLOPTIONS_H__7A313012_2592_4488_9437_66407D83FC11__INCLUDED_)
