/////////////////////////////////////////////////////////////////////////////
// Name:        stdactions.h
// Purpose:     wxWebUpdateAction-derived classes
// Author:      Francesco Montorsi
// Created:     2005/07/30
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_STDACTIONS_H_
#define _WX_STDACTIONS_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "stdactions.h"
#endif

// wxWidgets headers
#include "wx/webupdatedef.h"		// for WXDLLIMPEXP_WEBUPDATE macros only
#include "wx/webupdate.h"


// defined later
class WXDLLIMPEXP_WEBUPDATE wxDownloadThread;
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateDownload;
class WXDLLIMPEXP_WEBUPDATE wxWebUpdatePackage;
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateLocalPackageArray;
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateDlg;

extern WXDLLIMPEXP_DATA_WEBUPDATE(wxWebUpdateDownload) wxEmptyWebUpdateDownload;


//! The "run" action.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateActionRun : public wxWebUpdateAction
{
protected:

	//! The arguments to give to the program which is run.
	wxString m_strArgs;

	//! The path & name of the executable file to run.
	wxString m_strFile;

	//! The run flags: wxEXEC_ASYNC, wxEXEC_SYNC, wxEXEC_NOHIDE.
	int m_nExecFlag;

public:
    wxWebUpdateActionRun()
         : wxWebUpdateAction(wxT("run")) {}
    virtual ~wxWebUpdateActionRun() {}

public:

	//! Sets the property names & values for this action.
	virtual bool SetProperties(const wxArrayString &propnames,
							const wxArrayString &propvalues);

	//! Run this action.
	virtual bool Run() const;

	//! Returns a copy of this action.
	virtual wxWebUpdateAction *Clone() const
		{ return new wxWebUpdateActionRun(*this); }

private:
	DECLARE_CLASS(wxWebUpdateActionRun)
};


//! The "extract" action.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateActionExtract : public wxWebUpdateAction
{
protected:

	//! The place where the contents of the file will be extracted.
	wxString m_strWhere;

	//! The file to extract.
	wxString m_strFile;

	//! The type of archive.
	wxString m_strType;
	
	//! The name map for naming the extracted files.
	wxString m_strNameMap;

public:
    wxWebUpdateActionExtract()
         : wxWebUpdateAction(wxT("extract")) {}
    virtual ~wxWebUpdateActionExtract() {}

public:

	//! Sets the property names & values for this action.
	virtual bool SetProperties(const wxArrayString &propnames,
							const wxArrayString &propvalues);

	//! Run this action.
	virtual bool Run() const;

	//! Returns a copy of this action.
	virtual wxWebUpdateAction *Clone() const
		{ return new wxWebUpdateActionExtract(*this); }

private:
	DECLARE_CLASS(wxWebUpdateActionExtract)
};

#endif // _WX_STDACTIONS_H_

