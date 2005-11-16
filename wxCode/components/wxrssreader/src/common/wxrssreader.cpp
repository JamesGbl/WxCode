/////////////////////////////////////////////////////////////////////////////// 
// Name:        wxRSSReader.cpp 
// Purpose:     RSS Reader for wxForum 
// Author:      Ryan Norton 
// Created:     06/27/2005 
// RCS-ID:      $Id: wxrssreader.cpp,v 1.1 2005-11-16 22:34:45 ryannpcs Exp $
// Copyright:   (c) Ryan Norton 
// Licence:     Public Domain 
/////////////////////////////////////////////////////////////////////////////// 

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// wxRSSReader
//
// This is a rather simple example of an RSS reader application with 
// wxWidgets.
// 
// To use this sample, simply select Load wxForum Feed from the file menu 
// to load the RSS feed of the wxForum (wxforum.org). Select Load Custom 
// Feed to load a feed from any site you want.
//
// The program will put the site in the left list control and the posts
// it recieved from the currently selected feed from the left list 
// control in the right list control. Single-click on a feed in the left 
// list control to see the posts in it and double-click to update the 
// posts. Double-click a post in the right list control to see a preview 
// of it in the lower-right window.
// 
// Max 80 characters per line here :).
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Known bugs with wxRSSReader:
//
// 1) Several refreshing bugs on non-msw platforms
// 2) Some HTML is not fully parsed in post preview
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// ============================================================================ 
// Definitions 
// ============================================================================ 

// ----------------------------------------------------------------------------
// Pre-compiled header stuff
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// ---------------------------------------------------------------------------- 
// Macros
// ---------------------------------------------------------------------------- 

// wxRSSHTMLTYPE - HTML window type to compile wxRSSReader with
//    wxMOZILLA uses wxMozillaBrowser from the wxMozilla library
//    wxIEHTMLWIN uses wxIEHtmlWin from the wxActiveX library
//    the default, 0,uses wx's built-in wxHtmlWindow    
#define wxIEHTMLWIN 1 
#define wxMOZILLA 2 
#define wxRSSHTMLTYPE 0 

// Version of wxRSSReader
#define WXRSSREADERVERSION 1.0

//Hack - if this is defined wxURL::GetInputStream is self-contained
//(it in not in wx 2.6.2)
#define GISC

// ---------------------------------------------------------------------------- 
// Globals
// ---------------------------------------------------------------------------- 
//Current location of wxForum RSS Feed
static const wxString g_szwxforumrssurl =
     wxT("http://wxforum.shadonet.com/rss.php?"
         "forums%5B%5D=1&threadCount=50");

// ---------------------------------------------------------------------------- 
// Headers 
// ---------------------------------------------------------------------------- 

#include "wx/app.h"         //for wxApp
#include "wx/menu.h"        //for wxMenu
#include "wx/frame.h"       //for wxFrame
#include "wx/sizer.h"       //for positioning controls/wxBoxSizer 
#include "wx/listctrl.h"    //for wxListCtrl 
#include "wx/url.h"         //for wxURL/wxURI
#include "wx/gauge.h"       //for wxGauge
#include "wx/msgdlg.h"      //for wxMessageBox function
#include "wx/thread.h"      //for wxThread and wxCriticalSection
#include "wx/timer.h"       //for wxTimer
#include "wx/textdlg.h"     //for wxGetTextFromUser function
#include "wx/filename.h"    //for wxFileName
#include "wx/config.h"      //for wxConfigBase
#include "wx/dynarray.h"    //for wxArrayPtrVoid

//HTML window type (see wxRSSHTMLTYPE above)
#if wxRSSHTMLTYPE == wxIEHTMLWIN 
#include "IEHtmlWin.h" 
#elif wxRSSHTMLTYPE == wxMOZILLA 
#include "wx/wxMozillaBrowser.h" 
#else 
#include "wx/html/htmlwin.h" 
#endif 

// ============================================================================ 
// Declarations 
// ============================================================================ 

// ---------------------------------------------------------------------------- 
//
// Enumurations 
//
// ---------------------------------------------------------------------------- 

// IDs for the controls and the menu commands 
enum 
{ 
    // Menu event IDs 
    wxID_LOADFEED = 10001, //NB: Mac does not support menu ids of 0 
    wxID_LOADWXFORUMFEED, 
    wxID_GOTOPOST, 
    wxID_SETUPDATEFEEDINTERVAL,
//    wxID_STOP,   [built-in to wxWidgets] 
//    wxID_ABOUT,  [built-in to wxWidgets] 
//    wxID_EXIT,   [built-in to wxWidgets] 
    // Control event IDs 
    wxID_SLIDER,    
    wxID_NOTEBOOK, 
    wxID_POSTLISTCTRL, 
    wxID_FEEDLISTCTRL, 
    wxID_GAUGE
}; 

// ---------------------------------------------------------------------------- 
//
// wxRSSFeedData - data stored for each feed
// wxRSSPostData - data stored for each post from a feed
//
// ---------------------------------------------------------------------------- 

struct wxRSSPostData 
{ 
    wxString sTitle; //title of post
    wxString sLink;  //url to post
    wxString sData;  //content of post
};

struct wxRSSFeedData 
{ 
    wxString sLink;         //url of RSS feed
    wxArrayPtrVoid aPosts;  //posts (wxRSSPostData) in feed
    class wxRSSFeedThread* 
           pLoadingThread;  //loading thread (NULL if not/done loading)
};

// ---------------------------------------------------------------------------- 
//
// wxRSSReaderApp 
//
// ---------------------------------------------------------------------------- 

class wxRSSReaderApp : public wxApp 
{ 
public: 
    virtual bool OnInit(); 
}; 

// ---------------------------------------------------------------------------- 
//
// wxRSSParser - simple XML parser
//
// ---------------------------------------------------------------------------- 

class wxRSSParser 
{ 
public: 
    bool Create(const wxString& loc);   //Load all data from a location
    void Create(wxInputStream* m_in);   //Load all data from stream
    wxString NextTag();                 //Scan to next tag in stream 
    bool Ok();                          //Return true if still parsing
    wxString TagData();                 //Parse and return current tag data
    void HTMLUnencode(wxString& s);     //Unencode some complex XML/HTML
    void TagEnd(wxString tag);          //Scan past end of specified tag
    void Reset();                       //Scan to beginning of stream
    
    wxString m_data;    //raw RSS data from site
    wxString m_tag;     //content of currently parsed XML tag
    size_t m_nLastPos;  //current position in the RSS data
}; 

// ---------------------------------------------------------------------------- 
//
// wxRSSFrame - main frame
//
// ---------------------------------------------------------------------------- 

class wxRSSFrame : public wxFrame 
{ 
public: 
    //Ctor/Dtor
    wxRSSFrame(const wxString& title); 

    //Menu event handlers
    void OnLoadwxForumFeed(wxCommandEvent& event); 
    void OnLoadCustomFeed(wxCommandEvent& event); 
    void OnQuit(wxCommandEvent& event); 
    void OnAbout(wxCommandEvent& event); 
    void OnGoToPost(wxCommandEvent& event); 
    void OnSetUpdateFeedInterval(wxCommandEvent& event);

    //List event handlers
    void OnPostActivated(wxListEvent& event); 
    void OnFeedActivated(wxListEvent& event); 

    //Key event handlers
    void OnKeyDown(wxKeyEvent& event);

    //Mouse event handlers
    void OnMouseEnter(wxMouseEvent& );

    //Close event handlers
    void OnClose(wxCloseEvent& ); 

    //Load a feed from a urlstring
    void LoadFeed(const wxString& s); 
    
    //load one from the feed list/perform actual loading 
    void DoLoadFeed(long lFeed, bool bInBackground = false);      

    //"Adds an item" to a list control
    static void AddItem(wxListView* list, const wxString& szString, 
                 void* lData, const wxString& szTitle = wxT("X")); 
private: 
    class wxRSSFeedThread* m_thread;       //loading thread of current feed
    class wxRSSFeedTimer* m_timer;         //updates status bar for feed
    class wxRSSUpdateTimer* m_updatetimer; //updates all feeds periodically
    class wxListView* m_postlist;          //list of posts from a feed
    class wxListView* m_feedlist;          //list of feeds
    wxGauge* m_gauge;                      //gauge below preview window
    long m_lCurrentFeed;                   //current feed number

    //html viewing window
#if wxRSSHTMLTYPE == wxIEHTMLWIN 
    wxIEHtmlWin* m_htmlwin; 
#elif wxRSSHTMLTYPE == wxMOZILLA 
    wxMozillaBrowser* m_htmlwin; 
#else 
    class wxRSSHtmlWindow* m_htmlwin; 
#endif 

    //friends
    friend class wxRSSUpdateTimer;
    friend class wxRSSFeedTimer;
}; 

// ---------------------------------------------------------------------------- 
//
// wxRSSHtmlWindow - wxHtmlWindow derivative for launching user's browser
//
// ---------------------------------------------------------------------------- 

#if wxRSSHTMLTYPE == 0 // Only when wxHtmlWindow is wxRSSHTMLTYPE

class wxRSSHtmlWindow : public wxHtmlWindow
{
public:
    //Ctor/Dtor
    wxRSSHtmlWindow(wxWindow *parent, wxWindowID id = -1, 
                    const wxPoint& pos = wxDefaultPosition, 
                    const wxSize& size = wxDefaultSize);      

    //wxHtmlWindow override for linked clicked
    virtual void OnLinkClicked(const wxHtmlLinkInfo& link);
};

#endif //wxRSSHTMLTYPE == 0

// ---------------------------------------------------------------------------- 
//
// wxRSSUpdateTimer - constant timer to update all the feeds
//
// ---------------------------------------------------------------------------- 

class wxRSSUpdateTimer : public wxTimer
{
public:
    //Ctor/Dtor
    wxRSSUpdateTimer(wxRSSFrame* frame);
    
    //Timer procedure
    virtual void Notify();

    wxRSSFrame* m_frame; //main frame
};

// ---------------------------------------------------------------------------- 
//
// wxRSSFeedThread - thread to retrieve data from site
//
// ---------------------------------------------------------------------------- 

class wxRSSFeedThread : public wxThread 
{ 
public: 
    wxRSSFeedThread(wxRSSFeedData* pFeedData, wxListView* list, 
                    wxRSSFrame* frame);
    virtual ExitCode Entry();    

    wxRSSParser m_rssparser;      //Parser this thread is using
    wxRSSFrame* m_frame;          //Main frame (TODO:get rid of this)
    wxRSSFeedData* m_pFeedData;   //Data relating to current feed
    wxInputStream* m_in;          //Stream to site
    wxListView* m_postlist;       //list of posts (TODO:get rid of this)
    wxCriticalSection m_cs;       //Locking critical section for access
}; 

// ---------------------------------------------------------------------------- 
//
// wxRSSFeedTimer - timer to update download progress of feed
//
// ---------------------------------------------------------------------------- 

class wxRSSFeedTimer : public wxTimer 
{ 
public: 
    //Ctor/Dtor
    wxRSSFeedTimer(wxRSSFeedThread* thread, wxRSSFrame* frame,
                   wxRSSFeedData* pFeedData); 
    
    //Timer procedure
    virtual void Notify();

    wxRSSFeedThread* m_thread;    //Thread of most recently loading feed
    wxRSSFrame* m_frame;          //Main frame
    wxRSSFeedData* m_pFeedData;   //Data relating to current feed
}; 

// ============================================================================ 
// 
// Implementation 
// 
// ============================================================================ 

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// wxRSSReaderApp
//
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

IMPLEMENT_APP(wxRSSReaderApp) 

// ----------------------------------------------------------------------------
// wxRSSReaderApp::OnInit
//
// Simple initialization - creates and shows the main frame and
// sets the application and vendor name to tell wxConfig where to look
// ----------------------------------------------------------------------------

bool wxRSSReaderApp::OnInit() 
{ 
    SetVendorName(wxT("wxWidgets")); 
    SetAppName(wxT("wxRSSReader")); 

    wxRSSFrame *frame = new wxRSSFrame(wxT("wxRSSReader for wxForum")); 
    frame->Show(true); 
    
    return true; 
} 

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// wxRSSFrame
//
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ----------------------------------------------------------------------------
// wxRSSFrame Constructor
//
// 1) Create menus
// 2) Create controls
// 3) Load configuration   (wxConfigBase)
// 4) Connect events       (wxEvtHandler::Connect)
// 5) Create status bar
// 6) Start updating timer (wxRSSUpdateTimer)
// ----------------------------------------------------------------------------
wxRSSFrame::wxRSSFrame(const wxString& title) 
       : wxFrame(NULL, wxID_ANY, title, 
                 wxDefaultPosition, wxSize(700,700)), 
         m_timer(NULL)
{ 
    //  Create Menus 
    wxMenu *fileMenu = new wxMenu; 
    wxMenu *actionMenu = new wxMenu; 
    wxMenu *optionsMenu = new wxMenu; 
    wxMenu *helpMenu = new wxMenu; 

    fileMenu->Append(wxID_LOADWXFORUMFEED, 
                     wxT("Load wxForum Feed\tCtrl-L"), 
                     wxT("Get the RSS feed from wxForum")); 
    fileMenu->Append(wxID_LOADFEED, 
                     wxT("Load Custom Feed\tCtrl-F"), 
                     wxT("Get the RSS feed from a URL")); 
    fileMenu->AppendSeparator(); 
    fileMenu->Append(wxID_EXIT, 
                     wxT("E&xit\tAlt-X"), 
                     wxT("Quit this program")); 

    actionMenu->Append(wxID_GOTOPOST, 
                     wxT("&Go to this post\tCtrl-G"), 
                     wxT("Open browser to post from RSS feed")); 

    optionsMenu->Append(wxID_SETUPDATEFEEDINTERVAL,
                     wxT("&Set update interval\tCtrl-I"), 
                     wxT("Set time between feed updates")); 
                     
    helpMenu->Append(wxID_ABOUT, 
                     wxT("&About...\tF1"), 
                     wxT("Show about dialog")); 


    wxMenuBar *menuBar = new wxMenuBar(); 
    menuBar->Append(fileMenu, wxT("&File")); 
    menuBar->Append(actionMenu, wxT("&Action")); 
    menuBar->Append(optionsMenu, wxT("&Options")); 
    menuBar->Append(helpMenu, wxT("&Help")); 
    SetMenuBar(menuBar); 

    // do control stuff 
    wxPanel* panel = new wxPanel(this); 
    
    wxFlexGridSizer* sizer = new wxFlexGridSizer(3, 1, 0, 0); 
    sizer->AddGrowableRow(0); 
    sizer->AddGrowableCol(0);     

    wxBoxSizer* horsizer1 = new wxBoxSizer(wxHORIZONTAL); 
    wxFlexGridSizer* vertsizer = new wxFlexGridSizer(3, 1, 0, 0); 
    vertsizer->AddGrowableRow(0); 
    vertsizer->AddGrowableCol(0); 

    panel->SetSizer(horsizer1); 
    panel->SetAutoLayout(true); 

    m_feedlist = new wxListView(panel, wxID_FEEDLISTCTRL, 
                                wxDefaultPosition, wxDefaultSize, 
                                wxLC_REPORT | wxSUNKEN_BORDER); 

    m_feedlist->InsertColumn(0,wxT(""), wxLIST_FORMAT_CENTER, 20); 
    m_feedlist->InsertColumn(1,wxT("FeedURL"), wxLIST_FORMAT_LEFT, 
                             /*wxLIST_AUTOSIZE_USEHEADER*/305); 

    vertsizer->Add(m_feedlist, 0, 
                   wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5); 
    horsizer1->Add(vertsizer, 0, 
                   wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5); 


    m_postlist = new wxListView(panel, wxID_POSTLISTCTRL, 
                                wxDefaultPosition, wxSize(500, 150), 
                                wxLC_REPORT | wxSUNKEN_BORDER); 

    m_postlist->InsertColumn(0,wxT(""), wxLIST_FORMAT_CENTER, 20); 
    m_postlist->InsertColumn(1,wxT("Title"), wxLIST_FORMAT_LEFT, 
                             /*wxLIST_AUTOSIZE_USEHEADER*/305); 
    sizer->Add(m_postlist, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5);
    
#if wxRSSHTMLTYPE == wxIEHTMLWIN 
    m_htmlwin = new wxIEHtmlWin(panel, wxID_ANY, wxDefaultPosition, 
                                wxSize(500,500)); 
#elif wxRSSHTMLTYPE == wxMOZILLA 
    m_htmlwin = new wxMozillaBrowser(panel, wxID_ANY, wxDefaultPosition, 
                                     wxSize(500,400)); 
#else 
    m_htmlwin = new wxRSSHtmlWindow(panel, wxID_ANY, wxDefaultPosition, 
                                    wxSize(500,400)); 
#endif 
    sizer->Add(m_htmlwin, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5); 
    
    m_gauge = new wxGauge(); 
    m_gauge->Create(panel, wxID_GAUGE, 0, wxDefaultPosition, wxDefaultSize, 
                        wxGA_HORIZONTAL | wxGA_SMOOTH); 
    m_gauge->SetRange(100); 
    sizer->Add(m_gauge, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND , 5); 
    horsizer1->Add(sizer, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5); 

    wxConfigBase* conf = wxConfigBase::Get(); 
    if(!conf->Read(wxT("CurrentFeed"), &m_lCurrentFeed))
        m_lCurrentFeed = -1;
    wxString key, subkey, outstring, sTitle; 
    int i;
    for(i = 0; ; ++i) 
    { 
        key.clear();  key << i; 
        conf->SetPath(wxT("/"));
        if(!conf->Read(key, &outstring)) 
            break; 
        wxRSSFeedData* pFeedData = new wxRSSFeedData;
        pFeedData->pLoadingThread = NULL;
        pFeedData->sLink = outstring;
        AddItem(m_feedlist, wxURI(outstring).GetServer(), pFeedData); 

        for(int j = 0; ; ++j)
        {
            subkey.clear(); subkey << wxT("/") << key << wxT("/") << j;
            conf->SetPath(subkey);
            if(!conf->Read(wxT("Title"), &sTitle))
                break;
            wxRSSPostData* pPostData = new wxRSSPostData;
            pPostData->sTitle = sTitle;
            conf->Read(wxT("Link"), &pPostData->sLink);
            conf->Read(wxT("Data"), &pPostData->sData);
            pFeedData->aPosts.Add(pPostData);
        }
    } 
    
    //Check for corrupt config so we don't crash
    if(i <= m_lCurrentFeed)
    {
        wxMessageBox(wxT("Configuration corrupt, resetting"));
        m_lCurrentFeed = -1;
    }
        
    // Menu events 
    this->Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, 
        wxCommandEventHandler(wxRSSFrame::OnQuit)); 
    this->Connect(wxID_GOTOPOST, wxEVT_COMMAND_MENU_SELECTED, 
        wxCommandEventHandler(wxRSSFrame::OnGoToPost)); 
    this->Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, 
        wxCommandEventHandler(wxRSSFrame::OnAbout)); 
    this->Connect(wxID_LOADFEED, wxEVT_COMMAND_MENU_SELECTED, 
        wxCommandEventHandler(wxRSSFrame::OnLoadCustomFeed)); 
    this->Connect(wxID_LOADWXFORUMFEED, wxEVT_COMMAND_MENU_SELECTED, 
        wxCommandEventHandler(wxRSSFrame::OnLoadwxForumFeed)); 
    this->Connect(wxID_SETUPDATEFEEDINTERVAL, wxEVT_COMMAND_MENU_SELECTED, 
        wxCommandEventHandler(wxRSSFrame::OnSetUpdateFeedInterval)); 

    // Mouse events 
    m_postlist->Connect(wxID_POSTLISTCTRL, wxEVT_ENTER_WINDOW, 
        wxMouseEventHandler(wxRSSFrame::OnMouseEnter)); 
    m_feedlist->Connect(wxID_FEEDLISTCTRL, wxEVT_ENTER_WINDOW, 
        wxMouseEventHandler(wxRSSFrame::OnMouseEnter)); 
    m_htmlwin->Connect(wxID_ANY, wxEVT_ENTER_WINDOW, 
        wxMouseEventHandler(wxRSSFrame::OnMouseEnter)); 

    // ListCtrl events 
    this->Connect( wxID_POSTLISTCTRL, wxEVT_COMMAND_LIST_ITEM_ACTIVATED, 
        wxListEventHandler(wxRSSFrame::OnPostActivated)); 
    this->Connect( wxID_FEEDLISTCTRL, wxEVT_COMMAND_LIST_ITEM_ACTIVATED, 
        wxListEventHandler(wxRSSFrame::OnFeedActivated)); 
    m_postlist->Connect(wxID_POSTLISTCTRL, wxEVT_KEY_DOWN, 
        wxKeyEventHandler(wxRSSFrame::OnKeyDown), 0, this); 
    m_feedlist->Connect(wxID_FEEDLISTCTRL, wxEVT_KEY_DOWN, 
        wxKeyEventHandler(wxRSSFrame::OnKeyDown), 0, this); 

    // Close events
    this->Connect(wxID_ANY, wxEVT_CLOSE_WINDOW, 
                  wxCloseEventHandler(wxRSSFrame::OnClose)); 

    // Create status bar
    CreateStatusBar(1); 

    // Load current feed - stalls on windows on startup - 
    // most likely due to some socket stuff not being initialized
    // for some reason until after OnInit
#if 0
    if(m_lCurrentFeed != -1)
    {
        long lCurrentFeed = m_lCurrentFeed;
        m_lCurrentFeed = -1;
        DoLoadFeed(lCurrentFeed);
    }
#endif
    
    // Start the wxSocket API so we can use GetInputStream in
    // secondary threads
    wxSocketBase::Initialize();

    // Start the updating timer
    m_updatetimer = new wxRSSUpdateTimer(this);
    m_updatetimer->Start(20000);
} 

// ----------------------------------------------------------------------------
// wxRSSFrame::OnMouseEnter
//
// Sets the focus on this frame when the mouse enters it so that it recieves
// keyboard and other input
// ----------------------------------------------------------------------------
void wxRSSFrame::OnMouseEnter(wxMouseEvent& ) 
{   
    SetFocus();     
} 
    
// ----------------------------------------------------------------------------
// wxRSSFrame::OnClose
//
// 1) Stops and destroys the updating timer (wxRSSUpdateTimer)
// 2) Writes configuration so that the next time the app loads
//    it will start with the same list of feeds (wxConfigBase)
// ----------------------------------------------------------------------------
void wxRSSFrame::OnClose(wxCloseEvent& cl) 
{ 
    //stop + destroy update timer and feed timer
    m_updatetimer->Stop();   delete m_updatetimer;
    m_timer->Stop();         delete m_timer;
    
    //write configuration
    wxConfigBase* conf = wxConfigBase::Get(); 
    conf->DeleteAll();
    conf->SetPath(wxT("/"));
    conf->Write(wxT("CurrentFeed"), m_lCurrentFeed);

    wxString key, subkey;   wxRSSPostData* pPostData;
    for(int i = 0; i < m_feedlist->GetItemCount(); ++i) 
    {         
        conf->SetPath(wxT("/"));
        wxRSSFeedData* pFeedData = 
            ((wxRSSFeedData*)m_feedlist->GetItemData(i));    
        if(pFeedData->pLoadingThread)      
            pFeedData->pLoadingThread->Delete();
        key.clear();   key << i; 
        conf->Write(key, pFeedData->sLink); 
        for(size_t j = 0; j < pFeedData->aPosts.GetCount(); ++j)
        {
            pPostData = (wxRSSPostData*) pFeedData->aPosts[j];
            subkey.clear();  subkey << wxT("/") << key << wxT("/") << j;
            conf->SetPath(subkey);
            conf->Write(wxT("Title"), pPostData->sTitle);
            conf->Write(wxT("Link"), pPostData->sLink);
            conf->Write(wxT("Data"), pPostData->sData);
            delete pPostData;
        }
        delete pFeedData;
    } 

    cl.Skip(); 
} 

// ----------------------------------------------------------------------------
// wxRSSFrame::OnLoadCustomFeed
//
// Gets a url from a user to load and loads the RSS feed from it
// ----------------------------------------------------------------------------
void wxRSSFrame::OnLoadCustomFeed(wxCommandEvent& WXUNUSED(evt)) 
{   
    //get some text from the user
    wxString url = wxGetTextFromUser(wxT("Enter URL of Feed"));
    if(!url.empty()) //empty == cancel
    {
        //set the scheme of url to http if it does not have one
        //we could just do if !HasScheme here but this may take 
        //care of edge cases
        wxURI uriResult(url);
        uriResult.Resolve(wxURI(wxT("http:")));
        wxString finalurl = uriResult.BuildURI();
        LoadFeed(finalurl); 
    }
} 

// ----------------------------------------------------------------------------
// wxRSSFrame::OnLoadwxForumFeed
//
// Loads the RSS Feed from the wxForum (url stored in g_szwxforumrssurl)
// ----------------------------------------------------------------------------
void wxRSSFrame::OnLoadwxForumFeed(wxCommandEvent& WXUNUSED(evt)) 
{   
    LoadFeed(g_szwxforumrssurl); 
} 

// ----------------------------------------------------------------------------
// wxRSSFrame::LoadFeed
//
// Loads a feed from a url
// ----------------------------------------------------------------------------
void wxRSSFrame::OnGoToPost(wxCommandEvent& WXUNUSED(evt)) 
{ 
    wxInt32 nItem = m_postlist->GetFirstSelected();
    if(nItem != -1)
    {
        wxLaunchDefaultBrowser(
            ((wxRSSPostData*)m_postlist->GetItemData(nItem))->sLink
                               ); 
    }
} 

// ----------------------------------------------------------------------------
// wxRSSFrame::OnFeedActivated
//
// Called when user double-clicks the feed in the left list. Loads
// the clicked feed
// ----------------------------------------------------------------------------
void wxRSSFrame::OnFeedActivated(wxListEvent& WXUNUSED(event)) 
{ 
    wxInt32 nItem = m_feedlist->GetFirstSelected();
    if(nItem != -1)
        DoLoadFeed(nItem); 
} 

// ----------------------------------------------------------------------------
// wxRSSFrame::OnPostActivated
//
// Called when user double-clicks the post in the right list. Previews
// the clicked post in the lower-right previewing window.
// ----------------------------------------------------------------------------
void wxRSSFrame::OnPostActivated(wxListEvent& WXUNUSED(event)) 
{ 
    wxInt32 nItem = m_postlist->GetFirstSelected();
    m_postlist->SetItem(nItem, 0, wxT(" ")); 
#if wxRSSHTMLTYPE == wxIEHTMLWIN 
    m_htmlwin->LoadString(    
#else 
    m_htmlwin->SetPage(    
#endif 
        ((wxRSSPostData*)m_postlist->GetItemData(nItem))->sData);
} 

// ----------------------------------------------------------------------------
// wxRSSFrame::OnQuit
//
// Called from File->Quit - quits the application by closing the main frame
// ----------------------------------------------------------------------------
void wxRSSFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) 
{   
    Close(true);                                        
} 

// ----------------------------------------------------------------------------
// wxRSSFrame::OnAbout
//
// Help->About 
// Displays some info about this application
// ------------------------------------------------------------------------------
void wxRSSFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) 
{   
    wxMessageBox(
        wxString::Format(wxT("wxRSSReader v%f by Ryan Norton"),
                         WXRSSREADERVERSION
                        )
                );     
}

// ----------------------------------------------------------------------------
// wxRSSParser::OnSetUpdateFeedInterval
//
// Sets the interval our wxRSSUpdateTimer uses to update the feeds
// ----------------------------------------------------------------------------
void wxRSSFrame::OnSetUpdateFeedInterval(wxCommandEvent& WXUNUSED(event))
{  
    m_updatetimer->Stop();   
    int nInterval = 
        wxAtoi(wxGetTextFromUser(
                    wxT("Enter update interval in milliseconds")
                                )
              );
    m_updatetimer->Start(nInterval ? nInterval : 20000);
}

// ----------------------------------------------------------------------------
// wxRSSParser::OnKeyDown
//
// (Called from the context of m_feedlist or 
//  m_postlist - not the frame itself)
// Deletes a post/feed from one of the lists
// ----------------------------------------------------------------------------
void wxRSSFrame::OnKeyDown(wxKeyEvent& event)
{
    if( (event.GetKeyCode() == WXK_BACK ||
        event.GetKeyCode() == WXK_DELETE )  )
    { 
        //delete all selected items
        wxListView* list = (wxListView*)event.GetEventObject();
        long nSelectedItem = list->GetFirstSelected();
        while(nSelectedItem != -1) 
        { 
            if(list == m_postlist)
            {
                delete (wxRSSPostData*) list->GetItemData(nSelectedItem);
                wxRSSFeedData* pFeedData = (wxRSSFeedData*)
                    m_feedlist->GetItemData(m_lCurrentFeed);
                pFeedData->aPosts.RemoveAt((size_t)nSelectedItem);
            }
            else
            {
                wxRSSFeedData* pFeedData = 
                    (wxRSSFeedData*) list->GetItemData(nSelectedItem);
                if(pFeedData->pLoadingThread)      
                    pFeedData->pLoadingThread->Delete();
                for(size_t i = 0; i < pFeedData->aPosts.GetCount(); ++i)
                    delete (wxRSSPostData*) pFeedData->aPosts[i];
                delete pFeedData;
                if(nSelectedItem == m_lCurrentFeed)
                {
                    m_postlist->DeleteAllItems();
                    m_lCurrentFeed = -1;
                }
                else if (nSelectedItem < m_lCurrentFeed)
                    --m_lCurrentFeed;
            }

            list->DeleteItem(nSelectedItem); 
            nSelectedItem = list->GetNextSelected(nSelectedItem);
        } 
    } 
}

// ----------------------------------------------------------------------------
// wxRSSFrame::AddItem
//
// Helper for adding an item for a wxListControl
// ----------------------------------------------------------------------------
/*static*/ void wxRSSFrame::AddItem(wxListView* list, 
                                    const wxString& szString, void* lData, 
                                    const wxString& szTitle) 
{ 
    int nID = list->GetItemCount(); 
    wxListItem kNewItem; 
    kNewItem.SetAlign(wxLIST_FORMAT_LEFT); 
    kNewItem.SetId(nID); 
    kNewItem.SetMask(wxLIST_MASK_DATA); 
    kNewItem.SetData(lData); 
    list->InsertItem(kNewItem); 
    list->SetItem(nID, 0, szTitle); 
    list->SetItem(nID, 1, szString); 

    if (nID % 2) 
    { 
        kNewItem.SetBackgroundColour(wxColour(192,192,192)); 
        list->SetItem(kNewItem); 
    } 
} 

// ----------------------------------------------------------------------------
// wxRSSFrame::LoadFeed
//
// Loads a feed from a url
// ----------------------------------------------------------------------------
void wxRSSFrame::LoadFeed(const wxString& url) 
{ 
    wxRSSFeedData* pFeedData = new wxRSSFeedData;  pFeedData->sLink = url;
    pFeedData->pLoadingThread = NULL;
    AddItem(m_feedlist, wxURI(url).GetServer(), pFeedData); 
    DoLoadFeed(m_feedlist->GetItemCount() - 1); 
} 

// ----------------------------------------------------------------------------
// wxRSSFrame::DoLoadFeed
//
// Performs actual loading of a feed from a feed number
// 1) Detaches frame from currently loading thread, if any
// 2) updates the list of posts in the main frame
// 3) loads the feed if it is not currently loading
// ----------------------------------------------------------------------------
void wxRSSFrame::DoLoadFeed(long lFeed, bool bInBackground /* = false*/) 
{ 
    //Sanity check
    wxASSERT(lFeed != -1);

    //Perform some pre-processing if we are switching to a new feed
    //to show the user
    if(!bInBackground) //only do it if we are switching to this one
    {
        if(m_timer)
        {
            m_timer->Stop(); delete m_timer; m_timer = NULL;
            m_thread = NULL;            
        }
        
        //update list of posts in main frame
        if(lFeed != m_lCurrentFeed)
        {
            m_postlist->DeleteAllItems();
            m_lCurrentFeed = lFeed;
            wxRSSFeedData* pFeedData =
                ((wxRSSFeedData*)m_feedlist->GetItemData(lFeed));

            for(size_t i = 0; i < pFeedData->aPosts.GetCount(); ++i)
                AddItem(m_postlist, 
                    ((wxRSSPostData*)pFeedData->aPosts[i])->sTitle,
                    pFeedData->aPosts[i]);
        }
        
    }

    //clear feed item in left list (sets after it connects)
    m_feedlist->SetItem(lFeed, 0, wxT(" "));
    
    //Load a feed if it is not currently loading
    wxRSSFeedData* pData = (wxRSSFeedData*)m_feedlist->GetItemData(lFeed);
    if(pData->pLoadingThread == NULL)
    {
        //Start and load the loading thread and such
        if(bInBackground)
        {
            pData->pLoadingThread = 
                new wxRSSFeedThread(pData, m_postlist, this);
        }
        else
        {
            pData->pLoadingThread = m_thread = 
                new wxRSSFeedThread(pData, m_postlist, this);
            m_timer = new wxRSSFeedTimer(m_thread, this, pData);       
        }    
    }
    else
        wxMessageBox(wxT("Feed still loading!"));
}
 
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// wxRSSHtmlWindow
//
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#if wxRSSHTMLTYPE == 0 // Only when wxHtmlWindow is being used for previewing

// ----------------------------------------------------------------------------
// wxRSSHtmlWindow Constructor
//
// initializes members
// ----------------------------------------------------------------------------
wxRSSHtmlWindow::wxRSSHtmlWindow(wxWindow *parent, wxWindowID id /*= -1*/, 
                                 const wxPoint& pos /*= wxDefaultPosition*/, 
                                 const wxSize& size /*= wxDefaultSize*/) 
 :  wxHtmlWindow(parent, id, pos, size) 
{
}

// ----------------------------------------------------------------------------
// wxRSSHtmlWindow::OnLinkClicked
//
// override from wxHtmlWindow for when an HTML linked is clicked by the user -
// here we launch the user's default browser when he clicks a link,
// rather then browsing there in the window itself (wxHtmlWindow rendering
// isn't too current)
// ----------------------------------------------------------------------------
void wxRSSHtmlWindow::OnLinkClicked(const wxHtmlLinkInfo& link)
{   
    wxLaunchDefaultBrowser(link.GetHref());         
};

#endif //wxRSSHTMLTYPE == 0

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// wxRSSUpdateTimer
//
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ----------------------------------------------------------------------------
// wxRSSUpdateTimer Constructor
//
// initializes members
// ----------------------------------------------------------------------------
wxRSSUpdateTimer::wxRSSUpdateTimer(wxRSSFrame* frame) 
    : m_frame(frame)
{
}

// ----------------------------------------------------------------------------
// wxRSSUpdateTimer::Notify
//
// Timer procedure -
// Iterates through the feeds and updates them if they are not currently
//    updating (2nd param to DoLoadFeed signifies that we don't want to 
//    update the GUI to reflect each feed switch)
// ----------------------------------------------------------------------------
void wxRSSUpdateTimer::Notify()
{
    for(int i = 0; i < m_frame->m_feedlist->GetItemCount(); ++i)
    {
        wxRSSFeedData* pFeedData = 
            ((wxRSSFeedData*)m_frame->m_feedlist->GetItemData(i));

        if(pFeedData->pLoadingThread == NULL) //if not currently loading...
            m_frame->DoLoadFeed(i, true); //load/update it!
    }
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// wxRSSFeedThread
//
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ----------------------------------------------------------------------------
// wxRSSFeedThread Constructor
//
// 1) Initializes members 
// 2) Creates and Runs the actual thread
// ----------------------------------------------------------------------------
wxRSSFeedThread::wxRSSFeedThread(wxRSSFeedData* pFeedData, wxListView* list, 
            wxRSSFrame* frame) 
    : m_pFeedData(pFeedData), m_postlist(list), m_frame(frame)
{ 
    Create(); 
    Run(); 
} 

// ----------------------------------------------------------------------------
// wxRSSFeedThread::Entry
//
// Thread procedure -
// 1) Creates the RSS parser
// 2) Parses the XML RSS feed tag by tag until the "item" tag is reached,
//    at which point it puts parsed content from the tag into the post list
//    data
// 3) Signals the feed data that this thread and stopped loading
// ----------------------------------------------------------------------------
wxThread::ExitCode wxRSSFeedThread::Entry() 
{ 
    // Load the data
    if( m_rssparser.Create(m_pFeedData->sLink) )
    {
        //Process the RSS content
        do 
        { 
            while(m_rssparser.Ok() && 
                  m_rssparser.NextTag() != wxT("item")) {} 
        
            if(m_rssparser.Ok()) 
            { 
                while(m_rssparser.Ok() && 
                      m_rssparser.NextTag() != wxT("title")) {} 

                wxString szTitle = m_rssparser.TagData(); 
                while(m_rssparser.Ok() && 
                      m_rssparser.NextTag() != wxT("link")) {} 
                wxString szLink = m_rssparser.TagData(); 
                while(m_rssparser.Ok() && 
                      m_rssparser.NextTag() != wxT("description")) {} 
                wxString szPageHTML = m_rssparser.TagData(); 

                bool bFound = false;
                for(size_t i = 0; i < m_pFeedData->aPosts.GetCount(); ++i)
                {
                    wxRSSPostData* pPostData = 
                        (wxRSSPostData*) m_pFeedData->aPosts[i];

                    if(pPostData->sLink == szLink)
                    {
                        bFound  = true; break;
                    }
                }

                if( bFound == false)
                {
                    wxRSSPostData* pPostData = new wxRSSPostData;
                    pPostData->sTitle = szTitle; pPostData->sLink = szLink;
                    pPostData->sData = 
                        wxString(wxT("<html><body>")) + szPageHTML + 
                        wxT("\n<hr>\n<br>\n<a href=\"") + 
                        szLink + 
                        wxT("\">GO HERE TO GO TO THIS POST</a>") + 
                        wxT("</body></html>");
                    
                    //FIXME
                    //FIXME: GUI call here in secondary thread, nono on non-msw
                    //FIXME
                    if(m_postlist)
                        m_frame->AddItem(m_postlist, szTitle, pPostData); 
                    m_pFeedData->aPosts.Add(pPostData);
                }
                m_rssparser.TagEnd(wxT("item")); 
            } 
        }while(m_rssparser.Ok() && !TestDestroy()); 
    }

    //Signal that we are done
    m_cs.Enter();
    m_pFeedData->pLoadingThread = NULL;
    m_cs.Leave();

    //done...
    return 0; 
} 

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// wxRSSFeedTimer
//
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ----------------------------------------------------------------------------
// wxRSSFeedTimer Constructor
//
// 1) Initialize members
// 2) Signal to the main frame that we are beginning to load
// 3) Start this timer and set to update every 1/10 second
// ----------------------------------------------------------------------------
wxRSSFeedTimer::wxRSSFeedTimer(wxRSSFeedThread* thread, wxRSSFrame* frame,
                               wxRSSFeedData* pFeedData)
  : m_thread(thread), m_frame(frame), m_pFeedData(pFeedData)
{ 
    m_frame->SetStatusText(wxT("Loading Data...")); 
    Start(100); 
} 

// ----------------------------------------------------------------------------
// wxRSSFeedTimer::Notify
//
// Timer procedure - updates status bar text with the progress of the 
// currently loading feed.
// ----------------------------------------------------------------------------
void wxRSSFeedTimer::Notify() 
{ 
    if(m_pFeedData->pLoadingThread)
    {
        //Lock the critical section
        wxCriticalSectionLocker cslock(
                        m_pFeedData->pLoadingThread->m_cs
                                      );
        
        //if not still doing initial loading    
        if(m_pFeedData->pLoadingThread->m_rssparser.m_data.Length()) 
        { 
            wxString sStatus = wxT("Converting Data...  ");
            m_frame->m_gauge->SetValue(
                            (int)m_thread->m_rssparser.m_nLastPos
                                      ); 
            m_frame->m_gauge->SetRange(
                            (int)m_thread->m_rssparser.m_data.Length()
                                      ); 
            sStatus 
             << m_pFeedData->pLoadingThread->m_rssparser.m_nLastPos 
             << wxT(" of ")
             << m_pFeedData->pLoadingThread->m_rssparser.m_data.Length();
            m_frame->SetStatusText(sStatus); 
        }         
    }   
    else //done loading... shut down
    {
        m_frame->SetStatusText(wxT("Done loading"));
        m_frame->m_gauge->SetValue(0);
        
        //Mac doesn't update properly either the status bar or the various
        //controls - possibly because they are not in a panel... anyway
        //update them ourselves :)
#ifndef __WXMSW__
        m_frame->Refresh();
#endif
        Stop();
    }
} 

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// wxRSSParser
//
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ----------------------------------------------------------------------------
// wxRSSParser::Create
//
// Connects and loads the data from the location
// ----------------------------------------------------------------------------
bool wxRSSParser::Create(const wxString& loc) 
{ 
    wxURL url(loc);  
    wxInputStream* m_in = url.GetInputStream(); 

    m_nLastPos = 0;           
    size_t nRead;

    if(m_in)
    {
        //could use m_in->GetSize if not wxInvalidOffset or 0 also/instead
#define BUF_SIZE 100000 
        char buf[BUF_SIZE];  
        wxMBConv* conv = NULL; 
        wxMBConv* realconv = NULL; 
        bool bInit = true; 
        do 
        { 
            nRead = m_in->Read(buf, BUF_SIZE).LastRead();                

            //find encoding of stream - TEDIOUS 
            if(bInit) 
            { 
                if(nRead < BUF_SIZE) 
                    buf[nRead] = 0; 
                if( memcmp(buf, "<?xml", 5) == 0 ) 
                { 
                    char* b = buf; 
                    while(nRead  - (buf-b) >= 8 && 
                          memcmp(b++, "encoding", 8) != 0) {} 
                    if(*b) 
                    { 
                        b += 7; 
                        while(*b++ != '=' && *b) {} 
                        if(*b) 
                        { 
                            if(*b == '\"') ++b; 
                            char* encb = b; 
                            while(*b != ' ' && *b) {++b;} 
                            if(*(--b - 1) == '\"') --b; 
                            wxString sEnc = wxString(encb, wxConvLocal, 
                                                     (size_t)(b - encb)); 
                            //wxMessageBox(sEnc); 
                            conv = new wxCSConv(sEnc); 
                        } 
                    } 
                } 
                if(conv == NULL) 
                    realconv = &wxConvUTF8; 
                else 
                    realconv = conv; 
                bInit = false; 
            } 
            m_data += wxString(buf, *realconv, nRead); 
        }  while(nRead == BUF_SIZE); 

        if(conv) 
            delete conv; 
        delete m_in; 
        return true;
    }
    else
        return false;
} 

// ----------------------------------------------------------------------------
// wxRSSParser::NextTag
//
// Scans to the next tag in the stream, returning the name of it
// ----------------------------------------------------------------------------
wxString wxRSSParser::NextTag() 
{ 
    m_nLastPos = m_data.find(wxT('<'), m_nLastPos); 

    if(m_nLastPos != wxString::npos) 
    { 
        m_nLastPos++; 
            
        size_t nEndPos = m_data.find(wxT('>'), m_nLastPos) - m_nLastPos; 
        size_t i = m_nLastPos; 
        while(wxIsalpha(m_data[i]) && i != nEndPos) 
        { 
            ++i; 
        } 
        m_tag = m_data.substr(m_nLastPos, i - m_nLastPos); 
        //wxMessageBox(m_tag); 
        return m_tag; 
    } 
    else 
        return wxT(""); 
} 

// ----------------------------------------------------------------------------
// wxRSSParser::Ok
//
// Returns true if we are not at the end of the stream
// ----------------------------------------------------------------------------
bool wxRSSParser::Ok()
{
    return m_nLastPos != wxString::npos;
} 

// ----------------------------------------------------------------------------
// wxRSSParser::TagData
//
// Parses the data of the current tag and unencodes it
// ----------------------------------------------------------------------------
wxString wxRSSParser::TagData() 
{ 
    if(m_nLastPos == wxString::npos) 
        return wxT(""); 

    m_nLastPos = m_data.find(wxT('>'), m_nLastPos) + 1; 
    wxString sEndStr = wxString(wxT("</")+ 
                                 m_tag + wxT(">")); 
    size_t nTagEnd = m_data.find(sEndStr, 
                                 m_nLastPos); 
    
    wxString sRet; 
    if(nTagEnd != wxString::npos) 
    { 
        sRet = m_data.substr(m_nLastPos, nTagEnd - m_nLastPos); 
//            if(sRet.length() < 100) 
//                    wxMessageBox(sRet); 
        HTMLUnencode(sRet); 
        
        m_nLastPos = nTagEnd + sEndStr.length(); 
    }        
    else 
        m_nLastPos = nTagEnd; 
    return sRet;        
} 

// ----------------------------------------------------------------------------
// wxRSSParser::HTMLUnencode
//
// Converts HTML-encoded characters to "normal" ones
// FIXME
// FIXME: We still have problems with nested HTML tags in an &amp;
// FIXME
// ----------------------------------------------------------------------------
void wxRSSParser::HTMLUnencode(wxString& s) 
{ 
    wxString sRet; 

    for(size_t i = 0; i < s.length(); ++i) 
    { 
        if(s.c_str()[i] == wxT('&')) 
        { 
            if(s.length() - i >= 7) 
            { 
                if(s.c_str()[i] == wxT('&') && 
                    s.c_str()[i+1] == wxT('#') && 
                    wxIsdigit(s.c_str()[i+2]) && 
                    wxIsdigit(s.c_str()[i+3]) && 
                    wxIsdigit(s.c_str()[i+4]) && 
                    s.c_str()[i+5] == wxT(';')) 
                { 
                    sRet += ((wxChar) 
                                    (((s.c_str()[i+2] - wxT('0')) * 100) + 
                                    ((s.c_str()[i+3] - wxT('0')) * 10) + 
                                    ((s.c_str()[i+4] - wxT('0')) ) )
                            ); 
                    i += 6; 
                    continue; 
                } 
            } 
            if(s.length() - i >= 6) 
            { 
                if(wxTmemcmp(&s.c_str()[i], wxT("&apos;"), 6) == 0) 
                { 
                    sRet += wxT('\''); 
                    i += 5; 
                    continue; 
                } 
                if(wxTmemcmp(&s.c_str()[i], wxT("&nbsp;"), 6) == 0) 
                { 
                    sRet += wxT(' '); 
                    i += 5; 
                    continue; 
                } 
                if(wxTmemcmp(&s.c_str()[i], wxT("&quot;"), 6) == 0) 
                { 
                    sRet += wxT('\"'); 
                    i += 5; 
                    continue; 
                } 
            } 
            if(s.length() - i >= 5 && 
                wxTmemcmp(&s.c_str()[i], wxT("&amp;"), 5) == 0) 
            { 
                sRet += wxT('&'); 
                i += 4; //FIXME:  We should re-scan like this: 
//                    wxTmemmove((wxChar*)&s.c_str()[i+1], 
//                                (wxChar*)&s.c_str()[i+5], 
//                                s.length() - (i+5) ); 
//                    s.c_str()[i+5] 
//                    i -= 1; 
                continue; 
            } 
            if(s.length() - i >= 4) 
            { 
                if(wxTmemcmp(&s.c_str()[i], wxT("&lt;"), 4) == 0) 
                { 
                    sRet += wxT('<'); 
                    i += 3; 
                    continue; 
                } 
                if(wxTmemcmp(&s.c_str()[i], wxT("&gt;"), 4) == 0) 
                { 
                    sRet += wxT('>'); 
                    i += 3; 
                    continue; 
                } 
            } 
        } 

        sRet += s[i]; 
    } 

    s = sRet; 
} 

// ----------------------------------------------------------------------------
// wxRSSParser::TagEnd
//
// Scans past the end of a specified tag
// ----------------------------------------------------------------------------
void wxRSSParser::TagEnd(wxString tag) 
{ 
    if(m_nLastPos == wxString::npos) 
        return; 

    wxString sEndStr = wxString(wxT("</"))+ 
                                 tag + wxT(">"); 
                                  
    size_t nTagEnd = m_data.find(sEndStr, 
                                 m_nLastPos+1); 
    if(nTagEnd != wxString::npos)                                  
        m_nLastPos = nTagEnd + sEndStr.length(); 
    else 
        m_nLastPos = nTagEnd; 
} 

// ----------------------------------------------------------------------------
// wxRSSParser::Reset
//
// Resets parsing to the beginning of the stream
// ----------------------------------------------------------------------------
void wxRSSParser::Reset()   
{   
    m_nLastPos = 0; 
} 

//
// End of wxRSSReader
//
