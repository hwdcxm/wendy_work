
#include "stdafx.h"
#include "QSNative_Option.h"
#include "QServer.h"
#include "Protocol_SRV.h"  // wendy 
//#include "..\includeTJE\PathAPI.h"  // wendy 
//#include "BSM.h"	 // wendy 
//#include "BM1976.h"	// wendy
#include "Master.h"
//#include "QSNative_Tele.h" // wendy
//#include "../includeTJE/WrapperCompress.h" // wendy
//#include "GreenLeafCompress_i.c"
//#include "GreenLeafCompress.h"
//#include "QSNative_Charting.h" // wendy
#include "log.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//johnny modify tick长度增加
//extern BYTE g_OutputBuf[1024*1024] ;	// 用于最后的数据打包
extern BYTE g_OutputBuf[1024*1024*5] ;	// 用于最后的数据打包
extern Master	g_Master ; // wendy CMaster

//extern 
CLog	g_Log ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////

CQSNative_Option::CQSNative_Option()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
//	m_nTransDate = sysTime.wYear * 10000 + sysTime.wMonth * 100 + sysTime.wDay;
	m_nTransTime = sysTime.wHour * 100 + sysTime.wMinute;
	
	m_dwId = 1000;
	m_TimerID = NULL;

	m_fRiskInterest=0.0;
	if (m_arrMaxPainOpt.GetSize()>0)
	{
		m_arrMaxPainOpt.RemoveAll();
	}
	m_arrMaxPainOpt.SetSize(0,100);
//	m_hOptiEvent=NULL;
//	m_hStopdEvent = NULL;
//	m_hStopdEvent2 = NULL;
//	m_hStopEvent = NULL;
//	m_hRequRead = NULL;
//	m_hRespondThread = NULL;
//	m_hDataRead = NULL;
//	m_hCovThread = NULL;
	
	m_tmSend = ::GetCurrentTime() ;
	
	InitializeCriticalSection(&m_csUser);
}

CQSNative_Option::~CQSNative_Option()
{
	DeleteCriticalSection(&m_csUser);

}

void CQSNative_Option::OnIgpFileUpdate()
{

}

BOOL CQSNative_Option::Init( LPCSTR pszINIFile, IUnknown* pGroupsMng  ) 
{
	LoadConfig();
	
	Init();
	return FALSE ;
}


BOOL CQSNative_Option::Play()
{
	StartCOMs();
	
	StoreManage(TRUE);
	LoadPreviousOI();

	//johnny add 20170705 读取最痛option
	memset(&m_OptDatPath1,0,MAX_PATH);
	memset(&m_OptDatPath2,0,MAX_PATH);
	memset(&m_szExpPath, 0, MAX_PATH);
	memset(&m_szBackupPath, 0, MAX_PATH);
	memset(&m_szIVPath1,0,MAX_PATH);
	memset(&m_szIVPath2,0,MAX_PATH);

	CString str;
	CString inifile;
	CWrapperPath::GetExtPath(str);
	char szTemp[1000] ;
	inifile.Format( "%sMS.INI",  str ) ;
	
	GetPrivateProfileString( "optmaxpain", "Path1", "", m_OptDatPath1, 800, inifile ) ;
	GetPrivateProfileString( "optmaxpain", "Path2", "", m_OptDatPath2, 800, inifile ) ;

	GetPrivateProfileString( "option", "ExpPath", "", m_szExpPath, 800, inifile );
	GetPrivateProfileString( "option", "BackupPath", "", m_szBackupPath, 800, inifile );
	//GetPrivateProfileString( "option", "SavePath1", "", m_szIVPath1, 800, inifile ) ;
	//GetPrivateProfileString( "option", "SavePath2", "", m_szIVPath2, 800, inifile ) ;

	LoadMaxPain();

// 	m_TimerID =	timeSetEvent(5*60*1000,                    // delay
// 								10*1000,
// 								OneShotTimer,            // callback function
// 								(DWORD)this,             // user data
// 								TIME_PERIODIC/*TIME_ONESHOT*/ );  
	WriteLogFile("Start success.");
	return FALSE ;
}

HRESULT CQSNative_Option::WriteRecieveData(unsigned char *pBuf, long nLen,char* pFile)
{
	HANDLE m_hDateFile;
	char strFileDir[200];
	char strTemp[50];
	if(pFile==NULL)
		sprintf(strTemp,"\\W2TRecv.Dat");
	else
	{
		strcpy(strTemp,"\\");
		strcat(strTemp,pFile);
	}


	memset(strFileDir, 0, sizeof(strFileDir));
	GetCurrentDirectory(200, strFileDir);
	strcat(strFileDir, strTemp);

	m_hDateFile = CreateFile(strFileDir, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, 0, NULL);

	if(m_hDateFile == INVALID_HANDLE_VALUE)
		return 1;
	SetFilePointer(m_hDateFile, 0, NULL, FILE_BEGIN);
	DWORD dwTotal = SetFilePointer(m_hDateFile, 0, NULL, FILE_END);
	if(dwTotal > 5000000)
	{
		CloseHandle(m_hDateFile);
		m_hDateFile = CreateFile(strFileDir, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|
			FILE_SHARE_WRITE, NULL, TRUNCATE_EXISTING, 0, NULL);
	}

	SetFilePointer(m_hDateFile, 0, NULL, FILE_END);

	unsigned long nByteWrite = 0;

	if(nLen <= 0)
		return 2;

	WriteFile(m_hDateFile, pBuf, nLen, &nByteWrite, NULL);
	if(nByteWrite != nLen)
	{
		CloseHandle(m_hDateFile);
		return -1;
	}

	CloseHandle(m_hDateFile);
	return 0;
}


BOOL CQSNative_Option::Stop()
{
	//johnny add 20170705 保存最痛option
	SaveMaxPain();

	StoreManage(FALSE);
	SavePreviousOI();
	
	if(m_TimerID != NULL)
	{
	//	if(timeKillEvent(m_TimerID) == TIMERR_NOERROR)
			int yy=0;
	}
	WriteLogFile("Stop success !");

	StopCOMs();

	return FALSE ;
}


BOOL CQSNative_Option::CanProcRequest_W2t( int serverType ) 
{
	if( serverType==_QSERVERTYPE_OPTION )
		return TRUE ;

	return FALSE ;
}

BYTE CQSNative_Option::LoadConfig()
{
/*
	char inifile[256];
	GetCurrentDirectory(sizeof(inifile),inifile);
	
	strcat(inifile,"\\W2TOptionsServer.INI");

	m_MultCastPort=GetPrivateProfileInt("RECEIVEDATA","PORT",0,inifile);
	GetPrivateProfileString("RECEIVEDATA","NAME",0,m_MultCastName,sizeof(m_MultCastName),inifile);
	m_ServerPort=GetPrivateProfileInt("SERVER","PORT",0,inifile);
	
	char strTemp[100];
	memset(strTemp,0,sizeof(strTemp));
	GetPrivateProfileString("RECEIVEDATA","BINDIP",0,strTemp,sizeof(strTemp),inifile);		
	m_bstrBindIP = strTemp;
	memset(strTemp,0,sizeof(strTemp));
	GetPrivateProfileString("SERVER","BINDIP",0,strTemp,sizeof(strTemp),inifile);		
	m_bstrBindIPDoor = strTemp;
*/


	if(!m_UserList.IsEmpty()) m_UserList.RemoveAll();
	if(!m_UserList2.IsEmpty()) m_UserList2.RemoveAll();
	if(!m_UserList3.IsEmpty()) m_UserList3.RemoveAll();
	return 0;
}

HRESULT CQSNative_Option::StartCOMs()
{
	return S_OK;
}

HRESULT CQSNative_Option::StopCOMs()
{

	return S_OK;
}

void CQSNative_Option::OnNewFrame_TT( BYTE* pFrame, WORD len ) 
{
	_tagTTDataFrame* pStFrame=(_tagTTDataFrame *)pFrame;
	
	HandleFrame( pFrame, len ) ;

}

BOOL CQSNative_Option::OnRequest_W2t( BYTE* pPackage, int pkgSize ) 
{
	BYTE * pbuff = pPackage;
    int nLength = pkgSize;
	
	DOUBLE	ReqId=0,UserId=0;
	int		ip=0;	//读指针
	WORD	wdVal;
	DWORD	dwVal;
	BYTE	* pReq;
	int		nLen;
	WORD	Daof;
	WORD    wdType;
	
	//最小的buff长度,2+8+'WS'+Len+'ED' =18 现在变为16 = 8+'WS'+Len+'ED' 
	if(nLength<16)  //if(nLength<18)	
	{
		return 1;
	}
/*
	//读指针跳过Id num
	ip+=sizeof(WORD);
	//读入请求者的Id
	memcpy(&ReqId,pbuff+ip,sizeof(ReqId));
	ip+=sizeof(ReqId);
*/
	//前面的10byte变为8byte的网络协议包头，reqid现在没有用了，因为它是相当于door连newsserver的ip+port,现在集成在一起了。
	ip+=8;
	//接着应该是'WS'
	memcpy(&wdVal,pbuff+ip,sizeof(wdVal));
	ip+=sizeof(wdVal);
	//不是，只能退出了。
	if(wdVal!=START_FRAME)
	{
		return 1;
	}
	//读下一个,Len
	memcpy(&dwVal,pbuff+ip,sizeof(dwVal));
	ip+=sizeof(dwVal);
	
	//长度是否为nLength-2-8-2 (WORD + reqid + 'WS")  现在变为10 = 8+'WS'
	if(dwVal!=nLength-10)//if(dwVal!=nLength-12)	
	{
		return 1;
	}
	//读CS or DF
	memcpy(&wdVal,pbuff+ip,sizeof(wdVal));
	ip+=sizeof(wdVal);
	//CS =connect ,ds= disconnect,Df maybe rq request
	switch(wdVal)
	{
	case SERCONN_FRAME:		//增加一个连接，有个DoorServer连接上
		WriteLogFile("Accept a doorserver connect.");
		return 0;
		break;
	case SERDISC_FRAME:
		WriteLogFile("Accept a doorserver disconnect.");		
		return 0;
		break;
	case DATAOFFSET_FRAME:
		//读入Data offset
		memcpy(&Daof,pbuff+ip,sizeof(Daof));
		ip+=sizeof(Daof);
		//读入类型
		memcpy(&wdType,pbuff+ip,sizeof(wdType));
		ip+=sizeof(wdType);
		if(wdType=='RI')
			int nnnn=0;
		if(!(wdType==REQUESTR_FRAME || wdType==REQUESTC_FRAME || wdType==REQUESTD_FRAME)) // || wdType==RIQUESTR_FRAME || wdType==REQUESTX_FRAME || wdType==REQUESTY_FRAME
		{
			return 1;
		}
		//'CT'
		memcpy(&wdVal,pbuff+ip,sizeof(wdVal));
		ip+=sizeof(wdVal);
		if(wdVal!=REQUEST_OP )
		{
			return 1;
		}
		//读入UI flag
		memcpy(&wdVal,pbuff+ip,sizeof(wdVal));
		ip+=sizeof(wdVal);
		if(wdVal!=USERID_FRAME)
		{
			return 1;
		}
		//读入user id;
		memcpy(&UserId,pbuff+ip,sizeof(UserId));
#ifdef _SELF_DATA_
		UserId=ReqId;
#endif
		break;
	default:
		return 0;
		break;
	}
#ifdef _SELF_DATA_
	{
		BYTE bufft[1024];
		int uLen;
		WORD	wdValt;
		BYTE	btValt;
		DWORD	dwValt;
		float	ftValt;
		static	dwDate=20010101;
		
		
		memset(bufft,0,1024);
		
		wdValt=1;
		memcpy(bufft,&wdValt,2);		//id no
		memcpy(bufft+2,&UserId,8);		//user id
		wdValt='WS';
		memcpy(bufft+10,&wdValt,2);		//WS flag
		dwValt=31;
		memcpy(bufft+12,&dwValt,4);		//Frame length
		wdValt='DF';
		memcpy(bufft+16,&wdValt,2);		//DF
		wdValt=16;
		memcpy(bufft+18,&wdValt,2);		//data length
		wdValt='UI';
		memcpy(bufft+20,&wdValt,2);		//UI flag
		memcpy(bufft+22,&UserId,8);		//user id
		wdValt='DA';
		memcpy(bufft+30,&wdValt,2);		//DF

		wdValt=11;
		memcpy(bufft+32,&wdValt,2);		//Frame Length
		wdValt='SY';
		memcpy(bufft+34,&wdValt,2);		//'C2'
		btValt='A';
		memcpy(bufft+36,&btValt,1);		//Group code
		wdValt='DT';
		memcpy(bufft+37,&wdValt,2);		//'C2'
		dwValt=m_TodayData.GetTodayDate();
		memcpy(bufft+39,&dwValt,4);		//Date
		wdValt=8*1800+45*30;
		memcpy(bufft+43,&wdValt,2);		//Time

//		m_pOutIMultiAS->Write(bufft,45,&uLen);
		
	}
#endif
	//读入数据
	pReq=pbuff+10+Daof+2;
	nLen=nLength-10-Daof-2;
//	pReq=pbuff+12+Daof+2;
//	nLen=nLength-12-Daof-2;
//	ATLTRACE(">>>>Get a request.\n");
//	m_TodayData.m_RequestNum++;
	try
	{
		if(wdType == REQUESTR_FRAME) //  || wdType == RIQUESTR_FRAME
			HandleFrame_Request(pReq,nLen,ReqId,UserId);
		else
			HandleFrame_Request_Connect(pReq,nLen,ReqId,UserId);
	}
	catch(...)
	{
		int n=GetLastError();
	}
	return 0;

	return FALSE ;

}

BYTE CQSNative_Option::HandleFrame_Request(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId)
{
	STProtocol_DO_HEAD	StHead;
	WORD wdVal;

	if(nLength<sizeof(StHead))
	{
		return 1;
	}
	memcpy(&StHead,pBuff,sizeof(StHead));

	if(StHead.wLen != nLength - 4)
		return 1;
	memcpy(&wdVal,pBuff+StHead.wLen + 2,2);
	if(wdVal != 'ED')
		return 1;		

	memset(m_pOutBuff,0,MAX_OUTFRAME_SIZE);	
	switch(StHead.SubFlag)
	{
//	case 'RT': //Request Code Table
//		HandleFrame_Request_RT(pBuff,nLength,ReqId,UserId);
//		break;
	//case 'RI': //Request IV
	//	SendIVRequest(StHead.cbGroupCode, StHead.szItemCode, StHead.lTransdate, ReqId, UserId);
		break;
	case 'RQ': //Request Data
		HandleFrame_Request_RQ(pBuff,nLength,ReqId,UserId);
		break;
	case 'RD': //Request date
		HandleFrame_Request_RD(pBuff,nLength,ReqId,UserId);
		break;
	}
	return 0;
}

BYTE CQSNative_Option::HandleFrame_Request_Connect(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId)
{
	STProtocol_DO_HEAD	StHead;
	WORD wdVal;

	if(nLength<sizeof(StHead))
	{
		return 1;
	}
	memcpy(&StHead,pBuff,sizeof(StHead));

	if(StHead.wLen != nLength - 4)
		return 1;
	memcpy(&wdVal,pBuff+StHead.wLen + 2,2);
	if(wdVal != 'ED')
		return 1;		

	memset(m_pOutBuff,0,MAX_OUTFRAME_SIZE);	
	switch(StHead.SubFlag)
	{
	case 'UC': //connect update cannal
		{
			HandleFrame_Request_UC(pBuff,nLength,ReqId,UserId);

			if (StHead.cbGroupCode>='A' && strlen(StHead.szItemCode)>0)
			{
				SendIVRequest(StHead.cbGroupCode, StHead.szItemCode, StHead.lTransdate, ReqId, UserId);
				SendPreOIRequest(StHead.cbGroupCode, StHead.szItemCode, StHead.lTransdate, ReqId, UserId);
			}
		}
		break;
	case 'UD': //disconnect update cannal
		HandleFrame_Request_UD(pBuff,nLength,ReqId,UserId);
		break;
	case 'UX': 
		{
			HandleFrame_Request_UX(pBuff,nLength,ReqId,UserId);
			if (StHead.cbGroupCode>='A' && strlen(StHead.szItemCode)>0)
				SendLastPriceRequest(StHead.cbGroupCode, StHead.szItemCode, StHead.lTransdate, ReqId, UserId);
		}
		break;
	case 'UY': 
		HandleFrame_Request_UY(pBuff,nLength,ReqId,UserId);
		break;
	case 'UE': 
		{
			HandleFrame_Request_UE(pBuff,nLength,ReqId,UserId);
			if (StHead.cbGroupCode>='A' && strlen(StHead.szItemCode)>0)
				SendIVRequest(StHead.cbGroupCode, StHead.szItemCode, StHead.lTransdate, ReqId, UserId);
		}
		break;
	case 'UF': 
		HandleFrame_Request_UF(pBuff,nLength,ReqId,UserId);
		break;
	}
	return 0;
}

void CQSNative_Option::SendIVEveryMin(unsigned char cbGroupCode,LPCTSTR pszItemCode,long lTransdate,DWORD channelID)
{
	DayIVs temp;
	memset(&temp,0,sizeof(DayIVs));
	GetIV((LPSTR)(LPCTSTR)pszItemCode,temp);
	if (temp.month[0]==0)
	{
		return;
	}

	STProtocol_SR_RET2 StPrt;
	//	StPrt.ReqIdNum =1;
	//	StPrt.ReqID = 0;
	StPrt.headMark = 'AA55';
	StPrt.BegFlag = 'WS';
	StPrt.DFFlag = 'DF';
	StPrt.DataOff = 14;
	StPrt.CIFlag = 'CI';	
	StPrt.ChannelID = channelID;	
	StPrt.DAFlag = 'DA';	
	
	FrameLen	framelen;
	FrameHead	framehead;
	FrameID		frameid;
	
	framehead.wFrameType = 'DO';
	frameid.cbGroupCode = cbGroupCode;
	memcpy(frameid.szItemCode, pszItemCode, G_GENCODE__LEN);
	
	frameid.fStrikePrice = temp.close;
	if (lTransdate==0)
	{
		frameid.lTransdate = temp.month[0];
	}
	else
		frameid.lTransdate = lTransdate;
	frameid.cbOrder = 0;
	
	long offset = sizeof(FrameLen); 
	
	//memcpy(m_pOutBuff+sizeof(StPrt),&framelen, sizeof(FrameLen));
	//offset+=sizeof(FrameLen);
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&framehead, sizeof(FrameHead));
	offset+=sizeof(FrameHead);
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&frameid, sizeof(FrameID));
	offset+=sizeof(FrameID);
	WORD elementtype = 'IV';
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&elementtype, sizeof(ElementType));
	offset+=sizeof(elementtype);
	
	BYTE actualRowNum = 0;
	long off = offset;
	//memcpy(m_pOutBuff+sizeof(StPrt)+offset,&actualRowNum, sizeof(actualRowNum));
	offset+=sizeof(actualRowNum);
	
	float iv=0.0;
	int count=0;
	/*
	if (temp.iv[0]>0)
		count++;
	if (temp.iv[1]>0)
		count++;

	if (count==0)
		iv=0;
	else
		iv=(temp.iv[0]+temp.iv[1])/count;
	*/
	/*
	for (int i=1;i<TT_OPT_MAXMONTH;i++)
	{
		if (count>2)
		{
			break;
		}
		if (temp.month[i]%3==0 && temp.iv[i]!=0 && temp.month[i]!=0)
		{
			count++;
			iv+=temp.iv[i];
		}
	}
	*/
	iv=temp.iv[0];
	/*
	char logstr[MAX_PATH];
	sprintf(logstr,"SendIVEveryMin %.7s iv=%f month=%d",pszItemCode,iv,temp.month[0]);
	WriteLogFile(logstr);
	*/
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&iv,sizeof(float));
	offset+=sizeof(float);
	
	framelen = sizeof(FrameHead) + sizeof(FrameID) + sizeof(ElementType) + sizeof(char)
		+ sizeof(float);
	
	memcpy(m_pOutBuff+sizeof(StPrt),&framelen, sizeof(framelen));
	actualRowNum = count+1;
	memcpy(m_pOutBuff+sizeof(StPrt)+off,&actualRowNum, sizeof(actualRowNum));
	
	StPrt.Length = sizeof(StPrt) + offset - 10/*原来是12*/ + 2;
	StPrt.dataSize = sizeof(StPrt)+offset+2 - 8; //andy add 2007.08.31
	memcpy(m_pOutBuff,&StPrt,sizeof(StPrt));
	
	WORD wdVal = 'ED';
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&wdVal,sizeof(wdVal));
	
	SendOutFrame(m_pOutBuff,sizeof(StPrt)+offset+2);

}

BYTE CQSNative_Option::SendIVRequest(unsigned char cbGroupCode,LPCTSTR pszItemCode, long lTransdate, DOUBLE ReqId, DOUBLE UserId)
{
	BYTE RqType;
	
	DayIVs temp;
	memset(&temp,0,sizeof(DayIVs));
	GetIV((LPSTR)(LPCSTR)pszItemCode,temp);
	if (temp.month[0]==0)
	{
		return 0;
	}

	STProtocol_SR_RET StPrt;

	StPrt.headMark = 'AA55';
	StPrt.BegFlag = 'WS';
	StPrt.DFFlag = 'DF';
	StPrt.DataOff = 18;
	StPrt.UIFlag = 'UI';	
	StPrt.UserID = UserId;	
	StPrt.DAFlag = 'DA';	
	
	FrameLen	framelen;
	FrameHead	framehead;
	FrameID		frameid;
	
	framehead.wFrameType = 'DO';
	frameid.cbGroupCode = cbGroupCode;
	memcpy(frameid.szItemCode, pszItemCode, G_GENCODE__LEN);
	
	frameid.fStrikePrice = temp.close;
	if (lTransdate==0)
	{
		frameid.lTransdate = temp.month[0];
	}
	else
		frameid.lTransdate = lTransdate;
	frameid.cbOrder = 0;
	
	long offset = sizeof(FrameLen); 
	
	//memcpy(m_pOutBuff+sizeof(StPrt),&framelen, sizeof(FrameLen));
	//offset+=sizeof(FrameLen);
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&framehead, sizeof(FrameHead));
	offset+=sizeof(FrameHead);
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&frameid, sizeof(FrameID));
	offset+=sizeof(FrameID);
	WORD elementtype = 'IV';
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&elementtype, sizeof(ElementType));
	offset+=sizeof(elementtype);
	
	BYTE actualRowNum = 0;
	long off = offset;
	//memcpy(m_pOutBuff+sizeof(StPrt)+offset,&actualRowNum, sizeof(actualRowNum));
	offset+=sizeof(actualRowNum);

	float iv=0.0;
	int count=0;
	/*
	if (temp.iv[0]>0)
		count++;
	if (temp.iv[1]>0)
		count++;

	if (count==0)
		iv=0;
	else
		iv=(temp.iv[0] + temp.iv[1])/2;
	*/
	/*
	for (int i=1;i<TT_OPT_MAXMONTH;i++)
	{
		if (count>2)
		{
			break;
		}
		if (temp.month[i]%3==0 && temp.iv[i]!=0 && temp.month[i]!=0)
		{
			count++;
			iv+=temp.iv[i];
		}
	}
	*/
	iv=temp.iv[0];
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&iv,sizeof(float));
	offset+=sizeof(float);
	
	framelen = sizeof(FrameHead) + sizeof(FrameID) + sizeof(ElementType) + sizeof(char)
			 + sizeof(float);

	memcpy(m_pOutBuff+sizeof(StPrt),&framelen, sizeof(framelen));
	actualRowNum = count+1;
	memcpy(m_pOutBuff+sizeof(StPrt)+off,&actualRowNum, sizeof(actualRowNum));
	
	StPrt.Length = sizeof(StPrt) + offset - 10/*原来是12*/ + 2;
	StPrt.dataSize = sizeof(StPrt)+offset+2 - 8; //andy add 2007.08.31
	memcpy(m_pOutBuff,&StPrt,sizeof(StPrt));
	
	WORD wdVal = 'ED';
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&wdVal,sizeof(wdVal));
	/*
	char logstr[255];
	sprintf(logstr,"SendIVRequest  %.6s %d %f",pszItemCode,lTransdate,iv);
	WriteLogFile(logstr);
	*/
	SendOutFrame(m_pOutBuff,sizeof(StPrt)+offset+2);

	return 0;	
}

BYTE CQSNative_Option::SendLastPriceRequest(unsigned char cbGroupCode,LPCTSTR pszItemCode, long lTransdate, DOUBLE ReqId, DOUBLE UserId)
{
	int i;
	int nIndex,nMonthIdx;
	if((nIndex = EnumOptionItemCodeEx(pszItemCode, cbGroupCode)) == -1)
		return false;
	if((nMonthIdx = FindMonthEx(nIndex, lTransdate)) == -1)
		return false;

	BYTE RqType;

	STProtocol_SR_RET StPrt;

	StPrt.headMark = 'AA55';
	StPrt.BegFlag = 'WS';
	StPrt.DFFlag = 'DF';
	StPrt.DataOff = 18;
	StPrt.UIFlag = 'UI';	
	StPrt.UserID = UserId;	
	StPrt.DAFlag = 'DA';	
	
	FrameLen	framelen;
	FrameHead	framehead;
	FrameID		frameid;
	
	framehead.wFrameType = 'DP';
	frameid.cbGroupCode = cbGroupCode;
	memcpy(frameid.szItemCode, pszItemCode, G_GENCODE__LEN);
	
	frameid.fStrikePrice = 0;
	frameid.lTransdate = lTransdate;
	frameid.cbOrder = 0;
	
	long offset = sizeof(FrameLen); 
	
	//memcpy(m_pOutBuff+sizeof(StPrt),&framelen, sizeof(FrameLen));
	//offset+=sizeof(FrameLen);
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&framehead, sizeof(FrameHead));
	offset+=sizeof(FrameHead);
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&frameid, sizeof(FrameID));
	offset+=sizeof(FrameID);
	WORD elementtype = 'XP';
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&elementtype, sizeof(ElementType));
	offset+=sizeof(elementtype);
	
	BYTE actualRowNum = 0;
	long off = offset;
	//memcpy(m_pOutBuff+sizeof(StPrt)+offset,&actualRowNum, sizeof(actualRowNum));
	offset+=sizeof(actualRowNum);

	int nRowNum = m_optFm[nIndex].m_optMonth[nMonthIdx].ucTotalRow; 
	for(i = 0; i < nRowNum; i++)
	{
		if (m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_fStrike>0)
		{
			actualRowNum++;
			memcpy(m_pOutBuff+sizeof(StPrt)+offset,&m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_fStrike, sizeof(float));
			offset+=sizeof(float);

			memcpy(m_pOutBuff+sizeof(StPrt)+offset,&m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_dat[0].fLast, sizeof(float));
			offset+=sizeof(float);

			unsigned char cbIndicator = m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_dat[0].ucCabLast << 1;
			memcpy(m_pOutBuff+sizeof(StPrt)+offset,&cbIndicator, sizeof(unsigned char));
			offset+=sizeof(unsigned char);

			memcpy(m_pOutBuff+sizeof(StPrt)+offset,&m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_dat[1].fLast, sizeof(float));
			offset+=sizeof(float);
			
			cbIndicator = m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_dat[1].ucCabLast << 1;
			memcpy(m_pOutBuff+sizeof(StPrt)+offset,&cbIndicator, sizeof(unsigned char));
			offset+=sizeof(unsigned char);
			/*
			char logstr[255];
			sprintf(logstr,"SendLast  %.6s %d strike=%f calllast=%f putlast=%f",pszItemCode,lTransdate,
					m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_fStrike,
					m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_dat[0].fLast,
					m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_dat[1].fLast);
			WriteLogFile(logstr);
			*/
		}	
	}

	framelen = offset - 2;;

	memcpy(m_pOutBuff+sizeof(StPrt),&framelen, sizeof(framelen));
	memcpy(m_pOutBuff+sizeof(StPrt)+off,&actualRowNum, sizeof(actualRowNum));
	
	StPrt.Length = sizeof(StPrt) + offset - 10/*原来是12*/ + 2;
	StPrt.dataSize = sizeof(StPrt)+offset+2 - 8; //andy add 2007.08.31
	memcpy(m_pOutBuff,&StPrt,sizeof(StPrt));
	
	WORD wdVal = 'ED';
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&wdVal,sizeof(wdVal));
	/*
	char logstr[255];
	sprintf(logstr,"SendIVRequest  %.6s %d %f",pszItemCode,lTransdate,iv);
	WriteLogFile(logstr);
	*/
	SendOutFrame(m_pOutBuff,sizeof(StPrt)+offset+2);

	return 0;	
}

BYTE CQSNative_Option::HandleFrame_Request_RQ(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId)
{
	BYTE RqType;

	STProtocol_DO_HEAD	StHead;
	memcpy(&StHead,pBuff,sizeof(StHead));
	memcpy(&RqType,pBuff+sizeof(StHead),sizeof(RqType));
	/*
	if (RqType == 'I')
	{
		SendIVRequest(StHead.cbGroupCode, StHead.szItemCode, StHead.lTransdate, ReqId, UserId);
		return 0;
	}
	*/
	if(RqType != 'H')
		return 1;
	
	if(StHead.lTransdate == 0)
		SendAllHistoricalData(StHead.cbGroupCode, StHead.szItemCode, StHead.lTransdate, ReqId, UserId);
	else
		SendHistoricalData(StHead.cbGroupCode, StHead.szItemCode, StHead.lTransdate, ReqId, UserId);
		
	return 0;	
}


BYTE CQSNative_Option::HandleFrame_Request_RD(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId)
{

	STProtocol_DO_HEAD	StHead;
	memcpy(&StHead,pBuff,sizeof(StHead));

	if(StHead.lTransdate == 0)
		SendAllAvailableMonth(StHead.cbGroupCode,StHead.szItemCode ,StHead.lTransdate,ReqId ,UserId);
	else
		SendAvailableMonth(StHead.cbGroupCode,StHead.szItemCode ,StHead.lTransdate,ReqId ,UserId);

	return 0;	
}

BYTE CQSNative_Option::HandleFrame_Request_UC(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId)
{
	STProtocol_DO_HEAD	StHead;
	memcpy(&StHead,pBuff,sizeof(StHead));

	char cKey[20];
	memset(cKey,0,sizeof(cKey));
	memcpy(cKey,&StHead.cbGroupCode,1); //groupcode + itemcode + lDate
	memcpy(cKey+1,&StHead.szItemCode,8); //groupcode + itemcode + lDate
	char cDate[10];
	sprintf(cDate,"%08d",StHead.lTransdate); 
	strcat(cKey,cDate);
	CString strUser = cKey;
	OptUserInfo usinfo;

	if(m_UserList.Lookup((LPCTSTR)strUser,usinfo) == TRUE)
	{
		usinfo.dwCount ++;		
	}
	else
	{
		usinfo.GroupCode = StHead.cbGroupCode ;
		memcpy(usinfo.szItemCode ,StHead.szItemCode,8);
		usinfo.lTransDate = StHead.lTransdate ; 
		usinfo.dwChannelID = m_dwId;
		m_dwId++;
		usinfo.dwCount = 1 ;
	}
	
	STProtocol_UC_RET StPrt;
//	StPrt.ReqIdNum =1;
//	StPrt.ReqID = ReqId;
	StPrt.headMark = 'AA55';
	StPrt.BegFlag = 'WS';
	StPrt.Length = sizeof(StPrt) - 10/*原来是12*/;
	StPrt.ReqMode = 'UC';
	StPrt.ReqType = 'TE';
	StPrt.UIFlag = 'UI';
	StPrt.UserID = UserId;
	StPrt.CIFlag = 'CI';
	StPrt.ChannelID = usinfo.dwChannelID;
	StPrt.EndFlag = 'ED';

	StPrt.dataSize = sizeof(StPrt) - 8; //andy add 2007.08.31
	SendOutFrame((BYTE*)&StPrt,sizeof(StPrt));
		
	EnterCriticalSection(&m_csUser);
	m_UserList.SetAt((LPCTSTR)strUser, usinfo);	
	LeaveCriticalSection(&m_csUser);

	return 0;
}

BYTE CQSNative_Option::HandleFrame_Request_UX(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId)
{
	STProtocol_DO_HEAD	StHead;
	memcpy(&StHead,pBuff,sizeof(StHead));
	
	char cKey[20];
	memset(cKey,0,sizeof(cKey));
	memcpy(cKey,&StHead.cbGroupCode,1); //groupcode + itemcode + lDate
	memcpy(cKey+1,&StHead.szItemCode,8); //groupcode + itemcode + lDate
	char cDate[10];
	sprintf(cDate,"%08d",StHead.lTransdate); 
	strcat(cKey,cDate);
	CString strUser = cKey;
	OptUserInfo usinfo;
	
	if(m_UserList2.Lookup((LPCTSTR)strUser,usinfo) == TRUE)
	{
		usinfo.dwCount ++;		
	}
	else
	{
		usinfo.GroupCode = StHead.cbGroupCode ;
		memcpy(usinfo.szItemCode ,StHead.szItemCode,8);
		usinfo.lTransDate = StHead.lTransdate ; 
		usinfo.dwChannelID = m_dwId;
		m_dwId++;
		usinfo.dwCount = 1 ;
	}
	
	STProtocol_UC_RET StPrt;
	//	StPrt.ReqIdNum =1;
	//	StPrt.ReqID = ReqId;
	StPrt.headMark = 'AA55';
	StPrt.BegFlag = 'WS';
	StPrt.Length = sizeof(StPrt) - 10/*原来是12*/;
	StPrt.ReqMode = 'UX';
	StPrt.ReqType = 'TE';
	StPrt.UIFlag = 'UI';
	StPrt.UserID = UserId;
	StPrt.CIFlag = 'CI';
	StPrt.ChannelID = usinfo.dwChannelID;
	StPrt.EndFlag = 'ED';
	
	StPrt.dataSize = sizeof(StPrt) - 8; //andy add 2007.08.31
	SendOutFrame((BYTE*)&StPrt,sizeof(StPrt));
	
	EnterCriticalSection(&m_csUser);
	m_UserList2.SetAt((LPCTSTR)strUser, usinfo);	
	LeaveCriticalSection(&m_csUser);
	
	return 0;
}

BYTE CQSNative_Option::HandleFrame_Request_UE(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId)
{
	STProtocol_DO_HEAD	StHead;
	memcpy(&StHead,pBuff,sizeof(StHead));
	
	char cKey[20];
	memset(cKey,0,sizeof(cKey));
	memcpy(cKey,&StHead.cbGroupCode,1); //groupcode + itemcode + lDate
	memcpy(cKey+1,&StHead.szItemCode,8); //groupcode + itemcode + lDate
	char cDate[10];
	sprintf(cDate,"%08d",StHead.lTransdate); 
	strcat(cKey,cDate);
	CString strUser = cKey;
	OptUserInfo usinfo;
	
	if(m_UserList3.Lookup((LPCTSTR)strUser,usinfo) == TRUE)
	{
		usinfo.dwCount ++;		
	}
	else
	{
		usinfo.GroupCode = StHead.cbGroupCode ;
		memcpy(usinfo.szItemCode ,StHead.szItemCode,8);
		usinfo.lTransDate = StHead.lTransdate ; 
		usinfo.dwChannelID = m_dwId;
		m_dwId++;
		usinfo.dwCount = 1 ;
	}
	
	STProtocol_UC_RET StPrt;
	//	StPrt.ReqIdNum =1;
	//	StPrt.ReqID = ReqId;
	StPrt.headMark = 'AA55';
	StPrt.BegFlag = 'WS';
	StPrt.Length = sizeof(StPrt) - 10/*原来是12*/;
	StPrt.ReqMode = 'UE';
	StPrt.ReqType = 'TE';
	StPrt.UIFlag = 'UI';
	StPrt.UserID = UserId;
	StPrt.CIFlag = 'CI';
	StPrt.ChannelID = usinfo.dwChannelID;
	StPrt.EndFlag = 'ED';
	
	StPrt.dataSize = sizeof(StPrt) - 8; //andy add 2007.08.31
	SendOutFrame((BYTE*)&StPrt,sizeof(StPrt));
	
	EnterCriticalSection(&m_csUser);
	m_UserList3.SetAt((LPCTSTR)strUser, usinfo);	
	LeaveCriticalSection(&m_csUser);
	
	return 0;
}

BYTE CQSNative_Option::HandleFrame_Request_UD(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId)
{
	STProtocol_DO_HEAD	StHead;
	memcpy(&StHead,pBuff,sizeof(StHead));

	char cKey[20];
	memset(cKey,0,sizeof(cKey));
	memcpy(cKey,&StHead.cbGroupCode,1); //groupcode + itemcode + lDate
	memcpy(cKey+1,&StHead.szItemCode,8); //groupcode + itemcode + lDate
	char cDate[10];
	sprintf(cDate,"%08d",StHead.lTransdate); 
	strcat(cKey,cDate);
	CString strUser = cKey;
	OptUserInfo usinfo;

	if(m_UserList.Lookup((LPCTSTR)strUser,usinfo) == TRUE)
	{
		if(usinfo.dwCount > 0)
		{
			usinfo.dwCount --;				
			
			EnterCriticalSection(&m_csUser);		
			m_UserList.SetAt((LPCTSTR)strUser, usinfo);	
			LeaveCriticalSection(&m_csUser);
			
			STProtocol_UC_RET StPrt;
		//	StPrt.ReqIdNum =1;
		//	StPrt.ReqID = ReqId;
			StPrt.headMark = 'AA55';
			StPrt.BegFlag = 'WS';
			StPrt.Length = sizeof(StPrt) - 10/*原来是12*/;
			StPrt.ReqMode = 'UD';
			StPrt.ReqType = 'TE';
			StPrt.UIFlag = 'UI';
			StPrt.UserID = UserId;
			StPrt.CIFlag = 'CI';
			StPrt.ChannelID = usinfo.dwChannelID;
			StPrt.EndFlag = 'ED';

			StPrt.dataSize = sizeof(StPrt) - 8; //andy add 2007.08.31
			SendOutFrame((BYTE*)&StPrt,sizeof(StPrt));
		}
	}

	return 0;
}

BYTE CQSNative_Option::HandleFrame_Request_UY(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId)
{
	STProtocol_DO_HEAD	StHead;
	memcpy(&StHead,pBuff,sizeof(StHead));
	
	char cKey[20];
	memset(cKey,0,sizeof(cKey));
	memcpy(cKey,&StHead.cbGroupCode,1); //groupcode + itemcode + lDate
	memcpy(cKey+1,&StHead.szItemCode,8); //groupcode + itemcode + lDate
	char cDate[10];
	sprintf(cDate,"%08d",StHead.lTransdate); 
	strcat(cKey,cDate);
	CString strUser = cKey;
	OptUserInfo usinfo;
	
	if(m_UserList2.Lookup((LPCTSTR)strUser,usinfo) == TRUE)
	{
		if(usinfo.dwCount > 0)
		{
			usinfo.dwCount --;				
			
			EnterCriticalSection(&m_csUser);		
			m_UserList2.SetAt((LPCTSTR)strUser, usinfo);	
			LeaveCriticalSection(&m_csUser);
			
			STProtocol_UC_RET StPrt;
			//	StPrt.ReqIdNum =1;
			//	StPrt.ReqID = ReqId;
			StPrt.headMark = 'AA55';
			StPrt.BegFlag = 'WS';
			StPrt.Length = sizeof(StPrt) - 10/*原来是12*/;
			StPrt.ReqMode = 'UY';
			StPrt.ReqType = 'TE';
			StPrt.UIFlag = 'UI';
			StPrt.UserID = UserId;
			StPrt.CIFlag = 'CI';
			StPrt.ChannelID = usinfo.dwChannelID;
			StPrt.EndFlag = 'ED';
			
			StPrt.dataSize = sizeof(StPrt) - 8; //andy add 2007.08.31
			SendOutFrame((BYTE*)&StPrt,sizeof(StPrt));
		}
	}
	
	return 0;
}

BYTE CQSNative_Option::HandleFrame_Request_UF(BYTE *pBuff, int nLength, DOUBLE ReqId, DOUBLE UserId)
{
	STProtocol_DO_HEAD	StHead;
	memcpy(&StHead,pBuff,sizeof(StHead));
	
	char cKey[20];
	memset(cKey,0,sizeof(cKey));
	memcpy(cKey,&StHead.cbGroupCode,1); //groupcode + itemcode + lDate
	memcpy(cKey+1,&StHead.szItemCode,8); //groupcode + itemcode + lDate
	char cDate[10];
	sprintf(cDate,"%08d",StHead.lTransdate); 
	strcat(cKey,cDate);
	CString strUser = cKey;
	OptUserInfo usinfo;
	
	if(m_UserList3.Lookup((LPCTSTR)strUser,usinfo) == TRUE)
	{
		if(usinfo.dwCount > 0)
		{
			usinfo.dwCount --;				
			
			EnterCriticalSection(&m_csUser);		
			m_UserList3.SetAt((LPCTSTR)strUser, usinfo);	
			LeaveCriticalSection(&m_csUser);
			
			STProtocol_UC_RET StPrt;
			//	StPrt.ReqIdNum =1;
			//	StPrt.ReqID = ReqId;
			StPrt.headMark = 'AA55';
			StPrt.BegFlag = 'WS';
			StPrt.Length = sizeof(StPrt) - 10/*原来是12*/;
			StPrt.ReqMode = 'UF';
			StPrt.ReqType = 'TE';
			StPrt.UIFlag = 'UI';
			StPrt.UserID = UserId;
			StPrt.CIFlag = 'CI';
			StPrt.ChannelID = usinfo.dwChannelID;
			StPrt.EndFlag = 'ED';
			
			StPrt.dataSize = sizeof(StPrt) - 8; //andy add 2007.08.31
			SendOutFrame((BYTE*)&StPrt,sizeof(StPrt));
		}
	}
	
	return 0;
}

HRESULT CQSNative_Option::SendOutFrame(BYTE *pbuff, int nLen)
{
    //buffer都不大，不压缩
	
	BOOL ret =  SendData_W2t( pbuff, nLen ) ;
/*
	int		len;
#ifndef _NEW_NETWORKS_COM_ //added by jansen20030628(
	m_pOutIMultiAS->Write(pbuff,nLen,&len);
#else                      //新的网络模块
    //+----new added by jansen20030628-----------+
	if(m_pNetCommObjSrv!=NULL)
	{
	    BYTE * p=NULL;
		double dConnID=0;
		int nCount=0;

		HRESULT hRet=S_FALSE;
        int nSize=nLen-sizeof(WORD)-sizeof(double);
		p=pbuff;
		p+=sizeof(WORD);
        dConnID=*(double*)p;
		p+=sizeof(double);
        
		if(dConnID==0)//取全部列表.
		{
		   POSITION pos=NULL;
		   pos=m_mapSrvHash.GetStartPosition();
		   while(pos)
		   {
			   m_mapSrvHash.GetNextAssoc(pos,dConnID,nCount);
			   hRet=m_pNetCommObjSrv->ServerSendData(dConnID,p,nSize,LEVEL_HIGH,COMM_SEND_BUF);	
			   if(hRet!=S_OK)
			   {
				   GetDescFromID(dConnID,m_szIP,m_nPort);
				   sprintf(m_szTxt,"OptionsSrv send DataPacket to Door Failed size:%d Door(%s:%d) Error:%d",nSize,m_szIP,m_nPort,hRet);
				   WriteLogFile(m_szTxt);
			   }
		   }
		}
		else//当前的connID;
		{
			hRet=m_pNetCommObjSrv->ServerSendData(dConnID,p,nSize,LEVEL_HIGH,COMM_SEND_BUF);	
			if(hRet!=S_OK)
			{
				GetDescFromID(dConnID,m_szIP,m_nPort);
				sprintf(m_szTxt,"OptionsSrv send DataPacket to Door Failed size:%d Door(%s:%d) Error:%d",nSize,m_szIP,m_nPort,hRet);
				WriteLogFile(m_szTxt);
			}
		}
	}
	//+------end new added by jansen20030628---------+
	   
#endif*/

	return 0;
}


void CQSNative_Option::SendAllHistoricalData(unsigned char cbGroupCode, LPCTSTR pszItemCode, long lTransdate,
										  DOUBLE ReqId, DOUBLE UserId)
{	
	int nMonthNum = 0;
	int nIndex = EnumOptionItemCodeEx(pszItemCode, cbGroupCode);
	
	if(nIndex == -1)
		return ;

	nMonthNum = m_optFm[nIndex].ucMonthNum;
	if(nMonthNum == 0)
		return ;

	for(int i=0;i<nMonthNum;i++)
	{
		lTransdate = m_optFm[nIndex].m_optMonth[i].lTransDate;  
		SendHistoricalData(cbGroupCode, pszItemCode, lTransdate, ReqId, UserId);				
	}
}

void CQSNative_Option::SendHistoricalData(unsigned char cbGroupCode, LPCTSTR pszItemCode, long lTransdate,
										 DOUBLE ReqId, DOUBLE UserId)
{
	int i;
	int nIndex,nMonthIdx;
	if((nIndex = EnumOptionItemCodeEx(pszItemCode, cbGroupCode)) == -1)
		return;
	if((nMonthIdx = FindMonthEx(nIndex, lTransdate)) == -1)
		return;	
	
	STProtocol_SR_RET StPrt;

//	StPrt.ReqIdNum =1;
//	StPrt.ReqID = ReqId;
	StPrt.headMark = 'AA55';
	StPrt.BegFlag = 'WS';
	StPrt.DFFlag = 'DF';
	StPrt.DataOff = 18;
	StPrt.UIFlag = 'UI';	
	StPrt.UserID = UserId;	
	StPrt.DAFlag = 'DA';	

	FrameLen	framelen;
	FrameHead	framehead;
	FrameID		frameid;
	
	framehead.wFrameType = 'DO';
	frameid.cbGroupCode = cbGroupCode;
	memcpy(frameid.szItemCode, pszItemCode, G_GENCODE__LEN);

	frameid.fStrikePrice = 0;
	frameid.lTransdate = lTransdate;
	frameid.cbOrder = 0;

	long offset = sizeof(FrameLen); 

	//memcpy(m_pOutBuff+sizeof(StPrt),&framelen, sizeof(FrameLen));
	//offset+=sizeof(FrameLen);
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&framehead, sizeof(FrameHead));
	offset+=sizeof(FrameHead);
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&frameid, sizeof(FrameID));
	offset+=sizeof(FrameID);
	WORD elementtype = 'ST';
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&elementtype, sizeof(ElementType));
	offset+=sizeof(elementtype);

	int nRowNum = m_optFm[nIndex].m_optMonth[nMonthIdx].ucTotalRow; 
	BYTE actualRowNum = 0;
	long off = offset;
	//memcpy(m_pOutBuff+sizeof(StPrt)+offset,&actualRowNum, sizeof(actualRowNum));
	offset+=sizeof(actualRowNum);
	long BuffLen;
	
	for(i = 0; i < nRowNum; i++)
	{
		if(m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_fStrike != 0.0)
		{			
			actualRowNum ++;
			GetDataHistory(&m_optFm[nIndex], &m_optFm[nIndex].m_optMonth[nMonthIdx], 
				&m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i], 0,(char*)m_pOutBuff+sizeof(StPrt)+offset, BuffLen);
			offset+=BuffLen;
			memcpy(m_pOutBuff+sizeof(StPrt)+offset,&m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_fStrike,sizeof(float));
			offset += sizeof(float);
			memcpy(m_pOutBuff+sizeof(StPrt)+offset, &m_optFm[nIndex].m_optMonth[nMonthIdx].cDecimal, 1);
			offset += 1;
			GetDataHistory(&m_optFm[nIndex], &m_optFm[nIndex].m_optMonth[nMonthIdx], 
				&m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i], 1, (char*)m_pOutBuff+sizeof(StPrt)+offset, BuffLen);
			offset+=BuffLen;
			/*
			CString asd;
			asd.Format("%s",pszItemCode);
			if (asd.Find("00688",0)>=0 && m_optFm[nIndex].m_optMonth[nMonthIdx].lTransDate==201711)
			{
				asd.Format("%.20s-%d ask=%.6f,bid=%.6f,asknum=%.6f,binnum=%f,strike=%f",pszItemCode,0,
					m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_dat[0].fAsk,
					m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_dat[0].fBid,
					m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_dat[0].fAskQty,
					m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_dat[0].fBidQty,
					m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_fStrike);
				WriteLogFile((LPSTR)(LPCSTR)asd);
			}
			asd.Format("%s",pszItemCode);
			if (asd.Find("00688",0)>=0 && m_optFm[nIndex].m_optMonth[nMonthIdx].lTransDate==201711)
			{
				asd.Format("%.20s-%d ask=%.6f,bid=%.6f,asknum=%.6f,binnum=%f,strike=%f",pszItemCode,1,
					m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_dat[1].fAsk,
					m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_dat[1].fBid,
					m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_dat[1].fAskQty,
					m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_dat[1].fBidQty,
					m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_fStrike);
				WriteLogFile((LPSTR)(LPCSTR)asd);
			}
			*/
		}
	}
	
	framelen = offset - 2;

	memcpy(m_pOutBuff+sizeof(StPrt),&framelen, sizeof(framelen));
	memcpy(m_pOutBuff+sizeof(StPrt)+off,&actualRowNum, sizeof(actualRowNum));

	StPrt.Length = sizeof(StPrt) + offset - 10/*原来是12*/ + 2;
	StPrt.dataSize = sizeof(StPrt)+offset+2 - 8; //andy add 2007.08.31
	memcpy(m_pOutBuff,&StPrt,sizeof(StPrt));
	
	WORD wdVal = 'ED';
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&wdVal,sizeof(wdVal));

	SendOutFrame(m_pOutBuff,sizeof(StPrt)+offset+2);	
}

void CQSNative_Option::SendHistoricalData2(unsigned char cbGroupCode, LPCTSTR pszItemCode, long lTransdate,DWORD channelID)										 
{

	//发送整屏的即时数据
	int i;
	int nIndex,nMonthIdx;
	if((nIndex = EnumOptionItemCodeEx(pszItemCode, cbGroupCode)) == -1)
		return;
	if((nMonthIdx = FindMonthEx(nIndex, lTransdate)) == -1)
		return;	
	
	STProtocol_SR_RET2 StPrt;
//	StPrt.ReqIdNum =1;
//	StPrt.ReqID = 0;
	StPrt.headMark = 'AA55';
	StPrt.BegFlag = 'WS';
	StPrt.DFFlag = 'DF';
	StPrt.DataOff = 14;
	StPrt.CIFlag = 'CI';	
	StPrt.ChannelID = channelID;	
	StPrt.DAFlag = 'DA';	

	FrameLen	framelen;
	FrameHead	framehead;
	FrameID		frameid;
	
	framehead.wFrameType = 'DO';
	frameid.cbGroupCode = cbGroupCode;
	memcpy(frameid.szItemCode, pszItemCode, G_GENCODE__LEN);

	frameid.fStrikePrice = 0;
	frameid.lTransdate = lTransdate;
	frameid.cbOrder = 0;

	long offset = sizeof(FrameLen); 

	//memcpy(m_pOutBuff2+sizeof(StPrt),&framelen, sizeof(FrameLen));
	//offset+=sizeof(FrameLen);
	memcpy(m_pOutBuff2+sizeof(StPrt)+offset,&framehead, sizeof(FrameHead));
	offset+=sizeof(FrameHead);
	memcpy(m_pOutBuff2+sizeof(StPrt)+offset,&frameid, sizeof(FrameID));
	offset+=sizeof(FrameID);
	WORD elementtype = 'ST';
	memcpy(m_pOutBuff2+sizeof(StPrt)+offset,&elementtype, sizeof(ElementType));
	offset+=sizeof(elementtype);

	int nRowNum = m_optFm[nIndex].m_optMonth[nMonthIdx].ucTotalRow; 
	BYTE actualRowNum = 0;
	long off = offset;
	//memcpy(m_pOutBuff2+sizeof(StPrt)+offset,&actualRowNum, sizeof(actualRowNum));
	offset+=sizeof(actualRowNum);
	long BuffLen;

	for(i = 0; i < nRowNum; i++)
	{
		if(m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_fStrike != 0.0)
		{			
			actualRowNum ++;
			GetDataHistory(&m_optFm[nIndex], &m_optFm[nIndex].m_optMonth[nMonthIdx], 
				&m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i], 0,(char*)m_pOutBuff2+sizeof(StPrt)+offset, BuffLen);
			offset+=BuffLen;
			memcpy(m_pOutBuff2+sizeof(StPrt)+offset,&m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i].m_fStrike,sizeof(float));
			offset += sizeof(float);
			memcpy(m_pOutBuff2+sizeof(StPrt)+offset, &m_optFm[nIndex].m_optMonth[nMonthIdx].cDecimal, 1);
			offset += 1;
			GetDataHistory(&m_optFm[nIndex], &m_optFm[nIndex].m_optMonth[nMonthIdx], 
				&m_optFm[nIndex].m_optMonth[nMonthIdx].m_optLine[i], 1, (char*)m_pOutBuff2+sizeof(StPrt)+offset, BuffLen);
			offset+=BuffLen;

			if(offset > 89000)
			{
				WriteLogFile("Warning: SendHistoricalData2 buff overflow. data will lose");
				break;
			}
		}
	}
	
	framelen = offset - 2;

	memcpy(m_pOutBuff2+sizeof(StPrt),&framelen, sizeof(framelen));
	memcpy(m_pOutBuff2+sizeof(StPrt)+off,&actualRowNum, sizeof(actualRowNum));

	StPrt.Length = sizeof(StPrt) + offset - 10/*原来是12*/ + 2;
	StPrt.dataSize = sizeof(StPrt)+offset+2 - 8; //andy add 2007.08.31
	memcpy(m_pOutBuff2,&StPrt,sizeof(StPrt));
	
	WORD wdVal = 'ED';
	memcpy(m_pOutBuff2+sizeof(StPrt)+offset,&wdVal,sizeof(wdVal));

	SendOutFrame(m_pOutBuff2,sizeof(StPrt)+offset+2);	
}

HRESULT CQSNative_Option::SendAvailableMonthToUser(BYTE cbGroup,LPCTSTR pszItemCode,long lTransdate)
{//即时

	CString strUser;
	OptUserInfo usinfo;

	EnterCriticalSection(&m_csUser);		
	POSITION pos=m_UserList.GetStartPosition();
	int i=0;
	while(pos)
	{		
		m_UserList.GetNextAssoc(pos,strUser,usinfo);
		if(usinfo.dwCount > 0)
			SendAvailableMonth2(cbGroup, pszItemCode, lTransdate, usinfo.dwChannelID);	
	}	
	LeaveCriticalSection(&m_csUser);

	return S_OK;
}


HRESULT CQSNative_Option::SendAllAvailableMonth(BYTE cbGroup,LPCTSTR pszItemCode,long lTransdate,DOUBLE ReqId, DOUBLE UserId)
{
	for(int i = 0; i < m_nItemNum; i++)
	{
		SendAvailableMonth(m_optFm[i].cGCode, (char*)m_optFm[i].strICode, lTransdate, ReqId, UserId);	
	}	
	return S_OK;
}

HRESULT CQSNative_Option::SendAvailableMonth(BYTE cbGroup,LPCTSTR pszItemCode,long lTransdate,DOUBLE ReqId, DOUBLE UserId)
{
	STProtocol_SR_RET StPrt;

//	StPrt.ReqIdNum =1;
//	StPrt.ReqID = ReqId;
	StPrt.headMark = 'AA55';
	StPrt.BegFlag = 'WS';
	StPrt.DFFlag = 'DF';
	StPrt.DataOff = 18;
	StPrt.UIFlag = 'UI';	
	StPrt.UserID = UserId;	
	StPrt.DAFlag = 'DA';	

	FrameLen	framelen;
	FrameHead	framehead;
	FrameID		frameid;
	ElementType	elementtype;

	int nMonthNum = 0;
	int nIndex = EnumOptionItemCodeEx(pszItemCode, cbGroup);
	
	if(nIndex == -1)
	{
		return S_FALSE;
	}

	nMonthNum = m_optFm[nIndex].ucMonthNum;
	if(nMonthNum == 0)
	{
		return S_FALSE;
	}
	
	framelen = sizeof(FrameHead) + sizeof(FrameID) + sizeof(ElementType) + sizeof(char)
		+ sizeof(long) * nMonthNum;
	framehead.wFrameType = 'DO';
	memset(&frameid, NULL, sizeof(FrameID));
	frameid.cbGroupCode = m_optFm[nIndex].cGCode;
	memcpy(frameid.szItemCode, m_optFm[nIndex].strICode, G_GENCODE__LEN);
	frameid.lTransdate = lTransdate;
	
	long offset = 0; 

	memcpy(m_pOutBuff+sizeof(StPrt),&framelen, sizeof(FrameLen));
	offset+=sizeof(FrameLen);
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&framehead, sizeof(FrameHead));
	offset+=sizeof(FrameHead);
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&frameid, sizeof(FrameID));
	offset+=sizeof(FrameID);
	
	elementtype = 'DT';
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&elementtype, sizeof(ElementType));
	offset+=sizeof(elementtype);

	unsigned char cbMonthNum = nMonthNum; 
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&cbMonthNum, sizeof(char));
	offset+=sizeof(char);

//	CString logstr;
//	logstr.Format("%.8s,Monthnum=%d,List:",frameid.szItemCode,nMonthNum);

//	CString dest;
	for(int i = 0; i < nMonthNum; i ++)
	{
		memcpy(m_pOutBuff+sizeof(StPrt)+offset,&m_optFm[nIndex].m_optMonth[i].lTransDate, sizeof(long));
		offset+=sizeof(long);
//		dest.Format("%d,",m_optFm[nIndex].m_optMonth[i].lTransDate);
//		logstr+=dest;
	}

//	WriteLogFile((LPSTR)(LPCTSTR)logstr);

	StPrt.Length = sizeof(StPrt) + offset - 10/*原来是12*/ + 2;
	StPrt.dataSize = sizeof(StPrt)+offset+2 - 8; //andy add 2007.08.31
	memcpy(m_pOutBuff,&StPrt,sizeof(StPrt));
	
	WORD wdVal = 'ED';
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&wdVal,sizeof(wdVal));
	
	SendOutFrame(m_pOutBuff,sizeof(StPrt)+offset+2);	
	return S_OK;
}

HRESULT CQSNative_Option::SendAvailableMonth2(BYTE cbGroup,LPCTSTR pszItemCode,long lTransdate,DWORD channelID)
{//即时数据
	BYTE bf[1000];

	STProtocol_SR_RET2 StPrt;

//	StPrt.ReqIdNum =1;
//	StPrt.ReqID = 0;
	StPrt.headMark = 'AA55';
	StPrt.BegFlag = 'WS';
	StPrt.DFFlag = 'DF';
	StPrt.DataOff = 14;
	StPrt.CIFlag = 'CI';	
	StPrt.ChannelID = channelID;	
	StPrt.DAFlag = 'DA';		

	FrameLen	framelen;
	FrameHead	framehead;
	FrameID		frameid;
	ElementType	elementtype;

	int nMonthNum = 0;
	int nIndex = EnumOptionItemCodeEx(pszItemCode, cbGroup);
	
	if(nIndex == -1)
	{
		return S_FALSE;
	}

	nMonthNum = m_optFm[nIndex].ucMonthNum;
	if(nMonthNum == 0)
	{
		return S_FALSE;
	}
	
	framelen = sizeof(FrameHead) + sizeof(FrameID) + sizeof(ElementType) + sizeof(char)
		+ sizeof(long) * nMonthNum;
	framehead.wFrameType = 'DO';
	memset(&frameid, NULL, sizeof(FrameID));
	frameid.cbGroupCode = m_optFm[nIndex].cGCode;
	memcpy(frameid.szItemCode, m_optFm[nIndex].strICode, G_GENCODE__LEN);
	frameid.lTransdate = lTransdate;
	
	long offset = 0; 

	memcpy(bf+sizeof(StPrt),&framelen, sizeof(FrameLen));
	offset+=sizeof(FrameLen);
	memcpy(bf+sizeof(StPrt)+offset,&framehead, sizeof(FrameHead));
	offset+=sizeof(FrameHead);
	memcpy(bf+sizeof(StPrt)+offset,&frameid, sizeof(FrameID));
	offset+=sizeof(FrameID);
	
	elementtype = 'DT';
	memcpy(bf+sizeof(StPrt)+offset,&elementtype, sizeof(ElementType));
	offset+=sizeof(elementtype);

	unsigned char cbMonthNum = nMonthNum; 
	memcpy(bf+sizeof(StPrt)+offset,&cbMonthNum, sizeof(char));
	offset+=sizeof(char);

//	CString logstr;
//	logstr.Format("(2)%.8s,Monthnum=%d,List:",frameid.szItemCode,nMonthNum);
	
//	CString dest;

	for(int i = 0; i < nMonthNum; i ++)
	{
		memcpy(bf+sizeof(StPrt)+offset,&m_optFm[nIndex].m_optMonth[i].lTransDate, sizeof(long));
		offset+=sizeof(long);
//		dest.Format("%d,",m_optFm[nIndex].m_optMonth[i].lTransDate);
//		logstr+=dest;
	}

//	WriteLogFile((LPSTR)(LPCTSTR)logstr);

	StPrt.Length = sizeof(StPrt) + offset - 10/*原来是12*/ + 2;
	StPrt.dataSize = sizeof(StPrt)+offset+2 - 8 ; //andy add 2007.08.31
	memcpy(bf,&StPrt,sizeof(StPrt));

	WORD wdVal = 'ED';
	memcpy(bf+sizeof(StPrt)+offset,&wdVal,sizeof(wdVal));

	SendOutFrame(bf,sizeof(StPrt)+offset+2);	
	return S_OK;
}


void CQSNative_Option::HandleFrame( BYTE* pBuf, WORD Len) 
{

	_tagTTDataFrame* pStFrame = (_tagTTDataFrame *)pBuf;

	// 检查是否需要发 时间桢, 日期有否更新等.	
//	CheckFrameTime( pStFrame ) ;

	// 根据btTransCode解桢.
	switch( pStFrame->btTransCode )
	{
		case 128:	//	开盘信号.	
			ProceDataFrame_128( pStFrame ) ;
			break ;
		case 200:	//	时间桢.	
			ProceDataFrame_200( pStFrame ) ;

			SendRegularData();  //andy 2012.11.13 在收数据的线程里发整屏数据，这样可以保证执行到。
			break ;
		case 136:	//	option frame.
			ProceDataFrame_136( pStFrame ,pBuf,Len) ;
			break ;
		case 253:	//	日期修正frame.
			ProceDataFrame_253( pStFrame ) ;
			break ;		
		default:	
			break ;
	}//end switch.

}


void CQSNative_Option::CheckDay()
{
	try
	{
		CTime c=CTime::GetCurrentTime();
		int date=c.GetYear()*10000+c.GetMonth()*100+c.GetDay();
		DWORD m_date = 0;
		if (m_date!=date)
		{
			m_date=date;
			CString path ;
			CWrapperPath::GetExtPath( path ) ;
			CString filename="";
			filename.Format( "%sMS_%02d%02d.log", path, c.GetMonth(), c.GetDay() );
			int year=0;
			
			g_Log.LogExt("void CQSNative_Option::CheckDay() m_date!=date");
			CFileStatus  status;//获取文件相关时间
			if(CFile::GetStatus(filename, status))
			{
				g_Log.LogExt("void CQSNative_Option::CheckDay() CFile::GetStatus(filename, status)");
				year=status.m_ctime.GetYear();
				if (year<c.GetYear())
				{
					// 					g_Log.LogExt("void CMSDlg::CheckDay() year<c.GetYear()");
					// 					//::DeleteFile(filename);
					// 					{
					// 						g_Log.LogExt("void CMSDlg::CheckDay() CreateFile F");
					// 						HANDLE file = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ, NULL, 
					// 												OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					// 						g_Log.LogExt("void CMSDlg::CheckDay() CreateFile S");
					// 						if (file!=NULL)
					// 						{
					// 							FILETIME ctime,atime,wtime;
					// 							SYSTEMTIME sctime;
					// 							sctime.wYear=c.GetYear();
					// 							sctime.wMonth=c.GetMonth();
					// 							sctime.wDay=c.GetDay();
					// 							sctime.wHour =c.GetHour()-8;
					// 							sctime.wMinute=c.GetMinute();
					// 							sctime.wSecond =c.GetSecond();
					// 							sctime.wDayOfWeek=c.GetDayOfWeek();
					// 							sctime.wMilliseconds =0;
					// 							SystemTimeToFileTime(&sctime,&ctime);
					// 							atime=ctime;
					// 							wtime=ctime;
					// 							if(!SetFileTime(file,&ctime,&atime,&wtime))//设置时间修改
					// 							{
					// 								g_Log.LogExt("Delete log file of last year failed.");
					// 							}
					// 							CloseHandle(file);//关闭句柄
					// 						}
					
					::DeleteFile(filename);
					
				//}
				g_Log.LogExt("Delete log file of last year.");
			}
		}
	}
}
catch(...)
{ 
	g_Log.LogExt("void CQSNative_Option::CheckDay() Delete log file Failed.");
//	exit(0);
}
}

void CQSNative_Option::ProceDataFrame_128( _tagTTDataFrame * pStFrame ) 
{
	// paul, 2008-2-16, 统一为 tele server中处理方法. 

	BYTE* buff = (BYTE*)pStFrame ;

	// paul, 2008-2-16, 要检查, tele中这样处理的.
	if(strlen(pStFrame->arItemCode)!=0)
		return;

	long nDate = 0;

	// paul, 2008-2-16, del, 这是错的, time已经转成了 time_t
	//unsigned short usValue2;
	//memcpy(&usValue2,buff+sizeof(_tagTTDataFrame)+4,2);		//position 5 6 is time(unsigned short)

	if(pStFrame->btGroupCode == 'A')
	{
		char msg[100];
		
		memcpy(&nDate, buff + sizeof(_tagTTDataFrame), sizeof(long));

		if(m_nTransDate==0)
			m_nTransDate=nDate;

		WORD wTTFrameTime = (WORD)((DWORD)pStFrame->pTransData>>16);
		if(g_Master.m_MSCEType != MSCE_TYPE_PROXY)
		{
			WORD frametime=wTTFrameTime;
			WORD fTime = (wTTFrameTime/1800)*1800+(wTTFrameTime % 1800) / 30 * 30;
			int num=m_ArrHoukeTime_opt.GetSize();
			for(int i=0;i<num;i++)
			{
				if((fTime == m_ArrHoukeTime_opt[i].Time) || 
					(fTime == m_ArrHoukeTime_opt[i].Time + 1) || 
					(fTime == m_ArrHoukeTime_opt[i].Time + 2) ||
					(fTime == m_ArrHoukeTime_opt[i].Time + 3) ||
					(fTime == m_ArrHoukeTime_opt[i].Time + 4))
				{
					if(m_ArrHoukeTime_opt[i].GCode=='A')
					{
						CString exepath, inifile ;
						CWrapperPath::GetExtPath( exepath ) ;
						inifile.Format( "%sHouseKeeping.INI",  exepath ) ;
						CString app, key ;
						char temp[200] ;
						app="Options";
						key = "lastHouseKeepingDataDate";
						int lastHouseKeepingDataDate=0;
						lastHouseKeepingDataDate = GetPrivateProfileInt( app, key, 0, inifile ) ;
						if (m_nTransDate<=lastHouseKeepingDataDate)
						{
							sprintf(temp,"OptionsSaveIV today(%d) is housekeeping already.",m_nTransDate);
							WriteLogFile(temp);
						}
						else
						{
//							CQServer* pServer = g_Master.GetQServer( _QSERVERTYPE_CHARTING_HIS ) ;
//							if( pServer==NULL || pServer->IsType_Native()==FALSE )
//							{
//								break;
//							}
//							CQSNative_Charting* pNServer = (CQSNative_Charting*)( pServer->m_pNativeObj ) ;
//							if( pNServer==NULL )
//							{
//								break;
//							}

							CalcIV2();
							SaveIV();
							sprintf(temp,"Cal&Save OptionsIV.");
							WriteLogFile(temp);
							int sucnum=0,failed=0;
							for (int i=0;i<m_nItemNum;i++)
							{
								if (strlen(m_OptIV[0].m_items[i].item)>0)
								{
									if ( m_OptIV[0].m_items[i].iv[0]>0 && m_OptIV[0].date>0 )
									{
										OptionIV iv;
										memset(&iv,0,sizeof(OptionIV));
										
										iv.date =m_OptIV[0].date;
										iv.open =m_OptIV[0].m_items[i].open;
										iv.high =m_OptIV[0].m_items[i].high;
										iv.low  =m_OptIV[0].m_items[i].low;
										iv.close=m_OptIV[0].m_items[i].close;
										
										iv.iv   =m_OptIV[0].m_items[i].iv[0];
										iv.month=m_OptIV[0].m_items[i].month[0];
										
										float v=0;
										int count=0;
										for (int n=1;n<TT_OPT_MAXMONTH;n++)
										{
											if (count>2)
												break;
											
											if ( m_OptIV[0].m_items[i].month[n]>0 &&
												( m_OptIV[0].m_items[i].month[n]%100)%3==0 && 
												m_OptIV[0].m_items[i].iv[n]>0 )
											{
												v+=m_OptIV[0].m_items[i].iv[n];
												count++;
											}
										}
										v=(v+iv.iv)/(count+1);
										iv.iv2=v;
										char item[10]={0};
										sprintf(item,"A%.8s",m_OptIV[0].m_items[i].item);
										
									//	pNServer->m_HisFileMng.SaveOptionsIV(item,iv);
										sucnum++;
									}
									else
									{
										sprintf(temp,"Save OptionsIV %.8s iv is 0.skip",m_OptIV[0].m_items[i].item);
										WriteLogFile(temp);
										failed++;
									}
								}
							}
							if (sucnum>0)
							{
								sprintf(temp,"Housekeeping is over.Success[%d] Failed[%d]",sucnum,failed);
								WriteLogFile(temp);

								app="Options";
								key = "lastHouseKeepingDataDate";
								sprintf(temp,"%d",m_nTransDate);
								WritePrivateProfileString(app,key,temp,inifile);
							}
						}
					}
					
				}
			}
		}
	

		if(nDate != m_nTransDate && IsDateValid(nDate))
		{
			if(m_nTransDate != 0) //排除=0的情况
			{   //2.小于的日期为非法日期。
				//3.大于20日的日期也是非法日期。  
				int iDiff;
				if(DateDiff(nDate,m_nTransDate,iDiff) == FALSE)
				{
					sprintf(msg,"recv err date:%d today=%d",nDate,m_nTransDate);
					WriteLogFile(msg);
					return;
				}
				if(iDiff>=20 || iDiff<=0) 
				{
					sprintf(msg,"recv err date 2:%d today=%d",nDate,m_nTransDate);
					WriteLogFile(msg);
					return;
				}
			}
			

/*
			// paul, 2008-2-16, del.( 不用检查 ) 
			if(usValue2 >= 1800*24) 
			{	//andy add 2005.05.12 Day Start时间超过240000的无效					
				sprintf(msg,"recv err day start : tm = %d",usValue2);
				WriteLogFile(msg);
			}
			else
*/
			{
				CheckDay();
				sprintf(msg,"Date Start :%d -> %d",m_nTransDate,nDate);								
				WriteLogFile(msg);
				//add by andy 20090903
				// Bill Wong 说 (16:58):
				// options 問題應是和 ms 死的問題有關，其實 應在每天轉date前, 把 幾個dat 先save 才清。

				//johnny 20170705 增加最痛option处理
				CalMaxPain();
				SaveMaxPain();

				GetPreOI(nDate);
				SavePreviousOI();

				StoreManage(FALSE);

				
				CString filepath,filepath2;
				filepath.Format("Options_%d.dat",m_nTransDate);
				filepath2.Format("Options_%d.z",m_nTransDate);
				SetFileAttributes("Options.dat",FILE_ATTRIBUTE_NORMAL);
				SetFileAttributes(filepath,FILE_ATTRIBUTE_NORMAL);
				CopyFile("Options.dat",filepath,FALSE);
				
				if (!Compress(filepath,filepath2))
				{
					DeleteFile(filepath);
				}

				if (strlen(m_szBackupPath)>0)
				{
					SetFileAttributes(filepath2,FILE_ATTRIBUTE_NORMAL);
					filepath.Format("%s\\Options_%d.z",m_szBackupPath,m_nTransDate);
					CopyFile(filepath2,filepath,FALSE);
					WriteLogFile("Save Options.dat.");
				}
				DeleteFile(filepath2);
				

				m_nTransDate = nDate;

				ResetDataTable();
				SaveTime();

				//johnny add 20170815 每日开市清空IV列表
				m_OptIV.RemoveAll();
				//memset(&m_OptIV,0,sizeof(OptIV));
			}
		}
	}
	
	return;
}

BOOL CheckTime(_tagTTDataFrame * pStFrame)
{
	
	return TRUE;
}

void CQSNative_Option::ProceDataFrame_200( _tagTTDataFrame * pStFrame ) 
{
	ProceDataFrame_128(pStFrame);
}

void CQSNative_Option::ProceDataFrame_253( _tagTTDataFrame * pStFrame ) 
{
	BYTE* buff = (BYTE*)pStFrame ;

	long nDate = 0;
	
	DWORD dtNew,dtSpec,dtCurr;
	memcpy(&dtNew, buff + sizeof(_tagTTDataFrame), sizeof(long));												
	memcpy(&dtSpec, buff + sizeof(_tagTTDataFrame)+4, sizeof(long));												
	memcpy(&dtCurr, buff + sizeof(_tagTTDataFrame)+8, sizeof(long));												
	
	char msg[100];
	sprintf(msg,"recv 253. Group=%c Today = %d NewDate = %d CurrDate = %d flag = %x",
		pStFrame->btGroupCode,m_nTransDate,dtNew,dtCurr,dtSpec);
	WriteLogFile(msg);
	
	if(dtSpec == 0xfada && IsDateValid(dtNew) && IsDateValid(dtCurr))
	{
		if(m_nTransDate == dtCurr)
		{
			//clear data by group 
			m_nTransDate = dtNew;
			if(pStFrame->btGroupCode == 'A') //options属于A组数据	
			{ 
				SaveTime();
				ResetDataTable();
			}
		}																				
	}


}

void CQSNative_Option::ProceDataFrame_136( _tagTTDataFrame * pStFrame , BYTE* pBuf, int nFrameLen) 
{
	//if(pStFrame->btTransCode == 136 && pStFrame->btGroupCode == 'A')// check options frame 		
	static BYTE buf[1024*1024*2];
	if (nFrameLen>1024*100)
	{
		char logstr[100];
		sprintf(logstr,"Rec Options_136 len=%d",nFrameLen);
		WriteLogFile(logstr);
	}
	memcpy((char*)buf,(char*)pBuf,nFrameLen);

	char item[8];
	memcpy(item,pBuf+sizeof(_tagTTDataFrame),8);
	/*
	if (strlen(item)==0)
	{
		WORD wTTFrameTime = (WORD)((DWORD)pStFrame->pTransData>>16);
		struct tm *newtime;
		newtime = localtime( &pStFrame->lTime); 
		char szMsg[200];
		
		sprintf(szMsg,"recv 136: fm_mstm = %02d:%02d:%02d  len=%d",
			newtime->tm_hour,newtime->tm_min,newtime->tm_sec,nFrameLen);
		WriteLogFile(szMsg);
		int nnn=0;
	}
	*/
	if(pStFrame->btGroupCode == 'A')
	{	
		//struct tm *newtime;
		//newtime = localtime( &pStFrame->lTime); 
		//char szMsg[200];

		//sprintf(szMsg,"recv 136: fm_mstm = %02d:%02d:%02d  len=%d",
		//	newtime->tm_hour,newtime->tm_min,newtime->tm_sec,nFrameLen);
		//WriteLogFile(szMsg);
	
		GetNewOptionsData(pStFrame->btGroupCode,buf,nFrameLen);			
	}
}


void CQSNative_Option::ResetDataTable()
{   //即时通道
	Init();

	STProtocol_SR_RET2 StPrt;

	FrameLen  framelen;
	FrameHead framehead;
	FrameID   frameid;
	ElementType elementtype;
	framelen = sizeof(FrameHead) + sizeof(FrameID) + sizeof(ElementType) + 1;
	framehead.wFrameType = 'DO';
	memset(&frameid, NULL, sizeof(FrameID));
	elementtype = 'ST';

	BYTE byBuf[150];
	memset(byBuf, 0, sizeof(byBuf));
	unsigned short usLen = sizeof(StPrt);

	memcpy(byBuf + usLen, &framelen, sizeof(FrameLen));
	usLen += sizeof(FrameLen);

	memcpy(byBuf + usLen, &framehead, sizeof(FrameHead));
	usLen += sizeof(FrameHead);
	
	memcpy(byBuf + usLen, &frameid, sizeof(FrameID));
	usLen += sizeof(FrameID);

	memcpy(byBuf + usLen,&elementtype, sizeof(ElementType));
	usLen += sizeof(ElementType);

	usLen += sizeof(BYTE);  //strike num = 0

	usLen -= 2 + sizeof(StPrt);


	CString strUser;
	OptUserInfo usinfo;
	EnterCriticalSection(&m_csUser);	
	POSITION pos=m_UserList.GetStartPosition();
	int i=0;
	while(pos)
	{		
		m_UserList.GetNextAssoc(pos,strUser,usinfo);
		if(usinfo.dwCount > 0)
		{
		//	StPrt.ReqIdNum =1;
		//	StPrt.ReqID = 0;
			StPrt.headMark = 'AA55';
			StPrt.BegFlag = 'WS';
			StPrt.DFFlag = 'DF';
			StPrt.DataOff = 14;
			StPrt.CIFlag = 'CI';	
			StPrt.ChannelID = usinfo.dwChannelID;	
			StPrt.DAFlag = 'DA';		
			
			StPrt.Length = sizeof(StPrt)+ usLen + 2 - 10/*原来是12*/ + 2 ;
			StPrt.dataSize = sizeof(StPrt)+usLen+2+2 - 8; //andy add 2007.08.31
			memcpy(byBuf, &StPrt, sizeof(StPrt));
			
			WORD wdVal = 'ED';
			memcpy(byBuf+sizeof(StPrt)+usLen+2,&wdVal,2);

			SendOutFrame(byBuf,sizeof(StPrt)+usLen+2+2);	
		}
	}	
	LeaveCriticalSection(&m_csUser);

}

HRESULT CQSNative_Option::Init()
{
	int	ftime;
	FORCE_HOUKE_TIME_OPT	sfht;
				
	CString	spath;
	
//	CWrapperPath::GetRootPath(spath);
	spath=spath + "bin\\ms.ini";
	char szTemp[100]={0};
	char chGcode;
	CString strGcode;
	for(chGcode = 'A';chGcode < 'Z' ; chGcode++)
	{
		strGcode = chGcode;
		GetPrivateProfileString( "FORCEHOKE_OPT", strGcode, "", szTemp, 800, spath ) ;
		if( strlen(szTemp)>0)
		{
			ftime = atoi(szTemp);
			sfht.Time = (ftime / 100) * 1800 + (ftime % 100 ) * 30 ;
			sfht.GCode = chGcode;
			m_ArrHoukeTime_opt.Add(sfht);
		}
	}


	for(int i = 0; i < m_nItemNum; i ++)
	{
		m_optFm[i].Init();
	}
	m_nItemNum = 0;
	return S_OK;
}

BOOL isValidDate(int y, int m, int d)
{
    if(y < 1990 || y > 9999) 
		return FALSE;
    if(m < 1 || m > 12) 
		return FALSE;

    switch(m)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return (d>=1 && d<=31);
    case 4:
    case 6:
    case 9:
    case 11:
        return (d >= 1 && d <= 30);
    case 2:
        if((y/4 == 0 && y/100 != 0) || (y/400 == 0)) 
			return (d >= 1 && d <= 29);
        else 
			return (d >= 1 && d <= 28);
    }
    return FALSE;

}


HRESULT CQSNative_Option::GetNewOptionsData(BYTE cbGroup, BYTE* pBuf, int nFrameLen)
{
	long nOffset = 0;
	long nItemIdx, nMonthIdx, nStrikeIdx;
	long nTemp;
	
	//	BOOL bCreateMM = FALSE;
	//	BOOL bCreateSt = FALSE;
	//	BOOL bDelStrike = FALSE;
	//	BOOL bDelMM = FALSE;
	
	nOffset += sizeof(_tagTTDataFrame);
	long nLen = nFrameLen;
	
	COptRecFrame recFrame;
	memcpy(&recFrame, pBuf + nOffset, sizeof(COptRecFrame));
	nOffset += sizeof(COptRecFrame);
	
	if((nItemIdx = EnumOptionItemCodeEx((LPCTSTR)recFrame.ucICode, cbGroup)) == -1)
	{
		if((nItemIdx = InsertItemCodeEx((LPCTSTR)recFrame.ucICode, cbGroup)) == -1)
			return S_FALSE;
	}

	long lTransdate = recFrame.lTransDate; 

	
	//YYYYMMDD
	if (lTransdate > 999999L)
	{
// 		nTemp = lTransdate /10000L;
// 		
// 		if(!lTransdate || nTemp < 1990 || nTemp > 9999)
// 			return S_FALSE;
// 		
// 		nTemp = (lTransdate % 10000L)/100L;
// 		if(nTemp < 1 || nTemp > 12)
// 			return S_FALSE;
		int year = lTransdate / 10000L;
		int month = lTransdate % 10000L / 100L;
		int day = lTransdate % 100;
		if (isValidDate(year, month, day) == FALSE)
		{
			return S_FALSE;
		}
	}
	//YYYYMM
	else
	{
		nTemp = lTransdate /100L;
		
		if(!lTransdate || nTemp < 1990 || nTemp > 9999)
			return S_FALSE;
		
		nTemp = lTransdate % 100L;
		if(nTemp < 1 || nTemp > 12)
		return S_FALSE;

	}
	
	
	/*
	nTemp = lTransdate /100L;
	
	if(!lTransdate || nTemp < 1990 || nTemp > 9999)
		return S_FALSE;
	
	nTemp = lTransdate % 100L;
	if(nTemp < 1 || nTemp > 12)
		return S_FALSE;

*/

	if(recFrame.ucFlags != 63)		// Delete Month
	{
		if((nMonthIdx = FindMonthEx(nItemIdx, lTransdate)) == -1)
		{
			if((nMonthIdx = InsertMonthEx(nItemIdx, lTransdate)) == -1)
				return S_FALSE;
			else
			{
				SendAvailableMonthToUser(cbGroup,(char*)recFrame.ucICode,recFrame.lTransDate);
			}
		}
	}
	else
	{
		if((nMonthIdx = FindMonthEx(nItemIdx, lTransdate)) != -1)
		{
			// Delete Month;
			DeleteMonthEx(nItemIdx, nMonthIdx);
		}
		
		// SendFrame
		SendDelFrameToUser(cbGroup, &recFrame, FALSE);

		return S_OK;
	}

	float fStrike = recFrame.fStrike;

	COptFrame* pFm = &m_optFm[nItemIdx];
	COptMonth* pMf = &pFm->m_optMonth[nMonthIdx];

	if(recFrame.ucFlags != 62)			// Delete Strike
	{
		if((nStrikeIdx = FindStrikeEx(pMf, fStrike)) == -1)
		{
			if((nStrikeIdx = InsertStrikeEx(pMf, fStrike)) == -1)
				return S_FALSE;

			//add for test
			if(pMf->lTransDate >= 192710 && pMf->lTransDate <= 202710 && nItemIdx == 0)	// pMf->lTransDate == 200710 wendy test
			{
				WriteLogFile("------------- Insert StrikeEx -----------------");
				for(int i = 0; i < pMf->ucTotalRow; i ++)
				{
					char szmsg[500];
					sprintf(szmsg,"Insert StrikeEx : Date=%d Row=%d/%d strike=%6.0f",pMf->lTransDate,i,pMf->ucTotalRow,pMf->m_optLine[i].m_fStrike);
					WriteLogFile(szmsg);
				}
			}
			//add for test
		}
	}
	else
	{
		if((nStrikeIdx = FindStrikeEx(pMf, fStrike)) != -1)
		{
			// Delete data
			DeleteStrikeEx(pMf, nStrikeIdx);

//Andy add for test
#ifdef _DEBUG
	char strMsg[200];
	sprintf(strMsg,"DeleteStrikeEx : fStrike=%.3f nStrikeIdx=%d",fStrike,nStrikeIdx);
	WriteLogFile(strMsg);
#endif	
//Andy add for test

		}
		// SendFrame
		SendDelFrameToUser(cbGroup, &recFrame, TRUE);

		return S_OK;
	}
	COptLine* pSl = &pMf->m_optLine[nStrikeIdx];

	memset(pSl->opChg, 0, sizeof(pSl->opChg));
	for(;;)
	{
		//johnny modify 20170320
		//decimal:2 - 0=None, 1=1 decimal point, 2=2 decimal point2, 3=4 decimal points
		// Decimal
		//if(recFrame.ucDecimal==3)
		{
		//	pMf->cDecimal = 4;
		}
		//else
		//取消4位小数处理，改为client处理
		pMf->cDecimal = recFrame.ucDecimal;
		
		float fValue;
		float fTransdata = recFrame.fTrans;
		
		if(fTransdata < 0)
		{
			break;
		}
/*		
if((recFrame.ucFlags >= 0 && recFrame.ucFlags <= 4) || 
   (recFrame.ucFlags >= 60 && recFrame.ucFlags <= 63) ||
   recFrame.ucFlags == 50)
{
		if(recFrame.fTrans > 0 && recFrame.fTrans < 0.01)
			int yy=0;
		if(recFrame.fTrans < -100000 || recFrame.fTrans > 1000000)
			int y2y=0;
}
*/
//		char msg[100];
//		char szItem[10];
//		char szFile[100];
		static int iSeqNum = 0;


		switch(recFrame.ucFlags)
		{
		case OPTTXTYASK:		// 0 - Ask
			pSl->m_dat[recFrame.ucCallOrPut].fAsk = fTransdata;
			pSl->m_dat[recFrame.ucCallOrPut].ucTheoretAsk = recFrame.ucTheoretical;
			
			// Add Change
			pSl->opChg[recFrame.ucCallOrPut].ucAsk = 1;
			break;
		case OPTTXTYASKQTY:
			pSl->m_dat[recFrame.ucCallOrPut].fAskQty = fTransdata;
			
			// Add Change
			pSl->opChg[recFrame.ucCallOrPut].ucAskQty = 1;
			break;
		case OPTTXTYBID:
			pSl->m_dat[recFrame.ucCallOrPut].fBid = fTransdata;
			pSl->m_dat[recFrame.ucCallOrPut].ucTheoretBid = recFrame.ucTheoretical;
			pSl->m_dat[recFrame.ucCallOrPut].ucCabBid = recFrame.ucCabinet;

			// Add Change
			pSl->opChg[recFrame.ucCallOrPut].ucBid = 1;
			break;
		case OPTTXTYBIDQTY:
			pSl->m_dat[recFrame.ucCallOrPut].fBidQty = fTransdata;
/*
			//andy add 2009.05.15 for test 
			memset(szItem,0,sizeof(szItem));
			memcpy(szItem,recFrame.ucICode,8);
			if(recFrame.ucCallOrPut == 0 && recFrame.lTransDate==200905)			
			{
				sprintf(szFile,"RecvBidQty_%d",iSeqNum++);
				WriteRecieveData(pBuf,nFrameLen,szFile);

				sprintf(msg,"BIDQTY=%.2f Item=%s lMonth=%d file: %s",fTransdata,szItem,recFrame.lTransDate,szFile);
				WriteLogFile(msg);
			}
			//andy add 2009.05.15 for test 
*/
			// add Change
			pSl->opChg[recFrame.ucCallOrPut].ucBidQty = 1;
			break;
		case OPTTXTYLAST:
			pSl->m_dat[recFrame.ucCallOrPut].fLast = fTransdata;
			
			memcpy(&fValue, pBuf + nOffset, 4);
			nOffset += 4;
			pSl->m_dat[recFrame.ucCallOrPut].fLastQty = fValue;

			memcpy(&fValue, pBuf + nOffset, 4);
			nOffset += 4;
			pSl->m_dat[recFrame.ucCallOrPut].fCumulate = fValue;
			
			pSl->m_dat[recFrame.ucCallOrPut].ucCabLast = recFrame.ucCabinet;

			// Add Change
			pSl->opChg[recFrame.ucCallOrPut].ucLast = 1;
			pSl->opChg[recFrame.ucCallOrPut].ucLastQty = 1;
			pSl->opChg[recFrame.ucCallOrPut].ucCumulate = 1;
			break;
		case OPTTXTYHIGH:
			pSl->m_dat[recFrame.ucCallOrPut].fHigh = fTransdata;

			// Add Change
			pSl->opChg[recFrame.ucCallOrPut].ucHigh = 1;
			break;
		case OPTTXTYLOW:
			pSl->m_dat[recFrame.ucCallOrPut].fLow = fTransdata;

			// Add Change
			pSl->opChg[recFrame.ucCallOrPut].ucLow = 1;
			break;
		case OPTTXTYSUMMARY:
			pSl->m_OcDat[recFrame.ucCallOrPut].fOpenAsk = fTransdata;

			memcpy(&fValue, pBuf + nOffset, 4);
			nOffset += 4;
			pSl->m_OcDat[recFrame.ucCallOrPut].fOpenBid = fValue;

			memcpy(&fValue, pBuf + nOffset, 4);
			nOffset += 4;
			pSl->m_OcDat[recFrame.ucCallOrPut].fOpenLast = fValue;

			memcpy(&fValue, pBuf + nOffset, 4); // closing ask
			nOffset += 4;
			pSl->m_OcDat[recFrame.ucCallOrPut].fCloseAsk = fValue;

			memcpy(&fValue, pBuf + nOffset, 4);
			nOffset += 4;
			pSl->m_OcDat[recFrame.ucCallOrPut].fCloseBid = fValue;

			memcpy(&fValue, pBuf + nOffset, 4);
			nOffset += 4;
			pSl->m_OcDat[recFrame.ucCallOrPut].fCloseLast = fValue;

			memcpy(&fValue, pBuf + nOffset, 4);
			nOffset += 4;
			pSl->m_OcDat[recFrame.ucCallOrPut].fCloseCum = fValue;
			// Send Cumulate
			pSl->m_dat[recFrame.ucCallOrPut].fCumulate = fValue;

			memcpy(&fValue, pBuf + nOffset, 4);
			nOffset += 4;
			pSl->m_OcDat[recFrame.ucCallOrPut].fPreTurnover = fValue;

			memcpy(&fValue, pBuf + nOffset, 4);
			nOffset += 4;
			pSl->m_OcDat[recFrame.ucCallOrPut].fPreOI = fValue;
			// Send O/I
			pSl->m_dat[recFrame.ucCallOrPut].fOI = fValue;

			// Add Change
			pSl->opChg[recFrame.ucCallOrPut].ucCumulate = 1;
			pSl->opChg[recFrame.ucCallOrPut].ucOI = 1;
			break;
		case OPTTXTYCORLAST:
			//last price 
			// total no. of contracts traded 
			nOffset += 4;
			memcpy(&fValue, pBuf + nOffset, 4); /* cumulative vol. */
			nOffset += 4;
			pSl->m_dat[recFrame.ucCallOrPut].fCumulate = fValue;
			if (fValue == 0.0f)
			{
				pSl->m_dat[recFrame.ucCallOrPut].fLast = 0.0f;
				pSl->m_dat[recFrame.ucCallOrPut].ucCabLast = 0;

				// Add Change
				pSl->opChg[recFrame.ucCallOrPut].ucLast = 1;
			}

			// Add Change
			pSl->opChg[recFrame.ucCallOrPut].ucCumulate = 1;
			break;
		case OPTTXTYCORVOL :
			//last price ('fTransdata ') not used
			//total no. of contracts traded - skipped
			nOffset += 4;
			memcpy(&fValue, pBuf + nOffset, 4); /* cumulative vol. */
			nOffset += 4;
			pSl->m_dat[recFrame.ucCallOrPut].fCumulate = fValue;
			if (fValue == 0.0f)
			{
				pSl->m_dat[recFrame.ucCallOrPut].fLast = 0.0f;
				pSl->m_dat[recFrame.ucCallOrPut].ucCabLast = 0;

				// Add Change
				pSl->opChg[recFrame.ucCallOrPut].ucLast = 1;
			}

			// Add Change
			pSl->opChg[recFrame.ucCallOrPut].ucCumulate = 1;
			break;
		default :
			nOffset = nFrameLen;
			break;
		}
		if(nOffset <= nFrameLen - (sizeof(COptRecFrame) - 16))//由于原来的options server 传了crc和03进来,所以原先用<的bug没暴露出来.
		{
			memcpy((BYTE*)&recFrame + 16, pBuf + nOffset, sizeof(COptRecFrame) - 16);
			nOffset += (sizeof(COptRecFrame) - 16);
		}
		else
			break;
	}
	
	char cKey[20];
	memset(cKey,0,sizeof(cKey));
	memcpy(cKey,&cbGroup,1); //groupcode + itemcode + lDate
	memcpy(cKey+1,&recFrame.ucICode,8); //groupcode + itemcode + lDate
	char cDate[10];
	sprintf(cDate,"%08d",recFrame.lTransDate); 
	strcat(cKey,cDate);
	CString strUser = cKey;
	/*
 	CString asd;
	if (strUser.Find("02827",0)>=0 &&recFrame.lTransDate==201711)
	{
		asd.Format("%.20s-%d ask=%.6f,bid=%.6f,asknum=%.6f,binnum=%f,strike=%f",strUser,recFrame.ucCallOrPut,pSl->m_dat[recFrame.ucCallOrPut].fAsk,
			pSl->m_dat[recFrame.ucCallOrPut].fBid,pSl->m_dat[recFrame.ucCallOrPut].fAskQty,pSl->m_dat[recFrame.ucCallOrPut].fBidQty,fStrike);
 		WriteLogFile((LPSTR)(LPCSTR)asd);
	}
	*/
	COptChanged opChg;
	memset(&opChg, 0, sizeof(COptChanged));
	if(memcmp(&pSl->opChg[0], &opChg, sizeof(COptChanged)))
		SendDataToUser(pFm, pMf, pSl, 0,strUser);
	if(memcmp(&pSl->opChg[1], &opChg, sizeof(COptChanged)))
		SendDataToUser(pFm, pMf, pSl, 1,strUser);
	
	if (pSl->opChg[recFrame.ucCallOrPut].ucLast==1)
	{
		SendDataToUser2(pFm, pMf, pSl, 0,strUser);
	}
	
	return S_OK;
}

long CQSNative_Option::EnumOptionItemCodeEx(LPCTSTR pszItemCode, BYTE cbGroup)
{
	int i;
	if(pszItemCode == NULL || !strlen(pszItemCode))
		return -1;
	BOOL bRet = FALSE;
	for( i = 0; i < m_nItemNum; i++)
	{
// 		CString asd;
// 		asd.Format("%s,%s,",pszItemCode,m_optFm[i].strICode);
// 		if (strstr(pszItemCode,"2823")!=NULL && strstr((LPSTR)(LPCTSTR)m_optFm[i].strICode,"2823")!=NULL)
// 			WriteLogFile((LPSTR)(LPCSTR)asd);
		if((!memcmp(m_optFm[i].strICode, pszItemCode, 7) && 
			m_optFm[i].cGCode == cbGroup))
		{
			bRet = TRUE;
			break;
		}
	}
	if(bRet)
		return i;
	return -1;
}

long CQSNative_Option::EnumOptionItemCodeEx2(LPCTSTR pszItemCode, BYTE cbGroup)
{
	int i;
	if(pszItemCode == NULL || !strlen(pszItemCode))
		return -1;
	BOOL bRet = FALSE;
	for( i = 0; i < m_nItemNum2; i++)
	{
		// 		CString asd;
		// 		asd.Format("%s,%s,",pszItemCode,m_optFm[i].strICode);
		// 		if (strstr(pszItemCode,"2823")!=NULL && strstr((LPSTR)(LPCTSTR)m_optFm[i].strICode,"2823")!=NULL)
		// 			WriteLogFile((LPSTR)(LPCSTR)asd);
		//if((!memcmp(m_PreOI[i].strICode, pszItemCode, 7) && 
		//	m_PreOI[i].cGCode == cbGroup))
		{
			bRet = TRUE;
			break;
		}
	}
	if(bRet)
		return i;
	return -1;
}

long CQSNative_Option::InsertItemCodeEx(LPCTSTR pszItemCode, BYTE cbGroup)
{
	if(pszItemCode == NULL || !strlen(pszItemCode) || m_nItemNum == TT_OPT_MAXITEM)
		return -1;
	
	if(m_optFm[m_nItemNum].cGCode != 0)
	{
		m_optFm[m_nItemNum].Init();
	}
	memcpy(m_optFm[m_nItemNum].strICode, pszItemCode, G_GENCODE__LEN);
	m_optFm[m_nItemNum].cGCode = cbGroup;
	m_nItemNum ++;

	return (m_nItemNum - 1);
}

long CQSNative_Option::FindMonthEx(long nItemIdx, long lTransdate)
{
	if(lTransdate == 0)
		return -1;
	for(int i = 0; i < m_optFm[nItemIdx].ucMonthNum; i ++)
	{
		if(m_optFm[nItemIdx].m_optMonth[i].lTransDate == lTransdate)
			return i;
	}
	return -1;
}

long CQSNative_Option::FindMonthEx2(long nItemIdx, long lTransdate)
{
	if(lTransdate == 0)
		return -1;
	//for(int i = 0; i < m_PreOI[nItemIdx].ucMonthNum; i ++)
	{
		//if(m_PreOI[nItemIdx].m_optOIMonth[i].lTransDate == lTransdate)
			//return i;
	}
	return -1;
}

long CQSNative_Option::InsertMonthEx(long nItemIdx, long lTransdate)
{
	COptFrame* pFm = &m_optFm[nItemIdx];
	if((lTransdate == 0) || (pFm->ucMonthNum >= TT_OPT_MAXMONTH))
		return -1;

	if(!pFm->ucMonthNum)
	{
		pFm->m_optMonth[0].lTransDate = lTransdate;
		pFm->ucMonthNum ++;
		return 0;
	}
	else if(pFm->m_optMonth[pFm->ucMonthNum -1].lTransDate
		< lTransdate)
	{
		pFm->m_optMonth[pFm->ucMonthNum].lTransDate = lTransdate;
		pFm->ucMonthNum ++;
		return (pFm->ucMonthNum - 1);
	}
	for(int i = 0; i < pFm->ucMonthNum; i ++)
	{
		if(pFm->m_optMonth[i].lTransDate < lTransdate)
			continue;
		else if(pFm->m_optMonth[i].lTransDate == lTransdate)
			return i;
		else 
		{
			memcpy(pFm->m_optMonth + (i + 1), 
				pFm->m_optMonth + i, sizeof(COptMonth) * (pFm->ucMonthNum - i));
			pFm->m_optMonth[i].Init();
			pFm->m_optMonth[i].lTransDate = lTransdate;
			pFm->ucMonthNum ++;
			return i;
		}
	}
	return -1;
}

HRESULT CQSNative_Option::DeleteMonthEx(long nItemIdx, long nMonthIdx)
{
	COptFrame* pFm = &m_optFm[nItemIdx];
	if(nItemIdx < 0 || nMonthIdx < 0 ||nItemIdx >= m_nItemNum || 
		pFm->ucMonthNum >= nMonthIdx)
		return S_FALSE;

	if(nMonthIdx != pFm->ucMonthNum - 1)
	{
		memcpy(&pFm->m_optMonth[nMonthIdx], &pFm->m_optMonth[nMonthIdx + 1], 
			sizeof(COptMonth) * (pFm->ucMonthNum - 1 - nMonthIdx));
		pFm->m_optMonth[pFm->ucMonthNum - 1].Init();
	}
	else
		pFm->m_optMonth[pFm->ucMonthNum - 1].Init();
	pFm->ucMonthNum --;
	return S_OK;
}

long CQSNative_Option::FindStrikeEx(COptMonth* pMf, float fStrike)
{
	for(int i = 0; i < pMf->ucTotalRow; i ++)
	{
		if(pMf->m_optLine[i].m_fStrike == fStrike)
			return i;
	}
	return -1;
}

long CQSNative_Option::InsertStrikeEx(COptMonth* pMf, float fStrike)
{
	if(pMf->ucTotalRow >= TT_OPT_MAXROW || fStrike <= 0.0f)
	{
//		ATLTRACE("InsertStrikeEx TotalRow > 40");
		return -1;
	}

	if(!pMf->ucTotalRow)
	{
		pMf->m_optLine[0].m_fStrike = fStrike;
		pMf->ucTotalRow ++;
		return 0;
	}
	else if(pMf->m_optLine[pMf->ucTotalRow - 1].m_fStrike < fStrike)
	{
		pMf->m_optLine[pMf->ucTotalRow].m_fStrike = fStrike;
		pMf->ucTotalRow ++;
		return (pMf->ucTotalRow - 1);
	}
	for(int i = 0; i < pMf->ucTotalRow; i ++)
	{
		if(pMf->m_optLine[i].m_fStrike < fStrike)
			continue;
		else if(pMf->m_optLine[i].m_fStrike == fStrike)
			return i;
		else 
		{
//			memcpy(pMf->m_optLine + (i + 1), pMf->m_optLine + i, 
//				sizeof(COptLine) * (pMf->ucTotalRow - i));
			//内存地址重叠，应该用memmove！ 20070911 Andy
			memmove(pMf->m_optLine + (i + 1), pMf->m_optLine + i, 
				sizeof(COptLine) * (pMf->ucTotalRow - i));
			pMf->m_optLine[i].Init();
			pMf->m_optLine[i].m_fStrike = fStrike;
			pMf->ucTotalRow ++;
			return i;
		}
	}

	return -1;
}

HRESULT CQSNative_Option::DeleteStrikeEx(COptMonth* pMf, long nStrikeIdx)
{
	if(nStrikeIdx < 0 || pMf->ucTotalRow <= nStrikeIdx)
		return S_FALSE;

	if(pMf->ucTotalRow - 1 != nStrikeIdx)
	{
		memcpy(&pMf->m_optLine[nStrikeIdx], &pMf->m_optLine[nStrikeIdx + 1], 
			sizeof(COptLine) * (pMf->ucTotalRow - 1 - nStrikeIdx));
		pMf->m_optLine[pMf->ucTotalRow - 1].Init();
	}
	else
		pMf->m_optLine[nStrikeIdx].Init();
	pMf->ucTotalRow --;
	return S_OK;
}

HRESULT CQSNative_Option::SendDataToUser(COptFrame* pFm, COptMonth* pMf, COptLine* pSl, int nType,CString strUser)
{//即时数据
	OptUserInfo usinfo;
	BOOL bFind = m_UserList.Lookup((LPCSTR)strUser,usinfo);

	if(bFind == TRUE)
	{
		if(usinfo.dwCount > 0)
		{
			SendData(pFm,pMf,pSl,nType,usinfo.dwChannelID); 
		}
	}
	return S_OK;
}

HRESULT CQSNative_Option::SendDataToUser2(COptFrame* pFm, COptMonth* pMf, COptLine* pSl, int nType,CString strUser)
{//即时数据
	OptUserInfo usinfo;
	BOOL bFind = m_UserList2.Lookup((LPCSTR)strUser,usinfo);
	
	if(bFind == TRUE)
	{
		if(usinfo.dwCount > 0)
		{
			//SendData(pFm,pMf,pSl,nType,usinfo.dwChannelID); 
			SendData2(pFm,pMf,pSl,nType,usinfo.dwChannelID); 
		}
	}
	return S_OK;
}

HRESULT CQSNative_Option::SendData2(COptFrame* pFm, COptMonth* pMf, COptLine* pSl, int nType, DWORD channelID)
{
	if(pFm == NULL || pMf == NULL || pSl == NULL || nType < 0 || nType > 1)
		return S_FALSE;
	/*
	STProtocol_SR_RET2 StPrt;

//	StPrt.ReqIdNum =1;
//	StPrt.ReqID = 0;
	StPrt.headMark = 'AA55';
	StPrt.BegFlag = 'WS';
	StPrt.DFFlag = 'DF';
	StPrt.DataOff = 14;
	StPrt.CIFlag = 'CI';	
	StPrt.ChannelID = channelID;	
	StPrt.DAFlag = 'DA';	

	unsigned char cbIndicator;
	FrameHead	framehead;
	FrameID		frameid;
	unsigned short usEType;
	
	framehead.wFrameType = 'DP';
	frameid.cbGroupCode = pFm->cGCode;
	memcpy(frameid.szItemCode, pFm->strICode, G_GENCODE__LEN);

	frameid.fStrikePrice = pSl->m_fStrike;
	frameid.lTransdate = pMf->lTransDate;
	frameid.cbOrder = (unsigned char)(nType + 1);

	BYTE byBuf[300];
	memset(byBuf, 0, sizeof(byBuf));
	unsigned short usLen = sizeof(StPrt);

	usLen += 2;
	memcpy(byBuf + usLen, &framehead, sizeof(FrameHead));
	usLen += sizeof(FrameHead);
	
	memcpy(byBuf + usLen, &frameid, sizeof(FrameID));
	usLen += sizeof(FrameID);

	usEType = 'XP';
	memcpy(byBuf + usLen, &usEType, 2);
	usLen += 2;
	
	BYTE row = 1;
	memcpy(byBuf + usLen, &row, sizeof(BYTE));
	usLen += sizeof(BYTE);

	memcpy(byBuf + usLen, &pSl->m_fStrike, sizeof(float));
	usLen += sizeof(float);
	
	memcpy(byBuf + usLen, &pSl->m_dat[0].fLast, sizeof(float));
	usLen += sizeof(float);
	cbIndicator = pSl->m_dat[0].ucCabLast << 1;
	memcpy(byBuf + usLen, &cbIndicator, sizeof(unsigned char));
	usLen += sizeof(unsigned char);

	memcpy(byBuf + usLen, &pSl->m_dat[1].fLast, sizeof(float));
	usLen += sizeof(float);
	cbIndicator = pSl->m_dat[1].ucCabLast << 1;
	memcpy(byBuf + usLen, &cbIndicator, sizeof(unsigned char));
	usLen += sizeof(unsigned char);
	

	usLen -= 2 + sizeof(StPrt);
	memcpy(byBuf+sizeof(StPrt), &usLen, sizeof(short));

	StPrt.Length = sizeof(StPrt)+ usLen + 2 - 10+ 2 ;
	StPrt.dataSize = sizeof(StPrt)+usLen+2+2 - 8; //andy add 2007.08.31
	memcpy(byBuf, &StPrt, sizeof(StPrt));

	WORD wdVal = 'ED';
	memcpy(byBuf+sizeof(StPrt)+usLen+2,&wdVal,2);
		
	SendOutFrame(byBuf,sizeof(StPrt)+usLen+2+2);
	*/
	STProtocol_SR_RET2 StPrt;
	//	StPrt.ReqIdNum =1;
	//	StPrt.ReqID = 0;
	StPrt.headMark = 'AA55';
	StPrt.BegFlag = 'WS';
	StPrt.DFFlag = 'DF';
	StPrt.DataOff = 14;
	StPrt.CIFlag = 'CI';	
	StPrt.ChannelID = channelID;	
	StPrt.DAFlag = 'DA';	
	
	FrameLen	framelen;
	FrameHead	framehead;
	FrameID		frameid;
	
	framehead.wFrameType = 'DP';
	frameid.cbGroupCode = pFm->cGCode;
	memcpy(frameid.szItemCode, pFm->strICode, G_GENCODE__LEN);
	
	frameid.fStrikePrice = pSl->m_fStrike;
	frameid.lTransdate = pMf->lTransDate;
	frameid.cbOrder = 0;
	
	long offset = sizeof(FrameLen); 
	

	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&framehead, sizeof(FrameHead));
	offset+=sizeof(FrameHead);
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&frameid, sizeof(FrameID));
	offset+=sizeof(FrameID);
	
	WORD elementtype = 'XP';
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&elementtype, sizeof(ElementType));
	offset+=sizeof(elementtype);
	
	BYTE actualRowNum = 1;
	long off = offset;
	//memcpy(m_pOutBuff+sizeof(StPrt)+offset,&actualRowNum, sizeof(actualRowNum));
	offset+=sizeof(actualRowNum);

	unsigned char cbIndicator;
	memcpy(m_pOutBuff+sizeof(StPrt)+offset, &pSl->m_fStrike, sizeof(float));
	offset += sizeof(float);
	
	memcpy(m_pOutBuff+sizeof(StPrt)+offset, &pSl->m_dat[0].fLast, sizeof(float));
	offset += sizeof(float);
	cbIndicator = pSl->m_dat[0].ucCabLast << 1;
	memcpy(m_pOutBuff+sizeof(StPrt)+offset, &cbIndicator, sizeof(unsigned char));
	offset += sizeof(unsigned char);
	
	memcpy(m_pOutBuff+sizeof(StPrt)+offset, &pSl->m_dat[1].fLast, sizeof(float));
	offset += sizeof(float);
	cbIndicator = pSl->m_dat[1].ucCabLast << 1;
	memcpy(m_pOutBuff+sizeof(StPrt)+offset, &cbIndicator, sizeof(unsigned char));
	offset += sizeof(unsigned char);
	

	framelen = sizeof(FrameHead) + sizeof(FrameID) + sizeof(ElementType) + sizeof(char)
		+ sizeof(float)*3 + sizeof(char)*2;
	
	memcpy(m_pOutBuff+sizeof(StPrt),&framelen, sizeof(framelen));
	memcpy(m_pOutBuff+sizeof(StPrt)+off,&actualRowNum, sizeof(actualRowNum));
	
	StPrt.Length = sizeof(StPrt) + offset - 10/*原来是12*/ + 2;
	StPrt.dataSize = sizeof(StPrt)+offset+2 - 8; //andy add 2007.08.31
	memcpy(m_pOutBuff,&StPrt,sizeof(StPrt));
	
	WORD wdVal = 'ED';
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&wdVal,sizeof(wdVal));
	
	SendOutFrame(m_pOutBuff,sizeof(StPrt)+offset+2);

	/*
	char logstr[255];
	sprintf(logstr,"SendLast1  %.6s %d strike=%f calllast=%f putlast=%f",pFm->strICode,pMf->lTransDate,
			pSl->m_fStrike,pSl->m_dat[0].fLast,pSl->m_dat[1].fLast);
	WriteLogFile(logstr);
	*/

	return S_OK;
}

HRESULT CQSNative_Option::SendData(COptFrame* pFm, COptMonth* pMf, COptLine* pSl, int nType, DWORD channelID)
{
	if(pFm == NULL || pMf == NULL || pSl == NULL || nType < 0 || nType > 1)
		return S_FALSE;

	STProtocol_SR_RET2 StPrt;

//	StPrt.ReqIdNum =1;
//	StPrt.ReqID = 0;
	StPrt.headMark = 'AA55';
	StPrt.BegFlag = 'WS';
	StPrt.DFFlag = 'DF';
	StPrt.DataOff = 14;
	StPrt.CIFlag = 'CI';	
	StPrt.ChannelID = channelID;	
	StPrt.DAFlag = 'DA';	

	unsigned char cbIndicator;
	FrameHead	framehead;
	FrameID		frameid;
	unsigned short usEType;
	
	framehead.wFrameType = 'DO';
	frameid.cbGroupCode = pFm->cGCode;
	memcpy(frameid.szItemCode, pFm->strICode, G_GENCODE__LEN);

	frameid.fStrikePrice = pSl->m_fStrike;
	frameid.lTransdate = pMf->lTransDate;
	frameid.cbOrder = (unsigned char)(nType + 1);

	//Andy add for test
	/*
#ifdef _DEBUG
	char strMsg[200];
	char sItem[9];
	memset(sItem,0,sizeof(sItem));
	memcpy(sItem,frameid.szItemCode,8);
//	ATLTRACE("COptionsObj SendData : ItemCode = %s , Month =%d ,Strike = %.2f\n" ,
//		sItem,frameid.lTransdate ,frameid.fStrikePrice);
	sprintf(strMsg,"COptionsObj SendData : ItemCode = %s , Month =%d ,Strike = %.2f" ,
		sItem,frameid.lTransdate ,frameid.fStrikePrice);
	WriteLogFile(strMsg);
#endif
	*/
	//Andy add for test
	
	BYTE byBuf[300];
	memset(byBuf, 0, sizeof(byBuf));
	unsigned short usLen = sizeof(StPrt);

	usLen += 2;
	memcpy(byBuf + usLen, &framehead, sizeof(FrameHead));
	usLen += sizeof(FrameHead);
	
	memcpy(byBuf + usLen, &frameid, sizeof(FrameID));
	usLen += sizeof(FrameID);

	usEType = 'DP';
	memcpy(byBuf + usLen, &usEType, 2);
	usLen += 2;
	memcpy(byBuf + usLen, &pMf->cDecimal, 1);
	usLen += 1;

	if(pSl->opChg[nType].ucCumulate)
	{
		usEType = 'PT';				// Cumulate Vol.
		memcpy(byBuf + usLen, &usEType, 2);
		usLen += 2;
		memcpy(byBuf + usLen, &pSl->m_dat[nType].fCumulate, 4);
		usLen += 4;
	}

	if(pSl->opChg[nType].ucHigh)
	{
		usEType = 'PH';				// High
		memcpy(byBuf + usLen, &usEType, 2);
		usLen += 2;
		memcpy(byBuf + usLen, &pSl->m_dat[nType].fHigh, 4);
		usLen += 4;
	}

	if(pSl->opChg[nType].ucLow)
	{
		usEType = 'PL';				// Low
		memcpy(byBuf + usLen, &usEType, 2);
		usLen += 2;
		memcpy(byBuf + usLen, &pSl->m_dat[nType].fLow, 4);
		usLen += 4;
	}

	if(pSl->opChg[nType].ucLast)
	{
		usEType = 'PN';			// Last(Nominal)
		memcpy(byBuf + usLen, &usEType, 2);
		usLen += 2;
		memcpy(byBuf + usLen, &pSl->m_dat[nType].fLast, 4);
		usLen += 4;
		
		cbIndicator = pSl->m_dat[nType].ucCabLast << 1;
		memcpy(byBuf + usLen, &cbIndicator, 1);
		usLen += 1;
	}

	if(pSl->opChg[nType].ucBid)
	{
		usEType = 'PB';			// Bid
		memcpy(byBuf + usLen, &usEType, 2);
		usLen += 2;
		memcpy(byBuf + usLen, &pSl->m_dat[nType].fBid, 4);
		usLen += 4;
		
		cbIndicator = pSl->m_dat[nType].ucTheoretBid  + (pSl->m_dat[nType].ucCabBid << 1);
		memcpy(byBuf + usLen, &cbIndicator, 1);
		usLen += 1;
	}

	if(pSl->opChg[nType].ucAsk)
	{
		usEType = 'PA';				// Ask
		memcpy(byBuf + usLen, &usEType, 2);
		usLen += 2;
		memcpy(byBuf + usLen, &pSl->m_dat[nType].fAsk, 4);
		usLen += 4;
		
		cbIndicator = pSl->m_dat[nType].ucTheoretAsk;
		memcpy(byBuf + usLen, &cbIndicator, 1);
		usLen += 1;
	}
	
	if(pSl->opChg[nType].ucOI)
	{
		usEType = 'OI';				// O / I
		memcpy(byBuf + usLen, &usEType, 2);
		usLen += 2;
		memcpy(byBuf + usLen, &pSl->m_dat[nType].fOI, 4);
		usLen += 4;
	}

	if(pSl->opChg[nType].ucLastQty)
	{
		usEType = 'QN';				// Last Quantity
		memcpy(byBuf + usLen, &usEType, 2);
		usLen += 2;
		memcpy(byBuf + usLen, &pSl->m_dat[nType].fLastQty, 4);
		usLen += 4;
	}

	if(pSl->opChg[nType].ucAskQty)
	{
		usEType = 'QA';				// Ask Quantity
		memcpy(byBuf + usLen, &usEType, 2);
		usLen += 2;
		memcpy(byBuf + usLen, &pSl->m_dat[nType].fAskQty, 4);
		usLen += 4;
	}

	if(pSl->opChg[nType].ucBidQty)
	{
		usEType = 'QB';				// Bid Quantity
		memcpy(byBuf + usLen, &usEType, 2);
		usLen += 2;
		memcpy(byBuf + usLen, &pSl->m_dat[nType].fBidQty, 4);
		usLen += 4;
	}

	usLen -= 2 + sizeof(StPrt);
	memcpy(byBuf+sizeof(StPrt), &usLen, sizeof(short));

	StPrt.Length = sizeof(StPrt)+ usLen + 2 - 10/*原来是12*/ + 2 ;
	StPrt.dataSize = sizeof(StPrt)+usLen+2+2 - 8; //andy add 2007.08.31
	memcpy(byBuf, &StPrt, sizeof(StPrt));

	WORD wdVal = 'ED';
	memcpy(byBuf+sizeof(StPrt)+usLen+2,&wdVal,2);
		
	SendOutFrame(byBuf,sizeof(StPrt)+usLen+2+2);	

	return S_OK;
}

HRESULT CQSNative_Option::GetDataHistory(COptFrame* pFm, COptMonth* pMf, 
									COptLine* pSl, int nType,char* pBuff,long& nLen)
{
	if(pFm == NULL || pMf == NULL || pSl == NULL || nType < 0 || nType > 1)
		return S_FALSE;

	unsigned char cbIndicator;

	unsigned short usLen = 0;
	char* byBuf = pBuff;


	memcpy(byBuf + usLen, &pSl->m_dat[nType].fOI, 4);
	usLen += 4;

	memcpy(byBuf + usLen, &pSl->m_dat[nType].fCumulate, 4);
	usLen += 4;

	memcpy(byBuf + usLen, &pSl->m_dat[nType].fHigh, 4);
	usLen += 4;

	memcpy(byBuf + usLen, &pSl->m_dat[nType].fLow, 4);
	usLen += 4;

	memcpy(byBuf + usLen, &pSl->m_dat[nType].fLast, 4);
	usLen += 4;
	cbIndicator = pSl->m_dat[nType].ucCabLast << 1;
	memcpy(byBuf + usLen, &cbIndicator, 1);
	usLen += 1;

	memcpy(byBuf + usLen, &pSl->m_dat[nType].fLastQty, 4);
	usLen += 4;

	memcpy(byBuf + usLen, &pSl->m_dat[nType].fBidQty, 4);
	usLen += 4;

	memcpy(byBuf + usLen, &pSl->m_dat[nType].fBid, 4);
	usLen += 4;
	cbIndicator = pSl->m_dat[nType].ucTheoretBid + (pSl->m_dat[nType].ucCabBid << 1);
	memcpy(byBuf + usLen, &cbIndicator, 1);
	usLen += 1;

	memcpy(byBuf + usLen, &pSl->m_dat[nType].fAsk, 4);
	usLen += 4;
	cbIndicator = pSl->m_dat[nType].ucTheoretAsk;
	memcpy(byBuf + usLen, &cbIndicator, 1);
	usLen += 1;

	memcpy(byBuf + usLen, &pSl->m_dat[nType].fAskQty, 4);
	usLen += 4;
	/*
	CString asd;
	asd.Format("%.6s %d-%f,%f,%f,%f,%f",pFm->strICode,nType,pSl->m_fStrike,pSl->m_dat[nType].fAsk,pSl->m_dat[nType].fBid,
		pSl->m_dat[nType].fAskQty,pSl->m_dat[nType].fBidQty);
	WriteLogFile((LPSTR)(LPCSTR)asd);
	*/
	nLen = usLen;
	return S_OK;
}

HRESULT CQSNative_Option::SendDelFrameToUser(char cGCode, COptRecFrame* pRec, BOOL bDelStrike)
{	
	CString strUser;
	OptUserInfo usinfo;

	EnterCriticalSection(&m_csUser);		
	POSITION pos=m_UserList.GetStartPosition();
	int i=0;
	while(pos)
	{		
		m_UserList.GetNextAssoc(pos,strUser,usinfo);
		if(usinfo.dwCount > 0)
			SendDelFrame(cGCode, pRec, bDelStrike,usinfo.dwChannelID);	
	}	
	LeaveCriticalSection(&m_csUser);

	return S_OK;
}
HRESULT CQSNative_Option::SendDelFrame(char cGCode, COptRecFrame* pRec, BOOL bDelStrike,DWORD channelID)
{//即时数据
	if(pRec == NULL)
		return S_FALSE;

	STProtocol_SR_RET2 StPrt;

//	StPrt.ReqIdNum =1;
//	StPrt.ReqID = 0;
	StPrt.headMark = 'AA55';
	StPrt.BegFlag = 'WS';
	StPrt.DFFlag = 'DF';
	StPrt.DataOff = 14;
	StPrt.CIFlag = 'CI';	
	StPrt.ChannelID = channelID;	
	StPrt.DAFlag = 'DA';	

	FrameHead	framehead;
	FrameID		frameid;
	unsigned short usEType;
	
	framehead.wFrameType = 'DO';
	frameid.cbGroupCode = cGCode;
	memcpy(frameid.szItemCode, pRec->ucICode, G_GENCODE__LEN);

	frameid.fStrikePrice = pRec->fStrike;
	frameid.lTransdate = pRec->lTransDate;
	frameid.cbOrder = 0;
	
	
	BYTE byBuf[150];
	memset(byBuf, 0, sizeof(byBuf));
	unsigned short usLen = sizeof(StPrt);

	usLen += 2;
	memcpy(byBuf + usLen, &framehead, sizeof(FrameHead));
	usLen += sizeof(FrameHead);
	
	memcpy(byBuf + usLen, &frameid, sizeof(FrameID));
	usLen += sizeof(FrameID);

	if(bDelStrike)
	{
		usEType = 'CS';				// Delete Strike
		memcpy(byBuf + usLen, &usEType, 2);
		usLen += 2;

//Andy add for test
#ifdef _DEBUG
	char strMsg[200];
	char sItem[9];
	memset(sItem,0,sizeof(sItem));
	memcpy(sItem,frameid.szItemCode,8);	
	sprintf(strMsg,"TranslateNewFrame DeleteStrike : Item=%s,Transdate=%d,Strike= %.3f",sItem, frameid.lTransdate,frameid.fStrikePrice);
	WriteLogFile(strMsg);
#endif	
//Andy add for test

	}
	else
	{
		usEType = 'CM';				// Delete Month
		memcpy(byBuf + usLen, &usEType, 2);
		usLen += 2;
	}

	usLen -= 2 + sizeof(StPrt);
	memcpy(byBuf+sizeof(StPrt), &usLen, sizeof(short));

	StPrt.Length = sizeof(StPrt)+ usLen + 2 - 10/*原来是12*/ + 2 ;
	StPrt.dataSize = sizeof(StPrt)+usLen+2+2 - 8; //andy add 2007.08.31
	memcpy(byBuf, &StPrt, sizeof(StPrt));
	
	WORD wdVal = 'ED';
	memcpy(byBuf+sizeof(StPrt)+usLen+2,&wdVal,2);
	
	SendOutFrame(byBuf,sizeof(StPrt)+usLen+2+2);	

	return S_OK;
}

void CQSNative_Option::SortPainList()
{
	MaxPainOpt temp;
	for (int i=0;i<m_arrMaxPainOpt.GetSize();i++)
	{
		for (int j=0;j<m_arrMaxPainOpt.GetSize()-i-1;j++)
		{
			if (m_arrMaxPainOpt[j].date>m_arrMaxPainOpt[j+1].date)
			{
				temp=m_arrMaxPainOpt[j];
				m_arrMaxPainOpt[j]=m_arrMaxPainOpt[j+1];
				m_arrMaxPainOpt[j+1]=temp;
			}
		}
	}
}

BYTE CQSNative_Option::Compress( LPCSTR InFile, LPCSTR OutFile )
{
	//* start keyboard 
	try
	{
		CoInitialize(0);
//		ITTCompress* pObj;
		HRESULT	hr;
		
		CComBSTR bsIn((LPCSTR)InFile); 
		CComBSTR bsOut((LPCSTR)OutFile); 

//		hr=CoCreateInstance(CLSID_TTCompress,0,CLSCTX_ALL,IID_ITTCompress,(void**)&pObj);
//		if(FAILED(hr) || pObj==NULL)
		{
			CoUninitialize();
			CString logstr;
			logstr.Format("IID_ITTCompress create error=%d!",GetLastError());
			WriteLogFile((LPSTR)(LPCSTR)logstr);
			return 1;
		}

//		if (pObj!=NULL)
		{
//			hr=pObj->CompressAddFiles(bsIn,1);
		}
//		if (pObj!=NULL)
		{
//			hr=pObj->CompressFiles(bsOut);
		}
		
		CoUninitialize();
		if(hr!=S_OK)
		{
			return 1;
		}
		return 0;	
	}
	catch(...)
	{
		return 1;
	}
}
HANDLE SVEvent;
int    bWrite;
UINT WriteMaxPain(LPVOID para)
{
	CQSNative_Option* opt=(CQSNative_Option*)para;

	if (bWrite==0)
	{
		bWrite=1;
		/*
		char strFileName[MAX_PATH];
		strcpy(strFileName, "OptMaxPain.dat");
		
		HANDLE hFile;
		hFile = CreateFile(strFileName, GENERIC_READ|GENERIC_WRITE, 
			FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		
		DWORD dwLen;
		SetFilePointer(hFile, 0, 0, FILE_BEGIN);
		
		opt->SortPainList();
		int daynums=opt->m_MaxPainList.GetSize();
		
		char head[12]="OPTMAXPAIN2";
		
		BOOL bSec = WriteFile(hFile, &head, sizeof(head), &dwLen, NULL);
		if(!bSec || dwLen != sizeof(head))
		{
			bWrite=0;
			SetEvent(SVEvent);
			CloseHandle(hFile);
			return FALSE;
		}
		
		bSec = WriteFile(hFile, &daynums, sizeof(daynums), &dwLen, NULL);
		if(!bSec || dwLen != sizeof(daynums))
		{
			bWrite=0;
			SetEvent(SVEvent);
			CloseHandle(hFile);
			return FALSE;
		}
		
		for(int i=0;i<daynums;i++)
		{
			bSec = WriteFile(hFile, &opt->m_MaxPainList[i], sizeof(opt->m_MaxPainList[i]), &dwLen, NULL);
			if(!bSec || dwLen != sizeof(opt->m_MaxPainList[i]))
			{
				bWrite=0;
				SetEvent(SVEvent);
				CloseHandle(hFile);
				return FALSE;
			}
		}
		CloseHandle(hFile);
		*/	
		opt->WriteLogFile("=================WriteMaxPain Start=================");
		
		char strFileName[MAX_PATH];
		memset(strFileName, 0, sizeof(strFileName));
		strcpy(strFileName, "OptMaxPain.dat");
		
		HANDLE hFile;
		hFile = CreateFile(strFileName, GENERIC_READ|GENERIC_WRITE, 
			FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		
		DWORD dwLen;
		SetFilePointer(hFile, 0, 0, FILE_BEGIN);
		
		char head[12]="OPTMAXPAIN3";
		
		BOOL bRet = WriteFile(hFile, &head, sizeof(head), &dwLen, NULL);
		if(!bRet || dwLen != sizeof(head))
		{
			CloseHandle(hFile);
			return FALSE;
		}
		int nSize = opt->m_arrMaxPainOpt.GetSize();
		
		bRet = WriteFile(hFile, &nSize, sizeof(nSize), &dwLen, NULL);
		if(!bRet || dwLen != sizeof(nSize))
		{
			CloseHandle(hFile);
			return FALSE;
		}
		
		int i, j, k;
		for (i = 0; i < nSize; i++)
		{
			bRet = WriteFile(hFile, &(opt->m_arrMaxPainOpt[i].date), sizeof(opt->m_arrMaxPainOpt[i].date), &dwLen, NULL);
			if(!bRet || dwLen != sizeof(opt->m_arrMaxPainOpt[i].date))
			{
				CloseHandle(hFile);
				return FALSE;
			}
			int nItems = opt->m_arrMaxPainOpt[i].arrItemData.GetSize();
			bRet = WriteFile(hFile, &nItems, sizeof(nItems), &dwLen, NULL);
			if(!bRet || dwLen != sizeof(nItems))
			{
				CloseHandle(hFile);
				return FALSE;
			}
			for (j = 0; j < nItems; j++)
			{
				bRet = WriteFile(hFile, &(opt->m_arrMaxPainOpt[i].arrItemData[j].bGroup), sizeof(opt->m_arrMaxPainOpt[i].arrItemData[j].bGroup), &dwLen, NULL);
				if(!bRet || dwLen != sizeof(opt->m_arrMaxPainOpt[i].arrItemData[j].bGroup))
				{
					CloseHandle(hFile);
					return FALSE;
				}
				
				bRet = WriteFile(hFile, &(opt->m_arrMaxPainOpt[i].arrItemData[j].bItem), sizeof(opt->m_arrMaxPainOpt[i].arrItemData[j].bItem), &dwLen, NULL);
				if(!bRet || dwLen != sizeof(opt->m_arrMaxPainOpt[i].arrItemData[j].bItem))
				{
					CloseHandle(hFile);
					return FALSE;
				}
				
				int nMonths = opt->m_arrMaxPainOpt[i].arrItemData[j].arrMonthData.GetSize();
				bRet = WriteFile(hFile, &nMonths, sizeof(nMonths), &dwLen, NULL);
				if(!bRet || dwLen != sizeof(nMonths))
				{
					CloseHandle(hFile);
					return FALSE;
				}
				
				for (k = 0; k < nMonths; k++)
				{
					bRet = WriteFile(hFile, &(opt->m_arrMaxPainOpt[i].arrItemData[j].arrMonthData[k]), sizeof(MaxPainMonthData), &dwLen, NULL);
					if(!bRet || dwLen != sizeof(MaxPainMonthData))
					{
						CloseHandle(hFile);
						return FALSE;
					}
				}
			}
		}
		
		CloseHandle(hFile);

		
		memset(&opt->m_OptDatPath1,0,MAX_PATH);
		memset(&opt->m_OptDatPath2,0,MAX_PATH);
		CString str;
		CString inifile;
		CWrapperPath::GetExtPath(str);
		char szTemp[1000] ;
		inifile.Format( "%sMS.INI",  str ) ;
		
		GetPrivateProfileString( "optmaxpain", "Path1", "", opt->m_OptDatPath1, 800, inifile ) ;
		GetPrivateProfileString( "optmaxpain", "Path2", "", opt->m_OptDatPath2, 800, inifile ) ;
		
		if (strlen(opt->m_OptDatPath1)>0)
		{
			SetFileAttributes(opt->m_OptDatPath1,FILE_ATTRIBUTE_NORMAL);
			CopyFile(strFileName,opt->m_OptDatPath1,FALSE);
		}
		if (strlen(opt->m_OptDatPath2)>0)
		{
			SetFileAttributes(opt->m_OptDatPath2,FILE_ATTRIBUTE_NORMAL);
			CopyFile(strFileName,opt->m_OptDatPath2,FALSE);
		}
		
		//生成压缩文件
		SetFileAttributes(strFileName,FILE_ATTRIBUTE_NORMAL);
		opt->Compress(strFileName,"OptMaxPain.z");
		GetPrivateProfileString( "optmaxpain", "ZipPath1", "", opt->m_OptDatPath1, 800, inifile ) ;
		GetPrivateProfileString( "optmaxpain", "ZipPath2", "", opt->m_OptDatPath2, 800, inifile ) ;
		
		if (strlen(opt->m_OptDatPath1)>0)
		{
			SetFileAttributes(opt->m_OptDatPath1,FILE_ATTRIBUTE_NORMAL);
			CopyFile("OptMaxPain.z",opt->m_OptDatPath1,FALSE);	
		}
		if (strlen(opt->m_OptDatPath2)>0)
		{
			SetFileAttributes(opt->m_OptDatPath2,FILE_ATTRIBUTE_NORMAL);
			CopyFile("OptMaxPain.z",opt->m_OptDatPath2,FALSE);
		}
		char buffer[200];
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "WriteMaxPain=====number of days = %d", opt->m_arrMaxPainOpt.GetSize());
		opt->WriteLogFile(buffer);
		opt->WriteLogFile("=================WriteMaxPain End=================");
		bWrite=0;
		SetEvent(SVEvent);
	}

	return 0;
}

BOOL CQSNative_Option::SaveMaxPain()
{
	SVEvent = CreateEvent(
		NULL,
		TRUE,
		FALSE,
		NULL);

	AfxBeginThread(WriteMaxPain,(LPVOID)this);

	/*
	while(1)
	{
		DWORD rc=WaitForSingleObject(SVEvent,1000*60);
		
		if( rc==WAIT_OBJECT_0 )
		{
			if (bWrite==0)
			{
				break;
			}
		}
		if (rc==WAIT_FAILED || rc==WAIT_TIMEOUT)
		{
			if (bWrite==-1)
			{
				bWrite=0;
				return FALSE;
			}
		}

	}
	*/
	return TRUE;
}

void CQSNative_Option::VersionCompatibility(CMaxPainOptArray& maxPainOpt)
{
	maxPainOpt.RemoveAll();
	int i, j, k;
	for (i = 0; i < m_MaxPainList.GetSize(); i++)
	{
		MaxPainOpt opt;
		opt.date = m_MaxPainList[i].date;
		for (j = 0; j < TT_OPT_MAXITEM; j++)
		{
			char ch = m_MaxPainList[i].m_items[j].group;
			if (m_MaxPainList[i].m_items[j].group != 0)
			{
				MaxPainItemData itemData;
				itemData.bGroup = m_MaxPainList[i].m_items[j].group;
				memcpy(&itemData.bItem, &m_MaxPainList[i].m_items[j].item, sizeof(itemData.bItem));
				for (k = 0; k < TT_OPT_MAXMONTH; k++)
				{
					if (m_MaxPainList[i].m_items[j].month[k] != 0)
					{
						MaxPainMonthData monthData;
						monthData.dwMonth = m_MaxPainList[i].m_items[j].month[k];
						monthData.dStrike = m_MaxPainList[i].m_items[j].strike[k];
						monthData.dPvalue = m_MaxPainList[i].m_items[j].pvalue[k];
						itemData.arrMonthData.Add(monthData);
					}
				}
				opt.arrItemData.Add(itemData);
			}
		}
		maxPainOpt.Add(opt);
	}
	m_MaxPainList.RemoveAll();
	
}

void CQSNative_Option::VersionCompatibility(MaxPainOpt& opt, const OptMaxPain& temp)
{
	int i, j, k;
	opt.date = temp.date;
	for (j = 0; j < TT_OPT_MAXITEM; j++)
	{
		char ch = temp.m_items[j].group;
		if (temp.m_items[j].group != 0)
		{
			MaxPainItemData itemData;
			itemData.bGroup = temp.m_items[j].group;
			memcpy(&itemData.bItem, &temp.m_items[j].item, sizeof(itemData.bItem));
			for (k = 0; k < TT_OPT_MAXMONTH; k++)
			{
				if (temp.m_items[j].month[k] != 0)
				{
					MaxPainMonthData monthData;
					monthData.dwMonth = temp.m_items[j].month[k];
					monthData.dStrike = temp.m_items[j].strike[k];
					monthData.dPvalue = temp.m_items[j].pvalue[k];
					itemData.arrMonthData.Add(monthData);
				}
			}
			opt.arrItemData.Add(itemData);
		}
	}

}

BOOL CQSNative_Option::LoadMaxPain()
{
	char strFileName[MAX_PATH];
	strcpy(strFileName, "OptMaxPain.dat");
	WriteLogFile("=================LoadMaxPain Start=================");
	HANDLE hFile;
	hFile = CreateFile(strFileName, GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
	
	DWORD dwLen;
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	
	char head[12]={0};
	m_MaxPainList.RemoveAll();

	BOOL bSec = ReadFile(hFile, &head, sizeof(head), &dwLen, NULL);
	if(!bSec || dwLen != sizeof(head))
	{
		CloseHandle(hFile);
		return S_FALSE;
	}
	

	if (strnicmp(head,"OPTMAXPAIN1",sizeof(head)) == 0)
	{
		int daynums=0;
		
		WriteLogFile("LoadMaxPain====Version(OPTMAXPAIN1)");
		bSec = ReadFile(hFile, &daynums, sizeof(daynums), &dwLen, NULL);
		if(!bSec || dwLen != sizeof(daynums) || daynums<=0)
		{
			daynums = 0;
			CloseHandle(hFile);
			return S_FALSE;
		}
		for(int i=0;i<daynums;i++)
		{
			OldOptMaxPain temp;
			memset(&temp,0,sizeof(OldOptMaxPain));
			bSec = ReadFile(hFile, &temp, sizeof(temp), &dwLen, NULL);
			if(!bSec || dwLen != sizeof(temp))
			{
				CloseHandle(hFile);
				return S_FALSE;
			}
			
			if (!IsDateValid(temp.date))
			{
				CloseHandle(hFile);
				return S_FALSE;
			}

			OptMaxPain temp2;
			memset(&temp2,0,sizeof(OptMaxPain));
			temp2.date=temp.date;
			
			for (int n=0;n<TT_OPT_MAXITEM;n++)
			{
				memcpy(temp2.m_items[n].item,temp.m_items[n].item,8);
				temp2.m_items[n].group=temp.m_items[n].group;

				memcpy(temp2.m_items[n].strike,temp.m_items[n].strike,sizeof(double)*12);
				memcpy(temp2.m_items[n].month,temp.m_items[n].month,sizeof(int)*12);
				memcpy(temp2.m_items[n].pvalue,temp.m_items[n].pvalue,sizeof(double)*12);
			}
			
			m_MaxPainList.Add(temp2);
		}
		
		VersionCompatibility(m_arrMaxPainOpt);
	}
	else if (strnicmp(head,"OPTMAXPAIN2",sizeof(head)) == 0)
	{
		int daynums=0;
		
		WriteLogFile("LoadMaxPain====Version(OPTMAXPAIN2)");
		bSec = ReadFile(hFile, &daynums, sizeof(daynums), &dwLen, NULL);
		if(!bSec || dwLen != sizeof(daynums) || daynums<=0)
		{
			daynums = 0;
			CloseHandle(hFile);
			return S_FALSE;
		}
		for(int i=0;i<daynums;i++)
		{
			OptMaxPain temp;
			memset(&temp,0,sizeof(OptMaxPain));
			bSec = ReadFile(hFile, &temp, sizeof(temp), &dwLen, NULL);
			if(!bSec || dwLen != sizeof(temp))
			{
				CloseHandle(hFile);
				return S_FALSE;
			}
			
			if (!IsDateValid(temp.date))
			{
				CloseHandle(hFile);
				return S_FALSE;
			}
			
			m_MaxPainList.Add(temp);
		}
		
		VersionCompatibility(m_arrMaxPainOpt);
	}
	else if (strnicmp(head,"OPTMAXPAIN3",sizeof(head)) == 0)
	{
		int nSize;
		WriteLogFile("LoadMaxPain====Version(OPTMAXPAIN3)");
		m_arrMaxPainOpt.RemoveAll();
		BOOL bRet = ReadFile(hFile, &nSize, sizeof(nSize), &dwLen, NULL);
		if(!bRet || dwLen != sizeof(nSize))
		{
			CloseHandle(hFile);
//			
			return FALSE;
		}
		
		int i, j, k;
		for (i = 0; i < nSize; i++)
		{
			MaxPainOpt opt;
			bRet = ReadFile(hFile, &(opt.date), sizeof(opt.date), &dwLen, NULL);
			if(!bRet || dwLen != sizeof(opt.date))
			{
				CloseHandle(hFile);
				return FALSE;
			}
			int nItems;
			bRet = ReadFile(hFile, &nItems, sizeof(nItems), &dwLen, NULL);
			if(!bRet || dwLen != sizeof(nItems))
			{
				CloseHandle(hFile);
				return FALSE;
			}
			for (j = 0; j < nItems; j++)
			{
				MaxPainItemData itemData;
				bRet = ReadFile(hFile, &(itemData.bGroup), sizeof(itemData.bGroup), &dwLen, NULL);
				if(!bRet || dwLen != sizeof(itemData.bGroup))
				{
					CloseHandle(hFile);
					return FALSE;
				}
				
				bRet = ReadFile(hFile, &(itemData.bItem), sizeof(itemData.bItem), &dwLen, NULL);
				if(!bRet || dwLen != sizeof(itemData.bItem))
				{
					CloseHandle(hFile);
					return FALSE;
				}
				
				int nMonths;
				bRet = ReadFile(hFile, &nMonths, sizeof(nMonths), &dwLen, NULL);
				if(!bRet || dwLen != sizeof(nMonths))
				{
					CloseHandle(hFile);
					return FALSE;
				}
				
				for (k = 0; k < nMonths; k++)
				{
					MaxPainMonthData monthData;
					bRet = ReadFile(hFile, &monthData, sizeof(MaxPainMonthData), &dwLen, NULL);
					if(!bRet || dwLen != sizeof(MaxPainMonthData))
					{
						CloseHandle(hFile);
						return FALSE;
					}
					itemData.arrMonthData.Add(monthData);
				}
				opt.arrItemData.Add(itemData);
			}
			m_arrMaxPainOpt.Add(opt);
		}
	}
	
	
	CloseHandle(hFile);
	SortPainList();
	CString strMsg;
	strMsg.Format("LoadMaxPain ===> number of days = %d", m_arrMaxPainOpt.GetSize());
	char buffer[200];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, strMsg);
	WriteLogFile(buffer);
	WriteLogFile("=================LoadMaxPain End=================");
	return TRUE;
}

int  CQSNative_Option::FindPain(long date)
{
	if (!IsDateValid(date))
	{
		return -1;
	}
	for (int i=0;i<m_arrMaxPainOpt.GetSize();i++)
	{
		if (date == m_arrMaxPainOpt[i].date)
		{
			return i;
		}
	}
	return -1;
}

BOOL CQSNative_Option::CalMaxPain()
{
	OptMaxPain temp;
	memset(&temp,0,sizeof(OptMaxPain));
	int num = FindPain(m_nTransDate);
	if (num >= 0)
	{
		m_arrMaxPainOpt.RemoveAt(num);
	}
	
	temp.date=m_nTransDate;
	for(int i=0; i < m_nItemNum; i++)
	{
		if (strlen((char*)m_optFm[i].strICode)<=0)
			continue;
		memcpy(&temp.m_items[i].item,m_optFm[i].strICode,8);
		temp.m_items[i].group = m_optFm[i].cGCode;
			
		int month = m_optFm[i].ucMonthNum;
		
		for (int j=0; j < month; j++)
		{
			float maxpain[TT_OPT_MAXROW]={0};
			int rowNum = m_optFm[i].m_optMonth[j].ucTotalRow;
			for (int m=0; m < rowNum; m++)
			{
				double row_pain=0;
				float m_price=m_optFm[i].m_optMonth[j].m_optLine[m].m_fStrike;
				if (m_price<=0)
					continue;
				
				for(int n=0; n < rowNum; n++)
				{
					float row_price=m_optFm[i].m_optMonth[j].m_optLine[n].m_fStrike;

					if (row_price<=0)
						continue;
					if ( m_price > row_price )
					{
						row_pain += (m_price-row_price)*m_optFm[i].m_optMonth[j].m_optLine[n].m_dat[0].fOI;
					}
					if ( m_price < row_price )
					{
						row_pain += (row_price-m_price)*m_optFm[i].m_optMonth[j].m_optLine[n].m_dat[1].fOI;
					}
				}

				maxpain[m]=row_pain;
			}
			double low = maxpain[0];
			int index=0;
			for (m = 0; m < rowNum; m++)
			{
				if (maxpain[m] <= 0)
					continue;
				
				if (low > maxpain[m])
				{
					low = maxpain[m];
					index = m;
				}
			}
			temp.m_items[i].month[j]  = m_optFm[i].m_optMonth[j].lTransDate;
			temp.m_items[i].strike[j] = m_optFm[i].m_optMonth[j].m_optLine[index].m_fStrike;
			temp.m_items[i].pvalue[j] = low;
			
			CString asd;
			asd.Format("%.8s %d,%f,%f",temp.m_items[i].item,temp.m_items[i].month[j],
				temp.m_items[i].strike[j],temp.m_items[i].pvalue[j]);
			WriteLogFile((LPSTR)(LPCSTR)asd);
			
			if (low==0)
			{
				temp.m_items[i].strike[j]=0;
			}
		}
	}

	if (IsDateValid(temp.date))
	{
		MaxPainOpt opt;
		VersionCompatibility(opt, temp);
		m_arrMaxPainOpt.Add(opt);
	}
	
	CString msg;
	msg.Format("Cal MaxPain %d.",temp.date);
	WriteLogFile((LPSTR)(LPCSTR)msg);

	return TRUE;
}

HRESULT CQSNative_Option::StoreManage(BOOL bIsRead)
{
	char strFileName[MAX_PATH];
	strcpy(strFileName, "Options.dat");

	HANDLE hFile;
	if(bIsRead)
	{
		hFile = CreateFile(strFileName, GENERIC_READ|GENERIC_WRITE, 
			FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
	}
	else
	{
		hFile = CreateFile(strFileName, GENERIC_READ|GENERIC_WRITE, 
			FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	}
	if(hFile == INVALID_HANDLE_VALUE)
		return S_FALSE;

	DWORD dwLen;
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	if(bIsRead)
	{
		BOOL bSec = ReadFile(hFile, &m_nItemNum, sizeof(m_nItemNum), &dwLen, NULL);
		if(!bSec || dwLen != sizeof(m_nItemNum))
		{
			m_nItemNum = 0;
			CloseHandle(hFile);
			return S_FALSE;
		}
		bSec = ReadFile(hFile, &m_nTransDate, sizeof(m_nTransDate), &dwLen, NULL);
		if(!bSec || dwLen != sizeof(m_nTransDate))
		{
			m_nTransDate = 0;
			CloseHandle(hFile);
			return S_FALSE;
		}
		bSec = ReadFile(hFile, m_optFm, sizeof(m_optFm[0]) * m_nItemNum, &dwLen, NULL);
		if(!bSec || dwLen != sizeof(m_optFm[0]) * m_nItemNum)
		{
			for(int i = 0; i < m_nItemNum; i ++)
			{
				m_optFm[i].Init();
			}
			CloseHandle(hFile);
			return S_FALSE;
		}

		//add for test	
		/*
		if(m_nItemNum > 0)
		{
			for (int i=0;i<m_nItemNum;i++)
			{
				COptFrame* pFm = &m_optFm[i];
				CString asd;
				asd.Format("Option: %d '%s'",i,pFm->strICode);
				WriteLogFile((LPSTR)(LPCSTR)asd);
				if (asd.Find("00700")>=0)
				{
					int nnn=0;
				}
				
				WriteLogFile("------------- StoreManage Read ---------------");
				COptMonth* pMf;
				
				for(int j=0;j<pFm->ucMonthNum;j++)
				{
					if (strstr((char*)pFm->strICode,"00700")!=NULL)
					{
						pMf = &pFm->m_optMonth[j];
						for(int n = 0; n < pMf->ucTotalRow; n ++)
						{
							char szmsg[500];
							sprintf(szmsg,"StoreManage :%.8s Date=%d Row=%d/%d strike=%6.0f,COI=%6.0f,POI=%6.0f",
								m_optFm[i].strICode,pMf->lTransDate,n,pMf->ucTotalRow,pMf->m_optLine[n].m_fStrike,
								pMf->m_optLine[n].m_dat[0].fOI,pMf->m_optLine[n].m_dat[1].fOI);
							WriteLogFile(szmsg);
						}
					}	
				}
				
			}
		}
		*/
		//add for test
	}
	else
	{
		BOOL bSec = WriteFile(hFile, &m_nItemNum, sizeof(m_nItemNum), &dwLen, NULL);
		if(!bSec || dwLen != sizeof(m_nItemNum))
		{
			CloseHandle(hFile);
			return S_FALSE;
		}
		bSec = WriteFile(hFile, &m_nTransDate, sizeof(m_nTransDate), &dwLen, NULL);
		if(!bSec || dwLen != sizeof(m_nTransDate))
		{
			CloseHandle(hFile);
			return S_FALSE;
		}
		bSec = WriteFile(hFile, m_optFm, sizeof(m_optFm[0]) * m_nItemNum, &dwLen, NULL);
		if(!bSec || dwLen != sizeof(m_optFm[0]) * m_nItemNum)
		{
			CloseHandle(hFile);
			return S_FALSE;
		}
	}
	CloseHandle(hFile);
	return S_OK;
}


HRESULT CQSNative_Option::WriteLogFile(LPTSTR strLog)
{
	//return 0;
	HANDLE m_hLogFile;
	char strFileDir[200];
	char strTemp[50];
	sprintf(strTemp,"\\W2TOptions.LOG");
	
	memset(strFileDir, 0, sizeof(strFileDir));
	GetCurrentDirectory(200, strFileDir);
	strcat(strFileDir, strTemp);
	
	m_hLogFile = CreateFile(strFileDir, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, 0, NULL);
	
	if(m_hLogFile == INVALID_HANDLE_VALUE)
		return 1;
	
	
/*	SetFilePointer(m_hLogFile, 0, NULL, FILE_BEGIN);
	DWORD dwTotal = SetFilePointer(m_hLogFile, 0, NULL, FILE_END);
	if(dwTotal > 500100)
	{
		char *Buf = new char[300100];
		memset(Buf, 0, 300100);
		SetFilePointer(m_hLogFile, -300000, NULL, FILE_END);
		ReadFile(m_hLogFile, Buf, 300000, &dwTotal, NULL);
		CloseHandle(m_hLogFile);
		m_hLogFile = CreateFile(strFileDir, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|
			FILE_SHARE_WRITE, NULL, TRUNCATE_EXISTING, 0, NULL);
		SetFilePointer(m_hLogFile, 0, NULL, FILE_BEGIN);
		WriteFile(m_hLogFile, Buf, 300000, &dwTotal, NULL);
		delete Buf;
	}
*/	
	SetFilePointer(m_hLogFile, 0, NULL, FILE_END);
	
	unsigned long nByteWrite = 0;
	char strMsg[1000];
	memset(strMsg, 0, sizeof(strMsg));
	
	char strTime[100];
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	
	sprintf(strTime, "%d-%02d-%02d %02d:%02d:%02d", sysTime.wYear, sysTime.wMonth, 
		sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	
	lstrcpy(strMsg, strTime);
	lstrcat(strMsg, "     ");
	lstrcat(strMsg, strLog);
	lstrcat(strMsg, "\r\n");
	
	WriteFile(m_hLogFile, strMsg, lstrlen(strMsg), &nByteWrite, NULL);
	
	CloseHandle(m_hLogFile);
	return S_OK;
}

void CQSNative_Option::SendRegularData()
{
	//每过5分钟发送一次整屏数据	

	DWORD   tmCur = ::GetCurrentTime() ;
	if( labs(tmCur-m_tmSend)>=5*60*1000 )	// 5Mins.
	{
		m_tmSend = tmCur;

		CString strUser;
		OptUserInfo usinfo;
		
		EnterCriticalSection(&m_csUser);		
		POSITION pos=m_UserList.GetStartPosition();
		while(pos)
		{		
			m_UserList.GetNextAssoc(pos,strUser,usinfo);
			if(usinfo.dwCount > 0)
				SendHistoricalData2(usinfo.GroupCode,usinfo.szItemCode, usinfo.lTransDate,usinfo.dwChannelID);
		}	
		LeaveCriticalSection(&m_csUser);
	}
	
	//每分钟发送一次IV数据
	if (labs(tmCur-m_IVLastSend)>=60*1000)
	{
		m_IVLastSend=tmCur;

		CString strUser;
		OptUserInfo usinfo;
		
		EnterCriticalSection(&m_csUser);		
		POSITION pos=m_UserList.GetStartPosition();
		while(pos)
		{		
			m_UserList.GetNextAssoc(pos,strUser,usinfo);
			if(usinfo.dwCount > 0)
				SendIVEveryMin(usinfo.GroupCode,usinfo.szItemCode, usinfo.lTransDate,usinfo.dwChannelID);
		}
		//发给charting
		pos=m_UserList3.GetStartPosition();
		while(pos)
		{		
			m_UserList3.GetNextAssoc(pos,strUser,usinfo);
			if(usinfo.dwCount > 0)
				SendIVEveryMin(usinfo.GroupCode,usinfo.szItemCode, usinfo.lTransDate,usinfo.dwChannelID);
		}
		LeaveCriticalSection(&m_csUser);
	}

	//每隔5分钟发PreOI
	if( labs(tmCur-m_tmSend)>=5*60*1000 )	// 5Mins.
	{
		m_tmSend = tmCur;
		
		CString strUser;
		OptUserInfo usinfo;
		
		EnterCriticalSection(&m_csUser);		
		POSITION pos=m_UserList.GetStartPosition();
		while(pos)
		{		
			m_UserList.GetNextAssoc(pos,strUser,usinfo);
			if(usinfo.dwCount > 0)
				SendPreOIEvery3Min(usinfo.GroupCode,usinfo.szItemCode, usinfo.lTransDate,usinfo.dwChannelID);
		}	
		LeaveCriticalSection(&m_csUser);
	}

}


void  CALLBACK CQSNative_Option::OneShotTimer(UINT wTimerID, UINT msg, 
										  DWORD dwUser, DWORD dw1, DWORD dw2) 
{ 
	//每过5分钟发送一次整屏数据	
	CQSNative_Option* This = (CQSNative_Option*) dwUser;
	//test
//	static int i=1;
//	char str[100];
//	sprintf(str,"TimeSetEvent Occur %2d Times",i);
//	WriteLogFile(str); 
	//test

	CString strUser;
	OptUserInfo usinfo;

	EnterCriticalSection(&This->m_csUser);		
	POSITION pos=This->m_UserList.GetStartPosition();
	while(pos)
	{		
		This->m_UserList.GetNextAssoc(pos,strUser,usinfo);
		if(usinfo.dwCount > 0)
			This->SendHistoricalData2(usinfo.GroupCode,usinfo.szItemCode, usinfo.lTransDate,usinfo.dwChannelID);
	}	
	LeaveCriticalSection(&This->m_csUser);
} 


void CQSNative_Option::GetSaveTime()
{
	HANDLE hFile;
	DWORD	dwLen;

	char strFileDir[_MAX_PATH];
	
	memset(strFileDir, 0, sizeof(strFileDir));
	GetCurrentDirectory(_MAX_PATH, strFileDir);
	strcat(strFileDir, "\\W2tOptionsServer.sav");

	hFile = CreateFile(strFileDir,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_ALWAYS/*OPEN_EXISTING*/,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return ;
	}

	ReadFile(hFile,&m_nTransDate,sizeof(m_nTransDate),&dwLen,NULL);
	if(dwLen!=sizeof(m_nTransDate))
	{
		CloseHandle(hFile);
		return ;
	}

	CloseHandle(hFile);
}

void CQSNative_Option::SaveTime()
{
	if(m_nTransDate == 0)
		return;

	HANDLE hFile;
	DWORD	dwLen;

	char strFileDir[_MAX_PATH];
	
	memset(strFileDir, 0, sizeof(strFileDir));
	GetCurrentDirectory(_MAX_PATH, strFileDir);
	strcat(strFileDir, "\\W2tOptionsServer.sav");

	hFile = CreateFile(strFileDir,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_ALWAYS/*OPEN_EXISTING*/,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return ;
	}

	WriteFile(hFile,&m_nTransDate,sizeof(m_nTransDate),&dwLen,NULL);
	if(dwLen!=sizeof(m_nTransDate))
	{
		CloseHandle(hFile);
		return ;
	}

	CloseHandle(hFile);
}

BOOL CQSNative_Option::IsDateValid(DWORD dt)
{
	char szDate[100];
	COleDateTime oleDt;
	if(dt < 10000000 || dt > 99991231) 
	{	
		return FALSE;
	}

	sprintf(szDate,"%d/%d/%d",(dt%10000)%100,(dt%10000)/100,dt/10000);
	if(oleDt.ParseDateTime(szDate,VAR_DATEVALUEONLY) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL  CQSNative_Option::DateDiff(DWORD dt1,DWORD dt2,int& iDiff)
{
	
	COleDateTime tm1,tm2;
	COleDateTimeSpan tms;

	tm1.SetDate(dt1/10000,(dt1%10000)/100,dt1%100);
	if(tm1.GetStatus()!=COleDateTime::valid)
	{
		return FALSE;
	}
	tm2.SetDate(dt2/10000,(dt2%10000)/100,dt2%100);
	if(tm2.GetStatus()!=COleDateTime::valid)
	{
		return FALSE;
	}

    tms=tm1-tm2;
	if(tms.GetStatus()!=COleDateTimeSpan::valid)
	{
		return FALSE;
	}
	
	iDiff = tms.GetDays();
	
	return TRUE;
}

int  CQSNative_Option::GetCloseNum(COptMonth* month, float price)
{
	int ret=-1;
	if (price<=0)
		return -1;
	
	for (int i=0;i<month->ucTotalRow;i++)
	{
		if (month->m_optLine[i].m_fStrike>price)
		{
			ret=i;
			break;
		}
	}
	if (ret==0 || ret==-1)
		return ret;

	if ( month->m_optLine[ret].m_fStrike-price > price-month->m_optLine[ret-1].m_fStrike )
		ret=ret-1;
	
	return ret;
}

BOOL CQSNative_Option::GetItemPrice(DayIVs& opt)
{
	if (strlen(opt.item)<=0)
		return FALSE;
/*	
	CQServer* pServer = NULL; //g_Master.GetQServer( (double)_QSERVERTYPE_TELE ) ;
	if( pServer==NULL || pServer->IsType_Native()==FALSE )
		return FALSE;
	
//	CQSNative_Tele* pTele = (CQSNative_Tele*)(pServer->m_pNativeObj) ;

	if (pTele!=NULL)
	{
		CDictionary* pDic ;
		Key* pKey = pTele->m_TransSnapShot.FindItem( opt.group, opt.item, &pDic );
		if (pKey!=NULL)
		{
			opt.open=pKey->m_pData->m_fOpen;
			opt.high=pKey->m_pData->m_fHigh;
			opt.low=pKey->m_pData->m_fLow;
			opt.close=pKey->m_pData->m_fNominal;

			return TRUE;
		}
	}
	*/

	return FALSE;
}

long CQSNative_Option::GetExpiryDays(long day)
{
	int m_lExpiryDays = 0.0;
	
    if(IsDateValid(day) == TRUE)
	{
		
		SYSTEMTIME SysTime;
		GetSystemTime(&SysTime);
		long lToday = SysTime.wYear*10000 + SysTime.wMonth*100 + SysTime.wDay;
		
		if(DateDiff(day,lToday,m_lExpiryDays) == TRUE)
		{
			m_lExpiryDays = abs(m_lExpiryDays);
			return m_lExpiryDays;
		}
	}
	
	return m_lExpiryDays;
}

BOOL CQSNative_Option::LoadIV()
{
	char strFileName[MAX_PATH];
	strcpy(strFileName, "OptIV.dat");
	
	HANDLE hFile;
	hFile = CreateFile(strFileName, GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
	
	DWORD dwLen;
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	
	char head[8]={0};
	m_OptIV.RemoveAll();
	
	BOOL bSec = ReadFile(hFile, &head, sizeof(head), &dwLen, NULL);
	if(!bSec || dwLen != sizeof(head) || strnicmp(head,"OPTIV1",sizeof(head)) != 0 )
	{
		CloseHandle(hFile);
		return S_FALSE;
	}
	int daynums=0;
	
	bSec = ReadFile(hFile, &daynums, sizeof(daynums), &dwLen, NULL);
	if(!bSec || dwLen != sizeof(daynums) || daynums<=0)
	{
		daynums = 0;
		CloseHandle(hFile);
		return S_FALSE;
	}
	
	if ( strnicmp(head,"OPTIV1",sizeof(head)) == 0 )
	{
		for(int i=0;i<daynums;i++)
		{
			OldOptIV temp;
			memset(&temp,0,sizeof(OldOptIV));
			bSec = ReadFile(hFile, &temp, sizeof(temp), &dwLen, NULL);
			if(!bSec || dwLen != sizeof(temp))
			{
				CloseHandle(hFile);
				return S_FALSE;
			}
			
			if (!IsDateValid(temp.date))
			{
				CloseHandle(hFile);
				return S_FALSE;
			}

			OptIV temp2;
			memset(&temp2,0,sizeof(OptIV));
			temp2.date=temp.date;
			for (int n=0;n<TT_OPT_MAXITEM;n++)
			{
				memcpy(temp2.m_items[i].item,temp.m_items[i].item,8);
				temp2.m_items[i].open=temp.m_items[i].open;
				temp2.m_items[i].high=temp.m_items[i].high;
				temp2.m_items[i].low=temp.m_items[i].low;
				temp2.m_items[i].close=temp.m_items[i].close;
				temp2.m_items[i].group=temp.m_items[i].group;
				memcpy(temp2.m_items[i].iv,temp.m_items[i].iv,sizeof(float)*12);
				memcpy(temp2.m_items[i].month,temp.m_items[i].month,sizeof(int)*12);
			}
			
			m_OptIV.Add(temp2);
			//memcpy(&m_OptIV,&temp,sizeof(OptIV));
		}
	}
	else if ( strnicmp(head,"OPTIV2",sizeof(head)) == 0 )
	{
		for(int i=0;i<daynums;i++)
		{
			OptIV temp;
			memset(&temp,0,sizeof(OptIV));
			bSec = ReadFile(hFile, &temp, sizeof(temp), &dwLen, NULL);
			if(!bSec || dwLen != sizeof(temp))
			{
				CloseHandle(hFile);
				return S_FALSE;
			}
			
			if (!IsDateValid(temp.date))
			{
				CloseHandle(hFile);
				return S_FALSE;
			}
			
			m_OptIV.Add(temp);
			//memcpy(&m_OptIV,&temp,sizeof(OptIV));
		}
	}
	
	CloseHandle(hFile);

	SortIVList();
	return TRUE;
}

BOOL CQSNative_Option::SaveIV()
{
	char strFileName[MAX_PATH];
	strcpy(strFileName, "OptIV.dat");
	
	HANDLE hFile;
	hFile = CreateFile(strFileName, GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	
	DWORD dwLen;
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);

	SortIVList();
	int daynums=1;
	
	char head[8]="OPTIV2";
	
	BOOL bSec = WriteFile(hFile, &head, sizeof(head), &dwLen, NULL);
	if(!bSec || dwLen != sizeof(head))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	
	bSec = WriteFile(hFile, &daynums, sizeof(daynums), &dwLen, NULL);
	if(!bSec || dwLen != sizeof(daynums))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	
	for(int i=0;i<daynums;i++)
	{
		bSec = WriteFile(hFile, &m_OptIV[i], sizeof(m_OptIV[i]), &dwLen, NULL);
		if(!bSec || dwLen != sizeof(m_OptIV[i]))
		{
			CloseHandle(hFile);
			return FALSE;
		}
	}
	CloseHandle(hFile);
	
	/*
	memset(&m_szIVPath1,0,MAX_PATH);
	memset(&m_szIVPath2,0,MAX_PATH);
	CString str;
	CString inifile;
	CWrapperPath::GetExtPath(str);
	char szTemp[1000] ;
	inifile.Format( "%sMS.INI",  str ) ;
	
	GetPrivateProfileString( "option", "SavePath1", "", m_szIVPath1, 800, inifile ) ;
	GetPrivateProfileString( "option", "SavePath2", "", m_szIVPath2, 800, inifile ) ;
	
	if (strlen(m_szIVPath1)>0)
	{
		SetFileAttributes(m_szIVPath1,FILE_ATTRIBUTE_NORMAL);
		CopyFile(strFileName,m_szIVPath1,FALSE);
	}
	if (strlen(m_szIVPath2)>0)
	{
		SetFileAttributes(m_szIVPath2,FILE_ATTRIBUTE_NORMAL);
		CopyFile(strFileName,m_szIVPath2,FALSE);
	}
	*/

	return TRUE;
}

void CQSNative_Option::SortIVList()
{
	
	OptIV temp;
	memset(&temp,0,sizeof(OptIV));
	for (int i=0;i<m_OptIV.GetSize();i++)
	{
		for (int j=0;j<m_OptIV.GetSize()-i-1;j++)
		{
			if (m_OptIV[j].date>m_OptIV[j+1].date)
			{
				temp=m_OptIV[j];
				m_OptIV[j]=m_OptIV[j+1];
				m_OptIV[j+1]=temp;
			}
		}
	}
	
}

int CQSNative_Option::FindIV(long date)
{
	if (!IsDateValid(date))
	{
		return -1;
	}
	
	for (int i=0;i<m_OptIV.GetSize();i++)
	{
		if (date == m_OptIV[i].date)
		{
			return i;
		}
	}
	
	return 0;
}

int CQSNative_Option::IsIndex(char* str)
{
	//CString strCode = m_TTOptions->m_szItemCode;
	//strCode = strCode.Replace(" ","");
	if( memcmp(str,"HSI    ",7)==0 
		/*|| memcmp(str,"HSCEI  ",7)==0*/ 
		|| memcmp(str,"MHI    ",7)==0 
		/*|| memcmp(str,"MCH    ",7)==0*/ )
		return 1;
	
	return 0;
}

int CQSNative_Option::IsIndex2(char* str)
{
	//CString strCode = m_TTOptions->m_szItemCode;
	//strCode = strCode.Replace(" ","");
	if( /*memcmp(str,"HSI    ",7)==0 */
		memcmp(str,"HSCEI  ",7)==0 
		/*|| memcmp(str,"MHI    ",7)==0 */
		|| memcmp(str,"MCH    ",7)==0 )
		return 1;
	
	return 0;
}

BOOL CQSNative_Option::GetIV(char* szItem, DayIVs& temp)
{
	if (strlen(szItem)<=0)
	{
		return FALSE;
	}
	/*
	WARRANTOPT opt;
	BSMCALINFO calinfo ;
	double vBF,tCall,tPut;
	double iCall,iPut;
	char sItem[10]={0};
	if (m_fRiskInterest==0)
	{
		ReadExpiryDateListFile();
	}

	memcpy(sItem,szItem,8);
	if (IsIndex2(szItem))
	{
		memcpy(sItem,"HSCEI    ",8);
	}
	if (IsIndex(szItem))
	{
		memcpy(sItem,"HSI     ",8);
	}

	memset(&temp,0,sizeof(DayIVs));
	int locDate=0;
	
	for (int i=0;i<m_nItemNum;i++)
	{
		if (memicmp(sItem,m_optFm[i].strICode,8)!=0)
			continue;
		
		temp.group=m_optFm[i].cGCode;
		memcpy(temp.item,m_optFm[i].strICode,8);

		GetItemPrice(temp);
		if (temp.close>0)
		{	
			int count=0;
			for (int j=0;j<m_optFm[i].ucMonthNum;j++)
			{
				vBF=0;
				temp.month[j]=m_optFm[i].m_optMonth[j].lTransDate;
				if (IsIndex(temp.item))
				{
					OptIV desk;
					memset(&desk,0,sizeof(OptIV));
					desk.m_items[i].group='A';
					memcpy(desk.m_items[i].item,"VHSI    ",8);
					GetItemPrice(desk.m_items[i]);

					if(desk.m_items[i].close>0)
					{
						temp.iv[j]=desk.m_items[i].close;
						continue;
					}
				}

				int rowNum = m_optFm[i].m_optMonth[j].ucTotalRow;
				int num=GetCloseNum(m_optFm[i].m_optMonth+j, temp.close);
				int is=num-4;
				int ie=num+4;
				if (is<0)
					is=0;
				if (ie>rowNum)
					ie=rowNum;
				iCall = 0;
				iPut = 0;
				tCall = 0.0;
				tPut = 0.0;
				float iIV=0.0;
				
				long   transdate=GetExpiryTime(temp.group,temp.item,temp.month[j]);
				if(j==0)
					locDate=transdate;
				double Expiryday=GetExpiryDays(transdate)/365.0;

				for (int m=num;m<=num;m++)
				{
					COptLine* line=m_optFm[i].m_optMonth[j].m_optLine+m;
					if (line->m_dat[0].fLast>0)
					{
						memset(&calinfo,0,sizeof(BSMCALINFO));
						opt.bCallOption=TRUE;
						opt.warrantPrice=line->m_dat[0].fLast;
						opt.conversionRatio=1.0;
						opt.currentAssetPrice=temp.close;
						opt.R=0.0;
						opt.exercisePrice=line->m_fStrike;
						opt.T_t=Expiryday;
						opt.riskFreeInterestRate=m_fRiskInterest;
						opt.sd=0.83;
						CBM1976Wrapper::CalIV_BySection( opt, calinfo ) ;
						
						if (calinfo.impliedVolitility!=0)
						{
							iIV   += calinfo.impliedVolitility;
							count++;
						}	
					}

					if (line->m_dat[1].fLast>0)
					{
						memset(&calinfo,0,sizeof(BSMCALINFO));
						opt.bCallOption=FALSE;
						opt.warrantPrice=line->m_dat[1].fLast;
						opt.conversionRatio=1.0;
						opt.currentAssetPrice=temp.close;
						opt.R=0.0;
						opt.exercisePrice=line->m_fStrike;
						opt.T_t=Expiryday;
						opt.riskFreeInterestRate=m_fRiskInterest;
						opt.sd=0.83;
						CBM1976Wrapper::CalIV_BySection( opt, calinfo ) ;
						
						if (calinfo.impliedVolitility!=0)
						{
							iIV  += calinfo.impliedVolitility;
							count++;
						}	
					}
				}

				temp.iv[j] = iIV;
				
				if ( (locDate - m_nTransDate<=3) && (locDate - m_nTransDate>=0) )
				{
					if(j==2)
					{
						temp.iv[0]=temp.iv[1];
						temp.iv[1]=temp.iv[2];
						break;
					}
				}
				else if(j>=1)
					break;
			}
			if (IsIndex(temp.item))
				break;
			if (count>0)
			{
				temp.iv[0]=(temp.iv[0]+temp.iv[1])/count;
			}
			else
				temp.iv[0]=0;
		}
	}
	*/

	return TRUE;
}

BOOL CQSNative_Option::CalcIV2()
{
	m_OptIV.RemoveAll();	//不用保存所有，housekeeping会保存到data目录
	//memset(&m_OptIV,0,sizeof(OptIV));
	OptIV  temp;
	/*
	WARRANTOPT opt;
	BSMCALINFO calinfo ;
	double vBF,tCall,tPut;
	double iCall,iPut;
	int locDate=0;
	/*
	int num = FindIV(m_nTransDate);
	if (num >= 0)
	{
		m_OptIV.RemoveAt(num);
	}
	* /
	ReadExpiryDateListFile();
	memset(&temp,0,sizeof(OptIV));
	temp.date=m_nTransDate;

	for (int i=0;i<m_nItemNum;i++)
	{
		
		temp.m_items[i].group=m_optFm[i].cGCode;
		memcpy(temp.m_items[i].item,m_optFm[i].strICode,8);
		GetItemPrice(temp.m_items[i]);

		CString msgLog;
		msgLog.Format("%.8s CLOSE = %f ucMonthNum = %d", m_optFm[i].strICode, temp.m_items[i].close, m_optFm[i].ucMonthNum);
		WriteLogFile((LPSTR)(LPCSTR)msgLog);

		if (strstr(temp.m_items[i].item,"2827")!=NULL)
		{
			int nnnnn=0;
			//temp.m_items[i].close=40.250;
		}
		if (temp.m_items[i].close>0)
		{	
			int nn=0;
			for (int j=0;j<m_optFm[i].ucMonthNum;j++)
			{
				vBF=0;
				temp.m_items[i].month[j]=m_optFm[i].m_optMonth[j].lTransDate;
				if (IsIndex(temp.m_items[i].item))
				{
					OptIV desk;
					memset(&desk,0,sizeof(OptIV));
					desk.m_items[i].group='A';
					memcpy(desk.m_items[i].item,"VHSI    ",8);
					GetItemPrice(desk.m_items[i]);

					if(desk.m_items[i].close>0)
					{
						temp.m_items[i].iv[j]=desk.m_items[i].close;
						break;
					}
				}

				int rowNum = m_optFm[i].m_optMonth[j].ucTotalRow;
				int num=GetCloseNum(m_optFm[i].m_optMonth+j, temp.m_items[i].close);
				int is=num-4;
				int ie=num+4;
				if (is<0)
					is=0;
				if (ie>rowNum)
					ie=rowNum;
				iCall = 0;
				iPut = 0;
				tCall = 0.0;
				tPut = 0.0;
				float iIV=0;

				long   transdate=GetExpiryTime(temp.m_items[i].group,temp.m_items[i].item,temp.m_items[i].month[j]);
				double Expiryday=GetExpiryDays(transdate)/365.0;

				if (j==0)
				{
					locDate=transdate;
				}
				
				for (int m=num;m<=num;m++)
				{
					COptLine* line=m_optFm[i].m_optMonth[j].m_optLine+m;
					msgLog.Format("%.8s last0[%d] = %f", m_optFm[i].strICode, m, line->m_dat[0].fLast);
					WriteLogFile((LPSTR)(LPCSTR)msgLog);
					if (line->m_dat[0].fLast>0)
					{
						memset(&calinfo,0,sizeof(BSMCALINFO));
						opt.bCallOption=TRUE;
						opt.warrantPrice=line->m_dat[0].fLast;
						opt.conversionRatio=1.0;
						opt.currentAssetPrice=temp.m_items[i].close;
						opt.R=0.0;
						opt.exercisePrice=line->m_fStrike;
						opt.T_t=Expiryday;
						opt.riskFreeInterestRate=m_fRiskInterest;
						opt.sd=0.83;
						CBM1976Wrapper::CalIV_BySection( opt, calinfo ) ;
						
						if (calinfo.impliedVolitility>0 && calinfo.impliedVolitility<1)
						{
							iCall += line->m_dat[0].fOI;
							tCall += calinfo.impliedVolitility*line->m_dat[0].fOI;
							iIV	  += calinfo.impliedVolitility;
							if ( (locDate - m_nTransDate<=3) && (locDate - m_nTransDate>=0) )
							{
								if(j==0)
									;
								else
									nn++;
							}
							else
								nn++;
						}
						
						CString logstr;
						logstr.Format("%.8s Call %d  strike=%f,OI=%f,im=%f,tcall=%f,lastprice=%f,close=%f,Risk=%f,expirydat=%d", m_optFm[i].strICode,
							m,line->m_fStrike,line->m_dat[0].fOI,calinfo.impliedVolitility,tCall,line->m_dat[0].fLast,temp.m_items[i].close,m_fRiskInterest,transdate);
						WriteLogFile((LPSTR)(LPCSTR)logstr);
						
					}
					
					msgLog.Format("%.8s last1[%d] = %f", m_optFm[i].strICode, m, line->m_dat[1].fLast);
					WriteLogFile((LPSTR)(LPCSTR)msgLog);
					if (line->m_dat[1].fLast>0)
					{
						memset(&calinfo,0,sizeof(BSMCALINFO));
						opt.bCallOption=FALSE;
						opt.warrantPrice=line->m_dat[1].fLast;
						opt.conversionRatio=1.0;
						opt.currentAssetPrice=temp.m_items[i].close;
						opt.R=0.0;
						opt.exercisePrice=line->m_fStrike;
						opt.T_t=Expiryday;
						opt.riskFreeInterestRate=m_fRiskInterest;
						opt.sd=0.83;
						CBM1976Wrapper::CalIV_BySection( opt, calinfo ) ;
						
						if (calinfo.impliedVolitility>0 && calinfo.impliedVolitility<1)
						{
							iPut += line->m_dat[1].fOI;
							tPut += calinfo.impliedVolitility*line->m_dat[1].fOI;
							iIV	 += calinfo.impliedVolitility;
							//nn++;
							if ( (locDate - m_nTransDate<=3) && (locDate - m_nTransDate>=0) )
							{
								if(j==0)
									;
								else
									nn++;
							}
							else
								nn++;
						}
						
						CString logstr;
						logstr.Format("%.8s Put %d  strike=%f,OI=%f,im=%f,tput=%f,lastprice=%f", m_optFm[i].strICode,
							m,line->m_fStrike,line->m_dat[1].fOI,calinfo.impliedVolitility,tPut,line->m_dat[1].fLast);
						WriteLogFile((LPSTR)(LPCSTR)logstr);
						
					}

				}

				temp.m_items[i].iv[j] = iIV;

				if ( (locDate - m_nTransDate<=3) && (locDate - m_nTransDate>=0) )
				{
					if(j==2)
					{
						temp.m_items[i].iv[0]=temp.m_items[i].iv[1];
						temp.m_items[i].iv[1]=temp.m_items[i].iv[2];

						CString logstr;
						logstr.Format("%.8s change to next IV1=%f,IV2=%f", m_optFm[i].strICode,
							temp.m_items[i].iv[0],temp.m_items[i].iv[1]);
						WriteLogFile((LPSTR)(LPCSTR)logstr);

						break;
					}
				}
				else 
				{
					if(j>=1)
						break;
				}
			}
			if (IsIndex(temp.m_items[i].item))
				;
			else
			{
				if(nn>0)
				{
					if(nn>=4)
					{
						nn=4;
					}
					temp.m_items[i].iv[0]=(temp.m_items[i].iv[0]+temp.m_items[i].iv[1])/nn;
				}
				else
					temp.m_items[i].iv[0]=0;
			}

			CString logstr;
			logstr.Format("%.8s iv=%f,n=%d", m_optFm[i].strICode,temp.m_items[i].iv[0],nn);
			WriteLogFile((LPSTR)(LPCSTR)logstr);
			nn=0;

			for (j=1;j<m_optFm[i].ucMonthNum;j++)
			{
				vBF=0;
				temp.m_items[i].month[j]=m_optFm[i].m_optMonth[j].lTransDate;
				if (IsIndex(temp.m_items[i].item))
				{
					OptIV desk;
					memset(&desk,0,sizeof(OptIV));
					desk.m_items[i].group='A';
					memcpy(desk.m_items[i].item,"VHSI    ",8);
					GetItemPrice(desk.m_items[i]);

					if(desk.m_items[i].close>0)
					{
						temp.m_items[i].iv[j]=desk.m_items[i].close;
						continue;
					}
				}

				int rowNum = m_optFm[i].m_optMonth[j].ucTotalRow;
				int num=GetCloseNum(m_optFm[i].m_optMonth+j, temp.m_items[i].close);
				int is=num-4;
				int ie=num+4;
				if (is<0)
					is=0;
				if (ie>rowNum)
					ie=rowNum;
				iCall = 0;
				iPut = 0;
				tCall = 0.0;
				tPut = 0.0;
				long   transdate=GetExpiryTime(temp.m_items[i].group,temp.m_items[i].item,temp.m_items[i].month[j]);
				double Expiryday=GetExpiryDays(transdate)/365.0;

				for (int m=is;m<=ie;m++)
				{
					COptLine* line=m_optFm[i].m_optMonth[j].m_optLine+m;
					if (line->m_dat[0].fLast>0)
					{
						memset(&calinfo,0,sizeof(BSMCALINFO));
						opt.bCallOption=TRUE;
						opt.warrantPrice=line->m_dat[0].fLast;
						opt.conversionRatio=1.0;
						opt.currentAssetPrice=temp.m_items[i].close;
						opt.R=0.0;
						opt.exercisePrice=line->m_fStrike;
						opt.T_t=Expiryday;
						opt.riskFreeInterestRate=m_fRiskInterest;
						opt.sd=0.83;
						CBM1976Wrapper::CalIV_BySection( opt, calinfo ) ;
						
						if (calinfo.impliedVolitility>0 && calinfo.impliedVolitility<1)
						{
							iCall += line->m_dat[0].fOI;
							tCall += calinfo.impliedVolitility*line->m_dat[0].fOI;
						}
						
						CString logstr;
						logstr.Format("%.8s Call %d  strike=%f,OI=%f,im=%f,tcall=%f", m_optFm[i].strICode,
							m,line->m_fStrike,line->m_dat[0].fOI,calinfo.impliedVolitility,tCall);
						//WriteLogFile((LPSTR)(LPCSTR)logstr);
						
					}

					if (line->m_dat[1].fLast>0)
					{
						memset(&calinfo,0,sizeof(BSMCALINFO));
						opt.bCallOption=FALSE;
						opt.warrantPrice=line->m_dat[1].fLast;
						opt.conversionRatio=1.0;
						opt.currentAssetPrice=temp.m_items[i].close;
						opt.R=0.0;
						opt.exercisePrice=line->m_fStrike;
						opt.T_t=Expiryday;
						opt.riskFreeInterestRate=m_fRiskInterest;
						opt.sd=0.83;
						CBM1976Wrapper::CalIV_BySection( opt, calinfo ) ;
						
						if (calinfo.impliedVolitility>0 && calinfo.impliedVolitility<1)
						{
							iPut += line->m_dat[1].fOI;
							tPut += calinfo.impliedVolitility*line->m_dat[1].fOI;
						}
						
						CString logstr;
						logstr.Format("%.8s Put %d  strike=%f,OI=%f,im=%f,tput=%f", m_optFm[i].strICode,
							m,line->m_fStrike,line->m_dat[1].fOI,calinfo.impliedVolitility,tPut);
						//WriteLogFile((LPSTR)(LPCSTR)logstr);
						
					}

				}
				if (iCall==0 && iPut==0)
				{
					vBF=0;
				}
				else if (iPut==0)
				{
					vBF=tCall/iCall;
				}
				else if (iCall==0)
				{
					vBF=tPut/iPut;
				}
				else
					vBF = (tCall/iCall+tPut/iPut)/2.0;

				temp.m_items[i].iv[j]=vBF;

				CString logstr;
				logstr.Format("%.8s IV=%f,month=%d",temp.m_items[i].item,vBF,temp.m_items[i].month[j]);
				//WriteLogFile((LPSTR)(LPCSTR)logstr);
			}
		}
		else
		{
			CString logstr;
			logstr.Format("%.8s close price is 0.Skip.",temp.m_items[i].item);
			WriteLogFile((LPSTR)(LPCSTR)logstr);
		}
	}
	m_OptIV.Add(temp);
	//memcpy(&m_OptIV,&temp,sizeof(OptIV));
	*/
	return TRUE;
}

BOOL CQSNative_Option::CalcIV()
{
	m_OptIV.RemoveAll();	//不用保存所有，housekeeping会保存到data目录
	OptIV  temp;
	/*
	WARRANTOPT opt;
	BSMCALINFO calinfo ;
	double vBF,tCall,tPut;
	double iCall,iPut;
	/*
	int num = FindIV(m_nTransDate);
	if (num >= 0)
	{
		m_OptIV.RemoveAt(num);
	}
	* /
	ReadExpiryDateListFile();
	//memset(&m_OptIV,0,sizeof(OptIV));
	memset(&temp,0,sizeof(OptIV));
	temp.date=m_nTransDate;
	for (int i=0;i<m_nItemNum;i++)
	{
		
		temp.m_items[i].group=m_optFm[i].cGCode;
		memcpy(temp.m_items[i].item,m_optFm[i].strICode,8);

		GetItemPrice(temp.m_items[i]);
		if (temp.m_items[i].close>0)
		{	
			for (int j=0;j<m_optFm[i].ucMonthNum;j++)
			{
				vBF=0;
				temp.m_items[i].month[j]=m_optFm[i].m_optMonth[j].lTransDate;
				if (IsIndex(temp.m_items[i].item))
				{
					OptIV desk;
					memset(&desk,0,sizeof(OptIV));
					desk.m_items[i].group='A';
					memcpy(desk.m_items[i].item,"VHSI    ",8);
					GetItemPrice(desk.m_items[i]);

					if(desk.m_items[i].close>0)
					{
						temp.m_items[i].iv[j]=desk.m_items[i].close;
						continue;
					}
				}


				int rowNum = m_optFm[i].m_optMonth[j].ucTotalRow;
				int num=GetCloseNum(m_optFm[i].m_optMonth+j, temp.m_items[i].close);
				int is=num-4;
				int ie=num+4;
				if (is<0)
					is=0;
				if (ie>rowNum)
					ie=rowNum;
				iCall = 0;
				iPut = 0;
				tCall = 0.0;
				tPut = 0.0;
				long   transdate=GetExpiryTime(temp.m_items[i].group,temp.m_items[i].item,temp.m_items[i].month[j]);
				double Expiryday=GetExpiryDays(transdate)/365.0;

				for (int m=is;m<=ie;m++)
				{
					COptLine* line=m_optFm[i].m_optMonth[j].m_optLine+m;
					if (line->m_dat[0].fLast>0)
					{
						memset(&calinfo,0,sizeof(BSMCALINFO));
						opt.bCallOption=TRUE;
						opt.warrantPrice=line->m_dat[0].fLast;
						opt.conversionRatio=1.0;
						opt.currentAssetPrice=temp.m_items[i].close;
						opt.R=0.0;
						opt.exercisePrice=line->m_fStrike;
						opt.T_t=Expiryday;
						opt.riskFreeInterestRate=m_fRiskInterest;
						opt.sd=0.83;
						CBM1976Wrapper::CalIV_BySection( opt, calinfo ) ;
						
						if (calinfo.impliedVolitility!=0)
						{
							iCall += line->m_dat[0].fOI;
							tCall += calinfo.impliedVolitility*line->m_dat[0].fOI;
						}
						
						CString logstr;
						logstr.Format("%.8s Call %d  strike=%f,OI=%f,im=%f,tcall=%f", m_optFm[i].strICode,
							m,line->m_fStrike,line->m_dat[0].fOI,calinfo.impliedVolitility,tCall);
						WriteLogFile((LPSTR)(LPCSTR)logstr);
						
					}

					if (line->m_dat[1].fLast>0)
					{
						memset(&calinfo,0,sizeof(BSMCALINFO));
						opt.bCallOption=FALSE;
						opt.warrantPrice=line->m_dat[1].fLast;
						opt.conversionRatio=1.0;
						opt.currentAssetPrice=temp.m_items[i].close;
						opt.R=0.0;
						opt.exercisePrice=line->m_fStrike;
						opt.T_t=Expiryday;
						opt.riskFreeInterestRate=m_fRiskInterest;
						opt.sd=0.83;
						CBM1976Wrapper::CalIV_BySection( opt, calinfo ) ;
						
						if (calinfo.impliedVolitility!=0)
						{
							iPut += line->m_dat[1].fOI;
							tPut += calinfo.impliedVolitility*line->m_dat[1].fOI;
						}
						
						CString logstr;
						logstr.Format("%.8s Put %d  strike=%f,OI=%f,im=%f,tput=%f", m_optFm[i].strICode,
							m,line->m_fStrike,line->m_dat[1].fOI,calinfo.impliedVolitility,tPut);
						WriteLogFile((LPSTR)(LPCSTR)logstr);
						
					}

				}
				if (iCall==0 && iPut==0)
				{
					vBF=0;
				}
				else if (iPut==0)
				{
					vBF=tCall/iCall;
				}
				else if (iCall==0)
				{
					vBF=tPut/iPut;
				}
				else
					vBF = (tCall/iCall+tPut/iPut)/2.0;

				temp.m_items[i].iv[j]=vBF;

				if ( (transdate - m_nTransDate<=3) && (transdate - m_nTransDate>=0) && j>=2)
				{
					temp.m_items[i].iv[0]=temp.m_items[i].iv[1];
				}
				CString logstr;
				logstr.Format("%.8s IV=%f,month=%d",temp.m_items[i].item,vBF,temp.m_items[i].month[j]);
				WriteLogFile((LPSTR)(LPCSTR)logstr);
			}
		}
	}
	m_OptIV.Add(temp);
	//memcpy(&m_OptIV,&temp,sizeof(OptIV));
	*/
	return TRUE;
}

BYTE CQSNative_Option::ReadExpiryDateListFile()
{
	if (strlen(m_szExpPath)<=0)
	{
		CString logstr;
		logstr.Format("Open options.exp error.");
		WriteLogFile((LPSTR)(LPCSTR)logstr);
		return 1;
	}
	m_fRiskInterest  = 0.0446;
	
	m_ExpiryDateMap.RemoveAll();
	m_ItemSharesMap.RemoveAll();
	
	CString strItem;
	OptionsExpiryDateInfo expInfo;
	OptionsItemShares     itemshares;
	
	HANDLE hFile = CreateFile(m_szExpPath,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		CString logstr;
		logstr.Format("Open %s error.",m_szExpPath);
		WriteLogFile((LPSTR)(LPCSTR)logstr);
		return 1;
	}
	OptionsExpiryDateFileHead txthead;
	OptionsExpiryDateItem     txt;
	OptionsExpiryDate  expDate;
	DWORD dwTm;
	
	if(ReadFile(hFile,&txthead,sizeof(OptionsExpiryDateFileHead),&dwTm,NULL) !=0)		
	{
		if (memcmp(txthead.m_szFileID,DE_FILEID,strlen(DE_FILEID)) != 0)
		{
			CloseHandle(hFile);
			return 1;
		}
		
		m_fRiskInterest = txthead.m_fInterestRate;
		
		for(int i = 0; i< txthead.m_nItemCount; i++)
		{
			expInfo.expList.RemoveAll();
			
			if(!ReadFile(hFile,&txt,sizeof(OptionsExpiryDateItem),&dwTm,NULL))
			{
				CloseHandle(hFile);
				return 1;
			}	
			
			char szItem[10];
			memset(szItem,0,sizeof(szItem));
			szItem[0] = txt.cGroup;
			memcpy(&szItem[1],txt.szItemCode,8);
			strItem = szItem;
			
			itemshares.Shares = txt.m_lShares;
			m_ItemSharesMap.SetAt((LPCSTR)strItem, itemshares);
			
			for (int j = 0; j < txt.nMonthCount; j++)
			{
				if(!ReadFile(hFile,&expDate,sizeof(OptionsExpiryDate),&dwTm,NULL))
				{				
					CloseHandle(hFile);
					return 1;
				}	
				
				expInfo.expList.Add(expDate);
			}	
			m_ExpiryDateMap.SetAt((LPCSTR)strItem,expInfo); 
			
		}//end of for				
	}//end of if	
	
	CloseHandle(hFile);
	return 0;
	
}

int CQSNative_Option::GetExpiryTime(char group, char* itemcode, int month)
{
	int m_lExpiryTime = 0;
	
	CString strItem;
	strItem.Format("%c%.8s",group,itemcode);
	
	BOOL bFind = FALSE;
	OptionsExpiryDateInfo expInfo;
	if(m_ExpiryDateMap.Lookup((LPCTSTR)strItem,expInfo)== TRUE)
	{
		for(int i=0;i<expInfo.expList.GetSize();i++)
		{
			if (expInfo.expList.ElementAt(i).lMonth == month)
			{
				m_lExpiryTime = expInfo.expList.ElementAt(i).lExpiryDate;
				bFind = TRUE;
				break;
			}	
		}	
	}
	
	if(bFind == FALSE)  //取Default值
	{
		strItem = "ADEFAULT ";
		if(m_ExpiryDateMap.Lookup((LPCTSTR)strItem,expInfo)== TRUE)
		{
			for(int i=0;i<expInfo.expList.GetSize();i++)
			{
				if (expInfo.expList.ElementAt(i).lMonth == month)
				{
					m_lExpiryTime = expInfo.expList.ElementAt(i).lExpiryDate;
					bFind = TRUE;
					break;
				}
			}
		}
	}

	
	if (m_lExpiryTime==0)
	{
		char logstr[100];
		sprintf(logstr,"GetExpiryDate error.'%s' ",strItem);
		WriteLogFile(logstr);
	}

	return m_lExpiryTime;
}

int CQSNative_Option::FindItemInOption(char group, char* itemcode, OptionIV& iv)
{
	try
	{
	if (m_OptIV.GetSize()<=0)
	{
		return -1;
	}
	for (int i=0;i<m_nItemNum;i++)
	{
		if ( memicmp(itemcode,m_OptIV[0].m_items[i].item,8)==0 )
		{
			iv.date =m_OptIV[0].date;
			iv.open =m_OptIV[0].m_items[i].open;
			iv.high =m_OptIV[0].m_items[i].high;
			iv.low  =m_OptIV[0].m_items[i].low;
			iv.close=m_OptIV[0].m_items[i].close;
			
			iv.iv   =m_OptIV[0].m_items[i].iv[0];
			iv.month=m_OptIV[0].m_items[i].month[0];

			float v=0;
			int count=0;
			for (int n=1;n<TT_OPT_MAXMONTH;n++)
			{
				if (count>2)
					break;
				
				if ( m_OptIV[0].m_items[i].month[n]>0 &&
				   ( m_OptIV[0].m_items[i].month[n]%100)%3==0 && 
					 m_OptIV[0].m_items[i].iv[n]>0 )
				{
					v+=m_OptIV[0].m_items[i].iv[n];
					count++;
				}
			}
			v=(v+iv.iv)/(count+1);
			iv.iv2=v;

			return i;
		}
	}
	return -1;
	}
	catch(...)
	{
		char logstr[255];
		sprintf(logstr,"Get OptIV error.%.8s",itemcode);
		WriteLogFile(logstr);
		return -1;
	}
}

void CQSNative_Option::GetPreOI(long day)
{
	if ( day == m_nTransDate || m_nTransDate == 0 )
		return;

	m_lastDate=m_nTransDate;
	m_nItemNum2=m_nItemNum;
	for (int i=0;i<m_nItemNum2;i++)
	{
		if (m_optFm[i].ucMonthNum<=0)
			continue;
		#if 0
		memcpy(m_PreOI[i].strICode,m_optFm[i].strICode,8);
		m_PreOI[i].cGCode=m_optFm[i].cGCode;
		m_PreOI[i].ucMonthNum=m_optFm[i].ucMonthNum;

		for (int j=0;j<m_optFm[i].ucMonthNum;j++)
		{
			if(m_optFm[i].m_optMonth[j].lTransDate<=0)
				continue;

			m_PreOI[i].m_optOIMonth[j].lTransDate = m_optFm[i].m_optMonth[j].lTransDate;
			m_PreOI[i].m_optOIMonth[j].cDecimal   = m_optFm[i].m_optMonth[j].cDecimal;
			m_PreOI[i].m_optOIMonth[j].ucTotalRow = m_optFm[i].m_optMonth[j].ucTotalRow;

			for (int m=0;m<m_optFm[i].m_optMonth[j].ucTotalRow;m++)
			{
				m_PreOI[i].m_optOIMonth[j].m_optOILine[m].m_fStrike = m_optFm[i].m_optMonth[j].m_optLine[m].m_fStrike;
				m_PreOI[i].m_optOIMonth[j].m_optOILine[m].CallOI = m_optFm[i].m_optMonth[j].m_optLine[m].m_dat[0].fOI;
				m_PreOI[i].m_optOIMonth[j].m_optOILine[m].PutOI  = m_optFm[i].m_optMonth[j].m_optLine[m].m_dat[1].fOI;
			}
		}
		#endif
		
	}
}

BOOL CQSNative_Option::SavePreviousOI()
{
	char strFileName[MAX_PATH];
	strcpy(strFileName, "OptPreOI.dat");
	HANDLE hFile;


	hFile = CreateFile(strFileName, GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD dwLen;
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);


		BOOL bSec = WriteFile(hFile, &m_nItemNum2, sizeof(m_nItemNum2), &dwLen, NULL);
		if(!bSec || dwLen != sizeof(m_nItemNum2))
		{
			CloseHandle(hFile);
			return FALSE;
		}
		bSec = WriteFile(hFile, &m_lastDate, sizeof(m_lastDate), &dwLen, NULL);
		if(!bSec || dwLen != sizeof(m_lastDate))
		{
			CloseHandle(hFile);
			return FALSE;
		}
		#if 0
		bSec = WriteFile(hFile, m_PreOI, sizeof(m_PreOI[0]) * m_nItemNum2, &dwLen, NULL);
		if(!bSec || dwLen != sizeof(m_PreOI[0]) * m_nItemNum2)
		{
			CloseHandle(hFile);
			return FALSE;
		}

		for (int i=0;i<m_nItemNum2;i++)
		{
			if (memicmp(m_PreOI[i].strICode,"HSI    ",7)==0)
			{
				for (int j=0,m=0;j<TT_OPT_MAXROW && m<20;j++,m++)
				{
					char logstr[255];
					sprintf(logstr,"strike=%f calloi=%f putoi=%f",m_PreOI[i].m_optOIMonth[0].m_optOILine[j].m_fStrike,
						m_PreOI[i].m_optOIMonth[0].m_optOILine[j].CallOI,
						m_PreOI[i].m_optOIMonth[0].m_optOILine[j].PutOI);
					WriteLogFile(logstr);
				}
			}
		}
		#endif
	
	CloseHandle(hFile);

	return TRUE;
}

BOOL CQSNative_Option::LoadPreviousOI()
{
	char strFileName[MAX_PATH];
	strcpy(strFileName, "OptPreOI.dat");
	HANDLE hFile;
	
	
	hFile = CreateFile(strFileName, GENERIC_READ|GENERIC_WRITE, 
			FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
	
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	
	DWORD dwLen;
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	
	BOOL bSec = ReadFile(hFile, &m_nItemNum2, sizeof(m_nItemNum2), &dwLen, NULL);
		if(!bSec || dwLen != sizeof(m_nItemNum2))
		{
			//m_nItemNum = 0;
			CloseHandle(hFile);
			return FALSE;
		}
		bSec = ReadFile(hFile, &m_lastDate, sizeof(m_lastDate), &dwLen, NULL);
		if(!bSec || dwLen != sizeof(m_lastDate))
		{
			//m_nTransDate = 0;
			CloseHandle(hFile);
			return FALSE;
		}
		/*
		bSec = ReadFile(hFile, m_PreOI, sizeof(m_PreOI[0]) * m_nItemNum2, &dwLen, NULL);
		if(!bSec || dwLen != sizeof(m_PreOI[0]) * m_nItemNum2)
		{
			for(int i = 0; i < m_nItemNum2; i ++)
			{
				m_PreOI[i].Init();
			}
			CloseHandle(hFile);
			return FALSE;
		}
		*/
		/*
		for (int i=0;i<m_nItemNum;i++)
		{
			if (memicmp(m_PreOI[i].strICode,"00700   ",8)==0)
			{
				for (int j=0;j<TT_OPT_MAXROW;j++)
				{
					char logstr[255];
					sprintf(logstr,"strike=%f calloi=%f putoi=%f",m_PreOI[i].m_optOIMonth[0].m_optOILine[j].m_fStrike,
						m_PreOI[i].m_optOIMonth[0].m_optOILine[j].CallOI,
						m_PreOI[i].m_optOIMonth[0].m_optOILine[j].PutOI);
					WriteLogFile(logstr);
				}
			}
		}
		*/
	CloseHandle(hFile);
	
	return TRUE;
}

BYTE CQSNative_Option::SendPreOIRequest(unsigned char cbGroupCode,LPCTSTR pszItemCode, long lTransdate, DOUBLE ReqId, DOUBLE UserId)
{
	int i;
	int nIndex,nMonthIdx;
	if((nIndex = EnumOptionItemCodeEx2(pszItemCode, cbGroupCode)) == -1)
		return FALSE;
	if((nMonthIdx = FindMonthEx2(nIndex, lTransdate)) == -1)
		return FALSE;	

	BYTE RqType;
	
	STProtocol_SR_RET StPrt;

	StPrt.headMark = 'AA55';
	StPrt.BegFlag = 'WS';
	StPrt.DFFlag = 'DF';
	StPrt.DataOff = 18;
	StPrt.UIFlag = 'UI';	
	StPrt.UserID = UserId;	
	StPrt.DAFlag = 'DA';	
	
	FrameLen	framelen;
	FrameHead	framehead;
	FrameID		frameid;
	
	framehead.wFrameType = 'DO';
	frameid.cbGroupCode = cbGroupCode;
	memcpy(frameid.szItemCode, pszItemCode, G_GENCODE__LEN);
	
	frameid.fStrikePrice = 0;
	if (lTransdate==0)
	{
		//frameid.lTransdate = m_PreOI[nIndex].m_optOIMonth[nMonthIdx].lTransDate;
	}
	else
		frameid.lTransdate = lTransdate;
	frameid.cbOrder = 0;
	
	long offset = sizeof(FrameLen); 
	
	//memcpy(m_pOutBuff+sizeof(StPrt),&framelen, sizeof(FrameLen));
	//offset+=sizeof(FrameLen);
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&framehead, sizeof(FrameHead));
	offset+=sizeof(FrameHead);
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&frameid, sizeof(FrameID));
	offset+=sizeof(FrameID);
	WORD elementtype = 'PO';
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&elementtype, sizeof(ElementType));
	offset+=sizeof(elementtype);

	/*
	int nRowNum = m_PreOI[nIndex].m_optOIMonth[nMonthIdx].ucTotalRow; 
	BYTE actualRowNum = 0;
	long off = offset;
	//memcpy(m_pOutBuff+sizeof(StPrt)+offset,&actualRowNum, sizeof(actualRowNum));
	offset+=sizeof(actualRowNum);
	long BuffLen;
	
	for(i = 0; i < nRowNum; i++)
	{
		memcpy(m_pOutBuff+sizeof(StPrt)+offset,&m_PreOI[nIndex].m_optOIMonth[nMonthIdx].m_optOILine[i].m_fStrike, sizeof(float));
		offset+=sizeof(float);
		memcpy(m_pOutBuff+sizeof(StPrt)+offset,&m_PreOI[nIndex].m_optOIMonth[nMonthIdx].m_optOILine[i].CallOI, sizeof(float));
		offset+=sizeof(float);
		memcpy(m_pOutBuff+sizeof(StPrt)+offset,&m_PreOI[nIndex].m_optOIMonth[nMonthIdx].m_optOILine[i].PutOI, sizeof(float));
		offset+=sizeof(float);
		//
		char logstr[255];
		sprintf(logstr,"SendPreOI  %.6s %d calloi=%f putoi=%f",pszItemCode,lTransdate,m_PreOI[nIndex].m_optOIMonth[nMonthIdx].m_optOILine[i].CallOI,
				m_PreOI[nIndex].m_optOIMonth[nMonthIdx].m_optOILine[i].PutOI);
		WriteLogFile(logstr);
		//  * /
	}
	
	framelen = sizeof(FrameHead) + sizeof(FrameID) + sizeof(ElementType) + sizeof(char)
			 + sizeof(float)*3*nRowNum;
	

	memcpy(m_pOutBuff+sizeof(StPrt),&framelen, sizeof(framelen));
	actualRowNum = nRowNum;
	memcpy(m_pOutBuff+sizeof(StPrt)+off,&actualRowNum, sizeof(actualRowNum));
	*/
	
	StPrt.Length = sizeof(StPrt) + offset - 10/*原来是12*/ + 2;
	StPrt.dataSize = sizeof(StPrt)+offset+2 - 8; //andy add 2007.08.31
	memcpy(m_pOutBuff,&StPrt,sizeof(StPrt));
	
	WORD wdVal = 'ED';
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&wdVal,sizeof(wdVal));
	
	SendOutFrame(m_pOutBuff,sizeof(StPrt)+offset+2);

	return 0;	
}

void CQSNative_Option::SendPreOIEvery3Min(unsigned char cbGroupCode,LPCTSTR pszItemCode,long lTransdate,DWORD channelID)
{
	int i;
	int nIndex,nMonthIdx;
	if((nIndex = EnumOptionItemCodeEx2(pszItemCode, cbGroupCode)) == -1)
		return ;
	if((nMonthIdx = FindMonthEx2(nIndex, lTransdate)) == -1)
		return ;

	STProtocol_SR_RET2 StPrt;
	//	StPrt.ReqIdNum =1;
	//	StPrt.ReqID = 0;
	StPrt.headMark = 'AA55';
	StPrt.BegFlag = 'WS';
	StPrt.DFFlag = 'DF';
	StPrt.DataOff = 14;
	StPrt.CIFlag = 'CI';	
	StPrt.ChannelID = channelID;	
	StPrt.DAFlag = 'DA';	
	
	FrameLen	framelen;
	FrameHead	framehead;
	FrameID		frameid;
	
	framehead.wFrameType = 'DO';
	frameid.cbGroupCode = cbGroupCode;
	memcpy(frameid.szItemCode, pszItemCode, G_GENCODE__LEN);
	
	frameid.fStrikePrice = 0;
	if (lTransdate==0)
	{
		//frameid.lTransdate = m_PreOI[nIndex].m_optOIMonth[nMonthIdx].lTransDate;
	}
	else
		frameid.lTransdate = lTransdate;
	frameid.cbOrder = 0;
	
	long offset = sizeof(FrameLen); 
	
	//memcpy(m_pOutBuff+sizeof(StPrt),&framelen, sizeof(FrameLen));
	//offset+=sizeof(FrameLen);
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&framehead, sizeof(FrameHead));
	offset+=sizeof(FrameHead);
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&frameid, sizeof(FrameID));
	offset+=sizeof(FrameID);
	WORD elementtype = 'PO';
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&elementtype, sizeof(ElementType));
	offset+=sizeof(elementtype);
	
	BYTE actualRowNum = 0;
	long off = offset;
	//memcpy(m_pOutBuff+sizeof(StPrt)+offset,&actualRowNum, sizeof(actualRowNum));
	offset+=sizeof(actualRowNum);
	/*
	int nRowNum = m_PreOI[nIndex].m_optOIMonth[nMonthIdx].ucTotalRow; 
	for(i = 0; i < nRowNum; i++)
	{
		memcpy(m_pOutBuff+sizeof(StPrt)+offset,&m_PreOI[nIndex].m_optOIMonth[nMonthIdx].m_optOILine[i].m_fStrike, sizeof(float));
		offset+=sizeof(float);
		memcpy(m_pOutBuff+sizeof(StPrt)+offset,&m_PreOI[nIndex].m_optOIMonth[nMonthIdx].m_optOILine[i].CallOI, sizeof(float));
		offset+=sizeof(float);
		memcpy(m_pOutBuff+sizeof(StPrt)+offset,&m_PreOI[nIndex].m_optOIMonth[nMonthIdx].m_optOILine[i].PutOI, sizeof(float));
		offset+=sizeof(float);
		/*
		char logstr[255];
		sprintf(logstr,"SendPreOIEvery5mins  %.6s %d calloi=%f putoi=%f",pszItemCode,lTransdate,m_PreOI[nIndex].m_optOIMonth[nMonthIdx].m_optOILine[i].CallOI,
			m_PreOI[nIndex].m_optOIMonth[nMonthIdx].m_optOILine[i].PutOI);
		WriteLogFile(logstr);
		* /
	}
	
	framelen = sizeof(FrameHead) + sizeof(FrameID) + sizeof(ElementType) + sizeof(char)
			 + sizeof(float)*3*nRowNum;
	
	
	memcpy(m_pOutBuff+sizeof(StPrt),&framelen, sizeof(framelen));
	actualRowNum = nRowNum;
	memcpy(m_pOutBuff+sizeof(StPrt)+off,&actualRowNum, sizeof(actualRowNum));
	
	StPrt.Length = sizeof(StPrt) + offset - 10/*原来是12* / + 2;
	StPrt.dataSize = sizeof(StPrt)+offset+2 - 8; //andy add 2007.08.31
	memcpy(m_pOutBuff,&StPrt,sizeof(StPrt));
	
	WORD wdVal = 'ED';
	memcpy(m_pOutBuff+sizeof(StPrt)+offset,&wdVal,sizeof(wdVal));
	
	SendOutFrame(m_pOutBuff,sizeof(StPrt)+offset+2);
	*/
}