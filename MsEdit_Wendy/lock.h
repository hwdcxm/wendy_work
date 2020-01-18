// lock.h
//
//////////////////////////////////////////////////////////////////////

#ifndef _LOCK_H_
#define _LOCK_H_

class CAutoLock
{
public:
	CAutoLock()		{ InitializeCriticalSection(&m_cs); }
	~CAutoLock()	{ 	DeleteCriticalSection(&m_cs); }
private:
	CRITICAL_SECTION  m_cs ;	
public:
	void Lock()  { EnterCriticalSection(&m_cs); }
	void UnLock() { LeaveCriticalSection( &m_cs ); }
};

// auto lock and unlock
class CQILock
{
public:
	CQILock( CAutoLock* pLock ) { m_pLock = pLock ;  m_pLock->Lock() ; }
	~CQILock() { m_pLock->UnLock() ; }

public:
	CAutoLock*	m_pLock ;
	

} ;

#endif 
