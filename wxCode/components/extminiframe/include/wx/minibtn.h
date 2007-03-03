/////////////////////////////////////////////////////////////////////////////
// Name:        wx/minibtn.h
// Purpose:     wxCloseBoxBase, wxMaximizeBoxBase, wxMinimizeBoxBase, 
//              wxCollapseBoxBase
// Author:      Francesco Montorsi
// Created:     2004/03/03
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_MINIBTN_BASE_H_
#define _WX_MINIBTN_BASE_H_

// we need the definition of wxMiniButtonBase and wxMiniButton
// which are placed in palettefrm.h
#include "wx/palettefrm.h"


#ifdef wxEXTMINIFRM_USE_MINIBTN

// wxCloseBox is a window close button (the classical "x" in the
// top-right border of the window caption) which can be used in
// wxExtMiniFrames to hide the window.
class WXDLLIMPEXP_WXEXTMINIFRAME wxCloseBoxBase : public wxMiniButton
{
protected:
    void Draw(wxDC &, int, int);
    bool Exec();

public:
    wxCloseBoxBase() {}     // required by wxWidgets' RTTI system
    wxCloseBoxBase(wxExtMiniFrameBase *parent, int bSmall = -1) :
        wxMiniButton(parent, bSmall) {}
    virtual ~wxCloseBoxBase() {}
};


// A minibutton which has the same aspect of the standard
// wxMAXIMIZE_BOX; its behaviour is not defined.
// If you want to use it, derive a new class and override
// the Exec() function.
class WXDLLIMPEXP_WXEXTMINIFRAME wxMaximizeBoxBase : public wxMiniButton
{
protected:	
    void Draw(wxDC &, int, int);
    bool Exec();

public:
    wxMaximizeBoxBase() {}      // required by wxWidgets' RTTI system
    wxMaximizeBoxBase(wxExtMiniFrameBase *parent, int bSmall = -1) :
        wxMiniButton(parent, bSmall) {}
    virtual ~wxMaximizeBoxBase() {}
};


// A minibutton which has the same aspect of the standard
// wxMINIMIZE_BOX; its behaviour is not defined.
// If you want to use it, derive a new class and override
// the Exec() function.
class WXDLLIMPEXP_WXEXTMINIFRAME wxMinimizeBoxBase : public wxMiniButton
{
protected:	
    void Draw(wxDC &, int, int);
    bool Exec();

public:
    wxMinimizeBoxBase() {}      // required by wxWidgets' RTTI system
    wxMinimizeBoxBase(wxExtMiniFrameBase *parent, int bSmall = -1) :
        wxMiniButton(parent, bSmall) {}
    virtual ~wxMinimizeBoxBase() {}
};



// A wxMiniButton which allows the user to roll/unroll the 
// wxExtMiniFrame associated: roll/unroll is useful for
// those miniframes which are quite cumbersome.
//
// wxCollapseBox appears as a triangle pointing right when
// the window is rolled-up and pointing left when the window
// is unrolled...
//
class WXDLLIMPEXP_WXEXTMINIFRAME wxCollapseBoxBase : public wxMiniButton
{
protected:

    // no API in none OS can paint such a box; this is why we implement
    // this in the common version...
    void Draw(wxDC &, int, int);
    bool Exec();

public:
    wxCollapseBoxBase() {}      // required by wxWidgets' RTTI system
    wxCollapseBoxBase(wxExtMiniFrameBase *parent, int bSmall = -1) :
        wxMiniButton(parent, bSmall) {}
    virtual ~wxCollapseBoxBase() {}


    // Returns TRUE if the parent window is rolled up.
    bool isParentRolled() const;
};



// ----------------------
// wxMiniButton events
// ----------------------

BEGIN_DECLARE_EVENT_TYPES()

    // I hope those IDs are not used in your app...
    DECLARE_EXPORTED_EVENT_TYPE( WXDLLIMPEXP_WXEXTMINIFRAME,
                                 wxEVT_COMMAND_CLOSEBOX_CLICKED, wxID_HIGHEST + 1000 )
    DECLARE_EXPORTED_EVENT_TYPE( WXDLLIMPEXP_WXEXTMINIFRAME,
                                 wxEVT_COMMAND_MAXIMIZEBOX_CLICKED, wxID_HIGHEST + 1001 )
    DECLARE_EXPORTED_EVENT_TYPE( WXDLLIMPEXP_WXEXTMINIFRAME,
                                 wxEVT_COMMAND_MINIMIZEBOX_CLICKED, wxID_HIGHEST + 1002 )
    DECLARE_EXPORTED_EVENT_TYPE( WXDLLIMPEXP_WXEXTMINIFRAME,
                                 wxEVT_COMMAND_COLLAPSEBOX_CLICKED, wxID_HIGHEST + 1003 )

END_DECLARE_EVENT_TYPES()



#define EVT_CLOSEBOX_CLICKED(winid, fn)										\
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_COMMAND_CLOSEBOX_CLICKED, winid,		\
        wxID_ANY, (wxObjectEventFunction) (wxEventFunction)					\
        wxStaticCastEvent( wxCommandEventFunction, &fn ),					\
        (wxObject *) NULL ),

#define EVT_MAXIMIZEBOX_CLICKED(winid, fn)									\
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_COMMAND_MAXIMIZEBOX_CLICKED, winid,	\
        wxID_ANY, (wxObjectEventFunction) (wxEventFunction)					\
        wxStaticCastEvent( wxCommandEventFunction, &fn ),					\
        (wxObject *) NULL ),

#define EVT_MINIMIZEBOX_CLICKED(winid, fn)									\
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_COMMAND_MINIMIZEBOX_CLICKED, winid,	\
        wxID_ANY, (wxObjectEventFunction) (wxEventFunction)					\
        wxStaticCastEvent( wxCommandEventFunction, &fn ),					\
        (wxObject *) NULL ),

#define EVT_COLLAPSEBOX_CLICKED(winid, fn)									\
    DECLARE_EVENT_TABLE_ENTRY( wxEVT_COMMAND_COLLAPSEBOX_CLICKED, winid,	\
        wxID_ANY, (wxObjectEventFunction) (wxEventFunction)					\
        wxStaticCastEvent( wxCommandEventFunction, &fn ),					\
        (wxObject *) NULL ),

#endif      // wxEXTMINIFRM_USE_MINIBTN


#if defined(__WXMSW__)

    // Win32 can use some specific APIs to draw the wxMiniButton-derived classes
    #include "wx/msw/minibtn.h"

#elif defined(__WXGTK__)

    // GTK specific minibutton handling system
    #include "wx/gtk/minibtn.h"

#else

    // the implementation defined above is good enough for wxGTK and wxX11
    #define wxCloseBox      wxCloseBoxBase
    #define wxMinimizeBox   wxMinimizeBoxBase
    #define wxMaximizeBox   wxMaximizeBoxBase
    #define wxCollapseBox   wxCollapseBoxBase

#endif


#endif      // _WX_MINIBTN_BASE_H_


