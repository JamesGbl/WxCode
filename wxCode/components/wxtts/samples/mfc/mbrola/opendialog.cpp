/*
 * FPMs-TCTS SOFTWARE LIBRARY
 *
 * File:    opendialog.cpp
 * Purpose: Mbroli's Custom Open Dialog (with database choice)
 * Author:  Alain Ruelle
 * Email :  mbrola@tcts.fpms.ac.be
 *
 * Copyright (c) 1997 Faculte Polytechnique de Mons (TCTS lab)
 * All rights reserved.
 * PERMISSION IS HEREBY DENIED TO USE, COPY, MODIFY, OR DISTRIBUTE THIS
 * SOFTWARE OR ITS DOCUMENTATION FOR ANY PURPOSE WITHOUT WRITTEN
 * AGREEMENT OR R     OYALTY FEES.
 */
// OpenDialog.cpp : implementation file
//

#include "stdafx.h"
#include "mbroli.h"
#include "tts/mbrplay.h"
#include "OpenDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL openregfn(LPCTSTR lpszLabel, DWORD dwData)
{
	COpenDialog* pDlg=(COpenDialog*) dwData;
	pDlg->m_database.AddString(lpszLabel);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// COpenDialog dialog
COpenDialog::COpenDialog(int bd,LPCTSTR lpszDefExt,LPCTSTR lpszFileName,DWORD dwFlags,LPCTSTR lpszFilter,CWnd* pParentWnd)
	 : CFileDialog(TRUE,lpszDefExt,lpszFileName,dwFlags,lpszFilter,pParentWnd)
{
    m_ofn.Flags |= OFN_ENABLETEMPLATE;
	m_ofn.lpTemplateName=MAKEINTRESOURCE(IDD_OPEN_DIALOG);
	baseDefault=bd;
}

void COpenDialog::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenDialog)
	DDX_Control(pDX, IDC_DATABASE, m_database);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenDialog, CFileDialog)
	//{{AFX_MSG_MAP(COpenDialog)
	ON_CBN_SELCHANGE(IDC_DATABASE, OnSelchangeDatabase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenDialog message handlers

BOOL COpenDialog::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	MBR_RegEnumDatabaseCallback(openregfn,(DWORD)this);
	m_database.SetCurSel(baseDefault);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COpenDialog::OnSelchangeDatabase() 
{
	// TODO: Add your control notification handler code here
	baseDefault=m_database.GetCurSel();
}
