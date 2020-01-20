// TTOptions.cpp : Implementation of CTTOptions

#include "stdafx.h"
#include "Options.h"
#include "TTOptions.h"

/////////////////////////////////////////////////////////////////////////////
// CTTOptions


STDMETHODIMP CTTOptions::InitObject()
{
		CLogout("---STDMETHODIMP CTTOptions::InitObject() b \r\n");

	return S_OK;
}
STDMETHODIMP CTTOptions::Activate(int nState)
{
	CLogout("---STDMETHODIMP CTTOptions::Activate(nState=%d) b \r\n",nState);
	return S_OK;
}
STDMETHODIMP CTTOptions::GetState(int* pnState)
{
		CLogout("---STDMETHODIMP CTTOptions::GetState(pnState=%d) b \r\n",pnState);

		return S_OK;
}
STDMETHODIMP CTTOptions::GetByID(int nID, VARIANT** ppVar)
{
	CLogout("---STDMETHODIMP CTTOptions::GetByID(nID=%d,VARIANT** ppVar) b \r\n",nID);

	return S_OK;
}

STDMETHODIMP CTTOptions::PutByID(int nID, VARIANT* pVar)
{
			CLogout("---STDMETHODIMP CTTOptions::PutByID(nID=%d,VARIANT* pVar) b \r\n",nID);

	return S_OK;
}
STDMETHODIMP CTTOptions::GetByName(BSTR strName, VARIANT** ppVar)
{
		CLogout("---STDMETHODIMP CTTOptions::GetByName() b \r\n");

	return S_OK;
}
STDMETHODIMP CTTOptions::PutByName(BSTR strName, VARIANT* pVar)
{
		CLogout("---STDMETHODIMP CTTOptions::PutByName() b \r\n");

	return S_OK;
}