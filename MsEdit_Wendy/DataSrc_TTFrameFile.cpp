// DataSrc_TTFrameFile.cpp: implementation of the CDataSrc_TTFrameFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit_Wendy.h"
#include "DataSrc_TTFrameFile.h"

#include "Unpacker_TTFrame.h"
//#include "ShowData.h"
#include "MsEdit_WendyDlg.h"

extern CString s_Log;
#include "ProgressDlg.h"

extern CProgressDlg *g_pProDlg;
extern DWORD g_fileSize;

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

	CString s_Log_temp;
	DWORD m_filesize_read = 0; //static
	DWORD m_filesize_send = 0;
	int nLastSendMsg = 0;
	for( int i=0;i<m_files.GetSize() && m_nThreadStatu>0;i++ )
	{
		FILE* pFile ;
		pFile = fopen(m_files[i], "rb" ) ;
		if( pFile==NULL )
		{
			s_Log.Format( "WARNING: open %s error", m_files[i] );
//			g_Log.Log(logstr) ;
			continue ;
		}

		s_Log_temp.Format( "open %s OK And Start Read...\r\n", m_files[i] );
		s_Log = s_Log + s_Log_temp;
//		g_Log.Log(logstr) ;
		readsize = 1 ;
		while( readsize>0 && m_nThreadStatu>0 )
		{
			static DWORD m_whileCount = 0;
// 			if ( WaitForSingleObject(m_hRead,0)==WAIT_OBJECT_0||
// 				 WaitForSingleObject(m_hRead,0)==WAIT_FAILED )
// 				break;
			Sleep(m_SleepCount);
			if(g_SendFilePause==FALSE)
			{
				readsize=fread( buf, 1, FILE_RATE, pFile);
				if(readsize>0)
				{
					m_whileCount ++;
					ProcRecPackage( buf, readsize ) ;

					m_filesize_read = m_filesize_read + readsize;
					if ((g_fileSize - m_filesize_read) < 1024)
					{
						nLastSendMsg = 1;
					}
					if( (m_whileCount % 3 ==0 || nLastSendMsg ) && g_pProDlg->GetSafeHwnd() )
					{
						//Sleep( 100 );
						m_filesize_send = m_filesize_read / 1024;
						g_pProDlg->SendMessage( WM_PROGRESS_MESSAGE, 0, m_filesize_send);

					}
				}
			}
		}

		filesize = filesize + _filelength(pFile->_file);
		fclose( pFile ) ;
//		logstr.Format( "finish read %s", m_files[i] );
//		g_Log.Log(logstr) ;
		s_Log_temp.Format( "Finish Read %s \r\n", m_files[i] );
		s_Log = s_Log + s_Log_temp;
	}
//	logstr.Format( "finish ttframe files" );
	s_Log = s_Log + _T("EndThread DataSrc_TTFrameFile ");
	g_pProDlg->PostMessage( WM_PROGRESS_END, 0, 123 );
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
