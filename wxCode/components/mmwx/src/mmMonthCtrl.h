//
// Name     : mmMonthCtrl
// Purpose  : A panel displaying the days of the months as numbers.
//            The user can click on a day to raise an event.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//

#ifndef _MMMONTH_H_INCLUDED
#define _MMMONTH_H_INCLUDED

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "mmNavigator.h"
#include <wx/datetime.h>

// mmMonthCtrl styles

#define mmSHOW_TODAY       0x01
#define mmSHOW_SELECT      0x02
#define mmSHOW_MOVE        0x04
#define mmSHOW_BORDER_DAYS 0x08
#define mmSHOW_ALL         (mmSHOW_TODAY | mmSHOW_SELECT | mmSHOW_MOVE | mmSHOW_BORDER_DAYS)
#define mmPREV_BTN         0x10
#define mmNEXT_BTN         0x20
#define mmRESCALE_FONTS    0x40

// Calendar events

const mmEVT_MONTHCTRL_FIRST = wxEVT_FIRST + 5500;

const wxEventType mmEVT_MONTH_DAY_CHANGED       = mmEVT_MONTHCTRL_FIRST + 1;
const wxEventType mmEVT_MONTH_MONTH_CHANGED     = mmEVT_MONTHCTRL_FIRST + 2;
const wxEventType mmEVT_MONTH_YEAR_CHANGED      = mmEVT_MONTHCTRL_FIRST + 3;
const wxEventType mmEVT_MONTH_WEEKDAY_CLICKED   = mmEVT_MONTHCTRL_FIRST + 4;
const wxEventType mmEVT_MONTH_DAY_DOUBLECLICKED = mmEVT_MONTHCTRL_FIRST + 5;

/////////////////////////////////////////////////////////////////////////////

class mmMonthCtrl : public wxWindow
{
  public:
    mmMonthCtrl(wxWindow* parent,
	        const wxWindowID  id    = -1,
	        const wxDateTime& date  = wxDefaultDateTime,
                const wxPoint&    pos   = wxDefaultPosition,
                const wxSize&     size  = wxDefaultSize,
	        const long int    style = mmSHOW_ALL); // Constructor
   ~mmMonthCtrl(); // Destructor

    void SetDate(const wxDateTime& date);
    void SetStyle(const long int style) { mStyle = style; SetDate(mDate); }
    void SetTodayColour(wxColour* col);
    void SetSelectColour(wxColour* col);
    void SetMonthBackgroundColour(wxColour* col);

    wxDateTime&  GetDate(void)  { return mDate; }
    mmNavigator* GetNavigator() { return mMonthNavigator; }

  private:
    DECLARE_EVENT_TABLE()

    void UpdateDays(wxDC& dc);
    void UpdateDay (wxDC& dc, const wxDateTime& date,
	            int daycount,int weekcount);

    void OnPaint     (wxPaintEvent& event);
    void OnMotion    (wxMouseEvent& event);
    void OnEnterLeave(wxMouseEvent& event);
    void OnLeftDown  (wxMouseEvent& event);
    void OnNavigator (mmNavigatorEvent& event);

    wxDateTime CoordToDate  (int x, int y);
    void       CalculateSize(wxDC& dc);
    void       GenerateEvent(wxEventType type);

    wxDateTime    mDate;
    long int      mStyle;
    int           mDaysOfWeek;
    int           mWeeksOfMonth;
    int           mLastX, mLastY;
    int           mCellW, mCellH;
    int           mHdrH,  mNavW, mNavH;
    mmNavigator*  mMonthNavigator;
    wxWindow*     mDayHeader;
    wxStaticText* mDayNames[7];
    wxColour*     mTodayColour;
    wxColour*     mSelectColour;
    wxColour*     mMonthBackgroundColour;
    wxPen*        mTodayPen;
    wxPen*        mSelectPen;
    wxPen*        mMonthBackgroundPen;
    wxBrush*      mMonthBackgroundBrush;
}; // mmMonthCtrl

/////////////////////////////////////////////////////////////////////////////

#define EVT_MONTH_DAY_CHANGED(id,fn) \
  DECLARE_EVENT_TABLE_ENTRY( mmEVT_MONTH_DAY_CHANGED,      id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &fn, (wxObject*)NULL ),
#define EVT_MONTH_MONTH_CHANGED(id,fn) \
  DECLARE_EVENT_TABLE_ENTRY( mmEVT_MONTH_MONTH_CHANGED,    id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &fn, (wxObject*)NULL ),
#define EVT_MONTH_YEAR_CHANGED(id,fn) \
  DECLARE_EVENT_TABLE_ENTRY( mmEVT_MONTH_YEAR_CHANGED,     id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &fn, (wxObject*)NULL ),
#define EVT_MONTH_WEEKDAY_CLICKED(id,fn) \
  DECLARE_EVENT_TABLE_ENTRY( mmEVT_MONTH_WEEKDAY_CLICKED,  id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &fn, (wxObject*)NULL ),
#define EVT_MONTH_DAY_DOUBLECLICKED(id,fn) \
  DECLARE_EVENT_TABLE_ENTRY( mmEVT_MONTH_DAY_DOUBLECLICKED,id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &fn, (wxObject*)NULL ),

class mmMonthEvent : public wxCommandEvent
{
  public:
    mmMonthEvent(mmMonthCtrl* month, wxEventType type);

    const wxDateTime&         GetDate()    const { return mDate; }
    const wxDateTime::WeekDay GetWeekDay() const { return mWeekDay; }

  private:
    wxDateTime          mDate;
    wxDateTime::WeekDay mWeekDay;
}; // mmMonthEvent

#endif
