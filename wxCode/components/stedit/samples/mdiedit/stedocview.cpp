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

EditorDoc::EditorDoc() : wxSTEditorDoc(false)
{
}

EditorDoc::~EditorDoc()
{
}

bool EditorDoc::OnCreate(const wxString& path, long flags)
{
    if ( !wxSTEditorDoc::OnCreate(path, flags) )
        return false;

/*
    // subscribe to changes in the text control to update the document state
    // when it's modified
    GetTextCtrl()->Connect
    (
        wxEVT_COMMAND_TEXT_UPDATED,
        wxCommandEventHandler(EditorDoc::OnTextChange),
        NULL,
        this
    );
*/
    return true;
}

bool EditorDoc::OnNewDocument()
{
    bool ok = wxSTEditorDoc::OnNewDocument();

    if (ok)
    {
        delete GetTextCtrl()->Attach(this);
    }
    return ok;
}

bool EditorDoc::DoOpenDocument(const wxString& filename)
{
    wxString str;
    wxFileInputStream stream(filename);
    bool ok = stream.IsOk() && LoadFileToString(&str, stream, wxFileName(filename));

    if (ok)
    {
        GetTextCtrl()->SetTextAndInitialize(str);
        delete GetTextCtrl()->Attach(this);
    }
    return ok;
}

bool EditorDoc::DoSaveDocument(const wxString& filename)
{
    wxFileOutputStream stream(filename);

    return stream.IsOk() && GetTextCtrl()->SaveFile(stream);
}

/*
bool EditorDoc::IsModified() const
{
    wxSTEditor* wnd = GetTextCtrl();

    return wxSTEditorDoc::IsModified() || (wnd && wnd->IsModified());
}

void EditorDoc::Modify(bool modified)
{
    wxSTEditorDoc::Modify(modified);
    wxSTEditor* wnd = GetTextCtrl();
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
*/

// ----------------------------------------------------------------------------
// EditorDoc implementation
// ----------------------------------------------------------------------------

wxSTEditor* EditorDoc::GetTextCtrl() const
{
    wxView* view = GetFirstView();

    return view ? wxStaticCast(view, EditorView)->GetWindow() : NULL;
}

// ----------------------------------------------------------------------------
// EditorView implementation
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(EditorView, wxSTEditorView)
    EVT_MENU(wxID_COPY, EditorView::OnCopy)
    EVT_MENU(wxID_PASTE, EditorView::OnPaste)
    EVT_MENU(wxID_SELECTALL, EditorView::OnSelectAll)
END_EVENT_TABLE()

bool EditorView::OnCreate(wxDocument *doc, long flags)
{
    bool ok = wxSTEditorView::OnCreate(doc, flags);
    
    if (ok)
    {
        wxFrame* frame = wxStaticCast(doc->GetDocumentTemplate(), EditorDocTemplate)->CreateViewFrame(this);
        wxASSERT(frame == GetFrame());
        wxSTEditor* text = new wxSTEditor();
        
        ok = text->Create(frame);
        if (ok)
        {
            m_text = text;
            frame->SetIcon(wxICON(text));
            frame->Show();
        }
        else
        {
            delete text;
        }
    }
    return ok;
}

void EditorView::OnChangeFilename()
{
    wxSTEditorView::OnChangeFilename();
}

bool EditorView::OnClose(bool deleteWindow)
{
    if ( !wxSTEditorView::OnClose(deleteWindow) )
        return false;

    Activate(false);

    if ( deleteWindow )
    {
        m_text->Attach(new wxSTEditorRefData());
        GetFrame()->Destroy();
        SetFrame(NULL);
    }
    return true;
}
