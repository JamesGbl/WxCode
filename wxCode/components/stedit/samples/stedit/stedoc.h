// stedoc.h

#ifndef _STEDOC_H_
#define _STEDOC_H_

//-----------------------------------------------------------------------------
// wxSTEditorDoc/View are not really good wxWidgets doc/view citizens because
// - creation and destruction are not handled by wxDocManager
//     (see workarounds in wxSTEditorDoc ctor and dtor)
// - has only a dummy view instance
//     (not in sync with wxSTEditorRefData.m_editors)
// - wxView::GetFrame() returns NULL
//
// Still this module may be useful for
// - demonstating how to roll your own wxSTEditorRefData class type in user space
// - demonstating how to wire wxSTEditor/wxSTEditorRefData into the wxWidgets
//   doc/view framework (albeit in a somewhat limited way)
// - if your user app is built on the doc/view framework and has its own doc/views,
//   then wxSTEditor fits in better when it can produce a wxDocument instance
//   for you, eg you can implement "universal" File Open code that returns a non-NULL
//   wxDocument pointer always, if succesful,
//      wxDocument* OpenSomeFile(filename); // Returns a wxSTEditorDoc instance
//                                          // *or* some other wxDocument instance
// 
//-----------------------------------------------------------------------------

class wxSTEditorDoc : public wxDocument, public wxSTEditorRefData
{
    DECLARE_DYNAMIC_CLASS(wxSTEditorDoc)
public:
    wxSTEditorDoc();
    wxSTEditorDoc(bool living_in_wxSTEditorFrame);

    virtual ~wxSTEditorDoc();

    wxFileName GetFilename() const
    { 
        return wxSTEditorRefData::GetFilename();
    }

    virtual void SetFilename(const wxFileName& fileName, bool notifyViews = false)
    {
        wxSTEditorRefData::SetFilename(fileName, notifyViews);
        wxDocument::SetFilename(fileName.GetFullPath(), notifyViews);
    }
    virtual void Modify(bool mod)
    {
        wxSTEditorRefData::Modify(mod);
        wxDocument::Modify(mod);
    }
private:
    void Init();
    bool m_living_in_wxSTEditorFrame;
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
   static wxDocTemplate* ms_instance;
public:
   static wxDocTemplate* Create(wxDocManager*);
   static wxDocTemplate* GetInstance() { return ms_instance; }
};

#endif  // _STEDOC_H_
