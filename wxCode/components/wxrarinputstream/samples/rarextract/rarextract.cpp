#include "wx/wx.h"
#include "wx/listctrl.h"
#include "wx/slider.h"
#include "wx/notebook.h"
#include "../src/unrar/dll.hpp"
#include "wx/rarstream.h"
//#include "re.h"
/*
void* RAROpenArchive(struct RAROpenArchiveData *ArchiveData);
void* RAROpenArchiveEx(struct RAROpenArchiveDataEx *ArchiveData);
int    RARCloseArchive(void* hArcData);
int    RARReadHeader(void* hArcData,struct RARHeaderData *HeaderData);
int    RARReadHeaderEx(void* hArcData,struct RARHeaderDataEx *HeaderData);
int    RARProcessFile(void* hArcData,int Operation,char *DestPath,char *DestName);
void   RARSetCallback(void* hArcData,UNRARCALLBACK Callback,long UserData);
void   RARSetChangeVolProc(void* hArcData,CHANGEVOLPROC ChangeVolProc);
void   RARSetProcessDataProc(void* hArcData,PROCESSDATAPROC ProcessDataProc);
void   RARSetPassword(void* hArcData,char *Password);
int    RARGetDllVersion();
*/
enum
{
wxID_PLAY,wxID_STOP,wxID_NEXT,wxID_PREV, kSlider_ID, kPlayBox_ID
};
class MyFrame : public wxFrame
{
public:
    MyFrame() : wxFrame((wxWindow *) NULL, -1, wxT("Title"), wxPoint(0,0), wxSize(200,200))
    {
//    wxMessageBox(wxString::Format("%i", RARGetDllVersion()));
#if wxUSE_MENUS
    wxMenu *FileMenu = new wxMenu;
    wxMenu *HelpMenu = new wxMenu;
    wxMenu *ControlsMenu = new wxMenu;

    FileMenu->Append(wxID_OPEN, _T("&Open"), _T("Quit this program"));
    FileMenu->Append(wxID_CLOSE, _T("&Close"), _T("Quit this program"));
    FileMenu->AppendSeparator();
    FileMenu->Append(wxID_EXIT, _T("&Quit\tAlt+F4"), _T("Quit this program"));

    HelpMenu->Append(wxID_ABOUT, _T("&About...\tF1"), _T("Show about dialog"));

    ControlsMenu->Append(wxID_PLAY, _T("&Play"), _T("Play Selected File"));
    ControlsMenu->Append(wxID_STOP, _T("&Stop"), _T("Play Selected File"));
    ControlsMenu->AppendSeparator();
    ControlsMenu->Append(wxID_NEXT, _T("&Next"), _T("Move to next file"));
    ControlsMenu->Append(wxID_PREV, _T("P&rev"), _T("Move to previous file"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *MenuBar = new wxMenuBar();
    MenuBar->Append(FileMenu, _T("&File"));
    MenuBar->Append(ControlsMenu, _T("&Controls"));
    MenuBar->Append(HelpMenu, _T("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(MenuBar);
#endif // wxUSE_MENUS
    kSlider.Create(this, kSlider_ID, 0, 0, 0);

    kPlayBox.Create(this, kPlayBox_ID, wxPoint(0,0), wxSize(0,0),
                    wxLC_REPORT //wxLC_LIST
                    | wxSUNKEN_BORDER);
    nb.Create(this, -1, wxPoint(0,0), wxSize(200,200));
  //  wxMessageBox(wxString::Format("%i", nb.GetPageCount()));
    //nb.GetPage(0)->SetTitle("Page 1");
    }
    
    wxSlider kSlider;
    wxListCtrl kPlayBox;
    wxNotebook nb;
    
    void OnOpen(wxCommandEvent& WXUNUSED(event))
    {
    wxFileDialog kFD(this);
	if (kFD.ShowModal() == wxID_OK)
	{
    wxDirDialog kFDR(this);
    kFDR.ShowModal();

    wxRarInputStream in(kFD.GetPath());
    
    while(in.OpenNextFile())
    {
        in.ExtractFile(kFDR.GetPath(), NULL);
    }

/*
	    char buff[2000];
	    RAROpenArchiveData rx;
	    memset(&rx,0,sizeof(rx));
	    rx.ArcName=((char*) kFD.GetPath().c_str());
	    rx.OpenMode=RAR_OM_EXTRACT;
	    rx.CmtBuf=buff;
	    rx.CmtBufSize=2000;
	    int nError;
	    void* hRAR = RAROpenArchive(&rx);
//Users/ryannorton/Desktop
	    if (!hRAR)
	    wxMessageBox("AH!");
	    else
	    RARSetPassword(hRAR,"");
	    RARHeaderData hd;
	    while(true){
	    if ((nError = RARReadHeader(hRAR, &hd)) != 0)
	    {
            wxMessageBox(wxString::Format("AHH  %ld", nError));
		break;
	    }
	    else
	    {
		wxMessageBox(hd.FileName);
		if(
		(nError = RARProcessFile(hRAR, RAR_EXTRACT, (char*)kFDR.GetPath().c_str(), NULL))
		!= 0)
		wxMessageBox(wxString::Format("AHHPROCESS  %ld", nError));
	    }
	    }
	    
	    RARCloseArchive(hRAR);
*/
    }
    }
    DECLARE_EVENT_TABLE()
};
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(wxID_OPEN, MyFrame::OnOpen)
END_EVENT_TABLE()
class MyApp : public wxApp
{
public:
bool OnInit()
{
MyFrame *f = new MyFrame();
f->Show();
return true;
}
};
IMPLEMENT_APP(MyApp)