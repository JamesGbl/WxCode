/////////////////////////////////////////////////////////////////////////////
// Name:        MyPicsApp.h
// Purpose:     
// Author:      Ernesto Rangel Dallet
// Modified by: 
// Created:     12/04/05 19:21:11
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _MYPCISAPP_H_
#define _MYPCISAPP_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "MyPicsApp.cpp"
#endif


#include "wx/image.h"

// the wxMSWResources class
#include "wx/wxmswres.h"

class MyPicsApp: public wxApp
{    
    DECLARE_CLASS( MyPicsApp )
    DECLARE_EVENT_TABLE()

public:
    wxMSWResources m_Resources;
    wxLocale m_Locale;

    MyPicsApp();
    virtual bool OnInit();
    virtual int OnExit();

};

DECLARE_APP(MyPicsApp)

#endif  // _MYPCISAPP_H_
