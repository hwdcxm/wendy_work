#if !defined(AFX_EINFO_H__C9058785_A138_4F85_8A54_6C5DCB6D71EF__INCLUDED_)
#define AFX_EINFO_H__C9058785_A138_4F85_8A54_6C5DCB6D71EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EINFO.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEINFO dialog

class CEINFO : public CDialog
{
// Construction
public:
	CEINFO(CWnd* pParent = NULL);   // standard constructor

	char strGroup[100];
	char strItemCode[100];
	char strTime[100];
	char strLength[100];

	_Frame_e mFrame_e;

	int cmp_item(_Frame_e *pFrame_e);

	int cmp_time(_Frame_e *pFrame_e);


// Dialog Data
	//{{AFX_DATA(CEINFO)
	enum { IDD = IDD_E_INFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEINFO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEINFO)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EINFO_H__C9058785_A138_4F85_8A54_6C5DCB6D71EF__INCLUDED_)
