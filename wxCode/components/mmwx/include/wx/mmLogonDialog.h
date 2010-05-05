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

class mmLogonDialog : public wxDialog
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

        wxString GetUserName()
        {
            if(mUserNameEntry)
                return mUserNameEntry->GetValue();
            return wxEmptyString;
        }
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

        wxPanel      	*mPanel;
        wxStaticText *mUserNameLabel;
        wxTextCtrl   	*mUserNameEntry;
        wxStaticText *mPasswordLabel;
        wxTextCtrl   	*mPasswordEntry;
        wxStaticText *mMessage;
        wxButton     	*mOkBtn;
        wxButton     	*mCancelBtn;
        wxString      	mValidUser;
        wxString      	mValidPassword;
        bool          	mBtnWasOk;
}; // class mmLogon

#endif

