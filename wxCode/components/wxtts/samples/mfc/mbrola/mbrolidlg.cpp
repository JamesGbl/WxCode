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
// mbroliDlg.cpp : implementation file
//

#include "stdafx.h"
#include "apiext.h"
#include "mbroli.h"
#include "OpenDialog.h"
#include "mbroliDlg.h"
#include "tts/wxmbrola.h"
#include "tts/mbrplay.h"
#include "version.h"

/*#ifdef _DEBUG00
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/*
BOOL regfn(LPCTSTR lpszDatabase, DWORD dwUserData)
{
	CMbroliDlg* pDlg=(CMbroliDlg*)dwUserData;
	pDlg->ComboLang.AddString(lpszDatabase);
	return true;
}
*/

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg(int id);

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_title;
	CString	m_mbrolaversion;
	CString	m_info;
	CString	m_label;
	CString	m_path;
	CString	m_author;
	CString	m_copyright;
	//}}AFX_DATA
			
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int dbid;
	CVersionInfo version;

	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg(int id) : CDialog(CAboutDlg::IDD), dbid(id)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_title = "";
	m_mbrolaversion = _T("");
	m_info = _T("");
	m_label = "";
	m_path = "";
	m_author = _T("");
	m_copyright = _T("");
	//}}AFX_DATA_INIT
	// Get Application Title
	version.GetVersionInfo(AfxGetInstanceHandle(),"040904B0","ProductName");
	m_title=version.m_strVersionInfo+" version ";
	// Version extraction
	version.SetVersionToMajMin(1);
	m_title+=version.m_strVersionInfo;
	// Get Application Author
	version.GetVersionInfo(AfxGetInstanceHandle(),"040904B0","Comments");
	m_author=version.m_strVersionInfo;
	version.GetVersionInfo(AfxGetInstanceHandle(),"040904B0","LegalCopyright");
	m_copyright=version.m_strVersionInfo;
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	wxMbrola mbrola;
	m_mbrolaversion="Mbrola Synthesizer version ";
	m_mbrolaversion+=mbrola.GetVersion();
	m_info = mbrola.GetDatabaseAllInfo();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_STATICTITLE, m_title);
	DDX_Text(pDX, IDC_MBROLAVERSION, m_mbrolaversion);
	DDX_Text(pDX, IDC_INFO, m_info);
	DDX_Text(pDX, IDC_LABEL, m_label);
	DDX_Text(pDX, IDC_PATH, m_path);
	DDX_Text(pDX, IDC_AUTHOR, m_author);
	DDX_Text(pDX, IDC_COPYRIGHT, m_copyright);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMbroliDlg dialog

CMbroliDlg::CMbroliDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMbroliDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMbroliDlg)
	Line==_T("");
	EditLength = 1.0f;
	EditPitch = 1.0f;
	EditVoice = _FREQ_DEFAULT;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_LOGO);

	selbeg=-1;
	selend=0;
	errors=0;
	fileList=NULL;
	maxList=0;
	pitchMin=20;
	pitchMax=800;
	timeMin=20;
	timeMax=800;
	voiceMin=_FREQ_DEFAULT_MIN;
	voiceMax=_FREQ_DEFAULT_MAX;
}

void CMbroliDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMbroliDlg)
	DDX_Control(pDX, IDC_EDIT1, EditControl);
	DDX_Control(pDX, IDC_SPINVOICE, m_spinvoice);
	DDX_Control(pDX, IDC_EDITVOICE, m_EditVoiceCtrl);
	DDX_Text(pDX,IDC_EDIT1,Line);	
	DDX_Control(pDX, IDC_COMBOLANG, ComboLang);
	DDX_Control(pDX, IDC_SPINLENGTH, m_spinlength);
	DDX_Control(pDX, IDC_SPINPITCH, m_spinpitch);
	DDX_Control(pDX, IDC_STOP, m_stop);
	DDX_Control(pDX, IDC_EDITPITCH, m_EditPitchCtrl);
	DDX_Control(pDX, IDC_EDITLENGTH, m_EditLengthCtrl);
	DDX_Control(pDX, IDLOAD, m_load);
	DDX_Control(pDX, IDOK, m_speak);
	DDX_Text(pDX, IDC_EDITLENGTH, EditLength);
	DDX_Text(pDX, IDC_EDITPITCH, EditPitch);
	DDX_Text(pDX, IDC_EDITVOICE, EditVoice);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMbroliDlg, CDialog)
	//{{AFX_MSG_MAP(CMbroliDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK,OnSpeak)
	ON_BN_CLICKED(IDLOAD,OnLoad)
	ON_CBN_SELCHANGE(IDC_COMBOLANG, OnSelchangeCombolang)
	ON_WM_DESTROY()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_WM_VSCROLL()
	ON_EN_KILLFOCUS(IDC_EDITPITCH, OnKillfocusEditpitch)
	ON_EN_KILLFOCUS(IDC_EDITLENGTH, OnKillfocusEditlength)
	ON_EN_KILLFOCUS(IDC_EDITVOICE, OnKillfocusEditvoice)
	ON_BN_CLICKED(IDABOUT, OnAbout)
	ON_WM_LBUTTONDBLCLK()
	ON_EN_SETFOCUS(IDC_EDIT1, OnSetfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnKillfocusEdit1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMbroliDlg message handlers

BOOL CMbroliDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	CWnd::DragAcceptFiles();

	if (!m_mbrola.Init())
	{
		EndDialog(-1);
		return TRUE;
	}

	pitchMin = (int) (m_mbrola.GetMinPitch() * 100);
	pitchMax = (int) (m_mbrola.GetMaxPitch() * 100);

	timeMin = (int) (m_mbrola.GetMinDuration() * 100);
	timeMax = (int) (m_mbrola.GetMaxDuration() * 100);

	voiceMin = (int) (m_mbrola.GetMinFreq());
	voiceMax = (int) (m_mbrola.GetMaxFreq());

	EditControl.SetLimitText(UINT_MAX);
	ComboLang.SetCurSel(base);
	// spin control initialization
	UDACCEL accel[6]={{0,1},{1,25},{2,50},{3,75},{4,100},{5,200}};
	UDACCEL vaccel[6]={{0,100},{1,250},{2,500},{3,750},{4,1000},{5,2000}};
	m_spinpitch.SetRange(pitchMin,pitchMax);
	m_spinpitch.SetAccel(6,accel);
	if (pitchMin<100)
		m_spinpitch.SetPos(100);
	else
		m_spinpitch.SetPos(pitchMin);
	m_spinlength.SetRange(timeMin,timeMax);
	m_spinlength.SetAccel(6,accel);
	if (timeMin<100)
		m_spinlength.SetPos(100);
	else
		m_spinlength.SetPos(timeMin);
	m_spinvoice.SetRange(voiceMin,voiceMax);
	m_spinvoice.SetAccel(6,vaccel);

	// Load the list of databases into the combo
	CString dbStr;
	int noDatabases = m_mbrola.GetDatabaseCount();
	int i;
	for (i = 0; i < noDatabases; i++)
	{
		m_mbrola.GetDatabaseId(i, dbStr);
		ComboLang.AddString(dbStr);
	}

	// Load a default database
	if (!InitDataBase(m_mbrola.GetDefaultDatabase()))
	{
		EndDialog(-1);
		return TRUE;
	}	

#if 0
	// Read the command line parameters
	if (pCmdLine->bMinimize)
	{
		ShowWindow(SW_SHOWMINIMIZED);
	}
	if (pCmdLine->fileName.GetCount()>0)
	{
		POSITION pos;
		CString *pString;
		
		maxList=pCmdLine->fileName.GetCount();
		fileList=(char **) malloc(maxList*sizeof(char *));
		pos=pCmdLine->fileName.GetHeadPosition();
		for(i=0;i<maxList;i++)
		{
			pString=(CString *) pCmdLine->fileName.GetNext(pos);
			*(fileList+i)=(char *) malloc((pString->GetLength()+1)*sizeof(char));
			strcpy(*(fileList+i),LPCTSTR(*pString));
			delete pString;
		}
		pCmdLine->fileName.RemoveAll();
		if ((!pCmdLine->bMinimize) && (!pCmdLine->bNoShow))
		{
			ShowWindow(SW_SHOW);
		}
		OnSpeak();
		if (pCmdLine->bClose)
		{
			EndDialog(0);
		}
	}
#endif

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CMbroliDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMbroliDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Speak Button Click
void CMbroliDlg::OnSpeak() 
{
	int ml=0;
	int ret=0;

	isSpeakRunning=1;
	ModifyStyleEx(WS_EX_ACCEPTFILES,NULL,SWP_NOSIZE | SWP_NOMOVE);
	m_load.EnableWindow(FALSE);
	m_speak.EnableWindow(FALSE);
	m_stop.EnableWindow();
	ComboLang.EnableWindow(FALSE);
	m_EditLengthCtrl.EnableWindow(FALSE);
	m_EditPitchCtrl.EnableWindow(FALSE);
	m_EditVoiceCtrl.EnableWindow(FALSE);
	UpdateData(TRUE);
	m_mbrola.SetPitch(EditPitch);
	m_mbrola.SetDuration(EditLength);
	m_mbrola.SetFreq(EditVoice);
	if (maxList>0)
	{
		while ((ml<maxList) && (isSpeakRunning) && (!ret))
		{
			EditControl.SetRedraw(FALSE);
			OnLoadFile(*(fileList+ml));
			EditControl.SetRedraw(TRUE);
			UpdateData(TRUE);
			ret=m_mbrola.Play(Line, TRUE);
			++ml;
		}
		for(long j=0;j<maxList;j++)
			if (*(fileList+j))
			{
				free(*(fileList+j));
				*(fileList+j)=NULL;
			}
		free(fileList);
		fileList=NULL;
		maxList=0;
	}
	else
	{
		UpdateData(TRUE);
		if ((selbeg>=0) && (selbeg!=selend))
		{
			ret=m_mbrola.Play(CString("_ 100 0 120\n")+Line.Mid(selbeg,selend-selbeg+1)+"\n_ 100 0 120\n", TRUE);
		}
		else
		{
			ret=m_mbrola.Play(Line, TRUE);
		}
	}
	if (ret<0)
	{
		AfxMessageBox(m_mbrola.GetLastError(),MB_ICONHAND);
	}
	isSpeakRunning=0;
	m_stop.EnableWindow(FALSE);
	m_speak.EnableWindow();
	m_load.EnableWindow();
	ComboLang.EnableWindow();
	m_EditPitchCtrl.EnableWindow();
	m_EditLengthCtrl.EnableWindow();
	m_EditVoiceCtrl.EnableWindow();
	ModifyStyleEx(NULL,WS_EX_ACCEPTFILES,SWP_NOSIZE | SWP_NOMOVE);
}

void CMbroliDlg::OnLoad() 
{
	// TODO: Add extra validation here
	static char BASED_CODE szFilter[] = "Pho Files (*.pho)|*.pho||";

	if (strlen(sampPath)>0)
		SetCurrentDirectory(sampPath);
	
	COpenDialog OplDial(base,NULL,"*.pho", OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
	if (OplDial.DoModal()==IDOK)
	{
		CString buf;
		POSITION pos=OplDial.GetStartPosition();
		int i=0;

		for(;pos;i++) OplDial.GetNextPathName(pos);

		if (i>1)
		{
			pos=OplDial.GetStartPosition();
			while(pos)
			{
				buf=OplDial.GetNextPathName(pos);
				if (maxList==0)
					strcpy(sampPath,LPCTSTR(buf.Left(buf.ReverseFind('\\'))));
				fileList=(char **) realloc(fileList,(maxList+1)*sizeof(char*));
				*(fileList+maxList)=(char *) malloc((buf.GetLength()+1)*sizeof(char));
				strcpy(*(fileList+maxList),LPCTSTR(buf));
				maxList++;
			}
		}
		else
		{
			pos=OplDial.GetStartPosition();
			buf=OplDial.GetNextPathName(pos);
			strcpy(sampPath,LPCTSTR(buf.Left(buf.ReverseFind('\\'))));
		}
		if (base!=OplDial.GetDatabase())
		{
			base=OplDial.GetDatabase();
			ComboLang.SetCurSel(base);
			CString str;
			m_mbrola.GetDatabaseId(base, str);
			InitDataBase(str);
		}
		if (i>1)
			OnSpeak();
		else
			OnLoadFile(buf);
	}
}

void CMbroliDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CMbroliDlg::OnSelchangeCombolang() 
{
	// TODO: Add your control notification handler code here
	CString str;
	m_mbrola.GetDatabaseId(ComboLang.GetCurSel(), str);
	if (!InitDataBase(str))
		if(!InitDataBase(m_mbrola.GetDefaultDatabase()))
			EndDialog(-1);
		else
			ComboLang.SetCurSel(base);
	else
		base=ComboLang.GetCurSel();
}

void CMbroliDlg::OnDestroy() 
{
	int i;
	if (m_mbrola.IsPlaying())
		m_mbrola.Stop();
	if ((errors==5) || (errors==0))
	{
		if (fileList)
		{
			for(i=0;i<maxList;i++)
				if (*(fileList+i))
					free(*(fileList+i));
			free(fileList);
		}
	}
	CDialog::OnDestroy();
}

void CMbroliDlg::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	int sz;
	int i;
	
	maxList=DragQueryFile(hDropInfo,0xFFFFFFFF,NULL,0); // Query the number of file path dragged
	fileList=(char **) malloc(maxList*sizeof(char *));
	for(i=0;i<maxList;i++)
	{
		sz=DragQueryFile(hDropInfo,i,NULL,0); // Query the size of the first dropped path name

		*(fileList+i)=(char *) malloc((sz+2)*sizeof(char));
		DragQueryFile(hDropInfo,i,*(fileList+i),sz+1); // Get the path name
	}
	OnSpeak();
}

void CMbroliDlg::OnLoadFile(CString path,BOOL update)
{
	FILE *fin;
	CString buff;

	Line="";
	if (fin=fopen(path,"rb"))
	{
		EditControl.SetRedraw(FALSE);
		fseek(fin,0,SEEK_END);
		long l=ftell(fin);
		fseek(fin,0,SEEK_SET);
		long *ppos=NULL;
		char c;
		int nbr=0;
		bool bcr=false;
		while (!feof(fin))
		{
			c=fgetc(fin);
			if (c=='\n')
			{
				if (!bcr)
				{
					ppos=(long*)realloc(ppos,(++nbr)*sizeof(long));
					ppos[nbr-1]=ftell(fin)-1;
				}
				else
					bcr=false;
			}
			else if (c=='\r')
				bcr=true;
			else
				bcr=false;
		}
		char *txt=Line.GetBuffer(l+nbr+3);
		long sz,pos=0;
		fseek(fin,0,SEEK_SET);
		for(int i=0;i<nbr;i++)
		{
			sz=ppos[i]-ftell(fin);
			if (sz)
				fread(txt+pos,sizeof(char),sz,fin);
			pos+=sz;
			*(txt+pos)='\r';
			*(txt+pos+1)='\n';
			pos+=2;
			fgetc(fin);
		}
		if (ftell(fin)<l)
		{
			sz=l-ftell(fin);
			fread(txt+pos,sizeof(char),l-ftell(fin),fin);
			pos+=sz;
			*(txt+pos)='\r';
			*(txt+pos+1)='\n';
			pos+=2;
		}
		*(txt+pos)=0;
		Line.ReleaseBuffer();
		selbeg=-1;
		selend=0;
		if (update)
			UpdateData(FALSE);
		fclose(fin);
		CMbroliCommandLineInfo* pCmdLine=(CMbroliCommandLineInfo *) &(((CMbroliApp *) AfxGetApp())->cmdLine);
		if (!pCmdLine->bNoTitle)
		{
			buff=path.Mid(path.ReverseFind('\\')+1);
			buff+=" - ";
			path.LoadString(IDS_WINDOWTITLE);
			buff+=path;
			SetWindowText(buff);
		}
		EditControl.SetRedraw(TRUE);
		if (ppos)
			free(ppos);
	}
	else
	{
		char Message[400];
		sprintf(Message,"Error :\n   Cannot open the \n %s file.",LPCTSTR(path));
		MessageBeep(MB_ICONHAND);
		MessageBox(Message,AfxGetApp()->m_pszAppName);
	}
}

BOOL CMbroliDlg::InitDataBase(const CString& dbId)
{
	if (m_mbrola.SetCurrentDatabase(dbId))
	{
//		ComboLang.SetCurSel(n);
		EditVoice=m_mbrola.GetDefaultFreq();
		voiceBase=EditVoice;
		m_spinvoice.SetPos(EditVoice);
		UpdateData(FALSE);
		return TRUE;
	}
	else
	{
		MessageBeep(MB_ICONHAND);
		MessageBox(m_mbrola.GetLastError(),AfxGetApp()->m_pszAppName,MB_ICONSTOP);
		return FALSE;
	}
}

void CMbroliDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	if (m_mbrola.IsPlaying())
		m_mbrola.Stop();
	isSpeakRunning=0;
}

void CMbroliDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	EditPitch=(float) (LOWORD(m_spinpitch.GetPos())/100.00f);
	EditLength=(float) (LOWORD(m_spinlength.GetPos())/100.00f);
	EditVoice=LOWORD(m_spinvoice.GetPos());
	UpdateData(FALSE);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMbroliDlg::OnKillfocusEditpitch() 
{
	// TODO: Add your control notification handler code here
	int mn,mx;

	UpdateData(TRUE);
	
	m_spinpitch.GetRange(mn,mx);

	EditPitch=min(EditPitch,mx/100.00f);
	EditPitch=max(EditPitch,mn/100.00f);
	m_spinpitch.SetPos((int)(EditPitch*100));

	UpdateData(FALSE);
}

void CMbroliDlg::OnKillfocusEditlength() 
{
	// TODO: Add your control notification handler code here
	int mn,mx;

	UpdateData(TRUE);
	
	m_spinlength.GetRange(mn,mx);

	EditLength=min(EditLength,mx/100.00f);
	EditLength=max(EditLength,mn/100.00f);
	m_spinlength.SetPos((int)(EditLength*100));

	UpdateData(FALSE);
}

void CMbroliDlg::OnKillfocusEditvoice() 
{
	// TODO: Add your control notification handler code here
	int mn,mx;
	
	UpdateData(TRUE);
	
	m_spinvoice.GetRange(mn,mx);
	EditVoice=min(EditVoice,mx);
	EditVoice=max(EditVoice,mn);
	m_spinvoice.SetPos(EditVoice);
	
	UpdateData(FALSE);
}

void CMbroliDlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlgAbout(base);
	dlgAbout.DoModal();
}

void CMbroliDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	CPoint pt(point);
	EditControl.GetClientRect(&rect);
	EditControl.ClientToScreen(&rect);
	ClientToScreen(&pt);
	if (rect.PtInRect(pt))
	{
		int ln=HIWORD(EditControl.CharFromPos(pt));
		int pos=EditControl.LineIndex(ln);
		EditControl.SetSel(pos,pos+EditControl.LineLength(ln));
	}
	else
		CDialog::OnLButtonDblClk(nFlags, point);
}

void CMbroliDlg::OnSetfocusEdit1() 
{
	// TODO: Add your control notification handler code here
	EditControl.SetSel(selbeg,selend);
}

void CMbroliDlg::OnKillfocusEdit1() 
{
	// TODO: Add your control notification handler code here
	EditControl.GetSel(selbeg,selend);
	if ((selbeg>=0) && (selbeg!=selend))
	{
		int pos;
		selbeg=EditControl.LineIndex(EditControl.LineFromChar(selbeg));
		pos=EditControl.LineFromChar(selend-1);
		selend=EditControl.LineIndex(pos)+EditControl.LineLength(selend-1)-1;
	}
}
