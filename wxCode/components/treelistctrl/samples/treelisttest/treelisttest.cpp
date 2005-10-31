//////////////////////////////////////////////////////////////////////////////
// File:        treelisttest.cpp
// Purpose:     treelisttest application
// Maintainer:  Otto Wyss
// Created:     2004-12-21
// RCS-ID:      $Id: treelisttest.cpp,v 1.20 2005-10-31 17:17:02 wyo Exp $
// Copyright:   (c) 2004 wxCode
// Licence:     wxWindows
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------

// For compilers that support precompilation, includes <wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all 'standard' wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

//! wxWidgets headers
#include <wx/colordlg.h> // user colour dialog
#include <wx/fontdlg.h>  // user font dialog
#include <wx/numdlg.h>   // user number dialog
#include <wx/textdlg.h>  // text input dialog
#include <wx/image.h>    // image support
#include <wx/imaglist.h> // image list support

//! wxCode headers
#include "wx/treelistctrl.h" // wxTreeListCtrl control

// detecting memory leaks on Windows with _CrtSetBreakAlloc (<memory_number>)
#if defined(__WINDOWS__)
#include <crtdbg.h>
#endif

//----------------------------------------------------------------------------
// resources
//----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "treelisttest.xpm"
    #include "bitmaps/dirclosed.xpm"
    #include "bitmaps/diropened.xpm"
    #include "bitmaps/file.xpm"
#endif


//============================================================================
// declarations
//============================================================================

const wxString APP_NAME = _T("wxTreeListCtrl");
const wxString APP_VENDOR = _T("wxCode");
const wxString APP_VERSION = _T("1.0.0");
const wxString APP_MAINT = _T("Otto Wyss");
const wxString APP_LICENCE = _T("wxWindows");
const wxString APP_COPYRIGTH = _T("(C) 2005 Otto Wyss");

const wxString APP_DESCR = _("\
A tree list control presents information as a hierarchy, with \n\
items that may be expanded to show further items. Items in a \n\
tree list control are referenced by wxTreeItemId handles, \n\
which may be tested for validity by calling wxTreeItemId::IsOk.\
");
const wxString APP_WEBSITE = _T("http://wxcode.sourceforge.net/");

const wxString APP_INFOS = _("\
This application is derived from the demo sample of wyoGuide.\
");
const wxString APP_WYOGUIDE = _T("http://wyoguide.sourceforge.net");

// menu id's
enum {
    myID_DUMMY = wxID_HIGHEST,
    myID_ADDITEM,
    myID_INSERTAT,
    myID_DELETE,
    myID_FIND,
    myID_FINDFULL,
    myID_FINDEXPANDED,
    myID_FINDLEVEL,
    myID_FINDVISIBLE,
    myID_GOTO,
    myID_ATTRTEXTCOLOUR,
    myID_ATTRBACKCOLOUR,
    myID_ATTRBOLDFONT,
    myID_ATTRFONTSTYLE,
    myID_ATTRITEMIMAGE,
    myID_BUTTONSNORMAL,
    myID_BUTTONSTWISTER,
    myID_BUTTONSIMAGE,
    myID_HIDEROOT,
    myID_LINESATROOT,
    myID_LINESCONNECT,
    myID_ITEMBORDER,
    myID_ITEMHIGHLIGHT,
    myID_ITEMVARHEIGHT,
    myID_SHOWIMAGES,
    myID_ITEMEDIT,
    myID_ITEMVIRTUAL,
    myID_SELECTMULTIPLE,
    myID_SELECTEXTENDED,
    myID_SETATTRIBUTE,
    myID_GETCOUNT,
    myID_GETCHILDREN,
    myID_LISTALL,
    myID_LISTCHILDREN,
    myID_LISTSELECTED,
    myID_SETINDENT,
    myID_SETIMAGESIZE,
    myID_VETOEVENT,
};


//----------------------------------------------------------------------------
//! global application name
wxString g_appname;

//----------------------------------------------------------------------------
//! application
class App: public wxApp {
    friend class AppAbout;
    friend class AppFrame;

public:
    // standard overrides
    virtual bool OnInit();

    //! application exit function
    virtual int OnExit ();

private:
    AppFrame *m_frame;

};

// created dynamically by wxWidgets
DECLARE_APP (App);


//----------------------------------------------------------------------------
//! about box of the application APP_VENDOR-APP_NAME
class AppAbout: public wxDialog {

public:
    //! constructor
    AppAbout (wxWindow *parent, int milliseconds = 0);

    //! destructor
    ~AppAbout ();

    // event handlers
    void OnTimerEvent (wxTimerEvent &event);

private:
    // timer
    wxTimer *m_timer;

    DECLARE_EVENT_TABLE()
};


//----------------------------------------------------------------------------
//! frame of the application APP_VENDOR-APP_NAME.
class AppFrame: public wxFrame {

public:
    //! constructor
    AppFrame (const wxString &title);

    //! destructor
    ~AppFrame ();

    //! common event handlers
    void OnClose (wxCloseEvent &event);
    void OnAbout (wxCommandEvent &event);
    void OnExit (wxCommandEvent &event);
    void OnAddItem (wxCommandEvent &event);
    void OnInsertItem (wxCommandEvent &event);
    void OnDelete (wxCommandEvent &event);
    void OnFindItem (wxCommandEvent &event);
    void OnGotoItem (wxCommandEvent &event);
    void OnAttrTextColour (wxCommandEvent &event);
    void OnAttrBackColour (wxCommandEvent &event);
    void OnBoldFont (wxCommandEvent &event);
    void OnFontStyle (wxCommandEvent &event);
    void OnItemImage (wxCommandEvent &event);
    void OnButtonsNormals (wxCommandEvent &event);
    void OnButtonsTwister (wxCommandEvent &event);
    void OnButtonsImage (wxCommandEvent &event);
    void OnHideRoot (wxCommandEvent &event);
    void OnLinesAtRoot (wxCommandEvent &event);
    void OnLinesConnect (wxCommandEvent &event);
    void OnItemBorder (wxCommandEvent &event);
    void OnItemHighlight (wxCommandEvent &event);
    void OnItemVarHeight (wxCommandEvent &event);
    void OnShowImages (wxCommandEvent &event);
    void OnItemEditMode (wxCommandEvent &event);
    void OnItemVirtualMode (wxCommandEvent &event);
    void OnSelectMultiple (wxCommandEvent &event);
    void OnSelectExtended (wxCommandEvent &event);
    void OnGetCount (wxCommandEvent &event);
    void OnGetChildren (wxCommandEvent &event);
    void OnListAll (wxCommandEvent &event);
    void OnListChildren (wxCommandEvent &event);
    void OnListSelected (wxCommandEvent &event);
    void OnSetIndent (wxCommandEvent &event);
    void OnSetImageSize (wxCommandEvent &event);
    void OnVetoEvent (wxCommandEvent &event);
    void OnVetoingEvent (wxTreeEvent &event);

private:
    //! creates the application menu bar
    void CreateMenu ();

    // tree window
    wxTreeListCtrl* m_treelist;

    int m_imgsize;
    bool m_vetoEvent;

    void CheckStyle (int id, long flag);
    void ToggleStyle (int id, long flag);

    DECLARE_EVENT_TABLE()
};


//============================================================================
// implementation
//============================================================================

IMPLEMENT_APP (App)

//----------------------------------------------------------------------------
// App
//----------------------------------------------------------------------------

bool App::OnInit () {

#if defined(__WINDOWS__)
    // detecting memory leaks on Windows with _CrtSetBreakAlloc (<memory_number>)
    // _CrtSetBreakAlloc (13029);
#endif

    // set application and vendor name
    SetAppName (APP_NAME);
    SetVendorName (APP_VENDOR);
    g_appname.Append (APP_NAME);

    // about box shown for 3 seconds
    AppAbout (NULL, 3000);

    // create application frame
    m_frame = new AppFrame (APP_NAME);

    // open application frame
    m_frame->Layout ();
    m_frame->Show (true);
    SetTopWindow (m_frame);

    return true;
}

int App::OnExit () {
    return 0;
}


//----------------------------------------------------------------------------
// AppAbout
//----------------------------------------------------------------------------

const int myID_ABOUTTIMER = wxID_HIGHEST + 1;
BEGIN_EVENT_TABLE (AppAbout, wxDialog)
    EVT_TIMER (myID_ABOUTTIMER, AppAbout::OnTimerEvent)
END_EVENT_TABLE ()

AppAbout::AppAbout (wxWindow *parent,
                    int milliseconds)
        : wxDialog (parent, -1, _("About..."),
                    wxDefaultPosition, wxDefaultSize,
                    wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP) {

    // set timer if any
    m_timer = NULL;
    if (milliseconds > 0) {
        m_timer = new wxTimer (this, myID_ABOUTTIMER);
        m_timer->Start (milliseconds, wxTIMER_ONE_SHOT);
    }

    // about info
    wxFlexGridSizer *aboutinfo = new wxFlexGridSizer (2, 0, 2);
    aboutinfo->Add (new wxStaticText(this, -1, _("Vendor: ")),0, wxALIGN_LEFT);
    aboutinfo->Add (new wxStaticText(this, -1, APP_VENDOR),0, wxALIGN_LEFT);
    aboutinfo->Add (new wxStaticText(this, -1, _("Version: ")),0, wxALIGN_LEFT);
    aboutinfo->Add (new wxStaticText(this, -1, APP_VERSION),0, wxALIGN_LEFT);
    aboutinfo->Add (new wxStaticText(this, -1, _("Written by: ")),0, wxALIGN_LEFT);
    aboutinfo->Add (new wxStaticText(this, -1, APP_MAINT),0, wxALIGN_LEFT);
    aboutinfo->Add (new wxStaticText(this, -1, _("Licence type: ")),0, wxALIGN_LEFT);
    aboutinfo->Add (new wxStaticText(this, -1, APP_LICENCE),0, wxALIGN_LEFT);
    aboutinfo->Add (new wxStaticText(this, -1, _("Copyright: ")),0, wxALIGN_LEFT);
    aboutinfo->Add (new wxStaticText(this, -1, APP_COPYRIGTH),0, wxALIGN_LEFT);

    // about icontitle//info
    wxBoxSizer *aboutpane = new wxBoxSizer (wxHORIZONTAL);
    wxBitmap bitmap = wxBitmap(wxICON (treelisttest));
    aboutpane->Add (new wxStaticBitmap (this, -1, bitmap),
                    1, wxALIGN_CENTER | wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 20);
    aboutpane->Add (aboutinfo, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    aboutpane->Add (20, 0);

    // about complete
    wxBoxSizer *totalpane = new wxBoxSizer (wxVERTICAL);
    totalpane->Add (0, 10);
    wxStaticText *appname = new wxStaticText(this, -1, APP_NAME);
    appname->SetFont (wxFont (20, wxDEFAULT, wxNORMAL, wxBOLD));
    totalpane->Add (appname, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 40);
    totalpane->Add (0, 10);
    totalpane->Add (aboutpane, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    totalpane->Add (new wxStaticText(this, -1, wxGetTranslation(APP_DESCR)),
                    0, wxALIGN_LEFT | wxLEFT | wxRIGHT, 10);
    totalpane->Add (0, 6);
    totalpane->Add (new wxStaticText(this, -1, APP_WEBSITE),
                    0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    totalpane->Add (new wxStaticText(this, -1, wxGetTranslation(APP_INFOS)),
                    0, wxALIGN_LEFT | wxLEFT | wxRIGHT, 10);
    totalpane->Add (0, 6);
    totalpane->Add (new wxStaticText (this, -1, APP_WYOGUIDE),
                    0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    wxButton *okButton = new wxButton (this, wxID_OK, _("OK"));
    okButton->SetDefault();
    totalpane->Add (okButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizerAndFit (totalpane);
    CentreOnParent();
    ShowModal();
}

AppAbout::~AppAbout () {
    if (m_timer)  {
        delete m_timer;
        m_timer = NULL;
    }
}

//----------------------------------------------------------------------------
// event handlers
void AppAbout::OnTimerEvent (wxTimerEvent &WXUNUSED(event)) {
    if (m_timer) delete m_timer;
    m_timer = NULL;
    EndModal (wxID_OK);
}


//----------------------------------------------------------------------------
// AppFrame
//----------------------------------------------------------------------------

BEGIN_EVENT_TABLE (AppFrame, wxFrame)
    // common events
    EVT_CLOSE (                        AppFrame::OnClose)
    // file events
    EVT_MENU (wxID_EXIT,               AppFrame::OnExit)
    // edit events
    EVT_MENU (myID_ADDITEM,            AppFrame::OnAddItem)
    EVT_MENU (myID_INSERTAT,           AppFrame::OnInsertItem)
    EVT_MENU (myID_DELETE,             AppFrame::OnDelete)
    EVT_MENU (myID_FINDFULL,           AppFrame::OnFindItem)
    EVT_MENU (myID_FINDEXPANDED,       AppFrame::OnFindItem)
    EVT_MENU (myID_FINDLEVEL,          AppFrame::OnFindItem)
    EVT_MENU (myID_FINDVISIBLE,        AppFrame::OnFindItem)
    EVT_MENU (myID_GOTO,               AppFrame::OnGotoItem)
    // view events
    EVT_MENU (myID_ATTRTEXTCOLOUR,     AppFrame::OnAttrTextColour)
    EVT_MENU (myID_ATTRBACKCOLOUR,     AppFrame::OnAttrBackColour)
    EVT_MENU (myID_ATTRBOLDFONT,       AppFrame::OnBoldFont)
    EVT_MENU (myID_ATTRFONTSTYLE,      AppFrame::OnFontStyle)
    EVT_MENU (myID_ATTRITEMIMAGE,      AppFrame::OnItemImage)
    EVT_MENU (myID_BUTTONSNORMAL,      AppFrame::OnButtonsNormals)
    EVT_MENU (myID_BUTTONSTWISTER,     AppFrame::OnButtonsTwister)
    EVT_MENU (myID_BUTTONSIMAGE,       AppFrame::OnButtonsImage)
    EVT_MENU (myID_HIDEROOT,           AppFrame::OnHideRoot)
    EVT_MENU (myID_LINESATROOT,        AppFrame::OnLinesAtRoot)
    EVT_MENU (myID_LINESCONNECT,       AppFrame::OnLinesConnect)
    EVT_MENU (myID_ITEMBORDER,         AppFrame::OnItemBorder)
    EVT_MENU (myID_ITEMHIGHLIGHT,      AppFrame::OnItemHighlight)
    EVT_MENU (myID_ITEMVARHEIGHT,      AppFrame::OnItemVarHeight)
    EVT_MENU (myID_SHOWIMAGES,         AppFrame::OnShowImages)
    EVT_MENU (myID_ITEMEDIT,           AppFrame::OnItemEditMode)
    EVT_MENU (myID_ITEMVIRTUAL,        AppFrame::OnItemVirtualMode)
    EVT_MENU (myID_SELECTMULTIPLE,     AppFrame::OnSelectMultiple)
    EVT_MENU (myID_SELECTEXTENDED,     AppFrame::OnSelectExtended)
    // extra events
    EVT_MENU (myID_GETCOUNT,           AppFrame::OnGetCount)
    EVT_MENU (myID_GETCHILDREN,        AppFrame::OnGetChildren)
    EVT_MENU (myID_LISTALL,            AppFrame::OnListAll)
    EVT_MENU (myID_LISTCHILDREN,       AppFrame::OnListChildren)
    EVT_MENU (myID_LISTSELECTED,       AppFrame::OnListSelected)
    EVT_MENU (myID_SETINDENT,          AppFrame::OnSetIndent)
    EVT_MENU (myID_SETIMAGESIZE,       AppFrame::OnSetImageSize)
    EVT_MENU (myID_VETOEVENT,          AppFrame::OnVetoEvent)
    // help events
    EVT_MENU (wxID_ABOUT,              AppFrame::OnAbout)
    // tree
    EVT_TREE_BEGIN_LABEL_EDIT (-1,     AppFrame::OnVetoingEvent)
    EVT_TREE_END_LABEL_EDIT (-1,       AppFrame::OnVetoingEvent)
    EVT_TREE_ITEM_COLLAPSING (-1,      AppFrame::OnVetoingEvent)
    EVT_TREE_ITEM_EXPANDING (-1,       AppFrame::OnVetoingEvent)
    EVT_TREE_SEL_CHANGING (-1,         AppFrame::OnVetoingEvent)
END_EVENT_TABLE ()

AppFrame::AppFrame (const wxString &title)
        : wxFrame ((wxFrame *)NULL, -1, title, wxDefaultPosition, wxSize(480,320),
                    wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE) {

    // set icon and background
    SetIcon (wxICON (treelisttest));

    // create menu
    CreateMenu ();

    // set image size
    m_imgsize = -1;
    m_vetoEvent = false;

    // create tree
    m_treelist = new wxTreeListCtrl (this);
    CheckStyle (myID_BUTTONSNORMAL, wxTR_HAS_BUTTONS);
    CheckStyle (myID_BUTTONSTWISTER, wxTR_TWIST_BUTTONS);
    CheckStyle (myID_HIDEROOT, wxTR_HIDE_ROOT);
    CheckStyle (myID_LINESATROOT, wxTR_LINES_AT_ROOT);
    CheckStyle (myID_LINESCONNECT, wxTR_NO_LINES);
    CheckStyle (myID_ITEMBORDER, wxTR_ROW_LINES);
    CheckStyle (myID_ITEMHIGHLIGHT, wxTR_FULL_ROW_HIGHLIGHT);
    CheckStyle (myID_ITEMVARHEIGHT, wxTR_HAS_VARIABLE_ROW_HEIGHT);
    CheckStyle (myID_ITEMEDIT, wxTR_EDIT_LABELS);
    CheckStyle (myID_ITEMVIRTUAL, wxTR_VIRTUAL);
    CheckStyle (myID_SELECTMULTIPLE, wxTR_MULTIPLE);
    CheckStyle (myID_SELECTEXTENDED, wxTR_EXTENDED);

    // initialize tree
    wxSize sz = GetClientSize ();
    int k = wxMax (sz.x * 2/3, 200);
    m_treelist->AddColumn (_T("Main"), k - 32, wxALIGN_LEFT);
    m_treelist->SetColumnEditable (0, true);
    m_treelist->AddColumn (_T("Second"), wxMax (sz.x - k, 100), wxALIGN_LEFT);
    m_treelist->SetColumnEditable (1, true);
    m_treelist->SetColumnAlignment (1, wxALIGN_LEFT);
    wxTreeItemId root = m_treelist->AddRoot (_T("Root"));
    m_treelist->SetItemText (root, 1, wxString::Format (_T("Root, text 0")));
    wxTreeItemId parent;
    wxTreeItemId item;
    int n = 0;
    int m = 0;
    item = m_treelist->AppendItem (root, wxString::Format (_T("Item #%d"), ++n));
    m_treelist->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    parent = item;
    item = m_treelist->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    m_treelist->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = m_treelist->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    m_treelist->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = m_treelist->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    m_treelist->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = m_treelist->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    m_treelist->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = m_treelist->AppendItem (root, wxString::Format (_T("Item #%d"), ++n));
    m_treelist->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    parent = item;
    item = m_treelist->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    m_treelist->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = m_treelist->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    m_treelist->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = m_treelist->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    m_treelist->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = m_treelist->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    m_treelist->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = m_treelist->AppendItem (root, wxString::Format (_T("Item #%d"), ++n));
    m_treelist->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    parent = item;
    item = m_treelist->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    m_treelist->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = m_treelist->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    m_treelist->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = m_treelist->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    m_treelist->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = m_treelist->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    m_treelist->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    m_treelist->ExpandAll (root);

}

AppFrame::~AppFrame () {
    wxTreeItemId root = m_treelist->GetRootItem();
    m_treelist->DeleteChildren (root);
    m_treelist->DeleteRoot();
}

// common event handlers
void AppFrame::OnClose (wxCloseEvent &WXUNUSED(event)) {
    Destroy();
}

void AppFrame::OnAbout (wxCommandEvent &WXUNUSED(event)) {
    AppAbout (this);
}

void AppFrame::OnExit (wxCommandEvent &WXUNUSED(event)) {
    Close (true);
}

void AppFrame::OnAddItem (wxCommandEvent &WXUNUSED(event)) {
    wxString text = wxString::Format (_T("Item #%d"), m_treelist->GetCount()+1);
    m_treelist->AppendItem (m_treelist->GetSelection(), text);
}

void AppFrame::OnInsertItem (wxCommandEvent &WXUNUSED(event)) {
    wxString text = wxString::Format (_T("Item #%d"), m_treelist->GetCount()+1);
    wxTreeItemId c = m_treelist->GetSelection();
    m_treelist->InsertItem (m_treelist->GetItemParent(c), c, text);
}

void AppFrame::OnDelete (wxCommandEvent &WXUNUSED(event)) {
    m_treelist->Delete (m_treelist->GetSelection());
}

void AppFrame::OnFindItem (wxCommandEvent &event) {
    int mode = (wxTL_MODE_FIND_PARTIAL | wxTL_MODE_FIND_NOCASE);
    switch (event.GetId()) {
        case myID_FINDFULL:     {mode |= wxTL_MODE_NAV_FULLTREE; break;}
        case myID_FINDEXPANDED: {mode |= wxTL_MODE_NAV_EXPANDED; break;}
        case myID_FINDLEVEL:    {mode |= wxTL_MODE_NAV_LEVEL; break;}
        case myID_FINDVISIBLE:  {mode |= wxTL_MODE_NAV_VISIBLE; break;}
    }
    wxString text = wxGetTextFromUser (_T("Finds the item with the given text"),
                                       _T("Find text"));
    wxTreeItemId c = m_treelist->GetSelection();
    m_treelist->SelectItem (m_treelist->FindItem (c, text, mode));
}

void AppFrame::OnGotoItem (wxCommandEvent &WXUNUSED(event)) {
    int num = wxGetNumberFromUser (_T(""), _T("Enter the item number"),
                                   _T("Get value"));
    if (num < 0) return;
    wxTreeItemId item = m_treelist->GetRootItem();
    for (int i = 0; i < num; ++i) {
        item = m_treelist->GetNext (item);
        if (!item) break;
    }
    if (item) m_treelist->SelectItem (item);
}

void AppFrame::OnAttrTextColour (wxCommandEvent &WXUNUSED(event)) {
    wxTreeItemId c = m_treelist->GetSelection();
    wxColour col = wxGetColourFromUser (this, m_treelist->GetItemTextColour (c));
    if (col.Ok()) m_treelist->SetItemTextColour (c, col);
}

void AppFrame::OnAttrBackColour (wxCommandEvent &WXUNUSED(event)) {
    wxTreeItemId c = m_treelist->GetSelection();
    wxColour col = wxGetColourFromUser (this, m_treelist->GetItemBackgroundColour (c));
    if (col.Ok()) m_treelist->SetItemBackgroundColour (c, col);
}

void AppFrame::OnBoldFont (wxCommandEvent &WXUNUSED(event)) {
    bool bold = m_treelist->IsBold (m_treelist->GetSelection());
    m_treelist->SetItemBold (m_treelist->GetSelection(), !bold);
}

void AppFrame::OnFontStyle (wxCommandEvent &WXUNUSED(event)) {
    wxTreeItemId c = m_treelist->GetSelection();
    wxFont font = wxGetFontFromUser (this, m_treelist->GetItemFont (c));
    m_treelist->SetItemFont (c, font);
}

void AppFrame::OnItemImage (wxCommandEvent &WXUNUSED(event)) {
    wxTreeItemId c = m_treelist->GetSelection();
    int num = wxGetNumberFromUser (_T(""), _T("Enter the image number"),
                                   _T("Get number"), m_treelist->GetItemImage (c));
    if (num < 0) return;
    m_treelist->SetItemImage (c, wxTreeItemIcon_Normal, num);
}

void AppFrame::OnButtonsNormals (wxCommandEvent &event) {
    ToggleStyle (event.GetId(), wxTR_HAS_BUTTONS);
}

void AppFrame::OnButtonsTwister (wxCommandEvent &event) {
    ToggleStyle (event.GetId(), wxTR_TWIST_BUTTONS);
}

void AppFrame::OnButtonsImage (wxCommandEvent &WXUNUSED(event)) {
    if (m_treelist->GetButtonsImageList()) {
        m_treelist->SetButtonsImageList (NULL);
    }else{
        int size = wxBITMAP (file).GetWidth();
        wxImageList *images;
        if ((m_imgsize == -1) || (m_imgsize == size)) {
            images = new wxImageList (size, size, true);
            images->Add (wxBITMAP (dirclosed));
            images->Add (wxBITMAP (dirclosed));
            images->Add (wxBITMAP (diropened));
            images->Add (wxBITMAP (diropened));
        }else{
            images = new wxImageList (m_imgsize, m_imgsize, true);
            images->Add (wxBITMAP (dirclosed).ConvertToImage().Rescale (m_imgsize, m_imgsize));
            images->Add (wxBITMAP (dirclosed).ConvertToImage().Rescale (m_imgsize, m_imgsize));
            images->Add (wxBITMAP (diropened).ConvertToImage().Rescale (m_imgsize, m_imgsize));
            images->Add (wxBITMAP (diropened).ConvertToImage().Rescale (m_imgsize, m_imgsize));
        }
        m_treelist->AssignButtonsImageList (images);
    }
}

void AppFrame::OnHideRoot (wxCommandEvent &event) {
    ToggleStyle (event.GetId(), wxTR_HIDE_ROOT);
}

void AppFrame::OnLinesAtRoot (wxCommandEvent &event) {
    ToggleStyle (event.GetId(), wxTR_LINES_AT_ROOT);
}

void AppFrame::OnLinesConnect (wxCommandEvent &event) {
    ToggleStyle (event.GetId(), wxTR_NO_LINES);
}

void AppFrame::OnItemBorder (wxCommandEvent &event) {
    ToggleStyle (event.GetId(), wxTR_ROW_LINES);
}

void AppFrame::OnItemHighlight (wxCommandEvent &event) {
    ToggleStyle (event.GetId(), wxTR_FULL_ROW_HIGHLIGHT);
}

void AppFrame::OnItemVarHeight (wxCommandEvent &event) {
    ToggleStyle (event.GetId(), wxTR_HAS_VARIABLE_ROW_HEIGHT);
}

void AppFrame::OnShowImages (wxCommandEvent &WXUNUSED(event)) {
    if (m_treelist->GetImageList()) {
        m_treelist->SetImageList (NULL);
    }else{
        int size = wxBITMAP (file).GetWidth();
        wxImageList *images;
        if ((m_imgsize == -1) || (m_imgsize == size)) {
            images = new wxImageList (size, size, true);
            images->Add (wxBITMAP (dirclosed));
            images->Add (wxBITMAP (dirclosed));
            images->Add (wxBITMAP (diropened));
            images->Add (wxBITMAP (diropened));
            images->Add (wxBITMAP (file));
        }else{
            images = new wxImageList (m_imgsize, m_imgsize, true);
            images->Add (wxBITMAP (dirclosed).ConvertToImage().Rescale (m_imgsize, m_imgsize));
            images->Add (wxBITMAP (dirclosed).ConvertToImage().Rescale (m_imgsize, m_imgsize));
            images->Add (wxBITMAP (diropened).ConvertToImage().Rescale (m_imgsize, m_imgsize));
            images->Add (wxBITMAP (diropened).ConvertToImage().Rescale (m_imgsize, m_imgsize));
            images->Add (wxBITMAP (file).ConvertToImage().Rescale (m_imgsize, m_imgsize));
        }
        m_treelist->AssignImageList (images);
    }
}

void AppFrame::OnItemEditMode (wxCommandEvent &event) {
    ToggleStyle (event.GetId(), wxTR_EDIT_LABELS);
}

void AppFrame::OnItemVirtualMode (wxCommandEvent &event) {
    ToggleStyle (event.GetId(), wxTR_VIRTUAL);
}

void AppFrame::OnSelectMultiple (wxCommandEvent &event) {
    ToggleStyle (event.GetId(), wxTR_MULTIPLE);
}

void AppFrame::OnSelectExtended (wxCommandEvent &event) {
    ToggleStyle (event.GetId(), wxTR_EXTENDED);
}

void AppFrame::OnGetCount (wxCommandEvent &WXUNUSED(event)) {
    int count = m_treelist->GetCount();
    wxMessageBox (wxString::Format (_T("Total count of items: %d"), count),
                  _T("Total count"), wxOK | wxICON_INFORMATION);
}

void AppFrame::OnGetChildren (wxCommandEvent &WXUNUSED(event)) {
    int count = m_treelist->GetChildrenCount (m_treelist->GetSelection());
    wxMessageBox (wxString::Format (_T("Total count of items: %d"), count),
                  _T("Children count"), wxOK | wxICON_INFORMATION);
}

void AppFrame::OnListAll (wxCommandEvent &WXUNUSED(event)) {
    wxTreeItemId item = m_treelist->GetSelection();
    if (!item.IsOk()) item = m_treelist->GetRootItem ();
    wxTreeItemId last = m_treelist->GetNextSibling (item);
    wxString text;
    while (item.IsOk() && (item != last)) {
        text.Append (m_treelist->GetItemText (item) + _T("\n"));
        item = m_treelist->GetNext (item);
    }
    wxMessageBox (text, _T("All items"), wxOK | wxICON_INFORMATION);
}

void AppFrame::OnListChildren (wxCommandEvent &WXUNUSED(event)) {
    wxTreeItemId item = m_treelist->GetSelection();
    if (!item.IsOk()) item = m_treelist->GetRootItem ();
    wxString text;
    wxTreeItemIdValue cookie;
    item = m_treelist->GetFirstChild (item, cookie);
    while (item.IsOk()) {
        text.Append (m_treelist->GetItemText (item) + _T("\n"));
        item = m_treelist->GetNextSibling (item);
    }
    wxMessageBox (text, _T("Children items"), wxOK | wxICON_INFORMATION);
}

void AppFrame::OnListSelected (wxCommandEvent &WXUNUSED(event)) {
    wxArrayTreeItemIds items;
    int count = m_treelist->GetSelections (items);
    wxString text;
    for (int i = 0; i < count; ++i) {
        text.Append (m_treelist->GetItemText (items[i]) + _T("\n"));
    }
    wxMessageBox (text, _T("Selected items"), wxOK | wxICON_INFORMATION);
}

void AppFrame::OnSetIndent (wxCommandEvent &WXUNUSED(event)) {
    int indent = wxGetNumberFromUser (_T(""), _T("Enter the indent value"),
                                      _T("Get value"), m_treelist->GetIndent());
    if (indent > 0) m_treelist->SetIndent (indent);
}

void AppFrame::OnSetImageSize (wxCommandEvent &WXUNUSED(event)) {
    m_imgsize = wxGetNumberFromUser (_T(""), _T("Enter the image size"),
                                     _T("Get size"), m_imgsize);
    if (m_imgsize <= 0) return;
    wxImageList *images = m_treelist->GetImageList();
    if (!images) return;
    for (int i = 0; i < images->GetImageCount(); ++i) {
        images->Replace (i, wxBitmap(images->GetBitmap(i).ConvertToImage().Rescale (m_imgsize, m_imgsize)));
    }
}

void AppFrame::OnVetoEvent (wxCommandEvent &event) {
    m_vetoEvent = !m_vetoEvent;
    GetMenuBar()->Check (event.GetId(), m_vetoEvent);
}

void AppFrame::OnVetoingEvent (wxTreeEvent &event) {
    if (m_vetoEvent) event.Veto();
}

// private functions
void AppFrame::CreateMenu () {

    // File menu
    wxMenu *menuFile = new wxMenu;
    menuFile->Append (wxID_EXIT, _("&Quit\tCtrl+Q"));


    // find submenu
    wxMenu *menuFind = new wxMenu;
    menuFind->Append (myID_FINDFULL, _("full tree ..."));
    menuFind->Append (myID_FINDEXPANDED, _("expanded ..."));
    menuFind->Append (myID_FINDLEVEL, _("in level ..."));
    menuFind->Append (myID_FINDVISIBLE, _("visible ..."));

    // edit menu
    wxMenu *menuEdit = new wxMenu;
    menuEdit->Append (myID_ADDITEM, _T("&Append at parent"));
    menuEdit->Append (myID_INSERTAT, _T("&Insert after current"));
    menuEdit->Append (myID_DELETE, _T("&Delete this item"));
    menuEdit->AppendSeparator();
    menuEdit->Append (myID_FIND, _T("&Find item"), menuFind);
    menuEdit->Append (myID_GOTO, _T("&Goto item ..."));

    // Import submenu
    wxMenu *menuAttr = new wxMenu;
    menuAttr->Append (myID_ATTRTEXTCOLOUR, _("Text colour ..."));
    menuAttr->Append (myID_ATTRBACKCOLOUR, _("Background ..."));
    menuAttr->AppendCheckItem (myID_ATTRBOLDFONT, _("Bold font"));
    menuAttr->Append (myID_ATTRFONTSTYLE, _("Font style ..."));
    menuAttr->Append (myID_ATTRITEMIMAGE, _("Item image ..."));

    // view menu
    wxMenu *menuView = new wxMenu;
    menuView->AppendCheckItem (myID_BUTTONSNORMAL, _("Toggle &normal buttons"));
    menuView->AppendCheckItem (myID_BUTTONSTWISTER, _("Toggle &twister buttons"));
    menuView->AppendCheckItem (myID_BUTTONSIMAGE, _("Toggle image &buttons"));
    menuView->AppendSeparator();
    menuView->AppendCheckItem (myID_HIDEROOT, _("Toggle &hide root"));
    menuView->AppendCheckItem (myID_LINESATROOT, _("Toggle &lines at root"));
    menuView->AppendCheckItem (myID_LINESCONNECT, _("Toggle &connecting lines"));
    menuView->AppendCheckItem (myID_ITEMBORDER, _("Toggle &item border"));
    menuView->AppendCheckItem (myID_ITEMHIGHLIGHT, _("Toggle &full row highlight"));
    menuView->AppendCheckItem (myID_ITEMVARHEIGHT, _("Toggle &variable row height"));
    menuView->AppendCheckItem (myID_SHOWIMAGES, _("Toggle show ima&ges"));
    menuView->AppendSeparator();
    menuView->AppendCheckItem (myID_ITEMEDIT, _("Toggle &edit mode"));
    menuView->AppendCheckItem (myID_ITEMVIRTUAL, _("Toggle &virtual mode"));
    menuView->AppendCheckItem (myID_SELECTMULTIPLE, _("Toggle &select multiple"));
    menuView->AppendCheckItem (myID_SELECTEXTENDED, _("Toggle &select extended"));
    menuView->AppendSeparator();
    menuView->Append (myID_SETATTRIBUTE, _("Set &attribute"), menuAttr);

    // extra menu
    wxMenu *menuExtra = new wxMenu;
    menuExtra->Append (myID_GETCOUNT, _("Get total count"));
    menuExtra->Append (myID_GETCHILDREN, _("Get children count"));
    menuExtra->AppendSeparator();
    menuExtra->Append (myID_LISTALL, _("List all items"));
    menuExtra->Append (myID_LISTCHILDREN, _("List all children"));
    menuExtra->Append (myID_LISTSELECTED, _("List selected items"));
    menuExtra->AppendSeparator();
    menuExtra->Append (myID_SETINDENT, _("Set &Indent to ..."));
    menuExtra->Append (myID_SETIMAGESIZE, _("Set image si&ze..."));
    menuExtra->AppendSeparator();
    menuExtra->AppendCheckItem (myID_VETOEVENT, _("&Veto event"));

    // Help menu
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append (wxID_ABOUT, _("&About ...\tShift+F1"));

    // construct menu
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append (menuFile, _("&Tree"));
    menuBar->Append (menuEdit, _("&Edit"));
    menuBar->Append (menuView, _("&View"));
    menuBar->Append (menuExtra, _("&Extra"));
    menuBar->Append (menuHelp, _("&Help"));
    SetMenuBar (menuBar);

}

void AppFrame::CheckStyle (int id, long flag) {
    long style = m_treelist->GetWindowStyle();
    GetMenuBar()->Check (id, (style & flag) != 0);
}

void AppFrame::ToggleStyle (int id, long flag) {
    long style = m_treelist->GetWindowStyle();
    style = style ^ flag;
    m_treelist->SetWindowStyle (style);
    GetMenuBar()->Check (id, (style & flag) != 0);
}
