/////////////////////////////////////////////////////////////////////////////
// Name:        ownerrnd.h - Owner rendered resizeable control
// Purpose:     wxResizeableControl sample
// Author:      Markus Greither
// Modified by:
// Created:     
// RCS-ID:      $Id: ownerrnd.h,v 1.2 2005-09-11 15:25:31 frm Exp $
// Copyright:   (c) Markus Greither
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Begin single inclusion of this .h file condition
//-----------------------------------------------------------------------------

#ifndef _OWNERRND_H_
#define _OWNERRND_H_

//! Derived class from wxResizableControl that paints itself
class OwnerRenderedControl : public wxResizeableControl
{
   public:
    //! Ctor
    OwnerRenderedControl() : wxResizeableControl() {}
    //! Ctor
    OwnerRenderedControl(wxWindow *AParent, int AnId,const wxPoint &pos,
                          const wxSize &size,long style = 0,
                          const wxString &name = wxPanelNameStr)
      : wxResizeableControl(AParent,AnId,pos,size,style,name) {}

// Event handlers
    //! Paint handler
    void OnPaint(wxPaintEvent &event);

    //! Print or screen output routine
    void Paint(wxDC &dc,bool,wxSize &size);

 protected:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(OwnerRenderedControl)
};

#endif // _OWNERRND_H_

