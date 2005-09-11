/////////////////////////////////////////////////////////////////////////////
// Name:        childwnd.h - wxResizeableControl with a child
// Purpose:     wxResizeableControl sample
// Author:      Markus Greither
// Modified by:
// Created:     
// RCS-ID:
// Copyright:   (c) Markus Greither
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Begin single inclusion of this .h file condition
//-----------------------------------------------------------------------------

#ifndef _CHILDWND_H_
#define _CHILDWND_H_

class CustomTextControl : public wxTextCtrl
{
   public:
    //! Ctor
    CustomTextControl() : wxTextCtrl() {}
    //! Ctor
    CustomTextControl(wxWindow *AParent, int AnId,const wxString &text,
                          const wxPoint &pos = wxDefaultPosition,
                          const wxSize &size = wxDefaultSize,long style = 0,
                          const wxString &name = wxPanelNameStr)
      : wxTextCtrl(AParent,AnId,text,pos,size,style)
    {
    }
    //! Enable parent sizers on set focus
    void OnSetFocus(wxFocusEvent &);
    //! Disable parent sizers on set focus
    void OnKillFocus(wxFocusEvent &);

    DECLARE_EVENT_TABLE()
};

//! Derived class from wxResizableControl that has a child
class ChildControl : public wxResizeableControl
{
     CustomTextControl *Editor;
   public:
    //! Ctor
    ChildControl() : wxResizeableControl() {Editor = 0;}
    //! Ctor
    ChildControl(wxWindow *AParent, int AnId,const wxPoint &pos,
                          const wxSize &size,long style = 0,
                          const wxString &name = wxPanelNameStr)
      : wxResizeableControl(AParent,AnId,pos,size,style,name)
    {
        Editor = new CustomTextControl(this,-1,_("Text"), wxDefaultPosition, 
						wxDefaultSize, wxTE_MULTILINE);
        wxSizeEvent evt(size);
        AddPendingEvent(evt);
    }

// Event handlers
    //! Paint handler
    void OnPaint(wxPaintEvent &event);

    //! Print or screen output routine
    void Paint(wxDC &dc,bool,wxSize &size);
    //! Responds to size event
    void OnSize(wxSizeEvent &event);

 protected:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(ChildControl)
};

#endif // _CHILDWND_H_

