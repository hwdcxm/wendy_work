// Master.cpp: implementation of the Master class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsEdit_Wendy.h"
#include "Master.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

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
	if( m_pDataSrc_TTFrameFile!=NULL )
	{
		m_pDataSrc_TTFrameFile->Play() ;
	}
//	m_procTTFrame.Play() ;
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
