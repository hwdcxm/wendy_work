// TTOptionsDataTable.h: interface for the CTTOptionsDataTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TTOPTIONSDATATABLE_H__83B08D18_5CE4_11D3_BE07_00A0CC23E61B__INCLUDED_)
#define AFX_TTOPTIONSDATATABLE_H__83B08D18_5CE4_11D3_BE07_00A0CC23E61B__INCLUDED_

#include "OptionsDataStruct.h"
#define TT_OP_ITEMNUM  6  //the number of the Items

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTTOptionsDataTable  
{
public:
	CTTOptionsDataTable();
	g_txtoptS m_txtopt[TT_OP_ITEMNUM];

	virtual ~CTTOptionsDataTable();
		

};

#endif // !defined(AFX_TTOPTIONSDATATABLE_H__83B08D18_5CE4_11D3_BE07_00A0CC23E61B__INCLUDED_)
