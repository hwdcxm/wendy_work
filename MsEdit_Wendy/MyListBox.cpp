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

	CMenu menu;//�˵�
	CMenu* pPopupMenu;//�˵�ָ��
	menu.LoadMenu(IDR_MENU1);//����IDΪIDR_MENU1�Ĳ˵�
	pPopupMenu = menu.GetSubMenu(1);   //��ȡ�����˵��ĵ�һ���Ӳ˵�����ָ��

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
	
	
	CArray<int,int> aryListBoxSel; // ������������ѡ�����Ŀ������
	aryListBoxSel.SetSize(nCount);
	GetSelItems(nCount, aryListBoxSel.GetData());
	
	
	CString strSelAll=_T("");
	CString strText;
	for(int i=0; i<nCount; i++)
	{
		int nIndex = aryListBoxSel.GetAt(i); // ȡ��ѡ�����Ŀ������
		GetText(nIndex,strText); // �����������ѡ����ַ���
		strSelAll += strText;
		strSelAll += _T("\r\n");
	}
	
	
	// ���Ƶ�������
	HGLOBAL hClip;
	//����һ��HGLOBAL�����������ָ�������ڴ��
	if (OpenClipboard())
	{
		EmptyClipboard();//���������������
		hClip=GlobalAlloc(GMEM_MOVEABLE,strSelAll.GetLength()+1);
		//�ڶ��Ϸ�����ƶ����ڴ�飬���򷵻�һ���ڴ���
		char * buff; //����ָ���ַ��͵�ָ�����
		buff=(char*)GlobalLock(hClip);
		//�Է�����ڴ����м��������ڴ����ת����һ��ָ��,������Ӧ�����ü�������1
		strcpy(buff,strSelAll);
		//���û���������ݸ��Ƶ�ָ������У�ʵ���Ͼ��Ǹ��Ƶ�������ڴ����
		GlobalUnlock(hClip);
		//����д����ϣ����н����������������ü��������ּ�1
		SetClipboardData(CF_TEXT,hClip);
		//����������ݵ��ڴ�������������Դ������
		CloseClipboard();
		//�رռ����壬�ͷż�������Դ��ռ��Ȩ
		GlobalFree(hClip);
		
	}
}

void CMyListBox::OnMenuitem2Clean() 
{
	// TODO: Add your command handler code here
	ResetContent();
}
