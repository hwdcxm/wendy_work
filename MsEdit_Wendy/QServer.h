
/*
!%
!$QServer.h	[.]	头文件: 后台服务器对象
!$QServer.h	[<]	?include file
!$QServer.h	[*]	*****

	
	  后台服务器对象实际处理两种情况:

			远程的服务模块( QServer_Remote )/本地行情服务模块( QServer_Native )

	  远程的服务模块( W2t的通讯协议 ): 以前的W2t架构方式.
	  本地行情服务模块:				   实现远程服务器功能, 省去了TCP通讯部分.		
	  	
	 	
	  CQServer = 通道信息管理 + QServer_Remote/QServer_Native

*/


#ifndef	_QSERVER_H_
#define	_QSERVER_H_

#include <AfxTempl.h>
// #include "ChannelsMng.h"  // wendy 
#include <string>

#pragma pack(1)

//define QServer的类型
#define	_QSERVERTYPE_NONE					0		// 没有设置类型
#define	_QSERVERTYPE_TELE					1		// tele服务器	
#define	_QSERVERTYPE_CHARTING_HIS			2		// charting历史数据服务器	
#define	_QSERVERTYPE_CHARTING_CHANNEL		3		// charting即时数据服务器	
#define	_QSERVERTYPE_SYSINFO				4		// 系统信息服务器
#define	_QSERVERTYPE_NEWS					5		// 新闻服务器	
#define	_QSERVERTYPE_OPTION					6		// option服务器
#define	_QSERVERTYPE_LADAR					7		// ladar服务器
#define	_QSERVERTYPE_MSGMINE				8		// 信息地雷服务器
#define _QSERVERTYPE_DATACENTER				(9)		//datacenter

#define	_QSERVER_BACKUP_BASE				10000		// BACKUP的最小值.
/*
	所有的QServer都支持 backup.

	因此, 规定 原类型+10000, 为其backup类型.

*/


/*
	为了限制 对行情服务器的 频繁请求.
	记录最后5秒的请求个数信息, 来限制过多的请求.
	
	目前只处理 charting hisserver.

*/

/*
!% 
!?LAST5INFOS		[.] 结构: 5秒的请求计数
!?LAST5INFOS		[<] ?struct
*/
typedef struct tagLAST5INFOS
{

/*
!% 
!?LAST5INFOS.tmRec0		[.] LAST5INFOS属性: nReqsCount[0]对应的时间.
*/
	time_t		tmRec0 ;			

/*
!% 
!?LAST5INFOS.nReqsCount	[.] LAST5INFOS属性: 连续5秒的 请求数记录. 

	同时也可以用于 流量统计,
*/
	int			nReqsCount[5] ;		
										
}LAST5INFOS ;

#pragma pack(8)

/*
!% 
!?CMapDoubleToLAST5INFOS		[.] 类: CMap模板 ; 映射客户ID ----> LAST5INFOS
!?CMapDoubleToLAST5INFOS		[<] ?class
*/
typedef CMap<double,double,LAST5INFOS,LAST5INFOS&> CMapDoubleToLAST5INFOS;

/*
!% 
!$g_ReInit()	[.] 全局函数: 重新初始化LAST5INFOS
!$g_ReInit()	[<] ?global function
*/
void g_ReInit( LAST5INFOS& obj ) ;

/*
!% 
!$g_InputTick()	[.] 全局函数: 往LAST5INFOS中 增加一条信息( tm时间 增加一个请求 ) .
!$g_InputTick()	[<] ?global function
*/
void g_InputTick( time_t tm, LAST5INFOS& echoInfo ) ;	

/*
!% 
!$g_InputMultiTicks()	[.] 全局函数: 往LAST5INFOS中 增加 流量统计信息.
!$g_InputMultiTicks()	[<] ?global function
*/
void g_InputMultiTicks( time_t tm, int ticks, LAST5INFOS& echoInfo ) ;	

/*
!% 
!$g_CalRate()			[.] 全局函数: 计算 最后4秒的 平均流量.
!$g_CalRate()			[<] ?global function
*/
double g_CalRate( LAST5INFOS& echoInfo ) ;

/*
!% 
!$g_IsBusy_CH()			[.] 全局函数: 判断 当前时间(tmNow), charting hisserver 是否繁忙( 如果繁忙则丢掉请求 ).
!$g_IsBusy_CH()			[<] ?global function
*/
BOOL g_IsBusy_CH( time_t tmNow, LAST5INFOS& info ) ;

/*
!% 
!$g_IsBusy_CC()			[.] 全局函数: 判断 当前时间(tmNow), charting channel server 是否繁忙( 如果繁忙则丢掉请求 ).
!$g_IsBusy_CC()			[<] ?global function
*/
BOOL g_IsBusy_CC( time_t tmNow, LAST5INFOS& info ) ;

/*
!% 
!?CMapDoubleToInt		[.] 类: CMap模板 ; 映射double ----> int
!?CMapDoubleToInt		[<] ?class
*/
typedef CMap<double,double,int,int&> CMapDoubleToInt;

//用户名列表,CmyComm中的Passport不能传递
typedef CMap<double,double,std::string,std::string&> CMapDoubleToString;

/*
!% 
!?CQServer		[.]		类: 行情服务器
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
!?CQServer.FreeModules()		[.] 类CQServer成员函数: 释放模块( Remote/Native )
*/
	void FreeModules() ;


/*
!% 
!?CQServer.ReInit()		[.] 类CQServer成员函数: 重新初始化
*/
	void ReInit() ;

/*
!% 
!?CQServer.Play()		[.] 类CQServer成员函数: 启动工作
*/
	BOOL Play() ;

/*
!% 
!?CQServer.Stop()		[.] 类CQServer成员函数: 停止工作
*/
	BOOL Stop() ;

/*
!% 
!?CQServer.GetAllClients()	[.] 类CQServer成员函数: 取到QServer中的 clients列表. (用于channel==0的数据广播)
!?CQServer.GetAllClients()	[*] *****
*/
//	BOOL GetAllClients( CDoubleArray& echoClients ) ;	 // wendy 

/*
!% 
!?CQServer.GetChannelClients()	[.] 类CQServer成员函数: 取到QServer中某个channel的 clients列表. (用于channel!=0数据广播)
!?CQServer.GetChannelClients()	[*] *****
*/
//	BOOL GetChannelClients( DWORD dwChannel, CDoubleArray& echoClients ) ;	 // wendy 

/*
!% 
!?CQServer.AddClientRef()	[.] 类CQServer成员函数: 处理用户对 channel的连接信息.
*/
	BOOL AddClientRef( DWORD dwChannelID, double dClientID ) ;	

/*
!% 
!?CQServer.ReleaseClientRef()	[.] 类CQServer成员函数: 处理用户对 channel的断开信息.
*/
	BOOL ReleaseClientRef( DWORD dwChannelID, double dClientID ) ;	

/*
!% 
!?CQServer.AddClient()			[.] 类CQServer成员函数: 用户连接时的处理.
*/
	BOOL AddClient( double dClientID ) ;	

/*
!% 
!?CQServer.RemoveClient()		[.] 类CQServer成员函数: 用户断开时的处理.
*/
	BOOL RemoveClient( double dClientID ) ;	

/*
!% 
!?CQServer.RemoveAllClients()		[.] 类CQServer成员函数: 清除所有的channel信息.
*/
	BOOL RemoveAllClients() ;	

/*
!% 
!?CQServer.GetClientsCount()		[.] 类CQServer成员函数: 得到客户的数量.
*/
	int	 GetClientsCount() { return m_mapClients.GetCount() ; }	

/*
!% 
!?CQServer.FindClient()				[.] 类CQServer成员函数: 查找dClientID是否在QServer中.
*/
	BOOL FindClient( double	dClientID ) ;	

/*
!% 
!?CQServer.IsType_Remote()			[.] 类CQServer成员函数: 是远程后台服务器吗? 
*/
	BOOL IsType_Remote() { return (m_pRemoteObj!=NULL) ; }

/*
!% 
!?CQServer.IsType_Native()			[.] 类CQServer成员函数: 是本地服务器吗? 
*/
	BOOL IsType_Native() { return (m_pNativeObj!=NULL) ; }


/*
!% 
!?CQServer.CreateModule_Remote()	[.] 类CQServer成员函数: 创建远程服务模块
*/
	BOOL CreateModule_Remote( LPCSTR pszAddr, int port, BOOL bDisConnectAllClient ) ;
			
/*
!% 
!?CQServer.CreateModule_Native()	[.] 类CQServer成员函数: 创建本地服务模块
*/
	BOOL CreateModule_Native( CQS_Native* pObj ) ;
	
/*
!% 
!?CQServer.IsConnect()				[.] 类CQServer成员函数: 是否处于连接状态
*/
	BOOL IsConnect() ;

/*
!% 
!?CQServer.IsDisconnAllClient()		 [.] 类CQServer成员函数: 是否要断掉所有clients.
*/
	BOOL IsDisconnAllClient() ;


/*
!% 
!?CQServer.ProcRecPackage()			[.] 类CQServer成员函数: 处理客户端发来的包.
*/
	BOOL ProcPkg_FromClient( char* pPkgBuf, int pkgSize  ) ;

/*
!% 
!?CQServer.GetID()			[.] 类CQServer成员函数: 获取ID.
*/
	double GetID() ;

/*
!% 
!?CQServer.SetQServerType()	[.] 类CQServer成员函数: 设置QServer Type
*/
	void  SetQServerType( int type ) { m_nQServerType = type ; } 

/*
!% 
!?CQServer.GetQServerType()	[.] 类CQServer成员函数: 获得QServer Type
*/
	int  GetQServerType() { return m_nQServerType ; } 

/*
!% 
!?CQServer.SetInitQServerType()	[.] 类CQServer成员函数: 设置init QServer Type
*/
	void  SetInitQServerType( int type ) { m_nIniQServerType = type ; } 

/*
!% 
!?CQServer.IsCompatibleQServer()	 [.] 类CQServer成员函数: 是否兼容 某种类型的 QServer

  以前的设计欠考虑, 以前的CQServer中只能处理 一种类型的QServer.
  但是集成后, (update/his)server 变成了一个 charting server,  因此一个server可能是多个类型.

*/	
   BOOL IsCompatibleQServer( int serverType ) ;

public:


/*
!% 
!?CQServer.m_last5info			[.] 类CQServer属性: paul, 2004-9-8, 增加 流量统计功能.
*/
	LAST5INFOS	    m_last5info ;	

/*
!% 
!?CQServer.m_pRemoteObj			[.] 类CQServer属性: 远程服务对象.
*/
	CQServer_Remote*	m_pRemoteObj ;	

/*
!% 
!?CQServer.m_pNativeObj			[.] 类CQServer属性: 本地服务对象.
*/
	CQS_Native*			m_pNativeObj ;

public:

/*
!% 
!?CQServer.m_ChannelsMng		[.] 类CQServer属性: QServer对应的Channels信息.
*/
//	CChannelsMng		m_ChannelsMng ;		 // wendy 

/*
!% 
!?CQServer.m_mapClients			[.] 类CQServer属性: QServer关联的clients信息.
*/
	CMapDoubleToInt		m_mapClients ;		

/*
!% 
!?CQServer.m_nQServerType		[.] 类CQServer属性: 当前类型:  可在main与backup之间切换.
*/
	int				m_nQServerType ;	

/*
!% 
!?CQServer.m_nIniQServerType	[.] 类CQServer属性: ini中的 类型.
*/
	int				m_nIniQServerType ;	

} ;

/*
!% 
!?CQServerPtrArray		[.] 类: CArray模板 ; 单元为CQServer* 的 数组.
!?CQServerPtrArray		[<] ?class
*/
typedef CArray<CQServer*,CQServer* &> CQServerPtrArray ;

#endif