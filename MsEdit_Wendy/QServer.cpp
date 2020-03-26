
#include "stdafx.h"
#include "QServer.h"
#include "log.h"
//#include "..\includeTJE\WrapperSocket.h"  // wendy 
#include "QServer_Remote.h"
#include "QServer_Native.h"
#include "Master.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern Master g_Master ;
extern CLog	g_Log ;

void g_GetServerName( CQServer* pObj, CString& echoName )
{
	ASSERT(pObj!=NULL) ;

	if( pObj->m_pRemoteObj!=NULL )
	{
		echoName.Format( "%s(%d)", pObj->m_pRemoteObj->m_szServerAddr, pObj->m_pRemoteObj->m_nServerPort ) ;
	}
	else
	{
		echoName = "built-in" ;
	}

}

void g_GetServerTypeName_Native( int nType, CString& echoName )
{
	// 嵌入的server只 支持main, 不能切换类型了.

	CString szName1 ;
	switch( nType ) 
	{
		case _QSERVERTYPE_TELE:
			szName1 = "tele" ;
			break ;
		case _QSERVERTYPE_CHARTING_HIS:
		case _QSERVERTYPE_CHARTING_CHANNEL:
			szName1 = "charting" ;
			break ;
		case _QSERVERTYPE_SYSINFO:
			szName1 = "sys info" ;
			break ;
		case _QSERVERTYPE_NEWS:
			szName1 = "news" ;
			break ;
		case _QSERVERTYPE_OPTION:
			szName1 = "option" ;
			break ;
		case _QSERVERTYPE_LADAR:
			szName1 = "radar" ;
			break ;
		case _QSERVERTYPE_MSGMINE:
			szName1 = "msgmine" ;
			break ;
		default:
			szName1.Format( "not know-%d", nType ) ;
			break ;
	}
	echoName.Format( "%s(main)", szName1 ) ;

}


void g_GetServerTypeName( int nType, CString& echoName )
{
	BOOL bBackup = FALSE ;
	if( nType>=_QSERVER_BACKUP_BASE )
	{
		bBackup = TRUE ;
		nType -= _QSERVER_BACKUP_BASE ;
	}

	CString szName1 ;
	CString szName2 = (bBackup==TRUE) ? "backup" : "main" ;
	switch( nType ) 
	{
		case _QSERVERTYPE_TELE:
			szName1 = "tele" ;
			break ;
		case _QSERVERTYPE_CHARTING_HIS:
			szName1 = "chart his" ;
			break ;
		case _QSERVERTYPE_CHARTING_CHANNEL:
			szName1 = "chart update" ;
			break ;
		case _QSERVERTYPE_SYSINFO:
			szName1 = "sys info" ;
			break ;
		case _QSERVERTYPE_NEWS:
			szName1 = "news" ;
			break ;
		case _QSERVERTYPE_OPTION:
			szName1 = "option" ;
			break ;
		case _QSERVERTYPE_LADAR:
			szName1 = "radar" ;
			break ;
		case _QSERVERTYPE_MSGMINE:
			szName1 = "msgmine" ;
			break ;
		default:
			szName1.Format( "not know-%d", nType ) ;
			break ;
	}
	echoName.Format( "%s(%s)", szName1, szName2 ) ;

}

///////////////////////////////////////////////////////////////////////////////////////////
// CQServer
CQServer::CQServer()
{
	m_nIniQServerType = m_nQServerType = _QSERVERTYPE_NONE ;

	m_mapClients.InitHashTable( 1000 ) ;
	
//	g_ReInit( m_last5info ) ;

	m_pRemoteObj = NULL ;
	m_pNativeObj = NULL ;

}

CQServer::~CQServer()
{
	FreeModules() ;

}

void CQServer::FreeModules() 
{
	if( m_pRemoteObj!=NULL )
	{
		delete m_pRemoteObj ;
		m_pRemoteObj = NULL ;
	}
	if( m_pNativeObj!=NULL )
	{
		delete m_pNativeObj ;
		m_pNativeObj = NULL ;
	}

}


void CQServer::ReInit()
{
	m_nIniQServerType = m_nQServerType = _QSERVERTYPE_NONE ;

//	m_ChannelsMng.ReInit() ;  // wendy 

	m_mapClients.InitHashTable( 1000 ) ;

//	g_ReInit( m_last5info ) ;

}

BOOL CQServer::Play()
{
	if( m_pRemoteObj!=NULL )
	{
		return m_pRemoteObj->Play() ;
	}
	if( m_pNativeObj!=NULL )
	{
		return m_pNativeObj->Play() ;
	}
	return FALSE ;

}

BOOL CQServer::Stop()
{
	if( m_pRemoteObj!=NULL )
	{
		BOOL ret = m_pRemoteObj->Stop() ;
		m_pRemoteObj = NULL;
		return ret;
	}
	if( m_pNativeObj!=NULL )
	{
		return m_pNativeObj->Stop() ;
	}
	return FALSE ;

}
//
//BOOL CQServer::GetAllClients( CDoubleArray& echoClients ) 
//{
//	echoClients.SetSize( 0, 200 ) ;
//
//	POSITION pos = m_mapClients.GetStartPosition();
//	while (pos != NULL)
//	{
//		double		key ;
//		int			nCount ;
//		m_mapClients.GetNextAssoc(pos, key, nCount);
//		echoClients.Add( key ) ;
//
//	}
//	return TRUE ;
//
//}
//
//BOOL CQServer::GetChannelClients( DWORD dwChannel, CDoubleArray& echoClients ) 
//{
//
//	echoClients.SetSize( 0, 100 ) ;
//	CChannel* pChannel = m_ChannelsMng.FindChannel( dwChannel ) ;  // wendy 
//	if( pChannel==NULL )
//	{
//		return FALSE ;
//	}
//
//	int nSize = pChannel->GetClients( echoClients ) ;
//
//	return TRUE ;
//
//}

BOOL CQServer::AddClient( double dClientID )
{
	int nCount = 1 ;
	if( m_mapClients.Lookup( dClientID, nCount)==TRUE )
	{
		return TRUE ;
	}
	m_mapClients.SetAt( dClientID, nCount ) ;
	return TRUE ;

}

BOOL CQServer::AddClientRef( DWORD dwChannelID, double dClientID )
{
	// m_ChannelsMng do AddClientRef.
	BOOL bRet; // = m_ChannelsMng.AddClientRef( dwChannelID, dClientID ) ;  // wendy 
	if( bRet==FALSE )
		return FALSE ;

	// add client count in m_mapClients.
	int nCount = 1 ;
	if( m_mapClients.Lookup( dClientID, nCount)==TRUE )
	{
		nCount++ ;
	}
	m_mapClients.SetAt( dClientID, nCount ) ;
	return TRUE ;

}

BOOL CQServer::ReleaseClientRef( DWORD dwChannelID, double dClientID )
{
	// m_ChannelsMng do ReleaseClientRef.
	BOOL bRet; // = m_ChannelsMng.ReleaseClientRef( dwChannelID, dClientID ) ;  // wendy 
	if( bRet==FALSE )
		return FALSE ;

	// release client count in m_mapClients.
	int nCount ;
	if( m_mapClients.Lookup( dClientID, nCount)==FALSE )
	{
		CString name ;
//		CWrapperSocket::GetIPAddr( dClientID, name ) ;	 // wendy 
//		g_Log.LogExt( "ERR: ReleaseClientRef error: m_ChannelsMng&m_mapClients not match, channelID:%d, clientID:%s", dwChannelID, name ) ;	 // wendy 
		//ASSERT( FALSE ) ; // m_ChannelsMng与m_mapClients中信息不匹配!.
		return TRUE ;
	}

	nCount-- ;
	if( nCount>0 )
	{
		m_mapClients.SetAt( dClientID, nCount ) ;
	}
	else
	{
		m_mapClients.RemoveKey( dClientID ) ;
	}
	return TRUE ;

}

BOOL CQServer::RemoveClient( double dClientID ) 
{
//	m_ChannelsMng.RemoveClient( dClientID ) ;	 // wendy 
	m_mapClients.RemoveKey( dClientID ) ;
	
	return TRUE ;

}

BOOL CQServer::RemoveAllClients() 
{
//	m_ChannelsMng.DelAllChannels() ;  // wendy 
	m_mapClients.RemoveAll() ;
	
	return TRUE ;

}


BOOL CQServer::FindClient( double dClientID ) 
{
	int nCount ;
	if( m_mapClients.Lookup( dClientID, nCount )!=TRUE )
	{
		return FALSE ;
	}
	return TRUE ;

}

BOOL CQServer::CreateModule_Native( CQS_Native* pObj ) 
{
	FreeModules() ;

	m_pNativeObj = pObj ;
	return TRUE ;

}


BOOL CQServer::CreateModule_Remote( LPCSTR pszAddr, int port, BOOL bDisConnectAllClient ) 
{
	FreeModules() ;

	double dNetID ;
	//if( CWrapperSocket::CreateID( pszAddr, port, dNetID )==FALSE )  // wendy 
	{
		TRACE( "NetID create error: Addr: %s,  port:%d\r\n", pszAddr, port ) ;
		//g_OutputExt( "ERR: NetID create err: addr:%s,port:%d,no add to net!", szAddr, nPort ) ;
		return FALSE ;

	}

	m_pRemoteObj = new CQServer_Remote() ;
	m_pRemoteObj->m_pMaster = this ;
	m_pRemoteObj->m_szServerAddr = pszAddr ;
	m_pRemoteObj->m_nServerPort  = port ;
	m_pRemoteObj->m_bDisconnAllClient = bDisConnectAllClient ;
	m_pRemoteObj->m_dID = dNetID ;
//	m_pRemoteObj->m_commObj.Init( "", m_pRemoteObj->m_szServerAddr, m_pRemoteObj->m_nServerPort  ) ;	 // wendy 

	return TRUE ;

}

BOOL CQServer::IsConnect()
{
	if( m_pRemoteObj!=NULL )
	{
		return m_pRemoteObj->IsConnect() ;
	}
	if( m_pNativeObj!=NULL )
	{
		return TRUE ;

	}
	return FALSE ;

}

BOOL CQServer::IsDisconnAllClient() 
{
	if( m_pRemoteObj!=NULL )
	{
		return m_pRemoteObj->m_bDisconnAllClient ;
	}
	return FALSE ;

}

int  g_LocalRequestCount = 0 ;				// 当前的 本地请求个数
int  g_ProcessedLocalRequestCount = 0 ;		// 处理的 本地请求个数
BOOL CQServer::ProcPkg_FromClient( char* pPkgBuf, int pkgSize  ) 
{
// 	// 如果是远程服务对象, 转发.
// 	if( m_pRemoteObj!=NULL )
// 	{
// 		return m_pRemoteObj->SendPackage( pPkgBuf, pkgSize ) ;
// 	}
// 
// 	// 如果是本地服务模块, 本地处理.
// 	if( m_pNativeObj!=NULL )
// 	{
// 		// 不直接处理.
// 		//return m_pNativeObj->OnRequest_W2t( (BYTE*)pPkgBuf, pkgSize ) ;
// 
// 		// 写入缓冲区中, 在ProcTTFrame中处理.
// 		if( g_Master.m_cacheStream_Request.Write( (BYTE*)pPkgBuf, pkgSize )==FALSE )
// 		{
// 			TRACE( "WARNING: native request cache stream overflow!\r\n" ) ;
// 			g_Log.LogExt( "WARNING: native request cache stream overflow!" ) ;
// 
// 		}
// 		else
// 		{
// 			g_LocalRequestCount++ ;
// 		}
// 
// 	}
// 
 	return FALSE ;

}

double CQServer::GetID()
{
	if( m_pRemoteObj!=NULL )
	{
		return m_pRemoteObj->m_dID ;
	}

	if( m_pNativeObj!=NULL )
	{
		return (double)m_nQServerType ;
	}

	return 0.0 ;

}

BOOL CQServer::IsCompatibleQServer( int serverType ) 
{
	if( m_pRemoteObj!=NULL )
	{
		return (m_nQServerType==serverType ) ? TRUE : FALSE ;
	}

	if( m_pNativeObj!=NULL )
	{
		return m_pNativeObj->CanProcRequest_W2t( serverType ) ;
	}

	return FALSE ;

}