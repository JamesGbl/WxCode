///////////////////////////////////////////////////////////////////////////////
// Name:        stedoc.h
// Purpose:     docview interaction
// Author:      Troels K
// Modified by:
// Created:     2009-11-11
// Copyright:   (c) John Labenski, Troels K
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

class wxSTEditorDoc : public wxDocument
{
    typedef wxDocument base;
protected:
public:
    wxSTEditorDoc();

    static wxDocTemplate* GetDocTemplate(wxDocManager* manager = NULL);

    wxWindow* GetModalParent() { return GetDocumentWindow(); }

public:
    virtual ~wxSTEditorDoc();
    virtual void OnChangeFilename(bool notifyViews);
protected:
};

/////////////////////////////////////////////////////////////////////////////
