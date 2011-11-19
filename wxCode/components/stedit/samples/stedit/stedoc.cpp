// stedoc.cpp

#include <wx/wxprec.h>
#include <wx/docview.h>
#include "wx/stedit/stedit.h"

#include "stedoc.h"

IMPLEMENT_DYNAMIC_CLASS(wxSTEditorDoc, wxDocument)
IMPLEMENT_DYNAMIC_CLASS(wxSTEditorView, wxView)

wxSTEditorDoc::wxSTEditorDoc() : wxDocument(), wxSTEditorRefData()
{
    wxSTEditorDocTemplate::GetInstance()->InitDocument(this, wxEmptyString);
}

wxSTEditorDoc::~wxSTEditorDoc()
{
    // Normally you destroy a document by calling wxDocManager::CloseDocument()
    // but wxSTEditorDoc is destroyed from elsewhere (from wxObject::UnRef).
    // Hence this roundabout method:
    GetFirstView()->SetDocument(NULL); // decouple, so the view can be destroyed without this document being deleting once more
    DeleteAllViews(); // destroy view
}

/*static*/ wxDocTemplate* wxSTEditorDocTemplate::ms_instance = NULL;

wxSTEditorDocTemplate::wxSTEditorDocTemplate(wxDocManager* docManager) : wxDocTemplate(docManager, _("Editor Files"), wxT("*.txt"),
      wxT(""), wxT("txt"), wxT("Editor doc"), wxT("Editor view"),
          CLASSINFO(wxSTEditorDoc), CLASSINFO(wxSTEditorView))
{
    ms_instance = this;
}

/*static*/ wxDocTemplate* wxSTEditorDocTemplate::Create(wxDocManager* docManager)
{
   return new wxSTEditorDocTemplate(docManager);
}
