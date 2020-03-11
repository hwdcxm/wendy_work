
/*
!%
!$include/PathAPI.h	[.]	 头文件: 文件目录相关的API.
!$include/PathAPI.h	[<]	?include file

*/

#ifndef  __PATHAPI_H_
#define  __PATHAPI_H_

#include <afxtempl.h>

/*
!% 
!?CWrapperPath		[.] 类: 文件相关的API.
!?CWrapperPath		[<] ?class
*/
class CWrapperPath
{
public:

/*
!% 
!$CWrapperPath.GetCurDir()	[.] 类CWrapperPath成员函数: 取到当前路径	
*/
	static BOOL		GetCurDir( CString& echoPath ) ;

/*
!% 
!$CWrapperPath.SetCurDir()	[.] 类CWrapperPath成员函数: 设置当前路径
*/
	static void		SetCurDir( LPCSTR pszPath ) ;	

/*
!% 
!$CWrapperPath.SetCurDir()	[.] 类CWrapperPath成员函数: 设置当前路径
*/
	static void		SpitePath( LPCSTR pszPath, 
							   CString* pEchoDrive,  
							   CString* pEchoDir,  
							   CString* pEchoFName,  
							   CString* pEchoExt ) ; // _spitepath

/*
!% 
!$CWrapperPath.GetExtPath()	[.] 类CWrapperPath成员函数: 取到exe程序的 path.
*/
	static BOOL		GetExtPath( CString& echoPath  ) ;	
	static BOOL		GetRootPath( CString& echoPath  ) ;	
	static BOOL		GetFileName( CString& echoPath  ) ;

/*
!% 
!$CWrapperPath.GetPrePath()	[.] 类CWrapperPath成员函数: 取到上一级路径.
*/
	static BOOL		GetPrePath( LPCSTR  pszPath, CString& echoPrePath ) ;	

/*
!% 
!$CWrapperPath.GetFtpPrePath()	[.] 类CWrapperPath成员函数: ftp pre path
*/
	static BOOL GetFtpPrePath( LPCSTR  pszPath, CString& echoPrePath ) ;

/*
!% 
!$CWrapperPath.CreatePath()	[.] 类CWrapperPath成员函数: 根据文件名, 创建需要的路径.
*/
	static BOOL		CreatePath( const char* pszFile ) ;	

/*
!% 
!$CWrapperPath.IsFileExist()	[.] 类CWrapperPath成员函数: 文件是否存在.
*/
	static BOOL		IsFileExist( const char* pszFile ) ;	

/*
!% 
!$CWrapperPath.FindFiles()	[.] 类CWrapperPath成员函数: 在当前目录下, 查找匹配的文件(没有递归查找)
*/
	static BOOL		FindFiles( const char* pszMatchFiles, CStringArray& echoArray ) ;	

/*
!% 
!$CWrapperPath.FindSubDirs()	[.] 类CWrapperPath成员函数: 在当前目录下, 查找匹配的子目录(没有递归查找)
*/
	static BOOL		FindSubDirs( const char* pszMatchFiles, CStringArray& echoArray ) ;	

/*
!% 
!$CWrapperPath.FindFilesExt()	[.] 类CWrapperPath成员函数: 查找匹配的文件(递归查找)
*/
	static BOOL		FindFilesExt( const char* pszBaseDir, const char* pszMatchFiles, CStringArray& echoArray ) ;	

/*
!% 
!$CWrapperPath.GetFileSize()	[.] 类CWrapperPath成员函数: 取到文件大小.
*/
	static int		GetFileSize( LPCSTR pszFile ) ;
	static int		GetFileSize_A( LPCSTR pszFile ) ;

/*
!% 
!$CWrapperPath.FixFtpFileName()	[.] 类CWrapperPath成员函数: 修正ftp文件名. ( 把'\\' ---> '/' )
*/
	static void		FixFtpFileName( CString& szFile ) ;

/*
!% 
!$CWrapperPath.FixLocalFileName()	[.] 类CWrapperPath成员函数: 修正本地文件名. ( 把'/' ---> '\\' )
*/
	static void		FixLocalFileName( CString& szFile ) ;

} ;

/*
!% 
!?CDirs		[.] 类: 目录相关的APIs.
!?CDirs		[<] ?class
*/
class CDirs
{
public:
	CDirs() ;
	~CDirs() ;

/*
!% 
!?CDirs.ReInit()	[.] 类CDirs成员函数: 重新初始化
*/
	void ReInit() ;

/*
!% 
!?CDirs.FindDirs()	[.] 类CDirs成员函数: 查找子目录.
*/
	void FindDirs( const char* pszBaseDir, CStringArray& echoDirs ) ;	

protected:
	CStringArray	m_dirs ;
	
protected:
	void _FindDirs( const char* pszBaseDir ) ;	

} ;






#endif