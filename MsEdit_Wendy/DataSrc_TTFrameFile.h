// DataSrc_TTFrameFile.h: interface for the CDataSrc_TTFrameFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATASRC_TTFRAMEFILE_H__1DE14F0F_86C1_46E9_8B5B_C94F22C186BA__INCLUDED_)
#define AFX_DATASRC_TTFRAMEFILE_H__1DE14F0F_86C1_46E9_8B5B_C94F22C186BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WorkThread.h"

//#include "Unpacker_TTFrame.h"
class CUnpacker_TTFrame ;
class CDataSrc_TTFrameFile : public CWorkThread  
{
public:
	CDataSrc_TTFrameFile();
	virtual ~CDataSrc_TTFrameFile();

	BOOL Play() ;	//类CDataSrc_TTFrameFile成员函数: 开始工作
	virtual 	UINT	ThreadEntry () ;

	BOOL Init( CStringArray&  files ,int SleepCount = 0) ;

	DWORD			m_RecDataCount ;
	
	CAutoLock		m_lock ;

	CStringArray    m_files ;

	DWORD			m_SleepCount;

	void ProcRecPackage( char* pPkgBuf, int pkgSize ) ;

	CUnpacker_TTFrame*	m_pUnpacker ;

};

#endif // !defined(AFX_DATASRC_TTFRAMEFILE_H__1DE14F0F_86C1_46E9_8B5B_C94F22C186BA__INCLUDED_)
