//-----------------------------------------------------------------------------
// Name:        myprintout.h
// Purpose:     Resizeable controls sample: Printout for resizeable controls
// Author:      Markus Greither
// RCS-ID:
// Copyright:   (c) Markus Greither
// Licence:     wxWindows licence
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Begin single inclusion of this .h file condition
//-----------------------------------------------------------------------------

#ifndef _MYPRINTOUT_H_
#define _MYPRINTOUT_H_

//-----------------------------------------------------------------------------
// Class definition: MyPrintOut for printing resizeable controls
//-----------------------------------------------------------------------------

//! Class for printing resizeable controls
class MyPrintOut : public wxPrintout
{
  //! Window to print
  wxWindow *m_Control;
public:
    //! Ctor
    MyPrintOut(const wxString &title, wxWindow *ctrl)
     : wxPrintout(title) {m_Control = ctrl;}

    //! Page rendering
    bool OnPrintPage(int pagenum);
};


#endif // _MYPRINTOUT_H_
