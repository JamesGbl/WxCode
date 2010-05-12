//! \file mmMultiButton.cpp
//
// Name     : mmMultiButton
// Purpose  : A clickable button that can
//            - have a bitmap and/or a text label.
//            - function as a toggle ("on/off") button.
//            - turn active/inactive on entering/leaving it with the mouse.
//            - add a drop-down arrow to the bitmap in a separate section
//              (DROPDOWN) or as part of the bitmap (WHOLEDROPDOWN).
//
//            NOTE:
//            - mmMB_DROPDOWN and mmMB_WHOLEDROPDOWN cannot be used at
//              the same time.
//            - mmMB_STATIC and mmMB_AUTODRAW are mutually exclusive.
//
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#include <wx/mmwxdef.h>
#include "wx/mmMultiButton.h"

#ifdef __MMDEBUG__
	WXDLLIMPEXP_DATA_MMWX(extern wxTextCtrl*) gDebug;    //!< Global debug output window.
#endif

#include "../bitmaps/down_btn.xpm"
wxBitmap gDownBM(down_btn_xpm);
#include "../bitmaps/disable.xpm"
wxBitmap gDisableBM(wxDISABLE_BUTTON_BITMAP);

BEGIN_EVENT_TABLE(mmMultiButton, wxWindow)
    EVT_PAINT(mmMultiButton::OnPaint)
    EVT_MOUSE_EVENTS(mmMultiButton::OnMouse)
	EVT_MOUSE_CAPTURE_LOST(mmMultiButton::OnCaptureLost)
END_EVENT_TABLE()

/*! \brief Tile a bitmap.
 *
 * \param rect 		const wxRect&	The area we're tiling in.
 * \param dc 			wxDC&				The device context to draw on.
 * \param bitmap 	wxBitmap&		The bitmap to tile.
 * \return bool
 *
 */
WXDLLIMPEXP_MMWX bool TileBitmap(const wxRect &rect, wxDC &dc, wxBitmap &bitmap)
{
    int w = bitmap.GetWidth();
    int h = bitmap.GetHeight();

    int i, j;
    for(i = rect.x; i < rect.x + rect.width; i += w)
    {
        for(j = rect.y; j < rect.y + rect.height; j += h)
            dc.DrawBitmap(bitmap, i, j, TRUE);
    }
    return TRUE;
} // TileBitmap

/*! \brief Constructor.
 *
 * \param parent 			wxWindow*				The parent window.
 * \param id 					const wxWindowID	The ID of this window.
 * \param label 				const wxString&		The text to display on the button.
 * \param defaultBitmap 	const wxBitmap&		The bitmap to display on the button.
 * \param pos 				const wxPoint&			The button's position.
 * \param size 				const wxSize&			The button's size.
 * \param style 				const longint				The button's style.
 *
 */
mmMultiButton::mmMultiButton(wxWindow *parent,
                             const wxWindowID id,
                             const wxString &label,
                             const wxBitmap &defaultBitmap,
                             const wxPoint &pos,
                             const wxSize &size,
                             const long int style)
    : wxWindow(parent, id, pos, size, 0)
{
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

    if(!gDisableBM.GetMask())
    {   // Only set mask for this global bitmap once
        wxMask *mask = new wxMask(gDisableBM, *wxBLACK);
        gDisableBM.SetMask(mask);
    }

    mIsActivated           = FALSE;
    mHasFocus              = FALSE;
    mIsToggleDown          = FALSE;
    mIsWholeDropToggleDown = FALSE;
    mIsDropToggleDown      = FALSE;
    mIsSelected            = FALSE;

    mStyle    = style;
    mLabelStr = label;

    mDefaultBitmap  = defaultBitmap;
    mFocusBitmap    = mDefaultBitmap;
    mSelectedBitmap = mDefaultBitmap;
    mToggleBitmap   = mDefaultBitmap;

    FindAndSetSize();
    Refresh();
} // Constructor

/*! \brief Destructor.
 */
mmMultiButton::~mmMultiButton(void)
{
} // Destructor

/*! \brief Calculate and set the required size.
 *
 * \return void
 *
 */
void mmMultiButton::FindAndSetSize()
{
    // Set (total) border size
    if(mStyle & wxBU_AUTODRAW)
        mBorderSize = 3;
    else if((mStyle & wxRAISED_BORDER) || (mStyle & wxSUNKEN_BORDER))
        mBorderSize = 2;
    else if(mStyle & wxSIMPLE_BORDER)
        mBorderSize = 2;
    else
        mBorderSize = 0; // Default: No border

    // Set internal margin size (for each side)
    mMarginSize = 2;

    if(!(mStyle & mmMB_NO_AUTOSIZE))
    {   // Refresh and set size of button.
        int total_width = 0, total_height = 0;
        if(mDefaultBitmap.IsOk())
        {
            total_width  = mDefaultBitmap.GetWidth();  // NB! Should use largest bm
            total_height = mDefaultBitmap.GetHeight(); // NB! Should use largest bm
        }
        int labw = 0, labh = 0, ext = 0;
        if(mLabelStr != wxEmptyString)
            GetTextExtent(mLabelStr, &labw, &labh, &ext);
        labh += ext;
        if((mStyle & mmMB_WHOLEDROPDOWN))
            total_width = wxMax(total_width + gDownBM.GetWidth(), labw);
        else
            total_width = wxMax(total_width, labw);
        total_height += labh;
        if((mStyle & mmMB_DROPDOWN))
        {
            total_width += gDownBM.GetWidth();
            total_height = wxMax(total_height, gDownBM.GetHeight());
        }
        int w_extra = mBorderSize + 2 * mMarginSize;
        int h_extra = mBorderSize + 2 * mMarginSize;
        if(mStyle & mmMB_DROPDOWN)
            w_extra += 2 * mMarginSize;
        if(mLabelStr != wxEmptyString && mDefaultBitmap.IsOk())
            h_extra += mMarginSize;
        total_width  += w_extra;
        total_height += h_extra;
#ifdef __MMDEBUG__
        *gDebug << wxT("w,h:") << total_width << wxT(",") << total_height << wxT("\n");
#endif
        SetSize(total_width, total_height);
        SetAutoLayout(TRUE);
        Layout();
    }
} // FindAndSetSize

/*! \brief A mouse event occurred.
 *
 * \param event wxMouseEvent&	A reference to a wxMouseEvent object.
 * \return void
 *
 */
void mmMultiButton::OnMouse(wxMouseEvent &event)
// Update button state
{
#ifdef __MMDEBUG__
   *gDebug << wxT("mmMultiButton::OnMouse,type:") << event.GetEventType() << wxT("\n");
#endif

	// Get the mouse position.
    mIsActivated = FALSE;

    if(mStyle & mmMB_STATIC || !IsEnabled())
    {
        mHasFocus              = FALSE;
        mIsToggleDown          = FALSE;
        mIsWholeDropToggleDown = FALSE;
        mIsDropToggleDown      = FALSE;
        mIsSelected            = FALSE;
        return;
    }

    if(!(mStyle & mmMB_TOGGLE))
        mIsToggleDown          = FALSE;

    if(!(mStyle & mmMB_WHOLEDROPDOWN))
        mIsWholeDropToggleDown = FALSE;

    if(!(mStyle & mmMB_DROPDOWN))
        mIsDropToggleDown      = FALSE;

    bool focus_changed           = FALSE,
			toggle_changed          = FALSE,
			wholedroptoggle_changed = FALSE,
			droptoggle_changed      = FALSE,
			selected_changed        = FALSE;

    int w, h;
    GetClientSize(&w, &h);
    wxPoint mp = event.GetPosition();

    if(event.Entering())
    {   // ENTER
        if(mStyle & mmMB_AUTODRAW || mStyle & mmMB_FOCUS)
            focus_changed = !mHasFocus;
        mHasFocus = TRUE;
    }
    else if(event.Leaving())
    {   // LEAVE
        mIsSelected = FALSE;
        if(!mIsDropToggleDown && !mIsWholeDropToggleDown)
        {
            if(mStyle & mmMB_AUTODRAW || mStyle & mmMB_FOCUS)
                focus_changed = mHasFocus;
            mHasFocus = FALSE;
            if(HasCapture()){
				ReleaseMouse();
            }
        }
    }
    else if(event.LeftDown() || event.LeftDClick())
    {   // SELECT
        if(mStyle & mmMB_TOGGLE)
        {   // TOGGLE
            if(mIsSelected)
                selected_changed = TRUE;
            mIsSelected = FALSE;
            CaptureMouse();
        }
        else if(mStyle & mmMB_WHOLEDROPDOWN)
        {   // WHOLEDROPDOWN
            if(MouseIsOnButton())
            {
                if(!mIsSelected)
                    selected_changed = TRUE;
                mIsSelected = TRUE;
                wholedroptoggle_changed = TRUE;
                mIsWholeDropToggleDown = !mIsWholeDropToggleDown;
                if(mIsWholeDropToggleDown)
                    CaptureMouse();
                else
					if(HasCapture()){
						ReleaseMouse();
					}
            }
            else
            {   // Pressed outside of button
                if(mIsSelected)
                    selected_changed = TRUE;
                mIsSelected = FALSE;
                if(mIsWholeDropToggleDown)
                    wholedroptoggle_changed = TRUE;
                mIsWholeDropToggleDown = FALSE;
				if(HasCapture()){
					ReleaseMouse();
				}
            }
        }
        else if(mStyle & mmMB_DROPDOWN)
        {   // DROPDOWN
            if(mp.x > w - gDownBM.GetWidth() - mBorderSize && mp.x < w &&(mp.y > mBorderSize)&& mp.y < h - mBorderSize)
            {   // Drop down arrow pressed
                if(mIsSelected)
                    selected_changed = TRUE;
                mIsSelected = FALSE;
                droptoggle_changed = TRUE;
                mIsDropToggleDown = !mIsDropToggleDown;
                if(mIsDropToggleDown)
                    CaptureMouse();
                else
					if(HasCapture()){
						ReleaseMouse();
					}
            }
            else if(MouseIsOnButton())
            {   // Button (not arrow) pressed
                if(!mIsSelected)
                    selected_changed = TRUE;
                mIsSelected = TRUE;
                CaptureMouse();
            }
            else
            {   // Pressed outside of button
                if(mIsSelected)
                    selected_changed = TRUE;
                mIsSelected = FALSE;
                if(mIsDropToggleDown)
                    droptoggle_changed = TRUE;
                mIsDropToggleDown = FALSE;
				if(HasCapture()){
					ReleaseMouse();
				}
            }
        }
        else
        {   // 'Normal' button
            if(!mIsSelected)
                selected_changed = TRUE;
            mIsSelected = TRUE;
            CaptureMouse();
        }
        if(!MouseIsOnButton())
        {
            focus_changed = mHasFocus;
            mHasFocus = FALSE;
        }
    }
    else if(event.LeftUp())
    {   // ACTIVATE
        if(mStyle & mmMB_TOGGLE)
        {   // TOGGLE
            if(mIsSelected)
                selected_changed = TRUE;
            mIsSelected = FALSE;
            toggle_changed = TRUE;
            mIsToggleDown = !mIsToggleDown;
            if(HasCapture()){
				ReleaseMouse();
            }
        }
        else if(mStyle & mmMB_WHOLEDROPDOWN)
        {   // WHOLEDROPDOWN
            if(mIsSelected)
                selected_changed = TRUE;
            mIsSelected = FALSE;
            if(!mIsWholeDropToggleDown)
				if(HasCapture()){
					ReleaseMouse();
				}
        }
        else if(mStyle & mmMB_DROPDOWN)
        {   // DROPDOWN
            if(mIsSelected)
                selected_changed = TRUE;
            mIsSelected = FALSE;
            if(mp.x > w - gDownBM.GetWidth() - mBorderSize && mp.x < w && mp.y > mBorderSize && mp.y < h - mBorderSize)
            {   // Drop down arrow pressed
                if(!mIsDropToggleDown)
					if(HasCapture()){
						ReleaseMouse();
					}
            }
            else if(MouseIsOnButton())
            {   // Button (not arrow) pressed
                if(mIsDropToggleDown)
                    droptoggle_changed = TRUE;
                mIsDropToggleDown = FALSE;
                if(!droptoggle_changed)
                    mIsActivated = TRUE; // NOTE! SEND ACTIVATE SIGNAL!
				if(HasCapture()){
					ReleaseMouse();
				}
            }
        }
        else
        {   // 'Normal' button
            if(mIsSelected)
                selected_changed = TRUE;
            mIsSelected = FALSE;
            mIsActivated = TRUE; // NOTE! SEND ACTIVATE SIGNAL!
            if(HasCapture()){
				ReleaseMouse();
            }
        }
    }

    // Redraw only if necessary
    if(focus_changed || selected_changed || wholedroptoggle_changed || droptoggle_changed || toggle_changed)
    {
        Refresh();
        // Generate events to let derived class know what happened
        if(focus_changed)
        {   // ENTER/LEAVE
            wxCommandEvent ev(-1, GetId());
            if(mHasFocus)
                ev.SetEventType(wxEVT_ENTER_WINDOW);
            else
                ev.SetEventType(wxEVT_LEAVE_WINDOW);
            GetEventHandler()->ProcessEvent(ev); // Neccessary?
        }
        if(toggle_changed)
        {   // TOGGLE
            wxCommandEvent ev(mmEVT_TOGGLE, GetId());
            GetEventHandler()->ProcessEvent(ev);
        }
        if(wholedroptoggle_changed)
        {   // WHOLEDROPDOWN
            wxCommandEvent ev(mmEVT_WHOLEDROP_TOGGLE, GetId());
            GetEventHandler()->ProcessEvent(ev);
        }
        if(droptoggle_changed)
        {   // DROPDOWN
            wxCommandEvent ev(mmEVT_DROP_TOGGLE, GetId());
            GetEventHandler()->ProcessEvent(ev);
        }
        if(selected_changed)
        {   // SELECT
            wxCommandEvent ev(wxEVT_COMMAND_LEFT_CLICK, GetId());
            GetEventHandler()->ProcessEvent(ev);
        }
        if(mIsActivated)
        {   // ACTIVATE
            wxCommandEvent ev(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
            GetEventHandler()->ProcessEvent(ev);
        }
    } // if
    event.Skip();
} // OnMouse

/*! \brief The mouse capture was lost.
 *
 * \param event wxMouseCaptureLostEvent&	A reference to a wxMouseCaptureLostEvent object.
 * \return void
 *
 */
void mmMultiButton::OnCaptureLost(wxMouseCaptureLostEvent& event)
{
	if(HasCapture()){
		ReleaseMouse();
	}
}

/* Using OnPaint causes too much flicker.
 * Instead, call Refresh() manually after calling GDI functions
 * (such as e.g. SetBackgroundColour() etc.) in the main program.
 * */
/*! \brief A paint event occurred.
 *
 * \param event wxPaintEvent&	A reference to a wxPaintEvent object.
 * \return void
 *
 */
void mmMultiButton::OnPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);
    dc.SetBackground(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour(), wxSOLID));
    dc.Clear();
    RedrawBitmaps(dc);
    RedrawLabel(dc);
    RedrawBorders(dc);
} // OnPaint

/*! \brief Redraw the button's bitmaps.
 *
 * \param dc wxDC&	The device context to draw on.
 * \return void
 *
 */
void mmMultiButton::RedrawBitmaps(wxDC &dc)
// Redraw icons
{
    // Find the current bitmap
    wxBitmap bm = mDefaultBitmap; // Default bitmap
    if(mStyle & mmMB_FOCUS && mHasFocus)
        bm = mFocusBitmap; // Focused bitmap
    if(mStyle & mmMB_SELECT && mIsSelected)
        bm = mSelectedBitmap; // Selected bitmap
    if(mStyle & mmMB_TOGGLE && mIsToggleDown)
        bm = mToggleBitmap; // Toggle bitmap

    int w, h;
    GetClientSize(&w, &h);

    int offset = 0;
    if(mStyle & mmMB_AUTODRAW)
        if(((mHasFocus && mIsSelected) || mIsToggleDown || mIsWholeDropToggleDown) && !mIsDropToggleDown)
            offset = 1;

    // Draw the bitmap
    int lw = 0, lh = 0, ext = 0;
    if(mLabelStr != wxEmptyString)
        GetTextExtent(mLabelStr, &lw, &lh, &ext);
    lh += ext;
    int bmdown_x_off = 0;
    if((mStyle & mmMB_WHOLEDROPDOWN) || (mStyle & mmMB_DROPDOWN))
        bmdown_x_off = gDownBM.GetWidth();
    int x_off = bm.GetWidth()  + bmdown_x_off;
    int y_off = bm.GetHeight() + lh;
    int x = offset + wxMax(int((w - x_off) / 2), int((lw - x_off) / 2));
    int y = offset + int((h - y_off) / 2);
    if(bm.IsOk())
    {
        if(IsEnabled())
            dc.DrawBitmap(bm, x, y, TRUE);
        else
        {
            dc.DrawBitmap(bm, x, y, TRUE);
            wxRect rect(0, 0, GetClientSize().x, GetClientSize().y);
            TileBitmap(rect, dc, gDisableBM);
        }
    }

    // Draw the drop-down-arrow
    if((mStyle & mmMB_DROPDOWN || mStyle & mmMB_WHOLEDROPDOWN))
    {
        if(mIsDropToggleDown)
            offset = 1;
        int x_off = gDownBM.GetWidth();
        int y_off = gDownBM.GetHeight();
        int x = offset + w - x_off;
        int y = offset + int((h - y_off) / 2);
        dc.DrawBitmap(gDownBM, x, y, TRUE);
    }
} // RedrawBitmaps

/*! \brief Redraw the button's label.
 *
 * \param dc wxDC&	The device context to draw on.
 * \return void
 *
 */
void mmMultiButton::RedrawLabel(wxDC &dc)
// Redraw label
{
    int w, h;
    GetClientSize(&w, &h);

    int offset = 0;
    if(mStyle & mmMB_AUTODRAW)
        if(((mHasFocus && mIsSelected) || mIsToggleDown || mIsWholeDropToggleDown) && !mIsDropToggleDown)
            offset = 1;

    int lw = 0, lh = 0, ext = 0;
    GetTextExtent(mLabelStr, &lw, &lh, &ext);
    lh += ext;

    int bmdown_off = 0;
    if(mStyle & mmMB_DROPDOWN)
        bmdown_off = gDownBM.GetWidth();
    int x_off = lw + bmdown_off;
    int y_off = lh + mMarginSize;
    int x = offset + wxMax(int((w - x_off) / 2), int((lw - x_off) / 2));
    int y = offset + wxMax(h - y_off, 0);

    dc.SetFont(GetFont());
    if(IsEnabled())
    {
        dc.SetTextForeground(GetForegroundColour());
        dc.DrawText(mLabelStr, x, y);
    }
    else
    {
        dc.SetTextForeground(*wxWHITE);
        dc.DrawText(mLabelStr, x + 1, y + 1);
        dc.SetTextForeground(wxTheColourDatabase->Find(wxT("GREY")));
        dc.DrawText(mLabelStr, x, y);
        dc.SetTextForeground(*wxBLACK);
    }
} // RedrawLabel

/*! \brief Redraw the button's borders.
 *
 * \param dc wxDC&	The device context to draw on.
 * \return void
 *
 */
void mmMultiButton::RedrawBorders(wxDC &dc)
// Draw all borders of the button.
{
    if((mStyle & wxBU_AUTODRAW) ||
            // Don't draw borders if wxBitmapButton does it.
            ((mStyle & mmMB_AUTODRAW) && !mHasFocus) ||
            // If mmMB_AUTODRAW, borders are only drawn if button has focus.
            (!(mStyle & mmMB_AUTODRAW) && mStyle & wxNO_BORDER))
        // If not mmMB_AUTODRAW and no border, there's nothing to do.
        return;

    int w, h;
    GetClientSize(&w, &h);

    if(mStyle & mmMB_DROPDOWN)
        w -= gDownBM.GetWidth();

    if(mStyle & wxSIMPLE_BORDER)
    {
        dc.SetPen(*wxThePenList->FindOrCreatePen(wxTheColourDatabase->Find(wxT("GREY")), 1, wxSOLID));
        dc.DrawLine(0, 0, 0, h - 1);
        dc.DrawLine(0, 0, w, 0);
        dc.DrawLine(0, h - 1, w, h - 1);
        dc.DrawLine(w - 1, 0, w - 1, h - 1);
        // Drop-down arrow
        if(mStyle & mmMB_DROPDOWN)
        {
            w += gDownBM.GetWidth();
            int x = w - gDownBM.GetWidth() - 1;
            dc.SetPen(*wxThePenList->FindOrCreatePen(wxTheColourDatabase->Find(wxT("GREY")), 1, wxSOLID));
            dc.DrawLine(x, 0, x, h - 1);
            dc.DrawLine(x, 0, w, 0);
            dc.DrawLine(x, h - 1, w, h - 1);
            dc.DrawLine(w - 1, 0, w - 1, h - 1);
        }
    }
    else if((mStyle & wxSUNKEN_BORDER &&
             !(mIsSelected || mIsToggleDown || mIsWholeDropToggleDown) && !mIsDropToggleDown) ||
            ((mStyle & wxRAISED_BORDER) &&
             (mIsSelected || mIsToggleDown || mIsWholeDropToggleDown) && !mIsDropToggleDown) ||
            (!(mStyle & wxSUNKEN_BORDER) &&
             (mIsSelected || mIsToggleDown || mIsWholeDropToggleDown) && !mIsDropToggleDown))
    {   // Has focus, and is selected or toggled down
        dc.SetPen(*wxWHITE_PEN);
        dc.DrawLine(0,  h - 1, w - 1, h - 1);
        dc.DrawLine(w - 1, 0,  w - 1, h);
        dc.SetPen(*wxThePenList->FindOrCreatePen(wxTheColourDatabase->Find(wxT("GREY")), 1, wxSOLID));
        dc.DrawLine(0, 0, 0, h);
        dc.DrawLine(0, 0, w, 0);
        // Drop-down arrow
        if(mStyle & mmMB_DROPDOWN)
        {
            w += gDownBM.GetWidth();
            int x = w - gDownBM.GetWidth();
            dc.SetPen(*wxWHITE_PEN);
            dc.DrawLine(x,  h - 1, w - 1, h - 1);
            dc.DrawLine(w - 1, 0,  w - 1, h);
            dc.SetPen(*wxThePenList->FindOrCreatePen(wxTheColourDatabase->Find(wxT("GREY")), 1, wxSOLID));
            dc.DrawLine(x, 0, x, h);
            dc.DrawLine(x, 0, w, 0);
        }
    }
    else
    {   // Has focus, not selected or toggled down
        dc.SetPen(*wxWHITE_PEN);
        dc.DrawLine(0, 0, 0,  h - 1);
        dc.DrawLine(0, 0, w - 1, 0);
        dc.SetPen(*wxThePenList->FindOrCreatePen(wxTheColourDatabase->Find(wxT("GREY")), 1, wxSOLID));
        dc.DrawLine(0,  h - 1, w - 1, h - 1);
        dc.DrawLine(w - 1, 0,  w - 1, h);
        // Drop-down arrow
        if(mStyle & mmMB_DROPDOWN)
        {
            w += gDownBM.GetWidth();
            int x = w - gDownBM.GetWidth();
            if(!mIsDropToggleDown)
                dc.SetPen(*wxWHITE_PEN);
            else
                dc.SetPen(*wxThePenList->FindOrCreatePen(wxTheColourDatabase->Find(wxT("GREY")), 1, wxSOLID));
            dc.DrawLine(x,  0,  x,  h - 1);
            dc.DrawLine(x,  0,  w - 1, 0);
            if(mIsDropToggleDown)
                dc.SetPen(*wxWHITE_PEN);
            else
                dc.SetPen(*wxThePenList->FindOrCreatePen(wxTheColourDatabase->Find(wxT("GREY")), 1, wxSOLID));
            dc.DrawLine(x,  h - 1, w - 1, h - 1);
            dc.DrawLine(w - 1, 0,  w - 1, h);
        }
    }
} // RedrawBorders

/*! \brief Set the button's default bitmap.
 *
 * \param bm wxBitmap&	The bitmap.
 * \return void
 *
 */
void mmMultiButton::SetDefaultBitmap(wxBitmap &bm)
{
    mDefaultBitmap = bm;
    if(!mDefaultBitmap.IsOk())
        mDefaultBitmap = bm;
    Refresh();
} // SetDefaultBitmap

/*! \brief Set the bitmap to display when the button is focused.
 *
 * \param bm wxBitmap&	The bitmap.
 * \return void
 *
 */
void mmMultiButton::SetFocusBitmap(wxBitmap &bm)
{
    mFocusBitmap = bm;
    if(!mDefaultBitmap.IsOk())
        mDefaultBitmap = bm;
    Refresh();
} // SetFocusBitmap

/*! \brief Set the bitmap to display when the button is selected.
 *
 * \param bm wxBitmap&	The bitmap.
 * \return void
 *
 */
void mmMultiButton::SetSelectedBitmap(wxBitmap &bm)
{
    mSelectedBitmap = bm;
    if(!mDefaultBitmap.IsOk())
        mDefaultBitmap = bm;
    Refresh();
} // SetSelectedBitmap

/*! \brief Set the bitmap to display when the button is toggled.
 *
 * \param bm wxBitmap&	The bitmap.
 * \return void
 *
 */
void mmMultiButton::SetToggleBitmap(wxBitmap &bm)
{
    mToggleBitmap = bm;
    if(!mDefaultBitmap.IsOk())
        mDefaultBitmap = bm;
    Refresh();
} // SetToggleBitmap

/*! \brief Set the button's label.
 *
 * \param label wxString	The label.
 * \return void
 *
 */
void mmMultiButton::SetLabel(wxString label)
// Sets the string label.
{
    mLabelStr = label;
    Refresh();
} // SetLabel

/*! \brief Set the button's style flags.
 *
 * \param style const long	The style flags.
 * \return void
 *
 */
void mmMultiButton::SetStyle(const long style)
{
    mStyle = style;
    Refresh();
} // SetStyle

/*! \brief Set the button's focus state.
 *
 * \param hasFocus const bool	Focused if true, otherwise not.
 * \return void
 *
 */
void mmMultiButton::SetFocus(const bool hasFocus)
// Update button state.
{
    mHasFocus = hasFocus;
    Refresh();
} // SetSelected

/*! \brief Set the button's selected state.
 *
 * \param isSelected const bool	Selected if true, otherwise not.
 * \return void
 *
 */
void mmMultiButton::SetSelected(const bool isSelected)
// Update button state.
{
    mIsSelected = isSelected;
    Refresh();
} // SetSelected

/*! \brief Set the button's toggled state.
 *
 * \param isToggleDown const bool	Toggled if true, otherwise not.
 * \return void
 *
 */
void mmMultiButton::SetToggleDown(const bool isToggleDown)
// Update button state.
{
    mIsToggleDown = isToggleDown;
    Refresh();
} // SetToggleDown

/*! \brief Set the toggle state of the whole button.
 *
 * \param isWholeDropToggleDown const bool	Toggled if true, otherwise not.
 * \return void
 *
 */
void mmMultiButton::SetWholeDropToggleDown(const bool isWholeDropToggleDown)
// Update button state.
{
    mIsWholeDropToggleDown = isWholeDropToggleDown;
    Refresh();
} // SetWholeDropToggleDown

/*! \brief Set the toggle state of the small button segment.
 *
 * \param isDropToggleDown const bool	Toggled if true, otherwise not.
 * \return void
 *
 */
void mmMultiButton::SetDropToggleDown(const bool isDropToggleDown)
// Update button state.
{
    mIsDropToggleDown = isDropToggleDown;
    Refresh();
} // SetDropToggleDown

/*! \brief Check whether the mouse is on the button.
 *
 * \return bool True or false.
 *
 */
bool mmMultiButton::MouseIsOnButton()
{
    int cx = 0, cy = 0;
    ClientToScreen(&cx, &cy);
    int cw = 0, ch = 0;
    GetClientSize(&cw, &ch);
    int mpx, mpy;
    ::wxGetMousePosition(&mpx, &mpy);
    return(mpx >= cx && mpx <= cx + cw && mpy >= cy && mpy <= cy + ch);
} // MouseIsOnButton

/*! \brief Enable or disable the button.
 *
 * \param enable bool	Enabled if true, otherwise disabled.
 * \return bool	True on success, false otherwise.
 *
 */
bool mmMultiButton::Enable(bool enable)
{
    bool ret = wxWindowBase::Enable(enable);
    Refresh();
    return ret;
} // Enable

