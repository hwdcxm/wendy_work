
/*
!%
!$include/log.h	[.]	 头文件: 记录log信息
!$include/log.h	[<]	?include file

*/


#ifndef		_LOG_H_
#define		_LOG_H_

#include	"Lock.h"

/*
!% 
!?CLog		[.] 类: 记录log的类
!?CLog		[<] ?class
!?CLog		[*] *****
*/
class CLog
{
public:
	CLog() ;
	~CLog() ;

/*
!% 
!?CLog.Close()		[.] 类CLog成员函数: 关闭相关资源
*/
	void	Close() ;

/*
!% 
!?CLog.Init()		[.] 类CLog成员函数: 初始化
*/
	BOOL	Init( const char* pszFileName, DWORD nMaxFileSize=1024*1024*500, BOOL	bKeepFileOpen=FALSE ) ;  // wendy int  1024*500

/*
!% 
!?CLog.Log()		[.] 类CLog成员函数: 添加 日期时间的log.
!?CLog.Log()		[*] *****
*/
	BOOL	Log( const char* pszMsg ) ;

/*
!% 
!?CLog.LogExt()		[.] 类CLog成员函数: Log()扩展, 支持变参
!?CLog.LogExt()		[*] *****
*/
	BOOL	LogExt( LPCTSTR pszFormat, ... ) ;

/*
!% 
!?CLog.RawLog()		[.] 类CLog成员函数: 不加任何信息的 log
*/
	BOOL	RawLog( const char* pszMsg ) ;

/*
!% 
!?CLog.RawLogExt()	[.] 类CLog成员函数: 不加任何信息的 log
*/
	BOOL	RawLogExt( LPCTSTR pszFormat, ... ) ;

	BOOL	LogFrame( const char* pName, const char* pMsg ) ;
protected:
	BOOL	WriteEachFrame( const char* fileName, const char* pszMsg ) ;
public:
	BOOL EmptyLog();

/*
!% 
!?CLog.m_lock		[.] CLog属性: 锁
*/
	CAutoLock	m_lock ;			

/*
!% 
!?CLog.m_szFileName	[.] CLog属性: log文件名
*/
	CString		m_szFileName ;		

/*
!% 
!?CLog.m_nMaxFileSize	[.] CLog属性: log文件最大文件大小
*/
	DWORD			m_nMaxFileSize ;	// wendy int

/*
!% 
!?CLog.m_bKeepFileOpen	[.] CLog属性: 是否保持文件打开
*/
	BOOL		m_bKeepFileOpen ;	
	

protected:

/*
!% 
!?CLog.m_pFile	[.] CLog属性: 文件指针
*/
	FILE*		m_pFile ;

protected:

/*
!% 
!?CLog.WriteLog()	[.] 类CLog成员函数: 写log( 内部API )
*/
	BOOL	WriteLog( const char* pszMsg ) ;
	
} ;


#endif