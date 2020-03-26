
/*
!%
!$QServer_Native.h	[.]	头文件: 本地行情服务模块
!$QServer_Native.h	[<]	?include file
!$QServer_Native.h	[*]	*****

	  本地行情服务模块:	 实现远程服务器功能, 省去了TCP通讯部分.		
	  实际这只是一个基类的定义

*/


#ifndef	_QSERVER_NATIVE_H_
#define	_QSERVER_NATIVE_H_

#include <AfxTempl.h>

#pragma pack(1)

/*
    发送数据的回调函数定义:

	参数:
	
		void* 	pArg ;		// 初始化传入的参数
		
		BYTE* 	pBuf ;		//
		int 	nSize ;		// 待发送的数据.

	返回:
		BOOL			// 是否发送成功.
		

*/
class CQServer ;
typedef BOOL (*procSendData)( void* pArg, CQServer* pServer, BYTE* pBuf, int nSize ) ;

#pragma pack(8)

/*
!% 
!?CQS_Native		[.]		类: 本地的后台服务器对象(基类)
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
!?CQS_Native.Init()			[.] 类CQS_Native成员函数: 模块的初始化工作
*/	
   virtual BOOL Init( LPCSTR pszINIFile, IUnknown* pGroupsMng ) = 0 ; 

/*
!% 
!?CQS_Native.Play()			[.] 类CQS_Native成员函数: 启动模块工作
*/	
   virtual BOOL Play() = 0 ; 

/*
!% 
!?CQS_Native.Stop()			[.] 类CQS_Native成员函数: 停止模块工作
*/	
   virtual BOOL Stop() = 0 ; 


/*
!% 
!?CQS_Native.InitArgs_W2tSendData()	 [.] 类CQS_Native成员函数: 初始化W2t协议发送数据的 回调函数和参数.

  参数:
	procSendData 	pProc	// 回调函数指针
	void* 		pArg		// 回调函数的 参数
	
*/	
   virtual BOOL InitArgs_W2tSendData( procSendData pProc, void* pArg ) ; 
	

/*
!% 
!?CQS_Native.CanProcRequest_W2t()	 [.] 类CQS_Native成员函数: 是否可以处理某种类型的W2t协议请求.

    参数:
	int serverType ;	// 服务器类型( 参见DoorServer中定义 )

*/	
   virtual BOOL CanProcRequest_W2t( int serverType ) { return FALSE ; } 


/*
!% 
!?CQS_Native.OnNewFrame_TT()		[.] 类CQS_Native成员函数: 接收 TTFrame的桢.

    参数:
	BYTE* 	pFrame		//
	int 	frameSize	// TTFrame的桢

*/	
   virtual void OnNewFrame_TT( BYTE* pFrame, WORD len ) {}

/*
!% 
!?CQS_Native.OnRequest_W2t()		[.] 类CQS_Native成员函数: 处理 W2t协议的请求包

    参数:
	BYTE* 	pPackage	//
	int 	pkgSize		// W2t协议的请求包

*/	
   virtual BOOL OnRequest_W2t( BYTE* pPackage, int pkgSize ) { return FALSE ; }

/*
!% 
!?CQS_Native.OnIgpFileUpdate()		[.] 类CQS_Native成员函数: Igp文件改变的消息.

  说明:
	如果依赖于igp文件的模块, 则应该 保存数据, 并重新用GroupsMng初始化, 然后加载数据.

*/	
   virtual void OnIgpFileUpdate() {}

/*
!% 
!?CQS_Native.SendData_W2t()		[.] 类CQS_Native成员函数: 发送W2t数据.
*/	
	BOOL SendData_W2t( BYTE* pData, int dataSize ) ;

public:

/*
!% 
!?CQS_Native.m_pMaster			[.] 类CQS_Native属性: 主控对象
*/
	CQServer*		m_pMaster ;

/*
!% 
!?CQS_Native.m_pProc			[.] 类CQS_Native属性: W2t数据的发送函数指针.
*/
	procSendData	m_pProc ;

/*
!% 
!?CQS_Native.m_pArg				[.] 类CQS_Native属性: W2t数据的发送函数之 参数.
*/
	void*			m_pArg ;

} ;

#endif