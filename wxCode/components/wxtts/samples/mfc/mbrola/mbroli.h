/*
 * FPMs-TCTS SOFTWARE LIBRARY
 *
 * File:    mbroli.cpp
 * Purpose: Mbroli cwinapp object
 * Author:  Alain Ruelle
 * Email :  mbrola@tcts.fpms.ac.be
 *
 * Copyright (c) 1997 Faculte Polytechnique de Mons (TCTS lab)
 * All rights reserved.
 * PERMISSION IS HEREBY DENIED TO USE, COPY, MODIFY, OR DISTRIBUTE THIS
 * SOFTWARE OR ITS DOCUMENTATION FOR ANY PURPOSE WITHOUT WRITTEN
 * AGREEMENT OR ROYALTY FEES.
 */

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#ifndef __MBROLI_H__
#define __MBROLI_H__

#define VER_MBROLI	"2.30"

/////////////////////////////////////////////////////////////////////////////
// CMbroliCommandLineInfo:
// An overrided class to parse the flags and parameters of the command line
// See mbroli.cpp for the implementation of this class
//
class CMbroliCommandLineInfo : public CCommandLineInfo
{
// Datas
public :
	CObList fileName;
	BOOL bMinimize;
	BOOL bClose;
	BOOL bNoShow;
	BOOL bNoTitle;

// Implementation
public:
	CMbroliCommandLineInfo();
protected:
	virtual void ParseParam(LPCTSTR lpszParam,BOOL bFlag,BOOL bLast);
	// Callback function to parse the custom flags and parameters
};
  
/////////////////////////////////////////////////////////////////////////////
// CMbroliApp:
// See mbroli.cpp for the implementation of this class
//
class CMbroliApp : public CWinApp
{
public:
	CMbroliCommandLineInfo cmdLine;

public:
	CMbroliApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMbroliApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMbroliApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

#endif