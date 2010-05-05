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

#include "wx/mmMultiButton.h"

#ifdef __MMDEBUG__
extern wxTextCtrl *gDebug; // For global debug output
#endif

#include "wx/mmLogonDialog.h"

BEGIN_EVENT_TABLE(mmLogonDialog, wxDialog)
    EVT_BUTTON(wxID_OK,    mmLogonDialog::OnOkBtn)
    EVT_BUTTON(wxID_CANCEL, mmLogonDialog::OnCancelBtn)
END_EVENT_TABLE()

wxString TITLE_TEXT    = _("Logon");
wxString USERNAME_TEXT = _("Username: ");
wxString PASSWORD_TEXT = _("Password: ");

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

bool mmLogonDialog::ShowModal(wxString *user, wxString *password)
{
    wxDialog::ShowModal();
    if(user)
        *user     = mUserNameEntry->GetValue();
    if(password)
        *password = mPasswordEntry->GetValue();
    return mBtnWasOk;
} // ShowModal

void mmLogonDialog::Clear()
{
    mUserNameEntry->SetValue(wxEmptyString);
    mPasswordEntry->SetValue(wxEmptyString);
    mMessage->SetLabel(wxEmptyString);
    mUserNameEntry->SetFocus();
}

void mmLogonDialog::SetMessage(wxString &msg)
{
    mMessage->SetLabel(msg);
} // SetMessage

void mmLogonDialog::OnOkBtn(wxCommandEvent &event)
{
    mBtnWasOk = TRUE;
    EndModal(wxID_OK);
} // OnOkBtn

void mmLogonDialog::OnCancelBtn(wxCommandEvent &event)
{
    mBtnWasOk = FALSE;
    EndModal(wxID_CANCEL);
} // OnCancelBtn

void mmLogonDialog::SetValid(wxString &validUser, wxString &validPassword)
{
    mValidUser = validUser;
    mValidPassword = validPassword;
} // SetValid

bool mmLogonDialog::IsValid()
{   // Checks dialog entry against previously set values
    return (mUserNameEntry->GetValue() == mValidUser && mPasswordEntry->GetValue() == mValidPassword);
} // IsValid

bool mmLogonDialog::IsValid(wxString &user, wxString &password)
{   // Checks dialog entry against given values
    return (mUserNameEntry->GetValue() == user && mPasswordEntry->GetValue() == password);
} // IsValid

