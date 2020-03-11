
// 记录log信息的类.

#include "stdafx.h"
#include <stdio.h>
#include "log.h"
#include <io.h>
#include "PathAPI.h"
#include "tchar.h"
#include <stdarg.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CLog::CLog()
{
	m_pFile = NULL ;

	CString path ;
	CWrapperPath::GetExtPath( path ) ;

	m_szFileName.Format( "%slog.txt", path ) ;
	m_nMaxFileSize = 1024*1024 ;
	m_bKeepFileOpen = FALSE ;

}

CLog::~CLog()
{
	Close() ;

}

void CLog::Close()
{
	if( m_pFile!=NULL )
	{
		fclose(m_pFile) ;
		m_pFile = NULL ;

	}

}

BOOL CLog::Init( const char* pszFileName, int nMaxFileSize, BOOL	bKeepFileOpen )
{
	if( pszFileName==NULL || strlen(pszFileName)<=0 )
		return FALSE ;

	m_szFileName = pszFileName ;
	m_nMaxFileSize = max( 1024, nMaxFileSize ) ;
	m_bKeepFileOpen = bKeepFileOpen ;

	return TRUE ;

}

BOOL CLog::WriteLog( const char* pszMsg ) 
{
	//m_lock.Lock() ;

	// open file.
	if( m_pFile==NULL )
	{
		m_pFile = fopen( m_szFileName, "a" ) ;
		if( m_pFile==NULL )
		{
			//m_lock.UnLock() ;
			return FALSE ;			
		}

	}

	// check size.
	long lSize = _filelength( m_pFile->_file ) ;
	if( lSize>m_nMaxFileSize )	
	{
		fclose( m_pFile ) ;
		m_pFile = NULL ;

		///////////////////////////////////////////////
		// paul, 2006-6-19, add, backup log.
		CString bakFileName ;
		//bakFileName.Format( "%s.bak", m_szFileName ) ;

		CString drv, dir, fname ;
		CWrapperPath::SpitePath( m_szFileName, 
						&drv,  
						&dir,  
						&fname,  
						NULL ) ;

		// 备份更长时间( paul, 2007-8-22 )
		CTime now = CTime::GetCurrentTime() ;
		bakFileName.Format( "%s%s%s_bak_%04d%02d%02d_%02d%02d%02d.txt", drv, dir, fname, 
			now.GetYear(), now.GetMonth(), now.GetDay(), now.GetHour(), now.GetMinute(), now.GetSecond() ) ;

		::MoveFileEx( m_szFileName, bakFileName, MOVEFILE_REPLACE_EXISTING ) ;
		///////////////////////////////////////////////

		m_pFile = fopen( m_szFileName, "w" ) ;
		if( m_pFile==NULL )
		{
			//m_lock.UnLock() ;
			return FALSE ;			
		}

	}

	// log.
	fwrite( pszMsg, 1, strlen(pszMsg), m_pFile ) ;

	if( m_bKeepFileOpen==FALSE )
	{
		fclose( m_pFile ) ;
		m_pFile = NULL ;
	}
	//m_lock.UnLock() ;
	return TRUE ;

}

BOOL CLog::Log( const char* pMsg )
{
	try
	{
	m_lock.Lock() ;

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	char temp[1024*3] ;
	sprintf( temp, "%d-%02d-%02d %02d:%02d:%02d(%d), %s\r\n", sysTime.wYear, sysTime.wMonth, 
		sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, ::GetCurrentTime(), pMsg ) ;
	WriteLog( temp ) ;

	m_lock.UnLock() ;
	}
	catch(...)
	{
	}
	return TRUE ;

}


BOOL CLog::LogExt( LPCTSTR pszFormat, ... )
{
    char buf[1024*2] = "" ;
	va_list arglist;
	va_start(arglist, pszFormat);
    _vsnprintf( buf, 2048, pszFormat, arglist);
	va_end(arglist);

	return Log( buf ) ;
	
}

BOOL CLog::RawLog( const char* pszMsg ) 
{
	if( pszMsg==NULL )
		return FALSE ;

	m_lock.Lock() ;

	WriteLog( pszMsg ) ;

	m_lock.UnLock() ;
	return TRUE ;

}

BOOL CLog::RawLogExt( LPCTSTR pszFormat, ... ) 
{
    char buf[1024*2] = "" ;
	va_list arglist;
	va_start(arglist, pszFormat);
    _vsnprintf( buf, 2048, pszFormat, arglist);
	va_end(arglist);

	return RawLog( buf ) ;
	
}

BOOL CLog::WriteEachFrame( const char* fileName, const char* pszMsg ) 
{
	//m_lock.Lock() ;
	
	// open file.
	if( m_pFile==NULL )
	{
		m_pFile = fopen( fileName, "a" ) ;
		if( m_pFile==NULL )
		{
			//m_lock.UnLock() ;
			return FALSE ;			
		}
		
	}
	
	// check size.
	long lSize = _filelength( m_pFile->_file ) ;
	if( lSize>m_nMaxFileSize )	
	{
		fclose( m_pFile ) ;
		m_pFile = NULL ;
		
		///////////////////////////////////////////////
		// paul, 2006-6-19, add, backup log.
		CString bakFileName ;
		//bakFileName.Format( "%s.bak", m_szFileName ) ;
		
		CString drv, dir, fname ;
		CWrapperPath::SpitePath( fileName, 
			&drv,  
			&dir,  
			&fname,  
			NULL ) ;
		
		// 备份更长时间( paul, 2007-8-22 )
		CTime now = CTime::GetCurrentTime() ;
		bakFileName.Format( "%s%s%s_bak_%04d%02d%02d_%02d%02d%02d.txt", drv, dir, fname, 
			now.GetYear(), now.GetMonth(), now.GetDay(), now.GetHour(), now.GetMinute(), now.GetSecond() ) ;
		
		::MoveFileEx( fileName, bakFileName, MOVEFILE_REPLACE_EXISTING ) ;
		///////////////////////////////////////////////
		
		m_pFile = fopen( fileName, "w" ) ;
		if( m_pFile==NULL )
		{
			//m_lock.UnLock() ;
			return FALSE ;			
		}
		
	}
	
	// log.
	fwrite( pszMsg, 1, strlen(pszMsg), m_pFile ) ;
	
	if( m_bKeepFileOpen==FALSE )
	{
		fclose( m_pFile ) ;
		m_pFile = NULL ;
	}
	//m_lock.UnLock() ;
	return TRUE ;
	
}

BOOL CLog::LogFrame( const char* pName, const char* pMsg )
{
	try
	{
		m_lock.Lock() ;
		CString name;
		name.Format("%.7s.txt",pName);
		name.TrimRight(" ");
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		
		char temp[1024*3] ;
		sprintf( temp, "%d-%02d-%02d %02d:%02d:%02d(%d), %s\r\n", sysTime.wYear, sysTime.wMonth, 
			sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, ::GetCurrentTime(), pMsg ) ;
		WriteEachFrame( name, temp ) ;
		
		m_lock.UnLock() ;
	}
	catch(...)
	{
	}
	return TRUE ;
	
}