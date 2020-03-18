// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "msedit_wendy.h"
#include "ProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern DWORD g_fileSize;

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog


CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_progressCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CProgressDlg)
	ON_MESSAGE(WM_PROGRESS_MESSAGE, OnUpdateProgress )
	ON_WM_CLOSE()
	ON_MESSAGE(WM_PROGRESS_END, OnEndDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg message handlers

LRESULT CProgressDlg::OnUpdateProgress( WPARAM wParam, LPARAM lParam )
{
	DWORD nProgress = lParam;
	CString getstr;
	
	DWORD g_fileSizeKB = g_fileSize/1024;
	DWORD SetNum = nProgress * 100 / g_fileSizeKB;

	m_progressCtrl.SetPos( SetNum );

	getstr.Format("Read: %d KB",nProgress);
	m_pStatic_Read->SetWindowText(getstr);

	if( SetNum >= 100 )
	{
		//PostQuitMessage(0);
		//OnOK();
	}
	return NULL;
}

BOOL CProgressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_progressCtrl.SetRange(0,100);  // g_fileSize / 1024
	
	m_pStatic_Read=(CStatic*)GetDlgItem(IDC_STATIC_READ);
	m_pStatic_Size=(CStatic*)GetDlgItem(IDC_STATIC_SIZE);

//	int nLower, nUpper;
	CString getstrUpper;
//	m_progressCtrl.GetRange(nLower, nUpper);

	getstrUpper.Format("FilesSize: %d KB ", g_fileSize / 1024);
	m_pStatic_Size->SetWindowText(getstrUpper);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProgressDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
	DestroyWindow();

}

void CProgressDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::PostNcDestroy();
	if (AfxGetMainWnd()->IsWindowEnabled())
	{
		delete this;
	}
}

LRESULT CProgressDlg::OnEndDlg(WPARAM wParam, LPARAM lParam)
{
	OnClose();
	return NULL;
}
