///////////////////////////////////////////////////////////////////////////////
// Name:        stedocview.cpp
// Purpose:     MDI wxSTEditor app
// Author:      Troels K
// Modified by:
// Created:     2012-01-19
// Copyright:   (c) Troels K
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "wx/stedit/stedit.h"
#include "stedocview.h"
#include "../../src/wxext.h"

IMPLEMENT_CLASS(wxSTEditorDocBase, wxDocument)
IMPLEMENT_DYNAMIC_CLASS(wxSTEditorDoc, wxSTEditorDocBase)
IMPLEMENT_DYNAMIC_CLASS(wxSTEditorView, wxView)
IMPLEMENT_DYNAMIC_CLASS(wxSTEditorChildFrame,wxDocMDIChildFrame)

/*static*/ wxDocTemplate* wxSTEditorDocTemplate::ms_instance = NULL;

wxSTEditorDocTemplate::wxSTEditorDocTemplate(wxDocManager* docManager, wxClassInfo* frameClassInfo) : 
    wxDocTemplate(docManager, _("Text"), wxT("*.txt;*.text;*.h;*.c;*.cpp"),
      wxT(""), wxT("txt"), wxT("Editor doc"), wxT("Editor view"),
          CLASSINFO(wxSTEditorDoc), CLASSINFO(wxSTEditorView)),
    m_frameClassInfo(frameClassInfo)
{
    ms_instance = this;
}

/*static*/ wxDocTemplate* wxSTEditorDocTemplate::Create(wxDocManager* docManager)
{
   return new wxSTEditorDocTemplate(docManager, CLASSINFO(wxSTEditorChildFrame));
}

wxFrame* wxSTEditorDocTemplate::CreateViewFrame(wxView* view)
{
    wxSTEditorChildFrame* subframe = wxStaticCast(m_frameClassInfo->CreateObject(), wxSTEditorChildFrame);

    if (subframe->Create(view, wxStaticCast(wxTheApp->GetTopWindow(), wxMDIParentFrame)))
    {
    }
    else
    {
        wxDELETE(subframe);
    }
    return subframe;
}

bool wxSTEditorChildFrame::Create(wxView* view, wxMDIParentFrame* frame)
{
    bool ok = wxDocMDIChildFrame::Create(view->GetDocument(), view, frame, wxID_ANY, wxEmptyString);

    if (ok)
    {
        wxMenuBar* menubar = new wxMenuBar();
        wxMenu* menu;

        menu = new wxMenu();
        menu->Append(wxID_NEW);
        menu->Append(wxID_OPEN);
        menu->Append(wxID_CLOSE, wxGetStockLabel(wxID_CLOSE) + wxT("\t") + _("Ctrl+W"));
        menu->Append(wxID_SAVE);
        menu->Append(wxID_SAVEAS, wxGetStockLabelEx(wxID_SAVEAS) + wxT("\t") + _("Ctrl+Shift+S"));
        menu->Append(wxID_REVERT, _("Re&vert..."));
        menu->AppendSeparator();
        menu->Append(wxID_PRINT);
        menu->Append(wxID_PRINT_SETUP, _("Print Set&up..."));
        menu->Append(wxID_PREVIEW, wxGetStockLabelEx(wxID_PREVIEW) + wxT("\t") + _("Ctrl+Shift+P"));
        menu->AppendSeparator();
        menu->Append(wxID_EXIT, wxGetStockLabel(wxID_EXIT) + wxT("\t") + _("Ctrl+Q"));
        menubar->Append(menu, wxGetStockLabel(wxID_FILE));

        menu = new wxMenu();
        menu->Append(wxID_COPY);
        menu->Append(wxID_PASTE);
        menu->Append(wxID_SELECTALL, wxGetStockLabel(wxID_SELECTALL) + wxT("\t") + _("Ctrl+A"));
        menubar->Append(menu, wxGetStockLabel(wxID_EDIT));

        menu = new wxMenu();
        menu->Append(wxID_ABOUT, wxGetStockLabel(wxID_ABOUT) + wxT("\t") + _("Shift+F1"));
        menubar->Append(menu, wxGetStockLabel(wxID_HELP));

        SetMenuBar(menubar);
    }
    return ok;
}

// ----------------------------------------------------------------------------
// wxSTEditorDocBase: wxDocument and wxTextCtrl married
// ----------------------------------------------------------------------------

bool wxSTEditorDocBase::OnCreate(const wxString& path, long flags)
{
    if ( !wxDocument::OnCreate(path, flags) )
        return false;

    // subscribe to changes in the text control to update the document state
    // when it's modified
    GetTextCtrl()->Connect
    (
        wxEVT_COMMAND_TEXT_UPDATED,
        wxCommandEventHandler(wxSTEditorDocBase::OnTextChange),
        NULL,
        this
    );

    return true;
}

// Since text windows have their own method for saving to/loading from files,
// we override DoSave/OpenDocument instead of Save/LoadObject
bool wxSTEditorDocBase::DoSaveDocument(const wxString& filename)
{
    return GetTextCtrl()->SaveFile(filename);
}

bool wxSTEditorDocBase::DoOpenDocument(const wxString& filename)
{
    if ( !GetTextCtrl()->LoadFile(filename) )
        return false;

    // we're not modified by the user yet
    Modify(false);

    return true;
}

bool wxSTEditorDocBase::IsModified() const
{
    wxTextCtrl* wnd = GetTextCtrl();
    return wxDocument::IsModified() || (wnd && wnd->IsModified());
}

void wxSTEditorDocBase::Modify(bool modified)
{
    wxDocument::Modify(modified);

    wxTextCtrl* wnd = GetTextCtrl();
    if (wnd && !modified)
    {
        wnd->DiscardEdits();
    }
}

void wxSTEditorDocBase::OnTextChange(wxCommandEvent& event)
{
    Modify(true);

    event.Skip();
}

// ----------------------------------------------------------------------------
// wxSTEditorDoc implementation
// ----------------------------------------------------------------------------

wxTextCtrl* wxSTEditorDoc::GetTextCtrl() const
{
    wxView* view = GetFirstView();
    return view ? wxStaticCast(view, wxSTEditorView)->GetText() : NULL;
}

// ----------------------------------------------------------------------------
// wxSTEditorView implementation
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxSTEditorView, wxView)
    EVT_MENU(wxID_COPY, wxSTEditorView::OnCopy)
    EVT_MENU(wxID_PASTE, wxSTEditorView::OnPaste)
    EVT_MENU(wxID_SELECTALL, wxSTEditorView::OnSelectAll)
END_EVENT_TABLE()

bool wxSTEditorView::OnCreate(wxDocument *doc, long flags)
{
    if ( !wxView::OnCreate(doc, flags) )
        return false;

    wxFrame* frame = wxStaticCast(doc->GetDocumentTemplate(), wxSTEditorDocTemplate)->CreateViewFrame(this);
    wxASSERT(frame == GetFrame());
    m_text = new wxTextCtrl(frame, wxID_ANY, wxEmptyString,
                            wxDefaultPosition, wxDefaultSize,
                            wxTE_MULTILINE);
    frame->SetIcon(wxICON(text));
    frame->Show();

    return true;
}

void wxSTEditorView::OnDraw(wxDC *WXUNUSED(dc))
{
    // nothing to do here, wxTextCtrl draws itself
}

bool wxSTEditorView::OnClose(bool deleteWindow)
{
    if ( !wxView::OnClose(deleteWindow) )
        return false;

    Activate(false);

    if ( deleteWindow )
    {
        GetFrame()->Destroy();
        SetFrame(NULL);
    }
    return true;
}
