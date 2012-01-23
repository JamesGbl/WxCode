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
#include "../stedit/stedoc.h"
#include "stedocview.h"
#include "../../src/wxext.h"

IMPLEMENT_DYNAMIC_CLASS(EditorDoc, wxSTEditorDoc)
IMPLEMENT_DYNAMIC_CLASS(EditorView, wxView)
IMPLEMENT_DYNAMIC_CLASS(EditorChildFrame,wxDocMDIChildFrame)

/*static*/ wxDocTemplate* EditorDocTemplate::ms_instance = NULL;

EditorDocTemplate::EditorDocTemplate(wxDocManager* docManager, wxClassInfo* frameClassInfo) : 
    wxDocTemplate(docManager, _("Text"), wxT("*.txt;*.text;*.h;*.c;*.cpp"),
      wxT(""), wxT("txt"), wxT("Editor doc"), wxT("Editor view"),
          CLASSINFO(EditorDoc), CLASSINFO(EditorView)),
    m_frameClassInfo(frameClassInfo)
{
    ms_instance = this;
}

/*static*/ wxDocTemplate* EditorDocTemplate::Create(wxDocManager* docManager)
{
   return new EditorDocTemplate(docManager, CLASSINFO(EditorChildFrame));
}

wxFrame* EditorDocTemplate::CreateViewFrame(wxView* view)
{
    EditorChildFrame* subframe = wxStaticCast(m_frameClassInfo->CreateObject(), EditorChildFrame);

    if (subframe->Create(view, wxStaticCast(wxTheApp->GetTopWindow(), wxMDIParentFrame)))
    {
    }
    else
    {
        wxDELETE(subframe);
    }
    return subframe;
}

bool EditorChildFrame::Create(wxView* view, wxMDIParentFrame* frame)
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
        menu->Append(wxID_PROPERTIES, wxGetStockLabelEx(wxID_PROPERTIES) + wxT("\t") + _("Alt+Enter"));
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
// EditorDoc
// ----------------------------------------------------------------------------

bool EditorDoc::OnCreate(const wxString& path, long flags)
{
    if ( !wxSTEditorDoc::OnCreate(path, flags) )
        return false;

    // subscribe to changes in the text control to update the document state
    // when it's modified
    GetTextCtrl()->Connect
    (
        wxEVT_COMMAND_TEXT_UPDATED,
        wxCommandEventHandler(EditorDoc::OnTextChange),
        NULL,
        this
    );

    return true;
}

// Since text windows have their own method for saving to/loading from files,
// we override DoSave/OpenDocument instead of Save/LoadObject
bool EditorDoc::DoSaveDocument(const wxString& filename)
{
    return GetTextCtrl()->SaveFile(filename);
}

bool EditorDoc::DoOpenDocument(const wxString& filename)
{
    wxString str;
    wxFileInputStream stream(filename);
    bool ok = stream.IsOk() && LoadFileToString(&str, stream, wxFileName(filename), wxSTEditorPrefs(), wxSTEditorLangs());

    if (ok)
    {
        GetTextCtrl()->SetValue(str);

        // we're not modified by the user yet
        Modify(false);

        SetFilename(filename); // call virtual version
    }
    return ok;
}

bool EditorDoc::IsModified() const
{
    wxTextCtrl* wnd = GetTextCtrl();
    return wxSTEditorDoc::IsModified() || (wnd && wnd->IsModified());
}

void EditorDoc::Modify(bool modified)
{
    wxSTEditorRefData::Modify(modified);
    wxSTEditorDoc::Modify(modified);

    wxTextCtrl* wnd = GetTextCtrl();
    if (wnd && !modified)
    {
        wnd->DiscardEdits();
    }
}

void EditorDoc::OnTextChange(wxCommandEvent& event)
{
    Modify(true);

    event.Skip();
}

// ----------------------------------------------------------------------------
// EditorDoc implementation
// ----------------------------------------------------------------------------

wxTextCtrl* EditorDoc::GetTextCtrl() const
{
    wxView* view = GetFirstView();
    return view ? wxStaticCast(view, EditorView)->GetText() : NULL;
}

// ----------------------------------------------------------------------------
// EditorView implementation
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(EditorView, wxView)
    EVT_MENU(wxID_COPY, EditorView::OnCopy)
    EVT_MENU(wxID_PASTE, EditorView::OnPaste)
    EVT_MENU(wxID_SELECTALL, EditorView::OnSelectAll)
END_EVENT_TABLE()

bool EditorView::OnCreate(wxDocument *doc, long flags)
{
    if ( !wxView::OnCreate(doc, flags) )
        return false;

    wxFrame* frame = wxStaticCast(doc->GetDocumentTemplate(), EditorDocTemplate)->CreateViewFrame(this);
    wxASSERT(frame == GetFrame());
    m_text = new wxTextCtrl(frame, wxID_ANY, wxEmptyString,
                            wxDefaultPosition, wxDefaultSize,
                            wxTE_MULTILINE);
    frame->SetIcon(wxICON(text));
    frame->Show();

    return true;
}

void EditorView::OnDraw(wxDC *WXUNUSED(dc))
{
    // nothing to do here, wxTextCtrl draws itself
}

bool EditorView::OnClose(bool deleteWindow)
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
