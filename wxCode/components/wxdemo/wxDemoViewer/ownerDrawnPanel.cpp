/***************************************************************
 * Name:      ownerDrawnPanel.cpp
 * Purpose:   Implements class for panel with custom background colors
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "ownerDrawnPanel.h"
#include <wx/listimpl.cpp>

WX_DEFINE_LIST(cWindowList);

cWindowListElement::cWindowListElement(wxWindow *window, styleControl bs)
{
    this->window = window;
    this->bs = bs;
}

cWindowListElement::~cWindowListElement()
{
    ;
}


BEGIN_EVENT_TABLE(ownerDrawnPanel, ownerDrawnWindow)
    EVT_SIZE(ownerDrawnPanel::OnSize)
    WX_EVENT_TABLE_CONTROL_CONTAINER(ownerDrawnPanel)
END_EVENT_TABLE()

WX_DELEGATE_TO_CONTROL_CONTAINER(ownerDrawnPanel, wxWindow)

ownerDrawnPanel::ownerDrawnPanel(wxWindow *parent, styleBackground style, wxSize size) : ownerDrawnWindow(parent, wxID_ANY, style, size)
{
    SetThemeEnabled(true);
    m_container.SetContainerWindow(this);
}

ownerDrawnPanel::~ownerDrawnPanel()
{
    ;
}

void ownerDrawnPanel::AppendBorder(wxWindow *window, styleControl style)
{
    if(style.hasBorder) m_windowList.Append(new cWindowListElement(window, style));
}

void ownerDrawnPanel::OnSize(wxSizeEvent& event)
{
    if (GetAutoLayout()) Layout();

    event.Skip();
}

void ownerDrawnPanel::drawCustom(wxPaintDC *dc)
{
    wxRect rect;

    cWindowList::compatibility_iterator node = m_windowList.GetFirst();
    while(node)
    {
        cWindowListElement *current = node->GetData();

        if(current->window->IsShown())
        {
            rect = current->window->GetRect();
            wxBrush brush(current->bs.backColor);
            wxPen pen(current->bs.borderColor, 1);

            dc->SetPen(pen);
            dc->SetBrush(brush);

            if(current->bs.rounded) dc->DrawRoundedRectangle(rect.x-2, rect.y-2, rect.width+4, rect.height+4, 3);
            else dc->DrawRectangle(rect.x-1, rect.y-1, rect.width+2, rect.height+2);

            dc->SetPen(wxNullPen);
            dc->SetBrush(wxNullBrush);
        }

        node = node->GetNext();
    }
}
