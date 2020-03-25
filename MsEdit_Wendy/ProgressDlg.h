#if !defined(AFX_PROGRESSDLG_H__2081B7B4_6810_4434_A877_3D0FE940C7D2__INCLUDED_)
#define AFX_PROGRESSDLG_H__2081B7B4_6810_4434_A877_3D0FE940C7D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog

class CProgressDlg : public CDialog
{
// Construction
public:
	LRESULT OnEndDlg( WPARAM wParam, LPARAM lParam );
	CProgressDlg(CWnd* pParent = NULL);   // standard constructor

	CStatic* m_pStatic_Read;
	CStatic* m_pStatic_Size;
// Dialog Data
	//{{AFX_DATA(CProgressDlg)
	enum { IDD = IDD_PROGRESS };
	CProgressCtrl	m_progressCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgressDlg)
	afx_msg LRESULT OnUpdateProgress( WPARAM wParam, LPARAM lParam );
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDLG_H__2081B7B4_6810_4434_A877_3D0FE940C7D2__INCLUDED_)
