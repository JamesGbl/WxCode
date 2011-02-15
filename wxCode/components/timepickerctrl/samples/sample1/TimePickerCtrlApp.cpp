/***************************************************************
 * Name:      TimePickerCtrlApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Paul Breen (greenbreen@yahoo.com)
 * Created:   2011-01-23
 * Copyright: Paul Breen
 * License:   wxWindows
 **************************************************************/

#include "wx_pch.h"
#include "TimePickerCtrlApp.h"

//(*AppHeaders
#include "TimePickerCtrlMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(TimePickerCtrlApp);

bool TimePickerCtrlApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    TimePickerCtrlFrame* Frame = new TimePickerCtrlFrame(0);
    Frame->Show();
    SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
