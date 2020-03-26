#define G_TXTOCCFILEID         0xFF70         
#define G_GENCODE__LEN           8            /* File name length */ 
#define TT_OP_ITEMNUM			6

#define G_FRMSPECTRANSCODE     5

#define G_TXTFCODELEN          6             /* Length of cash item code in futures item code */
#define G_TXTOPTASK            0             /* Index to any 'askbid[2]' structure - this holds ask */
#define G_TXTOPTBID            1             /* Index to any 'askbid[2]' structure - this holds bid */
#define G_TXTOPTHLLO           0             /* Lowest price (index to g_txtoptcpS.hilo[]) */
#define G_TXTOPTHLHI           1             /* Highest price (index to g_txtoptcpS.hilo[]) */
#define G_TXTMAXOPTITEM        9             /* Maximum no. of options items less 1 */
#define G_TXTNUMOPTITEM        (G_TXTMAXOPTITEM + 1)
#define G_TXTMAXOPTMONTH       9             /* Maximum no. of options months less 1 */
#define G_TXTNUMOPTMONTH       (G_TXTMAXOPTMONTH + 1)
#define G_TXTMAXOPTSTRIKE      39            /* Maximum no. of options strike prices less 1 */
#define G_TXTNUMOPTSTRIKE      (G_TXTMAXOPTSTRIKE + 1)

#define OPTTXTYASK              0        /* Options trans. and special types - see 'frame3.des' */
#define OPTTXTYBID              1
#define OPTTXTYLAST             2
#define OPTTXTYHIGH             3
#define OPTTXTYLOW              4
#define OPTTXTYASKQTY           5
#define OPTTXTYBIDQTY           6
#define OPTTXTYSUMMARY          50
#define OPTTXTYCORLAST          60
#define OPTTXTYCORVOL           61
#define OPTTXTYDELSTRIKE        62
#define OPTTXTYDELMONTH         63
#define OPTSPTYMASKCALLPUT      0x01
#define OPTSPTYMASKTHEORET      0x02
#define OPTSPTYMASKCABINET      0x04
#define OPTSPTYMASKDECIMAL      0x18
#pragma pack(1)

typedef struct                               /* Options call/put details */
{
 unsigned theoretask:1;                      /* TRUE-Theoretical ask, FALSE-Actual ask */
 unsigned theoretbid:1;                      /* TRUE-Theoretical bid, FALSE-Actual bid */
 unsigned cabinetbid:1;                      /* TRUE-Cabinet bid */
 unsigned cabinetlast:1;                     /* TRUE-Cabinet last */
 unsigned reserved:4;                        /* Reserved - to make no. of bits a multiple of 8 */
 char reserve1;
 float ab[2],                                /* Ask/bid */
       last,                                 /* Last */
       vol,                                  /* Volume */
       oab[2],                               /* Opening ask/bid */
       olast,                                /* Opening last */
       hilo[2],                              /* Low/high */
       cuvol,                                /* Cumulative volume (total no. of contracts traded) */
       oi,                                   /* Open interest */
       ptover;                               /* Previous turnover */
 char reserve2[10];
} g_txtoptcpS;

typedef struct                               /* Options strike price details */
{
 float strikeprice;                          /* Strike price */
 char reserve1[4];
 g_txtoptcpS cp[2];                          /* Call/put details */
} g_txtoptstS;

typedef struct                               /* Options month details */
{
 long mth;                                   /* Trading month - yyyymm */
 char futcode[G_GENCODE__LEN];                 /* Corr. futures item code */
 int futhashval;                             /* Futures hash value (index to g_grptempS[]); -1=None */
 long request;							 /*	Whether be requested*/		
 char reserve1[18];
 unsigned stsorted:1;                        /* TRUE-The strike price details are sorted in ascending order of strike price */
 unsigned decimal:2;                         /* No. of decimal points for this strike price, bid and ask values */
 unsigned reserved:5;                        /* Reserved - to make no. of bits a multiple of 8 */
 
 char reserve[90];							 /**/
 g_txtoptstS st[G_TXTNUMOPTSTRIKE];          /* Strike price details */
} g_txtoptmmS;

typedef struct                               /* Options pages - data from HKFE */
{
 char groupcode;                             /* Group code; as defined in g_grpgpS */
 char itemcode[G_GENCODE__LEN];                /* Corr. cash item code */
 int itemhashval;                            /* Cash hash value (index to g_grptempS[]); -1=None */
 char fcode[G_TXTFCODELEN];                  /* Cash item code used in futures item code; NULL-terminated without trailing spaces */
 char reserve1[16];
 unsigned mmsorted:1;                        /* TRUE-The month details are sorted in ascending order of yyyymm */
 unsigned reserved:7;                        /* Reserved - to make no. of bits a multiple of 8 */
 char reserve2[93];
 g_txtoptmmS mm[G_TXTNUMOPTMONTH];           /* Month details */
} g_txtoptS;
//OCC FILE STRUCT
typedef struct                  /* Option cash itrem codes file header */
{
	WORD id;			        /* G_TXTOCCFILEID */
	BYTE reserved;              /* Reserved - 0x1A */
	WORD numitem;               /* No. of cash item codes */
	BYTE unused[27];
} g_txtoccheadS;


typedef struct                  /*Option cash item codes file detail */
{
	BYTE groupcode;                             /* Group code; as defined in g_grpgpS */
	BYTE itemcode[G_GENCODE__LEN];                /* Cash item code */
	BYTE fcode[G_TXTFCODELEN];                  /* Cash item code used in futures item code; NULL-terminated without trailing spaces */
	BYTE unused[16];
} g_txtoccS;

typedef struct
{
	unsigned char cbGroupCode;
	char          szItemCode[8];
	long		  lTransdate;
	float         fStrikePrice;
	unsigned char cbOrder;
} FrameID;
typedef struct
{	
	unsigned short wFrameType;
} FrameHead;

typedef unsigned short ElementType;

typedef unsigned short FrameLen;

typedef struct
{
	bool          bRequestMonth;
	unsigned char cbGroupCode;
	char          szItemCode[8];
	long          lTransdate;
	unsigned long dwID;
	bool          bWhole;
	unsigned long dwCount;
} RequestTable;

// kenny add code here
#define TT_OPT_MAXMONTH 12
#define TT_OPT_MAXROW 400
#define TT_OPT_MAXITEM 100

class COptData
{
public:
	float	fOI;
	float	fVol;
	float	fHigh;
	float	fLow;
	float	fLast;
	float	fBid;
	float	fAsk;
	float	fAskQty;
	float	fBidQty;
	float	fLastQty;
	float	fCumulate;

	unsigned char ucTheoretAsk;
	unsigned char ucTheoretBid;
	unsigned char ucCabBid;
	unsigned char ucCabLast;
};

class COptOCData
{
public:
	float fOpenAsk;
	float fOpenBid;
	float fOpenLast;

	float fCloseAsk;
	float fCloseBid;
	float fCloseLast;
	float fCloseCum;
	float fPreTurnover;
	float fPreOI;
};

class COptChanged
{
public:
	unsigned char ucAsk:1;			// 1
	unsigned char ucBid:1;			// 2
	unsigned char ucHigh:1;			// 3
	unsigned char ucLow:1;			// 4
	unsigned char ucLast:1;			// 5
	unsigned char ucAskQty:1;		// 6
	unsigned char ucBidQty:1;		// 7
	unsigned char ucLastQty:1;		// 8
	
	unsigned char ucCumulate:1;		// 1
	unsigned char ucOI:1;			// 2
	unsigned char ucRsv:6;			// 3~8
};

class COptLine
{
public:
	float	m_fStrike;
	COptData m_dat[2];
	COptOCData m_OcDat[2];
	COptChanged opChg[2];
public:
	COptLine(){Init();}
	void Init(void){memset(this, 0, sizeof(COptLine));}
};

class COptMonth
{
public:
	long lTransDate;
	unsigned char cDecimal;
	COptLine m_optLine[TT_OPT_MAXROW];
	unsigned char ucTotalRow;
public:
	COptMonth(){memset(this, 0, sizeof(COptMonth));}
	void Init(void)
	{
		lTransDate = 0;
		cDecimal = 0;
		for(int i = 0; i < ucTotalRow; i ++)
		{
			m_optLine[i].Init();
		}
		ucTotalRow = 0;
	}
};

class COptFrame
{
public:
	unsigned char cGCode;
	unsigned char strICode[8];
	COptMonth m_optMonth[TT_OPT_MAXMONTH];
	unsigned char ucMonthNum;
public:
	COptFrame(){memset(this, 0, sizeof(COptFrame));}
	void Init(void)
	{
		cGCode = 0;
		memset(strICode, 0, sizeof(strICode));
		for(int i = 0; i < ucMonthNum; i ++)
		{
			m_optMonth[i].Init();
		}
		ucMonthNum = 0;
	}
};

class COptRecFrame
{
public:
	unsigned char ucICode[8];
	unsigned long lTransDate;
	float fStrike;
	unsigned char ucFlags:6;
	unsigned char ucFlagsRsv:2;
	unsigned char ucCallOrPut:1;
	unsigned char ucTheoretical:1;
	unsigned char ucCabinet:1;
	unsigned char ucDecimal:2;
	unsigned char ucReserved:3;
	unsigned char ucRsv;
	float fTrans;
};

#pragma pack(8)

