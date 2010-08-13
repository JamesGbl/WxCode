
/*
 * main.cpp sample 1
 * ______________________________________________________________
 * This software is dual licensed under either the GNU GPL
 * version 2 or at your option any other version
 * ______________________________________________________________
 * or the wxWindows license.
 * Just a quick reminder of what that means:
 *
 * This software is released under the GNU GPL licence
 * with a few exceptins applied, check the wxWindows licence
 * to see what those are.
 * ______________________________________________________________
 *
 * Visit: http://opensource.org/ to see both-
 * the GNU GPL and wxWindows licences.
 *
 * This software has absolutely no warranty, express or implied.
 *
 * Just so you know, I don't care if you change the code
 * don't email me if you did someting to it.
 *
 * If you edit a function to change its behavior, it would be courtious
 * to others to let them know that the file is not an official release,
 * but you don't have to do that either.
 *
 * You must not misrepresent the origins of this software, if you distribute
 * it, let the user know where to get it and that you where not the original
 * creator. (Except for any code you add obviously)
 *
 * This notice may not be changed in any way and must remain at the top of every
 * source file.
 *
 * XSTC was developed by Nuklear Zelph
 * copyright (C) 2006
 */

#include "main.h"
IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame(NULL);
    frame->SetSize(wxSize(600,300));
    frame->Show();
    return true;
}

int MyApp::OnExit()
{
	return 0;
}


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_CLOSE(MyFrame::OnQuit)
    //EVT_MIDDLE_DCLICK(MyFrame::MiddleUP)
END_EVENT_TABLE()

MyFrame::MyFrame(wxWindow *parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos,
        const wxSize& size,
        long style)
    : wxFrame(parent, id, title, pos, size, style)
{
    MyXSTC = new XSTC(this, STC_ID, pos, size, 0, title);
    wxFileConfig* conf = MyXSTC->SetColorConf(true, wxT("test"), wxT("NZ"), wxT("xstc.tpx"), wxT("xstcG.tpx"));
    //wxRegConfig* conf = MyXSTC->SetColorConf("XSTCsample","oss","XSTCfile");
    conf->Exists(wxT("_"));//keeps away a warning
    MyXSTC->SetMgnsWidth(50,16,16);
#ifndef XSTC_NO_KEYS
    MyXSTC->usekeys = true;
    MyXSTC->Keys_Set.cpp1 = wxT("wxKeywords.txt");
#endif
    //MyXSTC->DarkStyle();
    //MyXSTC->BorlandStyle();
    MyXSTC->VisualStudioStyle();
    //MyXSTC->ClassicStyle();
    //MyXSTC->JeffStyle();
    //MyXSTC->ZenburnStyle();
    MyXSTC->markoutline=true;
    MyXSTC->FoldCircle();

#if wxMAJOR_VERSION <= 2
#if wxMINOR_VERSION <= 6
    fileobj = new wxFileDialog(this, wxT("Which file to read?"), wxT(""), wxT(""), wxT("*.*"), wxOPEN | wxCHANGE_DIR, wxDefaultPosition);//add file must exsist?
#else
    fileobj = new wxFileDialog(this, wxT("Which file to read?"), wxT(""), wxT(""), wxT("*.*"), wxFD_OPEN | wxFD_CHANGE_DIR | wxFD_FILE_MUST_EXIST, wxDefaultPosition);
#endif
#endif

    if(fileobj->ShowModal() == wxID_OK)
         //MyXSTC->LoadFileX(wxT("text.cxx"),true);
         MyXSTC->LoadFileX(fileobj->GetPath(), true);
    else
         Close();
}
MyFrame::~MyFrame()
{
	  wxDELETE(fileobj)
}

void MyFrame::OnQuit(wxCloseEvent& event)
{
   Close();
}

void MyFrame::MiddleUP(wxMouseEvent& event)
{
	if(fileobj->ShowModal() == wxID_OK)
         //MyXSTC->LoadFileX(wxT("text.cxx"),true);
         MyXSTC->LoadFileX(fileobj->GetPath(), true);
}
