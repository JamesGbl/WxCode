/***************************************************************
 * Name:      logoPanel.cpp
 * Purpose:   Implements class for panel with custom background colors and image
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "logoPanel.h"

logoPanel::logoPanel(wxWindow *parent, styleBackground style, wxString filename) : ownerDrawnWindow(parent, wxID_ANY, style, wxDefaultSize)
{
    wxFileName fn(filename, wxPATH_UNIX);
    m_hasImage = false;

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
            else { wxLogWarning(wxT("File '%s' has unknown format"), fn.GetFullPath().c_str()); Hide(); }

        } else { wxLogWarning(wxT("File '%s' not found"), fn.GetFullPath().c_str()); Hide(); }
    }

    if(m_hasImage) SetClientSize(-1, m_imgHeight + 6);
    else this->Hide();
}

logoPanel::~logoPanel()
{
    ;
}

void logoPanel::drawCustom(wxPaintDC *dc)
{
    if(m_hasImage)
    {
        wxSize size = GetClientSize();
        dc->DrawBitmap(m_img, size.x/2-m_imgWidth/2, size.y/2-m_imgHeight/2, true);
    }
}
