/////////////////////////////////////////////////////////////////////////////
// Name:        stdactions.cpp
// Purpose:     wxWebUpdateAction-derived classes
// Author:      Francesco Montorsi
// Created:     2005/07/30
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////




// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
    #include "wx/log.h"
    #include <wx/intl.h>        // for _() support
#endif

#include "wx/webupdate.h"
#include "wx/stdactions.h"
#include "wx/installer.h"
#include <wx/wfstream.h>
#include <wx/filesys.h>
#include <wx/utils.h>
#include <wx/archive.h>
#include <wx/zipstrm.h>
#include <wx/msgdlg.h>
#include <wx/app.h>
#include <wx/mimetype.h>


// wxWidgets RTTI
DEFINE_EVENT_TYPE(wxEVT_COMMAND_EXECUTE);

// default wxWebUpdate actions
IMPLEMENT_CLASS(wxWebUpdateActionRun, wxWebUpdateAction)
IMPLEMENT_CLASS(wxWebUpdateActionExtract, wxWebUpdateAction)
IMPLEMENT_CLASS(wxWebUpdateActionCopy, wxWebUpdateAction)
IMPLEMENT_CLASS(wxWebUpdateActionMkdir, wxWebUpdateAction)
IMPLEMENT_CLASS(wxWebUpdateActionMkfile, wxWebUpdateAction)
IMPLEMENT_CLASS(wxWebUpdateActionOpen, wxWebUpdateAction)

#include <wx/ptr_scpd.h>
wxDEFINE_SCOPED_PTR_TYPE(wxArchiveEntry);




// ---------------------
// wxWEBUPDATEACTIONRUN
// ---------------------

bool wxWebUpdateActionRun::Run() const
{
	// be sure that the file to run exists
	wxFileName f(m_strFile);
	if (!f.FileExists()) {

		wxLogUsrMsg(_("wxWebUpdateActionRun::Run - the file [%s] does not exist; proceeding anyway (maybe it's in PATH)"), m_strFile.c_str());

		// proceed: the executable could be in the system path...
	}

	int retcode = wxExecute(m_strFile + wxT(" ") + m_strArgs,
 							m_nExecFlag | wxEXEC_NODISABLE);

	// FIXME: how do we know if this retcode means success or not ?
	//        (some programs could not respect the 0=success UNIX standard...)
	if (retcode != 0)
		wxLogAdvMsg(wxT("wxWebUpdateActionRun::Run - got a non-null return code for ")
					wxT("the last command; proceeding anyway..."));
	return TRUE;
}

bool wxWebUpdateActionRun::SetProperties(const wxArrayString &propnames,
										const wxArrayString &propvalues)
{
	wxString flags;

	m_strArgs = wxEmptyString;			// the ARGS default value
	for (int i=0; i < (int)propnames.GetCount(); i++) {
		wxLogDebug(wxT("wxWebUpdateActionRun::SetProperties - name: [")
				+ propnames[i] + wxT("], value: [") + propvalues[i] + wxT("]"));

		if (propnames[i] == wxT("args"))
			m_strArgs = propvalues[i];
		else if (propnames[i] == wxT("file") || propnames[i] == wxT("cmd"))
			m_strFile = propvalues[i];
		else if (propnames[i] == wxT("flags"))
			flags = propvalues[i];
		else
			wxLogAdvMsg(wxT("wxWebUpdateActionRun::SetProperties - unknown property: ")
						+ propnames[i]);
	}

	// do substitutions on the paths
	m_strFile = wxWebUpdateInstaller::Get()->DoSubstitution(m_strFile);
	m_strArgs = wxWebUpdateInstaller::Get()->DoSubstitution(m_strArgs);

	// set defaults
	if (flags.IsEmpty())
		m_nExecFlag = wxEXEC_ASYNC;		// the FLAGS default value
	else if (flags == wxT("ASYNC"))
		m_nExecFlag = wxEXEC_ASYNC;
	else if (flags == wxT("SYNC"))
		m_nExecFlag = wxEXEC_SYNC;
	else {
		m_nExecFlag = wxEXEC_ASYNC;
		wxLogAdvMsg(wxT("wxWebUpdateActionRun::SetProperties - unknown exec flag: ")
						+ flags);
	}

	if (m_strFile.IsEmpty())
		m_strFile = wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("thisfile"));

	// validate the properties
	wxFileName f(m_strFile);			// the FILE property is required !

	// we won't do the wxFileName::FileExists check because the file we need to run
	// could be a file which does not exist yet (e.g. its in the update package)
	if (!f.IsOk())
		return FALSE;

	return TRUE;
}




// --------------------------
// wxWEBUPDATEACTIONEXTRACT
// --------------------------

bool wxWebUpdateActionExtract::Run() const
{
	wxArrayString orig, output;
	wxLogUsrMsg(_("wxWebUpdateActionExtract::Run - going to extract the file [%1$s] of type [%2$s] in [%3$s]"),
				m_strFile.c_str(), m_strType.c_str(), m_strWhere.c_str());

	// wxFileName wants a path separator at the end of directory names
	wxString dir(m_strWhere);
	if (dir.Last() != wxFileName::GetPathSeparator())
		dir += wxFileName::GetPathSeparator();

	// be sure that the destination directory exists
	wxFileName f(dir), f2(m_strFile);
	if (!f.DirExists() || !f2.FileExists()) {

		wxLogUsrMsg(_("wxWebUpdateActionExtract::Run - the folder [%1$s] or the file [%2$s] does not exist !"), m_strWhere.c_str(), m_strFile.c_str());
		return FALSE;
	}

	// parse the namemap
	wxArrayString compressed, extracted;
	int count = wxWebUpdateInstaller::Get()->ParsePairValueList(m_strNameMap, compressed, extracted);

	// do the substitutions also on the compressed filenames
	for (int i=0; i < count; i++)
		compressed[i] = wxWebUpdateInstaller::Get()->DoSubstitution(compressed[i]);

	// create the archive factory
	wxArchiveClassFactory *factory = NULL;
	if (m_strType == wxT("zip"))
		factory = new wxZipClassFactory;

	// extract the package
	wxArchiveEntryPtr entry;
	wxFFileInputStream input(m_strFile);
	wxArchiveInputStream *in = factory->NewStream(input);
	delete factory;

    while (entry.reset(in->GetNextEntry()), entry.get() != NULL)
    {
        // access meta-data
        wxString name = entry->GetName();

        // is this file/dir registered in the name map ?
        int idx = compressed.Index(name);
        if (idx != wxNOT_FOUND)
        	name = extracted[idx];
        wxString output = dir + name;

		// can be different from "dir" if "name" includes a relative path...
		wxString outputdir = wxFileName(output).GetPath();

		// intercept directories...
		if (!wxDirExists(outputdir)) {

			if (wxFileName(outputdir).Mkdir(0777, wxPATH_MKDIR_FULL))
				wxLogAdvMsg(wxT("wxWebUpdateActionExtract::Run - created the [") +
							outputdir + wxT("] folder"));
			else
				wxLogAdvMsg(wxT("wxWebUpdateActionExtract::Run - could not create the [") +
							outputdir + wxT("] folder.. proceeding anyway"));

			continue;		// this entry contains no data
		}

		// this is a file...
		wxLogUsrMsg(_("wxWebUpdateActionExtract::Run - extracting [%1$s] as [%2$s]..."),
                    name.c_str(), output.c_str());

        // now just dump this entry to a new uncompressed file...
		wxFileOutputStream out(output);
		if (!out.IsOk() || !out.Write(*in)) {

			wxLogUsrMsg(_("wxWebUpdateActionExtract::Run - couldn't decompress [%s]"), name.c_str());
			delete in;
			return FALSE;
		}
    }

	delete in;
	return TRUE;
}

bool wxWebUpdateActionExtract::SetProperties(const wxArrayString &propnames,
										const wxArrayString &propvalues)
{
	wxString flags;

	for (int i=0; i < (int)propnames.GetCount(); i++) {
		wxLogAdvMsg(wxT("wxWebUpdateActionExtract::SetProperties - name: [")
				+ propnames[i] + wxT("], value: [") + propvalues[i] + wxT("]"));

		if (propnames[i] == wxT("where"))
			m_strWhere = propvalues[i];
		else if (propnames[i] == wxT("file"))
			m_strFile = propvalues[i];
		else if (propnames[i] == wxT("type"))
			m_strType = propvalues[i];
		else if (propnames[i] == wxT("namemap"))
			m_strNameMap = propvalues[i];
		else
			wxLogAdvMsg(wxT("wxWebUpdateActionExtract::SetProperties - unknown property: ")
						+ propnames[i]);
	}

	// do substitutions on the paths
	m_strWhere = wxWebUpdateInstaller::Get()->DoSubstitution(m_strWhere);

	// by default, WebUpdater executables must be extracted with the '_'
	// character prepended
	wxString str = wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("updatername")) +
					wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("exe"));
	m_strNameMap = str + wxT("=_") + str + wxT(",") + m_strNameMap;

	// set defaults
	if (m_strFile.IsEmpty())		// the FILE default value is $(thisfile)
		m_strFile = wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("thisfile"));
	if (m_strWhere.IsEmpty())		// the WHERE default value is $(programdir)
		m_strWhere = wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("programdir"));
	if (m_strType.IsEmpty())		// the TYPE default value is "zip"
		m_strType = wxT("zip");

	// validate the properties
	wxFileName f(m_strWhere), f2(m_strFile);

	// we won't do the wxFileName::FileExists check because the file we need to run
	// could be a file which does not exist yet (e.g. its in the update package)
	//
	// NOTE: wxFileName::IsDir() only checks if the given string ends with a path
	//       separator character (there are no real ways to do a ::IsDir check
	//       without trying to access that path!) and thus we won't use it
	if (m_strWhere.IsEmpty() || m_strFile.IsEmpty() ||
		!f.IsOk() || !f2.IsOk())
		return FALSE;

	return TRUE;
}






// --------------------------
// wxWEBUPDATEACTIONCOPY
// --------------------------

bool wxWebUpdateActionCopy::Run() const
{
	wxArrayString orig, output;
	wxLogUsrMsg(_("wxWebUpdateActionCopy::Run - going to copy the file(s)/folder(s) [%1$s] in [%2$s]"),
				m_strFrom.c_str(), m_strTo.c_str());

	// wxFileName wants a path separator at the end of directory names
	wxString dir(m_strTo);
	if (dir.Last() != wxFileName::GetPathSeparator())
		dir += wxFileName::GetPathSeparator();

	// be sure that the destination directory exists
	wxFileName f(dir);
	if (!f.DirExists() && !m_bCreate) {

		wxLogAdvMsg(wxT("wxWebUpdateActionCopy::Run - the folder \"") + m_strTo +
				wxT("\" does not exist and create=0 !"));
		return FALSE;
	}

	// do we need to create the destination folder ?
	if (!f.DirExists()) {
		wxASSERT_MSG(m_bCreate,
			wxT("The create=0 case should have been already catched"));

		if (f.Mkdir(0777, wxPATH_MKDIR_FULL))
			wxLogAdvMsg(wxT("wxWebUpdateActionCopy::Run - created the [") +
							f.GetPath() + wxT("] folder"));
		else
			wxLogAdvMsg(wxT("wxWebUpdateActionCopy::Run - could not create the [") +
							f.GetPath() + wxT("] folder.. proceeding anyway"));
	}

	// do the copy
	// FIXME: how to handle the "m_bMove" flag ??
	if (!wxCopyFile(m_strFrom, m_strTo, m_bOverwrite))
		return FALSE;
	return TRUE;
}

bool wxWebUpdateActionCopy::SetProperties(const wxArrayString &propnames,
										const wxArrayString &propvalues)
{
	// set the defaults
	m_bMove = FALSE;
	m_bCreate = TRUE;
	m_bOverwrite = TRUE;

	for (int i=0; i < (int)propnames.GetCount(); i++) {
		wxLogAdvMsg(wxT("wxWebUpdateActionCopy::SetProperties - name: [")
				+ propnames[i] + wxT("], value: [") + propvalues[i] + wxT("]"));

		if (propnames[i] == wxT("from"))
			m_strFrom = propvalues[i];
		else if (propnames[i] == wxT("to"))
			m_strTo = propvalues[i];
		else if (propnames[i] == wxT("create"))
			m_bCreate = (propvalues[i] == wxT("1"));
		else if (propnames[i] == wxT("move"))
			m_bMove = (propvalues[i] == wxT("1"));
		else if (propnames[i] == wxT("overwrite"))
			m_bOverwrite = (propvalues[i] == wxT("1"));
		else
			wxLogAdvMsg(wxT("wxWebUpdateActionCopy::SetProperties - unknown property: ")
						+ propnames[i]);
	}

	// do substitutions on the paths
	m_strFrom = wxWebUpdateInstaller::Get()->DoSubstitution(m_strFrom);
	m_strTo = wxWebUpdateInstaller::Get()->DoSubstitution(m_strTo);

	// validate the properties
	wxFileName f(m_strFrom), f2(m_strTo);

	// we won't do the wxFileName::FileExists check because the file we need to run
	// could be a file which does not exist yet (e.g. its in the update package)
	//
	// NOTE: wxFileName::IsDir() only checks if the given string ends with a path
	//       separator character (there are no real ways to do a ::IsDir check
	//       without trying to access that path!) and thus we won't use it
	if (m_strFrom.IsEmpty() || m_strTo.IsEmpty() ||
		!f.IsOk() || !f2.IsOk())
		return FALSE;

	return TRUE;
}





// --------------------------
// wxWEBUPDATEACTIONMKDIR
// --------------------------

bool wxWebUpdateActionMkdir::Run() const
{
	wxArrayString orig, output;
	wxLogUsrMsg(_("wxWebUpdateActionMkdir::Run - going to make the folder [%s]"), m_strTarget.c_str());

	// wxFileName wants a path separator at the end of directory names
	wxString dir(m_strTarget);
	if (dir.Last() != wxFileName::GetPathSeparator())
		dir += wxFileName::GetPathSeparator();

	wxFileName f(dir);
	if (f.DirExists()) {

		wxLogAdvMsg(wxT("wxWebUpdateActionMkdir::Run - the folder \"") + m_strTarget +
			wxT("\" already exist... proceeding anyway"));
		return TRUE;
	}

	// create it !
	if (f.Mkdir(0777, wxPATH_MKDIR_FULL))
		wxLogDebug(wxT("wxWebUpdateActionMkdir::Run - created the [") +
		f.GetPath() + wxT("] folder"));
	else
		wxLogDebug(wxT("wxWebUpdateActionMkdir::Run - could not create the [") +
		f.GetPath() + wxT("] folder.. proceeding anyway"));

	return TRUE;
}

bool wxWebUpdateActionMkdir::SetProperties(const wxArrayString &propnames,
										const wxArrayString &propvalues)
{
	for (int i=0; i < (int)propnames.GetCount(); i++) {
		wxLogAdvMsg(wxT("wxWebUpdateActionMkdir::SetProperties - name: [")
				+ propnames[i] + wxT("], value: [") + propvalues[i] + wxT("]"));

		if (propnames[i] == wxT("dir"))
			m_strTarget = propvalues[i];
		else
			wxLogAdvMsg(wxT("wxWebUpdateActionMkdir::SetProperties - unknown property: ")
						+ propnames[i]);
	}

	// do substitutions on the paths
	m_strTarget = wxWebUpdateInstaller::Get()->DoSubstitution(m_strTarget);

	// validate the properties
	wxFileName f(m_strTarget);

	// we won't do the wxFileName::FileExists check because the file we need to run
	// could be a file which does not exist yet (e.g. its in the update package)
	//
	// NOTE: wxFileName::IsDir() only checks if the given string ends with a path
	//       separator character (there are no real ways to do a ::IsDir check
	//       without trying to access that path!) and thus we won't use it
	if (m_strTarget.IsEmpty() || !f.IsOk())
		return FALSE;

	return TRUE;
}




// --------------------------
// wxWEBUPDATEACTIONMKFILE
// --------------------------

bool wxWebUpdateActionMkfile::Run() const
{
	wxArrayString orig, output;
	wxLogUsrMsg(_("wxWebUpdateActionMkfile::Run - going to make the file [%s]"), m_strTarget.c_str());

	// do we have to create a folder ?
	wxFileName f(m_strTarget);
	if (f.FileExists()) {

		if (m_bOverwrite)
			wxLogAdvMsg(wxT("wxWebUpdateActionMkfile::Run - the file \"") + m_strTarget +
			wxT("\" already exist... proceeding anyway (overwrite=1)"));
		else
			return TRUE;		// exit
	}

	// create it !
	wxFileOutputStream out(f.GetFullPath());

	// do the encoding conversion
	wxCSConv converter(m_strEncoding);
	wxCharBuffer buf = m_strContent.mb_str(converter);
	const char *data = (const char*) buf;
	size_t bytes = strlen(data)*sizeof(char);

	// write
	if (out.Write(data, bytes).LastWrite() != bytes) {
		wxLogUsrMsg(_("wxWebUpdateActionMkfile::Run - could not create the [%s] file"),
			        f.GetFullPath().c_str());
		//wxDELETEA(data);
		return FALSE;
	}

	//wxDELETEA(data);
	wxLogUsrMsg(_("wxWebUpdateActionMkfile::Run - created the [%1$s] file with content [%2$s]..."),
                f.GetFullPath().c_str(), m_strContent.c_str());

	return TRUE;
}

bool wxWebUpdateActionMkfile::SetProperties(const wxArrayString &propnames,
										const wxArrayString &propvalues)
{
	// set defaults
	m_bOverwrite = TRUE;
	m_strEncoding = wxT("utf8");

	for (int i=0; i < (int)propnames.GetCount(); i++) {
		wxLogAdvMsg(wxT("wxWebUpdateActionMkfile::SetProperties - name: [")
				+ propnames[i] + wxT("], value: [") + propvalues[i] + wxT("]"));

		if (propnames[i] == wxT("file"))
			m_strTarget = propvalues[i];
		else if (propnames[i] == wxT("content"))
			m_strContent = propvalues[i];
		else if (propnames[i] == wxT("overwrite"))
			m_bOverwrite = (propvalues[i] == wxT("1"));
		else if (propnames[i] == wxT("encoding"))
			m_strEncoding = propvalues[i];
		else
			wxLogAdvMsg(wxT("wxWebUpdateActionMkfile::SetProperties - unknown property: ")
						+ propnames[i]);
	}

	// do substitutions on the paths
	m_strTarget = wxWebUpdateInstaller::Get()->DoSubstitution(m_strTarget);

	// validate the properties
	wxFileName f(m_strTarget);

	// we won't do the wxFileName::FileExists check because the file we need to run
	// could be a file which does not exist yet (e.g. its in the update package)
	//
	// NOTE: wxFileName::IsDir() only checks if the given string ends with a path
	//       separator character (there are no real ways to do a ::IsDir check
	//       without trying to access that path!) and thus we won't use it
	if (m_strTarget.IsEmpty() || !f.IsOk())
		return FALSE;

	return TRUE;
}




// --------------------------
// wxWEBUPDATEACTIONOPEN
// --------------------------

bool wxWebUpdateActionOpen::Run() const
{
	wxFileName f(m_strFile);
	wxLogUsrMsg(_("wxWebUpdateActionOpen::Run - opening the file [%s]"), m_strFile.c_str());

	if (!f.FileExists()) {

		wxLogUsrMsg(_("wxWebUpdateActionOpen::Run - the file [%s] does not exist !"), m_strFile.c_str());
		return FALSE;
	}

	// a little exception for Web pages: wxWidgets has the better function
	// wxLaunchDefaultBrowser which has a bteer error-checking and more
	// fallbacks, so use it if possible
    if (f.GetExt().StartsWith(wxT("htm")) && m_nExecFlag == wxEXEC_ASYNC)
    	return wxLaunchDefaultBrowser(m_strFile);

    // get the mime type
    wxFileType *ft;
	if (m_strMime.IsEmpty())
		if (f.GetExt().IsEmpty())
			return FALSE;		// how do we get the MIME type without extension ?
		else
		    ft = wxTheMimeTypesManager->GetFileTypeFromExtension(f.GetExt());
 	else
 		ft =  wxTheMimeTypesManager->GetFileTypeFromMimeType(m_strMime);

    if (!ft) {
        wxLogUsrMsg(_("wxWebUpdateActionOpen::Run - no default application can open the file [%s]"), m_strFile.c_str());
        return false;
    }

    wxString mt;
    ft->GetMimeType(&mt);

	// get the open command
    wxString cmd;
    bool ok = ft->GetOpenCommand(&cmd, wxFileType::MessageParameters(m_strFile));
    delete ft;

    if (!ok) {
	    wxLogUsrMsg(_("wxWebUpdateActionOpen::Run - cannot get the OPEN command for [%s]"), m_strFile.c_str());
    	return FALSE;
	}

    if (wxExecute (cmd, m_nExecFlag) == -1) {
        wxLogUsrMsg(_("wxWebUpdateActionOpen::Run - failed to launch application for [%s]"), m_strFile.c_str());
        return FALSE;
    }

	return TRUE;
}

bool wxWebUpdateActionOpen::SetProperties(const wxArrayString &propnames,
										const wxArrayString &propvalues)
{
	wxString flags;
	for (int i=0; i < (int)propnames.GetCount(); i++) {
		wxLogDebug(wxT("wxWebUpdateActionOpen::SetProperties - name: [")
				+ propnames[i] + wxT("], value: [") + propvalues[i] + wxT("]"));

		if (propnames[i] == wxT("file"))
			m_strFile = propvalues[i];
		else if (propnames[i] == wxT("mime"))
			m_strMime = propvalues[i];
		else if (propnames[i] == wxT("flags"))
			flags = propvalues[i];
		else
			wxLogAdvMsg(wxT("wxWebUpdateActionOpen::SetProperties - unknown property: ")
						+ propnames[i]);
	}

	// do substitutions on the paths
	m_strFile = wxWebUpdateInstaller::Get()->DoSubstitution(m_strFile);

	// set defaults
	if (flags.IsEmpty())
		m_nExecFlag = wxEXEC_ASYNC;		// the FLAGS default value
	else if (flags == wxT("ASYNC"))
		m_nExecFlag = wxEXEC_ASYNC;
	else if (flags == wxT("SYNC"))
		m_nExecFlag = wxEXEC_SYNC;
	else {
		m_nExecFlag = wxEXEC_ASYNC;
		wxLogAdvMsg(wxT("wxWebUpdateActionOpen::SetProperties - unknown exec flag: ")
						+ flags);
	}

	if (m_strFile.IsEmpty())
		m_strFile = wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("thisfile"));

	// validate the properties
	wxFileName f(m_strFile);			// the FILE property is required !

	// we won't do the wxFileName::FileExists check because the file we need to run
	// could be a file which does not exist yet (e.g. its in the update package)
	if (!f.IsOk())
		return FALSE;

	return TRUE;
}

