// CallOptionsFView.h : interface of the CCallOptionsFormView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALLOPTIONSFVIEW_H__67C974E9_CA3E_4073_A2AA_1303E1CF7682__INCLUDED_)
#define AFX_CALLOPTIONSFVIEW_H__67C974E9_CA3E_4073_A2AA_1303E1CF7682__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCallOptionsFormView : public CFormView
{
protected: // create from serialization only
	CCallOptionsFormView();
	DECLARE_DYNCREATE(CCallOptionsFormView)

public:
	//{{AFX_DATA(CCallOptionsFormView)
	enum{ IDD = IDD_CALLOPTIONS_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CCallOptionsDoc* GetDocument();
	CWnd m_Wnd;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCallOptionsFormView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCallOptionsFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCallOptionsFormView)
	afx_msg void OnBtnCall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CallOptionsFView.cpp
inline CCallOptionsDoc* CCallOptionsFormView::GetDocument()
   { return (CCallOptionsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLOPTIONSFVIEW_H__67C974E9_CA3E_4073_A2AA_1303E1CF7682__INCLUDED_)
