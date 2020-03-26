
/*
!%
!$QServer_Remote.h	[.]	头文件: 远程的后台服务器对象
!$QServer_Remote.h	[<]	?include file
!$QServer_Remote.h	[*]	*****
	
	  远程的服务模块( W2t的通讯协议 ): 以前的W2t架构方式.
	  MS中只是进行TCP通讯.	

*/


#ifndef	_QSERVER_REMOTE_H_
#define	_QSERVER_REMOTE_H_

#include <AfxTempl.h>
//#include "MyClientComm.h"  // wendy 

#pragma pack(1)

#pragma pack(8)

/*
!% 
!?CQServer_Remote		[.]		类: 远程的后台服务器对象
!?CQServer_Remote		[<]		?class
!?CQServer_Remote		[*]		*****
*/
class   CQServer ;
class	CQServer_Remote
{
public:
	CQServer_Remote() ;
	~CQServer_Remote() ;

	void ReInit() ;

/*
!% 
!?CQServer_Remote.Play()		[.] 类CQServer_Remote成员函数: 启动工作
*/
	BOOL Play() ;

/*
!% 
!?CQServer_Remote.Stop()		[.] 类CQServer_Remote成员函数: 停止工作
*/
	BOOL Stop() ;

/*
!% 
!?CQServer_Remote.SendPackage()		[.] 类CQServer_Remote成员函数: 发送数据包.
*/
	BOOL SendPackage( char* pPkgBuf, int pkgSize  ) ;

/*
!% 
!?CQServer_Remote.IsConnnect()		[.] 类CQServer_Remote成员函数: TCP线路是否连接
*/
	BOOL IsConnect() ;

	void OnConnect() ;

	void OnDisConnect() ;

public:

/*
!% 
!?CQServer_Remote.m_szServerAddr	[.] 类CQServer_Remote属性: server ip地址
*/
	CString			m_szServerAddr ;

/*
!% 
!?CQServer_Remote.m_nServerPort		[.] 类CQServer_Remote属性: server port
*/
	int				m_nServerPort ;

/*
!% 
!?CQServer_Remote.m_dID				[.] 类CQServer_Remote属性: 网络模块的ID.
*/
	double			m_dID ;				

/*
!% 
!?CQServer_Remote.m_bDisconnAllClient	[.] 类CQServer_Remote属性: 是否需要断掉所有client.
*/
	BOOL               m_bDisconnAllClient;

/*
!% 
!?CQServer_Remote.m_commObj				[.] 类CQServer_Remote属性: 通讯对象	
*/
//	CMyClientComm	m_commObj ;		 // wendy 	

/*
!% 
!?CQServer_Remote.m_pMaster				[.] 类CQServer_Remote属性: 主控对象
*/
	CQServer*		m_pMaster ;	

} ;

#endif