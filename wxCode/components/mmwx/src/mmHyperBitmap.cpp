//
// Name     : mmHyperBitmap
// Purpose  : A clickable bitmap that performs an action when clicked.
//            The default action is to bring up the default browser.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#include "wx/mmHyperBitmap.h"

#ifdef __MMDEBUG__
extern wxTextCtrl *gDebug; // For global debug output
#endif

BEGIN_EVENT_TABLE(mmHyperBitmap, wxWindow)
    EVT_PAINT(mmHyperBitmap::OnPaint)
    EVT_MOUSE_EVENTS(mmHyperBitmap::OnMouse)
END_EVENT_TABLE()

mmHyperBitmap::mmHyperBitmap(wxWindow *parent,
                             const wxWindowID id,
                             const wxBitmap &bitmap,
                             const wxString &url,
                             const wxPoint &pos,
                             const wxSize &size,
                             const long style)
    : wxWindow(parent, id, pos, size, style)
{
    mBitmap = bitmap;
    mStyle = style;
    mURL = url;
    mMargin = 0;
    mIsVisited = FALSE;
    mLeftIsDown = FALSE;
    mUnvisitedColour = new wxColour(0, 0, 255); // BLUE
    mVisitedColour   = new wxColour(128, 0, 128); // PURPLE
    wxFont font = GetFont();
    font.SetUnderlined(TRUE);
    SetFont(font);
    mEnterCursor = new wxCursor(wxCURSOR_HAND);
    mLeaveCursor = new wxCursor(wxCURSOR_ARROW);
} // Constructor

mmHyperBitmap::~mmHyperBitmap(void)
{
} // Destructor

void mmHyperBitmap::OnMouse(wxMouseEvent &event)
{
    if(event.Entering())
    {
        mLeftIsDown = FALSE;
        SetCursor(*mEnterCursor);
        Refresh();
    }
    else if(event.Leaving())
    {
        mLeftIsDown = FALSE;
        SetCursor(*mLeaveCursor);
        Refresh();
    }
    else if(event.LeftDown() || event.LeftDClick())
    {
        mLeftIsDown = TRUE;
        Refresh();
    }
    else if(event.LeftUp() && mLeftIsDown)
    {
        mIsVisited = TRUE;
        mLeftIsDown = FALSE;
        Refresh();
        ::wxSafeYield();

        if(mURL.StartsWith(wxT("http://")) || mURL.StartsWith(wxT("mailto:")))
            HyperExec(TRUE);
        else
            HyperExec(FALSE);
    }

    event.Skip();
} // OnMouse

void mmHyperBitmap::OnPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);
    dc.SetFont(GetFont());
    if(!(mStyle & mmHT_NO_AUTOSIZE) && mBitmap.IsOk())
    {
        int bw, bh;
        bw = mBitmap.GetWidth();
        bh = mBitmap.GetHeight();
        SetClientSize(wxSize(bw + 2 * mMargin, bh + 2 * mMargin));
    }
    if(mIsVisited)
        dc.SetTextForeground(*mVisitedColour);
    else
        dc.SetTextForeground(*mUnvisitedColour);
    dc.SetBackgroundMode(wxTRANSPARENT);
    dc.Clear();
    if(mBitmap.IsOk())
        dc.DrawBitmap(mBitmap, mMargin, mMargin, TRUE);
    if(mLeftIsDown)
    {
        int w, h;
        GetClientSize(&w, &h);
        dc.SetPen(*wxMEDIUM_GREY_PEN);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(0, 0, w, h);
    }
} // OnPaint

void mmHyperBitmap::HyperExec(bool isURL)
{
    wxString ext;
    if(isURL)
        ext = wxT(".htm");
    else
        ext = mURL.AfterLast('.');
    wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(ext);
    if(ft)
    {
        wxString desc;
        if(ft->GetDescription(&desc))
        {
            wxString cmd;
            if(ft->GetOpenCommand(&cmd, wxFileType::MessageParameters(mURL, wxEmptyString)))
            {
                if(isURL)
                    cmd.Replace(wxT("file://"), wxEmptyString);
                /*
                    wxString str;
                    str.Printf(_T("To open %s (%s) do '%s'.\n"),
                               mURL.c_str(), desc.c_str(), cmd.c_str());
                #ifdef __MMDEBUG__
                    *gDebug<<str;
                #endif
                */
                ::wxExecute(cmd);
            }
        } // if (ft->GetDescription)
        delete ft;
    } // if (ft)
} // HyperExec

