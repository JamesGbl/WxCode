/*
 * FPMs-TCTS SOFTWARE LIBRARY
 *
 * File:    mbrolidlg.cpp
 * Purpose: Mbroli Main Window & About Box
 * Author:  Alain Ruelle
 * Email :  mbrola@tcts.fpms.ac.be
 *
 * Copyright (c) 1997 Faculte Polytechnique de Mons (TCTS lab)
 * All rights reserved.
 * PERMISSION IS HEREBY DENIED TO USE, COPY, MODIFY, OR DISTRIBUTE THIS
 * SOFTWARE OR ITS DOCUMENTATION FOR ANY PURPOSE WITHOUT WRITTEN
 * AGREEMENT OR ROYALTY FEES.
 */

#ifndef __MBROLIDLG_H__
#define __MBROLIDLG_H__

#include "tts/wxmbrola.h"

// Maximum size of a database label
#define _MAX_BUFFSIZE		1024
#define _FREQ_DEFAULT		16000
#define _FREQ_DEFAULT_MAX	32000
#define _FREQ_DEFAULT_MIN	8000

/////////////////////////////////////////////////////////////////////////////
// CMbroliDlg dialog

class CMbroliDlg : public CDialog
{
	friend class wxMbrola;
// Construction
public:
	CMbroliDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMbroliDlg)
	enum { IDD = IDD_MBROLI_DIALOG };
	CEdit	EditControl;
	CSpinButtonCtrl	m_spinvoice;
	CEdit	m_EditVoiceCtrl;
	CString Line;
	CComboBox ComboLang;
	CSpinButtonCtrl	m_spinlength;
	CSpinButtonCtrl	m_spinpitch;
	CButton	m_stop;
	CEdit	m_EditPitchCtrl;
	CEdit	m_EditLengthCtrl;
	CButton	m_load;
	CButton	m_speak;
	float	EditLength;
	float	EditPitch;
	int		EditVoice;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMbroliDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	int isSpeakRunning;
	short errors;
	char sampPath[_MAX_PATH];	// starting path for loading .pho
	char **fileList;			// List of files dragged and dropped in the window
	int maxList;				// Number of files dropped in the window
	int base;					// Index of the current selected database
	int pitchMin,pitchMax;		// min & max pitch ratio (for the spinner)
	int timeMin,timeMax;		// min & max time ratio (for the spinner)
	int voiceMin,voiceMax;		// min & max voice frequency (for the spinner)
	int voiceBase;				// Base Frequency of the database
	int selbeg,selend;

	wxMbrola	m_mbrola;
public:
	void OnSpeakThread();
	// Speak all the phonemes, in a thread
	void OnLoadFile(CString path,BOOL update=TRUE);
	BOOL InitDataBase(const CString& dbId);
	// Initialize a database
	// if initialization fails, reload the previous one
	// if it still fails, exit program
	void SetState(int newState,int cond);
	// Change the state of the speak thread

	// Generated message map functions
	//{{AFX_MSG(CMbroliDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnSpeak();
	virtual void OnLoad();
	virtual void OnCancel();
	afx_msg void OnSelchangeCombolang();
	afx_msg void OnDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnStop();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKillfocusEditpitch();
	afx_msg void OnKillfocusEditlength();
	afx_msg void OnKillfocusEditvoice();
	afx_msg void OnAbout();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSetfocusEdit1();
	afx_msg void OnKillfocusEdit1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
