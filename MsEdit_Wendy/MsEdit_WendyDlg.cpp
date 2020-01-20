// MsEdit_WendyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MsEdit_Wendy.h"
#include "MsEdit_WendyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

Master g_Master;
HANDLE g_hEvent;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsEdit_WendyDlg dialog

CMsEdit_WendyDlg::CMsEdit_WendyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsEdit_WendyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsEdit_WendyDlg)
	m_TransCode = _T("");
	m_item = _T("");
	m_time = _T("");
	m_delay = _T("");
	m_time_end = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMsEdit_WendyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsEdit_WendyDlg)
	DDX_Control(pDX, IDC_LIST1, m_filelist);
	DDX_Text(pDX, IDC_TRANSCODE, m_TransCode);
	DDX_Text(pDX, IDC_ITEM, m_item);
	DDX_Text(pDX, IDC_TIME, m_time);
	DDX_Text(pDX, IDC_DELAY, m_delay);
	DDX_Text(pDX, IDC_TIME_END, m_time_end);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMsEdit_WendyDlg, CDialog)
	//{{AFX_MSG_MAP(CMsEdit_WendyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADDFILE, OnAddfile)
	ON_BN_CLICKED(IDC_DELFILE, OnDelfile)
	ON_BN_CLICKED(IDC_READ, OnRead)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsEdit_WendyDlg message handlers

BOOL CMsEdit_WendyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	g_hEvent = CreateEvent(NULL, FALSE, FALSE, "Data_CListCtr");
	if (g_hEvent) 
	{ 
		if (ERROR_ALREADY_EXISTS == GetLastError()) 
		{ 
			TRACE( "the instance is exist! \n");			
		} 
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMsEdit_WendyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMsEdit_WendyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMsEdit_WendyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMsEdit_WendyDlg::OnAddfile() 
{
	// TODO: Add your control notification handler code here
	CFileDialog fdlg(true, NULL, NULL, OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR, "(*.mds)|*.mds|All Files (*.*)|*.*||", NULL);
	if(fdlg.DoModal()==IDOK)
	{
	
		for (int i=0;i<m_filelist.GetCount();i++)
		{
			if (m_filelist.FindString(0,fdlg.GetPathName())>=0)
			{
				AfxMessageBox("Already add this file.");
				return;
			}
		}
		m_filelist.AddString(fdlg.GetPathName());
	}
}

void CMsEdit_WendyDlg::OnDelfile() 
{
	// TODO: Add your control notification handler code here
		int i,iState;
		
	int nItemSelected=m_filelist.GetCurSel();
	if(nItemSelected < 0)
	{
		MessageBox("��ѡ����Ҫɾ�����ļ�(choose item).");
		return ;
	}

	m_filelist.DeleteString(nItemSelected);

	m_filelist.SetFocus();
}

extern int counts;

CStringArray pre_files ;

extern CListDataFrame mCListDataFrame;
extern CListFrame_e mCListFrame_e;

void CMsEdit_WendyDlg::OnRead() 
{
	// TODO: Add your control notification handler code here
	counts = 0;
	memset(&g_Master.m_itemdesk,0,sizeof(Master::OutItem));
	int m_count ;
	m_count = m_filelist.GetCount();
	int m_precount = pre_files.GetSize();
	int m_needreadfs = 0;
	if (m_count==0)
	{
		AfxMessageBox("���������ļ�(Please add files).");
		return;
	}
	UpdateData(TRUE);
	//if (!CheckInfo())
	//{
	//	AfxMessageBox("����������(Input item error).");
		//return;
	//}

	char szTemp[256];
	CStringArray files ;

	if (m_count != m_precount)
	{
		m_needreadfs = 1;
	}
	else if (m_precount > 0)
	{
		for (int i=0;i<pre_files.GetSize();i++)
		{
			if (m_filelist.FindString(0,pre_files.GetAt(i))>=0)
			{
				TRACE("Already read this file(%d). \n",i+1);
			}
			else
			{
				m_needreadfs = 1;
			}
		}
	}

	
	TRACE("m_needreadfs=%d. \n",m_needreadfs);

	if (m_needreadfs || !mCListDataFrame.GetCount())
	{
		
		if(g_Master.m_MSCEType != MSCE_TYPE_PROXY)
		{
			g_Master.m_pDataSrc_TTFrameFile = new CDataSrc_TTFrameFile() ;
			g_Master.m_pDataSrc_TTFrameFile->m_pUnpacker=&g_Master.m_unpakcer_TTFrame;
			//g_Master.m_pDataSrc_TTFrameFile->m_pCacheStream = &g_Master.m_cacheStream_TTFrame ;
			
			//CStringArray files ;
			int m_count = m_filelist.GetCount();
			for( int i=0;i<m_count;i++ )
			{
				m_filelist.GetText(i,szTemp);
				if( strlen(szTemp)<=0 )
				{
					break ;
				}
				files.Add( szTemp ) ;
				memset(szTemp,0,sizeof(szTemp));
			}			
			g_Master.m_pDataSrc_TTFrameFile->Init( files,g_Master.m_itemdesk.delay );
		}
		
		Sleep(1000);
		
		if(files.GetSize())
		{
			pre_files.Copy(files);
		}

		if (mCListDataFrame.IsEmpty() == 0)
		{
			mCListDataFrame.RemoveAll();
		}

		if (mCListFrame_e.IsEmpty() == 0)
		{
			mCListFrame_e.RemoveAll();
		}

		ResetEvent(g_hEvent);
		
		TRACE("Read Files.... \n");
		g_Master.Play();
	}
	else
	{
		SetEvent(g_hEvent);
	}

	//OnCancel();
	pShowData =  new CShowData;
	pShowData->Create(IDD_SHOWDATA);
	pShowData->ShowWindow(SW_SHOW);
	
	//CShowData mCShowData;  //dlg.DoModal();
	//mCShowData.DoModal();
}

BOOL CMsEdit_WendyDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (mCListDataFrame.IsEmpty() == 0)
	{
		mCListDataFrame.RemoveAll();
	}
	
	if (mCListFrame_e.IsEmpty() == 0)
	{
		mCListFrame_e.RemoveAll();
	}
	
	if (g_Master.m_pDataSrc_TTFrameFile)
	{
		delete g_Master.m_pDataSrc_TTFrameFile;
		g_Master.m_pDataSrc_TTFrameFile = NULL;
	}

	return CDialog::DestroyWindow();
}