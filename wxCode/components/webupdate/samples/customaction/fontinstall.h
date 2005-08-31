/////////////////////////////////////////////////////////////////////////////
// Name:        fontinstall.h
// Purpose:     wxWebUpdateActionFontInstall - an example of a custom action
// Author:      Francesco Montorsi
// Created:     2005/08/26
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_FONTINSTALL_H_
#define _WX_FONTINSTALL_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "fontinstall.h"
#endif

// defines the base class wxWebUpdateAction
#include "wx/stdactions.h"


// The "fontinstall" action.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateActionFontInstall : public wxWebUpdateAction
{
	DECLARE_WEBUPDATEACTION(wxWebUpdateActionFontInstall, wxT("fontinstall"))

protected:		// here go the declarations of the variables used by this action

	// The path & name of the font to install.
	wxString m_strFile;
};


#endif // _WX_FONTINSTALL_H_

