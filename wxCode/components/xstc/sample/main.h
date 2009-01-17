
/*
 * main.h sample 1
 * 
 * this file is licensed under the wxWindows licence
 * just a quick reminder of what that means:
 * 
 * This software is released under the GNU GPL licence
 * with a few exceptins applied, check the wxWindows licence
 * to see what those are
 * 
 * visit: http://opensource.org/ to see both
 * the GNU GPL and wxWindows licences.
 * 
 * this software has absolutely no warranty, express or implied
 * 
 * just so you know, i don't care if you change the code
 * don't email me if you did someting to it.
 * 
 * no need to mark changes, you obviously may want to change the color
 * settings, it would only be a real pain if you had to mark them.
 * 
 * if you edit a function to change its behavior, it would be courtious
 * to others to let them know that the file is not an official release,
 * but you don't have to do that either.
 * 
 * XSTC was developed by Nuklear Zelph
 * copyright (C) 2006
 */

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/app.h>
#include <wx/frame.h>
#include <wx/window.h>
#include <wx/filedlg.h>
#include <wx/fileconf.h>
#include <wx/arrstr.h>

#ifdef XSTC_H_FLAT
  #include "xstc.h"
#else
  #include <wx/XSTC/xstc.h>
#endif

#define STYLE wxCAPTION | wxCLOSE_BOX | wxSYSTEM_MENU | wxRESIZE_BORDER

class MyApp : public wxApp
{
	public:
		virtual bool OnInit();
		int OnExit();
};

class MyFrame: public wxFrame
{
	public:
		MyFrame(wxWindow *parent,
        wxWindowID id = 1,
        const wxString& title = wxT("TextEditor"),
        const wxPoint& pos =wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = STYLE);
		~MyFrame();
	    XSTC *MyXSTC;
    enum
    {
       STC_ID = 1000
    };
	private:
            int flag;
        wxFileDialog *fileobj;
        void MiddleUP(wxMouseEvent& event);
		void OnQuit(wxCloseEvent& event);
		DECLARE_EVENT_TABLE()
};
