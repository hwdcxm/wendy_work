// MsEdit_WendyDlg.h : header file
//

#include "ShowData.h"

#if !defined(AFX_MSEDIT_WENDYDLG_H__9D89B960_275B_4ECE_9150_C385110DAFA2__INCLUDED_)
#define AFX_MSEDIT_WENDYDLG_H__9D89B960_275B_4ECE_9150_C385110DAFA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMsEdit_WendyDlg dialog

class CMsEdit_WendyDlg : public CDialog
{
// Construction
public:
	CMsEdit_WendyDlg(CWnd* pParent = NULL);	// standard constructor

	CShowData * pShowData;
// Dialog Data
	//{{AFX_DATA(CMsEdit_WendyDlg)
	enum { IDD = IDD_READMDS };
	CListBox	m_filelist;
	CString	m_TransCode;
	CString	m_item;
	CString	m_time;
	CString	m_delay;
	CString	m_time_end;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsEdit_WendyDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMsEdit_WendyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAddfile();
	afx_msg void OnDelfile();
	afx_msg void OnRead();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSEDIT_WENDYDLG_H__9D89B960_275B_4ECE_9150_C385110DAFA2__INCLUDED_)