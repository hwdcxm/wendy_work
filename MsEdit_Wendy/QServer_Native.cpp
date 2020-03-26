
#include "stdafx.h"
#include "QServer_Native.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CQS_Native::CQS_Native()
{
	m_pMaster = NULL ;

	m_pProc = NULL ;
	m_pArg  = NULL ;

}

CQS_Native::~CQS_Native()
{
}

BOOL CQS_Native::InitArgs_W2tSendData( procSendData pProc, void* pArg ) 
{
	m_pProc = pProc ;
	m_pArg  = pArg ;
	return TRUE ;

}

BOOL CQS_Native::SendData_W2t( BYTE* pData, int dataSize ) 
{
	if( m_pMaster==NULL || m_pProc==NULL )
		return FALSE ;

	BOOL ret = (*m_pProc)( m_pArg, m_pMaster, pData, dataSize ) ;
	return ret ;

}