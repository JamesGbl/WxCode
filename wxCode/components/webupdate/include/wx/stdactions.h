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


//! Declares a new wxWebUpdateAction-derived class which handles tags
//! with the given string name.
#define DECLARE_WEBUPDATEACTION(classname, string)				\
public:															\
    classname()	: wxWebUpdateAction(string) {}					\
    virtual ~classname() {}										\
																\
public:															\
	/* Sets the property names & values for this action. */		\
	virtual bool SetProperties(const wxArrayString &propnames,	\
							const wxArrayString &propvalues);	\
																\
	/* Run this action. */										\
	virtual bool Run() const;									\
																\
	/* Returns a copy of this action. */						\
	virtual wxWebUpdateAction *Clone() const					\
		{ return new classname(*this); }						\
																\
	DECLARE_CLASS(classname)



//! The "run" action.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateActionRun : public wxWebUpdateAction
{
	DECLARE_WEBUPDATEACTION(wxWebUpdateActionRun, wxT("run"))

protected:

	//! The arguments to give to the program which is run.
	wxString m_strArgs;

	//! The path & name of the executable file to run.
	wxString m_strFile;

	//! The run flags: wxEXEC_ASYNC, wxEXEC_SYNC, wxEXEC_NOHIDE.
	int m_nExecFlag;
};


//! The "extract" action.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateActionExtract : public wxWebUpdateAction
{
	DECLARE_WEBUPDATEACTION(wxWebUpdateActionExtract, wxT("extract"))

protected:

	//! The place where the contents of the file will be extracted.
	wxString m_strWhere;

	//! The file to extract.
	wxString m_strFile;

	//! The type of archive.
	wxString m_strType;
	
	//! The name map for naming the extracted files.
	wxString m_strNameMap;
};


//! The "copy" action; can be used also as "move" action.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateActionCopy : public wxWebUpdateAction
{
	DECLARE_WEBUPDATEACTION(wxWebUpdateActionCopy, wxT("copy"))

protected:

	//! The original file(s)/folder(s).
	wxString m_strFrom;

	//! The destination folder.
	wxString m_strTo;

	//! If TRUE this action will act as "move" instead of "copy".
	bool m_bMove;

	//! If TRUE this action will create the destination folder, if it does not exist.
	bool m_bCreate;

	//! If TRUE this action will overwrite the destination files, if they exist.
	bool m_bOverwrite;
};


//! The "make" action.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateActionMake : public wxWebUpdateAction
{
	DECLARE_WEBUPDATEACTION(wxWebUpdateActionMake, wxT("make"))

protected:

	//! The new folder/file to create.
	wxString m_strTarget;

	//! The content of the new file to create.
	wxString m_strContent;

	//! If TRUE then the <make> tag used the "dir" property and not the "file" one.
	bool m_bDir;

	//! If TRUE the file will be overwritten if it already exists.
	//! Has sense only when m_bDir == FALSE.
	bool m_bOverwrite;
};


#endif // _WX_STDACTIONS_H_

