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

#define FUNC_DECL(factory_name) DECL_##factory_name

extern CLog w_InofLog;

/////////////////////////////////////////////////////////////////////////////
// CShowData dialog

//#include "Datadef.h"
//typedef CArray<_tagTTDataFrame, _tagTTDataFrame&> CArrayItemDataFrame;
CArrayItemDataFrame mCArrayItemDataFrame;
CListDataFrame mCListDataFrame;
CListFrame_e mCListFrame_e;

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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowData message handlers
	
_tagTTDataFrame mTTDataFrame;
_Frame_e mFrame_e;
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

void FUNC_DECL(FactoryA) (void)
{
	int i, j;
	i = 0;
	j = 1;
}

void FUNC_DECL(FactoryB) (void)
{
	int i, j;
	i = 0;
	j = 1;
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

int CShowData::op_delay(_tagTTDataFrame *pStFrame)
{
	DECL_FactoryA();
	DECL_FactoryB();
	
	if (pME_WD->m_delay.GetLength() == 0)
	{
		noselcode = TRUE;
		return 1;
	}
	else
	{
		noselcode = FALSE;
		return 1;
	}
}

int CShowData::op_item_e(_Frame_e *pStFrame)
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

int CShowData::op_TransCode_e(_Frame_e *pStFrame)
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

int CShowData::op_time_e(_Frame_e *pStFrame)
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
			if (pStFrame->TradeTime >= ltime &&  pStFrame->TradeTime2 <= ltime_end)
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
			if (pStFrame->TradeTime == ltime)
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
			if (pStFrame->TradeTime2 == ltime_end)
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

int CShowData::op_delay_e(_Frame_e *pStFrame)
{
	DECL_FactoryA();
	DECL_FactoryB();
	
	if (pME_WD->m_delay.GetLength() == 0)
	{
		noselcode = TRUE;
		return 1;
	}
	else
	{
		noselcode = FALSE;
		return 1;
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
	m_list_data.InsertColumn(3,"Time", LVCFMT_LEFT, 110); 
	m_list_data.InsertColumn(4,"ItemCode", LVCFMT_LEFT, 100);
	m_list_data.InsertColumn(5,"TransCode", LVCFMT_LEFT, 90);
	m_list_data.InsertColumn(6,"DataLength", LVCFMT_LEFT, 90);
	m_list_data.InsertColumn(7,"TransData", LVCFMT_LEFT, 120);

	pME_WD = (CMsEdit_WendyDlg*)AfxGetMainWnd();

	if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))==0)
	{
		m_list_data.InsertColumn(2,"Key", LVCFMT_LEFT, 70); // nLength Key
		TotalRecord = mCListFrame_e.GetCount();
	}
	else
	{
		m_list_data.InsertColumn(2,"Length", LVCFMT_LEFT, 70); // nLength Key
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
	
	for( int i=0;i<g_Master.m_pDataSrc_TTFrameFile->m_files.GetSize();i++ )
	{
		DWORD fileSize;
		hFind = FindFirstFile(g_Master.m_pDataSrc_TTFrameFile->m_files[i] ,&fileInfo);
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
	}
	else
	{
		SetTimer( 1, 1000, NULL ) ;
		GetDlgItem(IDC_BTN_TIMER)->SetWindowText("Pause");
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
		if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))==0)
			{
				ListSTLFrame_eToListCtrl(mCListFrame_e,m_list_data);
			}
		else
			{
				ListSTLDataFrameToListCtr(mCListDataFrame,m_list_data);
			}
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

void CShowData::OnWriteSearch() 
{
	// TODO: Add your control notification handler code here

#if 1
	m_lock.Lock();

	CString s_eLog;
	GetDlgItem(IDC_WRITE_SEARCH)->EnableWindow(FALSE);
	w_InofLog.EmptyLog();

	if (memcmp("e",pME_WD->m_TransCode,sizeof(BYTE))!=0)
	{
		POSITION ps;
		_tagTTDataFrame mTTDataFrame;
		DWORD mIndexNo = 0;
		for(ps = mCListDataFrame.GetHeadPosition(); (ps) ; mCListDataFrame.GetNext(ps))
		{
			mTTDataFrame = mCListDataFrame.GetAt(ps);
			
			if (op_TransCode(&mTTDataFrame)&&op_item(&mTTDataFrame)&&op_time(&mTTDataFrame))
			{

				mIndexNo ++;
				CString CstrIndexNo;
				CstrIndexNo.Format("No:%ld,",mIndexNo);
	
				CString CStrItemCode;
				char CStrItem[10];
				memset(CStrItem,0,sizeof(char)*10);
				memcpy(CStrItem,(char *)&(mTTDataFrame.arItemCode[0]),sizeof(char)*8);
				CStrItemCode.Format("Item:%s,",CStrItem);
				
				CString CstrGroup;
				CstrGroup.Format("Group:%c,",mTTDataFrame.btGroupCode);
				
				CString CstrTrans;
				CstrTrans.Format("Trans:%c,",mTTDataFrame.btTransCode);
				
				CString CstrLength;
				CstrLength.Format("Len:%d,",mTTDataFrame.nLength);
				
				CString CstrTime;
				CstrTime.Format("Time:%ld,",mTTDataFrame.lTime);		
				
				CString CstrDataLength;
				CstrDataLength.Format("Dath:%d,",mTTDataFrame.nTransDataLength);
	
				CString CstrTransData;
				CstrTransData.Format("TrDa:0x%ld,",mTTDataFrame.pTransData);	
				
				s_eLog = CstrIndexNo + CStrItemCode + CstrGroup + CstrTrans + CstrLength +CstrTime +CstrDataLength + CstrTransData;
				w_InofLog.Log(s_eLog);

			}
			
		}
	}
	else
	{
		POSITION ps_e;
		_Frame_e mFrame_e;
		DWORD mIndexNo = 0;
		for(ps_e = mCListFrame_e.GetHeadPosition(); (ps_e) ; mCListFrame_e.GetNext(ps_e))
		{
			mFrame_e = mCListFrame_e.GetAt(ps_e);
			
			if (op_TransCode_e(&mFrame_e)&&op_item_e(&mFrame_e)&&op_time_e(&mFrame_e))
			{

				mIndexNo ++;
				CString CstrIndexNo;
				CstrIndexNo.Format("No:%ld,",mIndexNo);
				
				CString CStrItemCode;
				char CStrItem[10];
				memset(CStrItem,0,sizeof(char)*10);
				memcpy(CStrItem,(char *)&(mFrame_e.arItemCode[0]),sizeof(char)*8);
				CStrItemCode.Format("ItemCode:%s,",CStrItem);
				
				//CString Cstrdwval;
				//Cstrdwval.Format("dwval:%d,",mFrame_e.dwVal);
				
				//CString CstrBrokerNo;
				//CstrBrokerNo.Format("BrokerNo:%d,",mFrame_e.BrokerNo);
				
				CString CstrTradeTime;
				CstrTradeTime.Format("Time:%d,",mFrame_e.TradeTime);
				
				CString CstrTradeTime2;
				CstrTradeTime2.Format("Time2:%d,",mFrame_e.TradeTime2);		
				
				CString CstrKey;
				CstrKey.Format("Key:%d,",mFrame_e.Key);
				
				
				CString CstrPrice;
				CstrPrice.Format("Price:%f,",mFrame_e.Price);
				
				
				CString CstrQuantity;
				CstrQuantity.Format("Quantity:%d,",mFrame_e.Quantity);
				
				
				CString CstrTradeType;
				CstrTradeType.Format("TradeType:%d",mFrame_e.TyadeType);
				
				s_eLog = CstrIndexNo + CStrItemCode + CstrTradeTime + CstrTradeTime2 +CstrKey +CstrPrice + CstrQuantity + CstrTradeType;
				w_InofLog.Log(s_eLog);

			}
		}
	}
	
	GetDlgItem(IDC_WRITE_SEARCH)->EnableWindow(TRUE);
	
	m_lock.UnLock();
#endif
}


int CShowData::ListSTLFrame_eToListCtrl(CListFrame_e &mpFrame, CListCtrl &mp_list_data)
{

#if 1
	m_lock.Lock();
	if (InitClistGetHead == FALSE)
	{
		ps = mCListFrame_e.GetHeadPosition();
		InitClistGetHead = TRUE;
	}
	
	TotalRecord = mCListFrame_e.GetCount();
	GetRecord = m_list_data.GetItemCount();
	
	RecordInterval = TotalRecord / 10000;
	if (RecordInterval <= 0) 
	{
		RecordInterval = 100;
	}
	
	for(int i=0; (ps && i<1000) ; mCListFrame_e.GetNext(ps)) 
	{
		
		mFrame_e = mCListFrame_e.GetAt(ps);
		if (op_TransCode_e(&mFrame_e)&&op_item_e(&mFrame_e)&&op_time_e(&mFrame_e))
		{
			StructFrame_eToListCtrl(mFrame_e,m_list_data);
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
				TotalRecord = mCListFrame_e.GetCount();
			}
			ps = mCListFrame_e.FindIndex(SearchRecord);
			
		}
		
		SearchRecord ++;
		if (SearchRecord >= TotalRecord)
		{
			SearchRecord = TotalRecord;
		}
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
		if (noselitem && noselcode && noseltime)
		{
			GetDlgItem(IDC_WRITE_SEARCH)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_WRITE_SEARCH)->EnableWindow(TRUE);
		}
	}
#endif 
	m_lock.UnLock();
	
	return 0;
	
}

int CShowData::ListSTLDataFrameToListCtr(CListDataFrame &mpCListDataFrame, CListCtrl &mp_list_data)
{

#if 1
	m_lock.Lock();
	if (InitClistGetHead == FALSE)
	{
		ps = mCListDataFrame.GetHeadPosition();
		InitClistGetHead = TRUE;
	}
	
	TotalRecord= mCListDataFrame.GetCount();
	GetRecord = m_list_data.GetItemCount();
	
	RecordInterval = TotalRecord / 10000;
	if (RecordInterval <= 0) 
	{
		RecordInterval = 100;
	}
	
	for(int i=0; (ps && i<1000) ; mCListDataFrame.GetNext(ps))
	{
		
		mTTDataFrame = mCListDataFrame.GetAt(ps);
		if (op_TransCode(&mTTDataFrame)&&op_item(&mTTDataFrame)&&op_time(&mTTDataFrame))
		{
			StructTTDDataFrameToListCtrl(mTTDataFrame,m_list_data);
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
				TotalRecord = mCListDataFrame.GetCount();
			}
			ps = mCListDataFrame.FindIndex(SearchRecord);
			
		}
		
		SearchRecord ++;
		if (SearchRecord >= TotalRecord)
		{
			SearchRecord = TotalRecord;
		}
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
		SetTimer( 1, 30, NULL ) ;
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
		if (noselitem && noselcode && noseltime)
		{
			GetDlgItem(IDC_WRITE_SEARCH)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_WRITE_SEARCH)->EnableWindow(TRUE);
		}
	}
#endif 
	m_lock.UnLock();
	
	return 0;
	
}
	

int CShowData::StructFrame_eToListCtrl(_Frame_e &mpFrame_e, CListCtrl &mp_list_data)
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

	memset(string,0,sizeof(char)*25);
	memset(GroupCodeString,0,sizeof(char)*3);
	memset(CStrnLength,0,sizeof(char)*6);
	memset(CStrnKey,0,sizeof(char)*6);
	memset(CStrItemCode,0,sizeof(char)*10);
	memset(CStrTransCode,0,sizeof(char)*3);
	

	itoa(GetRecord+1,string,10);
	mp_list_data.InsertItem(GetRecord,string,0);
	
	memcpy(GroupCodeString,&mpFrame_e.btGroupCode,1);
	mp_list_data.SetItemText(GetRecord,1, GroupCodeString);
	
	_itoa(mpFrame_e.Key, CStrnKey, 10); // sizeof(int)  4 sizeof(int)*4
	mp_list_data.SetItemText(GetRecord,2, CStrnKey);

	
	CStrlTime.Format("%ld",mpFrame_e.TradeTime2); // wendy lTime
	mp_list_data.SetItemText(GetRecord,3, CStrlTime);
	
	memcpy(CStrItemCode,(char *)&(mpFrame_e.arItemCode[0]),sizeof(char)*8);
	mp_list_data.SetItemText(GetRecord,4, CStrItemCode);
	
	memcpy(CStrTransCode,&(mpFrame_e.btTransCode),1);
	mp_list_data.SetItemText(GetRecord,5, CStrTransCode);
	
	CStrnTransDataLength.Format("%d",mpFrame_e.nTransDataLength);
	mp_list_data.SetItemText(GetRecord,6, CStrnTransDataLength);
	
	CStrpTransData.Format("0x%ld",(long)mpFrame_e.pTransData);
	mp_list_data.SetItemText(GetRecord,7, CStrpTransData);
	GetRecord++;
	
	return 0;
}

int CShowData::StructTTDDataFrameToListCtrl(_tagTTDataFrame &mTTDataFrame, CListCtrl &mp_list_data)
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

	memset(string,0,sizeof(char)*25);
	memset(GroupCodeString,0,sizeof(char)*3);
	memset(CStrnLength,0,sizeof(char)*6);
	memset(CStrnKey,0,sizeof(char)*6);
	memset(CStrItemCode,0,sizeof(char)*10);
	memset(CStrTransCode,0,sizeof(char)*3);
	

	itoa(GetRecord+1,string,10);
	mp_list_data.InsertItem(GetRecord,string,0);
	
	memcpy(GroupCodeString,&mTTDataFrame.btGroupCode,1);
	mp_list_data.SetItemText(GetRecord,1, GroupCodeString);
	
	_itoa(mTTDataFrame.nLength, CStrnLength,10); // sizeof(int)  4
	mp_list_data.SetItemText(GetRecord,2, CStrnLength);
	
	CStrlTime.Format("%ld",mTTDataFrame.lTime); // wendy lTime
	mp_list_data.SetItemText(GetRecord,3, CStrlTime);
	
	memcpy(CStrItemCode,(char *)&(mTTDataFrame.arItemCode[0]),sizeof(char)*8);
	mp_list_data.SetItemText(GetRecord,4, CStrItemCode);
	
	memcpy(CStrTransCode,&(mTTDataFrame.btTransCode),1);
	mp_list_data.SetItemText(GetRecord,5, CStrTransCode);
	
	CStrnTransDataLength.Format("%d",mTTDataFrame.nTransDataLength);
	mp_list_data.SetItemText(GetRecord,6, CStrnTransDataLength);
	
	CStrpTransData.Format("0x%ld",(long)mTTDataFrame.pTransData);
	mp_list_data.SetItemText(GetRecord,7, CStrpTransData);
	GetRecord++;
	
	return 0;
}
