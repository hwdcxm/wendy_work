
/*
!%
!$QSNative_Option.h	[.]	ͷ�ļ�: �����������ģ��(OptionServer)
!$QSNative_Option.h	[<]	?include file
!$QSNative_Option.h	[*]	****

*/


#ifndef	_QSNATIVE_OPTION_H_
#define	_QSNATIVE_OPTION_H_

#include "QServer_Native.h"
#include "TTOptionsDataTable.h"

#include "datadef.h"

#include "Mmsystem.h"   //link Winmm.lib

#pragma pack(1)
// Options
#define DE_FILEID			"OptionsExpiryDateFile"		//file id
typedef struct
{	
	long lMonth;		//YYYYMM
	long lExpiryDate;   //YYYYMMDD
	
} OptionsExpiryDate;

typedef struct
{	
	char			szItemCode[8];		//��Ʊ��         
	int				nMonthCount;		//month������    
	char			cGroup;				//�� A/B/C ����  
	long			m_lShares;          //����
	char			m_szReserve[47];	//               
} OptionsExpiryDateItem;

typedef struct
{	
	char			m_szFileID[32] ;	//�ļ�ID    "OptionsExpiryDateFile"	
	char			m_szVersion[32];	//�汾��   
	unsigned short	m_LenOfHeader ;		//ͷ������  
	int				m_nItemCount;		//item�ĸ���   
	float			m_fInterestRate;    //��ȫ����     
	long			m_LastModifiedDate;	//            
	unsigned short	m_LastModifiedTime; //				
	char			m_szReserve[78] ;
	
} OptionsExpiryDateFileHead;


typedef CArray<OptionsExpiryDate,OptionsExpiryDate &> ExpiryDateList;

struct	OptionsExpiryDateInfo
{
	
	CString	ItemCode;	//Group+Item "A00700   "		
	ExpiryDateList expList;
	
	const void operator=(const OptionsExpiryDateInfo & Data)
	{
		ItemCode = Data.ItemCode;
		expList.Copy(Data.expList);
	}	
};
struct OptionsItemShares
{
	CString ItemCode;
	long    Shares;
};

typedef	CMap<CString,LPCSTR,OptionsExpiryDateInfo,OptionsExpiryDateInfo&> OptionsExpiryDateMap;
typedef	CMap<CString,LPCSTR,OptionsItemShares,OptionsItemShares&> OptionsItemSharesMap;

struct DayIVs
{
	char  group;
	char  item[8];
	float open;
	float high;
	float low;
	float close;
	float iv[TT_OPT_MAXMONTH];
	int	  month[TT_OPT_MAXMONTH];
	char  reserved[10];
};

struct OptIV
{
	int    date;
	DayIVs m_items[TT_OPT_MAXITEM];
	const void operator=(const OptIV& Data)
	{
		date=Data.date;
		memcpy(&m_items,&Data.m_items,sizeof(Data.m_items));
	}
};
typedef CArray<OptIV,OptIV&> CARRAYOptIV;

typedef struct tagMaxPainMonthData
{
	DWORD	dwMonth;		//		�·�
	double	dStrike;		//		��ʹֵʱ����ʹ��
	double	dPvalue;		//		��ʹֵ
	
}MaxPainMonthData;
typedef CArray<MaxPainMonthData, MaxPainMonthData&>  CMaxPainMonthDataArray;

typedef struct tagMaxPainItemData
{
	BYTE	bGroup;
	CHAR	bItem[8];		
	CMaxPainMonthDataArray	arrMonthData;
	void ResetContent()
	{
		bGroup = 0;
		memset(&bItem, 0, sizeof(bItem));
		arrMonthData.RemoveAll();
	}
	void operator = (const tagMaxPainItemData &newValue)
	{
		bGroup = newValue.bGroup;
		memcpy(&bItem, &newValue.bItem, sizeof(bItem));
		arrMonthData.RemoveAll();
		arrMonthData.Copy(newValue.arrMonthData);
	}
	tagMaxPainItemData()
	{
		ResetContent();
	}
	tagMaxPainItemData(const tagMaxPainItemData &newValue)
	{
		bGroup = newValue.bGroup;
		memcpy(&bItem, &newValue.bItem, sizeof(bItem));
		arrMonthData.RemoveAll();
		arrMonthData.Copy(newValue.arrMonthData);
	}
}MaxPainItemData;
typedef CArray<MaxPainItemData, MaxPainItemData&>  CMaxPainItemDataArray;

typedef struct tagMaxPainOpt
{
	DWORD	date;						//	���� 
	CMaxPainItemDataArray arrItemData;	
	void ResetContent()
	{
		date = 0;
		arrItemData.RemoveAll();
	}
	void operator = (const tagMaxPainOpt &newValue)
	{
		date = newValue.date;
		arrItemData.RemoveAll();
		arrItemData.Copy(newValue.arrItemData);
	}
	tagMaxPainOpt()
	{
		ResetContent();
	}
	tagMaxPainOpt(const tagMaxPainOpt &newValue)
	{
		date = newValue.date;
		arrItemData.RemoveAll();
		arrItemData.Copy(newValue.arrItemData);
	}
}MaxPainOpt;
typedef CArray<MaxPainOpt, MaxPainOpt&>  CMaxPainOptArray;

struct DayItems
{
	char	item[8];
	long	month[TT_OPT_MAXMONTH];		//�·���		//�·ݺ���ʹ��,��ʹֵһһ��Ӧ
	double	strike[TT_OPT_MAXMONTH];	//��ʹֵʱ����ʹ��
	double  pvalue[TT_OPT_MAXMONTH];	//��ʹֵ
	char    group;
	char	reserved[7];
};

struct OptMaxPain
{
	long	  date;				//����
	DayItems  m_items[TT_OPT_MAXITEM];
	const void operator=(const OptMaxPain& Data)
	{
		date=Data.date;
		memcpy(&m_items,&Data.m_items,sizeof(Data.m_items));
	}
};
typedef CArray<OptMaxPain,OptMaxPain&> CARRAYOptMaxPain;

struct OldDayItems
{
	char	item[8];
	long	month[12];		//�·���		//�·ݺ���ʹ��,��ʹֵһһ��Ӧ
	double	strike[12];	//��ʹֵʱ����ʹ��
	double  pvalue[12];	//��ʹֵ
	char    group;
	char	reserved[7];
};
struct OldOptMaxPain
{
	long	  date;				//����
	OldDayItems  m_items[TT_OPT_MAXITEM];
	const void operator=(const OldOptMaxPain& Data)
	{
		date=Data.date;
		memcpy(&m_items,&Data.m_items,sizeof(Data.m_items));
	}
};
typedef CArray<OldOptMaxPain,OldOptMaxPain&> CARRAYOldOptMaxPain;

struct OldDayIVs
{
	char  group;
	char  item[8];
	float open;
	float high;
	float low;
	float close;
	float iv[12];
	int	  month[12];
	char  reserved[10];
};

struct OldOptIV
{
	int    date;
	OldDayIVs m_items[TT_OPT_MAXITEM];
	const void operator=(const OldOptIV& Data)
	{
		date=Data.date;
		memcpy(&m_items,&Data.m_items,sizeof(Data.m_items));
	}
};
typedef CArray<OldOptIV,OldOptIV&> CARRAYOldOptIV;

#pragma pack(8)
struct FORCE_HOUKE_TIME_OPT
{
	BYTE	GCode;
	WORD	Time;
};

/*
!% 
!?CQSNative_Option		[.]		��: ���صĺ�̨����������(OptionServer)
!?CQSNative_Option		[<]		?class
!?CQSNative_Option		[*]		****
*/
class	CQSNative_Option : public CQS_Native
{
public:
	CQSNative_Option() ;
	virtual ~CQSNative_Option() ;
	
	void CheckDay();

/*
!% 
!?CQSNative_Option.CanProcRequest_W2t()	 [.] ��CQSNative_Option��Ա����: �Ƿ���Դ���ĳ�����͵�W2tЭ������.
*/	
    virtual BOOL CanProcRequest_W2t( int serverType ) ;


/*
!% 
!?CQSNative_Option.OnNewFrame_TT()		[.] ��CQSNative_Option��Ա����: ���� TTFrame����.
*/	
    virtual void OnNewFrame_TT( BYTE* pFrame, WORD len ) ;

/*
!% 
!?CQSNative_Option.OnRequest_W2t()		[.] ��CQSNative_Option��Ա����: ����W2t����.
*/	
    virtual BOOL OnRequest_W2t( BYTE* pPackage, int pkgSize ) ;


/*
!% 
!?CQSNative_Option.OnIgpFileUpdate()		[.] ��CQSNative_Option��Ա����: Igp�ļ��ı����Ϣ.

*/	
   virtual void OnIgpFileUpdate() ;



/*
!% 
!?CQSNative_Option.Init()					[.] ��CQSNative_Option��Ա����: ��ʼ��
*/	
   virtual BOOL Init( LPCSTR pszINIFile, IUnknown* pGroupsMng  ) ; 

/*
!% 
!?CQSNative_Option.Play()					[.] ��CQSNative_Option��Ա����: ����ģ�鹤��
*/	
   virtual BOOL Play() ; 

/*
!% 
!?CQSNative_Option.Stop()					[.] ��CQSNative_Option��Ա����: ֹͣģ�鹤��
*/	
   virtual BOOL Stop() ; 

public:
	BOOL IsDateValid(DWORD dt);	
	BOOL DateDiff(DWORD dt1,DWORD dt2,int& iDiff);

	static	void  CALLBACK OneShotTimer(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2) ;
	MMRESULT m_TimerID;

	COptFrame m_optFm[TT_OPT_MAXITEM];
	//COptFrame* m_optFm;
	long m_nItemNum;
	void ResetDataTable();
	DWORD m_dwId;

	void SaveTime();
	void GetSaveTime();

	HRESULT SendAvailableMonthToUser(BYTE cbGroup,LPCTSTR pszItemCode,long lTransdate);
	HRESULT SendAllAvailableMonth(BYTE cbGroup,LPCTSTR pszItemCode,long lTransdate,DOUBLE ReqId, DOUBLE UserId);
	HRESULT SendAvailableMonth(BYTE cbGroup,LPCTSTR pszItemCode,long lTransdate,DOUBLE ReqId, DOUBLE UserId);
	HRESULT SendAvailableMonth2(BYTE cbGroup,LPCTSTR pszItemCode,long lTransdate,DWORD channelID);
	void SendAllHistoricalData(unsigned char cbGroupCode,LPCTSTR pszItemCode,long lTransdate,DOUBLE ReqId, DOUBLE UserId);
	void SendHistoricalData(unsigned char cbGroupCode,LPCTSTR pszItemCode,long lTransdate,DOUBLE ReqId, DOUBLE UserId);
	void SendHistoricalData2(unsigned char cbGroupCode,LPCTSTR pszItemCode,long lTransdate,DWORD channelID);

	HRESULT WriteLogFile(LPTSTR strLog);
	HRESULT GetNewOptionsData(BYTE cbGroup,BYTE* buf, int nFrameLen);
	long EnumOptionItemCodeEx(LPCTSTR pszItemCode, BYTE cbGroup);
	long InsertItemCodeEx(LPCTSTR pszItemCode, BYTE cbGroup);
	long FindMonthEx(long nItemIdx, long lTransdate);
	long InsertMonthEx(long nItemIdx, long lTransdate);
	HRESULT DeleteMonthEx(long nItemIdx, long nMonthIdx);

	long FindStrikeEx(COptMonth* pMf, float fStrike);
	long InsertStrikeEx(COptMonth* pMf, float fStrike);
	HRESULT DeleteStrikeEx(COptMonth* pMf, long nStrikeIdx);

	HRESULT SendDataToUser(COptFrame* pFm, COptMonth* pMf, COptLine* pSl, int nType,CString strUser);
	HRESULT SendData(COptFrame* pFm, COptMonth* pMf, COptLine* pSl, int nType,DWORD channelID);
	HRESULT SendDelFrameToUser(char cGCode, COptRecFrame* pRec, BOOL bDelStrike);
	HRESULT SendDelFrame(char cGCode, COptRecFrame* pRec, BOOL bDelStrike,DWORD channelID);
	HRESULT GetDataHistory(COptFrame* pFm, COptMonth* pMf, COptLine* pSl, int nType,char* pBuff,long& nLen);
	HRESULT Init();
	HRESULT StoreManage(BOOL bIsRead);

	HRESULT SendOutFrame(BYTE *pbuff, int nLen);

//	HRESULT WriteRecieveData(unsigned char *pBuf, long nLen);
	HRESULT WriteRecieveData(unsigned char *pBuf, long nLen,char* pFile=NULL);


	HRESULT StartCOMs();
	HRESULT StopCOMs();

	BYTE HandleFrameRequest(BYTE * pBufTemp,int nLength);  //modified by jansen20030628only 

	//	liu paul ˵
	//CQSNative_Option :: OnRequest_W2t() , OneShotTimer() �����̶߳��� ���� CQServer�� ͨ����Ϣ, ����ǳ�����ȫ. 
	//�� �ⲿ��option server, ����MSʱ��, ���з��Ͱ� תΪ�� һ���߳�, ����ǰ�ȫ��. 
	//andyע: ��ʵ�����ݵ��̺߳���������̶߳��ڷ����ݸ��ͻ���,����һ���̲߳�֪�Ƿ�����.
	void SendRegularData();  //andy 2012.11.13 ȥ����ʱ�������ݵ��߳�,�ŵ�����������̵߳ķ���ȫ������.
	DWORD	m_tmSend ;		// ���һ�η���ȫ������ʱ��.

	CTTOptionsDataTable m_optionsdatatable;

	long m_nTransDate;
	short m_nTransTime;
	
	OptUserList m_UserList;
	
//	HANDLE m_hOptiEvent;
//
//	HANDLE	m_hStopEvent;		//�߳�ֹͣ
//	HANDLE	m_hStopdEvent;		//�߳��Ѿ�ֹͣ
//	HANDLE	m_hStopdEvent2;		//�߳��Ѿ�ֹͣ
//	HANDLE	m_hRequRead;		//���������ݿɶ�
//	HANDLE	m_hDataRead;		//Source Mas�������ݿɶ�
//
//	int m_nReqReadMasId;				//ID
//	int m_nReadMasId;				//ID
//
//	ITTMultiAS * m_pInIMultiAS;		//����Source���ݵ�Mas
//
//	ITTMultiAS * m_pInRIMultiAS;	//����W2T�������ݵ�Mas
//	ITTMultiAS * m_pOutIMultiAS;	//д��W2T�������ݵ�Mas

	CRITICAL_SECTION m_cs;
	CRITICAL_SECTION m_csUser;	

	DOUBLE	m_ReqIdDoor;  //��¼DoorServer��reqID;
	
	BYTE HandleFrame_Request(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId);
	BYTE HandleFrame_Request_Connect(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId);
	BYTE HandleFrame_Request_RQ(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId);
	BYTE HandleFrame_Request_RD(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId);
	BYTE HandleFrame_Request_UC(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId);
	BYTE HandleFrame_Request_UD(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId);

	BYTE LoadConfig();

	//johnny add 20170705 ���Ӵ�����ʹoption
	char m_OptDatPath1[MAX_PATH];
	char m_OptDatPath2[MAX_PATH];
	void SortPainList();
	BOOL SaveMaxPain();
	BOOL LoadMaxPain();
	void VersionCompatibility(CMaxPainOptArray& maxPainOpt);
	void VersionCompatibility(MaxPainOpt& maxPainOpt, const OptMaxPain& temp);
	BOOL CalMaxPain();
	int  FindPain(long date);
	CARRAYOptMaxPain m_MaxPainList;
	CMaxPainOptArray m_arrMaxPainOpt;
	//johnny add 20170810 ����IV
	CARRAYOptIV		 m_OptIV;
	//OptIV			 m_OptIV;
	BOOL CalcIV();
	BOOL CalcIV2();
	BOOL LoadIV();
	BOOL SaveIV();
	int  IsIndex(char* str);
	int  IsIndex2(char* str);
	int  FindIV(long date);
	void SortIVList();
	BOOL GetItemPrice(DayIVs& opt);
	int  GetCloseNum(COptMonth* month, float price);
	long GetExpiryDays(long day);
	BOOL GetExpInfo();
	BYTE ReadExpiryDateListFile();
	int  GetExpiryTime(char group, char* itemcode, int month);
	int  FindItemInOption(char group, char* itemcode, OptionIV& iv);
	float m_fRiskInterest;  //��ȫ����
	OptionsExpiryDateMap m_ExpiryDateMap;
	OptionsItemSharesMap m_ItemSharesMap;
	char m_szExpPath[MAX_PATH];
	char m_szIVPath1[MAX_PATH];
	char m_szIVPath2[MAX_PATH];
	//����ÿ���ӷ���iv
	long m_IVLastSend;
	BOOL GetIV(char* szItem, DayIVs& temp);
	BYTE SendIVRequest(unsigned char cbGroupCode,LPCTSTR pszItemCode, long lTransdate, DOUBLE ReqId, DOUBLE UserId);
	void SendIVEveryMin(unsigned char cbGroupCode,LPCTSTR pszItemCode, long lTransdate,DWORD channelID);

	//Johnny 20170918 ��������ǰһ��OI
	long m_lastDate;
	int  m_nItemNum2;
	//COptFrame m_PreOI[TT_OPT_MAXITEM]; // wendy COptOIFrame
	BOOL SavePreviousOI();
	BOOL LoadPreviousOI();
	void GetPreOI(long day);
	BYTE SendPreOIRequest(unsigned char cbGroupCode,LPCTSTR pszItemCode, long lTransdate, DOUBLE ReqId, DOUBLE UserId);
	void SendPreOIEvery3Min(unsigned char cbGroupCode,LPCTSTR pszItemCode, long lTransdate,DWORD channelID);
	long EnumOptionItemCodeEx2(LPCTSTR pszItemCode, BYTE cbGroup);
	long FindMonthEx2(long nItemIdx, long lTransdate);
	//johnny 20170928 ����UX��UYЭ�飬�����̶�ͨ�����ͳɼ�
	OptUserList m_UserList2;
	BYTE HandleFrame_Request_UX(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId);
	BYTE HandleFrame_Request_UY(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId);
	BYTE SendLastPriceRequest(unsigned char cbGroupCode,LPCTSTR pszItemCode, long lTransdate, DOUBLE ReqId, DOUBLE UserId);
	HRESULT SendDataToUser2(COptFrame* pFm, COptMonth* pMf, COptLine* pSl, int nType,CString strUser);
	HRESULT SendData2(COptFrame* pFm, COptMonth* pMf, COptLine* pSl, int nType, DWORD channelID);
	
	//johnny 20180306 ����UE��UFЭ�飬�����̶�ͨ����IV
	OptUserList m_UserList3;
	BYTE HandleFrame_Request_UE(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId);
	BYTE HandleFrame_Request_UF(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId);

	BYTE Compress( LPCSTR InFile, LPCSTR OutFile );

	IUnknown * m_pIShell;

	char m_szErrLog[150];

	int		m_MultCastPort;				//����Data
	char	m_MultCastName[40];			//Multicast name for input
	CComBSTR m_bstrBindIP;				//�󶨵�mulitcast������

	int		m_ServerPort;				//Self Port for output
	CComBSTR m_bstrBindIPDoor;			//�󶨵�doorserver������

	HANDLE m_hRespondThread;
	HANDLE	m_hCovThread;		//�����߳�

//	CComPtr<IUnknown>   m_pTcpipBios;

	BYTE		m_pOutBuff[MAX_W2TUP_BUFF];		//�������ݵ�buff
	BYTE		m_pOutBuff2[90000];		//���ͼ�ʱ���ݵ�buff    100��strike�ĳ�����9100,����strike֮��ԭ��8000��overflow.

	char m_szBackupPath[MAX_PATH];

	void HandleFrame( BYTE* pBuf, WORD Len) ;
	void ProceDataFrame_128( _tagTTDataFrame * pStFrame );
	void ProceDataFrame_200( _tagTTDataFrame * pStFrame ); 
	void ProceDataFrame_253( _tagTTDataFrame * pStFrame ); 
	void ProceDataFrame_136( _tagTTDataFrame * pStFrame , BYTE* pBuf, int nFrameLen);

	//johnny add 20180202

	CArray<FORCE_HOUKE_TIME_OPT , FORCE_HOUKE_TIME_OPT &> m_ArrHoukeTime_opt;
	BOOL CheckTime(_tagTTDataFrame * pStFrame);

} ;

#endif