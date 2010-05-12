//! \file mmLogonDialog.h
//
// Name     : mmLogonDialog
// Purpose  : Displays a logon dialog (username/(secret) password entry)
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#ifndef _MMLOGONDIALOG_INCLUDED
#define _MMLOGONDIALOG_INCLUDED

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/mmwxdef.h>

/*! \brief A simple log-on dialogue.
 */
class WXDLLIMPEXP_MMWX mmLogonDialog : public wxDialog
{
    public:
        mmLogonDialog(wxWindow *parent,
                      const wxWindowID id      = -1,
                      const wxString  &message = wxEmptyString,
                      const wxPoint   &pos     = wxDefaultPosition,
                      const wxSize    &size    = wxSize(245, 160),
                      const long       style   = 0);
        ~mmLogonDialog();

        void Clear();
        bool ShowModal(wxString *user = NULL, wxString *password = NULL);
        void SetMessage(wxString &msg);
        void SetValid(wxString &validUser, wxString &validPassword);
        bool IsValid();
        bool IsValid(wxString &user, wxString &password);

        /*! \brief Get the user name entered.
         *
         * \return wxString	The user name.
         *
         */
        wxString GetUserName()
        {
            if(mUserNameEntry)
                return mUserNameEntry->GetValue();
            return wxEmptyString;
        }
        /*! \brief Get the password entered.
         *
         * \return wxString	The password.
         *
         */
        wxString GetPassword()
        {
            if(mPasswordEntry)
                return mPasswordEntry->GetValue();
            return wxEmptyString;
        }

    private:
        DECLARE_EVENT_TABLE()

        void OnOkBtn(wxCommandEvent &event);
        void OnCancelBtn(wxCommandEvent &event);

        wxPanel      	*mPanel;						//!< The dialogue's panel.
        wxStaticText	*mUserNameLabel;	//!< the user name label.
        wxTextCtrl   	*mUserNameEntry;	//!< The user name text control.
        wxStaticText *mPasswordLabel;		//!< The password label.
        wxTextCtrl   	*mPasswordEntry;		//!<The password text control.
        wxStaticText 	*mMessage;				//!< The dialogue message text.
        wxButton     	*mOkBtn;						//!< The dialogue's "OK" button.
        wxButton     	*mCancelBtn;				//!< The dialogue's "Cancel" button.
        wxString      	mValidUser;					//!< The valid user name.
        wxString      	mValidPassword;		//!< The valid password.
        bool          		mBtnWasOk;				//!< The OK button was selected.
}; // class mmLogon

#endif

