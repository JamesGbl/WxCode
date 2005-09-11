/////////////////////////////////////////////////////////////////////////////
// Name:        childwnd.h - wxResizeableControl with a child
// Purpose:     wxResizeableControl sample
// Author:      Markus Greither
// Modified by:
// Created:     
// RCS-ID:      $Id: childwnd.h,v 1.4 2005-09-11 18:06:11 magr Exp $
// Copyright:   (c) Markus Greither
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Begin single inclusion of this .h file condition
//-----------------------------------------------------------------------------

#ifndef _CHILDWND_H_
#define _CHILDWND_H_

//! Derived class from wxResizableControl that has a child
class ParentControl : public wxResizeableParentControl
{
   public:
    //! Ctor
    ParentControl() : wxResizeableParentControl() {}
    //! Ctor
    ParentControl(wxWindow *AParent, int AnId,
                 const wxPoint &pos,
                 const wxSize &size,long style = 0,
                 const wxString &name = wxPanelNameStr)
      : wxResizeableParentControl(AParent,AnId,pos,size,style,name) {}

// Event handlers
    //! Paint handler
    void OnPaint(wxPaintEvent &event);
    //! Print or screen output routine
    void Paint(wxDC &dc,bool,wxSize &size);

 protected:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(ParentControl)
};

#endif // _CHILDWND_H_

