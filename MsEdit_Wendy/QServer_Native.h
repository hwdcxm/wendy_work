
/*
!%
!$QServer_Native.h	[.]	ͷ�ļ�: �����������ģ��
!$QServer_Native.h	[<]	?include file
!$QServer_Native.h	[*]	*****

	  �����������ģ��:	 ʵ��Զ�̷���������, ʡȥ��TCPͨѶ����.		
	  ʵ����ֻ��һ������Ķ���

*/


#ifndef	_QSERVER_NATIVE_H_
#define	_QSERVER_NATIVE_H_

#include <AfxTempl.h>

#pragma pack(1)

/*
    �������ݵĻص���������:

	����:
	
		void* 	pArg ;		// ��ʼ������Ĳ���
		
		BYTE* 	pBuf ;		//
		int 	nSize ;		// �����͵�����.

	����:
		BOOL			// �Ƿ��ͳɹ�.
		

*/
class CQServer ;
typedef BOOL (*procSendData)( void* pArg, CQServer* pServer, BYTE* pBuf, int nSize ) ;

#pragma pack(8)

/*
!% 
!?CQS_Native		[.]		��: ���صĺ�̨����������(����)
!?CQS_Native		[<]		?class
!?CQS_Native		[*]		*****
*/
class	CQServer ;
class	CQS_Native
{
public:
	CQS_Native() ;
	virtual ~CQS_Native() ;

/*
!% 
!?CQS_Native.Init()			[.] ��CQS_Native��Ա����: ģ��ĳ�ʼ������
*/	
   virtual BOOL Init( LPCSTR pszINIFile, IUnknown* pGroupsMng ) = 0 ; 

/*
!% 
!?CQS_Native.Play()			[.] ��CQS_Native��Ա����: ����ģ�鹤��
*/	
   virtual BOOL Play() = 0 ; 

/*
!% 
!?CQS_Native.Stop()			[.] ��CQS_Native��Ա����: ֹͣģ�鹤��
*/	
   virtual BOOL Stop() = 0 ; 


/*
!% 
!?CQS_Native.InitArgs_W2tSendData()	 [.] ��CQS_Native��Ա����: ��ʼ��W2tЭ�鷢�����ݵ� �ص������Ͳ���.

  ����:
	procSendData 	pProc	// �ص�����ָ��
	void* 		pArg		// �ص������� ����
	
*/	
   virtual BOOL InitArgs_W2tSendData( procSendData pProc, void* pArg ) ; 
	

/*
!% 
!?CQS_Native.CanProcRequest_W2t()	 [.] ��CQS_Native��Ա����: �Ƿ���Դ���ĳ�����͵�W2tЭ������.

    ����:
	int serverType ;	// ����������( �μ�DoorServer�ж��� )

*/	
   virtual BOOL CanProcRequest_W2t( int serverType ) { return FALSE ; } 


/*
!% 
!?CQS_Native.OnNewFrame_TT()		[.] ��CQS_Native��Ա����: ���� TTFrame����.

    ����:
	BYTE* 	pFrame		//
	int 	frameSize	// TTFrame����

*/	
   virtual void OnNewFrame_TT( BYTE* pFrame, WORD len ) {}

/*
!% 
!?CQS_Native.OnRequest_W2t()		[.] ��CQS_Native��Ա����: ���� W2tЭ��������

    ����:
	BYTE* 	pPackage	//
	int 	pkgSize		// W2tЭ��������

*/	
   virtual BOOL OnRequest_W2t( BYTE* pPackage, int pkgSize ) { return FALSE ; }

/*
!% 
!?CQS_Native.OnIgpFileUpdate()		[.] ��CQS_Native��Ա����: Igp�ļ��ı����Ϣ.

  ˵��:
	���������igp�ļ���ģ��, ��Ӧ�� ��������, ��������GroupsMng��ʼ��, Ȼ���������.

*/	
   virtual void OnIgpFileUpdate() {}

/*
!% 
!?CQS_Native.SendData_W2t()		[.] ��CQS_Native��Ա����: ����W2t����.
*/	
	BOOL SendData_W2t( BYTE* pData, int dataSize ) ;

public:

/*
!% 
!?CQS_Native.m_pMaster			[.] ��CQS_Native����: ���ض���
*/
	CQServer*		m_pMaster ;

/*
!% 
!?CQS_Native.m_pProc			[.] ��CQS_Native����: W2t���ݵķ��ͺ���ָ��.
*/
	procSendData	m_pProc ;

/*
!% 
!?CQS_Native.m_pArg				[.] ��CQS_Native����: W2t���ݵķ��ͺ���֮ ����.
*/
	void*			m_pArg ;

} ;

#endif