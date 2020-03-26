
/*
!%
!$QServer.h	[.]	ͷ�ļ�: ��̨����������
!$QServer.h	[<]	?include file
!$QServer.h	[*]	*****

	
	  ��̨����������ʵ�ʴ����������:

			Զ�̵ķ���ģ��( QServer_Remote )/�����������ģ��( QServer_Native )

	  Զ�̵ķ���ģ��( W2t��ͨѶЭ�� ): ��ǰ��W2t�ܹ���ʽ.
	  �����������ģ��:				   ʵ��Զ�̷���������, ʡȥ��TCPͨѶ����.		
	  	
	 	
	  CQServer = ͨ����Ϣ���� + QServer_Remote/QServer_Native

*/


#ifndef	_QSERVER_H_
#define	_QSERVER_H_

#include <AfxTempl.h>
// #include "ChannelsMng.h"  // wendy 
#include <string>

#pragma pack(1)

//define QServer������
#define	_QSERVERTYPE_NONE					0		// û����������
#define	_QSERVERTYPE_TELE					1		// tele������	
#define	_QSERVERTYPE_CHARTING_HIS			2		// charting��ʷ���ݷ�����	
#define	_QSERVERTYPE_CHARTING_CHANNEL		3		// charting��ʱ���ݷ�����	
#define	_QSERVERTYPE_SYSINFO				4		// ϵͳ��Ϣ������
#define	_QSERVERTYPE_NEWS					5		// ���ŷ�����	
#define	_QSERVERTYPE_OPTION					6		// option������
#define	_QSERVERTYPE_LADAR					7		// ladar������
#define	_QSERVERTYPE_MSGMINE				8		// ��Ϣ���׷�����
#define _QSERVERTYPE_DATACENTER				(9)		//datacenter

#define	_QSERVER_BACKUP_BASE				10000		// BACKUP����Сֵ.
/*
	���е�QServer��֧�� backup.

	���, �涨 ԭ����+10000, Ϊ��backup����.

*/


/*
	Ϊ������ ������������� Ƶ������.
	��¼���5������������Ϣ, �����ƹ��������.
	
	Ŀǰֻ���� charting hisserver.

*/

/*
!% 
!?LAST5INFOS		[.] �ṹ: 5����������
!?LAST5INFOS		[<] ?struct
*/
typedef struct tagLAST5INFOS
{

/*
!% 
!?LAST5INFOS.tmRec0		[.] LAST5INFOS����: nReqsCount[0]��Ӧ��ʱ��.
*/
	time_t		tmRec0 ;			

/*
!% 
!?LAST5INFOS.nReqsCount	[.] LAST5INFOS����: ����5��� ��������¼. 

	ͬʱҲ�������� ����ͳ��,
*/
	int			nReqsCount[5] ;		
										
}LAST5INFOS ;

#pragma pack(8)

/*
!% 
!?CMapDoubleToLAST5INFOS		[.] ��: CMapģ�� ; ӳ��ͻ�ID ----> LAST5INFOS
!?CMapDoubleToLAST5INFOS		[<] ?class
*/
typedef CMap<double,double,LAST5INFOS,LAST5INFOS&> CMapDoubleToLAST5INFOS;

/*
!% 
!$g_ReInit()	[.] ȫ�ֺ���: ���³�ʼ��LAST5INFOS
!$g_ReInit()	[<] ?global function
*/
void g_ReInit( LAST5INFOS& obj ) ;

/*
!% 
!$g_InputTick()	[.] ȫ�ֺ���: ��LAST5INFOS�� ����һ����Ϣ( tmʱ�� ����һ������ ) .
!$g_InputTick()	[<] ?global function
*/
void g_InputTick( time_t tm, LAST5INFOS& echoInfo ) ;	

/*
!% 
!$g_InputMultiTicks()	[.] ȫ�ֺ���: ��LAST5INFOS�� ���� ����ͳ����Ϣ.
!$g_InputMultiTicks()	[<] ?global function
*/
void g_InputMultiTicks( time_t tm, int ticks, LAST5INFOS& echoInfo ) ;	

/*
!% 
!$g_CalRate()			[.] ȫ�ֺ���: ���� ���4��� ƽ������.
!$g_CalRate()			[<] ?global function
*/
double g_CalRate( LAST5INFOS& echoInfo ) ;

/*
!% 
!$g_IsBusy_CH()			[.] ȫ�ֺ���: �ж� ��ǰʱ��(tmNow), charting hisserver �Ƿ�æ( �����æ�򶪵����� ).
!$g_IsBusy_CH()			[<] ?global function
*/
BOOL g_IsBusy_CH( time_t tmNow, LAST5INFOS& info ) ;

/*
!% 
!$g_IsBusy_CC()			[.] ȫ�ֺ���: �ж� ��ǰʱ��(tmNow), charting channel server �Ƿ�æ( �����æ�򶪵����� ).
!$g_IsBusy_CC()			[<] ?global function
*/
BOOL g_IsBusy_CC( time_t tmNow, LAST5INFOS& info ) ;

/*
!% 
!?CMapDoubleToInt		[.] ��: CMapģ�� ; ӳ��double ----> int
!?CMapDoubleToInt		[<] ?class
*/
typedef CMap<double,double,int,int&> CMapDoubleToInt;

//�û����б�,CmyComm�е�Passport���ܴ���
typedef CMap<double,double,std::string,std::string&> CMapDoubleToString;

/*
!% 
!?CQServer		[.]		��: ���������
!?CQServer		[<]		?class
!?CQServer		[*]		*****
*/
class   CQServer_Remote ;
class   CQS_Native ;
class	CQServer
{
public:
	CQServer() ;
	~CQServer() ;

/*
!% 
!?CQServer.FreeModules()		[.] ��CQServer��Ա����: �ͷ�ģ��( Remote/Native )
*/
	void FreeModules() ;


/*
!% 
!?CQServer.ReInit()		[.] ��CQServer��Ա����: ���³�ʼ��
*/
	void ReInit() ;

/*
!% 
!?CQServer.Play()		[.] ��CQServer��Ա����: ��������
*/
	BOOL Play() ;

/*
!% 
!?CQServer.Stop()		[.] ��CQServer��Ա����: ֹͣ����
*/
	BOOL Stop() ;

/*
!% 
!?CQServer.GetAllClients()	[.] ��CQServer��Ա����: ȡ��QServer�е� clients�б�. (����channel==0�����ݹ㲥)
!?CQServer.GetAllClients()	[*] *****
*/
//	BOOL GetAllClients( CDoubleArray& echoClients ) ;	 // wendy 

/*
!% 
!?CQServer.GetChannelClients()	[.] ��CQServer��Ա����: ȡ��QServer��ĳ��channel�� clients�б�. (����channel!=0���ݹ㲥)
!?CQServer.GetChannelClients()	[*] *****
*/
//	BOOL GetChannelClients( DWORD dwChannel, CDoubleArray& echoClients ) ;	 // wendy 

/*
!% 
!?CQServer.AddClientRef()	[.] ��CQServer��Ա����: �����û��� channel��������Ϣ.
*/
	BOOL AddClientRef( DWORD dwChannelID, double dClientID ) ;	

/*
!% 
!?CQServer.ReleaseClientRef()	[.] ��CQServer��Ա����: �����û��� channel�ĶϿ���Ϣ.
*/
	BOOL ReleaseClientRef( DWORD dwChannelID, double dClientID ) ;	

/*
!% 
!?CQServer.AddClient()			[.] ��CQServer��Ա����: �û�����ʱ�Ĵ���.
*/
	BOOL AddClient( double dClientID ) ;	

/*
!% 
!?CQServer.RemoveClient()		[.] ��CQServer��Ա����: �û��Ͽ�ʱ�Ĵ���.
*/
	BOOL RemoveClient( double dClientID ) ;	

/*
!% 
!?CQServer.RemoveAllClients()		[.] ��CQServer��Ա����: ������е�channel��Ϣ.
*/
	BOOL RemoveAllClients() ;	

/*
!% 
!?CQServer.GetClientsCount()		[.] ��CQServer��Ա����: �õ��ͻ�������.
*/
	int	 GetClientsCount() { return m_mapClients.GetCount() ; }	

/*
!% 
!?CQServer.FindClient()				[.] ��CQServer��Ա����: ����dClientID�Ƿ���QServer��.
*/
	BOOL FindClient( double	dClientID ) ;	

/*
!% 
!?CQServer.IsType_Remote()			[.] ��CQServer��Ա����: ��Զ�̺�̨��������? 
*/
	BOOL IsType_Remote() { return (m_pRemoteObj!=NULL) ; }

/*
!% 
!?CQServer.IsType_Native()			[.] ��CQServer��Ա����: �Ǳ��ط�������? 
*/
	BOOL IsType_Native() { return (m_pNativeObj!=NULL) ; }


/*
!% 
!?CQServer.CreateModule_Remote()	[.] ��CQServer��Ա����: ����Զ�̷���ģ��
*/
	BOOL CreateModule_Remote( LPCSTR pszAddr, int port, BOOL bDisConnectAllClient ) ;
			
/*
!% 
!?CQServer.CreateModule_Native()	[.] ��CQServer��Ա����: �������ط���ģ��
*/
	BOOL CreateModule_Native( CQS_Native* pObj ) ;
	
/*
!% 
!?CQServer.IsConnect()				[.] ��CQServer��Ա����: �Ƿ�������״̬
*/
	BOOL IsConnect() ;

/*
!% 
!?CQServer.IsDisconnAllClient()		 [.] ��CQServer��Ա����: �Ƿ�Ҫ�ϵ�����clients.
*/
	BOOL IsDisconnAllClient() ;


/*
!% 
!?CQServer.ProcRecPackage()			[.] ��CQServer��Ա����: ����ͻ��˷����İ�.
*/
	BOOL ProcPkg_FromClient( char* pPkgBuf, int pkgSize  ) ;

/*
!% 
!?CQServer.GetID()			[.] ��CQServer��Ա����: ��ȡID.
*/
	double GetID() ;

/*
!% 
!?CQServer.SetQServerType()	[.] ��CQServer��Ա����: ����QServer Type
*/
	void  SetQServerType( int type ) { m_nQServerType = type ; } 

/*
!% 
!?CQServer.GetQServerType()	[.] ��CQServer��Ա����: ���QServer Type
*/
	int  GetQServerType() { return m_nQServerType ; } 

/*
!% 
!?CQServer.SetInitQServerType()	[.] ��CQServer��Ա����: ����init QServer Type
*/
	void  SetInitQServerType( int type ) { m_nIniQServerType = type ; } 

/*
!% 
!?CQServer.IsCompatibleQServer()	 [.] ��CQServer��Ա����: �Ƿ���� ĳ�����͵� QServer

  ��ǰ�����Ƿ����, ��ǰ��CQServer��ֻ�ܴ��� һ�����͵�QServer.
  ���Ǽ��ɺ�, (update/his)server �����һ�� charting server,  ���һ��server�����Ƕ������.

*/	
   BOOL IsCompatibleQServer( int serverType ) ;

public:


/*
!% 
!?CQServer.m_last5info			[.] ��CQServer����: paul, 2004-9-8, ���� ����ͳ�ƹ���.
*/
	LAST5INFOS	    m_last5info ;	

/*
!% 
!?CQServer.m_pRemoteObj			[.] ��CQServer����: Զ�̷������.
*/
	CQServer_Remote*	m_pRemoteObj ;	

/*
!% 
!?CQServer.m_pNativeObj			[.] ��CQServer����: ���ط������.
*/
	CQS_Native*			m_pNativeObj ;

public:

/*
!% 
!?CQServer.m_ChannelsMng		[.] ��CQServer����: QServer��Ӧ��Channels��Ϣ.
*/
//	CChannelsMng		m_ChannelsMng ;		 // wendy 

/*
!% 
!?CQServer.m_mapClients			[.] ��CQServer����: QServer������clients��Ϣ.
*/
	CMapDoubleToInt		m_mapClients ;		

/*
!% 
!?CQServer.m_nQServerType		[.] ��CQServer����: ��ǰ����:  ����main��backup֮���л�.
*/
	int				m_nQServerType ;	

/*
!% 
!?CQServer.m_nIniQServerType	[.] ��CQServer����: ini�е� ����.
*/
	int				m_nIniQServerType ;	

} ;

/*
!% 
!?CQServerPtrArray		[.] ��: CArrayģ�� ; ��ԪΪCQServer* �� ����.
!?CQServerPtrArray		[<] ?class
*/
typedef CArray<CQServer*,CQServer* &> CQServerPtrArray ;

#endif