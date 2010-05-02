//
// Name     : mmDropMonth.cpp
// Purpose  : mmDropMonth methods.
//            This is a mmMonthCtrl that can be dropped down.
// Author   : Arne Morken
// Copyright: (C) 2000-2002, MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//

#include "mmDropMonth.h"

#ifdef __MMDEBUG__
extern wxTextCtrl* gDebug;
#endif

#define ID_MONTH_CTRL 8285

BEGIN_EVENT_TABLE(mmDropMonth,mmDropWindow)
  EVT_MONTH_DAY_CHANGED(ID_MONTH_CTRL,mmDropMonth::OnDateChange)
  EVT_NAVIGATOR_PREV2  (-1,mmDropMonth::OnDateChange)
  EVT_NAVIGATOR_PREV   (-1,mmDropMonth::OnDateChange)
  EVT_NAVIGATOR_NEXT   (-1,mmDropMonth::OnDateChange)
  EVT_NAVIGATOR_NEXT2  (-1,mmDropMonth::OnDateChange)
END_EVENT_TABLE()

mmDropMonth::mmDropMonth(wxWindow* parent,
                         const wxWindowID  id,
	                 const wxDateTime& date,
                         const wxPoint&    pos,
                         const wxSize&     size,
			 const long int    style)
: mmDropWindow(parent,id," "+date.FormatISODate()+" ",wxNullBitmap,
               pos,wxSize(80,30),style|wxNO_BORDER|mmMB_DROPDOWN,NULL,size)
{
  mUpdateLabel = mStyle & mmDROPMONTH_UPDATE_LABEL;
  mMonthCtrl = new mmMonthCtrl(this,ID_MONTH_CTRL,date,
		               wxDefaultPosition,size,
			       wxRAISED_BORDER|mmPREV_BTN|mmNEXT_BTN|mmSHOW_ALL|mmRESCALE_FONTS);
  SetChild(mMonthCtrl,size);
} // Constructor

mmDropMonth::~mmDropMonth()
{
  if (mMonthCtrl != NULL) { mMonthCtrl->Destroy(); mMonthCtrl = NULL; }
} // Destructor

void mmDropMonth::OnDateChange(mmMonthEvent& event)
{
#ifdef __MMDEBUG__
  *gDebug<<"date:"<<event.GetDate().FormatISODate()<<"\n";
#endif
  if (mUpdateLabel)
    SetLabel(" "+event.GetDate().FormatISODate()+" ");
  if (event.GetEventType() == mmEVT_MONTH_DAY_CHANGED && mParent != NULL)
  {
    mParent->Show(FALSE);
    SetWholeDropToggleDown(FALSE);
    SetDropToggleDown(FALSE);
  }
  event.SetId(GetId());
  GetParent()->ProcessEvent(event);
} // OnDateChange

void mmDropMonth::SetDate(wxDateTime& date)
{
  SetLabel(" "+date.FormatISODate()+" ");
} // SetDate

