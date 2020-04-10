// MyProgress.cpp : implementation file
//

#include "stdafx.h"
#include "MsEdit_Wendy.h"
#include "MyProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyProgress

CMyProgress::CMyProgress()
{
	m_crProgress = RGB(0,255,0);
    m_crBlank = RGB(255,255,255);
    m_crText = RGB(0,25,255);
}

CMyProgress::~CMyProgress()
{
}


BEGIN_MESSAGE_MAP(CMyProgress, CProgressCtrl)
	//{{AFX_MSG_MAP(CMyProgress)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyProgress message handlers

void CMyProgress::OnPaint() 
{
	//CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	//CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    PAINTSTRUCT ps;
    CDC *pDC = BeginPaint(&ps);
    int nPos = GetPos();
    int nMin,nMax;
    GetRange(nMin,nMax);
    CString csPos;
    csPos.Format("%d%%",nPos*100/(nMax-nMin));
    
    CRect clientRC;
    GetClientRect(clientRC);
    CSize sztext = pDC->GetTextExtent(csPos); //获取字符串的高度和宽度
    int nX = (clientRC.Width() - sztext.cx) /2; //计算中心位置
    int nY = (clientRC.Height() -sztext.cy) /2;
    pDC->SetBkMode(TRANSPARENT);
    
    
    //获取单位刻度
    double dFraction = (double) clientRC.Width()/(nMax-nMin);
    int nLeft = nPos * dFraction; //计算左边距
    CRect leftRC = clientRC;
    leftRC.right = nLeft;
    CRect rightRC = clientRC;
    rightRC.left = nLeft;
    
    pDC->FillRect(leftRC,&CBrush(m_crProgress));
    pDC->FillRect(rightRC,&CBrush(m_crBlank));
    pDC->SetTextColor(m_crText);
    pDC->TextOut(nX,nY,csPos);
    ReleaseDC(pDC);
    EndPaint(&ps);
    // Do not call CProgressCtrl::OnPaint() for painting messages
	
	// Do not call CProgressCtrl::OnPaint() for painting messages
}
