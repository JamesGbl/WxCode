//! \file mmHyperText.cpp
//
// Name     : mmHyperText
// Purpose  : A clickable text that performs an action when clicked.
//            The default action is to bring up the default browser.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#include <wx/mmwxdef.h>
#include "wx/mmHyperText.h"

#ifdef __MMDEBUG__
	WXDLLIMPEXP_DATA_MMWX(extern wxTextCtrl*) gDebug;    //!< Global debug output window.
#endif

BEGIN_EVENT_TABLE(mmHyperText, wxWindow)
    EVT_PAINT(mmHyperText::OnPaint)
    EVT_MOUSE_EVENTS(mmHyperText::OnMouse)
END_EVENT_TABLE()

/*! \brief Constructor.
 *
 * \param parent wxWindow*				The parent window.
 * \param id 		const wxWindowID	The ID of this window.
 * \param str 		const wxString&		The hyperlinked text.
 * \param url 		const wxString&		The URL to link to.
 * \param pos 	const wxPoint&			The button's position.
 * \param size 	const wxSize&			The button's size.
 * \param style 	const longint				The button's style.
 *
 */
mmHyperText::mmHyperText(wxWindow *parent,
                         const wxWindowID id,
                         const wxString &str,
                         const wxString &url,
                         const wxPoint &pos,
                         const wxSize &size,
                         const long style)
    : wxWindow(parent, id, pos, size, style)
{
    mText = str;
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

/*! \brief Destructor.
 */
mmHyperText::~mmHyperText(void)
{
} // Destructor

/*! \brief A mouse event occurred.
 *
 * \param event wxMouseEvent&	A reference to a wxMouseEvent object.
 * \return void
 *
 */
void mmHyperText::OnMouse(wxMouseEvent &event)
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

/*! \brief A paint event occurred.
 *
 * \param event wxPaintEvent&	A reference to a wxPaintEvent object.
 * \return void
 *
 */
void mmHyperText::OnPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);
    dc.SetFont(GetFont());
    if(!(mStyle & mmHT_NO_AUTOSIZE))
    {
        int lw, lh, ext;
        GetTextExtent(mText, &lw, &lh, &ext);
        SetClientSize(wxSize(lw + 2 * mMargin, lh + ext + 2 * mMargin));
    }
    if(mIsVisited)
        dc.SetTextForeground(*mVisitedColour);
    else
        dc.SetTextForeground(*mUnvisitedColour);
    dc.SetBackgroundMode(wxTRANSPARENT);
    dc.Clear();
    dc.DrawText(mText, mMargin, mMargin);
    if(mLeftIsDown)
    {
        int w, h;
        GetClientSize(&w, &h);
        dc.SetPen(*wxMEDIUM_GREY_PEN);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(0, 0, w, h);
    }
} // OnPaint

/*! \brief The text was clicked, execute the URL.
 *
 * \param isURL bool	True if the URL starts with "http:" or "mailto:", false otherwise i.e. "file:"
 * \return void
 *
 */
void mmHyperText::HyperExec(bool isURL)
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

