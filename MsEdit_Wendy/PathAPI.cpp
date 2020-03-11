
#include "stdafx.h"
#include "PathAPI.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CWrapperPath::FixFtpFileName( CString& szFile ) 
{
	int len = szFile.GetLength() ;
	if( len<=0 )
		return ;
	char* pBuf = szFile.GetBuffer( len ) ;
	for( int i=0;i<len;i++ )
	{
		// 处理双字节问题.
		if( (pBuf[i]&0x80)!=0x00 )
		{
			i++ ;
			continue ;
		}
		if( pBuf[i]=='\\' )
			pBuf[i] = '/' ;
	}
	szFile.ReleaseBuffer() ;

}

void CWrapperPath::FixLocalFileName( CString& szFile ) 
{
	int len = szFile.GetLength() ;
	if( len<=0 )
		return ;
	char* pBuf = szFile.GetBuffer( len ) ;
	for( int i=0;i<len;i++ )
	{
		// 处理双字节问题.
		if( (pBuf[i]&0x80)!=0x00 )
		{
			i++ ;
			continue ;
		}
		if( pBuf[i]=='/' )
			pBuf[i] = '\\' ;
	}
	szFile.ReleaseBuffer() ;

}

BOOL CWrapperPath::GetCurDir( CString& echoPath ) 
{
	echoPath = "" ;

	char szCurDir[_MAX_PATH];
	if( GetCurrentDirectory( _MAX_PATH, szCurDir )==0 )
		return FALSE ;

	echoPath = szCurDir ;
	return TRUE ;

}

void CWrapperPath::SetCurDir( LPCSTR pszPath ) 
{
	SetCurrentDirectory( pszPath ) ;

}

void CWrapperPath::SpitePath( LPCSTR pszPath, 
						CString* pEchoDrive,  
						CString* pEchoDir,  
						CString* pEchoFName,  
						CString* pEchoExt ) 
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	drive[0] = dir[0] = fname[0] = ext[0] = '\0' ; 
	_splitpath( pszPath, drive, dir, fname, ext );

	if( pEchoDrive!=NULL )		*pEchoDrive = drive ;
	if( pEchoDir!=NULL )		*pEchoDir = dir ;
	if( pEchoFName!=NULL )		*pEchoFName = fname ;
	if( pEchoExt!=NULL )		*pEchoExt = ext ;

}


BOOL CWrapperPath::GetExtPath( CString& echoPath  ) 
{
	echoPath = "" ;

	char szPath[MAX_PATH];
	memset( szPath,0,MAX_PATH);

	::GetModuleFileName( NULL,szPath,MAX_PATH);
	for(int i = strlen(szPath);i > 0;i--)
	{
		if(szPath[i] == '\\')
		{
			szPath[i] = 0;
			break;
		}
	}
	echoPath.Format( "%s\\", szPath ) ;
	return TRUE ;

}

BOOL CWrapperPath::GetFtpPrePath( LPCSTR  pszPath, CString& echoPrePath ) 
{
	echoPrePath = "" ;
	if( pszPath==NULL || strlen(pszPath)>=MAX_PATH )
		return FALSE ;

	char szTemp[MAX_PATH];
	strcpy( szTemp, pszPath ) ;

	int nCount = 0 ;
	int nAddr = strlen(szTemp) ;
	for( ;nAddr>=0;nAddr-- )
	{
		if(szTemp[nAddr]=='/')
		{
			szTemp[nAddr] = '\0' ;
				break;
		}

	}
	if( nAddr>=0 )
		echoPrePath	= szTemp ;

	return TRUE ;

}


BOOL CWrapperPath::GetPrePath( LPCSTR  pszPath, CString& echoPrePath ) 
{
	echoPrePath = "" ;
	if( pszPath==NULL || strlen(pszPath)>=MAX_PATH )
		return FALSE ;

	char szTemp[MAX_PATH];
	strcpy( szTemp, pszPath ) ;

	int nCount = 0 ;
	int nAddr = strlen(szTemp) ;
	for( ;nAddr>=0;nAddr-- )
	{
		if(szTemp[nAddr]=='\\')
		{
			nCount++ ;
			if( nCount==2 )
			{
				szTemp[nAddr+1] = '\0' ;
				break;
			}

		}

	}
	if( nAddr>=0 )
		echoPrePath	= szTemp ;

	return TRUE ;

}

BOOL CWrapperPath::CreatePath( const char* pszFile ) 
{
char szCurDir[_MAX_PATH];

	if( pszFile==NULL || GetCurrentDirectory( _MAX_PATH, szCurDir )==0 )
		return FALSE ;

	char path_buffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath( pszFile, drive, dir, fname, ext );
	sprintf( path_buffer, "%s%s", drive, dir ) ;

	BOOL  ret = TRUE ;
	if( SetCurrentDirectory( path_buffer )==FALSE )
	{
		char  seps[] = "\\";
		char* token = strtok( path_buffer+strlen(drive)+1, seps );
		if( token!=NULL )
			token = path_buffer ;
		while( token!=NULL )
		{
			BOOL ret = CreateDirectory( token, NULL ) ;
			if( ret==FALSE )
			{
				DWORD dwError = GetLastError() ;
				if( dwError==0xb7 )	// dir exist!
					ret = TRUE ;
				else
					break ;
			}
			SetCurrentDirectory( token ) ;
			token = strtok( NULL, seps );

		}

	}//end if( directory not exist! )
	SetCurrentDirectory( szCurDir ) ;
	return ret ;

}

BOOL CWrapperPath::IsFileExist( const char* pszFile ) 
{
	struct _finddata_t c_file;
	long hFile;

	if( (hFile = _findfirst( pszFile, &c_file )) == -1L )
	   return FALSE ;

    _findclose( hFile );
	return TRUE ;

}

BOOL CWrapperPath::FindFiles( const char* pszMatchFiles, CStringArray& echoArray )
{
	echoArray.SetSize( 0, 500 ) ;

	struct _finddata_t c_file;
	long hFile;

	if( (hFile = _findfirst( pszMatchFiles, &c_file )) == -1L )
	   return FALSE ;
	if( (c_file.attrib&_A_SUBDIR)==0 )
		echoArray.Add( CString(c_file.name) ) ;

    while( _findnext( hFile, &c_file ) == 0 )
	{
		if( (c_file.attrib&_A_SUBDIR)==0 )
			echoArray.Add( CString(c_file.name) ) ;
	}
    _findclose( hFile );

	return TRUE ;

}

BOOL CWrapperPath::FindSubDirs( const char* pszMatchFiles, CStringArray& echoArray ) 
{
	echoArray.SetSize( 0, 500 ) ;

	struct _finddata_t c_file;
	long hFile;

	if( (hFile = _findfirst( pszMatchFiles, &c_file )) == -1L )
	   return FALSE ;
	if( (c_file.attrib&_A_SUBDIR)!=0 )
		echoArray.Add( CString(c_file.name) ) ;

    while( _findnext( hFile, &c_file ) == 0 )
	{
		if( (c_file.attrib&_A_SUBDIR)!=0 )
			echoArray.Add( CString(c_file.name) ) ;
	}
    _findclose( hFile );

	return TRUE ;

}

BOOL CWrapperPath::FindFilesExt( const char* pszBaseDir, const char* pszMatchFiles, CStringArray& echoArray )
{
	CString szCurDir ;
	CWrapperPath::GetCurDir(szCurDir) ;

	CDirs dirs ;
	CStringArray  szDirs ;
	dirs.FindDirs( pszBaseDir, szDirs ) ;

	echoArray.SetSize( 0, 500 ) ;
	int nSize = szDirs.GetSize() ;
	for( int i=0;i<nSize;i++ )
	{
		CStringArray temps ;
		CString temp ;

		CWrapperPath::SetCurDir( szDirs[i] ) ;
		FindFiles( pszMatchFiles, temps ) ;
		
		int nTotal = temps.GetSize() ;
		for( int j=0;j<nTotal;j++ )
		{
			temp.Format( "%s\\%s", szDirs[i], temps[j] ) ;
			echoArray.Add( temp ) ;
		}

	}
	CWrapperPath::SetCurDir( szCurDir ) ;
	return TRUE ;

}

int	CWrapperPath::GetFileSize( LPCSTR pszFile ) 
{
	HANDLE hFile = NULL;
	hFile = CreateFile(pszFile,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return 0 ;

	DWORD dwLo = 0, dwHi = 0;
	dwLo = ::GetFileSize(hFile, &dwHi);
	CloseHandle(hFile) ;
	return (int)dwLo ;
	
}

//////////////////////////////////////////////////////////////////////////////////////
//

CDirs::CDirs()
{
}

CDirs::~CDirs()
{
}

void CDirs::ReInit()
{
	m_dirs.SetSize( 0, 10 ) ;
}

void CDirs::_FindDirs( const char* pszBaseDir )
{
   CFileFind finder;

   // build a string with wildcards
   CString strWildcard(pszBaseDir);
   strWildcard += _T("\\*.");

   // start working for files
   BOOL bWorking = finder.FindFile(strWildcard);

   while (bWorking)
   {
      bWorking = finder.FindNextFile();

      // skip . and .. files; otherwise, we'd
      // recur infinitely!

      if (finder.IsDots())
         continue;

      // if it's a directory, recursively search it

      if (finder.IsDirectory())
      {
         CString str = finder.GetFilePath();
		 m_dirs.Add( str ) ;
         _FindDirs(str);
      }
   }
   finder.Close();

}

void CDirs::FindDirs( const char* pszBaseDir, CStringArray& echoDirs )
{
	ReInit() ;
	_FindDirs(pszBaseDir) ;

	echoDirs.SetSize( 0, 500 ) ;

	CString szFirst = pszBaseDir ;
	echoDirs.Add( szFirst ) ;
	echoDirs.Append( m_dirs ) ;

}

