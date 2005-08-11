/////////////////////////////////////////////////////////////////////////////
// Name:        authdlg.h
// Purpose:     HTTP Authentication dialog for use in gui applications
//				
// Author:      Angelo Mandato
// Created:     2005/08/10
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Angelo Mandato (http://www.spaceblue.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_AUTHDLG_H_
#define _WX_AUTHDLG_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "authdlg.h"
#endif

#include <wx/dialog.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/intl.h>
#include "wx/httpenginedef.h"

class WXDLLIMPEXP_HTTPENGINE wxAuthenticateDlg : public wxDialog
{

public:

  // Properties
	wxString GetPassword(void);
	wxString GetUsername(void);
  bool GetRememberPassword(void);
	
	void SetPassword( const wxString &password );
	void SetUsername( const wxString &username );
  void SetRememberPassword( const bool &remember );

  wxAuthenticateDlg(wxWindow* parent, wxWindowID id, const wxString &caption = _("Authenticate"), const wxString &message = wxEmptyString, const bool &showRememberPasswd = false );
  virtual ~wxAuthenticateDlg();

private:

  wxTextCtrl        *m_tcUsername;
  wxTextCtrl        *m_tcPassword;
  wxCheckBox        *m_cbRememberPassword;
};

#endif
