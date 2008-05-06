/////////////////////////////////////////////////////////////////////////////
// Name:        wxActiveRecordGuiApp.h
// Purpose:     
// Author:      Matías Szeftel
// Modified by: 
// Created:     Fri 09 Feb 2007 00:58:40 ART
// RCS-ID:      
// Copyright:   (c) 2006 Matías Szeftel <mszeftel@yahoo.com.ar>
// Licence:     GPL (GNU Public License)
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXACTIVERECORDGUIAPP_H_
#define _WXACTIVERECORDGUIAPP_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "wxActiveRecordGuiApp.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "wxActiveRecordGeneratorFrm.h"
////@end includes

#include "DatabaseConnectionDlg.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * WxActiveRecordGuiApp class declaration
 */

class WxActiveRecordGuiApp: public wxApp
{    
    DECLARE_CLASS( WxActiveRecordGuiApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    WxActiveRecordGuiApp();

    /// Initialises member variables
    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin WxActiveRecordGuiApp event handler declarations
////@end WxActiveRecordGuiApp event handler declarations

////@begin WxActiveRecordGuiApp member function declarations
////@end WxActiveRecordGuiApp member function declarations

////@begin WxActiveRecordGuiApp member variables
////@end WxActiveRecordGuiApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(WxActiveRecordGuiApp)
////@end declare app

#endif
    // _WXACTIVERECORDGUIAPP_H_
