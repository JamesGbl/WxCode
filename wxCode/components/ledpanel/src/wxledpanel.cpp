/***************************************************************
 * Name:      wxledpanel.cpp
 * Purpose:   Code for Class wxLEDPanel
 * Author:    Christian Gräfe (info@mcs-soft.de)
 * Created:   2007-02-28
 * Copyright: Christian Gräfe (www.mcs-soft.de)
 * License:	  wxWindows licence
 **************************************************************/
// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
    #include <wx/dcbuffer.h>
#endif

#include "wx/wxledpanel.h"

#define TIMER_SCROLL_ID 1000

DEFINE_EVENT_TYPE(wxEVT_LEDPANEL_SCROLLED_OUT)

BEGIN_EVENT_TABLE(wxLEDPanel, wxControl)
	EVT_PAINT(wxLEDPanel::OnPaint)
	EVT_ERASE_BACKGROUND(wxLEDPanel::OnEraseBackground)
	EVT_TIMER(TIMER_SCROLL_ID,wxLEDPanel::OnScrollTimer)
END_EVENT_TABLE()

wxLEDPanel::wxLEDPanel() :
	m_textalign(wxALIGN_LEFT|wxALIGN_TOP),
	m_padLeft(1),
	m_padRight(1),
	m_scrollspeed(0),
	m_scrolldirection(wxLED_SCROLL_NONE)
{
}

wxLEDPanel::wxLEDPanel(wxWindow* parent, wxWindowID id, const wxSize& pointsize,
					const wxSize& fieldsize, int padding, const wxPoint& pos,
					long style, const wxValidator& validator) :
	m_textalign(wxALIGN_LEFT|wxALIGN_TOP),
	m_padLeft(1),
	m_padRight(1),
	m_scrollspeed(0),
	m_scrolldirection(wxLED_SCROLL_NONE)
{
	Create(parent,id,pointsize,fieldsize,padding,pos,style,validator);
}

bool wxLEDPanel::Create(wxWindow* parent, wxWindowID id, const wxSize& pointsize,
					const wxSize& fieldsize, int padding, const wxPoint& pos,
					long style, const wxValidator& validator)
{
	// Daten in Members speichern
	m_pointsize=pointsize;
	m_padding=padding;
	wxSize size;
	size.SetWidth((pointsize.GetWidth()+m_padding)*fieldsize.GetWidth()+padding);
    size.SetHeight((pointsize.GetHeight()+m_padding)*fieldsize.GetHeight()+padding);

	// Control erzeugen
	if(!wxControl::Create(parent,id,pos,size,style,validator))
		return false;

	// Hintergrundfarbe standartmasig schwarz
	this->SetBackgroundColour(*wxBLACK);

	// LED-Farbe standartmäsig rot
	this->SetLEDColour(wxLED_COLOUR_RED);

	// MatrixObjekt initialisieren zum Datenspeichern
	m_field.Init(0,fieldsize.GetWidth(),fieldsize.GetHeight());

	// Nimmt standartmäßig keine Inputevents entgegen
	this->Enable(false);

	// den Timer an das Objekt binden
	m_scrollTimer.SetOwner(this,TIMER_SCROLL_ID);

	return true;
}

// Element zeichnen
void wxLEDPanel::OnPaint(wxPaintEvent &event)
{
    wxBufferedPaintDC dc(this);
    dc.SetBackground(this->GetBackgroundColour());
    dc.Clear();
    DrawField(dc);
}

void wxLEDPanel::DrawField(wxDC& dc)
{
	wxPoint point;
	wxMemoryDC mdc_on(m_bmp_led_on);
	wxMemoryDC mdc_off(m_bmp_led_off);
	int width=mdc_on.GetSize().GetWidth();
	int height=mdc_on.GetSize().GetHeight();
	char data;

	// Zähler für Zeile und Spalte
    int x=0,y=0;

    for(int i=0;i<m_field.GetLength();++i)
    {
    	// Daten des Feldes
    	data=m_field.GetDataFrom(i);

		// Koordinaten
    	point.x=x*(m_pointsize.GetWidth()+m_padding)+m_padding;
    	point.y=y*(m_pointsize.GetHeight()+m_padding)+m_padding;

    	// zeichnen
    	if(data<1 || data>7)
			dc.Blit(point.x,point.y,width,height,&mdc_off,0,0);
        else
			dc.Blit(point.x,point.y,width,height,&mdc_on,0,0);

    	// hochzählen
        ++x;
        if(x==m_field.GetWidth()) {++y; x=0;}
    }
}

void wxLEDPanel::SetLEDColour(wxLEDColour colourID)
{
	// for drawing
	wxBrush brush;
	wxPen pen;

	// colourID speichern
	m_activ_colour_id=colourID;

	// create Bitmaps for "LED on" und "LED off"
	m_bmp_led_on.Create(m_pointsize.GetWidth()+m_padding,m_pointsize.GetHeight()+m_padding);
	m_bmp_led_off.Create(m_pointsize.GetWidth()+m_padding,m_pointsize.GetHeight()+m_padding);

	// draw "LED on"
	wxMemoryDC dc_on(m_bmp_led_on);

	// Clear Background
	dc_on.SetBackground(this->GetBackgroundColour());
    dc_on.Clear();

	// complete point
    pen.SetColour(s_colour_dark[colourID-1]);
    brush.SetColour(s_colour[colourID-1]);
    dc_on.SetPen(pen);
    dc_on.SetBrush(brush);
    dc_on.DrawEllipse(wxPoint(0,0),m_pointsize);

	// left top corner in lighter colour
	pen.SetColour(s_colour_light[colourID-1]);
	dc_on.SetPen(pen);
	dc_on.DrawEllipticArc(0,0,m_pointsize.GetWidth(),m_pointsize.GetHeight(),75.0,195.0);


	// draw "LED off"
	wxMemoryDC dc_off(m_bmp_led_off);

	// cleare Background
	dc_off.SetBackground(this->GetBackgroundColour());
    dc_off.Clear();

    // complete point
    pen.SetColour(s_colour_dark[colourID-1]);
    brush.SetColour(s_colour_verydark[colourID-1]);
    dc_off.SetPen(pen);
    dc_off.SetBrush(brush);
    dc_off.DrawEllipse(wxPoint(0,0),m_pointsize);

}

wxSize wxLEDPanel::DoGetBestSize() const
{
	wxSize size;
	size.SetWidth((m_pointsize.GetWidth()+m_padding)*m_field.GetWidth()+m_padding);
    size.SetHeight((m_pointsize.GetHeight()+m_padding)*m_field.GetHeight()+m_padding);
    return size;
}

void wxLEDPanel::OnScrollTimer(wxTimerEvent& event)
{
	if(m_scrollspeed==0) return;

	// the save way
	m_scrollTimer.Stop();

	// Scroll
	switch(m_scrolldirection)
	{
		case wxLED_SCROLL_NONE: return;
		case wxLED_SCROLL_LEFT: this->ShiftLeft(); break;
		case wxLED_SCROLL_RIGHT: this->ShiftRight(); break;
		case wxLED_SCROLL_DOWN: this->ShiftDown(); break;
		case wxLED_SCROLL_UP: this->ShiftUp(); break;
		default: return;
	}

	// Repaint
	this->Refresh();

	// start timer again
	m_scrollTimer.Start(m_scrollspeed);
}

void wxLEDPanel::SetText(const wxString& text, int align)
{
	// String emtpy
	if(text.IsEmpty()) return;

	// the MO for the Text
	MatrixObject* tmp=NULL;

	// save the align
	if(align!=-1) m_textalign=align;

	// save the string
	m_text=text;

	// get the MO for the text
	if(m_textalign&wxALIGN_CENTER_HORIZONTAL)
		tmp=m_font.GetMOForText(text,wxALIGN_CENTER_HORIZONTAL);
	else if(m_textalign&wxALIGN_RIGHT)
		tmp=m_font.GetMOForText(text,wxALIGN_RIGHT);
	else tmp=m_font.GetMOForText(text);	// wxALIGN_LEFT

	// save the MO, and delete the tmp
	m_text_mo.Init(*tmp);
	delete tmp;

	// Find the place for the text
	ResetTextPos();

	// Set in field
	m_field.SetDatesAt(m_text_pos,m_text_mo);
}

void wxLEDPanel::SetTextAlign(int a)
{
	// save value
	m_textalign=a;

	// Reset the Horizontal text position
	ResetTextPos();

	// Reinit the field
	m_field.Clear();
	m_field.SetDatesAt(m_text_pos,m_text_mo);
}

void wxLEDPanel::SetTextPaddingLeft(int padLeft)
{
	// Save value
	m_padLeft=padLeft;

	// Reset the text position
	ResetTextPos();

	// Reinit the field
	m_field.Clear();
	m_field.SetDatesAt(m_text_pos,m_text_mo);
}

void wxLEDPanel::SetTextPaddingRight(int padRight)
{
	// Save the Value
	m_padRight=padRight;

	// Reset the text position
	ResetTextPos();

	// Reinit the field
	m_field.Clear();
	m_field.SetDatesAt(m_text_pos,m_text_mo);
}

void wxLEDPanel::SetScrollSpeed(int speed)
{
	// the save way
	m_scrollTimer.Stop();

	// save speed
	m_scrollspeed=speed;

	// start timer
	if(m_scrollspeed>0 && m_scrolldirection!=wxLED_SCROLL_NONE)
		m_scrollTimer.Start(speed);
}

void wxLEDPanel::SetScrollDirection(wxLEDScrollDirection d)
{
	// the save way
	m_scrollTimer.Stop();

	// save direction
	m_scrolldirection=d;

	if(m_scrollspeed>0 && m_scrolldirection!=wxLED_SCROLL_NONE)
		m_scrollTimer.Start(m_scrollspeed);
}

void wxLEDPanel::ResetTextPos()
{
	// has a text?
	if(m_text_mo.GetData()==NULL) return;

	// horizontal text pos
	if(m_scrolldirection!=wxLED_SCROLL_LEFT && m_scrolldirection!=wxLED_SCROLL_RIGHT)
	{
		if(m_textalign & wxALIGN_RIGHT)
			m_text_pos.x=m_field.GetWidth()-m_text_mo.GetWidth()-m_padRight;
		else if(m_textalign & wxALIGN_CENTER_HORIZONTAL)
			m_text_pos.x=(m_field.GetWidth()-m_text_mo.GetWidth())/2;
		else // wxALING_LEFT
			m_text_pos.x=m_padLeft;
	}
	else if(m_scrolldirection==wxLED_SCROLL_LEFT)
		m_text_pos.x=m_field.GetWidth();
	else if(m_scrolldirection==wxLED_SCROLL_RIGHT)
		m_text_pos.x=-m_text_mo.GetWidth();

	// vertical text pos
	if(m_scrolldirection!=wxLED_SCROLL_UP && m_scrolldirection!=wxLED_SCROLL_DOWN)
	{
		if(m_textalign & wxALIGN_BOTTOM)
			m_text_pos.y=m_field.GetHeight()-m_text_mo.GetHeight();
		else if(m_textalign & wxALIGN_CENTER_VERTICAL)
			m_text_pos.y=(m_field.GetHeight()-m_text_mo.GetHeight())/2;
		else // wxALIGN TOP
			m_text_pos.y=0;
	}
	else if(m_scrolldirection==wxLED_SCROLL_UP)
		m_text_pos.y=m_field.GetHeight();
	else if(m_scrolldirection==wxLED_SCROLL_DOWN)
		m_text_pos.y=-m_text_mo.GetHeight();
}

void wxLEDPanel::ShiftLeft()
{
	// new text Pos
	m_text_pos.x--;
	// out of bound
	if(m_text_pos.x+m_text_mo.GetWidth()<=0)
		m_text_pos.x=m_field.GetWidth();

	// TODO optimize with shift
	m_field.Clear();
	m_field.SetDatesAt(m_text_pos,m_text_mo);
}

void wxLEDPanel::ShiftRight()
{
	// new text Pos
	m_text_pos.x++;
	// out of bound
	if(m_text_pos.x>m_field.GetWidth())
		m_text_pos.x=-m_text_mo.GetWidth()+1;	// TODO without +1 error (in SetDatesAt??)

	// TODO optimize with shift
	m_field.Clear();
	m_field.SetDatesAt(m_text_pos,m_text_mo);
}

void wxLEDPanel::ShiftUp()
{
	// new text Pos
	m_text_pos.y--;
	// out of bound
	if(m_text_pos.y+m_text_mo.GetHeight()<=0)
		m_text_pos.y=m_field.GetHeight();

	// TODO optimize with shift
	m_field.Clear();
	m_field.SetDatesAt(m_text_pos,m_text_mo);
}

void wxLEDPanel::ShiftDown()
{
	// new text Pos
	m_text_pos.y++;
	// out of bound
	if(m_text_pos.y>=m_field.GetHeight())
		m_text_pos.y=-m_text_mo.GetHeight();

	// TODO optimize with shift
	m_field.Clear();
	m_field.SetDatesAt(m_text_pos,m_text_mo);

}

// Red, Green, Blue, Yellow, Magenta, Cyan, Grey
const wxColour wxLEDPanel::s_colour[7]=
	{	wxColour(255,0,0), wxColour(0,255,0), wxColour(0,0,255),
		wxColour(255,255,0), wxColour(255,0,255), wxColour(0,255,255),
		wxColour(128,128,128) };

const wxColour wxLEDPanel::s_colour_dark[7]=
	{	wxColour(128,0,0), wxColour(0,128,0), wxColour(0,0,128),
		wxColour(128,128,0), wxColour(128,0,128), wxColour(0,128,128),
		wxColour(64,64,64) };

const wxColour wxLEDPanel::s_colour_verydark[7]=
	{	wxColour(64,0,0), wxColour(0,64,0), wxColour(0,0,64),
		wxColour(64,64,0), wxColour(64,0,64), wxColour(0,64,64),
		wxColour(32,32,32) };

const wxColour wxLEDPanel::s_colour_light[7]=
	{	wxColour(255,128,128), wxColour(128,255,128), wxColour(128,128,255),
		wxColour(255,255,128), wxColour(255,128,255), wxColour(128,255,255),
		wxColour(192,192,192) };
