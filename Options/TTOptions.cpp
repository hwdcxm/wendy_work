// TTOptions.cpp : Implementation of CTTOptions

#include "stdafx.h"
#include "Options.h"
#include "TTOptions.h"

/////////////////////////////////////////////////////////////////////////////
// CTTOptions


STDMETHODIMP CTTOptions::InitObject()
{
	return S_OK;
}
STDMETHODIMP CTTOptions::Activate(int nState)
{
	return S_OK;
}
STDMETHODIMP CTTOptions::GetState(int* pnState)
{
		return S_OK;
}
STDMETHODIMP CTTOptions::GetByID(int nID, VARIANT** ppVar)
{
	return S_OK;
}

STDMETHODIMP CTTOptions::PutByID(int nID, VARIANT* pVar)
{
	return S_OK;
}
STDMETHODIMP CTTOptions::GetByName(BSTR strName, VARIANT** ppVar)
{
	return S_OK;
}
STDMETHODIMP CTTOptions::PutByName(BSTR strName, VARIANT* pVar)
{
	return S_OK;
}