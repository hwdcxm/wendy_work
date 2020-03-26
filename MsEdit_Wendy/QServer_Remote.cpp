
#include "stdafx.h"
#include "QServer_Remote.h"
#include "Master.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern Master g_Master ;

CQServer_Remote::CQServer_Remote() 
{
	m_nServerPort = 0 ;

	m_dID = 0.0f ;

	m_bDisconnAllClient=TRUE;

//	m_commObj.m_pMaster =  this ;	 // wendy 

	m_pMaster = NULL ;

}

CQServer_Remote::~CQServer_Remote()
{
}

BOOL CQServer_Remote::IsConnect() 
{
	return FALSE; // m_commObj.IsConnect() ;  // wendy 

}

void CQServer_Remote::ReInit() 
{
	m_szServerAddr = "" ;
	m_nServerPort = 0 ;

	m_dID = 0.0f ;

}

BOOL CQServer_Remote::Play()
{
	return FALSE; //m_commObj.Play() ;  // wendy 

}

BOOL CQServer_Remote::Stop()
{
	return FALSE; // FFm_commObj.Stop() ;  // wendy 

}

BOOL CQServer_Remote::SendPackage( char* pPkgBuf, int pkgSize  ) 
{
//	if( m_commObj.IsConnect()==FALSE )
//		return FALSE ;
//
//	CMyStream* pStream = m_commObj.GetSendStream() ;
//	if( pStream==NULL )
//		return FALSE ;

	BOOL ret = FALSE; // pStream->Write( (BYTE*)pPkgBuf, pkgSize ) ;  // wendy 
	return ret ;

}

void CQServer_Remote::OnConnect() 
{
	//TRACE( "%s:%d OnConnect()!\r\n", m_szServerAddr, m_nServerPort ) ;


}

void CQServer_Remote::OnDisConnect() 
{
	//TRACE( "%s:%d OnDisConnect()!\r\n", m_szServerAddr, m_nServerPort ) ;


}

