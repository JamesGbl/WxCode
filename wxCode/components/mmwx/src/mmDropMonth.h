//
// Name     : mmDropMonth.h
// Purpose  : Include file for mmDropMonth class
// Author   : Arne Morken
// Copyright: (C) 2000-2002, MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//

#ifndef _WXDROPMONTH_H_INCLUDED_
#define _WXDROPMONTH_H_INCLUDED_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "mmDropWindow.h"
#include "mmMonthCtrl.h"

#define mmDROPMONTH_UPDATE_LABEL 0x800

class mmDropMonth : public mmDropWindow
{
  public:
    mmDropMonth(wxWindow* parent,
                const wxWindowID  id,
	        const wxDateTime& date  = wxDefaultDateTime,
                const wxPoint&    pos   = wxDefaultPosition,
                const wxSize&     size  = wxDefaultSize,
		const long int    style = 0);
   ~mmDropMonth();

    wxDateTime& GetDate() { if (mMonthCtrl)
	                      return mMonthCtrl->GetDate();
			    return (wxDateTime&)wxDefaultDateTime;
                          }
    void SetDate(wxDateTime& date);

  private:
    DECLARE_EVENT_TABLE()

    void OnDateChange(mmMonthEvent& event);
    mmMonthCtrl* mMonthCtrl;
    bool mUpdateLabel;
}; // mmDropMonth

#endif

