/***************************************************************
 * Name:      wxDemoViewerApp.cpp
 * Purpose:   Implements application class
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "wxDemoViewerApp.h"
#include "wxDemoViewerMain.h"

#if !defined(__WXMSW__) && !defined(__WXPM__)
    #include "resource/appIcon.xpm"
#endif

IMPLEMENT_APP(wxDemoViewerApp);

bool wxDemoViewerApp::OnInit()
{
    wxString cwd;

    if(argc>0)
    {
        wxFileName filename(argv[0]);

        cwd = filename.GetPath();

        wxFileName::SetCwd(cwd);
    }

    wxDemoViewerFrame *frame = new wxDemoViewerFrame(NULL, wxT("wxDemoViewer"), cwd);
    frame->SetIcon(wxICON(appIcon));
    frame->Show();

    return true;
}
