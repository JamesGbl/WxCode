//! \file mmDropMonth.cpp
//
// Name     : mmDropMonth.cpp
// Purpose  : mmDropMonth methods.
//            This is a mmMonthCtrl that can be dropped down.
// Author   : Arne Morken
// Copyright: (C) 2000-2002, MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#include <wx/mmwxdef.h>
#include "wx/mmDropMonth.h"

#ifdef __MMDEBUG__
	WXDLLIMPEXP_DATA_MMWX(extern wxTextCtrl *) gDebug;    //!< Global debug output window.
#endif

#define ID_MONTH_CTRL 8285

BEGIN_EVENT_TABLE(mmDropMonth, mmDropWindow)
    EVT_MONTH_DAY_CHANGED(ID_MONTH_CTRL, mmDropMonth::OnDateChange)
    EVT_NAVIGATOR_PREV2(-1, mmDropMonth::OnDateChange)
    EVT_NAVIGATOR_PREV(-1, mmDropMonth::OnDateChange)
    EVT_NAVIGATOR_NEXT(-1, mmDropMonth::OnDateChange)
    EVT_NAVIGATOR_NEXT2(-1, mmDropMonth::OnDateChange)
END_EVENT_TABLE()

/*! \brief Constructor.
 *
 * \param parent wxWindow*				The parent window.
 * \param id 		const wxWindowID	The ID of this window.
 * \param date 	wxDateTime&			The date to display on the button.
 * \param pos 	const wxPoint&			The button's position.
 * \param size 	const wxSize&			The button's size.
 * \param style 	const longint				The button's style.
 *
 */
mmDropMonth::mmDropMonth(wxWindow *parent,
                         const wxWindowID  id,
                         const wxDateTime &date,
                         const wxPoint    &pos,
                         const wxSize     &size,
                         const long int    style)
    : mmDropWindow(parent, id, wxT(" ") + date.FormatISODate() + wxT(" "), wxNullBitmap, pos, wxSize(80, 30), style | wxNO_BORDER | mmMB_DROPDOWN, NULL, size)
{
    mUpdateLabel = mStyle & mmDROPMONTH_UPDATE_LABEL;
    mMonthCtrl = new mmMonthCtrl(this, ID_MONTH_CTRL, date, wxDefaultPosition, size, wxRAISED_BORDER | mmPREV_BTN | mmNEXT_BTN | mmSHOW_ALL | mmRESCALE_FONTS);
    SetChild(mMonthCtrl, size);
} // Constructor

/*! \brief Destructor.
 */
mmDropMonth::~mmDropMonth()
{
    if(mMonthCtrl != NULL) {
        mMonthCtrl->Destroy();
        mMonthCtrl = NULL;
    }
} // Destructor

/*! \brief The date selection changed.
 *
 * \param event mmMonthEvent&	A reference to a mmMonthEvent object.
 * \return void
 *
 */
void mmDropMonth::OnDateChange(mmMonthEvent &event)
{
#ifdef __MMDEBUG__
    *gDebug << _("date:") << event.GetDate().FormatISODate() << wxT("\n");
#endif
    if(mUpdateLabel)
        SetLabel(wxT(" ") + event.GetDate().FormatISODate() + wxT(" "));
    if(event.GetEventType() == mmEVT_MONTH_DAY_CHANGED && mParent != NULL)
    {
        mParent->Show(FALSE);
        SetWholeDropToggleDown(FALSE);
        SetDropToggleDown(FALSE);
    }
    event.SetId(GetId());
    GetParent()->ProcessEvent(event);
} // OnDateChange

/*! \brief
 *
 * \param date wxDateTime&	A reference to a wxDateTime object.
 * \return void
 *
 */
void mmDropMonth::SetDate(wxDateTime &date)
{
    SetLabel(wxT(" ") + date.FormatISODate() + wxT(" "));
} // SetDate

