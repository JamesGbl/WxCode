/////////////////////////////////////////////////////////////////////////////
// Name:        app.h
// Purpose:     wxColourPicker sample app header
// Author:      Juan Antonio Ortega
// Created:     2006/06/17
// RCS-ID:      $Id: app.h,v 1.1 2006-06-27 18:10:54 ja_ortega Exp $
// Copyright:   (c) 2006 Juan Antonio Ortega
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef APP_H
#define APP_H

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class App : public wxApp{
public:
    virtual bool OnInit();
    virtual int OnExit();
};

DECLARE_APP(App)

#endif //APP
