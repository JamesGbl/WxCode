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

#ifndef __OPENDIALOG_H__
#define __OPENDIALOG_H__

/////////////////////////////////////////////////////////////////////////////
// COpenDialog dialog

class COpenDialog : public CFileDialog
{
// Construction
public:
	COpenDialog(int bd,LPCTSTR lpszDefExt=NULL,LPCTSTR lpszFileName=NULL,DWORD dwFlags=OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,LPCTSTR lpszFilter=NULL,CWnd* pParentWnd=NULL);
	// standard constructor

// Dialog Data
	//{{AFX_DATA(COpenDialog)
	enum { IDD = IDD_OPEN_DIALOG };
	CComboBox	m_database;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int nItem;
	char **pBase;
	int baseDefault;

public:
	int GetDatabase()
		{ return baseDefault; }

	// Generated message map functions
	//{{AFX_MSG(COpenDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDatabase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif