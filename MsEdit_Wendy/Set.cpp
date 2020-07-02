// Set.cpp : implementation file
//

#include "stdafx.h"
#include "msedit_wendy.h"
#include "Set.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSet dialog


CSet::CSet(CWnd* pParent /*=NULL*/)
	: CDialog(CSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSet)
	//}}AFX_DATA_INIT
}


void CSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSet)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSet, CDialog)
	//{{AFX_MSG_MAP(CSet)
	ON_BN_CLICKED(IDC_OPEN_SETFILE, OnOpenSetfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSet message handlers

extern int mode;
extern DWORD maxMB;

void CSet::OnOK() 
{
	// TODO: Add extra validation here
		
	CString str;
	GetDlgItemText(IDC_EDIT_MODE, str);
	long mode_set = atol(str);
	WritePrivateProfileString("set","mode",str,".\\set.ini");
	mode = mode_set;

	GetDlgItemText(IDC_EDIT_MAX, str);
	long maxMB_set = atol(str);
	WritePrivateProfileString("set","max",str,".\\set.ini");
	maxMB = maxMB_set;	


	FILE *stream;
	char list[10240]={0};
	int  i, numread, numwritten;
	
	char wlist1[] = "\n\n\n\n\n\
; mode:\n\
;-1: Only display the number of frames counted after reading the file, for example: ReadGetFrmes:14317824. \n\
;You can get the result, only after reading the file, \n\
;No matter what you fill in, just press the button(Start Read) to start reading the file and count the frames.\n\
;This mode only displays the statistics frame number, does not cache, and does not occupy too much memory.\n\
\n\
;0:  Write the record directly to the file(InfoMds.log).\n\
;This mode finds records and writes them to files according to the search conditions you set.\n\
;After writing, the frame number will be counted as mode 0. Note that it is not the number of records.\n\
;If you write too many records, it will take more time. It does not cache and does not use too much memory,\n\
\n\
;1:  Default. Show the records in the list box of the dialog, and it's up to you to decide whether to write to the file or not.\n\
;This mode is associated with the maximum threshold(max) for file size.\n\
;If the file size is less than the max value, and all records will be cached at one time for subsequent search operations\n\
;Otherwise, each search operation will be cached again according to the set conditions.\n\
;If no conditions are set, then 100 records in front of the file are displayed, can will write all records are cached.\n\
;If no conditions are set and the file size >= max, In addition to displaying only 100 records, only can write 200000 records are cached.\n\
;Memory usage based on file size.\n\
\n\
;2:  In this mode, all operations are cached at one time\n\
;In fact, the max value of mode 1 is set to the maximum:0x7FFFFFFF(MB);\n\
\n\
;1-2: When current system memory usage arrive 90% or Availability rate of virtual address space less 10%,\n\
;The thread of reading file operation will stop Ranning.\n\
\n\
;(default mode = 1)";

char wlist2[] = "\n\n\
;max:\n\
;Calculation unit is MB, Value is valid when mode when mode >= 1, it is the maximum file size threshold.\n\
;If the file size < max, and all records will be cached at one time for subsequent search operations.\n\
;If the file size >= max, each search operation will be cached again according to the set conditions.\n\
;When mode = 2, in fact, the max value is 0x7FFFFFFF(MB), So no file size can be greater than or equal to it\n\
\n\
;(default max = 500 [MB])";
	
	/* Open file in text mode: */
	numread = 1;
	if( (stream = fopen( "set.ini", "r+t" )) != NULL )
	{
		//while( numread > 0 )
		{
			numread = fread( list, sizeof(char), 10240, stream );
			TRACE( "numread %d...1 \n", numread );
		
			if (!strstr(list,wlist1))
			{
				wlist1[strlen(wlist1)]='\n';
				
				numwritten = fwrite( wlist1, sizeof( char ), sizeof(wlist1), stream );
				TRACE( "numwritten %d...1 \n", numwritten );
			}

			if (!strstr(list,wlist2))
			{
				wlist2[strlen(wlist2)]='\n';
				numwritten = fwrite( wlist2, sizeof( char ), sizeof(wlist2), stream );
				TRACE( "numwritten %d...2 \n", numwritten );
			}
		}		
		fclose( stream );
		
	}
	else
	{
		TRACE( "Problem opening the file\n" );
	}
					
	CDialog::OnOK();
}


BOOL CSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// for read set.ini value
	int mode_set = 1;
	DWORD maxMB_set =500;
	// TODO: Add extra initialization here
	mode_set =GetPrivateProfileInt("set","mode",1,".\\set.ini"); 
	maxMB_set =GetPrivateProfileInt("set","max",500,".\\set.ini");
	CString str;
	str.Format("%d",mode_set);
	SetDlgItemText(IDC_EDIT_MODE,str);

	str.Format("%d",maxMB_set);
	SetDlgItemText(IDC_EDIT_MAX,str);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSet::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_CHAR // 字符消息
		&& ((pMsg->hwnd == GetDlgItem(IDC_EDIT_MODE)->m_hWnd) || (pMsg->hwnd == GetDlgItem(IDC_EDIT_MAX)->m_hWnd)))
	{
		if (pMsg->hwnd == GetDlgItem(IDC_EDIT_MODE)->m_hWnd)
		{
			CString str;
			int nPos = 0;
			GetDlgItemText(IDC_EDIT_MODE, str); // 获取edit中文本

			int nSource,nDestination;
			((CEdit *)GetDlgItem(IDC_EDIT_MODE))->GetSel(nSource,nDestination);
		
			
			//只允许输入 数字, Backspace,Delete,- 
			if((pMsg->wParam >='0' && pMsg->wParam <='9')||(pMsg->wParam ==0x08)||(pMsg->wParam =='-'))
			{
				
			}
			else
			{
				return TRUE;
			}

			//只允许输入一个-且放在开始位置.
			if(pMsg->wParam == '-')
			{				
				if(str.IsEmpty())
				{
				}
				else				{
					// 此时选择了全部的内容
					if(nSource==0&&nDestination==str.GetLength() || nSource==0&&nDestination==0&&str.Left(1).Compare("-"))
					{
						
					}
					else
					{
						return TRUE;
					}
				}  
				
			}
			
			// 不允许在-前输入数字
			nPos = str.Find('-'); // 查找-的位置
			if(nPos>=0)			
			{
				if(nSource>=1)
				{
				}
				else if (nDestination>=1)
				{
				}
				else
				{
					return TRUE;
				}
			}


		}
		
		if (pMsg->hwnd == GetDlgItem(IDC_EDIT_MAX)->m_hWnd)
		{
			//只允许输入 数字, Backspace
			if((pMsg->wParam >='0' && pMsg->wParam <='9')||(pMsg->wParam ==0x08))
			{
				
			}
			else
			{
				return TRUE;
			}
			
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CSet::OnOpenSetfile() 
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL,"open",".\\set.ini",NULL,NULL,SW_SHOWNORMAL); // 打开当前目录下的set.ini 文件
}
