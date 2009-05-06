/***************************************************************
 * Name:      wxDemoViewerApp.h
 * Purpose:   Defines application class
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef WXDEMOVIEWERAPP_H
#define WXDEMOVIEWERAPP_H

#include <wx/app.h>
#include <wx/filename.h>

/*! \brief Class represents the application itself. */
class wxDemoViewerApp : public wxApp
{
    public:
        /*! \brief Main entry point to application. */
        virtual bool OnInit();
};

#endif
