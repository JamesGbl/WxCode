///////////////////////////////////////////////////////////////////////////////
// Name:        steart.cpp
// Purpose:     wxSTEditorArtProvider
// Author:      John Labenski, parts taken from wxGuide by Otto Wyss
// Modified by:
// Created:     29/05/2010
// RCS-ID:
// Copyright:   (c) John Labenski, Troels K, Otto Wyss
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "wx/stedit/stedefs.h"
#include "wx/stedit/steart.h"
#include "wxext.h"

//-----------------------------------------------------------------------------
// wxSTEditorArtProvider
//-----------------------------------------------------------------------------
#include "wx/image.h"

// Bitmaps used for the toolbar in the wxSTEditorFrame
#include "../art/pencil16.xpm"
#include "../art/pencil32.xpm"
#include "../art/new.xpm"
#include "../art/open.xpm"
#include "../art/save.xpm"
#include "../art/saveall.xpm"
#include "../art/saveas.xpm"
#include "../art/print.xpm"
#include "../art/print_preview.xpm"
#include "../art/print_setup.xpm"
#include "../art/print_page_setup.xpm"
#include "../art/x_red.xpm"

#include "../art/cut.xpm"
#include "../art/copy.xpm"
#include "../art/paste.xpm"
#include "../art/find.xpm"
#include "../art/findnext.xpm"
#include "../art/finddown.xpm"
#include "../art/findup.xpm"
#include "../art/replace.xpm"
#include "../art/undo.xpm"
#include "../art/redo.xpm"
#include "../art/cross.xpm"

#define ART(artid, xpmRc) \
    if (id == (artid)) return wxBitmap(xpmRc##_xpm);

static wxBitmap wxSTEditorArtProvider_GetBitmap(const wxArtID& id)
{
    ART(wxART_STEDIT_NEW,            new)
    ART(wxART_STEDIT_OPEN,           open)
    ART(wxART_STEDIT_SAVE,           save)
    ART(wxART_STEDIT_SAVEALL,        saveall)
    ART(wxART_STEDIT_SAVEAS,         saveas)
    ART(wxART_STEDIT_PRINT,          print)
    ART(wxART_STEDIT_PRINTPREVIEW,   print_preview)
    ART(wxART_STEDIT_PRINTSETUP,     print_setup)
    ART(wxART_STEDIT_PRINTPAGESETUP, print_page_setup)
    ART(wxART_STEDIT_EXIT,           x_red)
    ART(wxART_STEDIT_CUT,            cut)
    ART(wxART_STEDIT_COPY,           copy)
    ART(wxART_STEDIT_PASTE,          paste)
    ART(wxART_STEDIT_FIND,           find)
    ART(wxART_STEDIT_FINDNEXT,       findnext)
    ART(wxART_STEDIT_FINDUP,         findup)
    ART(wxART_STEDIT_FINDDOWN,       finddown)
    ART(wxART_STEDIT_REPLACE,        replace)
    ART(wxART_STEDIT_UNDO,           undo)
    ART(wxART_STEDIT_REDO,           redo)
    ART(wxART_STEDIT_CLEAR,          cross)

    return wxNullBitmap;
}

wxSTEditorArtProvider::wxSTEditorArtProvider() : wxArtProvider(), m_app_large(pencil32_xpm), m_app_small(pencil16_xpm)
{
}

wxBitmap wxSTEditorArtProvider::CreateBitmap(const wxArtID& id,
                                             const wxArtClient& client,
                                             const wxSize& reqSize_)
{
    wxBitmap bmp;
    wxSize reqSize = wxIconSize_System;

    if (id == wxART_STEDIT_PREFDLG_VIEW)
        bmp = wxArtProvider::GetBitmap(wxART_FIND, wxART_OTHER, reqSize);
    else if (id == wxART_STEDIT_PREFDLG_TABSEOL)
        bmp = wxArtProvider::GetBitmap(wxART_LIST_VIEW, wxART_OTHER, reqSize);
    else if (id == wxART_STEDIT_PREFDLG_FOLDWRAP)
        bmp = wxArtProvider::GetBitmap(wxART_COPY, wxART_OTHER, reqSize);
    else if (id == wxART_STEDIT_PREFDLG_PRINT)
        bmp = wxArtProvider::GetBitmap(wxART_PRINT, wxART_OTHER, reqSize);
    else if (id == wxART_STEDIT_PREFDLG_LOADSAVE)
        bmp = wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_OTHER, reqSize);
    else if (id == wxART_STEDIT_PREFDLG_HIGHLIGHT)
        bmp = wxArtProvider::GetBitmap(wxART_TIP, wxART_OTHER, reqSize);
    else if (id == wxART_STEDIT_PREFDLG_STYLES)
        bmp = wxArtProvider::GetBitmap(wxART_HELP_BOOK, wxART_OTHER, reqSize);
    else if (id == wxART_STEDIT_PREFDLG_LANGS)
        bmp = wxArtProvider::GetBitmap(wxART_HELP_SETTINGS, wxART_OTHER, reqSize);
    else if (id == wxART_STEDIT_APP)
    {
        // this logic has room for improvement
        bmp = (reqSize_ == wxSize(m_app_small.GetWidth(), m_app_small.GetHeight()))
           ? m_app_small
           : m_app_large;
    }
    else
    {
        bmp = wxSTEditorArtProvider_GetBitmap(id);
        reqSize = reqSize_;
    }

#if wxUSE_IMAGE
    if (bmp.IsOk())
    {
        // fit into transparent image with desired size hint from the client
        if (reqSize == wxDefaultSize)
        {
            // find out if there is a desired size for this client
            wxSize bestSize = GetSizeHint(client);
            if (bestSize != wxDefaultSize)
            {
                int bmp_w = bmp.GetWidth();
                int bmp_h = bmp.GetHeight();
                // want default size but it's smaller, paste into transparent image
                if ((bmp_h < bestSize.x) && (bmp_w < bestSize.y))
                {
                    wxPoint offset((bestSize.x - bmp_w)/2, (bestSize.y - bmp_h)/2);
                    wxImage img = bmp.ConvertToImage();
                    img.Resize(bestSize, offset);
                    bmp = wxBitmap(img);
                }
            }
        }
    }
#endif // wxUSE_IMAGE

    return bmp;
}

/*static*/ wxIcon wxSTEditorArtProvider::GetDefaultDialogIcon()
{
    return GetIcon(wxART_STEDIT_APP, wxDialogIconSize);
}
