//-----------------------------------------------------------------------------
// Name:        myframe.cpp
// Purpose:     resizeable controls sample: A derived frame, called MyFrame
// Author:      Markus Greither
// RCS-ID:      $Id: myframe.cpp,v 1.11 2005-11-21 20:53:39 frm Exp $
// Copyright:   (c) Markus Greither
// Licence:     wxWindows licence
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// GCC implementation
//-----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "myframe.h"
#endif

//-----------------------------------------------------------------------------
// Standard wxWindows headers
//-----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// For all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//-----------------------------------------------------------------------------
// Header of this .cpp file
//-----------------------------------------------------------------------------

#include "myframe.h"

//-----------------------------------------------------------------------------
// Remaining headers: Needed wx headers, then wx/contrib headers, then application headers
//-----------------------------------------------------------------------------

// Since setting an icon
#include "wx/image.h"
#include "wx/filename.h"
#include "wx/clipbrd.h"
#include "wx/print.h"
#include "wx/textctrl.h"
#include "myprintout.h"

#include "wx/url.h"
#include "wx/wfstream.h"

//-----------------------------------------------------------------------------
// Regular resources (the non-XRC kind).
//-----------------------------------------------------------------------------

// The application icon
// All non-MSW platforms use an xpm. MSW uses an .ico file
#if defined(__WXGTK__) || defined(__WXX11__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__)
    #include "mondrian.xpm"
#endif

//-----------------------------------------------------------------------------
// Resizeable controls
//-----------------------------------------------------------------------------

#include "wx/resizec.h"
#include "wx/textbox.h"

//-----------------------------------------------------------------------------
// User rendered resizeable controls
//-----------------------------------------------------------------------------

#include "ownerrnd.h"

//-----------------------------------------------------------------------------
// Resizeable controls with child text window
//-----------------------------------------------------------------------------


#include "childwnd.h"

//-----------------------------------------------------------------------------
// Event table: connect the events to the handler functions to process them
//-----------------------------------------------------------------------------


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_CLOSE,  MyFrame::OnQuit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    EVT_MENU(wxID_PASTE, MyFrame::OnPaste)
    EVT_UPDATE_UI(wxID_PASTE, MyFrame::OnPasteEnable)
    EVT_MENU(wxID_COPY, MyFrame::OnCopy)
    EVT_UPDATE_UI(wxID_COPY, MyFrame::OnCopyEnable)
    EVT_MENU(wxID_PRINT, MyFrame::OnPrint)
END_EVENT_TABLE()

//-----------------------------------------------------------------------------
// Public methods
//-----------------------------------------------------------------------------

void TestTextBox(wxResizeableControlCanvas *rc)
{
    wxResizeableParentControl *parent =
        new wxResizeableParentControl(rc,
                             0, wxPoint(300,250),
                             wxSize(450,200),
                             wxCLIP_SIBLINGS);

    wxTextBox *txt = new wxTextBox(parent, 0, wxDefaultPosition, wxSize(450, 200));
    parent->SetManagedChild(txt);

    txt->SetBackgroundColour(*wxWHITE);
    txt->InitDefaultStyles();

    wxString str1 = wxT("Hi !\nI'm a wxTextBox...\n\n");
    wxString str2 = wxT("This is the title of this text\n");
    wxString str3 = wxT("This is text styled with the default 'normal' ")
                    wxT("text style provided by wxTextBox.\nThere are ")
                    wxT("other various styles... see the docs for more info.");

    // we add the first string using a custom style
    txt->SetText(str1, wxTextStyle(wxTextAttr(*wxGREEN, wxNullColour,
                                    wxFont(14, wxFONTFAMILY_MODERN, wxFONTSTYLE_ITALIC,
                                    wxFONTWEIGHT_BOLD, FALSE, wxEmptyString)),
                                wxT("mystyle")));

    // the other strings will use some of the default styles
    txt->AppendText(str2, wxT("h1"));
    txt->AppendText(str3, wxT("normal"));

    // after all Set/AppendText calls we need to call Layout()
    txt->Layout();
    txt->SetCaretPos(wxTBCP_END);
    //txt->SetSelection(-1, -1);

#ifdef __WXDEBUG__
    // for debugging
    wxString content(txt->GetAllText());
    wxString inserted(str1 + str2 + str3);
    wxASSERT(content == inserted);
#endif
}

// Constructor
MyFrame::MyFrame(wxWindow* parent)
 : wxFrame(parent,-1,_T("Resizeable controls sample"),
           wxPoint(50, 50), wxSize(950, 640))
{
    // Set the icon for the frame.
    SetIcon(wxICON(appicon));

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_PRINT, _T("&Print preview"),
                     _T("Print preview/print active control"));
    menuFile->Append(wxID_CLOSE, _T("E&xit\tAlt-X"), _T("Quit this program"));

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, _T("&About...\tF1"), _T("Show about dialog"));

    wxMenu *menuEdit = new wxMenu;
    menuEdit->Append(wxID_PASTE, _T("&Paste"), _T("Paste new Control"));
    menuEdit->Append(wxID_COPY, _T("&Copy"), _T("Copy Control"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _T("&File"));
    menuBar->Append(menuEdit, _T("&Edit"));
    menuBar->Append(helpMenu, _T("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    CreateStatusBar(2);
    SetStatusText(_T("Welcome to wxWindows!"));
#endif // wxUSE_STATUSBAR

    // Create a canvas on which the resizeable controls are supported
    m_resizecanvas = new wxResizeableControlCanvas(this,-1);

    // Now create some resizeable controls
    wxBitmap bitmap;

    // get the path to our bitmap
    wxString sep(wxFileName::GetPathSeparator()), basepath = wxGetCwd();
    if (basepath.Last() != sep)
        basepath += sep;
    if (basepath.Right(6) == (wxT("build") + sep))  // are we running from MSVC6 ?
        basepath += wxString(wxT("..")) + sep + wxT("samples") + sep;

    if (!bitmap.LoadFile(basepath + wxT("builtwithwx.gif"),wxBITMAP_TYPE_GIF))
        wxMessageBox(_T("Couldn't load 'builtwithwx.gif' bitmap!"),
                     _T("Error"));

    // This one shows in the original size of the bitmap
    new wxBitmapControl((wxResizeableControlCanvas *)m_resizecanvas,
                        0,bitmap,wxPoint(10,10),
                        wxSize(bitmap.GetWidth(),bitmap.GetHeight()),
                       wxCLIP_SIBLINGS);
    // This one is enlarged and hanging half out of the canvas
    // Notice that the canvas scrollbars are set correcty
    // Try moving this around and see that the canvas scrollbars
    // are automatically updated
    new wxBitmapControl((wxResizeableControlCanvas *)m_resizecanvas,
                        0,bitmap,wxPoint(10,250),
                        wxSize(200,100),
                        wxCLIP_SIBLINGS);
    // This one has its own paint routine. Notice that the same routine
    // is used for both screen and printing
    new OwnerRenderedControl((wxResizeableControlCanvas *)m_resizecanvas,
                             0,wxPoint(10,100),
                             wxSize(250,200),
                             wxCLIP_SIBLINGS);

    // This has a child text editor. The sizing rectangles are shown
    // even if the child window has the focus, so that the user
    // does not need to switch between parent and child
    ParentControl *par = new ParentControl((wxResizeableControlCanvas *)m_resizecanvas,
                                           0,wxPoint(250,20),wxSize(200,200),wxCLIP_SIBLINGS);
    wxResizeableChildTextCtrl *ctrl = new wxResizeableChildTextCtrl(par,-1,_("text"));
    par->SetManagedChild(ctrl);

    //TestTextBox((wxResizeableControlCanvas *)m_resizecanvas);
}


//-----------------------------------------------------------------------------
// event handlers
//-----------------------------------------------------------------------------

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( _T("Sample application for resizeable controls.\n\n")
                _T("Demonstrates\n")
                _T("resizing, moving, cutting, pasting and printing\n")
                _T("of resizeable bitmap and custom controls.\n\n")
                _T("Build with %s"), wxVERSION_STRING);

    wxMessageBox(msg, _T("About RESIZECD"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnPaste(wxCommandEvent& WXUNUSED(event))
{
#ifdef __WXGTK__
    if (wxTheClipboard->IsOpened())
        return;
#endif

    if (wxTheClipboard->Open())
    {
        // If there is a bitmap in the clipboard, paste it into a new control
        if (wxTheClipboard->IsSupported(wxDF_BITMAP))
        {
            wxBitmapDataObject cdo;
            wxTheClipboard->GetData(cdo);
            wxSize sz(cdo.GetBitmap().GetWidth(),cdo.GetBitmap().GetHeight());
            wxBitmapControl *bmp = new wxBitmapControl((wxResizeableControlCanvas *)m_resizecanvas,
                                                       0,cdo.GetBitmap(),
                                                       wxPoint(10,10),sz,
                                                       wxCLIP_SIBLINGS);
#if wxCHECK_VERSION(2,5,0)
            bmp->ClearBackground();
#else
            bmp->Clear();
#endif
            bmp->SetFocus();
        }
        wxTheClipboard->Close();
    }
}

void MyFrame::OnPasteEnable(wxUpdateUIEvent& event)
{
#ifdef __WXGTK__
    if (wxTheClipboard->IsOpened())
        return;
#endif

    wxTheClipboard->Open();
    event.Enable(wxTheClipboard->IsSupported(wxDF_BITMAP));
    wxTheClipboard->Close();
}

void MyFrame::OnCopy(wxCommandEvent& event)
{
    wxPictureControl *bmp = wxDynamicCast(FindFocus(),wxPictureControl);
    if (bmp)
        bmp->OnEditCopy(event);
}

void MyFrame::OnCopyEnable(wxUpdateUIEvent& event)
{
    wxPictureControl *ctr = wxDynamicCast(FindFocus(),wxPictureControl);
    event.Enable(ctr != NULL);
}

void MyFrame::OnPrint(wxCommandEvent& WXUNUSED(event))
{
    // Create printouts for both previewing and printing
    MyPrintOut *printout = new MyPrintOut(_T("Test Print"),FindFocus());
    MyPrintOut *printout2 = new MyPrintOut(_T("Test Print"),FindFocus());

    // Create a print preview
    wxPrintPreview *preview = new wxPrintPreview(printout,printout2,(wxPrintData *)0);

    // Now create and show the preview in a preview frame
    wxPreviewFrame *frm = new wxPreviewFrame(preview,this,_T("Print preview"),
                                             wxPoint(10,10),wxSize(640,400));
    frm->Initialize();
    frm->Show();
}

//-----------------------------------------------------------------------------
// MyPrintOut for Resizeable Controls
//-----------------------------------------------------------------------------

bool MyPrintOut::OnPrintPage(int WXUNUSED(pagenum))
{
    // Get the device context
    wxDC *dc = GetDC();

    wxSize sz = dc->GetSize();

    // Print with border 1/10 of the page size
    dc->SetDeviceOrigin(sz.x/10,sz.y/10);
    sz.x = sz.x*8/10;
    sz.y = sz.y*8/10;

    // Make sure we have a wxResizeableControl
    wxResizeableControl *tmp = wxDynamicCast(m_Control,wxResizeableControl);
    if (tmp)
        tmp->Paint(*dc,true,sz);

    dc->SetDeviceOrigin(0,0);

    return true;
}

