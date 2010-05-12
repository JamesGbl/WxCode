//! \file mmLogonDialog.cpp
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

#include <iostream>

#include <wx/mmwxdef.h>
#include "wx/mmMultiButton.h"
#include "wx/mmLogonDialog.h"

#ifdef __MMDEBUG__
	WXDLLIMPEXP_DATA_MMWX(extern wxTextCtrl*g) Debug;    //!< Global debug output window.
#endif

BEGIN_EVENT_TABLE(mmLogonDialog, wxDialog)
    EVT_BUTTON(wxID_OK,    mmLogonDialog::OnOkBtn)
    EVT_BUTTON(wxID_CANCEL, mmLogonDialog::OnCancelBtn)
END_EVENT_TABLE()

WXDLLIMPEXP_DATA_MMWX(wxString) TITLE_TEXT    = _("Logon");					//!< The title text.
WXDLLIMPEXP_DATA_MMWX(wxString) USERNAME_TEXT = _("Username: ");	//!< The user name.
WXDLLIMPEXP_DATA_MMWX(wxString) PASSWORD_TEXT = _("Password: ");	//!< The password.

/*! \brief Constructor.
 *
 * \param parent 	wxWindow*				The parent window.
 * \param id 			const wxWindowID	The ID of this window.
 * \param message const wxString&		The message text.
 * \param pos 		const wxPoint&			The button's position.
 * \param size 		const wxSize&			The button's size.
 * \param style 		const longint				The button's style.
 *
 */
mmLogonDialog::mmLogonDialog(wxWindow *parent,
                             const wxWindowID id,
                             const wxString  &message,
                             const wxPoint   &pos,
                             const wxSize    &size,
                             const long       style)
    : wxDialog(parent, id, TITLE_TEXT, pos, size, style | wxDEFAULT_DIALOG_STYLE | wxDIALOG_MODAL)
{
    mPanel = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxCLIP_CHILDREN);

    wxLayoutConstraints *c = new wxLayoutConstraints;
    c->left.SameAs(this, wxLeft);
    c->right.SameAs(this, wxRight);
    c->top.SameAs(this, wxTop);
    c->bottom.SameAs(this, wxBottom);
    mPanel->SetConstraints(c);

    mUserNameLabel = new wxStaticText(mPanel, -1, USERNAME_TEXT, wxPoint(25, 20), wxSize(60, 20), wxALIGN_LEFT);
    mUserNameEntry = new wxTextCtrl(mPanel, -1, wxEmptyString, wxPoint(90, 20), wxSize(110, 25));
    mPasswordLabel = new wxStaticText(mPanel, -1, PASSWORD_TEXT, wxPoint(25, 50), wxSize(60, 20), wxALIGN_LEFT);
    mPasswordEntry = new wxTextCtrl(mPanel, -1, wxEmptyString, wxPoint(90, 50), wxSize(110, 25), wxTE_PASSWORD);
    mMessage = new wxStaticText(mPanel, -1, message, wxPoint(20, 80), wxSize(400, 20), wxALIGN_LEFT);
    mOkBtn     = new wxButton(mPanel, wxID_OK, _("Ok"),    wxPoint(55, 95), wxSize(60, 25));
    mCancelBtn = new wxButton(mPanel, wxID_CANCEL, _("Cancel") , wxPoint(125, 95), wxSize(60, 25));
    mUserNameEntry->SetFocus();
    mOkBtn->SetDefault();

    mPanel->SetAutoLayout(TRUE);
    mPanel->Layout();
    SetAutoLayout(TRUE);
    Layout();
    Centre(wxBOTH);
} // Constructor

/*! \brief Destructor.
 */
mmLogonDialog::~mmLogonDialog()
{
    if(mUserNameLabel != NULL)
    {
        mUserNameLabel->Destroy();
        mUserNameLabel = NULL;
    }
    if(mUserNameEntry != NULL)
    {
        mUserNameEntry->Destroy();
        mUserNameEntry = NULL;
    }
    if(mPasswordLabel != NULL)
    {
        mPasswordLabel->Destroy();
        mPasswordLabel = NULL;
    }
    if(mPasswordEntry != NULL)
    {
        mPasswordEntry->Destroy();
        mPasswordEntry = NULL;
    }
} // Destructor

/*! \brief Show the dialogue modally.
 *
 * \param user 			wxString*	The user name entered.
 * \param password 	wxString*	The password entered.
 * \return bool
 *
 */
bool mmLogonDialog::ShowModal(wxString *user, wxString *password)
{
    wxDialog::ShowModal();
    if(user)
        *user     = mUserNameEntry->GetValue();
    if(password)
        *password = mPasswordEntry->GetValue();
    return mBtnWasOk;
} // ShowModal

/*! \brief Clear the dialogue fields.
 *
 * \return void
 *
 */
void mmLogonDialog::Clear()
{
    mUserNameEntry->SetValue(wxEmptyString);
    mPasswordEntry->SetValue(wxEmptyString);
    mMessage->SetLabel(wxEmptyString);
    mUserNameEntry->SetFocus();
}

/*! \brief Set the dialogue's message.
 *
 * \param msg wxString&	The message.
 * \return void
 *
 */
void mmLogonDialog::SetMessage(wxString &msg)
{
    mMessage->SetLabel(msg);
} // SetMessage

/*! \brief The OK button was pressed.
 *
 * \param event wxCommandEvent&	A reference to a wxCommandEvent object.
 * \return void
 *
 */
void mmLogonDialog::OnOkBtn(wxCommandEvent &event)
{
    mBtnWasOk = TRUE;
    EndModal(wxID_OK);
} // OnOkBtn

/*! \brief The Cancel button was pressed.
 *
 * \param event wxCommandEvent&	A reference to a wxCommandEvent object.
 * \return void
 *
 */
void mmLogonDialog::OnCancelBtn(wxCommandEvent &event)
{
    mBtnWasOk = FALSE;
    EndModal(wxID_CANCEL);
} // OnCancelBtn

/*! \brief Set the valid user name and password.
 *
 * \param validUser 		wxString& The user name.
 * \param validPassword wxString&	The password.
 * \return void
 *
 */
void mmLogonDialog::SetValid(wxString &validUser, wxString &validPassword)
{
    mValidUser = validUser;
    mValidPassword = validPassword;
} // SetValid

/*! \brief Check whether the user name and password entered are valid using previously set values.
 *
 * \return bool	True if valid, false otherwise.
 *
 */
bool mmLogonDialog::IsValid()
{   // Checks dialog entry against previously set values
    return (mUserNameEntry->GetValue() == mValidUser && mPasswordEntry->GetValue() == mValidPassword);
} // IsValid

/*! \brief Check whether the user name and password entered are valid using the values passed.
 *
 * \param user 			wxString&	The valid user name.
 * \param password 	wxString&	The valid password.
 * \return bool			True if valid, false otherwise.
 *
 */
bool mmLogonDialog::IsValid(wxString &user, wxString &password)
{   // Checks dialog entry against given values
    return (mUserNameEntry->GetValue() == user && mPasswordEntry->GetValue() == password);
} // IsValid

