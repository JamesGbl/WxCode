//---------------------------------------------------------------------------
//
// Name:        wxOpenCommanderApp.cpp
// Author:      Armando Urdiales González
// Created:     06/12/2006 12:53:20
// Description:
//
//---------------------------------------------------------------------------

#include "wxOpenCommanderApp.h"
#include "wxOpenCommanderFrm.h"

IMPLEMENT_APP(wxOpenCommanderFrmApp)

bool wxOpenCommanderFrmApp::OnInit()
{
    wxOpenCommanderFrm* frame = new wxOpenCommanderFrm(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}

int wxOpenCommanderFrmApp::OnExit()
{
	return 0;
}
