//
// Name     : mmwxTest
// Purpose  : Test program for the mmwx library.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//
//========================================================
//	Gary Harris - April-May, 2010. Updated for wxWidgets 2.8.10.
// 	-See README.txt for changes.
//========================================================

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/mmDropWindow.h"
#include "wx/mmDropMenu.h"
#include "wx/mmNavigator.h"
#include "wx/mmMonthCtrl.h"
#include "wx/mmDropMonth.h"
#include "wx/mmLogonDialog.h"
#include "wx/mmHyperText.h"
#include "wx/mmHyperBitmap.h"
#include "wx/mmTextStretch.h"

#ifdef __MMDEBUG__
	wxTextCtrl *gDebug = NULL; // For global debug output
#endif

//wxString gVersion = APPVERSION;
wxString gVersion = wxT("2.0");

wxBitmap *gBMtest = NULL;

const wxColour wxBRIGHT_ORANGE(255, 180, 120);
const wxColour wxBRIGHT_YELLOW(255, 255, 120);
const wxColour wxBRIGHT_BLUE(100, 200, 255);
const wxColour wxDEFAULT_BG(214, 214, 214);

#define NAV1ID 2300
#define NAV2ID 2301
#define NAV3ID 2302
#define NAV3BUTID 2350
#define LOGONBUTID 2450
#define DROP1ID 2500
#define DROP2ID 2501
#define DROPMENU1ID 2600
#define CAL_ID1 5345
#define CAL_ID2 5346
#define CAL_ID3 5347

//////////////////////////////////////////////////////////////////////////////

class Main: public wxApp
{
    public:
        bool OnInit(void);
}; // Main

//////////////////////////////////////////////////////////////////////////////

class MyFrame: public wxFrame
{
    public:
        MyFrame(void);
        ~MyFrame();
        void CreateDebugWin();

    private:
#ifdef __MMDEBUG__
        wxFrame *gDebugFrame;
#endif
}; // MyFrame

//////////////////////////////////////////////////////////////////////////////

class MyPanel : public wxPanel
{
    public:
        MyPanel(wxWindow *parent, wxWindowID = -1, const wxPoint &pos = wxDefaultPosition, const wxSize  &size = wxDefaultSize, const long style = wxRAISED_BORDER);
        void MultiButtonTest(void);
        void DropWindowTest(void);
        void DropMenuTest(void);
        void MonthTest(void);
        void DropMonthTest(void);
        void NavigatorTest(void);
        void LogonTest(void);
        void HyperTest(void);
        void TextStretchTest(void);

    private:
        DECLARE_EVENT_TABLE()

        void OnCalendar(mmMonthEvent &event);
        void OnNav(mmNavigatorEvent &event);
        void OnNavBut(wxCommandEvent &event);
        void OnDropActivate(wxCommandEvent &event);
        void OnLogonBut(wxCommandEvent &event);

        mmNavigator   *mNav1;
        mmNavigator   *mNav2;
        mmNavigator   *mNav3;
        mmMultiButton *mNav3But;
        wxStaticText  *mDropLabel;
}; // MyPanel

//////////////////////////////////////////////////////////////////////////////

IMPLEMENT_APP(Main)

bool Main::OnInit(void)
{
    MyFrame *frame = new MyFrame();
    frame->Show(TRUE);
    return frame;
}

//////////////////////////////////////////////////////////////////////////////

MyFrame::MyFrame(void)
    : wxFrame(NULL, 1001, _("mmwx test program"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE)
{
#ifdef __MMDEBUG__
    CreateDebugWin();
#endif
    CreateStatusBar(2);
    SetStatusText(_("mmwx v.") + gVersion + _(" test / demo"), 0);
    SetStatusText(_("Copyright (C) 2000-2002 MindMatters, 2010 Gary Harris."), 1);
    SetSize(-1, -1, 670, 465);

    new MyPanel(this);
}

MyFrame::~MyFrame()
{
#ifdef __MMDEBUG__
    if(gDebug) {
        gDebugFrame->Destroy();
    }
#endif
}

#ifdef __MMDEBUG__
void MyFrame::CreateDebugWin()
{
    // ***
    // *** Change the x coord of wxPoint(1100, 20) if the debug window draws too far to the right for you.
    // ***
    gDebugFrame = new wxFrame(NULL, 1002, _("DEBUG - mmwx v.") + gVersion, wxPoint(1100, 20), wxSize(320, 670), wxSTAY_ON_TOP | wxDEFAULT_FRAME_STYLE);
    gDebug = new wxTextCtrl(gDebugFrame, 999, wxEmptyString, wxPoint(0, 0), wxSize(400, 600), wxTE_MULTILINE);
    wxFont *font = new wxFont(9, wxMODERN, wxNORMAL, wxNORMAL);
    gDebug->SetFont(*font);

    gDebugFrame->Show(TRUE);
} // CreateDebugWin
#endif

//////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MyPanel, wxPanel)
    EVT_MONTH_DAY_CHANGED(CAL_ID1, MyPanel::OnCalendar)
    EVT_MONTH_DAY_CHANGED(CAL_ID2, MyPanel::OnCalendar)
    EVT_MONTH_DAY_CHANGED(CAL_ID3, MyPanel::OnCalendar)
    EVT_NAVIGATOR_PREV(NAV2ID, MyPanel::OnNav)
    EVT_NAVIGATOR_NEXT(NAV2ID, MyPanel::OnNav)
    EVT_NAVIGATOR_PREV(NAV3ID, MyPanel::OnNav)
    EVT_NAVIGATOR_NEXT(NAV3ID, MyPanel::OnNav)
    EVT_BUTTON(NAV3BUTID,  MyPanel::OnNavBut)
    EVT_BUTTON(DROP1ID,    MyPanel::OnDropActivate)
    EVT_BUTTON(DROP2ID,    MyPanel::OnDropActivate)
    EVT_BUTTON(LOGONBUTID, MyPanel::OnLogonBut)
END_EVENT_TABLE()

MyPanel::MyPanel(wxWindow *parent,
                 wxWindowID 	id,
                 const wxPoint 	&pos,
                 const wxSize  	&size,
                 const long     	style)
    : wxPanel(parent, id, pos, size, style)
{
    int mWidth, mHeight;
    GetParent()->GetClientSize(&mWidth, &mHeight);
    SetSize(mWidth, mHeight);

    wxImage::AddHandler(new wxPNGHandler);

    MultiButtonTest();
    DropMenuTest();
    DropWindowTest();
    NavigatorTest();
    MonthTest();
    DropMonthTest();
    HyperTest();
    TextStretchTest();
    LogonTest();
} // MyPanel

//////////////////////////////////////////////////////////////////////////////

void MyPanel::MultiButtonTest(void)
// mmMultiButton demo
{
    // mmMultiButton demo

    wxStaticBox *box = new wxStaticBox(this, -1, wxT("mmMultiButton"), wxPoint(10, 10), wxSize(370, 150));

    wxBitmap *bmon = new wxBitmap(wxT("bitmaps/checked.png"), wxBITMAP_TYPE_PNG);
    wxBitmap *bmoff = new wxBitmap(wxT("bitmaps/unchecked.png"), wxBITMAP_TYPE_PNG);
    wxBitmap *bmcal = new wxBitmap(wxT("bitmaps/cal2.png"), wxBITMAP_TYPE_PNG);
    wxBitmap *bmyy = new wxBitmap(wxT("bitmaps/yin_yang.bmp"), wxBITMAP_TYPE_BMP);
    wxBitmap *bm_yy = new wxBitmap(wxT("bitmaps/yang_yin.bmp"), wxBITMAP_TYPE_BMP);

    // Focus demo
// TODO: FROM WX2.9.1 CONTROLS CAN BE CHILDREN OF THE STATICBOX, BUT NOT IN 2.8.10. CHANGE BACK WHEN CONVERTING TO @>(.
//    mmMultiButton *focusbtn = new mmMultiButton(box, -1, _("Focus me"), *bmyy, wxPoint(10, 15), wxDefaultSize, wxRAISED_BORDER | mmMB_FOCUS);
    mmMultiButton *focusbtn = new mmMultiButton(this, -1, _("Focus me"), *bmyy, wxPoint(20, 25), wxDefaultSize, wxRAISED_BORDER | mmMB_FOCUS);
    focusbtn->SetFocusBitmap(*bm_yy);

    // Select demo
//    mmMultiButton *selectbtn = new mmMultiButton(box, -1, _("Select me"), *bmyy, wxPoint(80, 15), wxDefaultSize, mmMB_AUTODRAW | wxRAISED_BORDER | mmMB_SELECT);
    mmMultiButton *selectbtn = new mmMultiButton(this, -1, _("Select me"), *bmyy, wxPoint(90, 25), wxDefaultSize, mmMB_AUTODRAW | wxRAISED_BORDER | mmMB_SELECT);
    selectbtn->SetSelectedBitmap(*bm_yy);
    selectbtn->SetBackgroundColour(*wxRED);
    selectbtn->SetForegroundColour(*wxWHITE);
    selectbtn->Refresh();

    // Toggle demo
//    mmMultiButton *togglebtn1 = new mmMultiButton(box, -1, _("Toggle me 1"), *bmyy, wxPoint(150, 15), wxDefaultSize, wxSIMPLE_BORDER | mmMB_TOGGLE);
    mmMultiButton *togglebtn1 = new mmMultiButton(this, -1, _("Toggle me 1"), *bmyy, wxPoint(160, 25), wxDefaultSize, wxSIMPLE_BORDER | mmMB_TOGGLE);
    togglebtn1->SetToggleBitmap(*bm_yy);

//    mmMultiButton *togglebtn = new mmMultiButton(box, -1, _("Toggle me 2"), *bmoff, wxPoint(220, 15), wxDefaultSize, wxNO_BORDER | mmMB_TOGGLE);
    mmMultiButton *togglebtn = new mmMultiButton(this, -1, _("Toggle me 2"), *bmoff, wxPoint(230, 25), wxDefaultSize, wxNO_BORDER | mmMB_TOGGLE);
    togglebtn->SetToggleBitmap(*bmon);

//    mmMultiButton *togglebtn2 = new mmMultiButton(box, -1, _("Toggle me 3"), *bmcal, wxPoint(290, 15), wxDefaultSize, mmMB_AUTODRAW | wxRAISED_BORDER | mmMB_TOGGLE);
    mmMultiButton *togglebtn2 = new mmMultiButton(this, -1, _("Toggle me 3"), *bmcal, wxPoint(300, 25), wxDefaultSize, mmMB_AUTODRAW | wxRAISED_BORDER | mmMB_TOGGLE);

    // Dropdown demo
//    mmMultiButton *dropbtn1 = new mmMultiButton(box, -1, _("Wholedropdown1"), *bmcal, wxPoint(10, 90), wxDefaultSize, mmMB_AUTODRAW | wxSIMPLE_BORDER | mmMB_WHOLEDROPDOWN);
    mmMultiButton *dropbtn1 = new mmMultiButton(this, -1, _("Wholedropdown1"), *bmcal, wxPoint(20, 100), wxDefaultSize, mmMB_AUTODRAW | wxSIMPLE_BORDER | mmMB_WHOLEDROPDOWN);

//    mmMultiButton *dropbtn2 = new mmMultiButton(box, -1, _("Wholedropdown2"), *bmon, wxPoint(110, 90), wxDefaultSize, mmMB_AUTODRAW | wxSUNKEN_BORDER | mmMB_WHOLEDROPDOWN | mmMB_TOGGLE);
    mmMultiButton *dropbtn2 = new mmMultiButton(this, -1, _("Wholedropdown2"), *bmon, wxPoint(120, 100), wxDefaultSize, mmMB_AUTODRAW | wxSUNKEN_BORDER | mmMB_WHOLEDROPDOWN | mmMB_TOGGLE);
    dropbtn2->SetToggleBitmap(*bmoff);

//    mmMultiButton *dropbtn3 = new mmMultiButton(box, -1, _("Dropdown1"), *bmcal, wxPoint(210, 90), wxDefaultSize, mmMB_AUTODRAW | wxSIMPLE_BORDER | mmMB_DROPDOWN);
    mmMultiButton *dropbtn3 = new mmMultiButton(this, -1, _("Dropdown1"), *bmcal, wxPoint(220, 100), wxDefaultSize, mmMB_AUTODRAW | wxSIMPLE_BORDER | mmMB_DROPDOWN);

//    mmMultiButton *dropbtn4 = new mmMultiButton(box, -1, _("Dropdown2"), *bmcal, wxPoint(290, 90), wxDefaultSize, mmMB_AUTODRAW | wxRAISED_BORDER | mmMB_DROPDOWN);
    mmMultiButton *dropbtn4 = new mmMultiButton(this, -1, _("Dropdown2"), *bmcal, wxPoint(300, 100), wxDefaultSize, mmMB_AUTODRAW | wxRAISED_BORDER | mmMB_DROPDOWN);

} // MultiButtonTest

//////////////////////////////////////////////////////////////////////////////

void MyPanel::DropMenuTest()
// mmDropMenu demo
{
    wxStaticBox *box = new wxStaticBox(this, -1, wxT("mmDropMenu"), wxPoint(380, 10), wxSize(90, 150));

    wxBitmap *bm   = new wxBitmap(wxT("bitmaps/yin_yang.bmp"), wxBITMAP_TYPE_BMP);

//    mmDropMenu *dropmenu = new mmDropMenu(box, DROPMENU1ID, _(" Menu #1 "), *bm, wxPoint(10, 15), wxDefaultSize, wxRAISED_BORDER | mmMB_DROPDOWN, NULL);
    mmDropMenu *dropmenu = new mmDropMenu(this, DROPMENU1ID, _(" Menu #1 "), *bm, wxPoint(390, 25), wxDefaultSize, wxRAISED_BORDER | mmMB_DROPDOWN, NULL);
    wxMenu *mMenuFile = new wxMenu;
    mMenuFile->Append(wxID_PRINT_SETUP, _("Printer &setup..."));
    mMenuFile->Append(wxID_PREVIEW,     _("Pre&view...\tCtrl-V"));
    mMenuFile->Append(wxID_PRINT,       _("&Print...\tCtrl-P"));
    mMenuFile->AppendSeparator();
    mMenuFile->Append(wxID_EXIT, _("&Exit\tAlt-X"));
    dropmenu->SetChild(mMenuFile);
} // DropMenuTest

//////////////////////////////////////////////////////////////////////////////

void MyPanel::DropWindowTest()
// mmDropWindow demo
{
    wxStaticBox *box = new wxStaticBox(this, -1, wxT("mmDropWindow"), wxPoint(10, 165), wxSize(250, 110));

    wxBitmap *bm   = new wxBitmap(wxT("bitmaps/cal2.png"), wxBITMAP_TYPE_PNG);

//    mDropLabel = new wxStaticText(box, -1, wxT("-----"), wxPoint(15, 85), wxSize(100, -1));
    mDropLabel = new wxStaticText(this, -1, wxT("-----"), wxPoint(25, 250), wxSize(100, -1));

    // DROPDOWN test. Create window first, set child
//    mmDropWindow *dw1 = new mmDropWindow(box, DROP1ID, _(" Drop it 1 "), *bm, wxPoint(15, 15), wxDefaultSize, wxRAISED_BORDER | mmMB_DROPDOWN, NULL, wxSize(200, 200));
    mmDropWindow *dw1 = new mmDropWindow(this, DROP1ID, _(" Drop it 1 "), *bm, wxPoint(25, 180), wxDefaultSize, wxRAISED_BORDER | mmMB_DROPDOWN, NULL, wxSize(200, 200));
//    wxWindow *win1 = new wxWindow(box, -1, wxDefaultPosition, wxSize(100, 150), wxSIMPLE_BORDER);
    wxWindow *win1 = new wxWindow(this, -1, wxDefaultPosition, wxSize(150, 320), wxSIMPLE_BORDER);
    win1->SetBackgroundColour(*wxBLUE);
    wxTextCtrl *txt1 = new wxTextCtrl(win1, -1, _("Some text1"), wxPoint(30, 30), wxSize(100, -1));
    dw1->SetChild(win1);

    // DROPDOWN test. Create child first, give it to constructor
//    wxWindow *win2 = new wxWindow(box, -1, wxDefaultPosition, wxSize(250, 100), wxRAISED_BORDER);
    wxWindow *win2 = new wxWindow(this, -1, wxDefaultPosition, wxSize(260, 265), wxRAISED_BORDER);
    win2->SetBackgroundColour(*wxRED);
    wxTextCtrl *txt2 = new wxTextCtrl(win2, -1, _("Some text2"), wxPoint(30, 30), wxSize(100, -1));
//    mmDropWindow *dw2 = new mmDropWindow(box, DROP2ID, _(" Drop it 2 "), *bm, wxPoint(100, 15), wxDefaultSize, wxNO_BORDER | mmMB_DROPDOWN, win2, wxSize(150, 250));
    mmDropWindow *dw2 = new mmDropWindow(this, DROP2ID, _(" Drop it 2 "), *bm, wxPoint(110, 180), wxDefaultSize, wxNO_BORDER | mmMB_DROPDOWN, win2, wxSize(150, 250));

    // WHOLEDROPDOWN test
//    wxWindow *win3 = new wxWindow(box, -1, wxDefaultPosition, wxSize(100, 200), wxRAISED_BORDER);
    wxWindow *win3 = new wxWindow(this, -1, wxDefaultPosition, wxSize(110, 365), wxRAISED_BORDER);
    win3->SetBackgroundColour(*wxRED);
    wxTextCtrl *txt3 = new wxTextCtrl(win3, -1, _("Some text3"), wxPoint(30, 30), wxSize(100, -1));
//    mmDropWindow *dw3 = new mmDropWindow(box, -1, _(" No 3 "), *bm, wxPoint(175, 15), wxDefaultSize, wxNO_BORDER | mmMB_WHOLEDROPDOWN, win3, wxSize(200, 100));
    mmDropWindow *dw3 = new mmDropWindow(this, -1, _(" No 3 "), *bm, wxPoint(185, 180), wxDefaultSize, wxNO_BORDER | mmMB_WHOLEDROPDOWN, win3, wxSize(200, 100));
} // DropWindowTest

void MyPanel::OnDropActivate(wxCommandEvent &event)
{
    if(event.GetId() == DROP1ID)
        mDropLabel->SetLabel(_("Drop 1 pressed"));
    else
        mDropLabel->SetLabel(_("Drop 2 pressed"));
} // OnDropActivate

//////////////////////////////////////////////////////////////////////////////

void MyPanel::NavigatorTest(void)
// mmNavigator demo
{
    wxStaticBox *box = new wxStaticBox(this, 1100, wxT("mmNavigator"), wxPoint(10, 280), wxSize(250, 110));
    wxStaticText *st1 = new wxStaticText(this, 1101, _("She's the tear that hangs inside my soul forever"), wxDefaultPosition, wxSize(300, -1), wxST_NO_AUTORESIZE | wxALIGN_CENTRE);
//    mNav1 = new mmNavigator(box, NAV1ID, st1, TRUE, TRUE, TRUE, TRUE, wxPoint(15, 15), wxSize(220, 28), wxSIMPLE_BORDER);
    mNav1 = new mmNavigator(this, NAV1ID, st1, TRUE, TRUE, TRUE, TRUE, wxPoint(25, 300), wxSize(220, 28), wxSIMPLE_BORDER);

    wxBitmap *bm   = new wxBitmap(wxT("bitmaps/yang_yin.bmp"), wxBITMAP_TYPE_BMP);
    wxStaticBitmap *st2 = new wxStaticBitmap(this, -1, *bm);
//    mNav2 = new mmNavigator(box, NAV2ID, st2, TRUE, TRUE, FALSE, FALSE, wxPoint(15, 50), wxSize(100, 50), wxRAISED_BORDER);
    mNav2 = new mmNavigator(this, NAV2ID, st2, TRUE, TRUE, FALSE, FALSE, wxPoint(25, 335), wxSize(100, 50), wxRAISED_BORDER);

    mNav3But = new mmMultiButton(this, NAV3BUTID, _("OK"), wxNullBitmap);
    //wxPoint(210,15), wxSize(100,100),
    //wxRAISED_BORDER);
//    mNav3 = new mmNavigator(box, NAV3ID, mNav3But, TRUE, TRUE, FALSE, FALSE, wxPoint(125, 50), wxSize(90, 30), wxRAISED_BORDER);
    mNav3 = new mmNavigator(this, NAV3ID, mNav3But, TRUE, TRUE, FALSE, FALSE, wxPoint(135, 335), wxSize(90, 30), wxRAISED_BORDER);
} // NavigatorTest

void MyPanel::OnNav(mmNavigatorEvent &event)
{
    int type = event.GetEventType();
#ifdef __MMDEBUG__
    *gDebug << wxT("OnNav,") << type << wxT("\n");
#endif
    if(event.GetId() == NAV2ID)
    {
        wxStaticBitmap *st2;
        if(type == mmEVT_NAVIGATOR_PREV)
        {
            wxBitmap *bm   = new wxBitmap(wxT("bitmaps/yang_yin.bmp"), wxBITMAP_TYPE_BMP);
            st2 = new wxStaticBitmap(this, -1, *bm);
        }
        else
        {
            wxBitmap *bm   = new wxBitmap(wxT("bitmaps/yin_yang.bmp"), wxBITMAP_TYPE_BMP);
            st2 = new wxStaticBitmap(this, -1, *bm);
        }
        mNav2->SetWindow(st2);
    }
    else if(event.GetId() == NAV3ID)
    {
        if(type == mmEVT_NAVIGATOR_PREV)
            mNav3But->SetLabel(_("-1"));
        else
            mNav3But->SetLabel(_("+1"));
    }
} // OnNav

void MyPanel::OnNavBut(wxCommandEvent &event)
{
#ifdef __MMDEBUG__
    *gDebug << wxT("OnNavBut\n");
#endif
    mNav3But->SetLabel(_("OK"));
} // OnNavBut

//////////////////////////////////////////////////////////////////////////////

void MyPanel::MonthTest()
// mmMonthCtrl demo
{
    wxStaticBox *box = new wxStaticBox(this, -1, wxT("mmMonthCtrl"), wxPoint(270, 165), wxSize(200, 160));

//    mmMonthCtrl *month = new mmMonthCtrl(box, CAL_ID1, wxDateTime::Today(), wxPoint(30, 20), wxSize(150, 130), wxSIMPLE_BORDER | mmPREV_BTN | mmNEXT_BTN | mmSHOW_ALL);
    mmMonthCtrl *month = new mmMonthCtrl(this, CAL_ID1, wxDateTime::Today(), wxPoint(300, 185), wxSize(150, 130), wxSIMPLE_BORDER | mmPREV_BTN | mmNEXT_BTN | mmSHOW_ALL);
} // MonthTest

//////////////////////////////////////////////////////////////////////////////

void MyPanel::DropMonthTest()
// mmDropMonth demo
{
    wxStaticBox *box = new wxStaticBox(this, -1, wxT("mmDropMonth"), wxPoint(270, 330), wxSize(200, 60));

//    mmDropMonth *month1 = new mmDropMonth(box, CAL_ID2, wxDateTime::Today(), wxPoint(15, 15), wxSize(150, 150), mmDROPMONTH_UPDATE_LABEL);
    mmDropMonth *month1 = new mmDropMonth(this, CAL_ID2, wxDateTime::Today(), wxPoint(285, 345), wxSize(150, 150), mmDROPMONTH_UPDATE_LABEL);
//    mmDropMonth *month2 = new mmDropMonth(box, CAL_ID3, wxDateTime::Today(), wxPoint(105, 15), wxSize(250, 250), mmDROPMONTH_UPDATE_LABEL);
    mmDropMonth *month2 = new mmDropMonth(this, CAL_ID3, wxDateTime::Today(), wxPoint(375, 345), wxSize(250, 250), mmDROPMONTH_UPDATE_LABEL);
} // DropMonthTest

void MyPanel::OnCalendar(mmMonthEvent &event)
{
#ifdef __MMDEBUG__
    *gDebug << wxT("OnCalendar,");
    *gDebug << event.GetDate().Format() << wxT("\n");
#endif
    event.Skip();
} // OnCalendar

//////////////////////////////////////////////////////////////////////////////

void MyPanel::HyperTest()
// mmHyperText/mmHyperBitmap demo
{
    wxStaticBox *box = new wxStaticBox(this, -1, wxT("mmHyperText/mmHyperBitmap"), wxPoint(480, 10), wxSize(160, 150));

//    mmHyperText *hyp1 = new mmHyperText(box, -1, wxT("MindMatters"), wxT("http://www.mindmatters.no"), wxPoint(15, 15), wxSize(20, 20));
    mmHyperText *hyp1 = new mmHyperText(this, -1, wxT("MindMatters"), wxT("http://www.codeblocks.org/"), wxPoint(495, 25), wxSize(20, 20));
    wxBitmap *bm = new wxBitmap(wxT("bitmaps/logo3.bmp"), wxBITMAP_TYPE_BMP);
//    mmHyperBitmap *hyp2 = new mmHyperBitmap(box, -1, *bm, wxT("http://www.mindmatters.no"), wxPoint(15, 40), wxSize(20, 20));
    mmHyperBitmap *hyp2 = new mmHyperBitmap(this, -1, *bm, wxT("http://www.codeblocks.org/"), wxPoint(495, 50), wxSize(20, 20));
} // HyperTest

//////////////////////////////////////////////////////////////////////////////

void MyPanel::TextStretchTest()
// mmTextStretch demo
{
    wxStaticBox *box = new wxStaticBox(this, -1, wxT("mmTextStretch"), wxPoint(480, 165), wxSize(160, 140));

//    mmTextStretch *ts1 = new mmTextStretch(box, -1, _("Drag the sides to resize.\nHold down Ctrl and drag the sides to move."), wxPoint(25, 25), wxSize(120, 105), mmTS_SIZE_ALL | wxTE_MULTILINE);
    mmTextStretch *ts1 = new mmTextStretch(this, -1, _("Drag the sides to resize.\nHold down Ctrl and drag the sides to move."), wxPoint(505, 190), wxSize(120, 105), mmTS_SIZE_ALL | wxTE_MULTILINE);
    ts1->SetSnapToGrid(TRUE);
} // TestStretchDemo

//////////////////////////////////////////////////////////////////////////////

void MyPanel::LogonTest()
// mmLogonDialog demo
{
    wxStaticBox *box = new wxStaticBox(this, -1, wxT("mmLogonDialog"), wxPoint(480, 310), wxSize(120, 80));

//    mmMultiButton *lb = new mmMultiButton(box, LOGONBUTID, _("Logon"), wxNullBitmap, wxPoint(25, 30), wxSize(70, 20), mmMB_NO_AUTOSIZE);
    mmMultiButton *lb = new mmMultiButton(this, LOGONBUTID, _("Logon"), wxNullBitmap, wxPoint(505, 340), wxSize(70, 20), mmMB_NO_AUTOSIZE);
} // LogonTest

void MyPanel::OnLogonBut(wxCommandEvent &ev)
{
    // Log on
    wxString message = wxEmptyString, user = wxEmptyString, password = wxEmptyString;
    bool cancelled = TRUE;
    mmLogonDialog *logon = new mmLogonDialog(this, -1, message);
    do
    {
        wxString user = wxT("mindmatters");
        wxString pass = wxT("mmwx");
        logon->Clear();
        logon->SetMessage(message);
        logon->SetValid(user, pass);
        cancelled = !logon->ShowModal();
        message = _("Wrong username or password. Try again!");
    } while(!logon->IsValid() && !cancelled);
    if(cancelled)
        wxMessageDialog(this, _("LOGON CANCELLED!\n")).ShowModal();
    else if(logon->IsValid())
        wxMessageDialog(this, _("LOGON OK!\n")).ShowModal();
    else
        wxMessageDialog(this, _("SHOULD NOT HAPPEN!\n")).ShowModal();
    logon->Destroy();
} // OnLogonBut

