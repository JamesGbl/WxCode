/////////////////////////////////////////////////////////////////////////////
// Name:        awx/outbar.cpp
// Purpose:     Outlook like bar widget
// Author:      Joachim Buermann
// Id:          $Id: outbar.cpp,v 1.1.1.1 2004/08/13 11:45:36 jb Exp $
// Copyright:   (c) 2003,2004 Joachim Buermann
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "wx/awx/outbar.h"

BEGIN_EVENT_TABLE(wxOutBarItem, wxWindow)
    EVT_ERASE_BACKGROUND(wxOutBarItem::OnErase)
    EVT_LEFT_DOWN(wxOutBarItem::OnMouseLeftDown)
    EVT_PAINT(wxOutBarItem::OnPaint)
END_EVENT_TABLE()

#define wxSelectedColour() wxColour(0xa9,0xd1,0xff)

wxOutBarItem::wxOutBarItem(wxWindow* parent,
					  wxWindowID id,
					  const wxPoint& pos,
					  const wxSize& size,
					  char** xpmimage,
					  const wxChar* label) :
    wxWindow(parent,id,pos,wxSize(120,70))
{
    SetName(label);
    m_bitmap = new wxBitmap(120,70);
    m_icon = new wxIcon((const char **)xpmimage);
    m_font = new wxFont(10,wxDEFAULT,wxNORMAL,wxBOLD);
    m_label = label;
    m_check = 0;
};

wxOutBarItem::~wxOutBarItem()
{
    delete m_icon;
    delete m_bitmap;
    delete m_font;
};

void wxOutBarItem::DrawItem(wxDC& dc)
{
    wxCoord w,h;
    dc.DrawIcon(*m_icon,28,2);
    dc.SetFont(*m_font);
    dc.GetTextExtent(m_label,&w,&h);
    int dx = (104 - w) >> 1;
    if(dx < 0) dx = 0;
    int dy = (20 - h) >> 1;
    if(dy < 0) dy = 0;
    dc.DrawText(m_label,dx,50+dy);
};

void wxOutBarItem::DrawOnBitmap()
{
    wxSize s = GetClientSize();
    if((m_bitmap->GetWidth() != s.GetWidth()) || 
	  (m_bitmap->GetHeight() != s.GetHeight())) {
	   m_bitmap->Create(s.x,s.y);
    }
    wxMemoryDC dc;
    dc.SelectObject(*m_bitmap);

    if(m_check) dc.SetBackground(wxBrush(wxSelectedColour(),wxSOLID));
    else dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    DrawItem(dc);

    dc.SelectObject(wxNullBitmap);
};

void wxOutBarItem::OnErase(wxEraseEvent &event)
{
    Redraw();
    event.Skip();
};

void wxOutBarItem::OnMouseLeftDown(wxMouseEvent& WXUNUSED(event))
{
    if(!m_check) {
	   wxCommandEvent ev(wxEVT_COMMAND_CHOICE_SELECTED,GetId());
	   GetParent()->ProcessEvent(ev);
    }
};

void wxOutBarItem::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    dc.DrawBitmap(*m_bitmap,0,0,false);
};

void wxOutBarItem::Redraw()
{
    wxClientDC dc(this);
    DrawOnBitmap();
    dc.DrawBitmap(*m_bitmap,0,0,false);
};

BEGIN_EVENT_TABLE(wxOutBar, wxScrolledWindow)
    EVT_CHOICE(-1,wxOutBar::OnChoice)
END_EVENT_TABLE()

wxOutBar::wxOutBar(wxWindow* parent,
			    wxWindowID id,
			    const wxPoint& pos,
			    const wxSize& size) :
    wxScrolledWindow(parent,id,pos,wxSize(120,-1),wxVSCROLL|wxSUNKEN_BORDER)
{
    SetBackgroundColour(*wxWHITE);
//    SetName(wxString("wxOutBar"));
    m_sizer = new wxBoxSizer(wxVERTICAL);
    m_items = 0;
    SetAutoLayout(true);
    SetSizer(m_sizer);
    m_selected = NULL;
};

wxOutBar::~wxOutBar()
{
};

void wxOutBar::Add(wxOutBarItem* item)
{
    // uncheck the last item (if it exists)
    if(m_selected) m_selected->Check(false);
    m_sizer->Add(item,0,wxGROW|wxALL,0);
    m_sizer->Layout();
    SetScrollbars(0,1,1,++m_items*70);
    m_selected = item;
    // the new added item will be selected automatically
    m_selected->Check(true);
};

void wxOutBar::OnChoice(wxCommandEvent& event)
{
    wxOutBarItem* item;
    item = (wxOutBarItem*)FindWindow(event.GetId());
    if(item) {
	   if(m_selected) {
		  m_selected->Check(false);
		  m_selected->Refresh();
	   }
	   item->Check(true);
	   item->Refresh();
	   m_selected = item;
	   event.Skip();
    }
};

bool wxOutBar::SetSelection(const wxChar* byName)
{
    wxOutBarItem* item;
    item = ((wxOutBarItem*)FindWindowByName(byName));
    if(item) {
	   if(m_selected) {
		  m_selected->Check(false);
		  m_selected->Refresh();
	   }
	   item->Check(true);
	   item->Refresh();
	   m_selected = item;
	   return true;
    }
    return false;
};
