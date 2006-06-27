/////////////////////////////////////////////////////////////////////////////
// Name:        app.cpp
// Purpose:     wxColourPicker sample app
// Author:      Juan Antonio Ortega
// Created:     2006/06/17
// RCS-ID:      $Id: app.cpp,v 1.1 2006-06-27 18:10:54 ja_ortega Exp $
// Copyright:   (c) 2006 Juan Antonio Ortega
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "app.h"
#include "mainframe.h"

IMPLEMENT_APP(App)

bool App::OnInit(){
    MainFrame *frame = new MainFrame(NULL);
    frame->Show(true);

    return true;
}

int App::OnExit(){
	return wxApp::OnExit();
}
