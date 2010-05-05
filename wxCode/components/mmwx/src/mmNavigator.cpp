//
// Name     : mmNavigator.cpp
// Purpose  : mmNavigator methods.
//            mmNavigator is a 'navigation toolbar', i.e. a window with
//            'previous' and 'next' buttons. The user may supply an
//            optional window to be placed between the navigator buttons.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#include "wx/mmNavigator.h"

#include "../bitmaps/prev_dbl_btn.xpm"
#include "../bitmaps/prev_btn.xpm"
#include "../bitmaps/next_btn.xpm"
#include "../bitmaps/next_dbl_btn.xpm"

#ifdef __MMDEBUG__
extern wxTextCtrl *gDebug; // For global debug output
#endif

static wxBitmap *gPrevDblBitmap    = NULL;
static wxBitmap *gPrevBitmap       = NULL;
static wxBitmap *gNextBitmap       = NULL;
static wxBitmap *gNextDblBitmap    = NULL;

BEGIN_EVENT_TABLE(mmNavigator, wxWindow)
    EVT_BUTTON(mmID_NAV_PREV2, mmNavigator::OnButton)
    EVT_BUTTON(mmID_NAV_PREV,  mmNavigator::OnButton)
    EVT_BUTTON(mmID_NAV_NEXT,  mmNavigator::OnButton)
    EVT_BUTTON(mmID_NAV_NEXT2, mmNavigator::OnButton)
END_EVENT_TABLE()

/////////////////////////////////////////////////////////////////////////////
// mmNavigatorEvent class
/////////////////////////////////////////////////////////////////////////////

mmNavigatorEvent::mmNavigatorEvent(mmNavigator *nav, wxEventType type)
    : wxCommandEvent(type, nav->GetId())
{
}

mmNavigator::mmNavigator(wxWindow *parent,
                         const wxWindowID id,
                         wxWindow *userWin,
                         const bool prev,
                         const bool next,
                         const bool prevDbl,
                         const bool nextDbl,
                         const wxPoint &pos,
                         const wxSize &size,
                         const long style)
    : wxWindow(parent, id, pos, size, style)
// Constructor
{
    mWindow        = NULL;
    mPrevDblButton = NULL;
    mPrevButton    = NULL;
    mNextButton    = NULL;
    mNextDblButton = NULL;

    if(!gPrevDblBitmap) gPrevDblBitmap = new wxBitmap(prev_dbl_btn_xpm);
    if(!gPrevBitmap)    gPrevBitmap    = new wxBitmap(prev_btn_xpm);
    if(!gNextBitmap)    gNextBitmap    = new wxBitmap(next_btn_xpm);
    if(!gNextDblBitmap) gNextDblBitmap = new wxBitmap(next_dbl_btn_xpm);

    // 'Double-Previous'-button
    if(prevDbl)
    {
        mPrevDblButton = new mmMultiButton(this, mmID_NAV_PREV2, wxEmptyString,
                                           *gPrevDblBitmap,
                                           wxDefaultPosition, wxDefaultSize,
                                           mmMB_AUTODRAW);
    }
    // 'Previous'-button
    if(prev)
    {
        mPrevButton = new mmMultiButton(this, mmID_NAV_PREV, wxEmptyString,
                                        *gPrevBitmap,
                                        wxDefaultPosition, wxDefaultSize,
                                        mmMB_AUTODRAW);
    }
    // 'Next'-button
    if(next)
    {
        mNextButton = new mmMultiButton(this, mmID_NAV_NEXT, wxEmptyString,
                                        *gNextBitmap,
                                        wxDefaultPosition, wxDefaultSize,
                                        mmMB_AUTODRAW);
    }
    // 'Double-Next'-button
    if(nextDbl)
    {
        mNextDblButton = new mmMultiButton(this, mmID_NAV_NEXT2, wxEmptyString,
                                           *gNextDblBitmap,
                                           wxDefaultPosition, wxDefaultSize,
                                           mmMB_AUTODRAW);
    }

    // Set constraints
    if(mPrevDblButton)
    {
        wxLayoutConstraints *con = new wxLayoutConstraints;
        con->top.SameAs(this, wxTop);
        con->bottom.SameAs(this, wxBottom);
        con->left.SameAs(this, wxLeft);
        con->width.AsIs();
        mPrevDblButton->SetConstraints(con);
    }
    if(mPrevButton)
    {
        wxLayoutConstraints *con = new wxLayoutConstraints;
        con->top.SameAs(this, wxTop);
        con->bottom.SameAs(this, wxBottom);
        if(mPrevDblButton)
            con->left.SameAs(mPrevDblButton, wxRight);
        else
            con->left.SameAs(this, wxLeft);
        con->width.AsIs();
        mPrevButton->SetConstraints(con);
    }
    if(mNextDblButton)
    {
        wxLayoutConstraints *con = new wxLayoutConstraints;
        con->top.SameAs(this, wxTop);
        con->bottom.SameAs(this, wxBottom);
        con->right.SameAs(this, wxRight);
        con->width.AsIs();
        mNextDblButton->SetConstraints(con);
    }
    if(mNextButton)
    {
        wxLayoutConstraints *con = new wxLayoutConstraints;
        con->top.SameAs(this, wxTop);
        con->bottom.SameAs(this, wxBottom);
        if(mNextDblButton)
            con->right.SameAs(mNextDblButton, wxLeft);
        else
            con->right.SameAs(this, wxRight);
        con->width.AsIs();
        mNextButton->SetConstraints(con);
    }
    SetWindow(userWin);
} // Constructor

mmNavigator::~mmNavigator()
// Destructor
{
    if(mPrevDblButton) {
        mPrevDblButton->Destroy();
        mPrevDblButton = NULL;
    }
    if(mPrevButton) {
        mPrevButton->Destroy();
        mPrevButton   = NULL;
    }
    if(mNextButton) {
        mNextButton->Destroy();
        mNextButton   = NULL;
    }
    if(mNextDblButton) {
        mNextDblButton->Destroy();
        mNextDblButton = NULL;
    }
} // Destructor

void mmNavigator::SetWindow(wxWindow *win)
// Set the optional window.
{
    if(mWindow)
        mWindow->Show(FALSE);
    mWindow = win;
    if(mWindow)
    {
        mWindow->Reparent(this);
        wxLayoutConstraints *con = new wxLayoutConstraints;
        con->top.SameAs(this, wxTop);
        con->bottom.SameAs(this, wxBottom);
        if(mPrevButton)
            con->left.SameAs(mPrevButton, wxRight);
        else if(mPrevDblButton)
            con->left.SameAs(mPrevDblButton, wxRight);
        else
            con->left.SameAs(this, wxLeft);
        if(mNextButton)
            con->right.SameAs(mNextButton, wxLeft);
        else if(mNextDblButton)
            con->right.SameAs(mNextDblButton, wxLeft);
        else
            con->right.SameAs(this, wxRight);
        mWindow->SetConstraints(con);
        SetAutoLayout(TRUE);
        Layout();
        mWindow->Show(TRUE);
    }
} // SetWindow

bool mmNavigator::SetBackgroundColour(const wxColour &colour)
{
    if(mWindow)
        mWindow->SetBackgroundColour(colour);
    if(mPrevDblButton)
        mPrevDblButton->SetBackgroundColour(colour);
    if(mPrevButton)
        mPrevButton->SetBackgroundColour(colour);
    if(mNextButton)
        mNextButton->SetBackgroundColour(colour);
    if(mNextDblButton)
        mNextDblButton->SetBackgroundColour(colour);
    wxWindow::SetBackgroundColour(colour);
    return TRUE;
} // SetBackgroundColour

void mmNavigator::OnButton(wxCommandEvent &event)
{
    GenerateEvent(event.GetId());
    switch(event.GetId())
    {
        case mmID_NAV_PREV2:
            GenerateEvent(mmEVT_NAVIGATOR_PREV2);
            break;
        case mmID_NAV_PREV:
            GenerateEvent(mmEVT_NAVIGATOR_PREV);
            break;
        case mmID_NAV_NEXT:
            GenerateEvent(mmEVT_NAVIGATOR_NEXT);
            break;
        case mmID_NAV_NEXT2:
            GenerateEvent(mmEVT_NAVIGATOR_NEXT2);
            break;
    }
} // OnButton

void mmNavigator::GenerateEvent(wxEventType type)
{
    mmNavigatorEvent ev(this, type);
    GetEventHandler()->ProcessEvent(ev);
} // GenerateEvent

