//-----------------------------------------------------------------------------
// Name:        myframe.h
// Purpose:     Resizeable controls sample: A derived frame, called MyFrame
// Author:      Markus Greither
// RCS-ID:      $Id: myframe.h,v 1.2 2005-09-11 15:25:31 frm Exp $
// Copyright:   (c) Markus Greither
// Licence:     wxWindows licence
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Begin single inclusion of this .h file condition
//-----------------------------------------------------------------------------

#ifndef _MYFRAME_H_
#define _MYFRAME_H_

//-----------------------------------------------------------------------------
// GCC interface
//-----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma interface "myframe.h"
#endif

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------

#include "wx/frame.h"

//-----------------------------------------------------------------------------
// Class definition: MyFrame
//-----------------------------------------------------------------------------

//! Main frame class for demo program
class MyFrame : public wxFrame
{
    //! Client window pointer (canvas for controls)
    wxWindow *m_resizecanvas;
public:

    //! ctor(s)
    MyFrame(wxWindow *wnd = (wxWindow *)NULL);

private:

    // Event handlers (these functions should _not_ be virtual)
    //! Show about dialog
    void OnAbout(wxCommandEvent& event);
    //! Quit program
    void OnQuit(wxCommandEvent& event);
    //! Paste clipboard graphics into canvas
    void OnPaste(wxCommandEvent& event);
    //! Enable/Disable paste event
    void OnPasteEnable(wxUpdateUIEvent& event);
    //! Copy current control content into clipboard
    void OnCopy(wxCommandEvent& event);
    //! Enable/Disable copy event
    void OnCopyEnable(wxUpdateUIEvent& event);
    //! Print current control content
    void OnPrint(wxCommandEvent& event);

    // Any class wishing to process wxWindows events must use this macro
    //! Event table
    DECLARE_EVENT_TABLE()

};

//-----------------------------------------------------------------------------
// End single inclusion of this .h file condition
//-----------------------------------------------------------------------------

#endif  // _MYFRAME_H_
