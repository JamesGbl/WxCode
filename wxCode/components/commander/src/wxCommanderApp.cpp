//---------------------------------------------------------------------------
//
// Name:        wxCommanderApp.cpp
// Author:      Armando Urdiales González
// Created:     06/12/2006 12:53:20
// Description:
//
//---------------------------------------------------------------------------

#include "wxCommanderApp.h"
#include "wxCommanderFrm.h"

IMPLEMENT_APP(wxCommanderFrmApp)

bool wxCommanderFrmApp::OnInit()
{
    wxCommanderFrm* frame = new wxCommanderFrm(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}

int wxCommanderFrmApp::OnExit()
{
	return 0;
}
