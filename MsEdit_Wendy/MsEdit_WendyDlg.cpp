// MsEdit_WendyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MsEdit_Wendy.h"
#include "MsEdit_WendyDlg.h"
#include "log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Master g_Master;
HANDLE g_hEvent;
//CLog w_Log;
CLog w_InofLog;
CString s_Log;
CString s_eLog;
CProgressDlg *g_pProDlg;
DWORD g_fileSizeKB;
DWORD g_fileSizeMB;

extern CListDataFrame mCListDataFrame;
extern CListFrame_e mCListFrame_e;

extern CListFrame_Simple mCListFrame_Simple;

extern DWORD DirectWrite_mIndexNo;

CStringArray pre_files ;

// for read set.ini value
int mode=1;
DWORD maxMB=500;

int pre_mode=mode;
DWORD pre_maxMB=maxMB;


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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
	m_time_end = _T("");
	m_fsize = 0;
	InitCreateGroupsMng = FALSE;
	m_needreadfs = 0;
	m_strfsize = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMsEdit_WendyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsEdit_WendyDlg)
	DDX_Control(pDX, IDC_SET, m_set);
	DDX_Control(pDX, IDC_LOG, m_loglist);
	DDX_Control(pDX, IDC_LIST1, m_filelist);
	DDX_Text(pDX, IDC_TRANSCODE, m_TransCode);
	DDX_Text(pDX, IDC_ITEM, m_item);
	DDX_Text(pDX, IDC_TIME, m_time);
	DDX_Text(pDX, IDC_TIME_END, m_time_end);
	DDX_Text(pDX, IDC_FSIZE, m_strfsize);
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
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SET, OnSet)
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

	m_loglist.SetHorizontalExtent(1200);  //HorizontalScorllbar
	//w_Log.Init("ReadMds.log",1024*1024*1024);  // wendy 1024*1024
	w_InofLog.Init("InfoMds.log",1024*1024*1024);  // wendy 1024*1024
	s_Log.Format("==========================Start Application==========================");
	SetTimer(1, 10, NULL) ;  // 350
	
	g_hEvent = CreateEvent(NULL, FALSE, FALSE, "Data_CListCtr");
	if (g_hEvent) 
	{ 
		if (ERROR_ALREADY_EXISTS == GetLastError()) 
		{ 
			TRACE( "ER: Event Data_CListCtr ALREADY_EXISTS ! \n");			
		} 
	}

	//读set.ini
	//如果 Key 值没有找到，则返回缺省的值 如果Key值第一个字符不为整数则返回0
	mode =GetPrivateProfileInt("set","mode",1,".\\set.ini"); 
	maxMB =GetPrivateProfileInt("set","max",500,".\\set.ini"); 

	if (mode >=2)
		{
			maxMB = 0x7FFFFFFF;
		} 

	m_set.LoadBitmaps(IDB_BITMAP1,IDB_BITMAP2);

	m_strfsize.Format("%d KB = %d MB",m_fsize,g_fileSizeMB);
	UpdateData(FALSE);


	CString title1, title ;
	GetWindowText(title1) ;		
	title.Format( "%s", title1) ;

	#ifdef _FUTURES_ONLY
	title = title + "-futures only";
	#endif

	SetWindowText( title ) ;
		

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

	CString cstrPathName,cstrFileName;
	CFileDialog fdlg(true, NULL, NULL, OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR, "(*.mds)|*.mds|All Files (*.*)|*.*||", NULL);
	int nRetDlgDoMo = fdlg.DoModal();
	if(nRetDlgDoMo==IDOK)
	{
	
		//for (int i=0;i<m_filelist.GetCount();i++) //wendy test
		{
			if (m_filelist.FindString(-1,fdlg.GetPathName())>=0) // wendy nStartAfter=0 LB_ERR
			{
				AfxMessageBox("Already add this file.");
				return;
			}
		}
		cstrPathName.Format("%s",fdlg.GetPathName());
		m_filelist.AddString(cstrPathName);

		cstrFileName.Format("%s",fdlg.GetFileName());

	}
	else if (nRetDlgDoMo == IDCANCEL)
	{
		cstrFileName.Format("No Select File");
	}

	s_Log.Format("OnAddfile FileName = %s",cstrFileName);

	SetBtnReadOrSearch(1);
}

void CMsEdit_WendyDlg::OnDelfile() 
{
	// TODO: Add your control notification handler code here
		
	int nItemSelected=m_filelist.GetCurSel();
	if(nItemSelected < 0)
	{
		MessageBox("Please choose file to delete.");
		return ;
	}

	CString DelFile;
	m_filelist.GetText(nItemSelected,DelFile);
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	
	_splitpath( DelFile, drive, dir, fname, ext );
	CString DelFileName = fname;
	DelFile = DelFileName + ext;

	s_Log.Format("OnDelfile DelFile = %s ",DelFile);

	m_filelist.DeleteString(nItemSelected);

	m_filelist.SetFocus();
	
	SetBtnReadOrSearch(2);
}

void CMsEdit_WendyDlg::OnRead() 
{
	// TODO: Add your control notification handler code here

	#if 1  // wendy test

	memset(&g_Master.m_itemdesk,0,sizeof(Master::OutItem));
	int m_count ;
	m_count = m_filelist.GetCount();
	int m_precount = pre_files.GetSize();

	m_needreadfs = 0;

	if (m_count==0)
	{
		AfxMessageBox("Please add files to list.");
		return;
	}
	UpdateData(TRUE);
	//if (!CheckInfo())
	//{
	//	AfxMessageBox("输入的项错误(Input item error).");
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

	if ( mode != pre_mode | maxMB != pre_maxMB)
	{
		m_needreadfs = 1;
	}

	if (g_fileSizeMB >=maxMB || mode <= 0)
	{
		m_needreadfs = 1;
		
		if (mode == 0)
		{
			w_InofLog.EmptyLog();
			DirectWrite_mIndexNo = 0;
		}
	}
	
	TRACE("m_needreadfs=%d. \n",m_needreadfs);

	if (m_needreadfs || !mCListDataFrame.GetCount())	//mCListFrame_e
	{
		//g_fileSizeKB = 0;
		if(g_Master.m_MSCEType != MSCE_TYPE_PROXY)
		{
			g_Master.m_unpakcer_TTFrame.ClearStream();  // g_Master.m_unpakcer_TTFrame.nBuffIp = 30;
			//g_Master.m_pDataSrc_TTFrameFile = new CDataSrc_TTFrameFile() ;
			g_Master.m_DataSrc_TTFrameFile.m_pUnpacker=&g_Master.m_unpakcer_TTFrame;
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
			g_Master.m_DataSrc_TTFrameFile.Init( files,g_Master.m_itemdesk.delay );
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

		if (mCListFrame_Simple.IsEmpty() == 0)
		{
			mCListFrame_Simple.RemoveAll();
		}

		ResetEvent(g_hEvent);

		pShowData =  new CShowData;

		if ( mode >= 1)
		{
			pShowData->Create(IDD_SHOWDATA);
			pShowData->ShowWindow(SW_SHOW);
		}

		g_pProDlg = new CProgressDlg();
		g_pProDlg->Create(CProgressDlg::IDD, NULL );
		g_pProDlg->ShowWindow( SW_SHOW );

		TRACE("Read Files.... \n");
		g_Master.Play();
		if (m_needreadfs == 1 && g_fileSizeMB < maxMB && mode >= 1)
		{
			GetDlgItem(IDC_READ)->SetWindowText("(Search)");
		}
	}
	else
	{
		SetEvent(g_hEvent);
		
		pShowData =  new CShowData;
		if ( mode >= 1)
		{
			pShowData->Create(IDD_SHOWDATA);
			pShowData->ShowWindow(SW_SHOW);
		}
	}

#endif // wendy test

}

BOOL CMsEdit_WendyDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CString str;
	CString errlog;	

	try
	{
		if (mCListDataFrame.IsEmpty() == 0)
		{
			mCListDataFrame.RemoveAll();
		}
		
		if (mCListFrame_e.IsEmpty() == 0)
		{
			mCListFrame_e.RemoveAll();
		}

		if (mCListFrame_Simple.IsEmpty() == 0)
		{
			mCListFrame_Simple.RemoveAll();
		}
			
		//if (g_Master.m_pDataSrc_TTFrameFile)
		//{
		//	delete g_Master.m_pDataSrc_TTFrameFile;
		//	g_Master.m_pDataSrc_TTFrameFile = NULL;
		//}

		KillTimer(1);
	}
	catch (CMemoryException* e)
	{
		e->GetErrorMessage((LPSTR)(LPCSTR)str,255);
		errlog.Format("MemoryException %d,%s",GetLastError(),str);
		//w_Log.Log(errlog);
	}
	catch (CFileException* e)
	{
		e->GetErrorMessage((LPSTR)(LPCSTR)str,255);
		errlog.Format("FileException %d,%s",GetLastError(),str);
		//w_Log.Log(errlog);
	}
	catch (CException* e)
	{
		e->GetErrorMessage((LPSTR)(LPCSTR)str,255);
		errlog.Format("Exception %d,%s",GetLastError(),str);
		//w_Log.Log(errlog);
	}
	
	//w_Log.Log("===========================End Application===========================");
	//w_Log.Close();
	w_InofLog.Close();
	return CDialog::DestroyWindow();
}

void CMsEdit_WendyDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (s_Log.GetLength())
	{
		//w_Log.Log(s_Log);
		char *token;
		CString str[10];
		char seps[] = "\n";
		int count = s_Log.GetLength();
		
		token = strtok(s_Log.GetBuffer(count), seps);
		
		str[0].Format("%s", token);
		//TRACE("%s", str[0]);
		m_loglist.AddString(str[0]);
		int i = 1;
		while(1)
		{
			token = strtok(NULL, seps);
			if(token == NULL)
				break;
			str[i].Format("%s", token);
			//TRACE("%s\n", str[i]);
			m_loglist.AddString(str[i]);
			i++;
		}
		s_Log="";

	}

	if (InitCreateGroupsMng == FALSE)
	{
		InitCreateGroupsMng = TRUE;
		g_Master.CreateGroupsMng();
	}

	MEMORYSTATUS stat;
	static DWORD m_whileCount = 0;
	m_whileCount ++;
	if(m_whileCount % 100*1000 ==0 )  // SetTimer(1, 10, NULL) ;  // 350
	{
		static DWORD percentVirMem=100,pre_percent = 0;

		 GlobalMemoryStatus (&stat);
		 percentVirMem = (stat.dwAvailVirtual/1024)*100 /(stat.dwTotalVirtual/1024);

		  if (pre_percent > stat.dwMemoryLoad && pre_percent >90 || percentVirMem < 10)
			{
			 TRACE("CMsEdit_WendyDlg...Because pre_percent(%d) > Now(%d), So Do Something..? \n", pre_percent,stat.dwMemoryLoad);
			}
		pre_percent = stat.dwMemoryLoad;
		
	}


	CDialog::OnTimer(nIDEvent);
}

int CMsEdit_WendyDlg::SetBtnReadOrSearch(int Flag)
{
	m_needreadfs = 0;
	int m_count = m_filelist.GetCount();
	int m_precount = pre_files.GetSize();

	if (GetFileSizeMB(m_filelist) >= maxMB || mode <= 0)
	{
		m_needreadfs = 1;
	}
	else
	{
		if (m_count != m_precount)
		{
			m_needreadfs = 1;
		}
		else if (m_precount > 0)
		{
			for (int i=0;i<pre_files.GetSize();i++)
			{
				if (m_filelist.FindString(-1,pre_files.GetAt(i))>=0)  // wendy nStartAfter=0 LB_ERR
				{
					TRACE("Already read this file(%d). \n",i+1);
				}
				else
				{
					m_needreadfs = 1;
				}
			}
		}
		else if (m_precount == 0)
		{
			m_needreadfs = 1;
		}
	}
	
	TRACE("SetBtnReadOrSearch m_needreadfs=%d. \n",m_needreadfs);



	if (m_needreadfs == 1)
	{
		GetDlgItem(IDC_READ)->SetWindowText("(Start Read)");
	}
	else
	{
		GetDlgItem(IDC_READ)->SetWindowText("(Search)");
	}

	return m_needreadfs;
}


DWORD CMsEdit_WendyDlg::GetFileSizeMB(CListBox &mClistBox)
{
	CStringArray files ;
	char szTemp[256];
	int i = 0;
	int m_count = mClistBox.GetCount();
	g_fileSizeKB = 1; // 0;

	for(i=0;i<m_count;i++ )
	{
		memset(szTemp,0,sizeof(szTemp));
		mClistBox.GetText(i,szTemp);
		if( strlen(szTemp)<=0 )
		{
			break ;
		}
		files.Add( szTemp ) ;
	}

	WIN32_FIND_DATA fileInfo;
	HANDLE hFind;
	
	for(i=0;i<files.GetSize();i++ )
	{
		DWORD fileSize = 0;
		hFind = FindFirstFile(files[i] ,&fileInfo);
		if(hFind != INVALID_HANDLE_VALUE)
		{
			fileSize = fileInfo.nFileSizeLow;
		}
		FindClose(hFind);
		g_fileSizeKB = g_fileSizeKB + fileSize/1024;		
	}

	m_fsize = g_fileSizeKB; //
	g_fileSizeMB = m_fsize /1024;

	m_strfsize.Format("%d KB = %d MB",m_fsize,g_fileSizeMB);

	UpdateData(FALSE);
	
	return g_fileSizeMB;
}

#include "Set.h"
void CMsEdit_WendyDlg::OnSet() 
{
	// TODO: Add your control notification handler code here
	CSet mSet;
	mSet.DoModal();
}
