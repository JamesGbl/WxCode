
/*
 * main.h sample 1
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
