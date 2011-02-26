// stedoc.h

#ifndef _STEDOC_H_
#define _STEDOC_H_

class wxSTEditorDoc : public wxDocument, public wxSTEditorRefData
{
    DECLARE_DYNAMIC_CLASS(wxSTEditorDoc)
public:
    wxSTEditorDoc();

    virtual ~wxSTEditorDoc();
    virtual void SetFilename(const wxFileName& fileName, bool notifyViews = false)
    {
        wxSTEditorRefData::SetFilename(fileName, notifyViews);
        wxDocument::SetFilename(fileName.GetFullPath(), notifyViews);
    }
};

class wxSTEditorView : public wxView
{
    DECLARE_DYNAMIC_CLASS(wxSTEditorView)
public:
    virtual void OnDraw(wxDC*)
    {
    }
    virtual ~wxSTEditorView()
    {
    }
#if wxUSE_PRINTING_ARCHITECTURE
    virtual wxPrintout* OnCreatePrintout()
    {
        // return new wxSTEditorPrintout();
        return wxView::OnCreatePrintout();
    }
#endif
};

class wxSTEditorDocTemplate : public wxDocTemplate
{
   wxSTEditorDocTemplate(wxDocManager*);
public:
   static wxSTEditorDocTemplate* Create(wxDocManager*);
};

#endif  // _STEDOC_H_
// stedoc.h

#ifndef _STEDOC_H_
#define _STEDOC_H_

class wxSTEditorDoc : public wxDocument, public wxSTEditorRefData
{
    DECLARE_DYNAMIC_CLASS(wxSTEditorDoc)
public:
    wxSTEditorDoc();

    virtual ~wxSTEditorDoc();
    virtual void SetFilename(const wxFileName& fileName, bool notifyViews = false)
    {
        wxSTEditorRefData::SetFilename(fileName, notifyViews);
        wxDocument::SetFilename(fileName.GetFullPath(), notifyViews);
    }
};

class wxSTEditorView : public wxView
{
    DECLARE_DYNAMIC_CLASS(wxSTEditorView)
public:
    virtual void OnDraw(wxDC*)
    {
    }
    virtual ~wxSTEditorView()
    {
    }
#if wxUSE_PRINTING_ARCHITECTURE
    virtual wxPrintout* OnCreatePrintout()
    {
        // return new wxSTEditorPrintout();
        return wxView::OnCreatePrintout();
    }
#endif
};

class wxSTEditorDocTemplate : public wxDocTemplate
{
   wxSTEditorDocTemplate(wxDocManager*);
public:
   static wxSTEditorDocTemplate* Create(wxDocManager*);
};

#endif  // _STEDOC_H_
