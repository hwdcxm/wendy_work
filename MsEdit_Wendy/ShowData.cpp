// ShowData.cpp : implementation file
//

#include "stdafx.h"
#include "MsEdit_Wendy.h"
#include "ShowData.h"
#include "MsEdit_WendyDlg.h"

#include "log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CLog w_InofLog;

/////////////////////////////////////////////////////////////////////////////
// CShowData dialog

//#include "Datadef.h"
//typedef CArray<_tagTTDataFrame, _tagTTDataFrame&> CArrayItemDataFrame;
CArrayItemDataFrame mCArrayItemDataFrame;
CListDataFrame mCListDataFrame;
CListFrame_e mCListFrame_e;

extern CListFrame_Simple	  mCListFrame_Simple;  // wendy add 2020.06.04

CShowData::CShowData(CWnd* pParent /*=NULL*/)
: CDialog(CShowData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShowData)
	m_strEditCell = _T("");
	//}}AFX_DATA_INIT
	
	//DataCount_bak = 1;
	TotalRecord = 0;
	GetRecord = 0;
	RecordInterval = 1;
	
	//ListIndex_bak = 0;
	
	SearchRecord = 0;
	SearchRecord_bak = 0;
	
	//memset(&CStrnTransDataLength,0,sizeof(CString));
	//memset(&CStrpTransData,0,sizeof(CString));
	//memset(&CStrlTime,0,sizeof(CString));
	//memset(StatusBar_fileinfo,0,sizeof(char)*128);
	
	m_filesize = 0;
	
	InitClistGetHead = FALSE;
	pME_WD = (CMsEdit_WendyDlg*)AfxGetMainWnd();
	
}


void CShowData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShowData)
	DDX_Control(pDX, IDC_EDIT_CELL, m_ctrlEditCell);
	DDX_Control(pDX, IDC_LIST_DATA, m_list_data);
	DDX_Text(pDX, IDC_EDIT_CELL, m_strEditCell);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShowData, CDialog)
//{{AFX_MSG_MAP(CShowData)
ON_WM_TIMER()
ON_NOTIFY(NM_DBLCLK, IDC_LIST_DATA, OnDblclkListData)
ON_EN_KILLFOCUS(IDC_EDIT_CELL, OnKillfocusEditCell)
ON_NOTIFY(NM_RCLICK, IDC_LIST_DATA, OnRclickListData)
ON_COMMAND(ID_MENUITEM_INFO, OnMenuitemInfo)
ON_COMMAND(ID_MENUITEM_COPY, OnMenuitemCopy)
ON_BN_CLICKED(IDC_BTN_TIMER, OnBtnTimer)
ON_BN_CLICKED(IDC_WRITE_SEARCH, OnWriteSearch)
	ON_BN_CLICKED(IDC_OPEN_DATAFILE, OnOpenDatafile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowData message handlers

_tagTTDataFrame mTTDataFrame;
_Frame_e mFrame_e;

_tagTTDataFrame *pFramePara;
_tagTTDataFrame *pFramePara_wf;	
extern HANDLE g_hEvent;

int CShowData::op_item(_tagTTDataFrame *pStFrame)
{
	int strcount = pME_WD->m_item.GetLength();
	if (strcount == 0)
	{
		noselitem = TRUE;
		return 1;
	}
	else
	{
		noselitem = FALSE;
		for(int i = 0; i< 8-strcount; i++)
		{
			pME_WD->m_item = pME_WD->m_item + ' ';
		}
		pME_WD->m_item.MakeUpper();
		
		CString cstrItem = pStFrame->arItemCode;
		cstrItem.MakeUpper();
		//		int ret = -2;
		//		ret = cstrItem.Find(pME_WD->m_item,0); 
		
		//if (memcmp(&(pStFrame->arItemCode),pME_WD->m_item,sizeof(char)*8)==0)		
		if (memcmp(cstrItem,pME_WD->m_item,sizeof(char)*8)==0) //if (ret>=0)
			return 1;
		else
			return 0;
	}
}

int CShowData::op_TransCode(_tagTTDataFrame *pStFrame)
{
	if (pME_WD->m_TransCode.GetLength() == 0)
	{
		noselcode = TRUE;
		return 1;
	}
	else
	{
		noselcode = FALSE;
		if (memcmp(&(pStFrame->btTransCode),pME_WD->m_TransCode,sizeof(BYTE))==0)
			return 1;
		else
			return 0;
	}
}

int CShowData::op_time(_tagTTDataFrame *pStFrame)
{
	int strtime_count = pME_WD->m_time.GetLength();
	int strtime_end_count = pME_WD->m_time_end.GetLength();
	if (strtime_count == 0 && strtime_end_count == 0)
	{
		noseltime = TRUE;
		return 1;
	}
	else
	{
		noseltime = FALSE;
		long ltime = 0, ltime_end=0;
		if (strtime_count>0)
		{
			ltime = atol(pME_WD->m_time);
		}
		
		if (strtime_end_count>0)
		{
			ltime_end = atol(pME_WD->m_time_end);
		}
		
		if (strtime_count >0 && strtime_end_count> 0)
		{
			if (pStFrame->lTime >= ltime &&  pStFrame->lTime <= ltime_end)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else if ( strtime_count >0 )
		{
			if (pStFrame->lTime == ltime)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else if ( strtime_end_count >0 )
		{
			if (pStFrame->lTime == ltime_end)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
}

extern Master g_Master;

BOOL CShowData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_StatusBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW, CRect(0,0,0,0), this, 0);
	
	CRect rect;
	GetWindowRect(rect);
	int strPartDim[2]= {rect.Width()/5*4, -1}; //分割数量
	m_StatusBar.SetParts(2, strPartDim);
	
	//	int strPartDim[3]= {100, 200, -1}; //分割数量
	//	m_StatusBar.SetParts(3, strPartDim);
	
	//设置状态栏文本  
	//m_StatusBar.SetText(StatusBar_fileinfo, 0, 0);
	//m_StatusBar.SetText(" ", 1, 0);
	//m_StatusBar.SetText(" ", 2, 0); 
	//下面是在状态栏中加入图标                  
	//m_StatusBar.SetIcon(1, SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),FALSE));//为第二个分栏中加的图标
	
	m_list_data.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES |LVS_EX_FLATSB);
	
	m_list_data.InsertColumn(0,"NO", LVCFMT_LEFT, 80);
	m_list_data.InsertColumn(1,"Group", LVCFMT_LEFT, 60);
	//m_list_data.InsertColumn(2,"Length", LVCFMT_LEFT, 70); // nLength Key
	m_list_data.InsertColumn(3,"Time", LVCFMT_LEFT, 150); 
	m_list_data.InsertColumn(4,"ItemCode", LVCFMT_LEFT, 100);
	m_list_data.InsertColumn(5,"TransCode", LVCFMT_LEFT, 90);
	m_list_data.InsertColumn(6,"DataLength", LVCFMT_LEFT, 90);
	m_list_data.InsertColumn(7,"TransData", LVCFMT_LEFT, 120);
	
	//pME_WD = (CMsEdit_WendyDlg*)AfxGetMainWnd();
	
	if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))==0)
	{
		m_list_data.InsertColumn(2,"Key", LVCFMT_LEFT, 70); 
		TotalRecord = mCListFrame_e.GetCount();
	}
	else if(memcmp("O",pME_WD->m_TransCode,sizeof(BYTE))==0 || memcmp("L",pME_WD->m_TransCode,sizeof(BYTE))==0
		|| memcmp("H",pME_WD->m_TransCode,sizeof(BYTE))==0
		|| memcmp("G",pME_WD->m_TransCode,sizeof(BYTE))==0
		|| memcmp("C",pME_WD->m_TransCode,sizeof(BYTE))==0)  // wendy add 2020.06.04
	{
		m_list_data.InsertColumn(2,"Price", LVCFMT_LEFT, 100); 
		TotalRecord = mCListFrame_Simple.GetCount();			// wendy add 2020.06.04
	}
	else
	{
		m_list_data.InsertColumn(2,"Length", LVCFMT_LEFT, 70); 
		TotalRecord =mCListDataFrame.GetCount();
	}
	
#if 1  // wendy test
	SetTimer( 1, 1000, NULL ) ;
	
	//frame_e_count = mCListFrame_e.GetCount();
	//DataCount= mCListFrame_e.GetCount();  //mCArrayItemDataFrame.GetSize();  //m_list_data.GetItemCount(); //
	StatusBar_fileinfo.Format("GetRecord=%ld,SearchRecord=%ld,TotalRecord=%ld",GetRecord,SearchRecord,TotalRecord);
	m_StatusBar.SetText(StatusBar_fileinfo, 0, 0);
	
	WIN32_FIND_DATA fileInfo;
	HANDLE hFind;
	
	for( int i=0;i<g_Master.m_DataSrc_TTFrameFile.m_files.GetSize();i++ )
	{
		DWORD fileSize;
		hFind = FindFirstFile(g_Master.m_DataSrc_TTFrameFile.m_files[i] ,&fileInfo);
		if(hFind != INVALID_HANDLE_VALUE)
		{
			fileSize = fileInfo.nFileSizeLow;
		}
		FindClose(hFind);
		m_filesize = m_filesize + fileSize;		
	}
	
	StatusBar_filesize.Format("Files size=%dKB",m_filesize / 1024);
	m_StatusBar.SetText(StatusBar_filesize, 1, 0);
	
	enable_timer = false;
	GetDlgItem(IDC_BTN_TIMER)->EnableWindow(FALSE);
	GetDlgItem(IDC_WRITE_SEARCH)->EnableWindow(FALSE);
	
#endif // wendy test
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CShowData::OnDblclkListData(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	NM_LISTVIEW*   pNMListView   =   (NM_LISTVIEW*)pNMHDR;  
	
	if (pNMListView->iItem < 0  || pNMListView->iSubItem < 0)
	{
		return;
	}
	
	CRect CEret;
	
	if(m_list_data.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem, LVIR_LABEL, CEret))//取得指定单元格大小
	{
		m_ctrlEditCell.SetRect(&CEret);//设置编辑框大小
		CRect offset;
		m_list_data.GetWindowRect(&offset);   
		ScreenToClient(&offset);   
		m_ctrlEditCell.MoveWindow(offset.TopLeft().x + CEret.TopLeft().x + 2, offset.TopLeft().y + CEret.TopLeft().y + 2, CEret.Width(), CEret.Height());//覆盖编辑框到指定单元格上
		char strCellText[100];
		strCellText[0] = NULL;
		m_list_data.GetItemText(pNMListView->iItem,pNMListView->iSubItem,strCellText,100);
		m_ctrlEditCell.SetWindowText(_T(strCellText));//取得指定单元格里的数据，显示在编辑框里
		m_ctrlEditCell.ShowWindow(TRUE);
		m_ctrlEditCell.SetFocus();//设置焦点
	}
	m_indexOfCell.x = pNMListView->iItem;
	m_indexOfCell.y = pNMListView->iSubItem;//保存指定单元格的索引
	
	*pResult = 0;
}

void CShowData::OnKillfocusEditCell() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_list_data.SetItemText(m_indexOfCell.x, m_indexOfCell.y, _T(m_strEditCell));//失去焦点的时候将原来选中的单元格设置为编辑框的值
	m_ctrlEditCell.ShowWindow(FALSE);
	UpdateData(FALSE);
	
}

void CShowData::OnRclickListData(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	
	NM_LISTVIEW*   pNMListView   =   (NM_LISTVIEW*)pNMHDR;  
	
	if (pNMListView->iItem < 0  || pNMListView->iSubItem < 0)
	{
		return;
	}
	
	m_indexOfCell.x = pNMListView->iItem;
	m_indexOfCell.y = pNMListView->iSubItem;//保存指定单元格的索引
	
	CRect CEret;
	CRect offset;
	
	if(m_list_data.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem, LVIR_LABEL, CEret))//取得指定单元格大小
	{
		m_list_data.GetWindowRect(&offset);   
		ScreenToClient(&offset);   
	}
	CPoint point;
	point.x = offset.TopLeft().x + CEret.TopLeft().x + 2;
	point.y = offset.TopLeft().y + CEret.TopLeft().y + 2;
	
	ClientToScreen(&point);
	
	CMenu menu;//菜单
	CMenu* pPopupMenu;//菜单指针
	menu.LoadMenu(IDR_MENU1);//加载ID为IDR_MENU1的菜单
	pPopupMenu = menu.GetSubMenu(0);   //获取弹出菜单的第一层子菜单的类指针
	
	//char strCellTxCode[10];
	//m_list_data.GetItemText(m_indexOfCell.x,5,strCellTxCode,10);  // because of m_list_data.InsertColumn(5,"TransCode", LVCFMT_LEFT, 90); so fill 5
	
	if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))==0) // if (strcmp(strCellTxCode,"e") == 0)
	{
		pPopupMenu->EnableMenuItem(ID_MENUITEM_INFO,MF_ENABLED); //MF_DISABLED MF_ENABLED
	}
	else
	{
		pPopupMenu->EnableMenuItem(ID_MENUITEM_INFO,MF_DISABLED); //MF_DISABLED MF_ENABLED
	}
	
	pPopupMenu->TrackPopupMenu(TPM_BOTTOMALIGN,point.x,point.y,this,NULL); //point.x,point.y TPM_BOTTOMALIGN  TPM_LEFTALIGN
	
	*pResult = 0;
}



void CShowData::OnMenuitemInfo() 
{
	// TODO: Add your command handler code here
	
	pEINFO =  new CEINFO;
	
	m_list_data.GetItemText(m_indexOfCell.x,2,pEINFO->strKey,100);
	m_list_data.GetItemText(m_indexOfCell.x,3,pEINFO->strTime,100);
	m_list_data.GetItemText(m_indexOfCell.x,4,pEINFO->strItemCode,100);
	
	pEINFO->Create(IDD_E_INFO);
	pEINFO->ShowWindow(SW_SHOW);
	
}

void CShowData::OnMenuitemCopy() 
{
	// TODO: Add your command handler code here
	char strCellText[100];
	strCellText[0] = NULL;
	m_list_data.GetItemText(m_indexOfCell.x,m_indexOfCell.y,strCellText,100);
	m_ctrlEditCell.SetWindowText(_T(strCellText));//取得指定单元格里的数据，显示在编辑框里
	m_ctrlEditCell.ShowWindow(FALSE);
	m_ctrlEditCell.SetSel(0,-1);
	m_ctrlEditCell.Copy();
}

void CShowData::OnBtnTimer() 
{
	// TODO: Add your control notification handler code here
	static BOOL entimer = true;
	if (entimer)
	{
		KillTimer(1);
		GetDlgItem(IDC_BTN_TIMER)->SetWindowText("Resume");
		GetDlgItem(IDC_WRITE_SEARCH)->EnableWindow(TRUE);
	}
	else
	{
		SetTimer( 1, 1000, NULL ) ;
		GetDlgItem(IDC_BTN_TIMER)->SetWindowText("Pause");
		GetDlgItem(IDC_WRITE_SEARCH)->EnableWindow(FALSE);
	}
	entimer = !entimer;
	
}

void CShowData::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();  //  availd for domodal dialog
	DestroyWindow();
}

void CShowData::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::PostNcDestroy();
	if (AfxGetMainWnd()->IsWindowEnabled())
	{
		delete this;
	}
}

void CShowData::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	int i = 0;
	//POSITION ps;
	
	//TRACE("OnTimer...\n");
	KillTimer(1);
	//TRACE("SetTimer...\n");
	
#if 1  // wendy test
	DWORD retEvent = WaitForSingleObject(g_hEvent, 0); //INFINITE
	if (retEvent == WAIT_OBJECT_0)
	{
		OnTimer_Frame_To_List();
		
		//		if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))==0)
		//			{
		//				ListSTLFrame_eToListCtrl(mCListFrame_e,m_list_data);
		//			}
		//		else if (memcmp("O",pME_WD->m_TransCode,sizeof(BYTE))==0)
		//			{
		//				ListSTLFrame_OToListCtrl(mCListFrame_O,m_list_data);
		//			}
		//		else			
		//			{
		//				ListSTLDataFrameToListCtr(mCListDataFrame,m_list_data);
		//			}
		SetEvent(g_hEvent);
	}
	else if (retEvent ==WAIT_TIMEOUT)
	{
		SetTimer( 1, 130, NULL ) ;
	}
#endif	
	
	//SetTimer( 1, 30, NULL ) ;	// wendy test
	CDialog::OnTimer(nIDEvent);
	
}

void CShowData::OnWriteSearch() 	// wendy add 2020.06.04
{
	// TODO: Add your control notification handler code here
	
	m_lock.Lock();
	
	CString cstr_write;
	GetDlgItem(IDC_WRITE_SEARCH)->EnableWindow(FALSE);
	w_InofLog.EmptyLog();
	
	
	POSITION ps;
	DWORD mIndexNo = 0;
	
	
	if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))==0)
		ps =  mCListFrame_e.GetHeadPosition();
	else if (memcmp("O",pME_WD->m_TransCode,sizeof(BYTE))==0 || memcmp("L",pME_WD->m_TransCode,sizeof(BYTE))==0
		|| memcmp("H",pME_WD->m_TransCode,sizeof(BYTE))==0
		|| memcmp("G",pME_WD->m_TransCode,sizeof(BYTE))==0
		|| memcmp("C",pME_WD->m_TransCode,sizeof(BYTE))==0)
		ps = mCListFrame_Simple.GetHeadPosition();		// wendy add 2020.06.04
	else
		ps = mCListDataFrame.GetHeadPosition();
	
	while(ps )	// for( ;(ps); ) 
	{
		if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))==0)
			pFramePara_wf = (_tagTTDataFrame *) &mCListFrame_e.GetAt(ps);
		else if (memcmp("O",pME_WD->m_TransCode,sizeof(BYTE))==0 || memcmp("L",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("H",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("G",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("C",pME_WD->m_TransCode,sizeof(BYTE))==0)
			pFramePara_wf = (_tagTTDataFrame *) &mCListFrame_Simple.GetAt(ps);	// wendy add 2020.06.04
		else
			pFramePara_wf = (_tagTTDataFrame *) &mCListDataFrame.GetAt(ps);
		
		
		if (op_TransCode(pFramePara_wf)&&op_item(pFramePara_wf)&&op_time(pFramePara_wf)) 
		{
			
			mIndexNo ++;
			CString CstrIndexNo;
			CstrIndexNo.Format("No:%ld,",mIndexNo);
			
			CString CstrGroup;
			CstrGroup.Format("Group:%c,",pFramePara_wf->btGroupCode);
			
			CString CStrItemCode;
			char CStrItem[10];
			memset(CStrItem,0,sizeof(char)*10);
			memcpy(CStrItem,(char *)&(pFramePara_wf->arItemCode[0]),sizeof(char)*8);
			CStrItemCode.Format("ItemCode:%s,",CStrItem);			
			
			CString CstrTrans;
			CstrTrans.Format("Trans:%c,",pFramePara_wf->btTransCode);
			
			
			CString CstrTradeTime;
			CString CstrKey;
			CString CstrPrice;
			
			CString CstrLength;
			CString CstrTime;
			CString CstrDataLength;
			CString CstrTransData;
			
			CString CstrTradeTime2;
			//CString CstrPrice;
			CString CstrQuantity;
			CString CstrTradeType;
			
			if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))==0)
			{
				_Frame_e * p_Frame_e = NULL;
				try
				{				
					p_Frame_e = dynamic_cast < _Frame_e *> ( pFramePara_wf);
				}
				catch (...) 
				{
					AfxMessageBox("catch Base class pointer convert failed !");
					return ;
				}
				if (p_Frame_e == NULL) 
				{
					AfxMessageBox("Base class pointer convert failed !");
					return ;
				}
				
				WORD	Time = p_Frame_e->TradeTime; //((_Frame_e*)pFramePara_wf) // TradeTime2
				tm		stTm;
				if ((stTm.tm_hour = Time / 1800) >= 24)
				{	
					stTm.tm_hour-=24;
				}
				else
				{
					stTm.tm_isdst=0;
				}
				Time %= 1800;
				stTm.tm_min = Time / 30;
				stTm.tm_sec = (Time % 30) * 2;	
				
				//((_Frame_e*)pFramePara_wf)
				CstrTradeTime.Format("Time[%ld] %02d:%02d:%02d,  ",p_Frame_e->TradeTime, stTm.tm_hour, stTm.tm_min,stTm.tm_sec);  //TradeTime2
				
				//				//((_Frame_e*)pFramePara_wf)
				//				CstrKey.Format("Key:%d,",p_Frame_e->Key);
				
				//				cstr_write = CstrIndexNo + CstrGroup + CStrItemCode  + CstrTrans +  CstrKey + CstrTradeTime; 
				
				//CString CstrTradeTime;
				//				CstrTradeTime.Format("Time:%d,",p_Frame_e->TradeTime);
				
				CstrTradeTime2.Format("Time2:%d,",p_Frame_e->TradeTime2);		
				
				//CString CstrKey;
				CstrKey.Format("Key:%d,",p_Frame_e->Key);
				
				
				//CString CstrPrice;
				CstrPrice.Format("Price:%f,",p_Frame_e->Price);
				
				
				//CString CstrQuantity;
				CstrQuantity.Format("Quantity:%d,",p_Frame_e->Quantity);
				
				
				//CString CstrTradeType;
				CstrTradeType.Format("TradeType:%d",p_Frame_e->TyadeType);
				
				cstr_write = CstrIndexNo + CstrGroup + CStrItemCode + CstrTrans  + CstrTradeTime + CstrTradeTime2 +CstrKey +CstrPrice + CstrQuantity + CstrTradeType;
				//w_InofLog.Log(s_eLog);
				
				
			}
			else if(memcmp("O",pME_WD->m_TransCode,sizeof(BYTE))==0 || memcmp("L",pME_WD->m_TransCode,sizeof(BYTE))==0
				|| memcmp("H",pME_WD->m_TransCode,sizeof(BYTE))==0
				|| memcmp("G",pME_WD->m_TransCode,sizeof(BYTE))==0
				|| memcmp("C",pME_WD->m_TransCode,sizeof(BYTE))==0)   // wendy add 2020.06.04
			{
				_Frame_Simple * p_Frame_Simple = NULL;
				try
				{
					p_Frame_Simple = dynamic_cast < _Frame_Simple *> ( pFramePara_wf);
				}				
				catch (...) 
				{
					AfxMessageBox("catch Base class pointer convert failed !");
					return ;
				}
				if (p_Frame_Simple == NULL)
				{
					AfxMessageBox("Base class pointer convert failed !");
					return ;
				}
				
				WORD	Time = p_Frame_Simple->Time; //((_Frame_Simple*)pFramePara_wf)
				tm		stTm;
				if ((stTm.tm_hour = Time / 1800) >= 24)
				{	
					stTm.tm_hour-=24;
				}
				else
				{
					stTm.tm_isdst=0;
				}
				Time %= 1800;
				stTm.tm_min = Time / 30;
				stTm.tm_sec = (Time % 30) * 2;
				
				//((_Frame_Simple*)pFramePara_wf)
				CstrTradeTime.Format("Time[%d] %02d:%02d:%02d,  ", p_Frame_Simple->Time, stTm.tm_hour, stTm.tm_min,stTm.tm_sec);
				
				//((_Frame_Simple*)pFramePara_wf)
				CstrPrice.Format("Price:%f,", p_Frame_Simple->Price);
				
				cstr_write = CstrIndexNo + CstrGroup + CStrItemCode  + CstrTrans +  CstrTradeTime  +CstrPrice; 
				
			}
			else
			{
				
				CstrLength.Format("Len:%d,",pFramePara_wf->nLength);
				
				CstrTime.Format("Time:%ld,",pFramePara_wf->lTime);		
				
				CstrDataLength.Format("Dath:%d,",pFramePara_wf->nTransDataLength);
				
				CstrTransData.Format("TrDa:0x%ld,",pFramePara_wf->pTransData);
				
				cstr_write = CstrIndexNo + CstrGroup + CStrItemCode  + CstrTrans +  CstrTime  +CstrDataLength + CstrTransData;
				
			}
			
			w_InofLog.Log(cstr_write);
			
		}	
		
		if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))==0)
			mCListFrame_e.GetNext(ps);
		else if (memcmp("O",pME_WD->m_TransCode,sizeof(BYTE))==0 || memcmp("L",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("H",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("G",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("C",pME_WD->m_TransCode,sizeof(BYTE))==0)
			mCListFrame_Simple.GetNext(ps);	// wendy add 2020.06.04
		else
			mCListDataFrame.GetNext(ps);
		
	}
	
	GetDlgItem(IDC_WRITE_SEARCH)->EnableWindow(TRUE);		
	m_lock.UnLock();
	
}


int CShowData::OnTimer_Frame_To_List(void)	// wendy add 2020.06.04
{
#if 1
	
	m_lock.Lock();
	if (InitClistGetHead == FALSE)
	{
		if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))==0)
			ps =  mCListFrame_e.GetHeadPosition();
		else if (memcmp("O",pME_WD->m_TransCode,sizeof(BYTE))==0 || memcmp("L",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("H",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("G",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("C",pME_WD->m_TransCode,sizeof(BYTE))==0)
			ps = mCListFrame_Simple.GetHeadPosition();		// wendy add 2020.06.04
		else
			ps = mCListDataFrame.GetHeadPosition();
		InitClistGetHead = TRUE;
	}
	
	if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))==0)
		TotalRecord = mCListFrame_e.GetCount();
	else if (memcmp("O",pME_WD->m_TransCode,sizeof(BYTE))==0 || memcmp("L",pME_WD->m_TransCode,sizeof(BYTE))==0
		|| memcmp("H",pME_WD->m_TransCode,sizeof(BYTE))==0
		|| memcmp("G",pME_WD->m_TransCode,sizeof(BYTE))==0
		|| memcmp("C",pME_WD->m_TransCode,sizeof(BYTE))==0)
		TotalRecord = mCListFrame_Simple.GetCount();	// wendy add 2020.06.04
	else
		TotalRecord = mCListDataFrame.GetCount();
	
	GetRecord = m_list_data.GetItemCount();
	
	RecordInterval = TotalRecord / 10000;
	if (RecordInterval <= 0) 
	{
		RecordInterval = 100;
	}
	
	for(int i=0; (ps && i<1000) ; ) 
	{
		if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))==0)
			pFramePara = (_tagTTDataFrame *) &mCListFrame_e.GetAt(ps);
		else if (memcmp("O",pME_WD->m_TransCode,sizeof(BYTE))==0 || memcmp("L",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("H",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("G",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("C",pME_WD->m_TransCode,sizeof(BYTE))==0)
			pFramePara = (_tagTTDataFrame *) &mCListFrame_Simple.GetAt(ps);	// wendy add 2020.06.04
		else
			pFramePara = (_tagTTDataFrame *) &mCListDataFrame.GetAt(ps);
		
		if (op_TransCode(pFramePara)&&op_item(pFramePara)&&op_time(pFramePara))
		{
			OnTimer_Frame_To_ListPara(pFramePara,m_list_data);	// wendy add 2020.06.04
			i++;
		}
		
		if (noselitem && noselcode && noseltime)
		{
			
			
			time_t t;
			srand((unsigned) time(&t));		
			SearchRecord = SearchRecord + rand()%100;
			if (i>=100)
			{
				SearchRecord = TotalRecord;
			}
			
			if (SearchRecord >= TotalRecord)
			{
				SearchRecord = TotalRecord-1;
			}
			if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))==0)
				ps = mCListFrame_e.FindIndex(SearchRecord);
			else if (memcmp("O",pME_WD->m_TransCode,sizeof(BYTE))==0 || memcmp("L",pME_WD->m_TransCode,sizeof(BYTE))==0
				|| memcmp("H",pME_WD->m_TransCode,sizeof(BYTE))==0
				|| memcmp("G",pME_WD->m_TransCode,sizeof(BYTE))==0
				|| memcmp("C",pME_WD->m_TransCode,sizeof(BYTE))==0)
				ps = mCListFrame_Simple.FindIndex(SearchRecord);	// wendy add 2020.06.04
			else
				ps = mCListDataFrame.FindIndex(SearchRecord);
			
			
		}
		
		SearchRecord ++;
		if (SearchRecord >= TotalRecord)
		{
			SearchRecord = TotalRecord;
		}
		
		if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))==0)
			mCListFrame_e.GetNext(ps);
		else if (memcmp("O",pME_WD->m_TransCode,sizeof(BYTE))==0 || memcmp("L",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("H",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("G",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("C",pME_WD->m_TransCode,sizeof(BYTE))==0)
			mCListFrame_Simple.GetNext(ps);	// wendy add 2020.06.04
		else
			mCListDataFrame.GetNext(ps);
		
		
	}
	
	if (SearchRecord < TotalRecord)
	{
		if (SearchRecord_bak == SearchRecord) // Terminate Loop that at sometime can not stop
		{
			TRACE("SetTimer But SearchRecord_bak == SearchRecord...\n");
			SearchRecord ++;
			if (SearchRecord >= TotalRecord)
			{
				SearchRecord = TotalRecord;
			}
		}
		
		SearchRecord_bak = SearchRecord;
		static int search = 0;
		//TRACE("SetTimer GetRecord=%d,SearchRecord=%d,TotalRecord=%ld,RecordInterval=%ld\n",GetRecord,SearchRecord,TotalRecord,RecordInterval);
		strsearch = "Search";
		for(int j =0; j < search; j++)
		{
			strsearch = strsearch + '.';
		}
		
		for(int k =0; k < 6-search; k++)
		{
			strsearch = strsearch + ' ';
		}
		
		search ++;
		
		if (search > 6)
		{
			search = 0;
		}
		
		StatusBar_fileinfo.Format("GetRecord=%d,SearchRecord=%d,TotalRecord=%ld   ",GetRecord,SearchRecord,TotalRecord);
		m_StatusBar.SetText(StatusBar_fileinfo+strsearch, 0, 0);
		
		if (enable_timer ==  false)
		{
			GetDlgItem(IDC_BTN_TIMER)->EnableWindow(TRUE);
			enable_timer = true;
		}
		SetTimer( 1, 30, NULL ) ;
	}
	else
	{
		TRACE("No SetTimer GetRecord=%d,SearchRecord=%d,TotalRecord=%ld,RecordInterval=%ld\n",GetRecord,SearchRecord,TotalRecord,RecordInterval);
		StatusBar_fileinfo.Format("GetRecord=%d,SearchRecord=%d,TotalRecord=%ld",GetRecord,SearchRecord,TotalRecord);
		m_StatusBar.SetText(StatusBar_fileinfo, 0, 0);
		
		enable_timer = false;
		GetDlgItem(IDC_BTN_TIMER)->EnableWindow(FALSE);	
		if(0) // test (noselitem && noselcode && noseltime)
		{
			GetDlgItem(IDC_WRITE_SEARCH)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_WRITE_SEARCH)->EnableWindow(TRUE);
		}
	}
	
	m_lock.UnLock();
	
#endif 	
	return 0;
	
}

int CShowData::OnTimer_Frame_To_ListPara(_tagTTDataFrame *pFrame, CListCtrl &mp_list_data)  // wendy add 2020.06.04
{
	static char string[25];
	static char GroupCodeString[3];
	static char CStrnLength[6];
	static CString CStrlTime;
	static char CStrItemCode[10];
	static char CStrTransCode[3];
	static CString CStrnTransDataLength;
	static CString CStrpTransData;
	static char CStrnKey[6];
	static char CStrnPrice[18];
	
	
	memset(string,0,sizeof(char)*25);
	memset(GroupCodeString,0,sizeof(char)*3);
	memset(CStrnLength,0,sizeof(char)*6);
	memset(CStrnKey,0,sizeof(char)*6);
	memset(CStrItemCode,0,sizeof(char)*10);
	memset(CStrTransCode,0,sizeof(char)*3);
	memset(CStrnPrice,0,sizeof(char)*18);
	
	
	itoa(GetRecord+1,string,10);
	mp_list_data.InsertItem(GetRecord,string,0);
	
	memcpy(GroupCodeString,&pFrame->btGroupCode,1);
	mp_list_data.SetItemText(GetRecord,1, GroupCodeString);
	
	if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))==0)
	{
		_Frame_e * p_Frame_e = NULL;
		try
		{		
			p_Frame_e = dynamic_cast < _Frame_e *> ( pFrame);
		}
		catch (...) 
		{
			AfxMessageBox("catch Base class pointer convert failed !");
			return -1 ;
		}
		if (p_Frame_e == NULL) 
		{
			AfxMessageBox("Base class pointer convert failed !");
			return -1;
		}
		
		_itoa(p_Frame_e->Key, CStrnKey, 10); // ((_Frame_e*)pFruame) sizeof(int)  4 sizeof(int)*4
		mp_list_data.SetItemText(GetRecord,2, CStrnKey);
		
		
		WORD	Time = p_Frame_e->TradeTime; // ((_Frame_e*)pFrame)  // TradeTime2
		tm		stTm;
		if ((stTm.tm_hour = Time / 1800) >= 24)
		{	
			stTm.tm_hour-=24;
		}
		else
		{
			stTm.tm_isdst=0;
		}
		Time %= 1800;
		stTm.tm_min = Time / 30;
		stTm.tm_sec = (Time % 30) * 2;
		
		//((_Frame_e*)pFrame)
		CStrlTime.Format("[%ld]  %02d:%02d:%02d",p_Frame_e->TradeTime, stTm.tm_hour, stTm.tm_min,stTm.tm_sec);  //TradeTime2
		mp_list_data.SetItemText(GetRecord,3, CStrlTime);
	}
	// wendy add 2020.06.04
	else if(memcmp("O",pME_WD->m_TransCode,sizeof(BYTE))==0 || memcmp("L",pME_WD->m_TransCode,sizeof(BYTE))==0
		|| memcmp("H",pME_WD->m_TransCode,sizeof(BYTE))==0
		|| memcmp("G",pME_WD->m_TransCode,sizeof(BYTE))==0
		|| memcmp("C",pME_WD->m_TransCode,sizeof(BYTE))==0)   // wendy add 2020.06.04
	{
		
		_Frame_Simple * p_Frame_Simple = NULL;
		try
		{		
			p_Frame_Simple = dynamic_cast < _Frame_Simple *> ( pFrame);
		}
		catch (...) 
		{
			AfxMessageBox("catch Base class pointer convert failed !");
			return -1 ;
		}
		if (p_Frame_Simple == NULL) 
		{
			AfxMessageBox("Base class pointer convert failed !");
			return -1;
		}
		
		sprintf(CStrnPrice, "%.4f ", p_Frame_Simple->Price); //((_Frame_Simple*)pFrame)
		mp_list_data.SetItemText(GetRecord,2, CStrnPrice);
		
		WORD	Time =p_Frame_Simple->Time; //((_Frame_Simple*)pFrame)
		tm		stTm;
		if ((stTm.tm_hour = Time / 1800) >= 24)
		{	
			stTm.tm_hour-=24;
		}
		else
		{
			stTm.tm_isdst=0;
		}
		Time %= 1800;
		stTm.tm_min = Time / 30;
		stTm.tm_sec = (Time % 30) * 2;
		
		//((_Frame_Simple*)pFrame)
		CStrlTime.Format("[%ld]  %02d:%02d:%02d",p_Frame_Simple->Time,stTm.tm_hour, stTm.tm_min,stTm.tm_sec); 
		mp_list_data.SetItemText(GetRecord,3, CStrlTime);
	}
	else
	{
		_itoa(pFrame->nLength, CStrnLength,10); // sizeof(int)  4
		mp_list_data.SetItemText(GetRecord,2, CStrnLength);
		
		CStrlTime.Format("%ld",pFrame->lTime); // wendy lTime
		mp_list_data.SetItemText(GetRecord,3, CStrlTime);
	}
	
	
	memcpy(CStrItemCode,(char *)&(pFrame->arItemCode[0]),sizeof(char)*8);
	mp_list_data.SetItemText(GetRecord,4, CStrItemCode);
	
	memcpy(CStrTransCode,&(pFrame->btTransCode),1);
	mp_list_data.SetItemText(GetRecord,5, CStrTransCode);
	
	CStrnTransDataLength.Format("%d",pFrame->nTransDataLength);
	mp_list_data.SetItemText(GetRecord,6, CStrnTransDataLength);
	
	CStrpTransData.Format("0x%ld",(long)pFrame->pTransData);
	mp_list_data.SetItemText(GetRecord,7, CStrpTransData);
	GetRecord++;
	
	return 0;
}

DWORD DirectWrite_mIndexNo = 0;

int CShowData::DirectWrite(_tagTTDataFrame * pFrame )
{
	m_lock.Lock();
	
	CString cstr_write;
	//w_InofLog.EmptyLog();
	
	
	POSITION ps;
	
	if (op_TransCode(pFrame)&&op_item(pFrame)&&op_time(pFrame)) 
	{
		DirectWrite_mIndexNo ++;
		CString CstrIndexNo;
		CstrIndexNo.Format("No:%ld,",DirectWrite_mIndexNo);
		
		CString CstrGroup;
		CstrGroup.Format("Group:%c,",pFrame->btGroupCode);
		
		CString CStrItemCode;
		char CStrItem[10];
		memset(CStrItem,0,sizeof(char)*10);
		memcpy(CStrItem,(char *)&(pFrame->arItemCode[0]),sizeof(char)*8);
		CStrItemCode.Format("ItemCode:%s,",CStrItem);			
		
		CString CstrTrans;
		CstrTrans.Format("Trans:%c,",pFrame->btTransCode);
		
		
		CString CstrTradeTime;
		CString CstrKey;
		CString CstrPrice; 
		
		CString CstrLength;
		CString CstrTime;
		CString CstrDataLength;
		CString CstrTransData;
		
		CString CstrTradeTime2;
		//CString CstrPrice;
		CString CstrQuantity;
		CString CstrTradeType;
		
		if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))==0)
		{
			_Frame_e * p_Frame_e = NULL;
			try
			{
				p_Frame_e = dynamic_cast < _Frame_e *> ( pFrame);
			}
			catch (...) 
			{
				AfxMessageBox("catch Base class pointer convert failed !");
				return -1 ;
			}
			if (p_Frame_e == NULL) 
			{
				AfxMessageBox("Base class pointer convert failed !");
				return -1;
			}
			
			WORD	Time = p_Frame_e->TradeTime; //((_Frame_e*)pFramePara_wf) // TradeTime2
			tm		stTm;
			if ((stTm.tm_hour = Time / 1800) >= 24)
			{	
				stTm.tm_hour-=24;
			}
			else
			{
				stTm.tm_isdst=0;
			}
			Time %= 1800;
			stTm.tm_min = Time / 30;
			stTm.tm_sec = (Time % 30) * 2;	
			
			//((_Frame_e*)pFramePara_wf)
			CstrTradeTime.Format("Time[%ld] %02d:%02d:%02d,  ",p_Frame_e->TradeTime, stTm.tm_hour, stTm.tm_min,stTm.tm_sec);  //TradeTime2
			
			//				//((_Frame_e*)pFramePara_wf)
			//				CstrKey.Format("Key:%d,",p_Frame_e->Key);
			
			//				cstr_write = CstrIndexNo + CstrGroup + CStrItemCode  + CstrTrans +  CstrKey + CstrTradeTime; 
			
			//CString CstrTradeTime;
			//				CstrTradeTime.Format("Time:%d,",p_Frame_e->TradeTime);
			
			CstrTradeTime2.Format("Time2:%d,",p_Frame_e->TradeTime2);		
			
			//CString CstrKey;
			CstrKey.Format("Key:%d,",p_Frame_e->Key);
			
			
			//CString CstrPrice;
			CstrPrice.Format("Price:%f,",p_Frame_e->Price);
			
			
			//CString CstrQuantity;
			CstrQuantity.Format("Quantity:%d,",p_Frame_e->Quantity);
			
			
			//CString CstrTradeType;
			CstrTradeType.Format("TradeType:%d",p_Frame_e->TyadeType);
			
			cstr_write = CstrIndexNo + CstrGroup + CStrItemCode + CstrTrans  + CstrTradeTime + CstrTradeTime2 +CstrKey +CstrPrice + CstrQuantity + CstrTradeType;
			//w_InofLog.Log(s_eLog);
			
			
		}
		else if(memcmp("O",pME_WD->m_TransCode,sizeof(BYTE))==0 || memcmp("L",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("H",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("G",pME_WD->m_TransCode,sizeof(BYTE))==0
			|| memcmp("C",pME_WD->m_TransCode,sizeof(BYTE))==0)   // wendy add 2020.06.04
		{
			_Frame_Simple * p_Frame_Simple = NULL;
			try
			{		
				p_Frame_Simple = dynamic_cast < _Frame_Simple *> ( pFrame);				
			}
			catch (...) 
			{
				AfxMessageBox("catch Base class pointer convert failed !");
				return -1 ;
			}
			if (p_Frame_Simple == NULL) 
			{
				AfxMessageBox("Base class pointer convert failed !");
				return -1 ;
			}
			
			WORD	Time = p_Frame_Simple->Time; //((_Frame_Simple*)pFramePara_wf)
			tm		stTm;
			if ((stTm.tm_hour = Time / 1800) >= 24)
			{	
				stTm.tm_hour-=24;
			}
			else
			{
				stTm.tm_isdst=0;
			}
			Time %= 1800;
			stTm.tm_min = Time / 30;
			stTm.tm_sec = (Time % 30) * 2;
			
			//((_Frame_Simple*)pFramePara_wf)
			CstrTradeTime.Format("Time[%d] %02d:%02d:%02d,  ", p_Frame_Simple->Time, stTm.tm_hour, stTm.tm_min,stTm.tm_sec);
			
			//((_Frame_Simple*)pFramePara_wf)
			CstrPrice.Format("Price:%f,", p_Frame_Simple->Price);
			
			cstr_write = CstrIndexNo + CstrGroup + CStrItemCode  + CstrTrans +  CstrTradeTime  +CstrPrice; 
			
		}
		else
		{
			
			CstrLength.Format("Len:%d,",pFrame->nLength);
			
			CstrTime.Format("Time:%ld,",pFrame->lTime);		
			
			CstrDataLength.Format("Dath:%d,",pFrame->nTransDataLength);
			
			CstrTransData.Format("TrDa:0x%ld,",pFrame->pTransData);
			
			cstr_write = CstrIndexNo + CstrGroup + CStrItemCode  + CstrTrans +  CstrTime  +CstrDataLength + CstrTransData;
			
		}
		
		w_InofLog.Log(cstr_write);
		
		}	
		
		m_lock.UnLock();
		
		return 0;
}

void CShowData::OnOpenDatafile() 
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL,"open",".\\InfoMds.log",NULL,NULL,SW_SHOWNORMAL); // 打开当前目录下的set.ini 文件
}
