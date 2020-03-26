
/*
!%
!$QServer_Remote.h	[.]	ͷ�ļ�: Զ�̵ĺ�̨����������
!$QServer_Remote.h	[<]	?include file
!$QServer_Remote.h	[*]	*****
	
	  Զ�̵ķ���ģ��( W2t��ͨѶЭ�� ): ��ǰ��W2t�ܹ���ʽ.
	  MS��ֻ�ǽ���TCPͨѶ.	

*/


#ifndef	_QSERVER_REMOTE_H_
#define	_QSERVER_REMOTE_H_

#include <AfxTempl.h>
//#include "MyClientComm.h"  // wendy 

#pragma pack(1)

#pragma pack(8)

/*
!% 
!?CQServer_Remote		[.]		��: Զ�̵ĺ�̨����������
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
!?CQServer_Remote.Play()		[.] ��CQServer_Remote��Ա����: ��������
*/
	BOOL Play() ;

/*
!% 
!?CQServer_Remote.Stop()		[.] ��CQServer_Remote��Ա����: ֹͣ����
*/
	BOOL Stop() ;

/*
!% 
!?CQServer_Remote.SendPackage()		[.] ��CQServer_Remote��Ա����: �������ݰ�.
*/
	BOOL SendPackage( char* pPkgBuf, int pkgSize  ) ;

/*
!% 
!?CQServer_Remote.IsConnnect()		[.] ��CQServer_Remote��Ա����: TCP��·�Ƿ�����
*/
	BOOL IsConnect() ;

	void OnConnect() ;

	void OnDisConnect() ;

public:

/*
!% 
!?CQServer_Remote.m_szServerAddr	[.] ��CQServer_Remote����: server ip��ַ
*/
	CString			m_szServerAddr ;

/*
!% 
!?CQServer_Remote.m_nServerPort		[.] ��CQServer_Remote����: server port
*/
	int				m_nServerPort ;

/*
!% 
!?CQServer_Remote.m_dID				[.] ��CQServer_Remote����: ����ģ���ID.
*/
	double			m_dID ;				

/*
!% 
!?CQServer_Remote.m_bDisconnAllClient	[.] ��CQServer_Remote����: �Ƿ���Ҫ�ϵ�����client.
*/
	BOOL               m_bDisconnAllClient;

/*
!% 
!?CQServer_Remote.m_commObj				[.] ��CQServer_Remote����: ͨѶ����	
*/
//	CMyClientComm	m_commObj ;		 // wendy 	

/*
!% 
!?CQServer_Remote.m_pMaster				[.] ��CQServer_Remote����: ���ض���
*/
	CQServer*		m_pMaster ;	

} ;

#endif