#if !defined(AFX_SHOWDATA_H__BB49E559_78CD_4C30_BAA6_7C3C6E294905__INCLUDED_)
#define AFX_SHOWDATA_H__BB49E559_78CD_4C30_BAA6_7C3C6E294905__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowData.h : header file
//
#include "EINFO.h"
/////////////////////////////////////////////////////////////////////////////
// CShowData dialog

#include "Datadef.h"
typedef CArray<_tagTTDataFrame, _tagTTDataFrame&> CArrayItemDataFrame;

typedef CList<_tagTTDataFrame, _tagTTDataFrame&> CListDataFrame;

typedef CList<_Frame_e, _Frame_e&> CListFrame_e;


#include "lock.h"

class CMsEdit_WendyDlg;

class CShowData : public CDialog
{
// Construction
public:
	CShowData(CWnd* pParent = NULL);   // standard constructor

	CEINFO *pEINFO;

	CAutoLock	m_lock ;

	CMsEdit_WendyDlg *pME_WD;
	
	int op_item(_tagTTDataFrame *pStFrame);
	
	int op_TransCode(_tagTTDataFrame *pStFrame);
	
	int op_time(_tagTTDataFrame *pStFrame);
	
	int op_delay(_tagTTDataFrame *pStFrame);
	
	BOOL noselitem, noselcode, noseltime, nosetdelay;
	
	// Dialog Data
	//{{AFX_DATA(CShowData)
	enum { IDD = IDD_SHOWDATA };
	CEdit	m_ctrlEditCell;
	CListCtrl	m_list_data;
	CString	m_strEditCell;
	//}}AFX_DATA

	CStatusBarCtrl m_StatusBar;
	CString StatusBar_fileinfo; // char [128]
	CString StatusBar_filesize;
	DWORD m_filesize;
	CString strsearch;
	BOOL enable_timer;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	POSITION ps;

	int DataCount_bak;
	int DataCount;
	int ListIndex;
	int Datainterval;

	int frame_e_count;

	char string[25];
	//CString GroupCodeString;
	char GroupCodeString[3];
	//CString CStrnLength;
	char CStrnLength[6];
	CString CStrlTime;
	//CString CStrItemCode;
	char CStrItemCode[10];
	//CString CStrTransCode;
	char CStrTransCode[3];

	CString CStrnTransDataLength;
	CString CStrpTransData;

	int ListIndex_bak;
	int GetIndex;
	int GetIndex_bak;

	CPoint m_indexOfCell;

	NM_LISTVIEW*   dlg_pNMListView;


	// Generated message map functions
	//{{AFX_MSG(CShowData)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusEditCell();
	afx_msg void OnRclickListData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemInfo();
	afx_msg void OnMenuitemCopy();
	afx_msg void OnBtnTimer();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWDATA_H__BB49E559_78CD_4C30_BAA6_7C3C6E294905__INCLUDED_)
