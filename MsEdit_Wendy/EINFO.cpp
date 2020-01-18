// EINFO.cpp : implementation file
//

#include "stdafx.h"
#include "MsEdit_Wendy.h"
#include "EINFO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ShowData.h"

/////////////////////////////////////////////////////////////////////////////
// CEINFO dialog


CEINFO::CEINFO(CWnd* pParent /*=NULL*/)
	: CDialog(CEINFO::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEINFO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEINFO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEINFO)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEINFO, CDialog)
	//{{AFX_MSG_MAP(CEINFO)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEINFO message handlers

BOOL CEINFO::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	// TODO: Add extra initialization here
	
	SetTimer( 1, 1000, NULL ) ;

	//	strGroup[100];
//	strItemCode[100];
//	strTime[100];
//	strLength[100];

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CEINFO::cmp_item(_Frame_e *pFrame_e)
{

	if (memcmp(&(pFrame_e->arItemCode),strItemCode,sizeof(char)*8)==0)
		return 1;
	else
		return 0;
	

}

int CEINFO::cmp_time(_Frame_e *pFrame_e)
{
	long ltime = atol(strTime);

	if (pFrame_e->lTime == ltime)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

extern CListFrame_e mCListFrame_e;
void CEINFO::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	POSITION ps;
	for(ps = mCListFrame_e.GetHeadPosition(); (ps) ; mCListFrame_e.GetNext(ps))
	{
		mFrame_e = mCListFrame_e.GetAt(ps);
		if (cmp_item(&mFrame_e) && cmp_time(&mFrame_e))
		{	
			CString Cstrdwval;
			Cstrdwval.Format("%d",mFrame_e.dwVal);
			GetDlgItem(IDC_EDIT1)->SetWindowText(Cstrdwval);

			CString CstrBrokerNo;
			CstrBrokerNo.Format("%d",mFrame_e.BrokerNo);
			GetDlgItem(IDC_EDIT2)->SetWindowText(CstrBrokerNo);

			CString CstrTradeTime;
			CstrTradeTime.Format("%d",mFrame_e.TradeTime);
			GetDlgItem(IDC_EDIT3)->SetWindowText(CstrTradeTime);

			CString CstrTradeTime2;
			CstrTradeTime2.Format("%d",mFrame_e.TradeTime2);
			GetDlgItem(IDC_EDIT4)->SetWindowText(CstrTradeTime2);

			CString CstrKey;
			CstrKey.Format("%d",mFrame_e.Key);
			GetDlgItem(IDC_EDIT5)->SetWindowText(CstrKey);

			CString CstrPrice;
			CstrPrice.Format("%f",mFrame_e.Price);
			GetDlgItem(IDC_EDIT6)->SetWindowText(CstrPrice);

			CString CstrQuantity;
			CstrQuantity.Format("%d",mFrame_e.Quantity);
			GetDlgItem(IDC_EDIT7)->SetWindowText(CstrQuantity);

			CString CstrTyadeType;
			CstrTyadeType.Format("%d",mFrame_e.TyadeType);
			GetDlgItem(IDC_EDIT8)->SetWindowText(CstrTyadeType);

			CString CstrlTemp;
			CstrlTemp.Format("%d",mFrame_e.lTemp);
			GetDlgItem(IDC_EDIT9)->SetWindowText(CstrlTemp);

			CString CstrbyTmp;
			CstrbyTmp.Format("%d",mFrame_e.byTmp);
			GetDlgItem(IDC_EDIT10)->SetWindowText(CstrbyTmp);
			break;
		}

	}

	CDialog::OnTimer(nIDEvent);
}
