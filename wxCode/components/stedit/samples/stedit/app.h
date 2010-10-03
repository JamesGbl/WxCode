// app.h

// ----------------------------------------------------------------------------
// wxStEditApp - the application class
// ----------------------------------------------------------------------------
class STEditorFrame;

class wxStEditApp : public wxApp
{
public:
    wxStEditApp() : wxApp() {}

    virtual bool OnInit();
    virtual int OnExit();
    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

    void CreateShell();
    bool ParseCmdLine(wxArrayString* fileNames, wxSTEditorOptions*, bool* recurse); // TODO: remove and use OnInitCmdLine + OnCmdLineParsed instead
    wxFrame* CreateHelpFrame(const wxString& caption, const char* text);
    void OnMenuEvent(wxCommandEvent& event);
    void OnSTEShellEvent(wxSTEditorEvent& event);

    STEditorFrame* GetTopWindow();
    wxLocale m_locale;
};
