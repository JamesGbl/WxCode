/////////////////////////////////////////////////////////////////////////////
// Name:        installer.h
// Purpose:     wxBaseInstaller and wxZIPInstaller
// Author:      Francesco Montorsi
// Created:     2005/07/14
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_INSTALLER_H_
#define _WX_INSTALLER_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "installer.h"
#endif

// wxWidgets headers
#include "wx/webupdatedef.h"		// for WXDLLIMPEXP_WEBUPDATE macros only
#include "wx/url.h"


//! .
class WXDLLIMPEXP_WEBUPDATE wxBaseInstaller : public wxObject
{
protected:

public:
	wxBaseInstaller() 
		{  }
	virtual ~wxBaseInstaller() {}


private:
	DECLARE_CLASS(wxBaseInstaller)
};

#endif // _WX_INSTALLER_H_

