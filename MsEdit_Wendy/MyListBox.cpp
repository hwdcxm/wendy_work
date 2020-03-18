// MyListBox.cpp : implementation file
//

#include "stdafx.h"
#include "msedit_wendy.h"
#include "MyListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyListBox

CMyListBox::CMyListBox()
{
}

CMyListBox::~CMyListBox()
{
}


BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
	//{{AFX_MSG_MAP(CMyListBox)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MENUITEM2_COPY, OnMenuitem2Copy)
	ON_COMMAND(ID_MENUITEM2_CLEAN, OnMenuitem2Clean)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListBox message handlers

void CMyListBox::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CMenu menu;//菜单
	CMenu* pPopupMenu;//菜单指针
	menu.LoadMenu(IDR_MENU1);//加载ID为IDR_MENU1的菜单
	pPopupMenu = menu.GetSubMenu(1);   //获取弹出菜单的第一层子菜单的类指针

	int nCount = GetSelCount();
	if(0 == nCount)
	{
		pPopupMenu->EnableMenuItem(ID_MENUITEM2_COPY,MF_DISABLED); //MF_DISABLED MF_ENABLED
	}
	else
	{
		pPopupMenu->EnableMenuItem(ID_MENUITEM2_COPY,MF_ENABLED); //MF_DISABLED MF_ENABLED
	}
	ClientToScreen(&point);
	pPopupMenu->TrackPopupMenu(TPM_BOTTOMALIGN,point.x,point.y,this,NULL); //point.x,point.y TPM_BOTTOMALIGN  TPM_LEFTALIGN

	CListBox::OnRButtonDown(nFlags, point);
}

void CMyListBox::OnMenuitem2Copy() 
{
	// TODO: Add your command handler code here
	int nCount = GetSelCount();
	if(0 == nCount)
		return;
	
	
	CArray<int,int> aryListBoxSel; // 数组用来保存选择的项目的索引
	aryListBoxSel.SetSize(nCount);
	GetSelItems(nCount, aryListBoxSel.GetData());
	
	
	CString strSelAll=_T("");
	CString strText;
	for(int i=0; i<nCount; i++)
	{
		int nIndex = aryListBoxSel.GetAt(i); // 取得选择的项目的索引
		GetText(nIndex,strText); // 根据索引获得选择的字符串
		strSelAll += strText;
		strSelAll += _T("\r\n");
	}
	
	
	// 复制到剪贴板
	HGLOBAL hClip;
	//定义一个HGLOBAL句柄变量用来指向分配的内存块
	if (OpenClipboard())
	{
		EmptyClipboard();//将剪贴板内容清空
		hClip=GlobalAlloc(GMEM_MOVEABLE,strSelAll.GetLength()+1);
		//在堆上分配可移动的内存块，程序返回一个内存句柄
		char * buff; //定义指向字符型的指针变量
		buff=(char*)GlobalLock(hClip);
		//对分配的内存块进行加锁，将内存块句柄转化成一个指针,并将相应的引用计数器加1
		strcpy(buff,strSelAll);
		//将用户输入的数据复制到指针变量中，实际上就是复制到分配的内存块中
		GlobalUnlock(hClip);
		//数据写入完毕，进行解锁操作，并将引用计数器数字减1
		SetClipboardData(CF_TEXT,hClip);
		//将存放有数据的内存块放入剪贴板的资源管理中
		CloseClipboard();
		//关闭剪贴板，释放剪贴板资源的占用权
		GlobalFree(hClip);
		
	}
}

void CMyListBox::OnMenuitem2Clean() 
{
	// TODO: Add your command handler code here
	ResetContent();
}
