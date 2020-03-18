
/*
!%
!$include/log.h	[.]	 ͷ�ļ�: ��¼log��Ϣ
!$include/log.h	[<]	?include file

*/


#ifndef		_LOG_H_
#define		_LOG_H_

#include	"Lock.h"

/*
!% 
!?CLog		[.] ��: ��¼log����
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
!?CLog.Close()		[.] ��CLog��Ա����: �ر������Դ
*/
	void	Close() ;

/*
!% 
!?CLog.Init()		[.] ��CLog��Ա����: ��ʼ��
*/
	BOOL	Init( const char* pszFileName, DWORD nMaxFileSize=1024*1024*500, BOOL	bKeepFileOpen=FALSE ) ;  // wendy int  1024*500

/*
!% 
!?CLog.Log()		[.] ��CLog��Ա����: ��� ����ʱ���log.
!?CLog.Log()		[*] *****
*/
	BOOL	Log( const char* pszMsg ) ;

/*
!% 
!?CLog.LogExt()		[.] ��CLog��Ա����: Log()��չ, ֧�ֱ��
!?CLog.LogExt()		[*] *****
*/
	BOOL	LogExt( LPCTSTR pszFormat, ... ) ;

/*
!% 
!?CLog.RawLog()		[.] ��CLog��Ա����: �����κ���Ϣ�� log
*/
	BOOL	RawLog( const char* pszMsg ) ;

/*
!% 
!?CLog.RawLogExt()	[.] ��CLog��Ա����: �����κ���Ϣ�� log
*/
	BOOL	RawLogExt( LPCTSTR pszFormat, ... ) ;

	BOOL	LogFrame( const char* pName, const char* pMsg ) ;
protected:
	BOOL	WriteEachFrame( const char* fileName, const char* pszMsg ) ;
public:
	BOOL EmptyLog();

/*
!% 
!?CLog.m_lock		[.] CLog����: ��
*/
	CAutoLock	m_lock ;			

/*
!% 
!?CLog.m_szFileName	[.] CLog����: log�ļ���
*/
	CString		m_szFileName ;		

/*
!% 
!?CLog.m_nMaxFileSize	[.] CLog����: log�ļ�����ļ���С
*/
	DWORD			m_nMaxFileSize ;	// wendy int

/*
!% 
!?CLog.m_bKeepFileOpen	[.] CLog����: �Ƿ񱣳��ļ���
*/
	BOOL		m_bKeepFileOpen ;	
	

protected:

/*
!% 
!?CLog.m_pFile	[.] CLog����: �ļ�ָ��
*/
	FILE*		m_pFile ;

protected:

/*
!% 
!?CLog.WriteLog()	[.] ��CLog��Ա����: дlog( �ڲ�API )
*/
	BOOL	WriteLog( const char* pszMsg ) ;
	
} ;


#endif