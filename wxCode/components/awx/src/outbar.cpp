/////////////////////////////////////////////////////////////////////////////
// Name:        awx/outbar.cpp
// Purpose:     Outlook like bar widget
// Author:      Joachim Buermann
// Id:          $Id: outbar.cpp,v 1.1.1.1 2004/08/13 11:45:36 jb Exp $
// Copyright:   (c) 2003,2004 Joachim Buermann
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "wx/awx-0.3/outbar.h"
#include "wx/awx-0.3/grayimage.h"

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
					  const wxString& label) :
    wxWindow(parent,id,pos,wxSize(32,32))
{
    SetName(label);
    m_bitmap = new wxBitmap(32,32);

    wxGrayImage* gray = new wxGrayImage(wxBitmap((const char **)xpmimage));
    gray->SetMaskColour(1, 1, 1);
    // a gray icon for the disabled state
    m_icon[0] = new wxBitmap(gray);
    // the enabled state shows the normal icon
    m_icon[1] = new wxBitmap((const char **)xpmimage);

    m_font = new wxFont(10,wxDEFAULT,wxNORMAL,wxBOLD);
    m_label = label;
    m_check = 0;

    int labelHeight = 0;

    if( ! label.IsEmpty() ) {
	   labelHeight = m_font->GetPointSize() << 1;
    }
    SetMinSize( wxSize( m_icon[1]->GetWidth(), 
				    m_icon[1]->GetHeight() + labelHeight + 2) );
};

wxOutBarItem::~wxOutBarItem()
{
    delete m_bitmap;
    delete m_font;
    delete m_icon[0];
    delete m_icon[1];
};

void wxOutBarItem::DrawItem(wxDC& dc)
{
    wxCoord w,h;
    
    dc.DrawBitmap(*m_icon[IsEnabled()],
			   (GetSize().GetWidth() - m_icon[1]->GetWidth()) >> 1, 2,true);
    dc.SetFont(*m_font);
    dc.GetTextExtent(m_label,&w,&h);
    int dx = (GetSize().GetWidth() - w) >> 1;
    if(dx < 0) dx = 0;

    dc.DrawText(m_label,dx,GetSize().GetHeight() - 
			 (m_font->GetPointSize() << 1));

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
    wxCoord w,h;
    wxMemoryDC dc;
    wxBitmap tmpBitmap(100,100);
    dc.SelectObject(tmpBitmap);

    dc.SetFont(item->GetFont());
    dc.GetTextExtent(item->GetLabel(),&w,&h);
    w += 8;

    wxSize itemSize = item->GetSize();

    if ( itemSize.GetWidth() > m_itemSize.GetWidth() ) {
	   m_itemSize.SetWidth(itemSize.GetWidth() + GetMargins().GetWidth() );
    }
    if ( w > m_itemSize.GetWidth() ) { 
	   m_itemSize.SetWidth( w );
    }
    if ( itemSize.GetHeight() > m_itemSize.GetHeight() ) {
	   m_itemSize.SetHeight(itemSize.GetHeight() );
    }
    m_sizer->SetMinSize(m_itemSize);

    // uncheck the last item (if it exists)
    if(m_selected) m_selected->Check(false);
    m_sizer->Add(item,0,wxGROW|wxALL,0);
    m_sizer->Layout();
    SetScrollbars(0,1,1,++m_items * m_itemSize.GetHeight());
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
