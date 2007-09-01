////////////////////////////////////////////////////////////////////////////
// Name:        awx/toolbar.cpp
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: toolbar.cpp,v 1.2 2004/11/05 10:48:58 jb Exp $
// Copyright:   (c) 2004 Joachim Buermann
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "wx/awx-0.3/toolbar.h"

const static wxColour kdetheme[14] =
{
    wxColour(0xf3,0xf7,0xf9),
    wxColour(0xf3,0xf7,0xf9),
    wxColour(0xee,0xf3,0xf7),
    wxColour(0xee,0xf3,0xf7),
    wxColour(0xea,0xf0,0xf4),
    wxColour(0xea,0xf0,0xf4),
    wxColour(0xe6,0xec,0xf1),
    wxColour(0xe6,0xec,0xf1),
    wxColour(0xe2,0xe9,0xef),
    wxColour(0xe2,0xe9,0xef),
    wxColour(0xdd,0xe5,0xec),
    wxColour(0xdd,0xe5,0xec),
    wxColour(0xd9,0xe2,0xea),
    wxColour(0xd9,0xe2,0xea),
};

static void DrawKDETheme(wxDC& dc,wxSize size)
{
    wxPen pen;
    pen.SetStyle(wxSOLID);
    wxBrush brush(kdetheme[13],wxSOLID);
    dc.SetBackground(brush);
    dc.Clear();
    for(int i=0;i<14;i++) {
	   pen.SetColour(kdetheme[i]);
	   dc.SetPen(pen);
	   dc.DrawLine(0,i,size.GetWidth()-1,i);
	   dc.DrawLine(0,size.GetHeight()-1-i,
				size.GetWidth()-1,size.GetHeight()-1-i);
    }
};

IMPLEMENT_DYNAMIC_CLASS(awxToolbar, wxWindow)
BEGIN_EVENT_TABLE(awxToolbar, wxWindow)
    EVT_PAINT(awxToolbar::OnPaint)
    EVT_SIZE(awxToolbar::OnSizeEvent)
    EVT_ERASE_BACKGROUND(awxToolbar::OnEraseBackground)
END_EVENT_TABLE()

awxToolbar::awxToolbar(wxWindow* parent,
		   wxWindowID id) :
    wxWindow(parent,id,
		   wxDefaultPosition,wxSize(20,20),
		   wxNO_FULL_REPAINT_ON_RESIZE)
{
    InitBase();
};

awxToolbar::~awxToolbar()
{
    delete m_bitmap;
    delete m_sizer;
};

void awxToolbar::Add(wxWindow* widget)
{
    static wxCoord w = 0;
    wxSize widgetSize = widget->GetSize();
    w += widgetSize.x;
    if(widgetSize.y > m_size.y) {
	   // for a better visibility of the upper and lower margin, 
	   // give two pixel more...
	   SetSize(m_size.x,widgetSize.y+2);
    }
    if(w > m_size.x) {
	   SetSize(w+4,m_size.y);
    }
    m_sizer->SetDimension(2,0,m_size.GetWidth()-4,m_size.y);
    m_sizer->Add(widget,0,wxALIGN_CENTER | wxALL,m_extraSpace);

    m_sizer->Layout();
};

//
// Only called, if the dimension was changed
//
void awxToolbar::DrawOnBitmap()
{
    if((m_bitmap->GetWidth() != m_size.x) || 
	  (m_bitmap->GetHeight() != m_size.y)) {
	   m_bitmap->Create(m_size.x,m_size.y);
	   m_painted = true;
    }
    wxMemoryDC dc;
    dc.SelectObject(*m_bitmap);

    if(!m_theme) {
	   wxBrush brush(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE),
				  wxSOLID);
	   dc.SetBackground(brush);
	   dc.Clear();
    }
    else {
	   DrawKDETheme(dc,m_size);
    }
    dc.SelectObject(wxNullBitmap);
};

void awxToolbar::EnableItem(int id,bool enable)
{
    wxWindow* item = FindWindow(id);
    if(item) item->Enable(enable);
};

void awxToolbar::InitBase()
{
    m_size = wxSize(20,20);
    m_sizer = new wxBoxSizer( wxHORIZONTAL );
    m_sizer->SetDimension(2,1,m_size.GetWidth()-4,m_size.y-2);
    m_bitmap = new wxBitmap(m_size.x,m_size.y);
    m_painted = false;
    m_theme = false;
    m_extraSpace = 0;
};

void awxToolbar::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
    int h,w;
    GetClientSize(&w,&h);
    m_size.Set(w,h);
#if __WXMSW__
    Redraw();
#endif
};

wxBitmap awxToolbar::GetBackgroundRegion(int id)
{
    wxWindow* win = FindWindow(id);
    if(m_painted && win) {
	   int x,y,w,h;
	   win->GetSize(&w,&h);
	   win->GetPosition(&x,&y);
	   // background clipping
	   wxRect r = GetRect();
	   int dx = (r.x + r.width) - x;
	   if(dx < w) w = dx;
	   if(w > 0) return m_bitmap->GetSubBitmap(wxRect(x,y,w,h));
    }
    return wxNullBitmap;
};

void awxToolbar::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    dc.DrawBitmap(*m_bitmap,0,0,false);
};

/*
  On win32 OnSizeEevnt is called only if the dimension has changed,
  NOT on Minimize (iconizing) and Maximize. Also NOT on the first
  start of the program!!!
  Within wxGtk (gtk2) OnSizeEvent is called also on start.
*/
void awxToolbar::OnSizeEvent(wxSizeEvent& event)
{
    // the given wxSizeEvent tells the current dimension during
    // resizing
    m_size = event.GetSize();
#if __WXMSW__
    DrawOnBitmap();
#else
    Redraw();
    Refresh();
#endif
};

void awxToolbar::Redraw()
{
    wxClientDC dc(this);
    DrawOnBitmap();
    dc.DrawBitmap(*m_bitmap,0,0,false);
};
