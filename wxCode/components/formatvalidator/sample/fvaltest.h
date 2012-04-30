/////////////////////////////////////////////////////////////////////////////
// Name:        fvaltest.h
// Purpose:     Format and validate number sample header
// Author:      Manuel Martin
// Modified by: MM Apr 2012
// Created:     Mar 2003
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef FORSTRSAM_H
    #define FORSTRSAM_H

#ifdef __GNUG__
    #pragma interface "fvaltest.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    bool OnInit();
};

/////////////////////////////////////////////////////////////////////////////
// Define a special text control to manage TAB & ENTER
class MyTextCtrl : public wxTextCtrl
{
public:
    MyTextCtrl(wxWindow *parent, wxWindowID id,
               const wxString& value = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxTextCtrlNameStr)
        : wxTextCtrl(parent, id, value, pos, size, style, validator, name)
    {
        m_origFlags = style;
    }

    void OnChar(wxKeyEvent& event);
    void OnIdle(wxIdleEvent& event);

private:
    long m_origFlags;

    DECLARE_EVENT_TABLE()
};

/////////////////////////////////////////////////////////////////////////////
// A dialog to select two colors
class MyColorsDialog : public wxDialog
{
public:
    MyColorsDialog(wxWindow* parent, const wxString& label);
    void SelectAndSetColor(wxCommandEvent& event);

private:
    wxButton *butB;
    wxButton *butF;
    wxButton *butRestore;
    wxTextCtrl* txct;
public:
    wxColor defFcolor;
    wxColor defBcolor;
};

/////////////////////////////////////////////////////////////////////////////
// Define a special text control to select colors
class MyTextColorSel : public wxTextCtrl
{
public:
    MyTextColorSel(wxWindow *parent, wxWindowID id,
               const wxString& value = wxEmptyString)
        : wxTextCtrl(parent, id, value, wxDefaultPosition, wxDefaultSize, wxTE_READONLY)
    {}

    void OnClick(wxMouseEvent& event);
    void SetFGColors(const wxColor& fcolor, const wxColor& bcolor);
    //This text control is 'ReadOnly', so its default system colors are different.
    void SetDefColors(const wxColor& fcolor, const wxColor& bcolor);

private:
    wxColor m_myForeColor;
    wxColor m_myBackColor;

    DECLARE_EVENT_TABLE()
};

class GridFrame;
class myPanel;
/////////////////////////////////////////////////////////////////////////////
// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title);
    ~MyFrame();

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnToggleBell(wxCommandEvent& event);
    void OnDialog(wxCommandEvent& event);
    void OnGrid(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    //members
private:
    myPanel *mPanel;
    GridFrame* gf;

    //This macro allows this class to process wxWindows events
    DECLARE_EVENT_TABLE()
};

/////////////////////////////////////////////////////////////////////////////
class myPanel: public wxPanel
{
public:
    myPanel(wxWindow* parent);
    ~myPanel();

    void OnMenuBell(bool opt);
private:
    // event handlers (these functions should _not_ be virtual)
    void OnButtonFPressed(wxCommandEvent& event);
    void OnButtonUPressed(wxCommandEvent& event);
    void OnButtonVPressed(wxCommandEvent& event);
    void OnButtonTTWPressed(wxCommandEvent& event);
    void OnButtonTFWPressed(wxCommandEvent& event);
    void OnEscKey(wxKeyEvent& event);

    //members
    wxCheckBox* doRound;
    wxCheckBox* beStrict;
    wxCheckBox* leftrim0;
    wxCheckBox* rigtrim0;
    wxTextCtrl* yourstyle;
    wxTextCtrl* yournumb;
    wxTextCtrl* yourresul;
    wxTextCtrl* yourunfor;
    wxTextCtrl* yourvalsty;
    wxTextCtrl* yourvalstC;
    wxTextCtrl* yourvalAls;
    wxTextCtrl* yourvalRep;
    wxTextCtrl* yourvalmin;
    wxTextCtrl* yourvalmax;
    wxTextCtrl* yourvaldat;
    MyTextCtrl* yourtest;
    MyTextColorSel* yourgoodcolor;
    MyTextColorSel* yourfailcolor;
    wxCheckBox* vOnChar;
    wxCheckBox* vOnEnTab;
    wxCheckBox* vOnKillF;
    wxCheckBox* vOnRetaF;
    wxCheckBox* vTransUn;
    wxCheckBox* vEscAct;
    wxCheckBox* vNoMsg;
    wxCheckBox* vAllowEmpty;
    wxCheckBox* vMinLimit;
    wxCheckBox* vMaxLimit;

private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

/////////////////////////////////////////////////////////////////////////////
//A dialog with several controls
class MyDialog : public wxDialog
{
public:
    MyDialog(MyFrame* parent);

};

/////////////////////////////////////////////////////////////////////////////
//A grid for testing validation on it
class GridFrame : public wxFrame
{
public:
    GridFrame(MyFrame* mainWin);
    ~GridFrame();

private:
    wxGrid *m_grid;
};

/////////////////////////////////////////////////////////////////////////////
// IDs for the controls and the menu commands
enum
{
    FSN_QUIT = 100,
    FSN_BELL,
    FSN_DLG,
    FSN_GRID,
    FSN_ABOUT,
    FSVN_BUTF,  //the format button id
    FSVN_BUTU,  //the unformat button id
    FSVN_VRESET, //validate button reset
    FSVN_VTOWIN, //transfer to window
    FSVN_VFRWIN, //transfer from window
    FSVN_VALTXT, //TextControl where validation is done
    FSVN_OKCOL,   //TextControl for good value colors
    FSVN_FACOL,   //TextControl for failed value colors
    FSVN_DIALOG, //dialog id
    FSVN_GRID    //grid id
};

#endif // FORSTRSAM_H

