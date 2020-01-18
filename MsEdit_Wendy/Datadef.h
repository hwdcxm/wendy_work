#ifndef _DATA_DEF_H_
#define _DATA_DEF_H_

//#include "W2tNetProtocol.h"
//#include "ChartingItem.h"
#include <afxtempl.h>

 
#pragma pack(1)

//
#define	MINIMAL_PRICE			(0.000001f)		// 定义合法的 最小价位.	

//data Frame define
#define TT_FRAME_UNKNOWN	0
#define TT_CHA_ITEMCODE 	1
#define TT_NUM_ITEMCODE 	2

#define	G_GRPFTXTLEN			38	/* TeleText stock page free text length	*/
#define	G_GRPTSHARELEN			7	/* Length of total shares field	in TeleText stock page */
#define	G_GRPTOVERLEN			7	/* Length of turnover field in TeleText	stock page */
#define	G_GRPBSHARELEN			4	/* Length of broker shares field in TeleText stock page	*/

struct _tagTTDataFrame{
	int nLength;
	time_t lTime;
	BYTE btGroupCode;
	char arItemCode[8];
	BYTE btTransCode;
	int nTransDataLength;
	void * pTransData;
};

struct _Frame_e  // get from _tagTTDataFrame
{
	int nLength;
	time_t lTime;
	BYTE btGroupCode;
	char arItemCode[8];
	BYTE btTransCode;
	int nTransDataLength;
	void * pTransData;

	DWORD	dwVal;
	WORD	BrokerNo;
	WORD	TradeTime;
	DWORD   TradeTime2; 
	WORD	Key;
	float	Price;
	unsigned long Quantity;
	BYTE	TyadeType;
	DWORD	lTemp;
	BYTE	byTmp;
};

struct _tagFrame201
{
	BYTE cSTX;
	BYTE cGCode;
	WORD wdLen;
	WORD wdTime;
	BYTE cFlag;	
	WORD wdUserId;
} ;

struct _tagFileTrans
{
	WORD wdUserId ;
	DWORD dwSystemId ;
	DWORD dwExtendSystemId ;
	DWORD dwExtendSystemIdMask ;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// charting中协议.

// trade tick( DIAO协议中使用 )
struct TRADETICK_DIAO
{
	BYTE	type;			
	WORD	tickTime;		//时间( HHSS )
	float	price;			//价位
	DWORD	shares;			//成交量
	WORD	ttickId;		//TradeTick编号(当天的编号)

} ;

// minute bar( DIAO协议中使用 )
struct MINUTEBAR_DIAO
{
	WORD	time ;
	float	open ;
	float	high ;
	float	low  ;
	float	close ;

} ;

// minute bar(TradeTick) ( DIAO协议中使用 )
struct TT_MINUTEBAR_DIAO
{
	WORD	time ;
	float	open ;
	float	high ;
	float	low  ;
	float	close ;
	DWORD	share ;
	float	turnover ;

} ;
typedef CArray< TT_MINUTEBAR_DIAO, TT_MINUTEBAR_DIAO & > CArrayTT_MINUTEBAR_DIAO ;

// 一天内的信息(  DIAO协议中使用 )
struct DAYINFO_DIAO
{
	WORD	recordCount ;		// 记录数
	DWORD	date ;				// 日期
	BYTE	ver ;				// 版本号

} ;

// day bar( DIAO协议中使用 )
struct DAYBAR_DIAO
{
	DWORD	date ;
	float	open ;
	float	high ;
	float	low  ;
	float	close ;
	double	share ;
	double	turnover ;

} ;

// 发给door 通道数据的包头( 通道广播包头部分 )
struct STProtocol_SR_UD
{
	//W2TPKGHAED				netHead ;

	//WORD	ReqIdNum;			//请求服务器个数
	//DOUBLE	ReqID;			//请求服务器ID

	WORD	BegFlag;			//开始标志'WS'
	DWORD	Length;				//长度不包括 'WS' 标记.
	WORD	DfFlag;				//'DF'
	WORD	DaOse;				//'DA'的位移
	WORD	CiFlag;				//'CI'
	DWORD	CannalID;			//CannalID
	WORD	Dalag;				//'DA'
};

// request data的 响应包头( 请求应答包 头部分 )
struct	STSERVERREQ
{
	//W2TPKGHAED				netHead ;

	//WORD	ReqIdNum;		//=1
	//DOUBLE	ReqId;			//请求ID

	WORD	WSFlag;			//'WS'
	DWORD	Len;
	WORD	DFFlag;			//'DF'
	WORD	DaOf;			//DAta位移
	WORD	UIFlag;			//'UI'
	DOUBLE	UserId;			//User id
	WORD	DAFlag;			//'DA'

};

// request data( compact )的 响应包头( 请求应答包(压缩包) 头部分 )
struct	STSERVERREQ_COMPACT
{
	//W2TPKGHAED				netHead ;

	//WORD	ReqIdNum;		//=1
	//DOUBLE	ReqId;			//请求ID

	WORD	WSFlag;			//'WS'
	DWORD	Len;
	WORD	DFFlag;			//'DF'
	WORD	DaOf;			//DAta位移
	WORD	UIFlag;			//'UI'
	DOUBLE	UserId;			//User id

	WORD	SPFlag;			//'SP'
	WORD	SecurityType ;	//'ZP'=ZIP压缩模式

	WORD	DAFlag;			//'DA'

};



/*
	添充 通道数据包头.
*/
void g_Init_SR_UD( STProtocol_SR_UD& data, DWORD chlID, int pkgSize ) ;


// price type
#define	PRICETYPE_NOMINAL		(0)		// nominal
#define	PRICETYPE_BID			(1)		// bid
#define	PRICETYPE_ASK			(2)		// ask
#define	PRICETYPE_BID_ASK		(3)		// bid&ask
#define	PRICETYPE_TRADETICK		(4)		// trade tick
#define	PRICETYPE_OPEN			(50)	// open price
#define	PRICETYPE_LOW			(60)	// low price
#define	PRICETYPE_HIGH			(70)	// high price	
#define	PRICETYPE_PRVCLOSE		(90)	// Previous closing price

// period type
#define	PERIODTYPE_TICK			(0)		// tick
#define	PERIODTYPE_MINUTE		(1)		// minute
#define	PERIODTYPE_HALFHOUR		(2)		// half hour
#define	PERIODTYPE_DAY			(3)		// day
#define	PERIODTYPE_WEEK			(4)		// week
#define	PERIODTYPE_MONTH		(5)		// month
#define	PERIODTYPE_YEAR			(6)		// year

// 'C2'协议头
struct STProtocol_C2_UFHEAD
{
	WORD	size;
	WORD	flag;		//'C2'
	BYTE	GCode;
	DOUBLE	ICode;
	BYTE	PriceType;	//0-Nominal 1-bid 2-ask 3-B&a 4-Trade Tick
	BYTE	PeriodType;	//0-Tick 1-min 2-half hour 3-day 4-week 5-mouth 6-year
	WORD	PeriodArg;
	WORD	SubFlag;	//'UF'
	WORD	TolRecords;	//==1
	DWORD	Date;
};

// Bid&ask tick 记录.
struct STProtocol_C2_RECORDAB
{
	WORD	Time;
	float	AskPrice;
	float	BidPrice;
	WORD	Id;
};

/*
	Bid&Ask的 tick update协议数据包
*/
struct STProtocol_C2_UFAB
{
	STProtocol_SR_UD		chlDataHead ;		// channel data head( W2t外层协议 )			

	STProtocol_C2_UFHEAD	Head;				// 'C2' 协议头
	STProtocol_C2_RECORDAB	Records;			// 记录

	WORD					eleED ;				// 'ED'
};

/*
	Bid&Ask的 tick snapshot协议数据包( C2-LF )
*/
struct STProtocol_C2_LFAB
{
	STSERVERREQ				HisDataHead ;		// his data head( W2t外层协议 )			

	STProtocol_C2_UFHEAD	Head;				// 'C2' 协议头
	STProtocol_C2_RECORDAB	Records;			// 记录

	WORD					eleED ;				// 'ED'
};


/*
	添充 Bid&Ask的 tick update协议 数据( 'UF' )
*/
BOOL g_Init_C2_UFAB( STProtocol_C2_UFAB& data,  DWORD chlID, DWORD date, char* pItemCode, WORD tm, float bid, float ask, WORD ID ) ;

/*
	添充 Bid&Ask的 SnapShot协议 数据( 'LF' )
*/
BOOL g_Init_C2_LFAB( STProtocol_C2_LFAB& data,  double UserID, DWORD date, char* pItemCode, WORD tm, float bid, float ask, WORD ID ) ;

/*
	添充 Bid&Ask的 SnapShot协议 数据( 无数据情况 ) ( 'td' )
*/
BOOL g_Init_C2_tdAB( STProtocol_C2_LFAB& data,  double UserID, DWORD date, char* pItemCode ) ;


struct STProtocol_C2_RECORDDAY
{
	float	OpenVal;			//周期内的开市价
	float	HigVal;				//周期内的最高价
	float	LowVal;				//周期内的最低价
	float	CloseVal;			//周期内的收市价
	//DWORD 	dwShare;
	//float 	fTurnover;
	DOUBLE	Shares;
	DOUBLE	Turnover;
};

/*
	day update协议数据包
*/
struct STProtocol_C2_UFDAY
{
	STProtocol_SR_UD		chlDataHead ;		// channel data head( W2t外层协议 )			

	STProtocol_C2_UFHEAD	Head;				// 'C2'协议头
	STProtocol_C2_RECORDDAY Records;			// 记录

	WORD					eleED ;				// 'ED' 

};

/*
	day snapshot协议数据包( C2-LF )
*/
struct STProtocol_C2_LFDAY
{
	STSERVERREQ				HisDataHead ;		// his data head( W2t外层协议 )			

	STProtocol_C2_UFHEAD	Head;				// 'C2'协议头
	STProtocol_C2_RECORDDAY Records;			// 记录

	WORD					eleED ;				// 'ED' 

};

/*
	添充 day bar update协议 数据.
*/
BOOL g_Init_C2_UF_Day( STProtocol_C2_UFDAY& data, DWORD chlID, DWORD date, char* pItemCode, float open, float high, float low, float close, double share, double turnover ) ;

	/*
		添充 Bid的 day bar update协议 数据.
	*/
	BOOL g_Init_C2_UF_Day_Bid( STProtocol_C2_UFDAY& data, DWORD chlID, DWORD date, char* pItemCode, float open, float high, float low, float close, double share, double turnover ) ;

		/*
			添充 Bid的 day bar SnapShot协议 数据.
		*/
		BOOL g_Init_C2_LF_Day_Bid( STProtocol_C2_LFDAY& data, double UserID, DWORD date, char* pItemCode, float open, float high, float low, float close, double share, double turnover ) ;

		/*
			添充 Bid的 day bar SnapShot协议 数据( 无数据情况 ) 'td' 
		*/
		BOOL g_Init_C2_td_Day_Bid( STProtocol_C2_LFDAY& data, double UserID, DWORD date, char* pItemCode ) ;


	/*
		添充 Ask的 day bar update协议 数据.
	*/
	BOOL g_Init_C2_UF_Day_Ask( STProtocol_C2_UFDAY& data, DWORD chlID, DWORD date, char* pItemCode, float open, float high, float low, float close, double share, double turnover ) ;

		/*
			添充 Ask的 day bar SnapShot协议 数据.
		*/
		BOOL g_Init_C2_LF_Day_Ask( STProtocol_C2_LFDAY& data, double UserID, DWORD date, char* pItemCode, float open, float high, float low, float close, double share, double turnover ) ;

		/*
			添充 Ask的 day bar SnapShot协议 数据( 无数据情况 ) 'td' 
		*/
		BOOL g_Init_C2_td_Day_Ask( STProtocol_C2_LFDAY& data, double UserID, DWORD date, char* pItemCode ) ;

	/*
		添充 nominal的 day bar update协议 数据.
	*/
	BOOL g_Init_C2_UF_Day_Nominal( STProtocol_C2_UFDAY& data, DWORD chlID, DWORD date, char* pItemCode, float open, float high, float low, float close, double share, double turnover ) ;

		/*
			添充 nominal的 day bar SnapShot协议 数据.
		*/
		BOOL g_Init_C2_LF_Day_Nominal( STProtocol_C2_LFDAY& data, double UserID, DWORD date, char* pItemCode, float open, float high, float low, float close, double share, double turnover ) ;

		/*
			添充 nominal的 day bar SnapShot协议 数据( 无数据情况 ) 'td' 
		*/
		BOOL g_Init_C2_td_Day_Nominal( STProtocol_C2_LFDAY& data, double UserID, DWORD date, char* pItemCode ) ;


struct STProtocol_C2_RECORDDT		//TradeTickD struct
{
	float	Price ;
};

/*
	price协议 更新数据包
*/
struct STProtocol_C2_UFDT
{
	STProtocol_SR_UD		chlDataHead ;		// channel data head( W2t外层协议 )			

	STProtocol_C2_UFHEAD	Head;				// 'C2'协议头
	STProtocol_C2_RECORDDT	Records;			// 记录

	WORD					eleED ;				// 'ED' 

};

/*
	price协议 snapshot数据包
*/
struct STProtocol_C2_LFDT
{
	STSERVERREQ				HisDataHead ;		// his data head( W2t外层协议 )			

	STProtocol_C2_UFHEAD	Head;				// 'C2'协议头
	STProtocol_C2_RECORDDT	Records;			// 记录

	WORD					eleED ;				// 'ED' 

};


/*
	添充 prvClose的 SnapShot协议 数据.
*/
BOOL g_Init_C2_LFDT_prvClose( STProtocol_C2_LFDT& data, double UserID, DWORD date, char* pItemCode, float val ) ;

/*
	添充 prvClose的 update协议 数据.
*/
BOOL g_Init_C2_UFDT_prvClose( STProtocol_C2_UFDT& data, DWORD chlID, DWORD date, char* pItemCode, float val ) ;

struct STProtocol_C2_RECORDABN
{
	WORD	Time ;		// hhmmss packed.
	float	Price ;
	WORD	PriceId;
};

/*
	通道广播数据包: 'C2' - 'UF' 的 price数据.
*/
struct STProtocol_C2_UFABN
{
	STProtocol_SR_UD		chlDataHead ;		// channel data head( W2t外层协议 )			

	STProtocol_C2_UFHEAD	Head;				// 'C2'协议头
	STProtocol_C2_RECORDABN Records;			// 记录

	WORD					eleED ;				// 'ED' 

};

/*
	UC请求应答数据包: 'C2' - 'LF' 的 price数据.
*/
struct STProtocol_C2_LFABN
{
	STSERVERREQ				HisDataHead ;		// his data head( W2t外层协议 )			

	STProtocol_C2_UFHEAD	Head;				// 'C2'协议头
	STProtocol_C2_RECORDABN Records;			// 记录

	WORD					eleED ;				// 'ED' 

};


/*
	添充 tick update协议 数据.
*/
BOOL g_Init_C2_UFABN( STProtocol_C2_UFABN& data, DWORD chlID, DWORD date, char* pItemCode, WORD tm, float val, WORD ID ) ;


	/*
		添充 nominal的 tick update协议 数据.
	*/
	BOOL g_Init_C2_UFABN_Nominal( STProtocol_C2_UFABN& data, DWORD chlID, DWORD date, char* pItemCode, WORD tm, float val, WORD ID ) ;


	/*
		添充 bid的 tick update协议 数据.
	*/
	BOOL g_Init_C2_UFABN_Bid( STProtocol_C2_UFABN& data, DWORD chlID, DWORD date, char* pItemCode, WORD tm, float val, WORD ID ) ;

	/*
		添充 ask的 tick update协议 数据.
	*/
	BOOL g_Init_C2_UFABN_Ask( STProtocol_C2_UFABN& data, DWORD chlID, DWORD date, char* pItemCode, WORD tm, float val, WORD ID ) ;

/*
	添充 tick snapshot协议 数据.
*/
BOOL g_Init_C2_LFABN( STProtocol_C2_LFABN& data, double UserID, DWORD date, char* pItemCode, WORD tm, float val, WORD ID ) ;

		/*
			添充 nominal的 tick Snapshot协议 数据.
		*/
		BOOL g_Init_C2_LFABN_Nominal( STProtocol_C2_LFABN& data, double UserID, DWORD date, char* pItemCode, WORD tm, float val, WORD ID ) ;

			/*
				添充 nominal的 tick Snapshot协议 数据( 无数据情况 ) 'td' 
			*/
			BOOL g_Init_C2_tdABN_Nominal( STProtocol_C2_LFABN& data, double UserID, DWORD date, char* pItemCode ) ;

		/*
			添充 bid的 tick Snapshot协议 数据.
		*/
		BOOL g_Init_C2_LFABN_Bid( STProtocol_C2_LFABN& data, double UserID, DWORD date, char* pItemCode, WORD tm, float val, WORD ID ) ;

			/*
				添充 bid的 tick Snapshot协议 数据( 无数据情况 )
			*/
			BOOL g_Init_C2_tdABN_Bid( STProtocol_C2_LFABN& data, double UserID, DWORD date, char* pItemCode ) ;

		/*
			添充 ask的 tick Snapshot协议 数据.
		*/
		BOOL g_Init_C2_LFABN_Ask( STProtocol_C2_LFABN& data, double UserID, DWORD date, char* pItemCode, WORD tm, float val, WORD ID ) ;

			/*
				添充 ask的 tick Snapshot协议 数据( 无数据情况 ) 'td' 
			*/
			BOOL g_Init_C2_tdABN_Ask( STProtocol_C2_LFABN& data, double UserID, DWORD date, char* pItemCode ) ;


//数据发送协议数据定义
//C2 protocol

/*
	'C2' DIAO协议头.

*/
struct STProtocol_C2_Head
{
	WORD	size;
	WORD	flag;		//'C2'
	BYTE	GCode;
	DOUBLE	ICode;
	BYTE	PriceType;	//0-Nominal 1-bid 2-ask 3-B&a 4-Trade Tick
	BYTE	PeriodType;	//0-Tick 1-min 2-half hour 3-day 4-week 5-mouth 6-year
	WORD	PeriodArg;
	WORD	SubType;	//'UC'UD''RQ'

};

// his server请求的头.
struct STProtocol_C2_HisHead
{
	BYTE	DType;			// 未用  ( 'H'- Historical, 'U' - Update channel )
	DWORD	SpanDate;		// Request how many trans date	
	WORD	SpanTime;		// 
	DWORD	EndDate;		// end date format YYYYMMDD 
	WORD	EndTime;		//
	WORD	RecsTotal;		//

};

/*
	rq--tick的head.
*/
struct CHART_rqtick_HEAD
{
	DWORD date ;			// tick日期.
	BYTE  ver ;				// 当天数据版本.	
	int	  StartID ;			// 第一个tickID.
	int   EndID ;			// 最后一个tickID.
	int	  TickSize ;		// tick个数.

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// 注释: 光靠上面的校验信息还不足够, 加上 FirstTick,LastTick相对安全很多( paul, 2007-8-2, fix bug. )

	WORD  FirstTickTime ;	//	
	float FirstTickPrice ;	// 第一笔的信息( 时间+价格 )

	WORD  LastTickTime ;	//
	float LastTickPrice ;	// 最后一笔信息( 时间+价格 )

} ;

/*
	rq--Minute的校验信息
*/
struct CHART_rqMinute_HEAD
{
	BYTE  	ver ;			// 本地数据版本.	
	DWORD	BeginDate ;		// 第一天数据日期.	
	DWORD	EndDate ;		// 最后一天数据日期.	
	DWORD	DayCount ;		// 总的天数.
	DWORD	RecCount ;		// 总的记录数.

	DWORD	CRC32 ;			// 记录数组的 CRC32( paul, 2008-1-24, add, 因为 ver还不够安全. )

} ;

/*
	rq--Day的校验信息
*/
struct CHART_rqDay_HEAD
{
	BYTE  	ver ;			// 本地数据版本.	
	DWORD	BeginDate ;		// 第一天数据日期.	
	DWORD	EndDate ;		// 最后一天数据日期.	
	DWORD	RecCount ;		// 总的记录数.

	DWORD	CRC32 ;			// 记录数组的 CRC32( paul, 2008-1-24, add, 因为 ver还不够安全. )

} ;
//Johnny 2015.04.23 rn协议头
/*
	rq--Day的校验信息
*/
struct CHART_rnItem_HEAD
{
	BYTE  	ver ;			// 本地数据版本.  0x0f	
	DWORD	BeginDate ;		// 请求数据日期.	
} ;

/*
   切割包的标记.
*/
struct INCISE_PKG_INFO
{
    int 	PkgID ;	  	// 切割包的标号( 从1开始, 编号是连续的. ) 
    BYTE 	finish ;	// 包结束了吗?   0--未结束, 1--结束了.

} ;

/*
	发向door的 通道连接和断开包.

*/
struct STProtocol_SR_CI
{
	//W2TPKGHAED				netHead ;

	//WORD	ReqIdNum;			//请求服务器个数
	//DOUBLE	ReqID;				//请求服务器ID

	WORD	BegFlag;			//开始标志'WS'
	DWORD	Length;
	WORD	ReqMode;			//'UC'/'UD'/'RQ'
	WORD	ReqType;			//'CT'/'TE'
	WORD	UiFlag;				//'UI'
	DOUBLE	UserID;				//userID
	WORD	CiFlag;				//'CI'
	DWORD	CannalID;			//CannalID
	WORD	EdFlag;				//'ED'
};

/*
	添充 通道连接和断开协议 数据.
*/
BOOL g_Init_SR_CI( STProtocol_SR_CI& data, WORD reqMode, WORD reqType, double userID, DWORD channelID ) ;

// trade tick数据.
struct STProtocol_C2_RECORDTT
{
	BYTE	byType;			//类型' ' and 'Y' 需要处理
	WORD	TickTime;		//时间
	float	Price;			//价位
	DWORD	Shares;			//成交量
	WORD	TtickId;		//TradeTick编号
};

struct STProtocol_C2_UFTT
{
	STProtocol_SR_UD		chlDataHead ;		// channel data head( W2t外层协议 )			

	STProtocol_C2_UFHEAD	Head;				// 'C2'协议头
	STProtocol_C2_RECORDTT Records;				// 记录

	WORD					eleED ;				// 'ED' 

};

struct STProtocol_C2_LFTT
{
	STSERVERREQ				HisDataHead ;		// his data head( W2t外层协议 )			

	STProtocol_C2_UFHEAD	Head;				// 'C2'协议头
	STProtocol_C2_RECORDTT Records;				// 记录

	WORD					eleED ;				// 'ED' 

};

/*
	添充 tradetick update协议 数据( 'LF' )
*/
BOOL g_Init_C2_UFTT( STProtocol_C2_UFTT& data,			// 返回的 数据包
						DWORD chlID,					// 通道号
						DWORD date,						// 日期
						char* pItemCode,				// 证券代码
						BYTE type, WORD tm, float price, DWORD share, WORD tickID ) ;	// 记录

/*
	添充 tradetick snapshot协议 数据( 'LF' )
*/
BOOL g_Init_C2_LFTT( STProtocol_C2_LFTT& data,			// 返回的 数据包
						double UserID,					// user id.
						DWORD date,						// 日期
						char* pItemCode,				// 证券代码
						BYTE type, WORD tm, float price, DWORD share, WORD tickID ) ;	// 记录

/*
	添充 tradetick snapshot协议 数据( 无数据情况 ) ( 'td' )
*/
BOOL g_Init_C2_tdTT( STProtocol_C2_LFTT& data,			// 返回的 数据包
						double UserID,					// user id.
						DWORD date,						// 日期
						char* pItemCode ) ;				// 证券代码

/*
	填充 应答数据的 压缩包.
*/
BOOL g_Init_AnswerCompactPkg( BYTE* pBuf, int& bufSize,				// 返回 数据区
							  double	userID,						// user ID							  
							  DWORD		SrcBufLen,					// 压缩前数据长度.
							  DWORD		crc32,						// crc32
							  BYTE*		pCompactData,				//	
							  int		compactDataSize ) ;			// 压缩数据			

// his server的应答头.
struct STProtocol_C2_ResponseHisHead
{
	DWORD	SpanDate;		// Request how many trans date	
	WORD	SpanTime;		// 
	DWORD	EndDate;		// end date format YYYYMMDD 
	WORD	EndTime;		//
	WORD	RecsTotal;		//

};

// 'C2'--'HT'应答包是:
//  STSERVERREQ + STProtocol_C2_Head + STProtocol_C2_ResponseHisHead + RecordRQ + N*recs.

/*
	填充 'C2' - 'HT' 的ticks(price)数据协议.
*/
struct PRICETICK ;
struct TRADETICK ;
struct BIDASKTICK ;
BOOL g_Init_C2_HT_Tick_Price( BYTE* pBuf, int& bufSize,					// 返回 数据区
							  double	userID,							// user ID	
							  char* pItemCode, DWORD date,				// itemcode&日期
							  PRICETICK* pArray, int arraySize,			// ticks序列
							  int priceType,							// price类型	
							  int& echoDIAOOffset, int& echoDIAOSize	) ;	// 返回的 DIAO区 位置和大小							

	BOOL g_Init_C2_HT_Tick_Price_Nominal( BYTE* pBuf, int& bufSize,			// 返回 数据区
								  double	userID,							// user ID	
								  char* pItemCode, DWORD date,				// itemcode&日期
								  PRICETICK* pArray, int arraySize,			// ticks序列
								  int& echoDIAOOffset, int& echoDIAOSize	) ;	// 返回的 DIAO区 位置和大小							

	BOOL g_Init_C2_HT_Tick_Price_Bid( BYTE* pBuf, int& bufSize,				// 返回 数据区
								  double	userID,							// user ID	
								  char* pItemCode, DWORD date,				// itemcode&日期
								  PRICETICK* pArray, int arraySize, 		// ticks序列
								  int& echoDIAOOffset, int& echoDIAOSize ) ;	// 返回的 DIAO区 位置和大小							

	BOOL g_Init_C2_HT_Tick_Price_Ask( BYTE* pBuf, int& bufSize,				// 返回 数据区
								  double	userID,							// user ID	
								  char* pItemCode, DWORD date,				// itemcode&日期
								  PRICETICK* pArray, int arraySize,			// ticks序列
								  int& echoDIAOOffset, int& echoDIAOSize ) ; // 返回的 DIAO区 位置和大小							

/*
	填充 'C2' - 'ht' 的ticks(price)数据协议.
*/
BOOL g_Init_C2_ht_Tick_Price( BYTE* pBuf, int& bufSize,					// 返回 数据区
							  double	userID,							// user ID	
							  char* pItemCode, CHART_rqtick_HEAD& head,	// itemcode&ver info
							  PRICETICK* pArray, int arraySize,			// ticks序列
							  int priceType,							// price类型	
							  int& echoDIAOOffset, int& echoDIAOSize	) ;	// 返回的 DIAO区 位置和大小							

	BOOL g_Init_C2_ht_Tick_Price_Nominal( BYTE* pBuf, int& bufSize,			// 返回 数据区
								  double	userID,							// user ID	
								  char* pItemCode, CHART_rqtick_HEAD& head,	// itemcode&ver info
								  PRICETICK* pArray, int arraySize,			// ticks序列
								  int& echoDIAOOffset, int& echoDIAOSize	) ;	// 返回的 DIAO区 位置和大小							

	BOOL g_Init_C2_ht_Tick_Price_Bid( BYTE* pBuf, int& bufSize,				// 返回 数据区
								  double	userID,							// user ID	
								  char* pItemCode, CHART_rqtick_HEAD& head,	// itemcode&ver info
								  PRICETICK* pArray, int arraySize, 		// ticks序列
								  int& echoDIAOOffset, int& echoDIAOSize ) ;	// 返回的 DIAO区 位置和大小							

	BOOL g_Init_C2_ht_Tick_Price_Ask( BYTE* pBuf, int& bufSize,				// 返回 数据区
								  double	userID,							// user ID	
								  char* pItemCode, CHART_rqtick_HEAD& head,	// itemcode&ver info
								  PRICETICK* pArray, int arraySize,			// ticks序列
								  int& echoDIAOOffset, int& echoDIAOSize ) ; // 返回的 DIAO区 位置和大小							

BOOL g_Init_C2_ht_Tick_TradeTick(	BYTE* pBuf, int& bufSize,					// 返回 数据区
									double	userID,								// user ID	
									char* pItemCode, CHART_rqtick_HEAD& head,	// itemcode&ver info
									TRADETICK* pArray, int arraySize,			// ticks序列
									int& echoDIAOOffset, int& echoDIAOSize	) ;	// 返回的 DIAO区 位置和大小							

BOOL g_Init_C2_ht_Tick_BidAsk(	BYTE* pBuf, int& bufSize,					// 返回 数据区
									double	userID,								// user ID	
									char* pItemCode, CHART_rqtick_HEAD& head,	// itemcode&ver info
									BIDASKTICK* pArray, int arraySize,			// ticks序列
									int& echoDIAOOffset, int& echoDIAOSize	) ;	// 返回的 DIAO区 位置和大小							

class CMinuteDayData ;
BOOL g_Init_C2_ht_Minute( BYTE* pBuf, int& bufSize,							// 返回 数据区
						  double	userID,									// user ID	
						  char* pItemCode, int PriceType, int PeriodType,	// 数据类型
						  CHART_rqMinute_HEAD& ServerVerifyInfo,			// Server端的数据校验信息.
						  INCISE_PKG_INFO&	   InciseInfo,					// 切割信息.		
						  CMinuteDayData* *pArray, int ArraySize,			// 返回数据
						  int& echoDIAOOffset, int& echoDIAOSize	) ;		// 返回的 DIAO区 位置和大小							

struct HF_DAYBAR ;
BOOL g_Init_C2_ht_Day(	BYTE* pBuf, int& bufSize,							// 返回 数据区
						  double	userID,									// user ID	
						  char* pItemCode, int PriceType,					// 数据类型
						  CHART_rqDay_HEAD& ServerVerifyInfo,				// Server端的数据校验信息.
						  HF_DAYBAR*	pArray, int ArraySize,				// 返回数据
						  int& echoDIAOOffset, int& echoDIAOSize	) ;		// 返回的 DIAO区 位置和大小							


/*
	填充 'C2' - 'HT' 的ticks(bid&ask)数据协议.
*/
struct BIDASKTICK ;
BOOL g_Init_C2_HT_Tick_BidAsk( BYTE* pBuf, int& bufSize,				// 返回 数据区
							  double	userID,							// user ID	
							  char* pItemCode, DWORD date,				// itemcode&日期
							  BIDASKTICK* pArray, int arraySize,		// ticks序列
							  int& echoDIAOOffset, int& echoDIAOSize ) ;	// 返回的 DIAO区 位置和大小							

/*
	填充 'C2' - 'HT' 的ticks(trade tick)数据协议.
*/
struct TRADETICK ;
BOOL g_Init_C2_HT_Tick_TradeTick( BYTE* pBuf, int& bufSize,				// 返回 数据区
							  double	userID,							// user ID	
							  char* pItemCode, DWORD date,				// itemcode&日期
							  TRADETICK* pArray, int arraySize,			// ticks序列
							  int& echoDIAOOffset, int& echoDIAOSize ) ;// 返回的 DIAO区 位置和大小							

/*
	DV记录
*/
struct STProtcol_C2_DV
{
	DWORD	CurrDate;		//当前日期
	DWORD	BeforeDate;		//前一交易日日期

	WORD	AskTickNum;		//Ask Price 个数
	BYTE	AskTickVer;		//Ask Price ver版本号start from 1 0 = No data
	WORD	AskMin1Num;		//Ask 1 Min 个数
	BYTE	AskMin1Ver;		//Ask 1 Min ver版本号start from 1 0 = No data
	WORD	AskMin15Num;	//Ask 15 Min 个数
	BYTE	AskMin15Ver;	//Ask 15 Min ver版本号start from 1 0 = No data

	WORD	BidTickNum;		//Bid Price 个数
	BYTE	BidTickVer;		//Bid Price ver版本号start from 1 0 = No data
	WORD	BidMin1Num;		//Bid 1 Min 个数
	BYTE	BidMin1Ver;		//Bid 1 Min ver版本号start from 1 0 = No data
	WORD	BidMin15Num;	//Bid 15 Min 个数
	BYTE	BidMin15Ver;	//Bid 15 Min ver版本号start from 1 0 = No data

	WORD	NominalTickNum;		//Nominal Price 个数
	BYTE	NominalTickVer;		//Nominal Price ver版本号start from 1 0 = No data
	WORD	NominalMin1Num;		//Nominal 1 Min 个数
	BYTE	NominalMin1Ver;		//Nominal 1 Min ver版本号start from 1 0 = No data
	WORD	NominalMin15Num;	//Nominal 15 Min 个数
	BYTE	NominalMin15Ver;	//Nominal 15 Min ver版本号start from 1 0 = No data

	WORD	TradeTickNum;		//Trade Price 个数
	BYTE	TradeTickVer;		//Trade Price ver版本号start from 1 0 = No data
	WORD	TradeMin1Num;		//Trade 1 Min 个数
	BYTE	TradeMin1Ver;		//Trade 1 Min ver版本号start from 1 0 = No data
	WORD	TradeMin15Num;		//Trade 15 Min 个数
	BYTE	TradeMin15Ver;		//Trade 15 Min ver版本号start from 1 0 = No data

	WORD	AskBidTickNum;		//Bid Price 个数
	BYTE	AskBidTickVer;		//Bid Price ver版本号start from 1 0 = No data

	BYTE	TradeTimeVer;		//交易时间段版本
};

/*
	填充 'C2' - 'HT' 的Day数据协议.
*/
struct HF_DAYBAR ;
BOOL g_Init_C2_HT_Day( BYTE* pBuf, int& bufSize,						// 返回 数据区
							  double	userID,							// user ID	
							  char*		pItemCode, 
							  int		priceType,
							  BYTE		ver,
							  HF_DAYBAR* pArray, int arraySize,			// bar序列
							  int& echoDIAOOffset, int& echoDIAOSize ) ;	// 返回的 DIAO区 位置和大小							

/*
	填充 'C2' - 'DV' 的协议.
*/
class CDateInfo ;
BOOL g_Init_C2_DV(  BYTE*			pBuf,	int& bufSize,						// 返回 数据区
					double			userID,										// user ID	
					char*			pItemCode, 
					CDateInfo*	*	pArray, int arraySize,						// bar序列
					int& echoDIAOOffset, int& echoDIAOSize ) ;					// 返回的 DIAO区 位置和大小							

/*
	填充 'C2' - 'TD' 的协议.
*/
class CDateInfo ;
BOOL g_Init_C2_TD(  BYTE*			pBuf,	int& bufSize,						// 返回 数据区
					double			userID,										// user ID	
					LPCSTR			pItemCode, 
					CDateInfo*	*	pArray, int arraySize,						// bar序列
					int& echoDIAOOffset, int& echoDIAOSize ) ;					// 返回的 DIAO区 位置和大小							

/*
	填充 'C2' - 'HT' 的分钟图数据协议.
*/
BOOL g_Init_C2_HT_Minute( BYTE* pBuf, int& bufSize,						// 返回 数据区
							  double	userID,							// user ID	
							  char*		pItemCode, 
							  int		priceType,
							  int		periodArg,
							  CMinuteDayData* * pArray, int arraySize,	// day序列
							  int& echoDIAOOffset, int& echoDIAOSize ) ;// 返回的 DIAO区 位置和大小		
					
/*
	填充 'C2' - 'rn' 的夜市数据协议.
*/
//Johnny 2015.04.23 新增协议'rn'	用于发送XItem日市等数据

struct XItemInfo
{
	char bOpenDayMarket;
	char bOpenNightMarket;
	char bHouseKeeping;
	DWORD	date ;	// YYYYMMDD
	float	open ;	
	float	high ;	
	float	low ;	
	float	close ;	
	double	share ;
	double	turnover ;
	char	temp[21] ;	// reserve.	
};//sizeof 52
BOOL g_Init_C2_hn_Day(	BYTE* pBuf, int& bufSize,						// 返回 数据区
					  double	userID,									// user ID	
					  char* pItemCode, int PriceType,					// 数据类型
					  XItemInfo*	pArray, int ArraySize,				// 返回数据
					  int& echoDIAOOffset, int& echoDIAOSize	) ;		// 返回的 DIAO区 位置和大小							

//Johnny 2015.04.23 收到'L' 自动发送给client

// 'C2'协议头
struct STProtocol_C2_UXHEAD
{
	WORD	size;
	WORD	flag;		//'C2'
	BYTE	GCode;
	DOUBLE	ICode;
	BYTE	PriceType;	//0-Nominal 1-bid 2-ask 3-B&a 4-Trade Tick
	BYTE	PeriodType;	//0-Tick 1-min 2-half hour 3-day 4-week 5-mouth 6-year
	WORD	PeriodArg;
	WORD	SubFlag;	//'UF'
};

/*
	通道广播数据包: 'C2' - 'UX' 的 Last price数据.
*/
struct STProtocol_C2_UXABN
{
	STProtocol_SR_UD		chlDataHead ;		// channel data head( W2t外层协议 )			

	STProtocol_C2_UXHEAD	Head;				// 'C2'协议头
	XItemInfo				Records;			// 记录

	WORD					eleED ;				// 'ED' 
};
/*
	添充 XItem 日市Last price协议 数据.
*/
BOOL g_Init_C2_UXABN( STProtocol_C2_UXABN& data, DWORD chlID, DWORD date, char* pItemCode, WORD tm, XItemInfo xitem) ;





#pragma pack(8)


//andy add for news 20070621

//060510_01.MDS里有news超过56000的限制，加大一倍 andy 20060529
//#define	BUFFERSIZE 	112000u
#define	BUFFERSIZE 	56000u
//bill说 news加大size後，client讀不到 andy 20060731

#define MAX_Group_Count 256
#define MAX_SubGroup_Count 256

#define MAX_W2TUP_BUFF		(2*1024*1024)		//W2T Update frame 最大的Size
//johnny
//#define MAX_OUTFRAME_SIZE	(10*1024)			//最大的数据发送字节，相当于60天的HIFC tick数据，每一天有5000Tick.
#define MAX_OUTFRAME_SIZE	(2*1024*1024)			//最大的数据发送字节，相当于60天的HIFC tick数据，每一天有5000Tick.

#define MAX_OUTFILE_SIZE	(1000*1024)			//最大的发送文件长度

#define REQUEST_OP			'OP'  //option
#define REQUEST_NE			'NE'  //news
#define REQUEST_SY			'SY'
#define REQUEST_VR			'VR'
#define REQUEST_RF			'RF'
#define REQUEST_RM			'RM'
#define REQUEST_RI			'RI'

#define STATUS_Active		_T('A')  //
#define STATUS_Suspended	_T('S')  //暂停
#define STATUS_Expire		_T('E')  //已过期

#define W2T_STATUS_Active		'PS' 
#define W2T_STATUS_Suspended	'OD' 
#define W2T_STATUS_Expire		'FB' 

#define MAX_USER_SUM    		 10 

//for 201 frame
#define STX (0x02)
#define ETX (0x03)
#define DLE (0x10)

#define WQ_SYSTEM_ID	(65535)
#define WQ_EXTENDSYSTEM_ID	(0x44000001)		//bit 30,26,0 is 1

#include <atldbcli.h>
//#include "wq.h"


#pragma pack(1)

struct xItemDAYDATAA
{
	float	open ;
	float	high ;
	float	low ;
	float	close ;
	
} ;

struct xItemSNAPSHOT   //结构和REPORTSNAPSHOT一样，保存夜市数据
{
	WORD	lastTime;		// 最后接收到的tick的接收时间
	
	float	prvClose;		//昨收价
	double  share ;			//股数
	double  turnover ;		//金额
	
	xItemDAYDATAA  dayDataN ;	// day图信息(nominal)
	xItemDAYDATAA  dayDataA ;	// day图信息(ask)
	xItemDAYDATAA  dayDataB ;	// day图信息(bid)
	
	float	RealNominal	;	//真实按盘价( dayDataN.close因为 做图原因, 去掉了开盘前的nominal价格, 这在recode引起问题. )
	
} ;

struct NightMarketItemData
{   //andy add 2013.05.07 夜市期货,T+1 data for x items 
	char szItem[10];
	char szXItem[10];
	DWORD dwLastDate;   
	xItemSNAPSHOT snap;
	
	NightMarketItemData()
	{
		Init();
	}

	Init()
	{
		memset(szItem,0,sizeof(szItem));		
		memset(szXItem,0,sizeof(szXItem));		
		dwLastDate = 0;
		memset(&snap,0,sizeof(snap));	
	}
	
	const void operator=(const NightMarketItemData& Data)
	{
		memcpy(szItem,Data.szItem,sizeof(szItem));
		memcpy(szXItem,Data.szXItem,sizeof(szXItem));
		dwLastDate = Data.dwLastDate;
		memcpy(&snap,&Data.snap,sizeof(snap));	
	}
} ;

typedef CArray< NightMarketItemData, NightMarketItemData & > CArrayNightMarketItemData ;


struct NightMarketItem
{   //andy add 2013.03.28 夜市期货
	char szItemSrc[10];
	char szItemDest[10];   //xitem
	char szItemZ[10];      //zItem
	DWORD dwLastDate;   //删除做copy操作的时间
	
	NightMarketItem()
	{
		Init();
	}

	Init()
	{
		memset(szItemSrc,0,sizeof(szItemSrc));
		memset(szItemDest,0,sizeof(szItemDest));
		memset(szItemZ,0,sizeof(szItemZ));
		dwLastDate = 0;
	}

	const void operator=(const NightMarketItem& Data)
	{
		memcpy(szItemSrc,Data.szItemSrc,sizeof(szItemSrc));
		memcpy(szItemDest,Data.szItemDest,sizeof(szItemDest));
		memcpy(szItemZ,Data.szItemZ,sizeof(szItemZ));
		dwLastDate = Data.dwLastDate;
	}
} ;

typedef CArray< NightMarketItem, NightMarketItem & > CArrayNightMarketItem ;


struct STProtocol_SR_RQ
{
	WORD	ReqIdNum;			//请求服务器个数
	DOUBLE	ReqID;				//请求服务器ID
	WORD	BegFlag;			//开始标志'WS'
	DWORD   Length;
	WORD	DFFlag;
	WORD	DataOff;			
	WORD	ReqMode;			//'UC'/'UD'/'RQ'
	WORD	ReqType;			//'CT'/'TE'/'SY'
	WORD	UIFlag;				//'UI'
	DOUBLE	UserID;				//userID
	WORD	DAFlag;				//分割标志
};

struct STProtocol_SR_RET		//Server传回数据
{
//  andy modify 2007.06.28
	//WORD	ReqIdNum;			//请求服务器个数
	//DOUBLE	ReqID;			//请求服务器ID
	DWORD	   headMark ;		// 'AA55'	
	DWORD	   dataSize ;	    //发送的数据长度，不包括head，size字段本身。
//  andy modify 2007.06.28
	WORD	BegFlag;			//开始标志'WS'
	DWORD   Length;
	WORD	DFFlag;
	WORD	DataOff;			
	WORD	UIFlag;				//'UI'
	DOUBLE	UserID;				//userID
	WORD	DAFlag;				//分割标志
};

struct STProtocol_SR_RET2		//Server传回即时数据
{
//  andy modify 2007.06.28
	//WORD	ReqIdNum;			//请求服务器个数
	//DOUBLE	ReqID;			//请求服务器ID
	DWORD	   headMark ;		// 'AA55'	
	DWORD	   dataSize ;	    //发送的数据长度，不包括head，size字段本身。
//  andy modify 2007.06.28
	WORD	BegFlag;			//开始标志'WS'
	DWORD   Length;
	WORD	DFFlag;
	WORD	DataOff;			
	WORD	CIFlag;				//'CI'
	DWORD	ChannelID;			//channelID
	WORD	DAFlag;				//分割标志
};

struct STProtocol_VR_RET		//Server传回数据
{
//  andy modify 2007.06.28
	//WORD	ReqIdNum;			//请求服务器个数
	//DOUBLE	ReqID;			//请求服务器ID
	DWORD	   headMark ;		// 'AA55'	
	DWORD	   dataSize ;	    //发送的数据长度，不包括head，size字段本身。
//  andy modify 2007.06.28
	WORD	BegFlag;			//开始标志'WS'
	DWORD   Length;
	WORD	UIFlag;				//'UI'
	DOUBLE	UserID;				//userID
	WORD    StatusFlag;
	WORD	Status;
	WORD	EndFlag;
};

struct STProtocol_DO_HEAD
{
	WORD	wLen;
	WORD	DOFlag;			//='DO';	
	unsigned char cbGroupCode;
	char          szItemCode[8];
	long		  lTransdate;
	float         fStrikePrice;
	unsigned char cbOrder;
	WORD	SubFlag;		//
};

struct STProtocol_UC_RET		//Server传回通道ID
{
//  andy modify 2007.06.28
	//WORD	ReqIdNum;			//请求服务器个数
	//DOUBLE	ReqID;			//请求服务器ID
	DWORD	   headMark ;		// 'AA55'	
	DWORD	   dataSize ;	    //发送的数据长度，不包括head，size字段本身。
//  andy modify 2007.06.28
	WORD	BegFlag;			//开始标志'WS'
	DWORD   Length;
	WORD	ReqMode;			//'UC'/'UD'/'RQ'
	WORD	ReqType;			//'CT'/'TE'/'NE'
	WORD	UIFlag;				//'UI'
	DOUBLE	UserID;				//userID
	WORD    CIFlag;
	DWORD	ChannelID;
	WORD	EndFlag;
};

struct STProtocol_UD_RET		//Server通知通道断开
{
//  andy modify 2007.06.28
	//WORD	ReqIdNum;			//请求服务器个数
	//DOUBLE	ReqID;			//请求服务器ID
	DWORD	   headMark ;		// 'AA55'	
	DWORD	   dataSize ;	    //发送的数据长度，不包括head，size字段本身。
//  andy modify 2007.06.28
	WORD	BegFlag;			//开始标志'WS'
	DWORD   Length;
	WORD	ReqMode;			//'CD'
	WORD	ReqType;			//'CT'/'TE'/'NE'
	WORD    CIFlag;
	DWORD	ChannelID;
	WORD	EndFlag;
};


struct STProtocol_DN_HEAD
{
	WORD	wLen;
	WORD	DNFlag;			//='DN';
	DWORD	Date;
	CHAR	GroupCode;
	CHAR	SubGroupCode;
	WORD	SubFlag;		//
};

struct STProtocol_DN_HL_HEAD
{	
	BYTE	flag;	//'U'=Update,'H'=History,'S'=Search
	WORD	TotalNum;
	WORD	NewsId;
	WORD	Time;
	DWORD	Date;
	WORD	StringLen;
};

struct STProtocol_DN_HL_HEAD2
{	
//	BYTE	flag;	//'U'=Update,'H'=History,'S'=Search
//	WORD	TotalNum;
	WORD	NewsId;
	WORD	Time;
	DWORD	Date;
	WORD	StringLen;
};

struct STProtocol_DN_RQ_HEAD
{	
	CHAR	ReqType;
	CHAR	HeadlineIndicator:1;
	CHAR	reserved:7;
	WORD    StartID;
	WORD	EndID;
};

struct STProtocol_DN_TS_HEAD
{	
	DWORD StartDate;
	WORD  StartTime;
	DWORD EndDate;
	WORD  EndTime;
	WORD  SearchFlag;
	WORD  KeyNum;	
};


typedef struct _tagPACKET
{
  char *buffer;
  unsigned long size;
} PACKET;

//struct _tagTTDataFrame{
//	int nLength;
//	time_t lTime;
//	BYTE btGroupCode;
//	char arItemCode[8];
//	BYTE btTransCode;
//	int nTransDataLength;
//	void * pTransData;
//};

typedef struct _StoryBuf
{	//for 149
	unsigned char NewsCode;						// 1
	unsigned short NewsId:12;
	unsigned short reserved:2;
	unsigned short FinalSegmentFlag:1;
	unsigned short CancelNewsFlag:1;	// 3
	short ReleaseTime;					//5

	unsigned short SegmentNo;
	unsigned short TotalLineNo;
	unsigned short NewsSize;
	
	unsigned short ContentSize;
	char Content[1000];

	char Language;
	char GroupCode;
}StoryBuf;

struct Frame149
{
	char GroupCode;
	long TransDate;
	unsigned char NewsCode;						// 1
	unsigned short NewsId:12;
	unsigned short reserved:2;
	unsigned short FinalSegmentFlag:1;
	unsigned short CancelNewsFlag:1;	// 3
	short ReleaseTime;					//5
	unsigned short TotalLineNo;
	char HeadlineText[164];
	char Language;
	unsigned short idx;
	CString TextLine;

	const void operator=(const Frame149& Data)
	{
		GroupCode = Data.GroupCode ;
		TransDate = Data.TransDate ; 
		NewsCode = Data.NewsCode ;						// 1
		NewsId = Data.NewsId ;
		FinalSegmentFlag = Data.FinalSegmentFlag ;
		CancelNewsFlag = Data.CancelNewsFlag ;	// 3
		ReleaseTime = Data.ReleaseTime ;					//5	
		TotalLineNo = Data.TotalLineNo ;			
		memcpy(HeadlineText,Data.HeadlineText,164);
		Language = Data.Language ;	
		TextLine = Data.TextLine ;
		idx = Data.idx ;
	}
};


typedef struct _NewsBuf
{	//for 131			
	char NewsGroupCode;			// subgroupcode
	long ActualDate;			// 5
	short ActualTime;			// 7
	unsigned short StoryNo;		// 9
	unsigned char useversion:1;
	unsigned char vesionline:1;
	unsigned char itemid:1;
	unsigned char headline:1;
	unsigned char headlineonly:1;
	unsigned char reserved:3;	// 10
	char Version[62];
	char Reference[6];
	unsigned short TotalLineNo;
	char UniqueItem[23];
	unsigned short CharNo;
	char TextLine[1000];
	unsigned short SegmentNo;
}NewsBuf;


struct Frame131
{
	char NewsGroupCode;
	long ActualDate;			// 5
	short ActualTime;			// 7
	unsigned short StoryNo;		// 9
	unsigned char useversion:1;
	unsigned char vesionline:1;
	unsigned char itemid:1;
	unsigned char headline:1;
	unsigned char headlineonly:1;
	unsigned char reserved:3;	// 10
	char Version[62];
	char Reference[6];
	unsigned short TotalLineNo;
	char UniqueItem[23];
	char HeadlineText[82];
	long TransDate;
	char GroupCode;
	unsigned short idx;
	CString TextLine;

	const void operator=(const Frame131& Data)
	{
		NewsGroupCode = Data.NewsGroupCode ; 
		ActualDate = Data.ActualDate ;
		ActualTime = Data.ActualTime ;	
		StoryNo = Data.StoryNo ;
		useversion = Data.useversion ;
		vesionline = Data.vesionline ;
		itemid = Data.itemid ;
		headline = Data.headline ;
		headlineonly = Data.headlineonly ;
		memcpy(Version,Data.Version,62);
		memcpy(Reference,Data.Reference,6);
		TotalLineNo = Data.TotalLineNo;
		memcpy(UniqueItem,Data.UniqueItem,23);	
		memcpy(HeadlineText,Data.HeadlineText,82);
		TransDate = Data.TransDate ;	
		GroupCode = Data.GroupCode ;
		idx = Data.idx; 
		TextLine = Data.TextLine ;
	}
};


struct FrameNews
{	long TransDate;
	long ActualDate;			
	unsigned short ActualTime;	
	unsigned short StoryNo;
	unsigned char GroupCode;
	unsigned char SubGroupCode;				
	unsigned char headlineonly; //only for 131
	char Version[65]; //only for 131
	char Reference[8];//only for 131
	unsigned short TotalLineNo;
	char UniqueItem[25];//only for 131
	char HeadlineText[170];
	unsigned short idx;
	char Language;  //only for 149
	unsigned short CancelNewsFlag;	//only for 149
	CString TextLine;

	const void operator=(const FrameNews& Data)
	{
		//memset(*this,0,sizeof(this)-sizeof(CString));
		TransDate =Data.TransDate ; 
		ActualDate = Data.ActualDate ;
		ActualTime = Data.ActualTime ;	
		StoryNo = Data.StoryNo ;
		GroupCode = Data.GroupCode ; 
		SubGroupCode = Data.SubGroupCode ;
		headlineonly = Data.headlineonly ;
		memcpy(Version,Data.Version,sizeof(Version));
		memcpy(Reference,Data.Reference,sizeof(Reference));
		TotalLineNo = Data.TotalLineNo;
		memcpy(UniqueItem,Data.UniqueItem,sizeof(UniqueItem));	
		memcpy(HeadlineText,Data.HeadlineText,sizeof(HeadlineText));		
		idx = Data.idx; 
		Language = Data.Language;  
		CancelNewsFlag = Data.CancelNewsFlag; 
		TextLine = Data.TextLine ;
	}

	const void operator=(const Frame131& Data)
	{
		memset(this,0,sizeof(*this) - sizeof(CString));//CString是指针，不能清为零
		TransDate = Data.TransDate ;	
		ActualDate = Data.ActualDate ;
		ActualTime = Data.ActualTime ;	
		StoryNo = Data.StoryNo ;
		GroupCode = Data.GroupCode ;
		SubGroupCode = Data.NewsGroupCode ; 		
		headlineonly = Data.headlineonly ;
		memcpy(Version,Data.Version,sizeof(Data.Version));
		memcpy(Reference,Data.Reference,sizeof(Data.Reference));
		TotalLineNo = Data.TotalLineNo;
		memcpy(UniqueItem,Data.UniqueItem,sizeof(Data.UniqueItem));	
		memcpy(HeadlineText,Data.HeadlineText,sizeof(Data.HeadlineText));
		idx = Data.idx; 
		Language = '0';  
		CancelNewsFlag = 0; 
		TextLine = Data.TextLine ;
	}
	const void operator=(const Frame149& Data)
	{
		memset(this,0,sizeof(*this) - sizeof(CString));
		TransDate = Data.TransDate ;	
		ActualDate = Data.TransDate ;
		ActualTime = Data.ReleaseTime ;	
		StoryNo = Data.NewsId ;		//caution
		GroupCode = Data.GroupCode ;
		SubGroupCode = Data.NewsCode; 		//caution
//		headlineonly = 0 ;	//caution
//		memcpy(Version,0,sizeof(Version));
//		memcpy(Reference,0,sizeof(Reference));
		TotalLineNo = Data.TotalLineNo;
//		memcpy(UniqueItem,0,sizeof(UniqueItem));	
		memcpy(HeadlineText,Data.HeadlineText,sizeof(Data.HeadlineText));
		idx = Data.idx; 
		Language = Data.Language ;  
		CancelNewsFlag = Data.CancelNewsFlag ; 
		TextLine = Data.TextLine ;
	}
};

struct UserID
{
	WORD    ReqNum;
	DOUBLE  ReqId;
	DOUBLE	UserId;
};

typedef CArray<UserID,UserID &> USER_ID;

struct UserInfo
{	
	//USER_ID UserIdList;
	DWORD dwChannelID;
	DWORD dwCount; //通道记数
	char GroupCode;
	char SubGroupCode;
	const void operator=(const UserInfo & Data)
	{
		//UserIdList.Copy(Data.UserIdList);
		dwChannelID = Data.dwChannelID; 
		dwCount = Data.dwCount;		
		GroupCode = Data.GroupCode ;
		SubGroupCode = Data.SubGroupCode ;
	}	
};

struct NewsIdx
{
	unsigned char GroupCode;			
	unsigned short ActualTime;	
	unsigned short StoryNo;		
	//以GroupCode + SubGroupCode + idx为key
};

//typedef CMap<ULONG,ULONG,Frame131,Frame131> TODAY131;
//typedef CMap<ULONG,ULONG,Frame149,Frame149> TODAY149;
typedef CMap<ULONGLONG,ULONGLONG,FrameNews,FrameNews> TODAYNews;
typedef CMap<USHORT,USHORT,UserInfo,UserInfo&> UserList;
typedef CMap<ULONGLONG,ULONGLONG,NewsIdx,NewsIdx&> NewsList;

//andy add 2007.08.24 for Options
struct OptUserInfo
{
	//USER_ID UserIdList;
	DWORD dwChannelID;
	DWORD dwCount; //通道记数
	char GroupCode;
	char szItemCode[8];
	long lTransDate;	
	const void operator=(const OptUserInfo & Data)
	{
		//UserIdList.Copy(Data.UserIdList);
		dwChannelID = Data.dwChannelID; 
		dwCount = Data.dwCount;		
		GroupCode = Data.GroupCode ;
		memcpy(szItemCode,Data.szItemCode,8);
		lTransDate = Data.lTransDate; 
	}		
};

typedef	CMap<CString,LPCSTR,OptUserInfo,OptUserInfo&> OptUserList;
//andy add 2007.08.24 for Options


class CProcGetNewsData
{
public:
	LONG m_RETURNVALUE;
	TCHAR m_Condition[501];
	LONG m_TransDate;
	LONG m_ActualDate;
	SHORT m_ActualTime;
	LONG m_StoryNo;
	TCHAR m_GroupCode[2];
	BYTE m_SubGroupCode;
	TCHAR m_HeadLine[171];
	TCHAR m_Detail[1024];
	SHORT m_TotalLine;
	VARIANT_BOOL m_bHeadlineOnly;
	TCHAR m_Version[66];
	TCHAR m_Reference[9];
	TCHAR m_UniqueItem[26];
	BYTE m_Stamp[8];
	SHORT m_idx;	
	TCHAR m_Language[2];
	VARIANT_BOOL m_CancelFlag;
	LONG m_DetailLen;

BEGIN_PARAM_MAP(CProcGetNewsData)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_Condition)
END_PARAM_MAP()

BEGIN_ACCESSOR_MAP(CProcGetNewsData, 2)
   BEGIN_ACCESSOR(0, true)	//  auto accessor
		COLUMN_ENTRY(1, m_TransDate)
		COLUMN_ENTRY(2, m_ActualDate)
		COLUMN_ENTRY(3, m_ActualTime)
		COLUMN_ENTRY(4, m_StoryNo)
		COLUMN_ENTRY(5, m_GroupCode)
		COLUMN_ENTRY(6, m_SubGroupCode)
		COLUMN_ENTRY(7, m_HeadLine)
		COLUMN_ENTRY(8, m_TotalLine)
		COLUMN_ENTRY_TYPE(9, DBTYPE_BOOL, m_bHeadlineOnly)
		COLUMN_ENTRY(10, m_Version)
		COLUMN_ENTRY(11, m_Reference)
		COLUMN_ENTRY(12, m_UniqueItem)
		COLUMN_ENTRY(13, m_idx)
		COLUMN_ENTRY(14, m_Language)
		COLUMN_ENTRY(15, m_CancelFlag)
		COLUMN_ENTRY(16, m_DetailLen)
   END_ACCESSOR()
   BEGIN_ACCESSOR(1, false) 
		COLUMN_ENTRY(1, m_TransDate)
		COLUMN_ENTRY(2, m_ActualDate)
		COLUMN_ENTRY(3, m_ActualTime)
		COLUMN_ENTRY(4, m_StoryNo)
		COLUMN_ENTRY(5, m_GroupCode)
		COLUMN_ENTRY(6, m_SubGroupCode)
		COLUMN_ENTRY(7, m_HeadLine)
		COLUMN_ENTRY(8, m_Detail)
		COLUMN_ENTRY(9, m_TotalLine)
		COLUMN_ENTRY_TYPE(10, DBTYPE_BOOL, m_bHeadlineOnly)
		COLUMN_ENTRY(11, m_Version)
		COLUMN_ENTRY(12, m_Reference)
		COLUMN_ENTRY(13, m_UniqueItem)
		COLUMN_ENTRY(14, m_Stamp)
		COLUMN_ENTRY(15, m_idx)
		COLUMN_ENTRY(16, m_Language)
		COLUMN_ENTRY(17, m_CancelFlag)
		COLUMN_ENTRY(18, m_DetailLen)
	END_ACCESSOR()
END_ACCESSOR_MAP()

//DEFINE_COMMAND(CdboProcGetStoryData1Accessor, _T("{ ? = CALL dbo.Proc_GetStoryData;1 (?) }"))
};

class CProcGetNewsDetail
{
public:
	LONG m_RETURNVALUE;
	LONG m_TransDate;
	SHORT m_ActualTime;
	LONG m_StoryNo;
	TCHAR m_cGroupCode[2];
	LONG m_Offset;
	LONG m_Size;
	TCHAR m_Detail[1024];

BEGIN_PARAM_MAP(CProcGetNewsDetail)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_TransDate)
	COLUMN_ENTRY(3, m_ActualTime)
	COLUMN_ENTRY(4, m_StoryNo)
	COLUMN_ENTRY(5, m_cGroupCode)
	COLUMN_ENTRY(6, m_Offset)
	COLUMN_ENTRY(7, m_Size)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CProcGetNewsDetail)
	COLUMN_ENTRY(1, m_Detail)
END_COLUMN_MAP()
//DEFINE_COMMAND(CdboProcGetStoryDetail1Accessor, _T("{ ? = CALL dbo.Proc_GetStoryDetail;1 (?,?,?,?,?,?) }"))
};

class CProcGetNewsDate
{
public:
	LONG m_RETURNVALUE;
	LONG m_TransDate;
	LONG m_coltransdate;

BEGIN_PARAM_MAP(CProcGetNewsDate)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_TransDate)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CProcGetNewsDate)
	COLUMN_ENTRY(1, m_coltransdate)
END_COLUMN_MAP()

//DEFINE_COMMAND(CProcGetNewsDate, _T("{ ? = CALL dbo.Proc_GetNewsDate;1 (?,?) }"))

};

class CProcGetInfo
{
public:
	LONG m_RETURNVALUE;
	LONG m_TransDate;
	TCHAR m_colGroupCode[2];
	BYTE m_colSubGroupCode;
	SHORT m_idx;

BEGIN_PARAM_MAP(CProcGetInfo)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_TransDate)
END_PARAM_MAP()

BEGIN_COLUMN_MAP(CProcGetInfo)
	COLUMN_ENTRY(1, m_colGroupCode)
	COLUMN_ENTRY(2, m_colSubGroupCode)
	COLUMN_ENTRY(3, m_idx)
END_COLUMN_MAP()

//DEFINE_COMMAND(CProcGetInfo, _T("{ ? = CALL dbo.Proc_GetInfo;1 (?) }"))

};
class CProcClearData
{
public:
	LONG m_RETURNVALUE;
	LONG m_TransDate;

BEGIN_PARAM_MAP(CProcClearData)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY(1, m_RETURNVALUE)
	SET_PARAM_TYPE(DBPARAMIO_INPUT)
	COLUMN_ENTRY(2, m_TransDate)
END_PARAM_MAP()


//DEFINE_COMMAND(CProcClearData, _T("{ ? = CALL dbo.Proc_ClearData;1 (?) }"))

};
/*
	
*/
struct STProtocol_C2_MD
{
	/*
	char cGCode;
	char Item[8];
	BYTE filety:3;
	BYTE prcety:3;
	BYTE action:2;
	long dt;
	WORD tm;
	BYTE transty:4;
	BYTE reload:1;
	BYTE reserved:3;
	float value;
	float origvalue;
	*/

	char cGCode;
	char Item[8];
	BYTE filety:3;
	BYTE prcety:3;
	BYTE action:2;
	long dt;
	WORD tm;
	BYTE transty:4;
	BYTE reload:1;
	BYTE usedt2:1;   //    /* TRUE-Use the second trans. date below (dt2) */
	BYTE reserved:2;
	float value;
	float origvalue;    
	
	//andy add 2013.10.11
	unsigned long exSysID;
	unsigned long exSysIDMask;
	long dt2;

};

#pragma pack(8)
//andy add for news 20070621



#endif