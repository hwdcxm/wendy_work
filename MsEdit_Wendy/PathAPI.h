
/*
!%
!$include/PathAPI.h	[.]	 ͷ�ļ�: �ļ�Ŀ¼��ص�API.
!$include/PathAPI.h	[<]	?include file

*/

#ifndef  __PATHAPI_H_
#define  __PATHAPI_H_

#include <afxtempl.h>

/*
!% 
!?CWrapperPath		[.] ��: �ļ���ص�API.
!?CWrapperPath		[<] ?class
*/
class CWrapperPath
{
public:

/*
!% 
!$CWrapperPath.GetCurDir()	[.] ��CWrapperPath��Ա����: ȡ����ǰ·��	
*/
	static BOOL		GetCurDir( CString& echoPath ) ;

/*
!% 
!$CWrapperPath.SetCurDir()	[.] ��CWrapperPath��Ա����: ���õ�ǰ·��
*/
	static void		SetCurDir( LPCSTR pszPath ) ;	

/*
!% 
!$CWrapperPath.SetCurDir()	[.] ��CWrapperPath��Ա����: ���õ�ǰ·��
*/
	static void		SpitePath( LPCSTR pszPath, 
							   CString* pEchoDrive,  
							   CString* pEchoDir,  
							   CString* pEchoFName,  
							   CString* pEchoExt ) ; // _spitepath

/*
!% 
!$CWrapperPath.GetExtPath()	[.] ��CWrapperPath��Ա����: ȡ��exe����� path.
*/
	static BOOL		GetExtPath( CString& echoPath  ) ;	
	static BOOL		GetRootPath( CString& echoPath  ) ;	
	static BOOL		GetFileName( CString& echoPath  ) ;

/*
!% 
!$CWrapperPath.GetPrePath()	[.] ��CWrapperPath��Ա����: ȡ����һ��·��.
*/
	static BOOL		GetPrePath( LPCSTR  pszPath, CString& echoPrePath ) ;	

/*
!% 
!$CWrapperPath.GetFtpPrePath()	[.] ��CWrapperPath��Ա����: ftp pre path
*/
	static BOOL GetFtpPrePath( LPCSTR  pszPath, CString& echoPrePath ) ;

/*
!% 
!$CWrapperPath.CreatePath()	[.] ��CWrapperPath��Ա����: �����ļ���, ������Ҫ��·��.
*/
	static BOOL		CreatePath( const char* pszFile ) ;	

/*
!% 
!$CWrapperPath.IsFileExist()	[.] ��CWrapperPath��Ա����: �ļ��Ƿ����.
*/
	static BOOL		IsFileExist( const char* pszFile ) ;	

/*
!% 
!$CWrapperPath.FindFiles()	[.] ��CWrapperPath��Ա����: �ڵ�ǰĿ¼��, ����ƥ����ļ�(û�еݹ����)
*/
	static BOOL		FindFiles( const char* pszMatchFiles, CStringArray& echoArray ) ;	

/*
!% 
!$CWrapperPath.FindSubDirs()	[.] ��CWrapperPath��Ա����: �ڵ�ǰĿ¼��, ����ƥ�����Ŀ¼(û�еݹ����)
*/
	static BOOL		FindSubDirs( const char* pszMatchFiles, CStringArray& echoArray ) ;	

/*
!% 
!$CWrapperPath.FindFilesExt()	[.] ��CWrapperPath��Ա����: ����ƥ����ļ�(�ݹ����)
*/
	static BOOL		FindFilesExt( const char* pszBaseDir, const char* pszMatchFiles, CStringArray& echoArray ) ;	

/*
!% 
!$CWrapperPath.GetFileSize()	[.] ��CWrapperPath��Ա����: ȡ���ļ���С.
*/
	static int		GetFileSize( LPCSTR pszFile ) ;
	static int		GetFileSize_A( LPCSTR pszFile ) ;

/*
!% 
!$CWrapperPath.FixFtpFileName()	[.] ��CWrapperPath��Ա����: ����ftp�ļ���. ( ��'\\' ---> '/' )
*/
	static void		FixFtpFileName( CString& szFile ) ;

/*
!% 
!$CWrapperPath.FixLocalFileName()	[.] ��CWrapperPath��Ա����: ���������ļ���. ( ��'/' ---> '\\' )
*/
	static void		FixLocalFileName( CString& szFile ) ;

} ;

/*
!% 
!?CDirs		[.] ��: Ŀ¼��ص�APIs.
!?CDirs		[<] ?class
*/
class CDirs
{
public:
	CDirs() ;
	~CDirs() ;

/*
!% 
!?CDirs.ReInit()	[.] ��CDirs��Ա����: ���³�ʼ��
*/
	void ReInit() ;

/*
!% 
!?CDirs.FindDirs()	[.] ��CDirs��Ա����: ������Ŀ¼.
*/
	void FindDirs( const char* pszBaseDir, CStringArray& echoDirs ) ;	

protected:
	CStringArray	m_dirs ;
	
protected:
	void _FindDirs( const char* pszBaseDir ) ;	

} ;






#endif