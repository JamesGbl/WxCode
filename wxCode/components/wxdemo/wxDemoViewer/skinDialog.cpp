/***************************************************************
 * Name:      skinDialog.cpp
 * Purpose:   Implements class encapsulating the Skin selection dialog
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "skinDialog.h"

BEGIN_EVENT_TABLE(skinDialog, wxDialog)
    EVT_BUTTON(idOkButton, skinDialog::OnOkClick)
END_EVENT_TABLE()

skinDialog::skinDialog(wxWindow *parent, wxString demoDir, wxString exeDir) : wxDialog(parent, wxID_ANY, wxT("Change skin"), wxDefaultPosition, wxSize(250, -1), wxDEFAULT_DIALOG_STYLE)
{
    m_sDemoDir = demoDir;
    m_sExeDir = exeDir;

    SetBackgroundColour(wxColour(212, 225, 241));

    wxBoxSizer *sizerVertical = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *sizerHorizontal = new wxBoxSizer(wxHORIZONTAL);

    sizerVertical->Add(sizerHorizontal, 1, wxEXPAND);

    m_pCombo = new wxComboBox(this, idCombo, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0, NULL, wxCB_READONLY);
    m_pOkButton = new wxButton(this, idOkButton, wxT("Ok"), wxDefaultPosition, wxSize(-1, 16));

    sizerHorizontal->Add(m_pCombo, 0, wxEXPAND | wxALL, 3);
    sizerHorizontal->Add(m_pOkButton, 0, wxEXPAND | wxALL, 3);
    sizerVertical->Add(new wxStaticText(this, wxID_ANY, wxT("requies application restart"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER), 1, wxEXPAND, 3);

    FindFiles();

    SetSizerAndFit(sizerVertical);

    Centre();
}

skinDialog::~skinDialog()
{
    ;
}

void skinDialog::FindFiles()
{
    wxDir dir(m_sDemoDir);

    if(!dir.IsOpened())
    {
        wxMessageBox(wxT("Failed to open directory"));
        return;
    }

    wxString filename;

    bool cont = dir.GetFirst(&filename, wxT("*.demo"), wxDIR_FILES);
    while(cont)
    {
        m_pCombo->Append(filename);

        cont = dir.GetNext(&filename);
    }

    if(m_pCombo->GetCount()>0) m_pCombo->Select(0);
}

void skinDialog::OnOkClick(wxCommandEvent &event)
{
    int sel = m_pCombo->GetSelection();

    if(sel!=wxNOT_FOUND)
    {
        wxFileName projectDir(m_sDemoDir + wxFileName::GetPathSeparator());

        projectDir.MakeRelativeTo(m_sExeDir);

        wxFileName::SetCwd(m_sExeDir);

        wxFileOutputStream fout(wxT("autostart.conf"));

        if(fout.IsOk())
        {
            wxTextOutputStream tout(fout);

            tout << projectDir.GetPath(wxPATH_GET_SEPARATOR, wxPATH_UNIX) + m_pCombo->GetString(sel);

            fout.Close();

            wxMessageBox(wxT("Skin changed! Please restart the aplication."));
        }

        wxFileName::SetCwd(m_sDemoDir);
    }

    EndDialog(0);
}
