// WorkThread.h: interface for the CWorkThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKTHREAD_H__2DEC6B33_0D96_4945_BE59_9A22C74E7B03__INCLUDED_)
#define AFX_WORKTHREAD_H__2DEC6B33_0D96_4945_BE59_9A22C74E7B03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "lock.h"

class CWorkThread  
{
public:
	CWorkThread();
	virtual ~CWorkThread();
	
	HANDLE		m_hThread;	// thread. CWorkThread����: �߳�handle
	
	int			m_nThreadStatu;		// CWorkThread����: �߳�״̬
	// thread statu:
	// <0, no thread.
	// =0, ask thread to end.
	// >0, thread runing.

	
	BOOL StartThread() ;	//��CWorkThread��Ա����: �����߳�
	
	void RequestEnd ();		// ��CWorkThread��Ա����: ����ֹͣ�߳�
	
	virtual void EndThread( int wait = -1 ); // block mode.  ��CWorkThread��Ա����: ����ʽֹͣ�߳�
	
	virtual	UINT	ThreadEntry () = 0;		// ��CWorkThread��Ա����: �̺߳���
	// ˵��:
	//		StartThread  -----> ���������߳�, m_nThreadStatu=1,
	//		����class��������ThreadEntry, ����Ϊһ��ģʽ:

};

#endif // !defined(AFX_WORKTHREAD_H__2DEC6B33_0D96_4945_BE59_9A22C74E7B03__INCLUDED_)
