//! \file mmDropMonth.h
//
// Name     : mmDropMonth.h
// Purpose  : Include file for mmDropMonth class
// Author   : Arne Morken
// Copyright: (C) 2000-2002, MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#ifndef _WXDROPMONTH_H_INCLUDED_
#define _WXDROPMONTH_H_INCLUDED_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/mmwxdef.h>
#include "mmDropWindow.h"
#include "mmMonthCtrl.h"

#define mmDROPMONTH_UPDATE_LABEL 0x800

/*! \brief An mmMultiButton with drop-down calendar.
 */
class WXDLLIMPEXP_MMWX mmDropMonth : public mmDropWindow
{
    public:
        mmDropMonth(wxWindow *parent,
                    const wxWindowID  id,
                    const wxDateTime &date  = wxDefaultDateTime,
                    const wxPoint    &pos   = wxDefaultPosition,
                    const wxSize     &size  = wxDefaultSize,
                    const long int    style = 0);
        ~mmDropMonth();

        /*! \brief Get the currently selected date.
         *
         * \return wxDateTime&	The date.
         *
         */
        wxDateTime &GetDate()
        {
            if(mMonthCtrl)
                return mMonthCtrl->GetDate();
            return (wxDateTime&)wxDefaultDateTime;
        }
        void SetDate(wxDateTime &date);

    private:
        DECLARE_EVENT_TABLE()

        void OnDateChange(mmMonthEvent &event);

        mmMonthCtrl *mMonthCtrl;								//!< The calendar control.
        bool 			mUpdateLabel;								//!< Whether to update the button label when the date changes.
}; // mmDropMonth

#endif

