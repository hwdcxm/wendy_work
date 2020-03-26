// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__7F56FA79_04FF_4E46_8DCB_071E5581A387__INCLUDED_)
#define AFX_STDAFX_H__7F56FA79_04FF_4E46_8DCB_071E5581A387__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define _USE_32BIT_TIME_T

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include "PathAPI.h"
#pragma pack(1)
struct OptionIV
{
	int	  date;	
	
	float open;
	float high;
	float low;
	float close;
	
	float iv;			//现月波幅
	float iv2;			//4个月平均波幅，现月，3个季月
	int	  month;		//月份
	char  reserved[10];
	
	const void operator=(const OptionIV& Data)
	{
		date=Data.date;
		open=Data.open;
		high=Data.high;
		low=Data.low;
		close=Data.close;
		iv=Data.iv;
		iv2=Data.iv2;
		month=Data.month;
	}
};
typedef CArray< OptionIV, OptionIV & > CArrayOptionIV ;

struct ShortSell
{
	char     m_szItemCode[8];
	char     m_szGroupCode;
	long     m_lTradeDate;
	int		 m_iSSCount;			//记录一天有多少比沽空，最大10
	double   m_fShares[10];
	double   m_fTurnover[10];
	char	 res[10];
};
typedef CArray< ShortSell, ShortSell & > CArrayShortSell ;

typedef struct tagTradeSessionEx
{
	DWORD	date;
	BYTE	numberTS;
	WORD	timeTradeSession[5][2];
}TradeSessionEx;


#pragma pack(8)
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#define WM_PROGRESS_MESSAGE WM_USER+1
#define WM_PROGRESS_END		WM_USER+2

#endif // !defined(AFX_STDAFX_H__7F56FA79_04FF_4E46_8DCB_071E5581A387__INCLUDED_)
