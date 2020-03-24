// Master.h: interface for the Master class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASTER_H__2541F30F_88E8_4021_BFDA_D00E8E0DF921__INCLUDED_)
#define AFX_MASTER_H__2541F30F_88E8_4021_BFDA_D00E8E0DF921__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Unpacker_TTFrame.h"
#include "DataSrc_TTFrameFile.h"

#define MSCE_TYPE_NONE			(0)
#define MSCE_TYPE_DATACENTER	(1)
#define MSCE_TYPE_PROXY			(2)

class Master  
{
public:
	Master();
	virtual ~Master();
	long m_FrameNums[MAX_PATH];
	//void ReInit();
	BOOL m_hStop;
	//void Init();
	BOOL CreateGroupsMng();
	IUnknown*	m_pGroupsMng ;

	struct OutItem 
	{
		char item[10];
		char transcode;
		int  delay;
		WORD starttime;
		WORD endtime;
	};
	OutItem m_itemdesk;
	int			m_MSCEType ;

	//CDataSrc_TTFrameFile*	m_pDataSrc_TTFrameFile ;
	CDataSrc_TTFrameFile	m_DataSrc_TTFrameFile ;
	CUnpacker_TTFrame		m_unpakcer_TTFrame ;

	void Stop();
	void Play();
};

#endif // !defined(AFX_MASTER_H__2541F30F_88E8_4021_BFDA_D00E8E0DF921__INCLUDED_)
