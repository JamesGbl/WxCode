/***************************************************************
 * Name:      ownerDrawnButton.cpp
 * Purpose:   Implements class for button with custom background colors and image
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "ownerDrawnButton.h"

BEGIN_EVENT_TABLE(ownerDrawnButton, ownerDrawnWindow)
	EVT_LEFT_DOWN(ownerDrawnButton::OnLeftClick)
	EVT_LEFT_UP(ownerDrawnButton::OnLeftRelease)
    EVT_ENTER_WINDOW(ownerDrawnButton::OnEnterWindow)
    EVT_LEAVE_WINDOW(ownerDrawnButton::OnLeaveWindow)
END_EVENT_TABLE()

ownerDrawnButton::ownerDrawnButton(wxWindow *parent, wxWindowID id, styleButton style, bool toggle) : ownerDrawnWindow(parent, id, style.bgStyle[BUTTON_STATE_NORMAL], wxDefaultSize)
{
    int b;

    m_style = style;
    m_selected = false;
    m_hasImage = false;
    m_toggle = toggle;
    m_currentState = BUTTON_STATE_NORMAL;

    if(style.bold[BUTTON_STATE_NORMAL]) b = wxFONTWEIGHT_BOLD;
    else b = wxFONTWEIGHT_NORMAL;
    m_font[BUTTON_STATE_NORMAL] = wxFont(style.fontSize, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, b);

    if(style.bold[BUTTON_STATE_HOVER]) b = wxFONTWEIGHT_BOLD;
    else b = wxFONTWEIGHT_NORMAL;
    m_font[BUTTON_STATE_HOVER] = wxFont(style.fontSize, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, b);

    if(style.bold[BUTTON_STATE_SELECTED]) b = wxFONTWEIGHT_BOLD;
    else b = wxFONTWEIGHT_NORMAL;
    m_font[BUTTON_STATE_SELECTED] = wxFont(style.fontSize, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, b);

    SetFont(m_font[BUTTON_STATE_SELECTED]);

    wxFileName filename(style.imgFile, wxPATH_UNIX);
    if(!style.imgFile.IsEmpty())
    {
        if(filename.FileExists())
        {
            if(m_img.LoadFile(filename.GetFullPath(), wxBITMAP_TYPE_ANY))
            {
                m_imgWidth = m_img.GetWidth();
                m_imgHeight = m_img.GetHeight();
                m_hasImage = true;
            }
            else wxLogWarning(wxT("File '%s' has unknown format"), filename.GetFullPath().c_str());

        } else wxLogWarning(wxT("File '%s' not found"), filename.GetFullPath().c_str());
    }
}

ownerDrawnButton::~ownerDrawnButton()
{
    ;
}

void ownerDrawnButton::OnLeftClick(wxMouseEvent &event)
{
    if(!m_selected || !m_toggle)
    {
        m_currentState = BUTTON_STATE_SELECTED;
        changeStyle(m_style.bgStyle[BUTTON_STATE_SELECTED]);
    }
}

void ownerDrawnButton::OnLeftRelease(wxMouseEvent &event)
{
    if(m_toggle)
    {
        m_selected = true;
        m_currentState = BUTTON_STATE_SELECTED;
        changeStyle(m_style.bgStyle[BUTTON_STATE_SELECTED]);
    }
    else {
        m_currentState = BUTTON_STATE_HOVER;
        changeStyle(m_style.bgStyle[BUTTON_STATE_HOVER]);
    }

    wxCommandEvent ev(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
    ev.SetEventObject(this);
    GetEventHandler()->ProcessEvent(ev);
}

void ownerDrawnButton::OnEnterWindow(wxMouseEvent &event)
{
    if(!m_selected || !m_toggle)
    {
        m_currentState = BUTTON_STATE_HOVER;
        changeStyle(m_style.bgStyle[BUTTON_STATE_HOVER]);
    }
}

void ownerDrawnButton::OnLeaveWindow(wxMouseEvent &event)
{
    if(!m_selected || !m_toggle)
    {
        m_currentState = BUTTON_STATE_NORMAL;
        changeStyle(m_style.bgStyle[BUTTON_STATE_NORMAL]);
    }
}

void ownerDrawnButton::resetStyle()
{
    m_currentState = BUTTON_STATE_NORMAL;
    changeStyle(m_style.bgStyle[BUTTON_STATE_NORMAL]);

    m_selected = false;
}

void ownerDrawnButton::select()
{
    m_currentState = BUTTON_STATE_SELECTED;
    changeStyle(m_style.bgStyle[BUTTON_STATE_SELECTED]);

    m_selected = true;

    wxCommandEvent ev(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
    ev.SetEventObject(this);
    GetEventHandler()->ProcessEvent(ev);
}

wxSize ownerDrawnButton::DoGetBestSize() const
{
	int width = 100, height = 20, textX, textY, maxX, maxY;

    GetTextExtent(m_style.title, &textX, &textY);

    if(textX>m_imgWidth) maxX = textX;
    else maxX = m_imgWidth;

    if(textY>m_imgHeight) maxY = textY;
    else maxY = m_imgHeight;

	if(m_hasImage)
	{
        if(m_style.imgPosition==wxTOP || m_style.imgPosition==wxBOTTOM)
        {
            width = maxX + 16;
            height = textY + m_imgHeight + 6;
        }
        else {
            width = textX + m_imgWidth + 18;
            height = maxY + 4;
        }

	} else {
	    width = textX + 16;
	    height = textY + 4;
	}

	return wxSize(width, height);
}

void ownerDrawnButton::drawCustom(wxPaintDC *dc)
{
    wxRect rect = GetClientRect();
    int imgPosX, imgPosY, textAlign = wxALIGN_LEFT;

    dc->SetFont(m_font[m_currentState]);
    dc->SetTextForeground(m_style.textColor[m_currentState]);

    if(m_hasImage)
    {
        switch(m_style.imgPosition)
        {
            case wxTOP:
                imgPosX = rect.width / 2 - m_imgWidth / 2;
                imgPosY = 2;
                rect.x = 2;
                rect.y = m_imgHeight + 4;
                rect.width -= 4;
                rect.height -= m_imgHeight + 6;
                textAlign = wxALIGN_CENTER;
                break;

            case wxRIGHT:
                imgPosX = rect.width - m_imgWidth - 2;
                imgPosY = rect.height / 2 - m_imgHeight / 2;
                rect.x = 2;
                rect.y = 2;
                rect.width -= m_imgWidth + 4;
                rect.height -= 4;
                textAlign = wxALIGN_RIGHT;
                break;

            case wxBOTTOM:
                imgPosX = rect.width / 2 - m_imgWidth / 2;
                imgPosY = rect.height - m_imgHeight - 2;
                rect.x = 2;
                rect.y = 2;
                rect.width -= 6;
                rect.height -= m_imgHeight + 4;
                textAlign = wxALIGN_CENTER;
                break;

            default:
            case wxLEFT:
                imgPosX = 2;
                imgPosY = rect.height / 2 - m_imgHeight / 2;
                rect.x = m_imgWidth + 6;
                rect.y = 2;
                rect.width -= m_imgWidth + 4;
                rect.height -= 4;
                textAlign = wxALIGN_LEFT;
                break;
        }

        dc->DrawBitmap(m_img, imgPosX, imgPosY, true);
    } else { textAlign = m_style.textAlign; rect.Deflate(2); }

    dc->DrawLabel(m_style.title, rect, textAlign | wxALIGN_CENTER_VERTICAL);
}
