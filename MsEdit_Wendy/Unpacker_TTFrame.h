// Unpacker_TTFrame.h: interface for the CUnpacker_TTFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNPACKER_TTFRAME_H__3635928F_F623_4B1D_85B6_DFD6FE77A7B6__INCLUDED_)
#define AFX_UNPACKER_TTFRAME_H__3635928F_F623_4B1D_85B6_DFD6FE77A7B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Datadef.h"

class CUnpacker_TTFrame  
{
public:
	CUnpacker_TTFrame();
	virtual ~CUnpacker_TTFrame();
	
	CArrayNightMarketItem  m_arrNightMarket;
	int m_TimeNightStart;		//记录夜市开始时间
	int m_TimeNightEnd;			//记录夜市结束时间
	BOOL CheckIsXItem(char cGCode,char* szXItem);
	BOOL LoadXItemTable();
	BOOL FormatItemCode(char* szItem);
	
	BOOL InputData( BYTE* pBuf, int bufSize ) ;
	
	void ClearStream() ;
	
	void HandleFrame_e(BYTE * buff,WORD Len);
	void HandleFrame_write_e(BYTE * buff,WORD Len);
	
	BOOL CheckHasXItem(char* szItem,char* szXItem); // wendy add 2020.05.29
	
	void HandleFrame_Simple(BYTE * buff,WORD Len);		// wendy add 2020.06.04
	void HandleFrame_write_Simple(BYTE * buff,WORD Len);

	void HandleFrame_Fun(_tagTTDataFrame* pStFrame, BYTE * buff,WORD Len);
	
	///////////////////////////////////////////////////////////////////////////
	// from hope's ttframe
	BOOL				bStxFound;
	BOOL				bDleFound;
	BYTE				lpbuf;
	int					nBuffIp;
	BYTE				buff[sizeof(_tagTTDataFrame)+1030];
	short				nsValue;
	int					len;
	short				bt;
	short				j;
	int					i;
	int					InFrameHead ;
	int					nTradIp;
	_tagTTDataFrame*	pStFrame;
	DWORD				dwVal;
	unsigned short		usValue;
	tm					stTm;
	time_t				nTm;
	int					nTradLen;
	unsigned long		ulValue;
	BYTE				tempbuf[100] ;
	int					nTempBufAddr ; 
	///////////////////////////////////////////////////////////////////////////
	
	_Frame_e			*pFrame_e;
	_Frame_e			mFrame_e;
		
};

#endif // !defined(AFX_UNPACKER_TTFRAME_H__3635928F_F623_4B1D_85B6_DFD6FE77A7B6__INCLUDED_)
