/***************************************************************
 * Name:      TimePickerCtrlMain.h
 * Purpose:   Defines Application Frame
 * Author:    Paul Breen (greenbreen@yahoo.com)
 * Created:   2011-01-23
 * Copyright: Paul Breen
 * License:   wxWindows
 **************************************************************/

#ifndef TIMEPICKERCTRLMAIN_H
#define TIMEPICKERCTRLMAIN_H

#include "timepickerctrl.h"

//(*Headers(TimePickerCtrlFrame)
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

class TimePickerCtrlFrame: public wxFrame
{
    public:

        TimePickerCtrlFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~TimePickerCtrlFrame();

    private:

        //(*Handlers(TimePickerCtrlFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)
        void OnTimePicker(wxCommandEvent& event);

        //(*Identifiers(TimePickerCtrlFrame)
        static const long ID_CUSTOM1;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(TimePickerCtrlFrame)
        TimePickerCtrl* Custom1;
        wxStatusBar* StatusBar1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // TIMEPICKERCTRLMAIN_H
