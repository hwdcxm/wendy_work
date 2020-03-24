// Master.cpp: implementation of the Master class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit_Wendy.h"
#include "Master.h"
#include "GroupsMng.h"
#include "GroupsMng_i.c"
#include "WTTCommon_i.c"
#include "log.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern CLog w_Log;
extern CString s_Log;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Master::Master()
{

}

Master::~Master()
{

}

void Master::Play() 
{
	//if( m_pDataSrc_TTFrameFile!=NULL )
	{
		m_DataSrc_TTFrameFile.Play() ;
	}
//	m_procTTFrame.Play() ;
}


BOOL Master::CreateGroupsMng()
{
	if( m_pGroupsMng!=NULL )
		return TRUE ;
	
	HRESULT ret = CoCreateInstance( CLSID_TTGroupsMng, 
		NULL, CLSCTX_ALL,
		IID_IUnknown, (LPVOID*)&m_pGroupsMng );
	int n=GetLastError();
	if( ret!=S_OK || m_pGroupsMng==NULL )
	{
		m_pGroupsMng = NULL ;
		//g_Log.Log("Groupmng.dll load fail!");
		s_Log.Format("Groupmng.dll load fail!");
		return  FALSE ;
	}
	
	//g_Log.Log("Groupmng.dll Ok.");
	s_Log.Format("Groupmng.dll Ok.");
	CComQIPtr<ITTObject,&IID_ITTObject> pObj(m_pGroupsMng);
	
	// ?? no set systemObject ??
	// ...
	
	try
	{
		int nRet = pObj->InitObject() ;
		//Johnny add 2014.12.24///////////////////////////////
		//RewriteNightTINI(m_pGroupsMng);
		
		//Johnny add 2014.12.24///////////////////////////////
	}
	catch(...)
	{
		
	}
	
	return TRUE ;
	
}

void Master::Stop() 
{
	
//	m_hStop=TRUE;
//	if( m_pDataSrc_TTFrameFile!=NULL )
//	{
//		m_pDataSrc_TTFrameFile->Stop() ;
//		delete m_pDataSrc_TTFrameFile;
//		m_pDataSrc_TTFrameFile=NULL;
//	}
//	if (&m_procTTFrame!=NULL)
//	{
//		m_procTTFrame.Stop() ;
//		memset(&m_procTTFrame,0,sizeof(m_procTTFrame));
//	}
//
//	ReInit();
//
//	tele.Stop();

// 	if( m_pGroupsMng!=NULL )
// 	{
// 		m_pGroupsMng->Release() ;
// 		m_pGroupsMng = NULL ;
// 	}	
	
}
