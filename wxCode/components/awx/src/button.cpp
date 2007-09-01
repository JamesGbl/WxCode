/////////////////////////////////////////////////////////////////////////////
// Name:        awx/button.cpp
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: button.cpp,v 1.2 2004/11/05 10:48:46 jb Exp $
// Copyright:   (c) 2003 Joachim Buermann
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/image.h>
#include <wx/settings.h>

#include "wx/awx-0.3/button.h"
#include "wx/awx-0.3/toolbar.h"

IMPLEMENT_DYNAMIC_CLASS(awxButton, wxWindow)

BEGIN_EVENT_TABLE(awxButton, wxWindow)
    EVT_MOUSE_EVENTS(awxButton::OnMouseEvent)
    EVT_PAINT(awxButton::OnPaint)
    EVT_SIZE(awxButton::OnSizeEvent)
    EVT_ERASE_BACKGROUND(awxButton::OnEraseBackground)
END_EVENT_TABLE()

#define wxMB_COLOR_OVER wxColour(0xE8,0xE8,0xE8)
#define wxMB_COLOR_BG wxColour(0xD7,0xD7,0xD7)
#define wxMB_TEXT_MARGIN 8

awxButton::awxButton(wxWindow * parent, wxWindowID id,
				 const wxPoint & pos,
				 const wxSize & size,
				 char **upXPM,
				 char **overXPM,
				 char **downXPM,
				 char **disXPM) :
    wxWindow(parent, id, pos, size)
{
    if(size == wxDefaultSize) {
	   m_width = 32;
	   m_height = 32;
    }
    else {
	   m_width = size.x;
	   m_height = size.y;
    }

    m_state = State_ButtonUp;
    m_enabled = true;
    m_painted = false;
    m_theme = false;

    memset(m_icons,0,sizeof(m_icons));

    if(upXPM) m_icons[0] = new wxIcon((const char **)upXPM);
    // if the over image is a NULL pointer, use a upper frame instead
    if(overXPM) m_icons[1] = new wxIcon((const char **)overXPM);
    // if the down image is a NULL pointer, use a lower frame instead
    if(downXPM) m_icons[2] = new wxIcon((const char **)downXPM);
    // without a disable image, the up image will be used
    if(disXPM) m_icons[3] = new wxIcon((const char **)disXPM);
    else {
	   if(upXPM) m_icons[3] = new wxIcon((const char **)upXPM);
    }
    // button font text
#ifdef __WIN32__
    m_font = new wxFont(8,wxDEFAULT,wxNORMAL,wxBOLD);
#else
    m_font = new wxFont(10,wxDEFAULT,wxNORMAL,wxBOLD);
#endif    
    m_laststate = State_ButtonNew;

    // use the up icon dimensions for the image
    int x = 0;
    if(m_icons[0]) x = m_icons[0]->GetWidth();
    if(x <= m_width) m_dx = (m_width - x) >> 1;
    else m_dx = 0;
    m_dy = 0;

    SetSize(m_width,m_height);
    m_bitmap = new wxBitmap(m_width,m_height);

    awxToolbar* mtb = wxDynamicCast(GetParent(),awxToolbar);
    if(mtb) {
	   EnableTheme(mtb->HasTheme());
    }
};

void awxButton::SetText(const wxChar* text)
{
    int w,h,x = 0;
    wxClientDC dc(this);
    dc.SetFont(*m_font);
    m_text = text;
    dc.GetTextExtent(text,&w,&h);
    if(w > m_width) m_width = w + wxMB_TEXT_MARGIN;
    m_height += m_font->GetPointSize()+2;
    SetSize(m_width,m_height);

    if(m_icons[0]) x = m_icons[0]->GetWidth();
    if(x <= m_width) m_dx = (m_width - x) >> 1;
    else m_dx = 0;

    m_bitmap->Create(m_width,m_height);
    Redraw();
};

awxButton::~awxButton()
{
    for(int i=0;i<4;i++) {
	   if(m_icons[i]) delete m_icons[i];
    }
    delete m_bitmap;
    delete m_font;
};

void awxButton::DrawBorder(wxDC& dc, BorderType border)
{
    if(!m_theme) {
	   wxColour bg = GetParent()->GetBackgroundColour();
	   wxBrush brush_over(wxMB_COLOR_OVER,wxSOLID);
	   dc.SetBrush(brush_over);
	   dc.SetPen(*wxTRANSPARENT_PEN);
	   dc.DrawRectangle(0,0,m_width,m_height);
    }
    wxPen light(wxColour(0xFF,0xFF,0xFF),1,wxSOLID);
    wxPen dark(wxColour(0x80,0x80,0x80),1,wxSOLID);
    wxPen corner(wxMB_COLOR_BG,1,wxSOLID);

    switch(border) {
    case Border_High:
	   dc.SetPen(light);
	   dc.DrawLine(1,0,m_width-2,0);
	   dc.DrawLine(0,1,0,m_height-2);
	   dc.SetPen(dark);
	   dc.DrawLine(0,m_height-1,m_width-1,m_height-1);
	   dc.DrawLine(m_width-1,0,m_width-1,m_height-1);
	   break;
    case Border_Sunken:
	   dc.SetPen(dark);
	   dc.DrawLine(1,0,m_width-2,0);
	   dc.DrawLine(0,1,0,m_height-2);
	   dc.SetPen(light);
	   dc.DrawLine(1,m_height-1,m_width-2,m_height-1);
	   dc.DrawLine(m_width-1,1,m_width-1,m_height-2);
	   break;
    default:
	   break;
    }
};

void awxButton::DrawOnBitmap()
{
    wxCoord dx = 0;
    wxCoord dy = 0;
    wxCoord w;
    wxCoord h;

    wxMemoryDC dc;
    dc.SelectObject(*m_bitmap);
    // use the system background colour for buttons (wxSYS_COLOUR_BTNFACE)
    // if there is no theme enabled
    if(!m_theme) {
	   wxBrush brush(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE),
				  wxSOLID);
	   wxColour bg = m_parent->GetBackgroundColour();
	   dc.SetBackground(brush);
	   dc.Clear();
    }
    else {
	   awxToolbar* mtb = wxDynamicCast(GetParent(),awxToolbar);
	   if(mtb) {
		  wxBitmap bmp = mtb->GetBackgroundRegion(GetId());
		  if(bmp != wxNullBitmap && bmp.Ok()) {
			 dc.DrawBitmap(bmp,0,0,false);
		  }
	   }
    }
    dc.SetFont(*m_font);

    switch(m_state) {
    case State_ButtonUp:
	   if(m_icons[m_state]) dc.DrawIcon(*m_icons[m_state],m_dx,m_dy);
	   break;
    case State_ButtonOver:
	   if(m_icons[m_state]) dc.DrawIcon(*m_icons[m_state],m_dx,m_dy);
	   else {
		  DrawBorder(dc,Border_High);
		  if(m_icons[State_ButtonUp]) {
			 dc.DrawIcon(*m_icons[State_ButtonUp],m_dx,m_dy);
		  }
	   }
	   break;
    case State_ButtonDown:
	   if(m_icons[m_state]) dc.DrawIcon(*m_icons[m_state],m_dx,m_dy);
	   else {
		  DrawBorder(dc,Border_Sunken);
		  if(m_icons[State_ButtonUp]) {
			 dc.DrawIcon(*m_icons[State_ButtonUp],m_dx+1,m_dy+1);
		  }
	   }
	   dx = dy = 1;
	   break;
    case State_ButtonDis:
	   if(m_icons[m_state]) dc.DrawIcon(*m_icons[m_state],m_dx,m_dy);
	   else {
	   }
	   break;
    default:
	   dc.SelectObject(wxNullBitmap);
	   return;
    }

    dc.GetTextExtent(m_text,&w,&h);
    if(w > m_width) w = m_width;
    if(h > m_height) h = m_height;

    if(!m_text.IsEmpty()) {
	   if(m_enabled) {
		  dc.SetTextForeground(wxColour(0,0,0));
		  dc.DrawText(m_text,((m_width-w)>>1)+1+dx,m_height-h+dy);
	   }
	   else {
		  dc.SetTextForeground(wxColour(255,255,255));
		  dc.DrawText(m_text,((m_width-w)>>1)+1+dx,m_height-h+dy);
		  dc.SetTextForeground(wxColour(128,128,128));
		  dc.DrawText(m_text,((m_width-w)>>1)+dy,m_height-h-1+dy);
	   }
    }

    dc.SelectObject(wxNullBitmap);
};

void awxButton::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    if(!m_painted) {
	   DrawOnBitmap();
	   m_painted = true;
    }
    dc.DrawBitmap(*m_bitmap,0,0,false);
};

void awxButton::OnEraseBackground(wxEraseEvent& event)
{
#ifdef __WXMSW__
    Redraw();
    event.Skip();
#else
    // at this time, the background image isn't repaint by the toolbar,
    // so defer the redraw for later
    wxSizeEvent ev(GetSize(),GetId());
    AddPendingEvent(ev);
    event.Skip();
#endif
};

void awxButton::OnMouseEvent(wxMouseEvent& event)
{
    if(!m_enabled) return;

    wxCommandEvent ev(wxEVT_COMMAND_MENU_SELECTED,GetId());
    ev.SetEventObject(GetParent());

    if(event.ButtonDown()) {
	   m_state = State_ButtonDown;
	   Redraw();
    }
    else if(event.ButtonUp()) {
	   m_state = State_ButtonOver;
	   wxPoint pos = event.GetPosition();
	   if((pos.x < GetSize().GetWidth()) && 
		 (pos.y < GetSize().GetHeight())) {
		  GetParent()->ProcessEvent(ev);
	   }
	   Redraw();
    }
    else if (event.Entering()) {
	   m_state = State_ButtonOver;
	   Redraw();
	   return;
    }
    else if(event.Leaving()) {
	   if((m_state == State_ButtonOver) || (m_state == State_ButtonDown)) {
		  m_state = State_ButtonUp;
		  Redraw();
		  return;
	   }
    }
};

void awxButton::OnSizeEvent(wxSizeEvent &event)
{
    Redraw();
    event.Skip();
};

void awxButton::Enable()
{
    if(!m_enabled) {
	   m_state = m_laststate;
	   m_enabled = true;
	   Redraw();
    }
};

void awxButton::Disable()
{
    if(m_enabled) {
	   m_laststate = m_state;
	   m_enabled = false;
	   m_state = State_ButtonDis;
	   Redraw();
    }
};

bool awxButton::Press()
{
    if(m_state != State_ButtonDown) {
	   m_state = State_ButtonDown;
	   Redraw();
	   return true;
    }
    return false;
};

void awxButton::Redraw()
{
    if(m_painted) {    
	   DrawOnBitmap();
	   wxClientDC dc(this);
	   dc.DrawBitmap(*m_bitmap,0,0,false);
    }
};

bool awxButton::Release()
{
    if(m_state == State_ButtonDown) {
	   m_state = State_ButtonUp;
	   Redraw();
	   return true;
    }
    return false;
};

BEGIN_EVENT_TABLE(awxCheckButton, awxButton)
    EVT_MOUSE_EVENTS(awxCheckButton::OnMouseEvent)
END_EVENT_TABLE()

void awxCheckButton::OnMouseEvent(wxMouseEvent & event)
{
    if(!m_enabled) return;

    wxCommandEvent ev(wxEVT_COMMAND_MENU_SELECTED,GetId());
    ev.SetEventObject(GetParent());

    if(event.ButtonDown()) {
	   m_state = State_ButtonDown;
	   Redraw();
    }
    else if(event.ButtonUp()) {
	   wxPoint pos = event.GetPosition();
	   if((pos.x < GetSize().GetWidth()) && 
		 (pos.y < GetSize().GetHeight())) {
		  // mouse up must inside button range, toggle internal state
		  m_snapin ^= true;
		  if(m_snapin) m_state = State_ButtonDown;
		  else m_state = State_ButtonOver;
		  // set it in the event
		  ev.SetInt(m_snapin);
		  // and send it to the parent
		  GetParent()->ProcessEvent(ev);
		  Redraw();
	   }
    }
    else if (event.Entering()) {
	   // do not redraw ButtonOver state if button is pressed
	   if(m_snapin) return;
	   m_state = State_ButtonOver;
	   Redraw();
	   return;
    }
    else if(event.Leaving()) {
	   // do not redraw ButtonOver state if button is pressed
	   if(m_snapin) return;
	   if((m_state == State_ButtonOver) || (m_state == State_ButtonDown)) {
		  m_state = State_ButtonUp;
		  Redraw();
		  return;
	   }
    }
};

bool awxCheckButton::Press()
{
    bool result = false;
    if((result = awxButton::Press()) == true) {
	   m_snapin = true;
    }
    return result;
};

bool awxCheckButton::Release()
{
    bool result = false;
    if((result = awxButton::Release()) == true) {
	   m_snapin = false;
    }
    return result;
};

BEGIN_EVENT_TABLE(awxRadioButton, awxButton)
    EVT_MOUSE_EVENTS(awxRadioButton::OnMouseEvent)
END_EVENT_TABLE()

awxRadioButton::awxRadioButton(wxWindow * parent, wxWindowID id,
						 const wxPoint & pos,
						 const wxSize & size,
						 char **upXPM,char **overXPM,
						 char **downXPM,char **disXPM,
						 awxRadioButtonBox* selectBox) :
    awxCheckButton(parent,id,pos,size,upXPM,overXPM,downXPM,disXPM)
{
    m_box = selectBox; 
    if(m_box) m_box->Add(this);
};

void awxRadioButton::OnMouseEvent(wxMouseEvent & event)
{
    if(!m_enabled) return;
    if(m_snapin) return;

    wxCommandEvent ev(wxEVT_COMMAND_MENU_SELECTED,GetId());
    ev.SetEventObject(GetParent());

    if(event.ButtonDown()) {
	   m_state = State_ButtonDown;
	   Redraw();
    }
    else if(event.ButtonUp()) {
	   wxPoint pos = event.GetPosition();
	   if((pos.x < GetSize().GetWidth()) && 
		 (pos.y < GetSize().GetHeight())) {
		  // mouse up must inside button range, toggle internal state
		  m_snapin ^= true;

		  if(m_snapin) {
			 if(m_box) m_box->ReleaseAll();
			 m_snapin = true;
		  }
		  if(m_snapin) m_state = State_ButtonDown;
		  else m_state = State_ButtonOver;

		  // send it to the parent
		  GetParent()->ProcessEvent(ev);
		  Redraw();
	   }
    }
    else if (event.Entering()) {
	   m_state = State_ButtonOver;
	   Redraw();
	   return;
    }
    else if(event.Leaving()) {
	   if((m_state == State_ButtonOver) || (m_state == State_ButtonDown)) {
		  m_state = State_ButtonUp;
		  Redraw();
		  return;
	   }
    }
};

BEGIN_EVENT_TABLE(awxSeparator, awxButton)
    EVT_ERASE_BACKGROUND(awxSeparator::OnEraseBackground)
END_EVENT_TABLE()

void awxSeparator::DrawOnBitmap()
{
    wxMemoryDC dc;
    dc.SelectObject(*m_bitmap);

    if(!m_theme) {
	   wxBrush brush(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE),
				  wxSOLID);
	   dc.SetBackground(brush);
	   dc.Clear();
    }
    else {
	   awxToolbar* mtb = wxDynamicCast(GetParent(),awxToolbar);
	   if(mtb) {
		  wxBitmap bmp = mtb->GetBackgroundRegion(GetId());
		  if(bmp.Ok()) {
			 dc.DrawBitmap(bmp,0,0,false);
		  }
	   }
    }
    dc.SetPen(*wxLIGHT_GREY_PEN);
    // separator is a vertical relief line
    dc.DrawLine(m_width>>1,1,m_width>>1,m_height-3);
    dc.SetPen(*wxMEDIUM_GREY_PEN);
    dc.DrawLine((m_width>>1)+1,2,(m_width>>1)+1,m_height-2);
};
