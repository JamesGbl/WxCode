/////////////////////////////////////////////////////////////////////////////
// Name:        contrib/samples/ogl/studio/dialogs.cpp
// Purpose:     Implements Studio dialogs
// Author:      Julian Smart
// Modified by:
// Created:     12/07/98
// RCS-ID:      $Id: dialogs.cpp,v 1.1 2007/03/28 15:15:50 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "dialogs.h"
#include "doc.h"
#include "view.h"
#include "app.h"

IMPLEMENT_CLASS(csLabelEditingDialog, wxDialog)

BEGIN_EVENT_TABLE(csLabelEditingDialog, wxDialog)
    EVT_BUTTON(wxID_OK, csLabelEditingDialog::OnOK)
END_EVENT_TABLE()

csLabelEditingDialog::csLabelEditingDialog(wxWindow* parent)
{
    //wxLoadFromResource(this, parent, wxT("shape_label_dialog"));

    // Accelerators
    const wxAcceleratorEntry accel[] =
    {
        wxAcceleratorEntry(wxACCEL_CTRL, WXK_RETURN, wxID_OK)
    };
    SetAcceleratorTable(wxAcceleratorTable(WXSIZEOF(accel), accel));

    Centre();

    wxTextCtrl* textCtrl = XRCCTRL(*this, "text", wxTextCtrl);
    wxASSERT(textCtrl);

//    textCtrl->SetAcceleratorTable(accel);

    textCtrl->SetFocus();
}

void csLabelEditingDialog::OnOK(wxCommandEvent& event)
{
   wxTextCtrl* textCtrl = XRCCTRL(*this, "text", wxTextCtrl);
     wxASSERT(textCtrl);

    SetShapeLabel(textCtrl->GetValue());

    event.Skip();
}

void csLabelEditingDialog::SetShapeLabel(const wxString& label)
{
    wxTextCtrl* textCtrl = XRCCTRL(*this, "text", wxTextCtrl);
    wxASSERT(textCtrl);

    m_label = label;

    textCtrl->SetValue(label);
}

IMPLEMENT_CLASS(csSettingsDialog, wxDialog)

BEGIN_EVENT_TABLE(csSettingsDialog, wxDialog)
    EVT_BUTTON(wxID_OK, csSettingsDialog::OnOK)
END_EVENT_TABLE()

#define PROPERTY_DIALOG_WIDTH   400
#define PROPERTY_DIALOG_HEIGHT  400

// For 400x400 settings dialog, size your panels to about 375x325 in dialog editor

csSettingsDialog::csSettingsDialog(wxWindow* parent):
    wxDialog(parent, wxID_ANY, wxT("Settings"), wxPoint(0, 0), wxSize(PROPERTY_DIALOG_WIDTH, PROPERTY_DIALOG_HEIGHT))
{
    m_generalSettings = NULL;
    m_diagramSettings = NULL;

    m_notebook = new wxNotebook(this, ID_PROPERTY_NOTEBOOK,
         wxPoint(2, 2), wxSize(PROPERTY_DIALOG_WIDTH - 4, PROPERTY_DIALOG_HEIGHT - 4));

    m_generalSettings = new wxPanel;

    #ifdef  _DEBUG
    //bool success =
    #endif
    //               wxLoadFromResource(m_generalSettings, m_notebook, wxT("general_settings_dialog"));
    //wxASSERT_MSG( (success), wxT("Could not load general settings panel."));
    m_notebook->AddPage(m_generalSettings, _("General"), true);

    m_diagramSettings = new wxPanel;

    #ifdef  _DEBUG
    //success =
    #endif
    //          wxLoadFromResource(m_diagramSettings, m_notebook, wxT("diagram_settings_dialog"));
    //wxASSERT_MSG( (success), wxT("Could not load diagram settings panel."));
    m_notebook->AddPage(m_diagramSettings, _("Diagram"));

    int largeButtonWidth = 70;
    int largeButtonHeight = 22;

    wxButton* okButton = new wxButton(this, wxID_OK, _("OK"), wxPoint(0, 0), wxSize(largeButtonWidth, largeButtonHeight));
    wxButton* cancelButton = new wxButton(this, wxID_CANCEL, _("Cancel"), wxPoint(0, 0), wxSize(largeButtonWidth, largeButtonHeight));
    wxButton* helpButton = new wxButton(this, wxID_HELP, _("Help"), wxPoint(0, 0), wxSize(largeButtonWidth, largeButtonHeight));

    // Constraints for the notebook
    wxLayoutConstraints *c = new wxLayoutConstraints;
    c->top.SameAs     (this, wxTop, 5);
    c->left.SameAs    (this, wxLeft, 5);
    c->right.SameAs   (this, wxRight, 5);
    c->bottom.SameAs  (cancelButton, wxTop, 5);
    m_notebook->SetConstraints(c);

    // Constraints for the Help button
    c = new wxLayoutConstraints;
    c->width.AsIs();
    c->height.AsIs();
    c->right.SameAs    (this, wxRight, 5);
    c->bottom.SameAs   (this, wxBottom, 5);
    helpButton->SetConstraints(c);

    // Constraints for the Cancel button
    c = new wxLayoutConstraints;
    c->width.AsIs();
    c->height.AsIs();
    c->right.SameAs    (helpButton, wxLeft, 5);
    c->bottom.SameAs   (this, wxBottom, 5);
    cancelButton->SetConstraints(c);

    // Constraints for the OK button
    c = new wxLayoutConstraints;
    c->width.AsIs();
    c->height.AsIs();
    c->right.SameAs    (cancelButton, wxLeft, 5);
    c->bottom.SameAs   (this, wxBottom, 5);
    okButton->SetConstraints(c);

    okButton->SetDefault();
    okButton->SetFocus();

    Layout();
    Centre();
}

void csSettingsDialog::OnOK(wxCommandEvent& event)
{
    event.Skip();
}

bool csSettingsDialog::TransferDataToWindow()
{
    wxTextCtrl* gridSpacing = XRCCTRL(*m_diagramSettings, "gridspacing", wxTextCtrl);
    wxASSERT_MSG(gridSpacing, wxT("Could not find grid spacing control."));

    wxChoice* gridStyle = XRCCTRL(*m_diagramSettings, "gridstyle", wxChoice);
    wxASSERT_MSG(gridStyle, wxT("Could not find grid style control."));

    gridStyle->SetSelection(wxGetApp().GetGridStyle());

    wxString str;
    str.Printf(wxT("%d"), wxGetApp().GetGridSpacing());
    gridSpacing->SetValue(str);

    return true;
}

bool csSettingsDialog::TransferDataFromWindow()
{
    wxTextCtrl* gridSpacing = XRCCTRL(*m_diagramSettings, "gridspacing", wxTextCtrl);
    wxASSERT_MSG(gridSpacing, wxT("Could not find grid spacing control."));

    wxChoice* gridStyle = XRCCTRL(*m_diagramSettings, "gridstyle", wxChoice);
    wxASSERT_MSG(gridStyle, wxT("Could not find grid style control."));

    wxGetApp().SetGridStyle(gridStyle->GetSelection());
    wxString str = gridSpacing->GetValue();
    long grid_spacing;
    str.ToLong( &grid_spacing);
    wxGetApp().SetGridSpacing(grid_spacing);

    if (wxGetApp().GetGridStyle() == csGRID_STYLE_DOTTED)
    {
        wxMessageBox(wxT("Dotted grid style not yet implemented."), wxGetApp().GetAppDisplayName(), wxICON_EXCLAMATION);
        return false;
    }

    // Apply settings to all open diagram documents
    for (wxObjectList::compatibility_iterator node = wxDocManager::GetDocumentManager()->GetDocuments().GetFirst();
         node;
         node = node->GetNext())
    {
        wxDocument* doc = wxStaticCast(node->GetData(), wxDocument);
        if (wxDynamicCast(doc, csDiagramDocument))
        {
            csDiagramDocument* diagramDoc = wxStaticCast(doc, csDiagramDocument);
            wxDiagram* diagram = diagramDoc->GetDiagram();

            diagram->SetGridSpacing((double) wxGetApp().GetGridSpacing());
            switch (wxGetApp().GetGridStyle())
            {
                case csGRID_STYLE_NONE:
                {
                    diagram->SetSnapToGrid(false);
                    break;
                }
                case csGRID_STYLE_INVISIBLE:
                {
                    diagram->SetSnapToGrid(true);
                    break;
                }
                case csGRID_STYLE_DOTTED:
                {
                    // TODO (not implemented in OGL)
                    break;
                }
            }
        }
    }

    return true;
}

/*
 * Shape properties dialog (tabbed)
 */


IMPLEMENT_CLASS(csShapePropertiesDialog, wxDialog)

BEGIN_EVENT_TABLE(csShapePropertiesDialog, wxDialog)
    EVT_BUTTON(wxID_OK, csShapePropertiesDialog::OnOK)
END_EVENT_TABLE()

#define SHAPE_PROPERTY_DIALOG_WIDTH   400
#define SHAPE_PROPERTY_DIALOG_HEIGHT  400

// For 400x400 settings dialog, size your panels to about 375x325 in dialog editor

csShapePropertiesDialog::csShapePropertiesDialog(wxWindow* parent, const wxString& title,
    wxPanel* attributeDialog, const wxString& attributeDialogName):
    wxDialog(parent, wxID_ANY, title, wxPoint(0, 0), wxSize(SHAPE_PROPERTY_DIALOG_WIDTH, SHAPE_PROPERTY_DIALOG_HEIGHT))
{
    m_attributeDialog = attributeDialog;
    m_alternativeAttributeDialog = NULL;
    m_generalPropertiesDialog = NULL;

    m_notebook = new wxNotebook(this, ID_SHAPE_PROPERTY_NOTEBOOK,
         wxPoint(2, 2), wxSize(SHAPE_PROPERTY_DIALOG_WIDTH - 4, SHAPE_PROPERTY_DIALOG_HEIGHT - 4));

    m_generalPropertiesDialog = new csGeneralShapePropertiesDialog;
    #ifdef  _DEBUG
    //bool success =
    #endif
    //               wxLoadFromResource(m_generalPropertiesDialog, m_notebook, wxT("general_shape_properties_dialog"));
    //wxASSERT_MSG( (success), wxT("Could not load general properties panel."));
    m_notebook->AddPage(m_generalPropertiesDialog, _("General"));

    /*
    if (!wxLoadFromResource(m_attributeDialog, m_notebook, attributeDialogName))
    {
        wxMessageBox(_("Could not load the attribute dialog for this shape."), _("Studio"), wxICON_EXCLAMATION);
        wxDELETE(m_attributeDialog);
    }
    else
    */
    {
        m_notebook->AddPage(m_attributeDialog, _("Attributes"));
    }

    // Try the alternative dialog (test code)
    wxString str(attributeDialogName);
    str += wxT("1");
    m_alternativeAttributeDialog = new wxPanel;
    //if (wxLoadFromResource(m_alternativeAttributeDialog, m_notebook, str))
    {
        m_notebook->AddPage(m_alternativeAttributeDialog, _("Attributes (alternative)"));
    }
    //else
    {
        //wxDELETE(m_alternativeAttributeDialog);
    }

    int largeButtonWidth = 70;
    int largeButtonHeight = 22;

    wxButton* okButton = new wxButton(this, wxID_OK, _("OK"), wxPoint(0, 0), wxSize(largeButtonWidth, largeButtonHeight));
    wxButton* cancelButton = new wxButton(this, wxID_CANCEL, _("Cancel"), wxPoint(0, 0), wxSize(largeButtonWidth, largeButtonHeight));
    wxButton* helpButton = new wxButton(this, wxID_HELP, _("Help"), wxPoint(0, 0), wxSize(largeButtonWidth, largeButtonHeight));

    // Constraints for the notebook
    wxLayoutConstraints *c = new wxLayoutConstraints;
    c->top.SameAs     (this, wxTop, 5);
    c->left.SameAs    (this, wxLeft, 5);
    c->right.SameAs   (this, wxRight, 5);
    c->bottom.SameAs  (helpButton, wxTop, 5);
    m_notebook->SetConstraints(c);

    // Constraints for the Help button
    c = new wxLayoutConstraints;
    c->width.AsIs();
    c->height.AsIs();
    c->right.SameAs    (this, wxRight, 5);
    c->bottom.SameAs   (this, wxBottom, 5);
    helpButton->SetConstraints(c);

    // Constraints for the Cancel button
    c = new wxLayoutConstraints;
    c->width.AsIs();
    c->height.AsIs();
    c->right.SameAs    (helpButton, wxLeft, 5);
    c->bottom.SameAs   (this, wxBottom, 5);
    cancelButton->SetConstraints(c);

    // Constraints for the OK button
    c = new wxLayoutConstraints;
    c->width.AsIs();
    c->height.AsIs();
    c->right.SameAs    (cancelButton, wxLeft, 5);
    c->bottom.SameAs   (this, wxBottom, 5);
    okButton->SetConstraints(c);

    okButton->SetDefault();
    okButton->SetFocus();

    SetDefaults();

    Layout();
    Centre();
}

void csShapePropertiesDialog::OnOK(wxCommandEvent& event)
{
    wxTextCtrl* textCtrl = XRCCTRL(*m_generalPropertiesDialog, "text", wxTextCtrl);
    wxASSERT(textCtrl);

    m_generalPropertiesDialog->SetShapeLabel(textCtrl->GetValue());

    event.Skip();
}

// Set some suitable defaults in the attribute dialogs (in the first instance,
// just set all wxChoices to the first element)
void csShapePropertiesDialog::SetDefaults()
{
    if (!m_attributeDialog)
        return;

    wxWindowList::compatibility_iterator node;
    for (node = m_attributeDialog->GetChildren().GetFirst();
         node;
         node = node->GetNext())
    {
        wxWindow* child = wxStaticCast(node->GetData(), wxWindow);

        if (wxDynamicCast(child, wxChoice))
        {
            wxChoice* choice = wxStaticCast(child, wxChoice);
            choice->SetSelection(0);
        }
    }

    if (!m_alternativeAttributeDialog)
        return;

    for (node = m_alternativeAttributeDialog->GetChildren().GetFirst();
         node;
         node = node->GetNext())
    {
        wxWindow* child = wxStaticCast(node->GetData(), wxWindow);

        if (wxDynamicCast(child, wxChoice))
        {
            wxChoice* choice = wxStaticCast(child, wxChoice);
            choice->SetSelection(0);
        }
    }
}

/*
 * csGeneralShapePropertiesDialog
 */

IMPLEMENT_CLASS(csGeneralShapePropertiesDialog, wxPanel)

BEGIN_EVENT_TABLE(csGeneralShapePropertiesDialog, wxPanel)
END_EVENT_TABLE()

csGeneralShapePropertiesDialog::csGeneralShapePropertiesDialog()
{
}

void csGeneralShapePropertiesDialog::SetShapeLabel(const wxString& label)
{
    wxTextCtrl* textCtrl = XRCCTRL(*this, "text", wxTextCtrl);
    wxASSERT(textCtrl);

    m_label = label;

    textCtrl->SetValue(label);
}

/*
 * csThinRectangleDialog
 */

IMPLEMENT_CLASS(csThinRectangleDialog, wxPanel)

BEGIN_EVENT_TABLE(csThinRectangleDialog, wxPanel)
END_EVENT_TABLE()

csThinRectangleDialog::csThinRectangleDialog()
{
}

/*
 * csWideRectangleDialog
 */

IMPLEMENT_CLASS(csWideRectangleDialog, wxPanel)

BEGIN_EVENT_TABLE(csWideRectangleDialog, wxPanel)
END_EVENT_TABLE()

csWideRectangleDialog::csWideRectangleDialog()
{
}

/*
 * csTriangleDialog
 */

IMPLEMENT_CLASS(csTriangleDialog, wxPanel)

BEGIN_EVENT_TABLE(csTriangleDialog, wxPanel)
END_EVENT_TABLE()

csTriangleDialog::csTriangleDialog()
{
}

/*
 * csSemiCircleDialog
 */

IMPLEMENT_CLASS(csSemiCircleDialog, wxPanel)

BEGIN_EVENT_TABLE(csSemiCircleDialog, wxPanel)
END_EVENT_TABLE()

csSemiCircleDialog::csSemiCircleDialog()
{
}

/*
 * csCircleDialog
 */

IMPLEMENT_CLASS(csCircleDialog, wxPanel)

BEGIN_EVENT_TABLE(csCircleDialog, wxPanel)
END_EVENT_TABLE()

csCircleDialog::csCircleDialog()
{
}

/*
 * csCircleShadowDialog
 */

IMPLEMENT_CLASS(csCircleShadowDialog, wxPanel)

BEGIN_EVENT_TABLE(csCircleShadowDialog, wxPanel)
END_EVENT_TABLE()

csCircleShadowDialog::csCircleShadowDialog()
{
}

/*
 * csOctagonDialog
 */

IMPLEMENT_CLASS(csOctagonDialog, wxPanel)

BEGIN_EVENT_TABLE(csOctagonDialog, wxPanel)
END_EVENT_TABLE()

csOctagonDialog::csOctagonDialog()
{
}

/*
 * csGroupDialog
 */

IMPLEMENT_CLASS(csGroupDialog, wxPanel)

BEGIN_EVENT_TABLE(csGroupDialog, wxPanel)
END_EVENT_TABLE()

csGroupDialog::csGroupDialog()
{
}

/*
 * csTextBoxDialog
 */

IMPLEMENT_CLASS(csTextBoxDialog, wxPanel)

BEGIN_EVENT_TABLE(csTextBoxDialog, wxPanel)
END_EVENT_TABLE()

csTextBoxDialog::csTextBoxDialog()
{
}
