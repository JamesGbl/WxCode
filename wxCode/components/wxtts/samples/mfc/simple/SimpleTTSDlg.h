// SimpleTTSDlg.h : header file
//

#if !defined(AFX_SIMPLETTSDLG_H__C9C2FAAB_CE27_11D1_8369_444553540000__INCLUDED_)
#define AFX_SIMPLETTSDLG_H__C9C2FAAB_CE27_11D1_8369_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "tts/freephone.h"
#include "tts/wxmbrola.h"

/////////////////////////////////////////////////////////////////////////////
// CSimpleTTSDlg dialog

class CSimpleTTSDlg : public CDialog
{
// Construction
public:
	CSimpleTTSDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSimpleTTSDlg)
	enum { IDD = IDD_SIMPLETTS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimpleTTSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON			m_hIcon;
	wxFreePhone     m_freephone;
	wxMbrola		m_mbrola;

	// Generated message map functions
	//{{AFX_MSG(CSimpleTTSDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSpeak();
	afx_msg void OnSaveToFile();
	afx_msg void OnSaveToWaveFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLETTSDLG_H__C9C2FAAB_CE27_11D1_8369_444553540000__INCLUDED_)
