/***************************************************************
 * Name:      ownerDrawnComboBox.cpp
 * Purpose:   Implements class encapsulating single Combo Box item and class for Combo Box with owner-drawn list items
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "ownerDrawnComboBox.h"
#include <wx/listimpl.cpp>

WX_DEFINE_LIST(ownerDrawnComboBoxDataList);

ownerDrawnComboBoxData::ownerDrawnComboBoxData(wxString text, wxString value, int imgIndex, int lexer)
{
    this->text = text;
    this->value = value;
    this->imgIndex = imgIndex;
    this->lexer = lexer;
}

ownerDrawnComboBoxData::~ownerDrawnComboBoxData()
{
    ;
}


ownerDrawnComboBox::ownerDrawnComboBox(wxWindow *parent, wxWindowID id, wxImageList *imgList, wxColour textColor, wxColour backColor) : wxOwnerDrawnComboBox(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY | wxBORDER_NONE)
{
    m_pImgList = imgList;
    m_textColor = textColor;
    m_backColor = backColor;

    m_pImgList->GetSize(0, m_imgSize.x, m_imgSize.y);
    m_imgCount = m_pImgList->GetImageCount();
}

ownerDrawnComboBox::~ownerDrawnComboBox()
{
    ;
}

void ownerDrawnComboBox::setData(ownerDrawnComboBoxDataList *data)
{
    m_pData = data;

    Clear();

    ownerDrawnComboBoxDataList::compatibility_iterator node = m_pData->GetFirst();
    while(node)
    {
        ownerDrawnComboBoxData *current = node->GetData();
        Append(current->text, current);

        node = node->GetNext();
    }

    if(m_pData->GetCount()>0) Select(0);
}

void ownerDrawnComboBox::OnDrawItem(wxDC &dc, const wxRect &rect, int item, int flags) const
{
    if(item==wxNOT_FOUND) return;
    ownerDrawnComboBoxData *d = (ownerDrawnComboBoxData*)GetClientData(item);
    if(d==NULL) return;
    wxRect tRect = rect;

    if((flags & wxODCB_PAINTING_SELECTED) && !(flags & wxODCB_PAINTING_CONTROL)) dc.SetTextForeground(m_backColor);
    else dc.SetTextForeground(m_textColor);

    tRect.Deflate(2);
    if(m_imgCount>0) tRect.x += 20;

    dc.DrawLabel(d->text, tRect, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
}

void ownerDrawnComboBox::OnDrawBackground(wxDC &dc, const wxRect &rect, int item, int flags) const
{
    wxBrush brush;
    int index, yPos;

    ownerDrawnComboBoxData *d = (ownerDrawnComboBoxData*)GetClientData(item);
    if(d==NULL) return;

    if((flags & wxODCB_PAINTING_SELECTED) && !(flags & wxODCB_PAINTING_CONTROL)) brush = wxBrush(m_textColor);
    else brush = wxBrush(m_backColor);

    dc.SetBackground(brush);
    dc.Clear();
    dc.SetBackground(wxNullBrush);

    if(flags & wxODCB_PAINTING_CONTROL) yPos = rect.height / 2 - m_imgSize.y / 2;
    else yPos = rect.y + 2;

    index = d->imgIndex;
    if(index<m_imgCount) dc.DrawBitmap(m_pImgList->GetBitmap(index), 2, yPos, true);
}

wxCoord ownerDrawnComboBox::OnMeasureItem(size_t item) const
{
    return m_imgSize.y + 4;
}

wxCoord ownerDrawnComboBox::OnMeasureItemWidth(size_t item) const
{
    int textX, textY;

    ownerDrawnComboBoxData *d = (ownerDrawnComboBoxData*)GetClientData(item);

    if(d!=NULL)
    {
        GetTextExtent(d->text, &textX, &textY);
        return textX + m_imgSize.x + 20;
    } else return 120;
}
