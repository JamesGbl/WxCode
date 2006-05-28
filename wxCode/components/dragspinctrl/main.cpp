
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "wxsciunitparser.h"
#include "fdragspinctrl.h"
//#include "stextctrl.h"
#include <fstream>
using namespace std;




class MyApp : public wxApp
{

public:
    virtual bool OnInit();
};

class MyPanel : public wxPanel
{

public:
    
    MyPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style , const wxString& name = "");
       
    
//private:
        
        wxStaticText*st2;
        wxStaticText*st3;
        wxStaticText*st4;
        wxStaticText*st5;
        wxStaticText*st6;
       
        wxFDragSpinCtrl* sc2;
        wxFDragSpinCtrl* sc3;
        wxFDragSpinCtrl* sc4;
        wxFDragSpinCtrl* sc5;
        wxFDragSpinCtrl* sc6;
        
        wxFlexGridSizer* sizer;

        wxSciUnitParser* scup1;
        wxSciUnitParser* scup2;
        wxSciUnitParser* scup3;
        wxSciUnitParser* scup4;   
       
        /*wxSTextCtrl *tc1,*tc2;
        wxRTextCtrl *tc3,*tc4;*/
};
class MyPanel2 : public wxPanel
{

public:
    
    MyPanel2(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style , const wxString& name = "");
       
    
//private:
        
        wxStaticText*st2;
        wxStaticText*st3;
        wxStaticText*st4;
        wxStaticText*st5;
        wxStaticText*st6;
       
        wxFDragSpinCtrl* sc2;
        wxFDragSpinCtrl* sc3;
        wxFDragSpinCtrl* sc4;
        wxFDragSpinCtrl* sc5;
        wxFDragSpinCtrl* sc6;
        
        wxFlexGridSizer* sizer;

        wxSciUnitParser* scup1;
        wxSciUnitParser* scup2;
        wxSciUnitParser* scup3;
        wxSciUnitParser* scup4;   
       
        /*wxSTextCtrl *tc1,*tc2;
        wxRTextCtrl *tc3,*tc4;*/
};

class MyFrame : public wxFrame
{
public:
    
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size,
            long style = wxCAPTION|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxCLOSE_BOX);
    
     
    
//private:
       wxNotebook* n;
       
        MyPanel* container;
       
};




IMPLEMENT_APP(MyApp)



bool MyApp::OnInit()
{
    
    MyFrame *frame = new MyFrame(_T("Parser Example"),
                                 wxPoint(50, 50), //wxSize(275, 200));
                                 wxSize(300, 220));
    
    
    frame->Show(TRUE);
    
    /*frame->container->sc6->SetValue(1);
    wxSleep(1);  frame->container->sc6->SetValue(2);
    wxSleep(1);  frame->container->sc6->SetValue(3);
    wxSleep(1);  frame->container->sc6->SetValue(4);
    wxSleep(1);  frame->container->sc6->SetValue(5);
    wxSleep(1);  frame->container->sc6->SetValue(6);
    wxSleep(1);  frame->container->sc6->SetValue(7);
    wxSleep(1);  frame->container->sc6->SetValue(8);
    wxSleep(1);  frame->container->sc6->SetValue(9);
    wxSleep(1);  frame->container->sc6->SetValue(10);
    */
    return TRUE;
}
MyPanel::MyPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style , const wxString&)
                 :wxPanel(parent,id,pos,size, wxBORDER_NONE)
{
    
    wxString expr[]={_T("k"),_T("h"),_T("da"),_T(""),_T("dc"),_T("c"),_T("m")};
    wxConversion c3 = wxConversion(_T("a"),cOpMult,1);
    scup1 = new wxSciUnitParser();
    scup2 = new wxSciUnitParser();
    scup3 = new wxSciUnitParser();
    scup4 = new wxSciUnitParser();
    
    /*tc3 = new wxRTextCtrl(this,-1,_T("txctrlR1"));
    tc4 = new wxRTextCtrl(this,-1,_T("txctrlR2"));
    tc1 = new wxSTextCtrl(this,-1,_T("txctrl1"),tc3);
    tc2 = new wxSTextCtrl(this,-1,_T("txctrl2"),tc4);
    */
  	
    sizer = new wxFlexGridSizer(2);
   
    st2 = new wxStaticText(this, -1,_T("Text1"),wxPoint(150,50),wxSize(100,20), 0,_T("staticText2"));
    st3 = new wxStaticText(this, -1,_T("Text2"),wxPoint(150,75),wxSize(100,20), 0,_T("staticText3"));
    st4 = new wxStaticText(this, -1,_T("Slashes"),wxPoint(150,75),wxSize(100,20), 0,_T("staticText4"));
    st5 = new wxStaticText(this, -1,_T("No Conv"),wxPoint(150,75),wxSize(100,20), 0,_T("staticText5"));
    st6 = new wxStaticText(this, -1,_T("``^2'' ``^3''"),wxPoint(150,75),wxSize(100,20), 0,_T("staticText6"));
    
    scup1->ReadConversionsFromFile("c3.txt");          
    scup1->InsertExpansion(c3,expr,7,10,1000);
    scup2->ReadConversionsFromFile("c3.txt"); 
    scup3->SetProperties(wxSUP_USE_RICH_OUTPUT);
    scup4->ReadConversionsFromFile("c.txt");
    scup1->print_conversions(); 
    scup4->print_conversions();
       
    // The DRAGSPIN CTRL
    
    sc2 = new wxFDragSpinCtrl(this,2580,wxPoint(40,-1),wxSize(100,25),wxDB_HORIZONTAL|wxDB_FREEZE_CURSOR|wxDB_SHOW_RANGE_POPUP,
                0,-10,10,3,2,
                _T("N"),scup1,_T("PRUEBA"));
    
    sc3 = new wxFDragSpinCtrl(this,2581,wxPoint(40,75),wxSize(120,18),wxDB_VERTICAL|wxDB_HIDE_CURSOR|wxDB_SHOW_RANGE_POPUP|wxDB_CHANGE_CURSOR,
                0,-100,100,2,2,
                _T("C"),scup2);
    
    sc4 = new wxFDragSpinCtrl(this,2582,wxPoint(40,75),wxSize(120,18),wxDB_HORIZONTAL|wxDB_HIDE_CURSOR|wxDB_SHOW_RANGE_POPUP,
                0,-100,100,2,2,
                _T("l/s"),scup4);

    sc5 = new wxFDragSpinCtrl(this,2583,wxPoint(40,20),wxSize(100,25),wxDB_HORIZONTAL|wxDB_FREEZE_CURSOR|wxDB_SHOW_RANGE_POPUP,
                0,-10,10,3,2,
                _T("unit"),scup3);
    
    sc6 = new wxFDragSpinCtrl(this,2584,wxPoint(40,20),wxSize(100,25),wxDB_HORIZONTAL|wxDB_FREEZE_CURSOR|wxDB_SHOW_RANGE_POPUP,
                0,-10,10,3,2,
                _T("m^3"),scup3);                          
             
    sc3->SetDCLogarithmic(true);
    sc4->SetDCLogarithmic(false);
    sc2->SetValue(5);



    sizer->Add(st2);
    sizer->Add(sc2);
    sizer->Add(st3);
    sizer->Add(sc3);
    sizer->Add(st4);
    sizer->Add(sc4);
    sizer->Add(st5);
    sizer->Add(sc5);
    sizer->Add(st6);
    sizer->Add(sc6);
      
    /*sizer->Add(tc1);
    sizer->Add(tc2);
    */
    /*sc6->Disable();
    sc5->Disable();
    sc3->Disable();
    sc2->Disable();
    tc1->Disable();*/ 
    
    

    
    SetSizer(sizer);
    Show();

  
    //Disable();
   
}
MyPanel2::MyPanel2(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style , const wxString&)
                 :wxPanel(parent,id,pos,size, wxBORDER_NONE)
{
    
    wxString expr[]={_T("k"),_T("h"),_T("da"),_T(""),_T("dc"),_T("c"),_T("m")};
    wxConversion c3 = wxConversion(_T("a"),cOpMult,1);
    scup1 = new wxSciUnitParser();
    scup2 = new wxSciUnitParser();
    scup3 = new wxSciUnitParser();
    scup4 = new wxSciUnitParser();
    
    /*tc3 = new wxRTextCtrl(this,-1,_T("txctrlR1"));
    tc4 = new wxRTextCtrl(this,-1,_T("txctrlR2"));
    tc1 = new wxSTextCtrl(this,-1,_T("txctrl1"),tc3);
    tc2 = new wxSTextCtrl(this,-1,_T("txctrl2"),tc4);
    */
  	
    sizer = new wxFlexGridSizer(2);
   
    st2 = new wxStaticText(this, -1,_T("Text1"),wxPoint(150,50),wxSize(100,20), 0,_T("staticText2"));
    st3 = new wxStaticText(this, -1,_T("Text2"),wxPoint(150,75),wxSize(100,20), 0,_T("staticText3"));
    st4 = new wxStaticText(this, -1,_T("Slashes"),wxPoint(150,75),wxSize(100,20), 0,_T("staticText4"));
    st5 = new wxStaticText(this, -1,_T("No Conv"),wxPoint(150,75),wxSize(100,20), 0,_T("staticText5"));
    st6 = new wxStaticText(this, -1,_T("``^2'' ``^3''"),wxPoint(150,75),wxSize(100,20), 0,_T("staticText6"));
    
    scup1->ReadConversionsFromFile("c3.txt");          
    scup1->InsertExpansion(c3,expr,7,10,1000);
    scup2->ReadConversionsFromFile("c3.txt"); 
    scup3->SetProperties(wxSUP_USE_RICH_OUTPUT);
    scup4->ReadConversionsFromFile("c.txt");
    scup1->print_conversions(); 
    scup4->print_conversions();
       
    // The DRAGSPIN CTRL
    
    sc2 = new wxFDragSpinCtrl(this,2580,wxPoint(40,-1),wxSize(100,25),wxDB_HORIZONTAL|wxDB_FREEZE_CURSOR|wxDB_SHOW_RANGE_POPUP,
                0,-10,10,3,2,
                _T("N"),scup1,_T("PRUEBA"));
    
    sc3 = new wxFDragSpinCtrl(this,2581,wxPoint(40,75),wxSize(120,18),wxDB_VERTICAL|wxDB_HIDE_CURSOR|wxDB_SHOW_RANGE_POPUP|wxDB_CHANGE_CURSOR,
                0,-100,100,2,2,
                _T("C"),scup2);
    
    sc4 = new wxFDragSpinCtrl(this,2582,wxPoint(40,75),wxSize(120,18),wxDB_HORIZONTAL|wxDB_HIDE_CURSOR|wxDB_SHOW_RANGE_POPUP,
                0,-100,100,2,2,
                _T("l/s"),scup4);

    sc5 = new wxFDragSpinCtrl(this,2583,wxPoint(40,20),wxSize(100,25),wxDB_HORIZONTAL|wxDB_FREEZE_CURSOR|wxDB_SHOW_RANGE_POPUP,
                0,-10,10,3,0,
                _T("unit"),scup3);
    
    sc6 = new wxFDragSpinCtrl(this,2584,wxPoint(40,20),wxSize(100,25),wxDB_HORIZONTAL|wxDB_FREEZE_CURSOR|wxDB_SHOW_RANGE_POPUP,
                0,-10,10,3,1,
                _T("m^3"),scup3);                          
             
    sc3->SetDCLogarithmic(true);
    sc4->SetDCLogarithmic(false);
    sc2->SetValue(7);



    sizer->Add(st2);
    sizer->Add(sc2);
    sizer->Add(st3);
    sizer->Add(sc3);
    sizer->Add(st4);
    sizer->Add(sc4);
    sizer->Add(st5);
    sizer->Add(sc5);
    sizer->Add(st6);
    sizer->Add(sc6);
      
    /*sizer->Add(tc1);
    sizer->Add(tc2);
    */
    //sc6->Disable();
    //sc5->Disable();
    sc3->Disable();
    sc2->Disable();
    //sc4->Disable(); 
    
    

    
    SetSizer(sizer);
    Show();

  
    //Disable();
   
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
       : wxFrame(NULL, -1, title, pos, size, style)
{   
     n = new wxNotebook(this,-1, wxPoint(50,50), wxSize(300,300));
    
    
     n->AddPage((wxNotebookPage*)new MyPanel(n,-1, wxPoint(50,50),wxSize(300,220),0,_T("....")) , _T("one"));
     n->AddPage((wxNotebookPage*)new MyPanel2(n,-1, wxPoint(50,50),wxSize(300,220),0,_T("....")) , _T("two"));
}


