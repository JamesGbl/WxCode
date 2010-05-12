//! \file mmMonthCtrl.h
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

#ifndef _MMMONTH_H_INCLUDED
#define _MMMONTH_H_INCLUDED

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/mmwxdef.h>
#include <wx/datetime.h>
#include "mmNavigator.h"

// mmMonthCtrl styles

#define mmSHOW_TODAY       			0x01
#define mmSHOW_SELECT      			0x02
#define mmSHOW_MOVE        			0x04
#define mmSHOW_BORDER_DAYS	0x08
#define mmSHOW_ALL         			(mmSHOW_TODAY | mmSHOW_SELECT | mmSHOW_MOVE | mmSHOW_BORDER_DAYS)
#define mmPREV_BTN         				0x10
#define mmNEXT_BTN         				0x20
#define mmRESCALE_FONTS    		0x40

// Calendar events

const int mmEVT_MONTHCTRL_FIRST = wxEVT_FIRST + 5500;

const wxEventType mmEVT_MONTH_DAY_CHANGED       = mmEVT_MONTHCTRL_FIRST + 1;
const wxEventType mmEVT_MONTH_MONTH_CHANGED     = mmEVT_MONTHCTRL_FIRST + 2;
const wxEventType mmEVT_MONTH_YEAR_CHANGED      = mmEVT_MONTHCTRL_FIRST + 3;
const wxEventType mmEVT_MONTH_WEEKDAY_CLICKED   = mmEVT_MONTHCTRL_FIRST + 4;
const wxEventType mmEVT_MONTH_DAY_DOUBLECLICKED = mmEVT_MONTHCTRL_FIRST + 5;

WXDLLIMPEXP_DATA_MMWX(extern wxColour) mmPALE_RED;
WXDLLIMPEXP_DATA_MMWX(extern wxColour) mmPALE_GREY;
WXDLLIMPEXP_DATA_MMWX(extern wxColour) mmBRIGHT_ORANGE;
WXDLLIMPEXP_DATA_MMWX(extern wxColour) mmBRIGHT_BLUE;
WXDLLIMPEXP_DATA_MMWX(extern wxBrush)  mmBRIGHT_BLUE_BRUSH;

/////////////////////////////////////////////////////////////////////////////

/*! \brief A calendar control.
 */
class WXDLLIMPEXP_MMWX mmMonthCtrl : public wxWindow
{
    public:
        mmMonthCtrl(wxWindow *parent,
                    const wxWindowID  id    = -1,
                    const wxDateTime &date  = wxDefaultDateTime,
                    const wxPoint    &pos   = wxDefaultPosition,
                    const wxSize     &size  = wxDefaultSize,
                    const long int    style = mmSHOW_ALL); // Constructor
        ~mmMonthCtrl(); // Destructor

        void SetDate(const wxDateTime &date);
        /*! \brief Set the control's style flags.
         *
         * \param style const longint	The style flags.
         * \return void
         *
         */
        void SetStyle(const long int style) {
            mStyle = style;
            SetDate(mDate);
        }
        void SetTodayColour(wxColour *col);
        void SetSelectColour(wxColour *col);
        void SetMonthBackgroundColour(wxColour *col);

        /*! \brief Get the selected date.
         *
         * \return wxDateTime& The date.
         *
         */
        wxDateTime &GetDate()
        {
            return mDate;
        }
        /*! \brief Get a pointer to the navigator control.
         *
         * \return mmNavigator*	The navigator pointer.
         *
         */
        mmNavigator *GetNavigator()
        {
            return mMonthNavigator;
        }

    private:
        DECLARE_EVENT_TABLE()

        void UpdateDays(wxDC &dc);
        void UpdateDay(wxDC &dc, const wxDateTime &date, int daycount, int weekcount);

        void OnPaint(wxPaintEvent &event);
        void OnMotion(wxMouseEvent &event);
        void OnEnterLeave(wxMouseEvent &event);
        void OnLeftDown(wxMouseEvent &event);
        void OnNavigator(mmNavigatorEvent &event);

        wxDateTime	CoordToDate(int x, int y);
        void       			CalculateSize(wxDC &dc);
        void       			GenerateEvent(wxEventType type);

        wxDateTime   	mDate;											//!< The date.
        long int      		mStyle;											//!< The control's style.
        int           			mDaysOfWeek;							//!< Max. number of days in a week.
        int           			mWeeksOfMonth;						//!< Max. number of weeks in a month.
        int           			mLastX;											//!< The last selection's x coordinate.
        int						mLastY;											//!< The last selection's y coordinate.
        int           			mCellW;										//!< The cell width.
        int						mCellH;											//!< The cell height.
        int           			mHdrH;											//!< The header's height.
        int  						mNavW;										//!< The navigation control's width.
        int						mNavH;											//!< The navigation control's height.
        mmNavigator 	*mMonthNavigator;						//!< The navigator control.
        wxWindow     	*mDayHeader;								//!< The window header.
        wxStaticText		*mDayNames[7];						//!< Day names.
        wxColour     		*mTodayColour;							//!< Today's date's colour.
        wxColour     		*mSelectColour;							//!< The selected date's colour.
        wxColour     		*mMonthBackgroundColour;	//!< Month background colour.
        wxPen        		*mTodayPen;								//!< The pen used for today's date.
        wxPen        		*mSelectPen;								//!< The pen used for the selected date .
        wxPen        		*mMonthBackgroundPen;			//!< The pen used for the month background .
        wxBrush      		*mMonthBackgroundBrush;		//!< The brush used for the month background .
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

/*! \brief Calendar control event.
 */
class mmMonthEvent : public wxCommandEvent
{
    public:
        mmMonthEvent(mmMonthCtrl *month, wxEventType type);

        /*! \brief Get the date.
         *
         * \return const wxDateTime&	The date.
         *
         */
        const wxDateTime &GetDate() const {return mDate;}

        /*! \brief Get the day of the week.
         *
         * \return const wxDateTime::WeekDay	The day of the week.
         *
         */
        const wxDateTime::WeekDay GetWeekDay() const {return mWeekDay;}

    private:
        wxDateTime          			mDate;				//!< The date.
        wxDateTime::WeekDay 	mWeekDay;		//!< The day of the week.
}; // mmMonthEvent

#endif
