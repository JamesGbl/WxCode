// SimpleTTS.h : main header file for the SIMPLETTS application
//

#if !defined(AFX_SIMPLETTS_H__C9C2FAA9_CE27_11D1_8369_444553540000__INCLUDED_)
#define AFX_SIMPLETTS_H__C9C2FAA9_CE27_11D1_8369_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSimpleTTSApp:
// See SimpleTTS.cpp for the implementation of this class
//

class CSimpleTTSApp : public CWinApp
{
public:
	CSimpleTTSApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimpleTTSApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSimpleTTSApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLETTS_H__C9C2FAA9_CE27_11D1_8369_444553540000__INCLUDED_)
