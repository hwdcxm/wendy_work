#if !defined(AFX_MYPROGRESS_H__A13409BA_7CB7_419B_9E84_41FFEAFB93DF__INCLUDED_)
#define AFX_MYPROGRESS_H__A13409BA_7CB7_419B_9E84_41FFEAFB93DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyProgress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyProgress window

class CMyProgress : public CProgressCtrl
{
// Construction
public:
	CMyProgress();
	COLORREF m_crProgress;
	COLORREF m_crBlank;
    COLORREF m_crText;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyProgress)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyProgress();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyProgress)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPROGRESS_H__A13409BA_7CB7_419B_9E84_41FFEAFB93DF__INCLUDED_)
