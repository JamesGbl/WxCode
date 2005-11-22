/*
  demowxplot By Germán Carrera

  code taken from some examples, from wxwindows samples, plplot webpage examples
  and old wxplot - plplot

  is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Library Public License as published
  by the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

*/


#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
    #include "wx/mdi.h"
#endif


#if !wxUSE_PRINTING_ARCHITECTURE
#error You must set wxUSE_PRINTING_ARCHITECTURE to 1 in setup.h to compile this demo.
#endif

// Set this to 1 if you want to test PostScript printing under MSW.
// However, you'll also need to edit src/msw/makefile.nt.
#define wxTEST_POSTSCRIPT_IN_MSW 0

#include <ctype.h>
#include "wx/metafile.h"
#include "wx/print.h"
#include "wx/printdlg.h"

#include "wx/accel.h"

#if wxTEST_POSTSCRIPT_IN_MSW
#include "wx/generic/printps.h"
#include "wx/generic/prntdlgg.h"
#endif

#include "wxPlot/wxPlot.h"
#include <math.h>
#include "demowxplot.h"

#define PI 3.1415926535
#define PERIMETERPTS 100
#define LEVELS 10

#define XPTS   35		/* Data points in x */
#define YPTS   46		/* Data points in y */

IMPLEMENT_APP(MyApp)

// ---------------------------------------------------------------------------
// global variables
// ---------------------------------------------------------------------------

MyFrame *frame = (MyFrame *) NULL;


wxPrintData *g_printData = (wxPrintData*) NULL ;

wxPageSetupData* g_pageSetupData = (wxPageSetupData*) NULL;

wxList my_children;

static int gs_nFrames = 0;
bool arrwindow[7];
int maximized;



// ---------------------------------------------------------------------------
// event tables
// ---------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyFrame, wxMDIParentFrame)
    EVT_MENU(MDI_ABOUT, MyFrame::OnAbout)
    
    EVT_MENU(MDI_NEW_WINDOW1, MyFrame::OnNewWindow1)
    EVT_MENU(MDI_NEW_WINDOW2, MyFrame::OnNewWindow2)
    EVT_MENU(MDI_NEW_WINDOW3, MyFrame::OnNewWindow3)
    EVT_MENU(MDI_NEW_WINDOW4, MyFrame::OnNewWindow4)
    EVT_MENU(MDI_NEW_WINDOW5, MyFrame::OnNewWindow5)
    EVT_MENU(MDI_NEW_WINDOW6, MyFrame::OnNewWindow6)
    
    EVT_MENU(ID_PRINT, MyFrame::OnPrint)
    EVT_MENU(ID_PRINT_SETUP, MyFrame::OnPrintSetup)
    EVT_MENU(ID_PREVIEW,MyFrame::OnPrintPreview)
    EVT_MENU(ID_PAGE_SETUP, MyFrame::OnPageSetup)
  
    EVT_MENU_OPEN       (MyFrame::OnMenuOpen)
    
    
    EVT_MENU(MDI_QUIT, MyFrame::OnQuit)
    EVT_CLOSE(MyFrame::OnClose)
    EVT_SIZE(MyFrame::OnSize)
    
    
    
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(MyChild, wxMDIChildFrame)
    EVT_MENU(MDI_CHILD_QUIT, MyChild::OnQuit)
    EVT_MENU(MDI_REFRESH, MyChild::OnRefresh)

    EVT_MENU(MDI_CHANGE_POSITION, MyChild::OnChangePosition)
    EVT_MENU(MDI_CHANGE_SIZE, MyChild::OnChangeSize)

    EVT_SIZE(MyChild::OnSize)
    EVT_MOVE(MyChild::OnMove)

    EVT_CLOSE(MyChild::OnClose)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
    EVT_MOUSE_EVENTS(MyCanvas::OnEvent)
    EVT_SIZE(MyCanvas::ReDraw)
END_EVENT_TABLE()


// ---------------------------------------------------------------------------
// MyApp
// ---------------------------------------------------------------------------

// Initialise this in OnInit, not statically
bool MyApp::OnInit()
{
  
#ifdef __WXMSW__
#if 0
    wxMenu* windowMenu = new wxMenu;
    windowMenu->Append(5000, _T("My menu item!"));
    frame->SetWindowMenu(windowMenu);
#endif
#endif


    // Make a menubar
    file_menu = new wxMenu;

    file_menu->Append(MDI_NEW_WINDOW1, _T("&Plot1\tCtrl-1"), _T("Create a new child window"));
    file_menu->Append(MDI_NEW_WINDOW2, _T("&Plot2\tCtrl-2"), _T("Create a new child window"));
    file_menu->Append(MDI_NEW_WINDOW3, _T("&Plot3\tCtrl-3"), _T("Create a new child window"));
    file_menu->Append(MDI_NEW_WINDOW4, _T("&Plot4\tCtrl-4"), _T("Create a new child window"));
    file_menu->Append(MDI_NEW_WINDOW5, _T("&Plot5\tCtrl-5"), _T("Create a new child window"));
    file_menu->Append(MDI_NEW_WINDOW6, _T("&Plot6\tCtrl-6"), _T("Create a new child window"));
    file_menu->AppendSeparator();
    file_menu->Append( ID_PRINT, _T("&Print...") );
    file_menu->Append( ID_PRINT_SETUP, _T("&Print &Setup...") );
    file_menu->Append( ID_PAGE_SETUP, _T("&Page Set&up...") );
    file_menu->Append( ID_PREVIEW, _T("&Print Pre&view...") );
    
    file_menu->Enable( ID_PRINT, false);
    file_menu->Enable( ID_PAGE_SETUP, false);
    file_menu->Enable( ID_PREVIEW, false);
    
    file_menu->AppendSeparator();

    file_menu->Append(MDI_QUIT, _T("&Exit\tAlt-X"), _T("Quit the program"));

    wxMenu *help_menu = new wxMenu;
    help_menu->Append(MDI_ABOUT, _T("&About\tF1"));

    wxMenuBar *menu_bar = new wxMenuBar;

    menu_bar->Append(file_menu, _T("&File"));
    menu_bar->Append(help_menu, _T("&Help"));

    frame = new MyFrame((wxFrame *)NULL, -1, _T("wxPlot Demo"),
                        wxPoint(-1, -1), wxSize(500, 500),
                        wxDEFAULT_FRAME_STYLE, file_menu);

    frame->SetMenuBar(menu_bar);

    frame->Show(TRUE);

    SetTopWindow(frame);

    return TRUE;
}

// ---------------------------------------------------------------------------
// MyFrame
// ---------------------------------------------------------------------------

MyFrame::MyFrame(wxWindow *parent,
                 const wxWindowID id,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size,
                 const long style,wxMenu *file_menu_)
       : wxMDIParentFrame(parent, id, title, pos, size,
                          style | wxNO_FULL_REPAINT_ON_RESIZE | wxFRAME_NO_WINDOW_MENU)
{
    wxAcceleratorEntry entries[3];
   
    entries[1].Set(wxACCEL_CTRL, (int) 'X', MDI_QUIT);
    entries[2].Set(wxACCEL_CTRL, (int) 'A', MDI_ABOUT);
    wxAcceleratorTable accel(3, entries);
    SetAcceleratorTable(accel);

    file_menu=file_menu_;

    g_printData = new wxPrintData;
    g_pageSetupData = new wxPageSetupDialogData;
}

void MyFrame::OnClose(wxCloseEvent& event)
{
    event.Skip();
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event) )
{
    (void)wxMessageBox(_T("Demo wxPlot - plplot-5.5.1\n")
                       _T("By German Carrera 2005\n")
                       _T("with the colaboration of\n")
                       _T("Amparo Gil & Javier J. Segura\n")
                       _T("at the University of Cantabria\n")
                       _T(""), _T("About Demo wxPlot"));
}

void MyFrame::OnNewWindow1(wxCommandEvent& WXUNUSED(event))
{
    if(arrwindow[1]==false)
      {
        MyChild *subframe = new MyChild(frame, _T("wxplot Demo"),
                                    wxPoint(-1, -1), wxSize(-1, -1),
                                    wxDEFAULT_FRAME_STYLE, 1);

        wxString title;
        title.Printf(_T("wxplot Demo %d"), 1);

        gs_nFrames++;
      
        subframe->SetTitle(title);

        int width, height;
        subframe->GetClientSize(&width, &height);
    
        int type=1;
    
        MyCanvas *canvas = new MyCanvas(subframe, wxPoint(0, 0), wxSize(width, height), type);

        subframe->canvas = canvas;
    
        subframe->SetClientSize(325, 325);
        canvas->SetSize(325, 325);

        mchild[1]=subframe;

        subframe->Show(TRUE);
        
        arrwindow[1]=true;
      }  
    
}

void MyFrame::OnNewWindow2(wxCommandEvent& WXUNUSED(event))
{
    if(arrwindow[2]==false)
      {
        MyChild *subframe = new MyChild(frame, _T("wxplot Demo"),
                                    wxPoint(-1, -1), wxSize(-1, -1),
                                    wxDEFAULT_FRAME_STYLE, 2);

        wxString title;
        title.Printf(_T("wxplot Demo %d"), 2);

        gs_nFrames++;
      
        subframe->SetTitle(title);

        int width, height;
        subframe->GetClientSize(&width, &height);
    
        int type=2;
    
        MyCanvas *canvas = new MyCanvas(subframe, wxPoint(0, 0), wxSize(width, height), type);

        subframe->canvas = canvas;
    
        subframe->SetClientSize(325, 325);
        canvas->SetSize(325, 325);

        mchild[2]=subframe;

        subframe->Show(TRUE);
        
        arrwindow[2]=true;
      }  
    
}

void MyFrame::OnNewWindow3(wxCommandEvent& WXUNUSED(event))
{
    if(arrwindow[3]==false)
      {
        MyChild *subframe = new MyChild(frame, _T("wxplot Demo"),
                                    wxPoint(-1, -1), wxSize(-1, -1),
                                    wxDEFAULT_FRAME_STYLE, 3);

        wxString title;
        title.Printf(_T("wxplot Demo %d"), 3);

        gs_nFrames++;
      
        subframe->SetTitle(title);

        int width, height;
        subframe->GetClientSize(&width, &height);
    
        int type=3;
    
        MyCanvas *canvas = new MyCanvas(subframe, wxPoint(0, 0), wxSize(width, height), type);

        subframe->canvas = canvas;
    
        subframe->SetClientSize(325, 325);
        canvas->SetSize(325, 325);

        mchild[3]=subframe;

        subframe->Show(TRUE);
        
        arrwindow[3]=true;
      }  
    
}

void MyFrame::OnNewWindow4(wxCommandEvent& WXUNUSED(event))
{
    if(arrwindow[4]==false)
      {
        MyChild *subframe = new MyChild(frame, _T("wxplot Demo"),
                                    wxPoint(-1, -1), wxSize(-1, -1),
                                    wxDEFAULT_FRAME_STYLE, 4);

        wxString title;
        title.Printf(_T("wxplot Demo %d"), 4);

        gs_nFrames++;
      
        subframe->SetTitle(title);

        int width, height;
        subframe->GetClientSize(&width, &height);
    
        int type=4;
    
        MyCanvas *canvas = new MyCanvas(subframe, wxPoint(0, 0), wxSize(width, height), type);

        subframe->canvas = canvas;
    
        subframe->SetClientSize(325, 325);
        canvas->SetSize(325, 325);

        mchild[4]=subframe;

        subframe->Show(TRUE);
        
        arrwindow[4]=true;
      }  
    
}

void MyFrame::OnNewWindow5(wxCommandEvent& WXUNUSED(event))
{
    if(arrwindow[5]==false)
      {
        MyChild *subframe = new MyChild(frame, _T("wxplot Demo"),
                                    wxPoint(-1, -1), wxSize(-1, -1),
                                    wxDEFAULT_FRAME_STYLE, 5);

        wxString title;
        title.Printf(_T("wxplot Demo %d"), 5);

        gs_nFrames++;
      
        subframe->SetTitle(title);

        int width, height;
        subframe->GetClientSize(&width, &height);
    
        int type=5;
    
        MyCanvas *canvas = new MyCanvas(subframe, wxPoint(0, 0), wxSize(width, height), type);

        subframe->canvas = canvas;
    
        subframe->SetClientSize(325, 325);
        canvas->SetSize(325, 325);

        mchild[5]=subframe;

        subframe->Show(TRUE);
        
        arrwindow[5]=true;
      }  
    
}

void MyFrame::OnNewWindow6(wxCommandEvent& WXUNUSED(event))
{
    if(arrwindow[6]==false)
      {
        MyChild *subframe = new MyChild(frame, _T("wxplot Demo"),
                                    wxPoint(-1, -1), wxSize(-1, -1),
                                    wxDEFAULT_FRAME_STYLE, 6);

        wxString title;
        title.Printf(_T("wxplot Demo %d"), 6);

        gs_nFrames++;
      
        subframe->SetTitle(title);

        int width, height;
        subframe->GetClientSize(&width, &height);
    
        int type=6;
    
        MyCanvas *canvas = new MyCanvas(subframe, wxPoint(0, 0), wxSize(width, height), type);

        subframe->canvas = canvas;
    
        subframe->SetClientSize(325, 325);
        canvas->SetSize(325, 325);

        mchild[6]=subframe;

        subframe->Show(TRUE);
        
        arrwindow[6]=true;
      }  
    
}

void MyFrame::OnSize(wxSizeEvent& event)
{
    int w, h;
    GetClientSize(&w, &h);

    GetClientWindow()->SetSize(w, h);
   
#ifdef __WXUNIVERSAL__   
    event.Skip();
#endif
}

void MyFrame::OnPrint(wxCommandEvent& WXUNUSED(event))
{
   wxPrintDialogData printDialogData(* g_printData);

    wxPrinter printer(& printDialogData);
    MyPrintout printout("My printout",mchild[maximized]->canvas);
    if (!printer.Print(this, &printout, TRUE))
        wxMessageBox(_T("There was a problem printing.\nPerhaps your current printer is not set correctly?"), _T("Printing"), wxOK);
    else
    {
        (*g_printData) = printer.GetPrintDialogData().GetPrintData();
    }
}

void MyFrame::OnPrintPreview(wxCommandEvent& WXUNUSED(event))
{
    // Pass two printout objects: for preview, and possible printing.
    wxPrintDialogData printDialogData(* g_printData);
    wxPrintPreview *preview = new wxPrintPreview(new MyPrintout("My printout",mchild[maximized]->canvas), new MyPrintout("My printout",mchild[maximized]->canvas), & printDialogData);
    if (!preview->Ok())
    {
        delete preview;
        wxMessageBox(_T("There was a problem previewing.\nPerhaps your current printer is not set correctly?"), _T("Previewing"), wxOK);
        return;
    }
    
    wxPreviewFrame *frame = new wxPreviewFrame(preview, this, _T("Demo Print Preview"), wxDefaultPosition, wxDefaultSize); 
    frame->Centre(wxBOTH);
    frame->Initialize();
    frame->Show(TRUE);
}

void MyFrame::OnPrintSetup(wxCommandEvent& WXUNUSED(event))
{
    wxPrintDialogData printDialogData(* g_printData);
    wxPrintDialog printerDialog(this, & printDialogData);
    
    printerDialog.GetPrintDialogData().SetSetupDialog(TRUE);
    printerDialog.ShowModal();

    (*g_printData) = printerDialog.GetPrintDialogData().GetPrintData();
}

void MyFrame::OnMenuOpen(wxMenuEvent& WXUNUSED(event))
{
  if(maximized>0)
    {
      file_menu->Enable( ID_PRINT, true);
      file_menu->Enable( ID_PAGE_SETUP, true);
      file_menu->Enable( ID_PREVIEW, true);
    }
  if(maximized==0)
    {
      file_menu->Enable( ID_PRINT, false);
      file_menu->Enable( ID_PAGE_SETUP, false);
      file_menu->Enable( ID_PREVIEW, false);
    }
}

void MyFrame::OnPageSetup(wxCommandEvent& WXUNUSED(event))
{
    (*g_pageSetupData) = * g_printData;

    wxPageSetupDialog pageSetupDialog(this, g_pageSetupData);
    pageSetupDialog.ShowModal();
    
    (*g_printData) = pageSetupDialog.GetPageSetupData().GetPrintData();
    (*g_pageSetupData) = pageSetupDialog.GetPageSetupData();
}

// ---------------------------------------------------------------------------
// MyCanvas
// ---------------------------------------------------------------------------

MyCanvas::MyCanvas(wxWindow *parent, const wxPoint& pos, const wxSize& size, const int type_)
:wxScrolledWindow(parent, -1, pos, size, 
                           wxSUNKEN_BORDER |
                           wxNO_FULL_REPAINT_ON_RESIZE)
{
    type=type_;

    SetBackgroundColour(wxColour(_T("BLACK")));

}


void MyCanvas::OnDraw(wxDC& dc)
{

  wxBrush brush(*wxBLACK_BRUSH);
  
  dc.SetBackground(brush);

  if(type==1)
    Plot1(dc,0,0);
  if(type==2)
    Plot2(dc,0,0,0);
  if(type==3)
    Plot2(dc,1,0,0);
  if(type==4)
    Plot2(dc,2,0,0);
  if(type==5)
    Plot2(dc,3,0,0);
  if(type==6)
    Plot3(dc,0,0);
}

/* $Id: demowxplot.cpp,v 1.2 2005-11-22 15:56:49 carreracg Exp $

	Simple line plot and multiple windows demo.

   Copyright (C) 2004  Rafael Laboissiere

  This file is part of PLplot.

  PLplot is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Library Public License as published
  by the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  PLplot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with PLplot; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

void MyCanvas::Plot1(wxDC& dc, int w_, int h_)
{
  wxPlot *wxplot;
  int w,h;
  static PLFLT x[101], y[101];
  
  
  this->GetSize(&w,&h);

  if(w_!=0 && h_!=0)
    {
      w=w_;
      h=h_;
    }
  
  wxplot=new wxPlot(&dc, w, h);

  PLINT space0 = 0, mark0 = 0, space1 = 1500, mark1 = 1500;
  int i;

/* For the final graph we wish to override the default tick intervals, and
 * so do not use plenv().
*/
  wxplot->adv(0);

/* Use standard viewport, and define X range from 0 to 360 degrees, Y range
 * from -1.2 to 1.2. 
*/
  wxplot->vsta();
  wxplot->wind(0.0, 360.0, -1.2, 1.2);
  
/* Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y. */

  wxplot->col0(1);
    
  wxplot->box("bcnst", 60.0, 2, "bcnstv", 0.2, 2);

/* Superimpose a dashed line grid, with 1.5 mm marks and spaces. 
 * plstyl expects a pointer!
*/
  wxplot->styl(1, &mark1, &space1);
  wxplot->col0(2);
  wxplot->box("g", 30.0, 0, "g", 0.2, 0);
  wxplot->styl(0, &mark0, &space0);

  wxplot->col0(3);
  wxplot->lab("Angle (degrees)", "sine", "#frPLplot Example 1 - Sine function");

    for (i = 0; i < 101; i++) {
	x[i] = 3.6 * i;
	y[i] = sin(x[i] * PI / 180.0);
    }

  wxplot->col0(4);
  wxplot->line(101, x, y);
  
  delete(wxplot);
}

/* $Id: demowxplot.cpp,v 1.2 2005-11-22 15:56:49 carreracg Exp $

	plshade demo, using color fill.

	Maurice LeBrun
	IFS, University of Texas at Austin
	20 Mar 1994
*/

void MyCanvas::cmap1_init(int gray)
{
  PLFLT i[2], h[2], l[2], s[2];

  i[0] = 0.0;		/* left boundary */
  i[1] = 1.0;		/* right boundary */

  if (gray) {
    h[0] = 0.0;		/* hue -- low: red (arbitrary if s=0) */
    h[1] = 0.0;		/* hue -- high: red (arbitrary if s=0) */

    l[0] = 0.5;		/* lightness -- low: half-dark */
    l[1] = 1.0;		/* lightness -- high: light */

    s[0] = 0.0;		/* minimum saturation */
    s[1] = 0.0;		/* minimum saturation */
  } else {
    h[0] = 240; /* blue -> green -> yellow -> */
    h[1] = 0;   /* -> red */

    l[0] = 0.6;
    l[1] = 0.6;

    s[0] = 0.8;
    s[1] = 0.8;
  }

  plscmap1n(256);
  c_plscmap1l(0, 2, i, h, l, s, NULL);
}


void MyCanvas::Plot2(wxDC& dc, int k, int w_, int h_)
{

    wxPlot *wxplot;
    int w,h;
 
    int xpts=30;
    int ypts=30;

    this->GetSize(&w,&h);

    if(w_!=0 && h_!=0)
    {
      w=w_;
      h=h_;
    }
    wxplot=new wxPlot(&dc, w, h);

    static int opt[] = {1, 2, 3, 3};
    static PLFLT alt[] = {60.0, 20.0, 60.0, 60.0};
    static PLFLT az[] = {30.0, 60.0, 120.0, 160.0};
    static char *title[4] =
    {
      "#frPLplot Example 8 - Alt=60, Az=30, Opt=1",
      "#frPLplot Example 8 - Alt=20, Az=60, Opt=2",
      "#frPLplot Example 8 - Alt=60, Az=120, Opt=3",
      "#frPLplot Example 8 - Alt=60, Az=160, Opt=4"
    };

    PLFLT x[xpts];
    PLFLT y[xpts];
    PLFLT z[xpts][ypts];
    PLFLT *zp[xpts];

    int i;
    int j;
    PLFLT xx, yy, r;


    for (i = 0; i < xpts; i++) {
      x[i] = (double) (i - (xpts / 2)) / (double) (xpts / 2);
    }
    for (i = 0; i < ypts; i++)
      y[i] = (double) (i - (ypts / 2)) / (double) (ypts / 2);
    for (i = 0; i < xpts; i++) {
      xx = x[i];
      zp[i]= z[i];
    for (j = 0; j < ypts; j++) {
      yy = y[j];
      r = sqrt(xx * xx + yy * yy);
      z[i][j] = -r * r * cos(2.0 * 3.141592654 * r);
    }
    }

  wxplot->adv(0);
  wxplot->vpor(0.0, 1.0, 0.0, 0.9);
  wxplot->wind(-1.0, 1.0, -0.9, 1.1);
  wxplot->col(Red);

  wxplot->w3d(1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, alt[k], az[k]);
  wxplot->box3("bnstu", "x axis", 0.0, 0,
	   "bnstu", "y axis", 0.0, 0,
	   "bcdmnstuv", "z axis", 0.0, 0);

  //wxplot->col(Red);
  //wxplot->plot3d(x, y, zp, xpts, ypts, opt[k], 1);
  
  plscmap1n(256);
  cmap1_init(0);
    
  wxplot->surf3d(x, y, zp, xpts, ypts, MAG_COLOR | SURF_CONT | BASE_CONT, NULL, opt[k]);
  
  wxplot->col0(3);
  wxplot->mtex("t", 1.0, 0.5, 0.5, title[k]);
  delete(wxplot);
}


void MyCanvas::Plot3(wxDC& dc, int w_, int h_)
{
    int i;
    char string[20];
    PLFLT y0[10];

    wxPlot *wxplot;
    int w,h;

    this->GetSize(&w,&h);

    if(w_!=0 && h_!=0)
    {
      w=w_;
      h=h_;
    }
  
    wxplot=new wxPlot(&dc, w, h);

/* Parse and process command line arguments */

   // (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

/* Initialize plplot */


    wxplot->adv(0);
    wxplot->vsta();
    wxplot->wind(1980.0, 1990.0, 0.0, 35.0);
    wxplot->box("bc", 1.0, 0, "bcnv", 10.0, 0);
    wxplot->col0(2);
    wxplot->lab("Year", "Widget Sales (millions)", "#frPLplot Example 12");

    y0[0] = 5;
    y0[1] = 15;
    y0[2] = 12;
    y0[3] = 24;
    y0[4] = 28;
    y0[5] = 30;
    y0[6] = 20;
    y0[7] = 8;
    y0[8] = 12;
    y0[9] = 3;

    for (i = 0; i < 10; i++) 
    {
	wxplot->col0(i + 1);
    
	wxplot->psty(0);
	plfbox((1980. + i), y0[i],wxplot);
	sprintf(string, "%.0f", y0[i]);
	wxplot->ptex((1980. + i + .5), (y0[i] + 1.), 1.0, 0.0, .5, string);
	sprintf(string, "%d", 1980 + i);
	wxplot->mtex("b", 1.0, ((i + 1) * .1 - .05), 0.5, string);
    }
    
    delete(wxplot);
}

void MyCanvas::plfbox(PLFLT x0, PLFLT y0, wxPlot *wxplot)
{
    PLFLT x[4], y[4];

    x[0] = x0;
    y[0] = 0.;
    x[1] = x0;
    y[1] = y0;
    x[2] = x0 + 1.;
    y[2] = y0;
    x[3] = x0 + 1.;
    y[3] = 0.;
    wxplot->fill(4, x, y);
    wxplot->col0(1);
    wxplot->lsty(1);
    wxplot->line(4, x, y);
}

void MyCanvas::OnEvent(wxMouseEvent& event)
{

}

void MyCanvas::ReDraw(wxSizeEvent& event)
{
  this->Refresh();
}


// ---------------------------------------------------------------------------
// MyChild
// ---------------------------------------------------------------------------

MyChild::MyChild(wxMDIParentFrame *parent, const wxString& title,
                 const wxPoint& pos, const wxSize& size,
                 const long style, int type_)
       : wxMDIChildFrame(parent, -1, title, pos, size,
                         style | wxNO_FULL_REPAINT_ON_RESIZE)
{
    canvas = (MyCanvas *) NULL;
    
    SetBackgroundColour(wxColour(_T("BLACK")));

    my_children.Append(this);

    type=type_;

    SetSizeHints(100, 100);
}

MyChild::~MyChild()
{
    arrwindow[type]=false;
    maximized=0;
    my_children.DeleteObject(this);
}

void MyChild::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    arrwindow[type]=false;
    maximized=0;
    Close(TRUE);
}

void MyChild::OnRefresh(wxCommandEvent& WXUNUSED(event))
{
    if ( canvas )
        canvas->Refresh();
}

void MyChild::OnChangePosition(wxCommandEvent& WXUNUSED(event))
{
    Move(10, 10);
}

void MyChild::OnChangeSize(wxCommandEvent& WXUNUSED(event))
{
    SetClientSize(100, 100);
}

void MyChild::OnActivate(wxActivateEvent& event)
{
    if ( event.GetActive() && canvas )
        canvas->SetFocus();
}

void MyChild::OnMove(wxMoveEvent& event)
{
    event.Skip();
}

void MyChild::OnSize(wxSizeEvent& event)
{
#ifdef WIN32
  if(this->IsMaximized())
  {
    maximized=type;
  }else
  {
    maximized=0;
  }
#else
  maximized=type;
#endif
    event.Skip();
}


void MyChild::OnClose(wxCloseEvent& event)
{
    gs_nFrames--;
    arrwindow[type]=false;
    maximized=0;

    event.Skip();
}

/*
MyPrintout class
*/

MyPrintout::MyPrintout(char *title, MyCanvas *canvas_)
{
  title="MyPrintOut";
  canvas=canvas_;
}

bool MyPrintout::OnPrintPage(int page)
{
  int ppiScreenX, ppiScreenY;
  int ppiPrinterX, ppiPrinterY;
  double scale;
  double overallScale;
  int w,h;
  int pageWidth, pageHeight;
#ifdef WIN32
  dc= new wxDC();
#endif
  dc = GetDC();
 

  if (dc)
    {
      GetPPIScreen(&ppiScreenX, &ppiScreenY);
      GetPPIPrinter(&ppiPrinterX, &ppiPrinterY);
      scale = (double)((double)ppiPrinterX/(double)ppiScreenX);
      dc->GetSize(&w, &h);
      GetPageSizePixels(&pageWidth, &pageHeight);
      overallScale = scale * ((double)w/(double)pageWidth);
      dc->SetUserScale(overallScale, overallScale);
      int pagew=(int)(pageWidth/scale);
      int pageh=(int)(pageHeight/scale);
      
      if(maximized==1)
        canvas->Plot1(*dc,pagew,pageh);
      if(maximized==2)
        canvas->Plot2(*dc,0,pagew,pageh);
      if(maximized==3)
        canvas->Plot2(*dc,1,pagew,pageh);
      if(maximized==4)
        canvas->Plot2(*dc,2,pagew,pageh);
      if(maximized==5)
        canvas->Plot2(*dc,3,pagew,pageh);
      if(maximized==6)
        canvas->Plot3(*dc,pagew,pageh);
      

      dc->SetDeviceOrigin(0, 0);
      dc->SetUserScale(1.0, 1.0);
      
      char buf[200];
      sprintf(buf, "PLOT %d", maximized);
   
      return TRUE;
    }
  else
    return FALSE;
    
}

/*
MyPrintOut Class
*/

bool MyPrintout::OnBeginDocument(int startPage, int endPage)
{
    if (!wxPrintout::OnBeginDocument(startPage, endPage))
        return FALSE;
    
    return TRUE;
}

void MyPrintout::GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo)
{
  *minPage = 1;
  *maxPage = 1;
  *selPageFrom = 1;
  *selPageTo = 1;
}

bool MyPrintout::HasPage(int pageNum)
{
    return (pageNum > 0 && pageNum <= 6);
}


