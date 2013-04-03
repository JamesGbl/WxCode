///////////////////////////////////////////////////////////////////////////////
// Name:        wxplotctrl.cpp
// Purpose:     Simple wxSTEditor app
// Author:      John Labenski
// Modified by:
// Created:     04/01/98
// RCS-ID:
// Copyright:   (c) John Labenski
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////


// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/cmdline.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/splitter.h>

#include "wx/plotctrl/plotctrl.h"
#include "wx/plotctrl/plotprnt.h"

// ----------------------------------------------------------------------------

enum Menu_IDs
{
    ID_OPEN = 10,
    ID_FUNC,
    ID_PRINT,
    ID_PRINT_PREVIEW,
    ID_QUIT,

    ID_ADD_MARKER,

    ID_SCROLL_ON_THUMB,
    ID_CROSSHAIR_CURSOR,

    ID_DRAW_SYMBOLS,
    ID_DRAW_LINES,
    ID_DRAW_SPLINE,
    ID_DRAW_GRID,
    ID_SHOW_XAXIS,
    ID_SHOW_YAXIS,
    ID_SHOW_XLABEL,
    ID_SHOW_YLABEL,
    ID_SHOW_TITLE,
    ID_SHOW_KEY,

    ID_ABOUT
};

// ----------------------------------------------------------------------------
// wxPlotCtrlApp - the application class
// ----------------------------------------------------------------------------
class wxPlotCtrlApp : public wxApp
{
public:
    wxPlotCtrlApp() : wxApp(), m_frame(NULL) {}
    virtual bool OnInit();
    virtual int OnExit();

    wxFrame* m_frame;
};

IMPLEMENT_APP(wxPlotCtrlApp)

// ----------------------------------------------------------------------------
// wxPlotCtrlFrame - the application frame
// ----------------------------------------------------------------------------
class wxPlotCtrlFrame : public wxFrame
{
public:
    wxPlotCtrlFrame();

    void OnMenu(wxCommandEvent& event);
    void OnPlotCtrl(wxPlotCtrlEvent& event);

    wxPlotCtrl *m_plotCtrl;
    wxTextCtrl *m_textCtrl;

    wxString m_functionStr;

private:
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// wxPlotCtrlApp - the application class
// ----------------------------------------------------------------------------

bool wxPlotCtrlApp::OnInit()
{
    m_frame = new wxPlotCtrlFrame();
    m_frame->Show(true);

    return true;
}

int wxPlotCtrlApp::OnExit()
{
    return wxApp::OnExit();
}

// ----------------------------------------------------------------------------
// wxPlotCtrlFrame - the application frame
// ----------------------------------------------------------------------------
BEGIN_EVENT_TABLE(wxPlotCtrlFrame, wxFrame)
    EVT_MENU                        (wxID_ANY, wxPlotCtrlFrame::OnMenu)

    EVT_PLOTCTRL_ADD_CURVE          (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_DELETING_CURVE     (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_DELETED_CURVE      (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)

    EVT_PLOTCTRL_CURVE_SEL_CHANGING (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_CURVE_SEL_CHANGED  (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)

    EVT_PLOTCTRL_MOUSE_MOTION       (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_CLICKED            (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_DOUBLECLICKED      (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_POINT_CLICKED      (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_POINT_DOUBLECLICKED(wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)

    EVT_PLOTCTRL_AREA_SEL_CREATING  (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_AREA_SEL_CHANGING  (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_AREA_SEL_CREATED   (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)

    EVT_PLOTCTRL_VIEW_CHANGING      (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_VIEW_CHANGED       (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)

    EVT_PLOTCTRL_CURSOR_CHANGING    (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_CURSOR_CHANGED     (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)

    EVT_PLOTCTRL_ERROR              (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)

    EVT_PLOTCTRL_BEGIN_TITLE_EDIT   (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_END_TITLE_EDIT     (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_BEGIN_X_LABEL_EDIT (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_END_X_LABEL_EDIT   (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_BEGIN_Y_LABEL_EDIT (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_END_Y_LABEL_EDIT   (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)

    EVT_PLOTCTRL_MOUSE_FUNC_CHANGING (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)
    EVT_PLOTCTRL_MOUSE_FUNC_CHANGED  (wxID_ANY, wxPlotCtrlFrame::OnPlotCtrl)

END_EVENT_TABLE()

wxPlotCtrlFrame::wxPlotCtrlFrame() : wxFrame()
{
    m_plotCtrl = NULL;
    m_textCtrl = NULL;
    m_functionStr = wxT("1.2*x*sin((x^2)/2)");

    if (!wxFrame::Create(NULL, wxID_ANY, wxT("wxPlotCtrl sample"),
                         wxDefaultPosition, wxSize(600, 400)))
        return;

    wxStatusBar* statusBar = CreateStatusBar(1);
    SetStatusBar(statusBar);

    // create a menu bar
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(ID_OPEN, _T("&Open file...\tCtrl-O"), _T("Open a data file..."));
    fileMenu->Append(ID_FUNC, _T("Add &function...\tCtrl-F"), _T("Plot a function curve..."));
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_PRINT, _T("&Print...\tCtrl-P"), _T("Print current window"));
    fileMenu->Append(ID_PRINT_PREVIEW, _T("Print p&review...\tCtrl-R"), _T("Print preview of the current window"));
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_QUIT, _T("E&xit\tAlt-X"), _T("Quit this program"));

    wxMenu *markerMenu = new wxMenu;
    markerMenu->Append(ID_ADD_MARKER,  _T("Add some markers"), _T("Add assorted markers to the plot"));

    wxMenu *plotMenu = new wxMenu;
    plotMenu->Append(ID_SCROLL_ON_THUMB,  _T("Scroll on thumb release"), _T("Delay scrolling until thumb is released"), true);
    plotMenu->Append(ID_CROSSHAIR_CURSOR, _T("Crosshair cursor"), _T("Show the cursor as a crosshair"), true);

    wxMenu *viewMenu = new wxMenu;
    viewMenu->Append(ID_DRAW_SYMBOLS, _T("Draw curve symbols"), _T("Draw symbols for each curve data point"), true);
    viewMenu->Append(ID_DRAW_LINES,   _T("Draw curve lines"),   _T("Draw the curves as interconnecting lines"), true);
    viewMenu->Append(ID_DRAW_SPLINE,  _T("Draw curve spline"),  _T("Draw the curves as splines"), true);
    viewMenu->Append(ID_DRAW_GRID,    _T("Draw plot grid"),     _T("Draw the gridlines in the plot"), true);
    viewMenu->AppendSeparator();
    viewMenu->Append(ID_SHOW_XAXIS,  _T("Show x-axis"), _T("Show the x-axis"), true);
    viewMenu->Append(ID_SHOW_YAXIS,  _T("Show y-axis"), _T("Show the y-axis"), true);
    viewMenu->Append(ID_SHOW_XLABEL, _T("Show x-axis label"), _T("Show the x-axis label"), true);
    viewMenu->Append(ID_SHOW_YLABEL, _T("Show y-axis label"), _T("Show the y-axis label"), true);
    viewMenu->Append(ID_SHOW_TITLE,  _T("Show title"), _T("Show the title of the plot"), true);
    viewMenu->Append(ID_SHOW_KEY,    _T("Show key"), _T("Show a key for the plot"), true);

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(ID_ABOUT, _T("&About...\tF1"), _T("Show about dialog"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu,   _T("&File"));
    menuBar->Append(markerMenu, _T("&Markers"));
    menuBar->Append(plotMenu,   _T("&Plot"));
    menuBar->Append(viewMenu,   _T("&View"));
    menuBar->Append(helpMenu,   _T("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

    wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY);
    splitter->SetMinimumPaneSize(50);
    splitter->SetSashGravity(.9);

    m_textCtrl = new wxTextCtrl(splitter, wxID_ANY, wxEmptyString,
                                wxDefaultPosition, wxDefaultSize,
                                wxTE_MULTILINE|wxTE_READONLY);

    m_plotCtrl = new wxPlotCtrl(splitter, wxID_ANY);

    m_textCtrl->AppendText(
        wxT("Move origin: Scrollbars, left-mouse drag on axis labels, arrow keys, shift+ctrl+alt and left mouse drag.\n")
        wxT("Zoom plot: shift+ctrl and left mouse drag to select area or [qwe|asd|zxc] keys, where 's' fits plot.\n")
        wxT("Select curve: shift key cycles through them.\n")
        wxT("Delete curve: delete key.\n")
        wxT("Select points: shift + left mouse drag to select area.\n")
        wxT("Unselect points: ctrl + left mouse drag to select area.\n")
        wxT("Move cursor: Select point w/ left mouse button, move forward and backwards using ',<' or '.>' keys.\n")
    );
    m_textCtrl->AppendText(wxT("For the complete list of mouse and key functions, please see the functions\n  wxPlotCtrl::ProcessAreaEVT_MOUSE_EVENTS and wxPlotCtrl::ProcessAreaEVT_CHAR\n\n"));

    m_textCtrl->SetInsertionPointEnd();

    splitter->SplitHorizontally(m_plotCtrl, m_textCtrl, 200);

    // setup the menu items to match the state of the wxPlotCtrl
    plotMenu->Check(ID_SCROLL_ON_THUMB,  m_plotCtrl->GetScrollOnThumbRelease());
    plotMenu->Check(ID_CROSSHAIR_CURSOR, m_plotCtrl->GetCrossHairCursor());

    viewMenu->Check(ID_DRAW_SYMBOLS,     m_plotCtrl->GetDrawSymbols());
    viewMenu->Check(ID_DRAW_LINES,       m_plotCtrl->GetDrawLines());
    viewMenu->Check(ID_DRAW_SPLINE,      m_plotCtrl->GetDrawSpline());
    viewMenu->Check(ID_DRAW_GRID,        m_plotCtrl->GetDrawGrid());

    viewMenu->Check(ID_SHOW_XAXIS,  m_plotCtrl->GetShowXAxis());
    viewMenu->Check(ID_SHOW_YAXIS,  m_plotCtrl->GetShowYAxis());
    viewMenu->Check(ID_SHOW_XLABEL, m_plotCtrl->GetShowXAxisLabel());
    viewMenu->Check(ID_SHOW_YLABEL, m_plotCtrl->GetShowYAxisLabel());
    viewMenu->Check(ID_SHOW_TITLE,  m_plotCtrl->GetShowPlotTitle());
    viewMenu->Check(ID_SHOW_KEY,    m_plotCtrl->GetShowKey());
}

void wxPlotCtrlFrame::OnMenu(wxCommandEvent& event)
{
    switch (event.GetId())
    {
        case ID_OPEN :
        {
            wxFileDialog fileDialog(this,
                                    wxT("Open file"),
                                    wxT(""),
                                    wxT(""),
                                    wxT("All files (*)|*"),
                                    wxFD_OPEN | wxFD_FILE_MUST_EXIST);

            if (fileDialog.ShowModal() == wxID_OK)
            {
                wxPlotData plotData;
                plotData.LoadFile(fileDialog.GetPath());
                if (plotData.Ok())
                {
                    m_plotCtrl->AddCurve(plotData, true, true);
                }
            }
            break;
        }
        case ID_FUNC :
        {
            wxString func = m_functionStr;

            while (!func.IsEmpty())
            {
                func = wxGetTextFromUser(wxT("Enter function to plot"),
                                         wxT("Function curve"),
                                         func, this);

                if (func.IsEmpty()) return;

                wxPlotFunction plotFunc;
                plotFunc.Create(func, wxT("x"));
                if (!plotFunc.Ok())
                {
                    int ret = wxMessageBox(plotFunc.GetErrorMsg(), wxT("Function error"), wxOK|wxCANCEL|wxICON_ERROR, this);
                    if (ret == wxCANCEL) break;
                }
                else
                {
                    m_functionStr = func;
                    m_plotCtrl->AddCurve(plotFunc, true, true);
                    break;
                }
            }

            break;
        }

        case ID_PRINT :
        case ID_PRINT_PREVIEW :
        {
            wxPlotPrintout plotPrint(m_plotCtrl, wxT("wxPlotCtrl Printout"));

            if (event.GetId() == ID_PRINT)
                plotPrint.ShowPrintDialog();
            else
                plotPrint.ShowPrintPreviewDialog(wxT("wxPlotCtrl Print Preview"));

            break;
        }

        case ID_QUIT : Close(true); return;


        case ID_ADD_MARKER :
        {
            wxString s;
            wxGenericPen p(wxGenericColour(255,0,0), 1);

            wxPlotMarker m;

            m.CreatePointMarker(wxPoint2DDouble(2,2), p);
            m_plotCtrl->AddMarker(m);
            s += wxString::Format(wxT("Adding red PointMarker at (2, 2)\n"));

            m.CreateLineMarker(wxRect2DDouble(-1, 1, 2, -2), wxGenericPen(wxGenericColour(0,255,0), 1));
            m_plotCtrl->AddMarker(m);
            s += wxString::Format(wxT("Adding green LineMarker at (-1, 1, 2, -2)\n"));

            m.CreateLineMarker(wxRect2DDouble(-1, -1, 2, 2), wxGenericPen(wxGenericColour(0,0,255), 1));
            m_plotCtrl->AddMarker(m);
            s += wxString::Format(wxT("Adding blue LineMarker at (-1, -1, 2, 2)\n"));

            m.CreateHorizLineMarker(-2, p);
            m_plotCtrl->AddMarker(m);
            s += wxString::Format(wxT("Adding red HorizLineMarker at y = -2\n"));

            m.CreateVertLineMarker(3, p);
            m_plotCtrl->AddMarker(m);
            s += wxString::Format(wxT("Adding red VertLineMarker at x = 3\n"));

            m.CreateCrossMarker(wxPoint2DDouble(-5, -5), wxGenericPen(wxGenericColour(0,0,0), 1));
            m_plotCtrl->AddMarker(m);
            s += wxString::Format(wxT("Adding black CrossMarker at (-5, -5)\n"));

            m.CreateRectMarker(wxRect2DDouble(-9,-9,1,1), p, wxNullGenericBrush);
            m_plotCtrl->AddMarker(m);
            s += wxString::Format(wxT("Adding red RectMarker at (-9, -9, 1, 1)\n"));

            m.CreateRectMarker(wxPoint2DDouble(6,6), wxSize(20,20), wxGenericPen(wxGenericColour(0,0,255), 1), wxNullGenericBrush);
            m_plotCtrl->AddMarker(m);
            s += wxString::Format(wxT("Adding blue RectMarker at (6, 6) or pixel size (20, 20)\n"));

            m_textCtrl->AppendText(s);
            break;
        }

        case ID_SCROLL_ON_THUMB  : m_plotCtrl->SetScrollOnThumbRelease(event.IsChecked()); break;
        case ID_CROSSHAIR_CURSOR : m_plotCtrl->SetCrossHairCursor(event.IsChecked()); break;
        case ID_DRAW_SYMBOLS     : m_plotCtrl->SetDrawSymbols(event.IsChecked()); break;
        case ID_DRAW_LINES       : m_plotCtrl->SetDrawLines(event.IsChecked()); break;
        case ID_DRAW_SPLINE      : m_plotCtrl->SetDrawSpline(event.IsChecked()); break;
        case ID_DRAW_GRID        : m_plotCtrl->SetDrawGrid(event.IsChecked()); break;

        case ID_SHOW_XAXIS  : m_plotCtrl->SetShowXAxis(event.IsChecked()); break;
        case ID_SHOW_YAXIS  : m_plotCtrl->SetShowYAxis(event.IsChecked()); break;
        case ID_SHOW_XLABEL : m_plotCtrl->SetShowXAxisLabel(event.IsChecked()); break;
        case ID_SHOW_YLABEL : m_plotCtrl->SetShowYAxisLabel(event.IsChecked()); break;
        case ID_SHOW_TITLE  : m_plotCtrl->SetShowPlotTitle(event.IsChecked()); break;
        case ID_SHOW_KEY    : m_plotCtrl->SetShowKey(event.IsChecked()); break;

        case ID_ABOUT :
        {
            wxMessageBox(wxString::Format(
                    _T("Welcome to %s using %s!\n\nThis is the wxPlotCtrl wxCode sample\nrunning under %s."),
                    wxPLOTCTRL_VERSION_STRING, wxVERSION_STRING, wxGetOsDescription().c_str()
                 ),
                 _T("About wxPlotCtrl sample"),
                 wxOK | wxICON_INFORMATION,
                 this);

            return;
        }
    }
}

void wxPlotCtrlFrame::OnPlotCtrl(wxPlotCtrlEvent& event)
{
    event.Skip();

    if (!m_plotCtrl) return; // wait until the pointer is set

    wxEventType eventType = event.GetEventType();
    int active_index = m_plotCtrl->GetActiveIndex();

    if (eventType == wxEVT_PLOTCTRL_ERROR)
    {
        SetStatusText(event.GetString());
    }
    else if (eventType == wxEVT_PLOTCTRL_MOUSE_MOTION)
    {

        wxString s = wxString::Format(wxT("Mouse (%g %g) Active index %d, "),
                                      event.GetX(), event.GetY(), active_index);

        if (m_plotCtrl->IsCursorValid())
        {
            wxPoint2DDouble cursorPt = m_plotCtrl->GetCursorPoint();
            s += wxString::Format(wxT("Cursor curve %d, data index %d, point (%g %g)"),
                m_plotCtrl->GetCursorCurveIndex(), m_plotCtrl->GetCursorDataIndex(),
                cursorPt.m_x, cursorPt.m_y);
        }

        SetStatusText(s);
    }
    else if (eventType == wxEVT_PLOTCTRL_VIEW_CHANGING)
    {
        SetStatusText(wxString::Format(wxT("View Changing (%g, %g)"), event.GetX(), event.GetY()));
    }
    else if (eventType == wxEVT_PLOTCTRL_VIEW_CHANGED)
    {
        SetStatusText(wxString::Format(wxT("View Changed (%g, %g)"), event.GetX(), event.GetY()));
    }
    else
    {
        m_textCtrl->AppendText(wxString::Format(wxT("%s xy(%g %g) CurveIndex %d, IsDataCurve %d DataIndex %d, MouseFn %d\n"),
            wxPlotCtrl_GetEventName(eventType).c_str(),
            event.GetX(), event.GetY(), event.GetCurveIndex(),
            (int)event.IsDataCurve(), event.GetCurveDataIndex(),
            event.GetMouseFunction()));

        m_textCtrl->SetInsertionPointEnd();
    }
}
