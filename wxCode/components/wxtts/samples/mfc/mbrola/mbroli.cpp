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
// mbroli.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "mbroli.h"
#include "mbroliDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMbroliCommandLineInfo

CMbroliCommandLineInfo::CMbroliCommandLineInfo()
{
	bNoShow=FALSE;
	bNoTitle=FALSE;
	bMinimize=FALSE;
	bClose=FALSE;
}

void CMbroliCommandLineInfo::ParseParam(LPCTSTR lpszParam,BOOL bFlag,BOOL bLast)
{
	if (!bFlag)
	// if the parsed param is a parameter
	{
		fileName.AddTail((CObject *) new CString(lpszParam));
	}
	else
	// otherwise it's a flag
	{
		CString cflag=lpszParam;

		cflag.MakeUpper();
		if ((cflag=="M") || (cflag=="MIN") || (cflag=="MINIMIZE"))
			bMinimize=TRUE;
		else if ((cflag=="E") || (cflag=="EXIT"))
			bClose=TRUE;
		else if ((cflag=="N") || (cflag=="NOSHOW"))
			bNoShow=TRUE;
		else if ((cflag=="T") || (cflag=="NOTITLE"))
			bNoTitle=TRUE;
		else
			CCommandLineInfo::ParseParam(lpszParam,bFlag,bLast);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMbroliApp

BEGIN_MESSAGE_MAP(CMbroliApp, CWinApp)
	//{{AFX_MSG_MAP(CMbroliApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMbroliApp construction

CMbroliApp::CMbroliApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMbroliApp object

CMbroliApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMbroliApp initialization

BOOL CMbroliApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
#ifdef _AFXDLL
		Enable3dControls();			// Call this when using MFC in a shared DLL
#else
		Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

		CMbroliDlg dlg;
		m_pMainWnd = &dlg;
		EnableShellOpen();
		//RegisterShellFileTypes();

		// Parse the command line parameters and flags
		ParseCommandLine(cmdLine);

		dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
