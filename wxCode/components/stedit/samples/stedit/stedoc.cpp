// stedoc.cpp

#include <wx/wxprec.h>
#include <wx/docview.h>
#include <wx/stedit/stedit.h>

#include "stedoc.h"

IMPLEMENT_DYNAMIC_CLASS(wxSTEditorDoc, wxDocument)
IMPLEMENT_DYNAMIC_CLASS(wxSTEditorView, wxView)

#if (wxVERSION_NUMBER >= 2902)
inline wxDocTemplate* wxDocManager_FindTemplate(wxDocManager* docManager, const wxClassInfo* docClassInfo)
{
   return docManager->FindTemplate(docClassInfo);
}
#else
static wxDocTemplate* wxDocManager_FindTemplate(wxDocManager* docManager, const wxClassInfo* classinfo)
{
   for (wxList::compatibility_iterator node = docManager->GetTemplates().GetFirst();
        node;
        node = node->GetNext())
   {
      wxDocTemplate* t = wxStaticCast(node->GetData(), wxDocTemplate);
      if (classinfo == t->GetDocClassInfo())
      {
         return t;
      }
   }
   return NULL;
}
#endif

wxSTEditorDoc::wxSTEditorDoc() : wxDocument(), wxSTEditorRefData()
{
    wxDocManager* docManager = wxDocManager::GetDocumentManager();
    wxDocTemplate* docTemplate = wxDocManager_FindTemplate(docManager, CLASSINFO(wxSTEditorDoc));
    docTemplate->InitDocument(this, wxEmptyString);
}

wxSTEditorDoc::~wxSTEditorDoc()
{
    // Normally you destroy a document by calling wxDocManager::CloseDocument()
    // but wxSTEditorDoc is destroyed from elsewhere (from wxObject::UnRef).
    // Hence this:
    GetFirstView()->SetDocument(NULL); // decouple, so the view can be destroyed without this document being deleting once more
    DeleteAllViews(); // destroy view
}

wxSTEditorDocTemplate::wxSTEditorDocTemplate(wxDocManager* docManager) : wxDocTemplate(docManager, _("Editor Files"), wxT("*.txt"),
      wxT(""), wxT("txt"), wxT("Editor doc"), wxT("Editor view"),
          CLASSINFO(wxSTEditorDoc), CLASSINFO(wxSTEditorView))
{
}

/*static*/ wxSTEditorDocTemplate* wxSTEditorDocTemplate::Create(wxDocManager* docManager)
{
   return new wxSTEditorDocTemplate(docManager);
}
