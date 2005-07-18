/////////////////////////////////////////////////////////////////////////////
// Name:        installer.cpp
// Purpose:     wxBaseInstaller and wxZIPInstaller
// Author:      Francesco Montorsi
// Created:     2005/06/23
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
	#pragma implementation "installer.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
#include "wx/log.h"
#endif

#include "wx/installer.h"
#include "wx/wfstream.h"
#include "wx/filesys.h"


// wxWidgets RTTI
IMPLEMENT_CLASS(wxWebUpdateAction, wxObject)
IMPLEMENT_CLASS(wxWebUpdateInstaller, wxObject)


#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_USER_EXPORTED_OBJARRAY(wxWebUpdateActionArray);




// ---------------------
// wxWEBUPDATEACTION
// ---------------------


