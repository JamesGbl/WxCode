/***************************************************************
 * Name:      TimePickerCtrlMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Paul Breen (greenbreen@yahoo.com)
 * Created:   2011-01-23
 * Copyright: Paul Breen
 * License:   wxWindows
 **************************************************************/

#include "wx_pch.h"
#include "TimePickerCtrlMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(TimePickerCtrlFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(TimePickerCtrlFrame)
const long TimePickerCtrlFrame::ID_CUSTOM1 = wxNewId();
const long TimePickerCtrlFrame::idMenuQuit = wxNewId();
const long TimePickerCtrlFrame::idMenuAbout = wxNewId();
const long TimePickerCtrlFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(TimePickerCtrlFrame,wxFrame)
    //(*EventTable(TimePickerCtrlFrame)
    EVT_TIMEPICKER(TimePickerCtrlFrame::OnTimePicker)
    //*)
END_EVENT_TABLE()

TimePickerCtrlFrame::TimePickerCtrlFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(TimePickerCtrlFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxBoxSizer* BoxSizer1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(131,27));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    Custom1 = new TimePickerCtrl(this,ID_CUSTOM1,wxEmptyString,wxDefaultPosition,wxSize(118,45),0);
    BoxSizer1->Add(Custom1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    BoxSizer1->SetSizeHints(this);

    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&TimePickerCtrlFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&TimePickerCtrlFrame::OnAbout);
    //*)
}

TimePickerCtrlFrame::~TimePickerCtrlFrame()
{
    //(*Destroy(TimePickerCtrlFrame)
    //*)
}

void TimePickerCtrlFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void TimePickerCtrlFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void TimePickerCtrlFrame::OnTimePicker(wxCommandEvent& event)
{
    // Add code here to handle events triggered by the TimePickerCtrl
    //wxMessageBox(_("In OnTimePicker"));
}
