/***************************************************************
 * Name:      ownerDrawnSearchCtrl.cpp
 * Purpose:   Implements class for search control with custom background and text color and image
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "ownerDrawnSearchCtrl.h"

BEGIN_EVENT_TABLE(ownerDrawnSearchCtrl, wxPanel)
    EVT_SIZE(ownerDrawnSearchCtrl::onResize)
    EVT_PAINT(ownerDrawnSearchCtrl::onPaint)
    EVT_ERASE_BACKGROUND(ownerDrawnSearchCtrl::onEraseBackground)
END_EVENT_TABLE()

ownerDrawnSearchCtrl::ownerDrawnSearchCtrl(wxWindow *parent, wxWindowID id, wxColour textColor, wxColour backColour, wxString filename) : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxCLIP_CHILDREN)
{
    m_backColor = backColour;
    m_hasImage = false;

    wxFileName fn(filename, wxPATH_UNIX);
    if(!filename.IsEmpty())
    {
        if(fn.FileExists())
        {
            if(m_img.LoadFile(fn.GetFullPath(), wxBITMAP_TYPE_ANY))
            {
                m_imgWidth = m_img.GetWidth();
                m_imgHeight = m_img.GetHeight();
                m_hasImage = true;
            }
            else wxLogWarning(wxT("File '%s' has unknown format"), fn.GetFullPath().c_str());

        } else { wxLogWarning(wxT("File '%s' not found"), fn.GetFullPath().c_str()); m_img = wxNullBitmap; }
    }

    m_pTextCtrl = new wxTextCtrl(this, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxBORDER_NONE);
    m_pTextCtrl->SetBackgroundColour(m_backColor);
    m_pTextCtrl->SetForegroundColour(textColor);
}

ownerDrawnSearchCtrl::~ownerDrawnSearchCtrl()
{
    ;
}

void ownerDrawnSearchCtrl::onResize(wxSizeEvent &event)
{
    wxRect rect = GetClientRect();

    if(m_hasImage) { rect.x = m_imgWidth + 8; rect.y = 3; rect.width -= m_imgWidth + 10; rect.height -= 4; }
    else { rect.x = 2; rect.y = 3; rect.width -= 4; rect.height -= 4; }

    m_pTextCtrl->SetSize(rect);

    Refresh();
    event.Skip();
}

wxSize ownerDrawnSearchCtrl::DoGetBestSize() const
{
	int height;

	if(m_hasImage) height = m_imgHeight + 4;
	else height = 20;

	return wxSize(-1, height);
}

void ownerDrawnSearchCtrl::onEraseBackground(wxEraseEvent &event)
{
    ;
}

wxString ownerDrawnSearchCtrl::GetValue()
{
    return m_pTextCtrl->GetValue();
}

void ownerDrawnSearchCtrl::SetValue(wxString value)
{
    m_pTextCtrl->SetValue(value);
}

void ownerDrawnSearchCtrl::Focus()
{
    m_pTextCtrl->SetFocus();
}

void ownerDrawnSearchCtrl::onPaint(wxPaintEvent &event)
{
    wxRect rect = GetClientRect();
    wxPaintDC dc(this);
    wxBrush brush(m_backColor);

    dc.SetBackground(brush);
    dc.Clear();
    dc.SetBackground(wxNullBrush);

    if(m_hasImage) dc.DrawBitmap(m_img, 2, rect.height / 2 - m_imgHeight / 2, true);
}
