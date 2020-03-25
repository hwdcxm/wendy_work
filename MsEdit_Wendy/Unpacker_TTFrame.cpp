// Unpacker_TTFrame.cpp: implementation of the CUnpacker_TTFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit_Wendy.h"
#include "Unpacker_TTFrame.h"
#include "Master.h"
#include "ShowData.h"
#include "MsEdit_WendyDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//DWORD Count_BreakCList = 0;

extern Master g_Master;
extern CArrayItemDataFrame mCArrayItemDataFrame;
extern CListDataFrame mCListDataFrame;
extern CListFrame_e mCListFrame_e;

#include "log.h"
extern CString s_eLog;
extern CLog w_InofLog;

extern DWORD g_fileSizeMB;
extern CMsEdit_WendyDlg* gpMsEdit_WendyDlg;
DWORD BigFile_NoSelectGetTotalRecord = 0;

extern DWORD mBufferMaxMB;
extern int mCanBuffer;

DWORD ReadGetFrames = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUnpacker_TTFrame::CUnpacker_TTFrame()
{
	ClearStream() ;
}

CUnpacker_TTFrame::~CUnpacker_TTFrame()
{

}

BOOL CUnpacker_TTFrame::CheckIsXItem(char cGCode,char* szXItem)
{
	char szItem[10];
	memset(szItem,0,sizeof(szItem));
	szItem[0] = cGCode;
	memcpy(szItem+1,szXItem,8);

	int iCount = m_arrNightMarket.GetSize();
	
	for(int i=0;i<iCount;i++)
	{
		if(memcmp(szItem,m_arrNightMarket[i].szItemDest,9)==0)
		{			
			return TRUE;
		}
	}
	return FALSE;
}


BOOL CUnpacker_TTFrame::FormatItemCode(char* szItem)
{
	for(int i=0;i<9;i++)
	{
		if(szItem[i] == 0)
			szItem[i] = 32;
	}
	
	szItem[9] = 0;
	
	return TRUE;
}

BOOL CUnpacker_TTFrame::LoadXItemTable()
{
	char inifile[256];
	char szPath[256];
	::GetModuleFileName( NULL,szPath,MAX_PATH);
	for(int i = strlen(szPath);i > 0;i--)
	{
		if(szPath[i] == '\\')
		{
			szPath[i] = 0;
			break;
		}
	}
	
	strcpy(inifile,szPath);
	strcat(inifile,"\\nightT.ini");
	
	
	char strTemp[100];	
	char szItem[50];
	char szxItem[50];
	char szzItem[50];

	m_arrNightMarket.RemoveAll();
	int iCount=GetPrivateProfileInt("CodeTable","Count",0,inifile);
	//Johnny 2015.05.14
	m_TimeNightStart=GetPrivateProfileInt("TradeTime","startofnighttrade",0,inifile);
	m_TimeNightEnd=GetPrivateProfileInt("TradeTime","endofnighttrade",0,inifile);

	for(i=0; i<iCount;i++)
	{
		NightMarketItem itemkt;
		
		sprintf(szItem,"Item%d",i+1);
		sprintf(szxItem,"XItem%d",i+1);
		sprintf(szzItem,"ZItem%d",i+1);
		
		char strTemp[100];
		memset(strTemp,0,sizeof(strTemp));
		GetPrivateProfileString("CodeTable",szItem,0,strTemp,sizeof(strTemp),inifile);
		FormatItemCode(strTemp);
		strcpy(itemkt.szItemSrc,strTemp);
		
		memset(strTemp,0,sizeof(strTemp));
		GetPrivateProfileString("CodeTable",szxItem,0,strTemp,sizeof(strTemp),inifile);		
		FormatItemCode(strTemp);
		strcpy(itemkt.szItemDest,strTemp);
		
		// 		memset(strTemp,0,sizeof(strTemp));
		// 		GetPrivateProfileString("CodeTable",szzItem,0,strTemp,sizeof(strTemp),inifile);		
		// 		FormatItemCode(strTemp);
		// 		strcpy(itemkt.szItemZ,strTemp);
		
		m_arrNightMarket.Add(itemkt);
	}
	
	return TRUE;
}


BOOL CUnpacker_TTFrame::InputData( BYTE* pBuf, int bufSize ) 
{
	if( pBuf==NULL || bufSize<=0 )
		return FALSE ;


	//andy add 2013.10.31 
	LONG lfmTime;
	BOOL bxItem = FALSE;   //是否xItem
	BOOL bXFrame = FALSE;  //是否'X'frame
	BOOL bNight = FALSE;   //是否夜市交易时间
	BOOL bCheckFrame = FALSE; //夜市交易时间,xItem的high,low,vol这几个帧要丢掉,那是全天的数据,只取'X'里面封装的high,low,vol帧
    //andy 2013.10.31

	int nReadLength ;
	int procSize = 0 ;
	do
	{
		if( nTempBufAddr<=0 )
		{
			nReadLength = min( 100, bufSize-procSize ) ;
			if( nReadLength<=0 )
				break ;

			memcpy( tempbuf, pBuf+procSize, nReadLength ) ;
			procSize += nReadLength ;

			nTempBufAddr = 0 ;
		}

		lpbuf = tempbuf[nTempBufAddr++] ;	
		if( nTempBufAddr>=nReadLength )
			nTempBufAddr = 0 ;


		//解Frame
		if(bStxFound&&!bDleFound)
		{//若不是STX曾经被找到并且Esc不是前一字符
			if(lpbuf==STX&&!bDleFound)
			{//若找到STX前面的字符全部废弃
				nBuffIp=sizeof(_tagTTDataFrame);//设定缓存的指针
				bStxFound=true;
				bDleFound=false;
				buff[nBuffIp++]=lpbuf;
				continue;
			}
			else
			{
				continue;
			}
		}//若不是STX曾经被找到并且Esc不是前一字符
		if(lpbuf==ETX&&!bDleFound)
		{//找到一帧
			//ATLTRACE("Find a frame .\n");
			unsigned char * buf=buff+sizeof(_tagTTDataFrame)+1; //for crc16

			//放入buff
			buff[nBuffIp]=lpbuf;
			//-Frame len verify begin
			memcpy(&nsValue,buff+sizeof(_tagTTDataFrame)+2,sizeof(nsValue));
			if((unsigned)nBuffIp!=nsValue+sizeof(_tagTTDataFrame)-1)
			{		//frame Length is Wrong
				goto label2;
			};
			//-Frame len verify end
			//----Verify CRC16 BEGIN
			len=0;
			memcpy(&len,buf+1,2);
			if((unsigned)len>sizeof(buff))
			{		//frame Length is Wrong
				goto label2;
			};
			bt = 0;
			for(j=0;j<len-4;j++)
			{
				bt = (bt ^ (((int)*buf++) << 8));
				for (i = 0; i < 8; ++i)
					if (bt & 0x8000)
						bt = ((bt << 1) ^ 0x1021);
					else
						bt = bt << 1;
			}
			bt  =bt & 0xFFFF;
			memcpy(&nsValue,buff+nBuffIp-2,sizeof(nsValue));
			if(nsValue!=bt)
			{//crc error
				//Error frame handle
				goto label2;
			}
			//----Verify CRC16 END
			//--------------------------------------------------------------------------------
			//以下为对frame的处理
			//清空Frame头部
			memset(pStFrame,0,sizeof(_tagTTDataFrame));
			//frame总长
			pStFrame->nLength=nBuffIp+sizeof(_tagTTDataFrame);
			//position 5 6 is time(unsigned short)
			memcpy(&usValue,buff+sizeof(_tagTTDataFrame)+4,2);
			//---以下解压时间

			bxItem = FALSE; 
			bXFrame = FALSE;
			bNight = FALSE; 

			bCheckFrame = FALSE; 
			
			memcpy((BYTE *)&pStFrame->pTransData+2,&usValue,sizeof(usValue));
			if ((stTm.tm_hour = usValue / 1800) >= 24)
			{	
				stTm.tm_hour-=24;
			}
			else
			{
				stTm.tm_isdst=0;
			}
			usValue %= 1800;
			stTm.tm_min = usValue / 30;
			stTm.tm_sec = (usValue % 30) * 2;
			nTm=mktime(&stTm);
			pStFrame->lTime=nTm;

			lfmTime = stTm.tm_hour*100 + stTm.tm_min;

			//Johnny 20150514 修改夜市判断时间
			if ( m_TimeNightStart>0 && m_TimeNightEnd>0 )
			{
				if ( (lfmTime >= m_TimeNightStart && lfmTime <= m_TimeNightEnd) || 
					(lfmTime >= 0 && lfmTime <= abs(m_TimeNightEnd-2400)) )
					bNight = TRUE;
			}
			else
				if( (lfmTime >= 1630 && lfmTime <= 2530) || (lfmTime >= 0 && lfmTime <= 130) )
 					bNight = TRUE;

			pStFrame->btGroupCode=buff[sizeof(_tagTTDataFrame)+1];	//position 2 is group code.
			
			memcpy(&pStFrame->nTransDataLength,buff+sizeof(_tagTTDataFrame)+2,2);	//position 3 4 is frame length
			if((buff[sizeof(_tagTTDataFrame)+6]>' ')&&(buff[sizeof(_tagTTDataFrame)+6]<128))
			{//
				nTradIp+=14;	//for decompress frame
				memcpy(&pStFrame->arItemCode,buff+sizeof(_tagTTDataFrame)+6,8);
			}
			else
			{
				memset(pStFrame->arItemCode,0,8);
			}

			if(buff[sizeof(_tagTTDataFrame)+6]>127)
			{
				WORD wd=TT_CHA_ITEMCODE;
				memcpy(&pStFrame->pTransData,&wd,2);
				pStFrame->btTransCode=buff[sizeof(_tagTTDataFrame)+6];
				nTradIp+=6;		//Itemcode is speci

			}
			//before 20071026
			/*
			else if(buff[sizeof(_tagTTDataFrame)+6]==1)
			{		//数字ItemCode 转化为 5 位数字后加空格
				nTradIp+=9;	//for decompress frame
				memcpy(&usValue,buff+sizeof(_tagTTDataFrame)+7,sizeof(usValue));
				//_itot(usValue,pStFrame->arItemCode,10);
				//tcslen();
				sprintf(pStFrame->arItemCode,"%05d",usValue);
				memset(&pStFrame->arItemCode[5],' ',3);
				pStFrame->btTransCode=buff[sizeof(_tagTTDataFrame)+9];

				///////////////////////////////////////////////////////
				// hope, add, 2003-11-03
					if(pStFrame->btTransCode>127)
					{
						pStFrame->btTransCode='?';
					}
				// 
				///////////////////////////////////////////////////////


				WORD wd=TT_NUM_ITEMCODE;
				memcpy(&pStFrame->pTransData,&wd,2);

			}
			else
			{//字符ItemCode
				pStFrame->btTransCode=buff[sizeof(_tagTTDataFrame)+14];

				///////////////////////////////////////////////////////
				// hope, add, 2003-11-03
				if(pStFrame->btTransCode > 127)
				{
					pStFrame->btTransCode = '?';
				}
				//
				///////////////////////////////////////////////////////

				WORD wd=TT_CHA_ITEMCODE;
				memcpy(&pStFrame->pTransData,&wd,2);

			}
			*/
			else
			{//<= 127
				if(buff[sizeof(_tagTTDataFrame)+6]>' ')
				{//字符形式的itemcode
					pStFrame->btTransCode=buff[sizeof(_tagTTDataFrame)+14];
					if(pStFrame->btTransCode > 127)
					{
						pStFrame->btTransCode = '?';
					}
					WORD wd=TT_CHA_ITEMCODE;
					memcpy(&pStFrame->pTransData,&wd,2);
				}
				else
				{
					WORD wd=TT_NUM_ITEMCODE;
					switch(buff[sizeof(_tagTTDataFrame)+6])
					{
					case 1:
						//数字ItemCode 转化为 5 位数字后加空格
						nTradIp+=9;	//for decompress frame
						memcpy(&usValue,buff+sizeof(_tagTTDataFrame)+7,sizeof(usValue));
						sprintf(pStFrame->arItemCode,"%05d",usValue);
						memset(&pStFrame->arItemCode[5],' ',3);
						pStFrame->btTransCode=buff[sizeof(_tagTTDataFrame)+9];
						break;
					case 2:
						//数字ItemCode 转化为 5 位数字后加空格
						nTradIp+=10;	//for decompress frame
						memcpy(&usValue,buff+sizeof(_tagTTDataFrame)+7,sizeof(usValue));
						sprintf(pStFrame->arItemCode,"%05d",usValue);
						memset(&pStFrame->arItemCode[5],' ',3);
						pStFrame->btTransCode=buff[sizeof(_tagTTDataFrame)+10];
						break;
					case 3:
						//数字ItemCode 转化为 5 位数字后加空格
						nTradIp+=11;	//for decompress frame
						memcpy(&dwVal,buff+sizeof(_tagTTDataFrame)+7,sizeof(dwVal));
						sprintf(pStFrame->arItemCode,"%05d",dwVal);
						memset(&pStFrame->arItemCode[5],' ',3);
						pStFrame->btTransCode=buff[sizeof(_tagTTDataFrame)+11];
						break;
					case 4:
						//数字ItemCode 转化为 5 位数字后加空格
						nTradIp+=12;	//for decompress frame
						memcpy(&dwVal,buff+sizeof(_tagTTDataFrame)+7,sizeof(dwVal));
						sprintf(pStFrame->arItemCode,"%05d",dwVal);
						memset(&pStFrame->arItemCode[5],' ',3);
						pStFrame->btTransCode=buff[sizeof(_tagTTDataFrame)+12];
						break;
					case 5:
						//数字ItemCode 转化为 5 位数字后加空格
						nTradIp+=16;	//for decompress frame
						memcpy(pStFrame->arItemCode,buff+sizeof(_tagTTDataFrame)+7,8);
						pStFrame->btTransCode=buff[sizeof(_tagTTDataFrame)+16];
						wd=TT_CHA_ITEMCODE;
						break;
					default:
						//an error frame
						//Error frame handle
						//This->m_ErrorFrame++;
						//This->m_ErrorByte+=nBuffIp-sizeof(_tagTTDataFrame)+1;
						//
						goto label2;
						break;
					}
					if(pStFrame->btTransCode > 127)
					{
						pStFrame->btTransCode = '?';
					}
					memcpy(&pStFrame->pTransData,&wd,2);
				}
			}
			//////for debug
			if(pStFrame->btTransCode == 136)
			{
				int hh = 0;
			}

			if(CheckIsXItem(pStFrame->btGroupCode,pStFrame->arItemCode))
				bxItem = TRUE;

			////////
			//-----以下为分解包
char temp[2000];
memcpy(temp,buff,sizeof(buff));
			memmove(buff+sizeof(_tagTTDataFrame),buff+nTradIp+1,sizeof(buff)-nTradIp-2);
			//nBuffip = sizeof(_tagTTDataFrame) + TTdatalen - 1;
			nBuffIp=nBuffIp-nTradIp-1;

			InFrameHead = 1;
			do
			{

/*
				if( pStFrame->btTransCode=='E' )
				{
					TRACE( "e frame\r\n" ) ;
				}
*/
				bCheckFrame = FALSE;   //一个frame里面有多个trans code。

				switch(pStFrame->btTransCode)
				{
				case 'A':
				case 'B':
				case 'C':					
					nTradLen=sizeof(float);		//is 4
					break;
				case 'E':	// paul, 2004-12-24, add, F group prv close.
					memcpy(&nTradLen,buff+sizeof(_tagTTDataFrame),2);
					nTradLen+=2;
					break;
				
				////////////////////////////////// 
				// new forex
				case 'y':
					nTradLen=20;				//
					break;
				////////////////////////////////// 

				case 'F':
					nTradLen=G_GRPFTXTLEN;
					break;
				case 'G':
				case 'H':
					bCheckFrame = TRUE;
					nTradLen=sizeof(float);			//is 4
					break;
				case 'I':
				case 'L':
				case 'M':
					nTradLen=sizeof(float);			//is 4
					break;
				case 'N':
					nTradLen=sizeof(float)*2;		//is 8
					break;
				case 'O':
					bCheckFrame = TRUE;
					nTradLen=sizeof(float);			//is 4
					break;
				case 'R':
					bCheckFrame = TRUE;
					switch(buff[sizeof(_tagTTDataFrame)]&3)
					{
					case 3:
						nTradLen=sizeof(double)*2+1;	//is 17
						break;
					case 1:
					case 2:
						nTradLen=sizeof(double)+1;		//is 9
						break;
					}
					break;
				case 'S':
					bCheckFrame = TRUE;
					nTradLen=G_GRPTSHARELEN+1;
					break;
				case 'T':
					bCheckFrame = TRUE;
					nTradLen=G_GRPTOVERLEN+1;
					break;
				case 'X':
					bXFrame = TRUE;
					nTradLen = 2;
					break;
				case 'a':
					nTradLen=sizeof(float)*2;		//is 8
					break;
				case 'b':
					nTradLen=sizeof(float)+1;		//is 5
					break;
				case 'f':
					nTradLen=55;		//calcu by hope
					break;
				case 'g':
					//应该使用Fram中的第11~12指出Data的长度(不包括CRC)
					//现在使用Fram本身的长度
					nTradLen=nBuffIp-2;
					break;
				case 'h':
					nTradLen=12;		//calcu by hope
					break;
				case 'i':
					nTradLen = 0;
					memcpy(&nTradLen,buff+sizeof(_tagTTDataFrame),2);
//						memcpy(&nTradLen,buff+18,2);

					nTradLen+=2;
					break;
				case 'l':
					nTradLen=sizeof(float)+sizeof(unsigned long)+sizeof(unsigned long);		//is 12
					break;
				case 'p':
					{
						WORD wdVal;
						
						if((DWORD(pStFrame->pTransData)&0x0000ffff)==TT_NUM_ITEMCODE)
						{
							switch(buff[sizeof(_tagTTDataFrame)]&2)
							{
							case 0:
								nTradLen=abs((char)buff[sizeof(_tagTTDataFrame)+1])*(G_GRPBSHARELEN+sizeof(short))+2;
								break;
							case 2:
								bt=1;
								nsValue=0;
								for(i=0;i<8;i++)
								{
									if(bt&buff[sizeof(_tagTTDataFrame)+1]) nsValue++;
									bt*=2;
								}
								nTradLen=nsValue*(G_GRPBSHARELEN+sizeof(short))+2;
								break;
							}
						}
						else
						{
							memcpy(&wdVal,&buff[sizeof(_tagTTDataFrame)+7],2);
							if(wdVal)
							{
								nTradLen=wdVal*2+8;
							}
							else
							{
								nTradLen=8;
							}
						}
					}
					break;
				case 'o':
					{
						WORD wdVal;
						
						if((DWORD(pStFrame->pTransData)&0x0000ffff)==TT_NUM_ITEMCODE)
						{
							switch(buff[sizeof(_tagTTDataFrame)]&2)
							{
							case 0:
								nTradLen=abs((char)buff[sizeof(_tagTTDataFrame)+1])*(G_GRPBSHARELEN+sizeof(short))+2;
								break;
							case 2:
								bt=1;
								nsValue=0;
								for(i=0;i<8;i++)
								{
									if(bt&buff[sizeof(_tagTTDataFrame)+1]) nsValue++;
									bt*=2;
								}
								nTradLen=nsValue*(G_GRPBSHARELEN+sizeof(short))+2;
								break;
							}
						}
						else
						{
							memcpy(&wdVal,&buff[sizeof(_tagTTDataFrame)+7],2);
							if(wdVal)
							{
								nTradLen=wdVal*2+8;
							}
							else
							{
								nTradLen=8;
							}
						}
					}
					break;
				case 'q':
					{
						WORD wdVal;
						
						if((DWORD(pStFrame->pTransData)&0x0000ffff)==TT_NUM_ITEMCODE)
						{
							switch(buff[sizeof(_tagTTDataFrame)]&3)
							{
							case 0:
								nTradLen=2;
								break;
							default:
								nTradLen=(buff[sizeof(_tagTTDataFrame)+1]&63)*sizeof(short)+2;
								break;
							}
						}
						else
						{
							memcpy(&wdVal,&buff[sizeof(_tagTTDataFrame)+7],2);
							if(wdVal)
							{
								nTradLen=wdVal*2+8;
							}
							else
							{
								nTradLen=8;
							}
						}
					}
					break;
				case 'e':    //andy add 2014.01.28 'e'代替't'
					if(!buff[sizeof(_tagTTDataFrame)+4])
					{
						nTradLen=3;
					}
					else
					{
						if(buff[sizeof(_tagTTDataFrame)+4]==1)
						{
							nTradLen=21;		//calcu by Hope
						}
						else
						{
							nTradLen=4+1+buff[sizeof(_tagTTDataFrame)+4];
							for(i=0;i<buff[sizeof(_tagTTDataFrame)+4]-1;i++)
							{ 
								if(buff[sizeof(_tagTTDataFrame)+6+i]&8) nTradLen+=1;
								if(buff[sizeof(_tagTTDataFrame)+6+i]&16) nTradLen+=sizeof(unsigned long);		//is 4
								if(buff[sizeof(_tagTTDataFrame)+6+i]&32) nTradLen+=sizeof(float);		//is 4
								if(buff[sizeof(_tagTTDataFrame)+6+i]&64) nTradLen+=4;  //TranTime + Key
								if(buff[sizeof(_tagTTDataFrame)+6+i]&128) nTradLen+=sizeof(unsigned short);		//is 2
							}
							nTradLen+=15;		//No 1. is still exit len=14
						}
					}
					//mCArrayItemDataFrame.Add(*pStFrame);
					//mCListDataFrame.AddTail(*pStFrame);
					//HandleFrame_e(buff, sizeof(_tagTTDataFrame)+nTradLen);
					break;
				case 't':
					if(!buff[sizeof(_tagTTDataFrame)+2])
					{
						nTradLen=3;
					}
					else
					{
						if(buff[sizeof(_tagTTDataFrame)+2]==1)
						{
							nTradLen=18;		//calcu by Hope
						}
						else
						{
							nTradLen=2+1+buff[sizeof(_tagTTDataFrame)+2];
							for(i=0;i<buff[sizeof(_tagTTDataFrame)+2]-1;i++)
							{
								if(buff[sizeof(_tagTTDataFrame)+4+i]&8) nTradLen+=1;
								if(buff[sizeof(_tagTTDataFrame)+4+i]&16) nTradLen+=sizeof(unsigned long);		//is 4
								if(buff[sizeof(_tagTTDataFrame)+4+i]&32) nTradLen+=sizeof(float);		//is 4
								if(buff[sizeof(_tagTTDataFrame)+4+i]&64) nTradLen+=3;
								if(buff[sizeof(_tagTTDataFrame)+4+i]&128) nTradLen+=sizeof(unsigned short);		//is 2
							}
							nTradLen+=14;		//No 1. is still exit len=14
						}
					}
					break;
				case 'u':
					nTradLen=sizeof(unsigned long)*2;			//calcu by hope
					break;
				case 'v':
					{
						WORD wdVal;
						memcpy(&wdVal,buff+sizeof(_tagTTDataFrame),2);
						nTradLen=wdVal+2;		//calcu by hope
					}
					break;
				case 'x':
					nTradLen=buff[sizeof(_tagTTDataFrame)]*(sizeof(char)+sizeof(float)+sizeof(unsigned long))+1;		//calcu by hope
					break;
				case 128:			//Trading day
					if(!InFrameHead)
					{
						nBuffIp=-1;
						break;
					}
					nTradLen=sizeof(long int);		//calcu by hope
					break;
				case 129:			//System is active.
					if(!InFrameHead)
					{
						nBuffIp=-1;
						break;
					}
					nTradLen=0;
					break;
				case 130:			//A Message from center
					if(!InFrameHead)
					{
						nBuffIp=-1;
						break;
					}
					nTradLen=nBuffIp-2;
					break;
				case 131:			//news message
				case 132:
				case 133:
				case 134:
				case 136:
				case 137:
				case 141:
				case 142:
				case 143:
				case 144:
				case 145:
				case 146:
				case 147:
				case 148:
				case 149:
					if(!InFrameHead)
					{
						nBuffIp=-1;
						break;
					}
					nTradLen=nBuffIp-2;
					break;
					//				case 200:		//系统发送的时间
					memcpy(&ulValue,buff+sizeof(_tagTTDataFrame)+6,sizeof(ulValue));
					stTm.tm_year=ulValue/10000-1900;
					stTm.tm_mon=(ulValue-(stTm.tm_year+1900)*10000)/100-1;
					stTm.tm_mday=ulValue-(stTm.tm_year+1900)*10000-(stTm.tm_mon+1)*100;
					nBuffIp=-2;
					break;
				case 200:
					//memcpy(&This->m_TodayDate,buff+sizeof(_tagTTDataFrame),sizeof(This->m_TodayDate));
				case 201:
				case 202:
				case 203:
				case 204:
				case 205:
				case 206:
				case 207:
				case 208:
				case 209:
				case 210:
				case 211:
				case 212:
				case 213:
				case 214:
				case 215:
				case 216:
				case 217:
				case 218:
				case 219:
				case 220:
				case 221:
				case 222:
				case 223:
				case 224:
				case 225:
				case 226:
				case 227:
				case 228:
				case 229:
				case 230:
				case 231:
				case 232:
				case 233:
				case 234:
				case 235:
				case 236:
				case 237:
				case 238:
				case 239:
				case 240:
				case 241:
				case 242:
				case 243:
				case 244:
				case 245:
				case 246:
					if(!InFrameHead)
					{
						nBuffIp=-1;
						break;
					}
					nTradLen=nBuffIp-2;
					break;
				case 247:
					if(!InFrameHead)
					{
						nBuffIp=-1;
						break;
					}
					
					nBuffIp=-1;
					break;
				case 248:
				case 249:
				case 250:
				case 251:
				case 252:
				case 253:
				case 254:
					if(!InFrameHead)
					{
						nBuffIp=-1;
						break;
					}
					nTradLen=nBuffIp-2;
					break;
				case 255:	//Dixon 说 (11:17):255是dummy transaction，收到後可以忽略掉
					//if(!InFrameHead)
					{
						nBuffIp=-1;
						break;
					}
					nTradLen=nBuffIp-2;
					break;
				default:		//unknow Frame
					nBuffIp=-1;
					break;
			}
			if((nBuffIp-nTradLen-1)<0)
			{
				nBuffIp=-1;
			}
			if( (nBuffIp>0) && (nTradLen>=0) )		// 2003-10-22, hope, add (nTradLen>=0).	
			{
				InFrameHead = 0;
				pStFrame->nLength=sizeof(_tagTTDataFrame)+nTradLen;
				pStFrame->nTransDataLength=nTradLen;
				//ATLASSERT( pStFrame->nTransDataLength>=0 ) ; 
					
				//HandleFrame(buff,sizeof(_tagTTDataFrame)+nTradLen);
				//if( m_pProc!=NULL )
				{
					if((bCheckFrame && bNight && bXFrame==FALSE && bxItem) == FALSE) //4个条件都满足,不发这个帧
					{
						//(*m_pProc)( m_pProcArg, buff, sizeof(_tagTTDataFrame)+nTradLen );
						//g_Master.tele.HandleFrame(buff,sizeof(_tagTTDataFrame)+nTradLen);
						//mCArrayItemDataFrame.Add(*pStFrame); // xxx long time
						//mCListDataFrame.AddTail(*pStFrame);
						//mCListDataFrame.AddHead(*pStFrame);
						ReadGetFrames ++;
				if ( mCanBuffer > 0)
				{
						if (g_fileSizeMB <mBufferMaxMB)
							{
								if (pStFrame->btTransCode == 'e')
								{
									HandleFrame_e(buff, sizeof(_tagTTDataFrame)+nTradLen);
								}
								else
								{	
									mCListDataFrame.AddTail(*pStFrame);
								}
							}
						else
							{
								if (gpMsEdit_WendyDlg->pShowData->op_TransCode(pStFrame)&&gpMsEdit_WendyDlg->pShowData->op_item(pStFrame)&&gpMsEdit_WendyDlg->pShowData->op_time(pStFrame))
									{
											if (gpMsEdit_WendyDlg->pShowData->noselitem && gpMsEdit_WendyDlg->pShowData->noselcode && gpMsEdit_WendyDlg->pShowData->noseltime)
											{
												BigFile_NoSelectGetTotalRecord ++;
											}
											else
											{
												BigFile_NoSelectGetTotalRecord = 0;
											}
										if (pStFrame->btTransCode == 'e')
										{
											HandleFrame_e(buff, sizeof(_tagTTDataFrame)+nTradLen);
										}
										else
										{	
											mCListDataFrame.AddTail(*pStFrame);
										}
									}
							}
				}
						
						else if ( mCanBuffer > -1)
						{	
								if (pStFrame->btTransCode == 'e')
								{
									HandleFrame_e(buff, sizeof(_tagTTDataFrame)+nTradLen);
								}
								else if (gpMsEdit_WendyDlg->pShowData->op_TransCode(pStFrame)&&gpMsEdit_WendyDlg->pShowData->op_item(pStFrame)&&gpMsEdit_WendyDlg->pShowData->op_time(pStFrame))
								{
									CString CStrItemCode_log;
									char CStrItemCode[10];
									memset( CStrItemCode, 0, sizeof(CStrItemCode) ) ;
									memcpy(CStrItemCode,(char *)&(pStFrame->arItemCode[0]),sizeof(char)*8);
									CStrItemCode_log.Format("ItemCode:%s,",CStrItemCode);
									
									CString Cstrdwval;
									Cstrdwval.Format("Group:%c,",pStFrame->btGroupCode);
									
									CString CstrBrokerNo;
									CstrBrokerNo.Format("Trans:%c,",pStFrame->btTransCode);
									
									CString CstrTradeTime;
									CstrTradeTime.Format("Time:%ld,",pStFrame->lTime);
									
									CString CstrTradeTime2;
									CstrTradeTime2.Format("Length:%d,",pStFrame->nLength);		
									
									CString CstrKey;
									CstrKey.Format("DataLength:%d,",pStFrame->nTransDataLength);
				
									
									s_eLog = CStrItemCode_log + Cstrdwval + CstrBrokerNo + CstrTradeTime + CstrTradeTime2 +CstrKey;
									w_InofLog.Log(s_eLog);
								}
						}
													
					}
				}

				pStFrame->btTransCode=buff[sizeof(_tagTTDataFrame)+nTradLen];
				memmove(buff+sizeof(_tagTTDataFrame),buff+sizeof(_tagTTDataFrame)+nTradLen+1,sizeof(buff)-sizeof(_tagTTDataFrame)-nTradLen-2);
				nBuffIp=nBuffIp-nTradLen-1;
			}
			else
			{
			}
			}while(nBuffIp>3);

label2:
			nTradIp=sizeof(_tagTTDataFrame);
			//-----以上为分解包
			//--------------------------------------------------------------------------------
			nBuffIp=sizeof(_tagTTDataFrame);
			bStxFound=false;
			bDleFound=false;
			continue;
		}//找到一帧
		if(lpbuf==DLE)
		{			//now is DLE
			if(bDleFound)
			{		//if last is DLE too
				bDleFound=false;	//now is 0x10 not DLE
				buff[nBuffIp++]=lpbuf;
				//				nDleInFrame++;
			}
			else bDleFound=true;
		}
		else
		{					//now is not DLE
			//			if(lpbuf==STX) nDleInFrame++;
			//			else if(lpbuf==ETX) nDleInFrame++;
			bDleFound=false;
			buff[nBuffIp++]=lpbuf;
			if(nBuffIp>1030+sizeof(_tagTTDataFrame))
			{
				nBuffIp=sizeof(_tagTTDataFrame);
			}
		}

	}while( procSize<bufSize || nTempBufAddr>0 ) ;
	return TRUE ;

}

void CUnpacker_TTFrame::ClearStream() 
{
	bStxFound=false;
	bDleFound=false;
	nBuffIp=sizeof(_tagTTDataFrame);
	i=1;
	InFrameHead = 1;
	nTradIp=sizeof(_tagTTDataFrame);

	memset(buff,0,sizeof(_tagTTDataFrame)+1030);
	pStFrame=(_tagTTDataFrame *)buff;
	time(&nTm);
	stTm = *gmtime(&nTm);
	nTempBufAddr = 0 ; 

	pFrame_e = &mFrame_e;

}


void CUnpacker_TTFrame::HandleFrame_e(BYTE * buff,WORD Len)
{
	_tagTTDataFrame* pHead=(_tagTTDataFrame *)buff;

	if (pHead->btTransCode != 'e')
	{
		TRACE(" Because Tx Not e, So return \n");
		return;
	}
		
	WORD	Time=(WORD)((DWORD)pHead->pTransData>>16);
	DWORD	usTotal;//WORD	usTotal;
	BYTE	bySendNum = 0;
	UINT	uiTotal;

	//Transaction总数
	memcpy(&usTotal, buff + sizeof(_tagTTDataFrame), sizeof(DWORD));
	//第几个被送来N
	memcpy(&bySendNum, buff + sizeof(_tagTTDataFrame) + 4, 1);
	if(!bySendNum)
	{
		return ;
	}
	uiTotal=usTotal;

	BYTE	byHi;
	int		nOffset;
	int		nOffsetData;

	memcpy(&byHi, buff + sizeof(_tagTTDataFrame) + 5, 1);
	nOffset = sizeof(_tagTTDataFrame) + 6;
	nOffsetData=nOffset;				//指向数据块
	nOffsetData+=(bySendNum-1);

	DWORD	dwVal;
	WORD	BrokerNo;
	WORD	TradeTime;
	DWORD   TradeTime2; 
	WORD	Key;
	float	Price;
	unsigned long Quantity;
	BYTE	TyadeType;
	DWORD	lTemp;
	int		i;
	BYTE	byTmp;
	char str[512]="" ;
	// 读取第一块
	memcpy(&BrokerNo,buff+nOffsetData,2);
	memcpy(&dwVal,buff+nOffsetData+2,4);
	memcpy(&Price,buff+nOffsetData+6,4);
	memcpy(&Quantity,buff+nOffsetData+10,4);
	memcpy(&TyadeType,buff+nOffsetData+14,1);

	lTemp = dwVal & 0x0003FFFF;
	TradeTime2 = lTemp;  //memcpy(&TradeTime, &lTemp, 2);

	//andy 2019.6.19 added
	if(TradeTime2<60000) 
		TradeTime2 = TradeTime2+262144; // 18bit最大262144，溢出了！
	//char szTmp[100];
	//sprintf(szTmp,"pHead->btTransCode=%c",pHead->btTransCode);
	//OutputDebugString(szTmp);
	//andy 2019.6.19 added

	lTemp = ((dwVal & 0x3FFC0000) >> 18);
	memcpy(&Key, &lTemp, 2);

	nOffsetData+=15;
	
	TradeTime=TradeTime2/10000*1800 + (TradeTime2%10000)/100*30+(TradeTime2%100)/2;

//	char temp[10] ;
//	memset( temp, 0, sizeof(temp) ) ;
//	temp[0]	= pHead->btGroupCode ;
//	memcpy( temp+1, pHead->arItemCode, 8 ) ;
//	{
//		sprintf(str,"%s, tradeticker-- TickTime:%d %d, key: %u, price:%.4f, share:%d, total:%d, BrokerNo:%d\r\n", 
//			temp, TradeTime,TradeTime2, Key, Price, Quantity, usTotal, BrokerNo);
//	}

	if ( mCanBuffer>0)
	{
		mCListDataFrame.AddTail(*pHead);
	}

	memcpy(mFrame_e.arItemCode,(char *)&( pHead->arItemCode[0]),sizeof(char)*8);
	memcpy(&(mFrame_e.btGroupCode),&(pHead->btGroupCode),1);
	memcpy(&(mFrame_e.btTransCode),&(pHead->btTransCode),1);
	mFrame_e.nLength = pHead->nLength;
	mFrame_e.lTime = pHead->lTime;
	mFrame_e.nTransDataLength = pHead->nTransDataLength;
	mFrame_e.pTransData = pHead->pTransData;

	mFrame_e.dwVal = dwVal;
	mFrame_e.BrokerNo = BrokerNo;
	mFrame_e.TradeTime = TradeTime;
	mFrame_e.TradeTime2 = TradeTime2;
	mFrame_e.Key = Key;
	mFrame_e.Price = Price;
	mFrame_e.Quantity = Quantity;
	mFrame_e.TyadeType = TyadeType;
	mFrame_e.lTemp = lTemp;
	mFrame_e.byTmp = byTmp;

	if ( mCanBuffer>0)
		{
			mCListFrame_e.AddTail(*pFrame_e);
		}
	else if (gpMsEdit_WendyDlg->pShowData->op_TransCode_e(&mFrame_e)&&gpMsEdit_WendyDlg->pShowData->op_item_e(&mFrame_e)&&gpMsEdit_WendyDlg->pShowData->op_time_e(&mFrame_e))
		{

		CString CStrItemCode_log;
		char CStrItemCode[10];
		memset( CStrItemCode, 0, sizeof(CStrItemCode) ) ;
		memcpy(CStrItemCode,(char *)&(mFrame_e.arItemCode[0]),sizeof(char)*8);
		CStrItemCode_log.Format("ItemCode:%s,",CStrItemCode);
		
		CString Cstrdwval;
		Cstrdwval.Format("dwval:%d,",mFrame_e.dwVal);
		
		CString CstrBrokerNo;
		CstrBrokerNo.Format("BrokerNo:%d,",mFrame_e.BrokerNo);
		
		CString CstrTradeTime;
		CstrTradeTime.Format("Time:%d,",mFrame_e.TradeTime);
		
		CString CstrTradeTime2;
		CstrTradeTime2.Format("Time2:%d,",mFrame_e.TradeTime2);		
		
		CString CstrKey;
		CstrKey.Format("Key:%d,",mFrame_e.Key);
		
		
		CString CstrPrice;
		CstrPrice.Format("Price:%f,",mFrame_e.Price);
		
		
		CString CstrQuantity;
		CstrQuantity.Format("Quantity:%d,",mFrame_e.Quantity);
		
		
		CString CstrTradeType;
		CstrTradeType.Format("TradeType:%d",mFrame_e.TyadeType);
		
		s_eLog = CStrItemCode_log + Cstrdwval + CstrTradeTime + CstrTradeTime2 +CstrKey +CstrPrice + CstrQuantity + CstrTradeType;
		w_InofLog.Log(s_eLog);
		}

	for(i = 0; i < bySendNum-1; i ++)
	{
		memcpy(&byTmp, buff + nOffset, 1);
		nOffset ++;
		
		if((byTmp & 128)==128)
		{
			memcpy(&BrokerNo,buff+nOffsetData,2);
			nOffsetData+=2;
		}
		
		if((byTmp & 64)==64)
		{
			memcpy(&dwVal,buff+nOffsetData,4);
			lTemp = dwVal & 0x0003FFFF;
			TradeTime2 = lTemp;		
			TradeTime=(TradeTime2 / 10000) * 1800 + (TradeTime2 % 10000)/100 * 30;
			lTemp = ((dwVal & 0x3FFC0000) >> 18);
			memcpy(&Key, &lTemp, 2);
			nOffsetData+=4;
		}
		else
		{
			Key++;
		}
		
		if((byTmp & 32) ==32)
		{
			memcpy(&Price,buff+nOffsetData,4);
			nOffsetData+=4;
		}
		
		if((byTmp&16)==16)
		{
			memcpy(&Quantity,buff+nOffsetData,4); 
			nOffsetData+=4;
		}
		
		if((byTmp&8)==8)
		{
			memcpy(&TyadeType,buff+nOffsetData,1);
			nOffsetData++;
		}

//		memset( temp, 0, sizeof(temp) ) ;
//		temp[0] = pHead->btGroupCode ;
//		memcpy( temp+1, pHead->arItemCode, 8 ) ;
//		TRACE( "%s, tradeticker-- TickTime:%d %d, key: %u, price:%.4f, share:%d, TyadeType:%c, BrokerNo:%d\r\n", 
//			temp, TradeTime,TradeTime2, Key, Price, Quantity, TyadeType, BrokerNo ) ;
//		sprintf(str,"%s, tradeticker-- TickTime:%d %d, key: %u, price:%.4f, share:%d, total:%d, BrokerNo:%d\r\n", 
//			temp, TradeTime,TradeTime2, Key, Price, Quantity, usTotal, BrokerNo);


	memcpy(mFrame_e.arItemCode,(char *)&( pHead->arItemCode[0]),sizeof(char)*8);
	memcpy(&(mFrame_e.btGroupCode),&(pHead->btGroupCode),1);
	memcpy(&(mFrame_e.btTransCode),&(pHead->btTransCode),1);
	mFrame_e.nLength = pHead->nLength;
	mFrame_e.lTime = pHead->lTime;
	mFrame_e.nTransDataLength = pHead->nTransDataLength;
	mFrame_e.pTransData = pHead->pTransData;

	mFrame_e.dwVal = dwVal;
	mFrame_e.BrokerNo = BrokerNo;
	mFrame_e.TradeTime = TradeTime;
	mFrame_e.TradeTime2 = TradeTime2;
	mFrame_e.Key = Key;
	mFrame_e.Price = Price;
	mFrame_e.Quantity = Quantity;
	mFrame_e.TyadeType = TyadeType;
	mFrame_e.lTemp = lTemp;
	mFrame_e.byTmp = byTmp;
	
	//mCListFrame_e.AddTail(*pFrame_e);
		if ( mCanBuffer>0)
		{
			mCListFrame_e.AddTail(*pFrame_e);
		}
	else if (gpMsEdit_WendyDlg->pShowData->op_TransCode_e(&mFrame_e)&&gpMsEdit_WendyDlg->pShowData->op_item_e(&mFrame_e)&&gpMsEdit_WendyDlg->pShowData->op_time_e(&mFrame_e))
		{

		CString CStrItemCode_log;
		char CStrItemCode[10];
		memset( CStrItemCode, 0, sizeof(CStrItemCode) ) ;
		memcpy(CStrItemCode,(char *)&(mFrame_e.arItemCode[0]),sizeof(char)*8);
		CStrItemCode_log.Format("ItemCode:%s,",CStrItemCode);
		
		CString Cstrdwval;
		Cstrdwval.Format("dwval:%d,",mFrame_e.dwVal);
		
		CString CstrBrokerNo;
		CstrBrokerNo.Format("BrokerNo:%d,",mFrame_e.BrokerNo);
		
		CString CstrTradeTime;
		CstrTradeTime.Format("Time:%d,",mFrame_e.TradeTime);
		
		CString CstrTradeTime2;
		CstrTradeTime2.Format("Time2:%d,",mFrame_e.TradeTime2);		
		
		CString CstrKey;
		CstrKey.Format("Key:%d,",mFrame_e.Key);
		
		
		CString CstrPrice;
		CstrPrice.Format("Price:%f,",mFrame_e.Price);
		
		
		CString CstrQuantity;
		CstrQuantity.Format("Quantity:%d,",mFrame_e.Quantity);
		
		
		CString CstrTradeType;
		CstrTradeType.Format("TradeType:%d",mFrame_e.TyadeType);
		
		s_eLog = CStrItemCode_log + Cstrdwval + CstrTradeTime + CstrTradeTime2 +CstrKey +CstrPrice + CstrQuantity + CstrTradeType;
		w_InofLog.Log(s_eLog);
		}
	}
}

