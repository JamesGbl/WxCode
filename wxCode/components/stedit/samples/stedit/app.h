// app.h

#include "wx/app.h"

class CommandLine
{
// Attributes
public:
   enum wxLanguage m_lang;
   wxArrayString m_fileNames;
   bool m_recurse;

// Construction
public:
   CommandLine();
};

// ----------------------------------------------------------------------------
// wxStEditApp - the application class
// ----------------------------------------------------------------------------

class wxStEditApp : public wxApp
{
public:
    wxStEditApp();

    // wxApp overridden virtual functions
    virtual bool OnInit();
    virtual int OnExit();
    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

    void CreateShell();
    wxFrame* CreateHelpFrame(const wxString& caption, const char* text);
    void OnMenuEvent(wxCommandEvent& event);
    void OnSTEShellEvent(wxSTEditorEvent& event);

    wxSTEditorFrame* GetTopWindow();

protected:
    wxSTEditorOptions m_steOptions;
    CommandLine m_cmdline;
    wxLocale m_locale;
};
