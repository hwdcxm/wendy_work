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
extern DWORD g_fileSizeKB;
extern DWORD BigFile_NoSelectGetTotalRecord;

extern CMsEdit_WendyDlg* gpMsEdit_WendyDlg;
extern int mode;
extern DWORD ReadGetFrames;


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
	DWORD m_filesize_readKB = 0; //static
	DWORD m_filesize_sendKB = 0;
	int nLastSendMsg = 0;

	MEMORYSTATUS stat;
	BigFile_NoSelectGetTotalRecord = 0;
	ReadGetFrames = 0;
	
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

					m_filesize_readKB = m_filesize_readKB + readsize/1024;
					if ((g_fileSizeKB - m_filesize_readKB) < 2 )  // FILE_RATE +3
					{
						nLastSendMsg = 1;
					}
					if( (m_whileCount % 3 ==0 || nLastSendMsg ) && g_pProDlg->GetSafeHwnd() )
					{
						//Sleep( 100 );
						m_filesize_sendKB = m_filesize_readKB; //  / 1024;
						g_pProDlg->SendMessage( WM_PROGRESS_MESSAGE, 0, m_filesize_sendKB);

						if ( mode <= 0 && nLastSendMsg==1 ) // mode <= -1
							{
								g_pProDlg->SendMessage( WM_PROGRESS_MESSAGE, 1, ReadGetFrames);
							}

					}

					if(m_whileCount % 1024 ==0 )
					{
					  static DWORD percentVirMem=100,pre_percent = 0;

					  GlobalMemoryStatus (&stat);
					  
		 			 percentVirMem = (stat.dwAvailVirtual/1024)*100 /(stat.dwTotalVirtual/1024);
					 //TRACE("%ld percent of memory is in use.\n", stat.dwMemoryLoad);
					 //TRACE ("There are percentVirMem =%ld virtual memory.\n",percentVirMem);	
					 
					  if (pre_percent > stat.dwMemoryLoad && pre_percent >90 || percentVirMem < 10 || BigFile_NoSelectGetTotalRecord >=200000)
					  	{
					  		m_nThreadStatu = -1 ;
							if (pre_percent >90)
							{
								TRACE("Because pre_percent(%d)[>90%%] > Now(%d), So EndThread m_nThreadStatu = -1.\n", pre_percent,stat.dwMemoryLoad);
								AfxMessageBox("Current system memory usage arrive 90% ! \n You can reduce made value or max value in configuration file set.ini ");
							}

							if (percentVirMem < 10)
							{
								TRACE("Because percentVirMem(%d) < 10%%, So EndThread m_nThreadStatu = -1.\n", percentVirMem);
								AfxMessageBox("Availability rate of virtual address space less 10% ! \n You can reduce made value or max value in configuration file set.ini");
							}

							if (BigFile_NoSelectGetTotalRecord >=200000)
							{
								TRACE("BigFile_NoSelectGetTotalRecord(>=200000)=%d, So EndThread m_nThreadStatu = -1.\n", BigFile_NoSelectGetTotalRecord);
							}
					  	}
					  pre_percent = stat.dwMemoryLoad;
					  
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
	if ( mode>=1)  //  mode>=0
	{
		g_pProDlg->PostMessage( WM_PROGRESS_END, 0, 123 );
	}
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

	if ( mode<=0)
	{
		delete gpMsEdit_WendyDlg->pShowData;
	}

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
	
	//m_RecDataCount += pkgSize ;
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
