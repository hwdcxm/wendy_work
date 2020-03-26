// TTOptionsDataTable.cpp: implementation of the CTTOptionsDataTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TTOptionsDataTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTTOptionsDataTable::CTTOptionsDataTable()
{
	int i;
	for( i= 0; i< TT_OP_ITEMNUM; i++)
	{
		::ZeroMemory(&m_txtopt[i], sizeof(g_txtoptS));
		lstrcpy(m_txtopt[i].itemcode,"");
		
		for(int j= 0; j<  G_TXTNUMOPTMONTH ; j++)
		{
			m_txtopt[i].mm[j].decimal = -1;
			m_txtopt[i].mm[j].futhashval = 0;
			for(int k =0; k <G_TXTNUMOPTSTRIKE ; k++)
			{
				for(int l = 0 ; l < 2 ; l ++)
				{
					m_txtopt[i].mm[j].st[k].cp[l].ab[0] = -1;
					m_txtopt[i].mm[j].st[k].cp[l].ab[1] = -1;
					m_txtopt[i].mm[j].st[k].cp[l].cabinetbid = 0;
					m_txtopt[i].mm[j].st[k].cp[l].cabinetlast =0;
					m_txtopt[i].mm[j].st[k].cp[l].cuvol = -1;
					m_txtopt[i].mm[j].st[k].cp[l].hilo[0] = -1;
					m_txtopt[i].mm[j].st[k].cp[l].hilo[1] = -1;
					m_txtopt[i].mm[j].st[k].cp[l].last = -1;
					m_txtopt[i].mm[j].st[k].cp[l].oab[0] = -1;
					m_txtopt[i].mm[j].st[k].cp[l].oab[1] = -1;
					m_txtopt[i].mm[j].st[k].cp[l].olast = -1;
					m_txtopt[i].mm[j].st[k].cp[l].ptover = -1;
					m_txtopt[i].mm[j].st[k].cp[l].theoretask = 0;
					m_txtopt[i].mm[j].st[k].cp[l].theoretbid = 0;
					m_txtopt[i].mm[j].st[k].cp[l].vol = -1;
				}
			}
		}
	}

}

CTTOptionsDataTable::~CTTOptionsDataTable()
{	
}
