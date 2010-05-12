//! \file mmMonthCtrl.cpp
//
// Name     : mmMonthCtrl
// Purpose  : A panel displaying the days of the months as numbers.
//            The user can click on a day to raise an event.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#include <wx/mmwxdef.h>
#include "wx/mmMonthCtrl.h"

#ifdef __MMDEBUG__
	WXDLLIMPEXP_DATA_MMWX(extern wxTextCtrl*) gDebug;    //!< Global debug output window.
#endif

wxColour mmPALE_RED(240, 200, 200);
wxColour mmPALE_GREY(230, 230, 230);
wxColour mmBRIGHT_ORANGE(255, 180, 120);
wxColour mmBRIGHT_BLUE(150, 170, 230);
wxBrush  mmBRIGHT_BLUE_BRUSH(mmBRIGHT_BLUE, wxSOLID);

#define mmID_MONTH_NAVIGATOR 4451

BEGIN_EVENT_TABLE(mmMonthCtrl, wxWindow)
    EVT_PAINT(mmMonthCtrl::OnPaint)
    EVT_LEAVE_WINDOW(mmMonthCtrl::OnEnterLeave)
    EVT_MOTION(mmMonthCtrl::OnMotion)
    EVT_LEFT_DOWN(mmMonthCtrl::OnLeftDown)
    EVT_NAVIGATOR_PREV2(mmID_MONTH_NAVIGATOR, mmMonthCtrl::OnNavigator)
    EVT_NAVIGATOR_PREV(mmID_MONTH_NAVIGATOR, mmMonthCtrl::OnNavigator)
    EVT_NAVIGATOR_NEXT(mmID_MONTH_NAVIGATOR, mmMonthCtrl::OnNavigator)
    EVT_NAVIGATOR_NEXT2(mmID_MONTH_NAVIGATOR, mmMonthCtrl::OnNavigator)
END_EVENT_TABLE()

/////////////////////////////////////////////////////////////////////////////

/*! \brief Constructor.
 *
 * \param month mmMonthCtrl*	The month control for which these events are being handled.
 * \param type 	wxEventType		The type of event.
 *
 */
mmMonthEvent::mmMonthEvent(mmMonthCtrl *month, wxEventType type)
    : wxCommandEvent(type, month->GetId())
{
#ifdef __MMDEBUG__
    *gDebug << _("id:") << month->GetId() << wxT("\n");
#endif
    mDate = month->GetDate();
} // mmMonthEvent constructor

/////////////////////////////////////////////////////////////////////////////

/*! \brief Constructor.
 *
 * \param parent wxWindow*				The parent window.
 * \param id 		const wxWindowID	The ID of this window.
 * \param date 	const wxDateTime&	The date to be displayed.
 * \param pos 	const wxPoint&			The button's position.
 * \param size 	const wxSize&			The button's size.
 * \param style 	const longint				The button's style.
 *
 */
mmMonthCtrl::mmMonthCtrl(wxWindow *parent,
                         const wxWindowID  id,
                         const wxDateTime &date,
                         const wxPoint    &pos,
                         const wxSize     &size,
                         const long int    style)
    : wxWindow(parent, id, pos, size, style)
{
    mTodayPen = NULL;
    mSelectPen = NULL;
    mMonthBackgroundPen = NULL;
    mMonthBackgroundBrush = NULL;

    SetTodayColour((wxColour*)wxRED);
    SetSelectColour(&mmBRIGHT_ORANGE);
    SetMonthBackgroundColour((wxColour*)wxWHITE);

    mDaysOfWeek   = 7;
    mWeeksOfMonth = 6;

    // Month name / year
    mmMultiButton *datelabel = new mmMultiButton(this, -1, wxEmptyString, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
    bool prev = style & mmPREV_BTN;
    bool next = style & mmNEXT_BTN;
    mMonthNavigator = new mmNavigator(this, mmID_MONTH_NAVIGATOR, datelabel, prev, next, prev, next, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
    wxLayoutConstraints *navcon = new wxLayoutConstraints;
    navcon->top.SameAs(this, wxTop);
    navcon->height.Absolute(20);
    navcon->left.SameAs(this, wxLeft);
    navcon->right.SameAs(this, wxRight);
    mMonthNavigator->SetConstraints(navcon);

    SetAutoLayout(TRUE);
    Layout();

    mLastX = 10000;
    mLastY = 10000;
    mStyle = style;
    SetDate(date);
} // mmMonthCtrl constructor

/*! \brief Destructor.
 */
mmMonthCtrl::~mmMonthCtrl()
{
    if(mMonthNavigator)
    {
        mMonthNavigator->Destroy();
        mMonthNavigator = NULL;
    }
} // Destructor

/*! \brief Set the colour in which to display today's date.
 *
 * \param col wxColour*	The colour.
 * \return void
 *
 */
void mmMonthCtrl::SetTodayColour(wxColour *col)
{
    mTodayColour = col;
    mTodayPen = wxThePenList->FindOrCreatePen(*col, 1, wxSOLID);
} // SetTodayColour

/*! \brief Set the colour in which to display the selected date.
 *
 * \param col wxColour*	The colour.
 * \return void
 *
 */
void mmMonthCtrl::SetSelectColour(wxColour *col)
{
    mSelectColour = col;
    mSelectPen = wxThePenList->FindOrCreatePen(*col, 1, wxSOLID);
} // SetSelectColour

/*! \brief Set the background colour.
 *
 * \param col wxColour*	The colour.
 * \return void
 *
 */
void mmMonthCtrl::SetMonthBackgroundColour(wxColour *col)
{
    mMonthBackgroundColour = col;
    mMonthBackgroundPen = wxThePenList->FindOrCreatePen(*col, 1, wxSOLID);
    mMonthBackgroundBrush = wxTheBrushList->FindOrCreateBrush(*col, wxSOLID);
    //mMonthBackgroundBrush = new wxBrush(*col,wxSOLID);
} // SetMonthBackgroundColour

/*! \brief Set the date.
 *
 * \param date const wxDateTime&	The date.
 * \return void
 *
 */
void mmMonthCtrl::SetDate(const wxDateTime &date)
// Update all
{
    mDate = date;
    wxClientDC dc(this);
    CalculateSize(dc);
    UpdateDays(dc);
} // Update

/*! \brief The mouse entered or left the control.
 *
 * \param event wxMouseEvent&	A reference to a wxMouseEvent object.
 * \return void
 *
 */
void mmMonthCtrl::OnEnterLeave(wxMouseEvent &event)
// Remove the select rectangle when leaving
{
    if(mCellW != 0 && mCellH != 0)
    {
        int  w, h;
        GetClientSize(&w, &h);
        if(mLastX >= 0           && mLastX < w &&
                mLastY >= mNavH + mHdrH && mLastY < h)
        {
            wxDateTime lastdate = CoordToDate(mLastX, mLastY);
            if((int)lastdate.GetJDN() != (int)mDate.GetJDN())
            {
                wxClientDC dc(this);
                dc.SetBrush(*wxTRANSPARENT_BRUSH);
                dc.SetPen(*mMonthBackgroundPen);
                dc.DrawRectangle(int(mLastX / mCellW)*mCellW + 1,
                                 int((mLastY - mNavH - mHdrH) / mCellH)*mCellH + mNavH + mHdrH + 1,
                                 mCellW - 2, mCellH - 2);
                dc.SetPen(*wxBLACK_PEN);
            }
        }
    }
} // OnEnterLeave

/*! \brief The mouse moved within the control
 *
 * \param event wxMouseEvent&	A reference to a wxMouseEvent object.
 * \return void
 *
 */
void mmMonthCtrl::OnMotion(wxMouseEvent &event)
// Draw/remove the 'select'-rectangle
{
    if(mCellW != 0 && mCellH != 0 && mStyle & mmSHOW_MOVE)
    {
        if(event.GetX() >= 0           && event.GetX() < mDaysOfWeek  * mCellW &&
                event.GetY() >= mNavH + mHdrH && event.GetY() < mWeeksOfMonth * mCellH + mNavH + mHdrH)
        {
            wxDateTime evtdate  = CoordToDate(event.GetX(), event.GetY());
            wxDateTime lastdate = CoordToDate(mLastX, mLastY);
            if(lastdate != evtdate)
            {
                wxClientDC dc(this);
                dc.SetBrush(*wxTRANSPARENT_BRUSH);
                // Remove previous select rectangle
                int last_x = int(mLastX / mCellW) * mCellW;
                int last_y = int((mLastY - mNavH - mHdrH) / mCellH) * mCellH + mNavH + mHdrH;
                dc.SetPen(*mMonthBackgroundPen);
                dc.DrawRectangle(last_x + 1, last_y + 1, mCellW - 2, mCellH - 2);
                if(evtdate.GetMonth() == mDate.GetMonth())
                {
                    // Draw new rectangle
                    int x = int(event.GetX() / mCellW) * mCellW;
                    int y = int((event.GetY() - mNavH - mHdrH) / mCellH) * mCellH + mNavH + mHdrH;
                    dc.SetPen(*wxBLACK_PEN);
                    dc.DrawRectangle(x + 1, y + 1, mCellW - 2, mCellH - 2);
                    UpdateDay(dc, lastdate, x, y);
                }
            }
            mLastX = event.GetX();
            mLastY = event.GetY();
        }
    }
} // OnMotion

/*! \brief The left mouse button was depressed.
 *
 * \param event wxMouseEvent&	A reference to a wxMouseEvent object.
 * \return void
 *
 */
void mmMonthCtrl::OnLeftDown(wxMouseEvent &event)
{
    if(event.GetX() > 0           && event.GetX() < mDaysOfWeek  * mCellW &&
            event.GetY() > mHdrH + mNavH && event.GetY() < mWeeksOfMonth * mCellH + mHdrH + mNavH)
    {
        int x = int(event.GetX() / mCellW) * mCellW;
        int y = int((event.GetY() - mHdrH - mNavH) / mCellH) * mCellH + mHdrH + mNavH;
        wxDateTime tmpdate = CoordToDate(x, y);
        if(tmpdate.GetMonth() == mDate.GetMonth())
        {
            SetDate(tmpdate);
            // Tell parent that a day was clicked and month panel was updated
            GenerateEvent(mmEVT_MONTH_DAY_CHANGED);
        }
    }
} // OnLeftDown

/*! \brief The navigator control was clicked.
 *
 * \param event mmNavigatorEvent&	A reference to a mmNavigatorEvent object.
 * \return void
 *
 */
void mmMonthCtrl::OnNavigator(mmNavigatorEvent &event)
{
    if(event.GetEventType() == mmEVT_NAVIGATOR_PREV2) {
        mDate -= wxDateSpan(0, 12);
    }
    else if(event.GetEventType() == mmEVT_NAVIGATOR_PREV) {
        mDate -= wxDateSpan(0, 1);
    }
    else if(event.GetEventType() == mmEVT_NAVIGATOR_NEXT) {
        mDate += wxDateSpan(0, 1);
    }
    else if(event.GetEventType() == mmEVT_NAVIGATOR_NEXT2) {
        mDate += wxDateSpan(0, 12);
    }

    SetDate(mDate);
    // Tell parent that a navigator button was clicked and month panel was updated
    GenerateEvent(event.GetEventType());
} // OnNavigator

/*! \brief A paint event occurred.
 *
 * \param event wxPaintEvent&	A reference to a wxPaintEvent object.
 * \return void
 *
 */
void mmMonthCtrl::OnPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);
    CalculateSize(dc);

    // Clear entire area (sans navigator)
    int  w, h;
    GetClientSize(&w, &h);
    dc.SetPen(*mMonthBackgroundPen);
    dc.SetBrush(*mMonthBackgroundBrush);
    dc.DrawRectangle(0, mNavH, w, h - mNavH);

    UpdateDays(dc);
} // OnPaint

/*! \brief Update the calendar display.
 *
 * \param dc wxDC&	The device context to draw on.
 * \return void
 *
 */
void mmMonthCtrl::UpdateDays(wxDC &dc)
// Update all dates
{
    // Init DC
    wxFont font = GetFont();
    dc.SetFont(font);
    if(mStyle & mmRESCALE_FONTS)
    {
        font.SetPointSize(int(mCellH / 2));
        dc.SetFont(font);
        mMonthNavigator->GetWindow()->SetFont(font);
    }

    CalculateSize(dc);
    dc.SetTextForeground(*wxBLACK);
    dc.SetPen(*wxBLACK_PEN);

    // Update navigator date label if date changed
    if(mMonthNavigator)
    {
        wxLayoutConstraints *navcon = new wxLayoutConstraints;
        navcon->top.SameAs(this, wxTop);
        navcon->height.Absolute(mNavH);
        navcon->left.SameAs(this, wxLeft);
        navcon->right.SameAs(this, wxRight);
        mMonthNavigator->SetConstraints(navcon);
        SetAutoLayout(TRUE);
        Layout();
        mmMultiButton *btn = (mmMultiButton*)mMonthNavigator->GetWindow();
        if(btn)
        {
            wxString month_label;
            month_label.Printf(wxT("%s %d "), mDate.GetMonthName(mDate.GetMonth()).c_str(), mDate.GetYear());
            int w, h;
            btn->GetSize(&w, &h);
            long dw, dh;
            dc.GetTextExtent(month_label, &dw, &dh);
#ifdef __MMDEBUG__
            //*gDebug<<"w:"<<w<<",dw:"<<dw<<"\n";
            //*gDebug<<"h:"<<h<<",dh:"<<dh<<"\n";
#endif
            if(dw > w)
                month_label.Printf(wxT("%s %d "), mDate.GetMonthName(mDate.GetMonth()).Left(3).c_str(), mDate.GetYear());
            //btn->SetSize(w,2*dh);
            //Layout();
            if(btn->GetLabel() != month_label)
                btn->SetLabel(month_label);
        }
    }

    // Day names
    dc.SetBrush(mmBRIGHT_BLUE_BRUSH);
    dc.DrawRectangle(0, mNavH, mNavW, mHdrH);
    wxDateTime tmpdate = wxDateTime(1, mDate.GetMonth(), mDate.GetYear());
    tmpdate.SetToPrevWeekDay(wxDateTime::Mon);
    tmpdate.Set(tmpdate.GetDay(), tmpdate.GetMonth(), tmpdate.GetYear(), 0, 0, 0, 0);
    for(int daycount = 0; daycount < mDaysOfWeek; daycount++)
    {
        wxString day_name = wxString(tmpdate.GetWeekDayName(tmpdate.GetWeekDay()));
        day_name = day_name.Left((unsigned int)2);
        long dw, dh;
        dc.GetTextExtent(day_name, &dw, &dh);
        dc.DrawText(day_name, daycount * mCellW + int(mCellW / 2 - dw / 2), mNavH + 1);
        tmpdate += wxDateSpan::Days(1);
    } // for

    // Draw dates
    tmpdate = wxDateTime(1, mDate.GetMonth(), mDate.GetYear());
    tmpdate.SetToPrevWeekDay(wxDateTime::Mon);
    tmpdate.Set(tmpdate.GetDay(), tmpdate.GetMonth(), tmpdate.GetYear(), 0, 0, 0, 0);
    for(int weekcount = 0; weekcount < mWeeksOfMonth; weekcount++) // Weeks
    {
        for(int daycount = 0; daycount < mDaysOfWeek; daycount++) // Days
        {
            UpdateDay(dc, tmpdate, daycount, weekcount);
            tmpdate += wxDateSpan::Days(1);
        }
    }
} // UpdateDays

/*! \brief Update the display for a given day when the mouse enters or leaves.
 *
 * \param dc 				wxDC&						The device context for drawing.
 * \param date 			const wxDateTime&	The date.
 * \param daycount 	int							The day number.
 * \param weekcount 	int							The week number.
 * \return void
 *
 */
void mmMonthCtrl::UpdateDay(wxDC &dc, const wxDateTime &date, int daycount, int weekcount)
{
    if(date.GetMonth() == mDate.GetMonth())
    {   // Date is in current month ('enabled')
        // Draw inner border, erase background
        if((int)date.GetJDN() == (int)wxDateTime::Today().GetJDN() && (mStyle & mmSHOW_TODAY))
            dc.SetPen(*mTodayPen); // Today's date
        else if((int)date.GetJDN() == (int)mDate.GetJDN() && (mStyle & mmSHOW_SELECT))
            dc.SetPen(*mSelectPen); // Selected (clicked)
        else
            dc.SetPen(*mMonthBackgroundPen); // Default (no inner border)
        dc.SetBrush(*mMonthBackgroundBrush); // Erase background
        dc.DrawRectangle(daycount * mCellW, weekcount * mCellH + mHdrH + mNavH, mCellW, mCellH);
        // Draw outer border
        if(((int)date.GetJDN() == (int)mDate.GetJDN() && (mStyle & mmSHOW_SELECT)))
            dc.SetPen(*mSelectPen);
        else
            dc.SetPen(*mMonthBackgroundPen);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(daycount * mCellW + 1, weekcount * mCellH + mHdrH + mNavH + 1, mCellW - 2, mCellH - 2);
        // Set text colour
        if(date.IsWorkDay())
            dc.SetTextForeground(*wxBLACK);
        else
            dc.SetTextForeground(*wxRED);
    }
    else
    {   // Date is not in current month ('disabled')
        // Erase background
        dc.SetPen(*mMonthBackgroundPen);
        dc.SetBrush(*mMonthBackgroundBrush);
        dc.DrawRectangle(daycount * mCellW + 1, weekcount * mCellH + mHdrH + mNavH + 1, mCellW - 2, mCellH - 2);
        if(!(mStyle & mmSHOW_BORDER_DAYS))
            return;
        // Set text colour
        if(date.IsWorkDay())
            dc.SetTextForeground(mmPALE_GREY);
        else
            dc.SetTextForeground(mmPALE_RED);
    }
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    // Draw text for this date
    wxString str;
    str.Printf(wxT(" %2d "), date.GetDay());
    long dw, dh;
    dc.GetTextExtent(str, &dw, &dh);
    dc.DrawText(str, daycount * mCellW + int(mCellW / 2 - dw / 2), weekcount * mCellH + mHdrH + mNavH + 1);
} // UpdateDay

/*! \brief Calculate the control's size.
 *
 * \param dc wxDC&
 * \return void
 *
 */
void mmMonthCtrl::CalculateSize(wxDC &dc)
{
    int  w, h;
    GetClientSize(&w, &h);
    /*
    if (mMonthNavigator)
      mMonthNavigator->GetSize(&mNavW,&mNavH);
    */
    long dw, dh, ddesc;
    dc.GetTextExtent(wxT("X_"), &dw, &dh, &ddesc);
    mNavH = dh + ddesc;
    mNavW = w;
    mHdrH = mNavH;
    mCellH = int((h - mNavH - mHdrH) / mWeeksOfMonth) - 1; // Cell height
    mCellW = int(w / mDaysOfWeek); // Cell width
#ifdef __MMDEBUG__
    //*gDebug<<"cli w,h:"<<w<<","<<h<<"\n";
    //*gDebug<<"nav w,h:"<<mNavW<<","<<mNavH<<"\n";
#endif
} // CalculateSize

/*! \brief Convert a set of coordinates to the date they represent on the calendar.
 *
 * \param x int
 * \param y int
 * \return wxDateTime
 *
 */
wxDateTime mmMonthCtrl::CoordToDate(int x, int y)
{
    wxDateTime tmpdate = mDate;
    tmpdate -= wxDateSpan::Days(mDate.GetDay());
    tmpdate -= wxDateSpan::Days(tmpdate.GetWeekDay() - 1);
    tmpdate += wxDateSpan::Days(((int((y - mHdrH - mNavH) / mCellH) % mWeeksOfMonth) * mDaysOfWeek + int(x / mCellW) % mDaysOfWeek));
    tmpdate.Set(tmpdate.GetDay(), tmpdate.GetMonth(), tmpdate.GetYear(), 0, 0, 0, 0);
    return tmpdate;
} // CoordToDate

/*! \brief Generate an event.
 *
 * \param type wxEventType	The event type.
 * \return void
 *
 */
void mmMonthCtrl::GenerateEvent(wxEventType type)
{
    mmMonthEvent ev(this, type);
    if(GetGrandParent())
        GetGrandParent()->ProcessEvent(ev); // NB! Nasty!
} // GenerateEvent

