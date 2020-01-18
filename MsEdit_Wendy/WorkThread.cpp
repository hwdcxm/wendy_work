// WorkThread.cpp: implementation of the CWorkThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit_Wendy.h"
#include "WorkThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DWORD WINAPI g_WorkThreadEntry(LPVOID pParam)
{
	CWorkThread* pv = (CWorkThread*)pParam ;
	return pv->ThreadEntry() ;

}

CWorkThread::CWorkThread()
{
	m_hThread = NULL ;
	m_nThreadStatu = -1 ;

}

CWorkThread::~CWorkThread()
{
	EndThread() ;
}

BOOL CWorkThread::StartThread() 
{
	if( m_nThreadStatu>=0 )
	{
		TRACE( "old thread not end!\r\n" ) ;
		return FALSE ;
	}

    DWORD dwThreadID;
	m_nThreadStatu = 1 ;

	// create thread only support to none MFC objs,
	// error to CSocket ects.
    m_hThread = CreateThread( NULL,
						   0,
						   (LPTHREAD_START_ROUTINE)g_WorkThreadEntry,
						   (LPVOID)this,
						   0,
						   &dwThreadID);

	return TRUE ;

}


void CWorkThread::RequestEnd ()
{
	if( m_hThread==NULL )
		return ;

	if( m_nThreadStatu>0 )
		m_nThreadStatu=0;

}

void CWorkThread::EndThread( int wait )
{
	if( m_hThread==NULL )
		return ;
	if( m_nThreadStatu<0 )	// 表明无线程在运行, 2005-7-5, add.
	{
	    CloseHandle(m_hThread);
		m_hThread = NULL ;	
		return ;
	}

	m_nThreadStatu=0;
	int nCount = 0 ;
	while( m_nThreadStatu>=0 && nCount<100 )
	{
		nCount++ ;
		Sleep(50);
		
		if( wait>=0 && nCount*50>wait ) 
			break ;

	}

	if( m_nThreadStatu>=0 )
	{
		TerminateThread( m_hThread, -1 ) ;
		m_nThreadStatu = -1 ;
	}

	if( m_hThread!=NULL )
	{
		CloseHandle(m_hThread);
		m_hThread = NULL ;	
	}

}
