///////////////////////////////////////////////////////////////////////////////
// Name:        stedoc.cpp
// Purpose:     docview interaction
// Author:      Troels K
// Modified by:
// Created:     2009-11-11
// Copyright:   (c) John Labenski, Troels K
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "wx/stedit/stedoc.h"

/////////////////////////////////////////////////////////////////////////////
// wxDatabaseDocument

wxSTEditorDoc::wxSTEditorDoc() : wxDocument()
{
}

wxSTEditorDoc::~wxSTEditorDoc()
{
}

void wxSTEditorDoc::OnChangeFilename(bool notifyViews)
{
#if (wxVERSION_NUMBER > 2900)
    base::OnChangeFilename(notifyViews);
#endif
    if (notifyViews)
    {
        //m_wnd->OnChangeFilename();
    }
}

/////////////////////////////////////////////////////////////////////////////
// EditorView

class EditorView : public wxView
{
    typedef wxView base;
    DECLARE_DYNAMIC_CLASS(EditorView)
public:
    EditorView() : wxView()
    {
    }
    virtual ~EditorView()
    {
    }
    virtual void OnDraw(wxDC*)
    {
    }
    virtual wxPrintout* OnCreatePrintout();
};

IMPLEMENT_DYNAMIC_CLASS(EditorView, wxView)

wxPrintout* EditorView::OnCreatePrintout()
{
    //wxSTEditorDoc* doc = wxStaticCast(GetDocument(), wxSTEditorDoc);
    //return printout;
    return base::OnCreatePrintout();
}

/*static*/ wxDocTemplate* wxSTEditorDoc::GetDocTemplate(wxDocManager* docManager)
{
    static wxDocTemplate* doctemplate = NULL;
    if (docManager)
    {
       doctemplate = new wxDocTemplate(docManager, wxT("Text Files"), wxT("*.")wxT("txt"), 
           wxT(""), wxT("txt"), wxT("text doc"), wxT("text view"),
           CLASSINFO(wxSTEditorDoc), CLASSINFO(EditorView));
    }
    return doctemplate;
}

/////////////////////////////////////////////////////////////////////////////
