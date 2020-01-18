// DataSrc_TTFrameFile.cpp: implementation of the CDataSrc_TTFrameFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit_Wendy.h"
#include "DataSrc_TTFrameFile.h"

#include "Unpacker_TTFrame.h"
//#include "ShowData.h"
#include "MsEdit_WendyDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern HANDLE g_hEvent;

CDataSrc_TTFrameFile::CDataSrc_TTFrameFile()
{

}

CDataSrc_TTFrameFile::~CDataSrc_TTFrameFile()
{

}

BOOL CDataSrc_TTFrameFile::Play() 
{
	//m_nThreadStatu = 1 ;

	StartThread() ;
	//m_hThread=(HANDLE*)_beginthread(ReadThread,0,this);	
	return TRUE ;

}

BOOL		g_SendFilePause = FALSE;
#define FILE_RATE	(10*1024)
#include <io.h>
UINT CDataSrc_TTFrameFile::ThreadEntry ()
{
	m_RecDataCount = 0 ;

	char buf[1024*10] ;
	int  readsize = 1 ;
	unsigned long filesize = 0;
	for( int i=0;i<m_files.GetSize() && m_nThreadStatu>0;i++ )
	{
		FILE* pFile ;
		pFile = fopen(m_files[i], "rb" ) ;
		if( pFile==NULL )
		{
//			logstr.Format( "WARNING: open %s error", m_files[i] );
//			g_Log.Log(logstr) ;
			continue ;
		}

//		logstr.Format( "open %s OK", m_files[i] );
//		g_Log.Log(logstr) ;
		readsize = 1 ;
		while( readsize>0 && m_nThreadStatu>0 )
		{
// 			if ( WaitForSingleObject(m_hRead,0)==WAIT_OBJECT_0||
// 				 WaitForSingleObject(m_hRead,0)==WAIT_FAILED )
// 				break;
			Sleep(m_SleepCount);
			if(g_SendFilePause==FALSE)
			{
				readsize=fread( buf, 1, FILE_RATE, pFile);
				if(readsize>0)
				{
					ProcRecPackage( buf, readsize ) ;
				}
			}
		}

		filesize = filesize + _filelength(pFile->_file);
		fclose( pFile ) ;
//		logstr.Format( "finish read %s", m_files[i] );
//		g_Log.Log(logstr) ;
	}
//	logstr.Format( "finish ttframe files" );
//	g_Log.Log(logstr) ;
//	if ( g_Master.m_itemdesk.transcode!=' ' && g_Master.m_itemdesk.transcode<MAX_PATH )
//	{
//		logstr.Format("Frame-'%c' total='%d'",g_Master.m_itemdesk.transcode,g_Master.m_FrameNums[g_Master.m_itemdesk.transcode]);
//		g_Log.Log(logstr);
//	}
				
	m_nThreadStatu = -1 ;
// 	CloseHandle(g_Master.m_pDataSrc_TTFrameFile->m_hRead);
// 	g_Master.m_pDataSrc_TTFrameFile->m_hRead=NULL;
//	g_Log.Log("EndThread DataSrc_TTFrameFile.");
	//_endthread();
	Sleep(1600);
	SetEvent(g_hEvent);
	return 0;
}

void CDataSrc_TTFrameFile::ProcRecPackage( char* pPkgBuf, int pkgSize ) 
{
	//TRACE( "rec size:%d\r\n", pkgSize ) ;
	
/*
	如果在 接收线程中 解帧并处理, 可能影响接收效率.
*/
	
//	g_Master.m_pDataSrc_TTFrameFile->m_pUnpacker->InputData( (BYTE*)pPkgBuf, pkgSize ) ;  // wendy
	m_pUnpacker->InputData( (BYTE*)pPkgBuf, pkgSize ) ;  // wendy
	
	m_RecDataCount += pkgSize ;
}

BOOL CDataSrc_TTFrameFile::Init( CStringArray&  files,int SleepCount ) 
{
	m_files.SetSize( 0, 10 ) ;
	m_files.Append( files ) ;

	// 以字节流方式传输.
	//m_DataStream.Init( 512*1024, _STREAMTYPE_ByteStream ) ;		// 512K, 字节流方式.
	m_SleepCount = SleepCount;
	return TRUE ;

}
