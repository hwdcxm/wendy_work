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
	
	HANDLE		m_hThread;	// thread. CWorkThread属性: 线程handle
	
	int			m_nThreadStatu;		// CWorkThread属性: 线程状态
	// thread statu:
	// <0, no thread.
	// =0, ask thread to end.
	// >0, thread runing.

	
	BOOL StartThread() ;	//类CWorkThread成员函数: 启动线程
	
	void RequestEnd ();		// 类CWorkThread成员函数: 请求停止线程
	
	virtual void EndThread( int wait = -1 ); // block mode.  类CWorkThread成员函数: 阻塞式停止线程
	
	virtual	UINT	ThreadEntry () = 0;		// 类CWorkThread成员函数: 线程函数
	// 说明:
	//		StartThread  -----> 启动工作线程, m_nThreadStatu=1,
	//		派生class必须重载ThreadEntry, 以下为一般模式:

};

#endif // !defined(AFX_WORKTHREAD_H__2DEC6B33_0D96_4945_BE59_9A22C74E7B03__INCLUDED_)
