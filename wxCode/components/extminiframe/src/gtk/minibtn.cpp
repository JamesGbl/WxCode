/////////////////////////////////////////////////////////////////////////////
// Name:        gtk/palettefrm.cpp
// Purpose:     wxExtMiniFrame, wxExtMiniFrame, wxMiniButton
//              wxCloseBox, wxMaximizeBox, wxMinimizeBox, wxCollapseBox
// Author:      Francesco Montorsi
// Created:     2004/03/03
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


// includes
#include "wx/settings.h"
#include "wx/tokenzr.h"
#include "wx/defs.h"
#include "wx/object.h"
#include "wx/frame.h"
#include "wx/dcscreen.h"
#include "wx/minibtn.h"
#include "wx/log.h"
#include "wx/msgdlg.h"


// include GTK headers
#include "gtk/gtk.h"
#include "wx/gtk/win_gtk.h"
#include <gdk/gdk.h>
#include <gdk/gdkprivate.h>
#include <gdk/gdkx.h>


// RTTI class declarations
IMPLEMENT_DYNAMIC_CLASS(wxCloseBox, wxMiniButton)
IMPLEMENT_DYNAMIC_CLASS(wxMaximizeBox, wxMiniButton)
IMPLEMENT_DYNAMIC_CLASS(wxMinimizeBox, wxMiniButton)
IMPLEMENT_DYNAMIC_CLASS(wxCollapseBox, wxMiniButton)

// required for wxCollapseBox::CreateButtonWidget
extern GtkWidget  *wxGetRootWindow();



// ------------------------
// wxCLOSEBOX
// ------------------------

char *wxCloseBox::m_pixmap[] =
{
/* columns rows colors chars-per-pixel */
"5 5 16 1",
"  c Gray0",
". c #bf0000",
"X c #00bf00",
"o c #bfbf00",
"O c #0000bf",
"+ c #bf00bf",
"@ c #00bfbf",
"# c None",
"$ c #808080",
"% c Red",
"& c Green",
"* c Yellow",
"= c Blue",
"- c Magenta",
"; c Cyan",
": c Gray100",
/* pixels */
" ### ",
"# # #",
"## ##",
"# # #",
" ### "
};




// ------------------------
// wxMAXIMIZEBOX
// ------------------------

char *wxMaximizeBox::m_pixmap[] =
{
/* columns rows colors chars-per-pixel */
"5 5 16 1",
"  c Gray0",
". c #bf0000",
"X c #00bf00",
"o c #bfbf00",
"O c #0000bf",
"+ c #bf00bf",
"@ c #00bfbf",
"# c None",
"$ c #808080",
"% c Red",
"& c Green",
"* c Yellow",
"= c Blue",
"- c Magenta",
"; c Cyan",
": c Gray100",
/* pixels */
"     ",
" ### ",
" ### ",
" ### ",
"     "
};




// ------------------------
// wxMINIMIZEBOX
// ------------------------

char *wxMinimizeBox::m_pixmap[] =
{
/* columns rows colors chars-per-pixel */
"5 5 16 1",
"  c Gray0",
". c #bf0000",
"X c #00bf00",
"o c #bfbf00",
"O c #0000bf",
"+ c #bf00bf",
"@ c #00bfbf",
"# c None",
"$ c #808080",
"% c Red",
"& c Green",
"* c Yellow",
"= c Blue",
"- c Magenta",
"; c Cyan",
": c Gray100",
/* pixels */
"#####",
"#####",
"#####",
"#####",
"     "
};



// ------------------------
// wxCOLLAPSEBOX
// ------------------------

char *wxCollapseBox::m_pixmapUnrolled[] =
{
/* columns rows colors chars-per-pixel */
"5 5 16 1",
"  c Gray0",
". c #bf0000",
"X c #00bf00",
"o c #bfbf00",
"O c #0000bf",
"+ c #bf00bf",
"@ c #00bfbf",
"# c None",
"$ c #808080",
"% c Red",
"& c Green",
"* c Yellow",
"= c Blue",
"- c Magenta",
"; c Cyan",
": c Gray100",
/* pixels */
" ####",
"   ##",
"    #",
"   ##",
" ####"
};

char *wxCollapseBox::m_pixmapRolled[] =
{
/* columns rows colors chars-per-pixel */
"5 5 16 1",
"  c Gray0",
". c #bf0000",
"X c #00bf00",
"o c #bfbf00",
"O c #0000bf",
"+ c #bf00bf",
"@ c #00bfbf",
"# c None",
"$ c #808080",
"% c Red",
"& c Green",
"* c Yellow",
"= c Blue",
"- c Magenta",
"; c Cyan",
": c Gray100",
/* pixels */
"     ",
"#   #",
"## ##",
"#####",
"#####"
};

void wxCollapseBox::CreateButtonWidget(char **xpm)
{
    // create the pixmap & its mask
    GdkBitmap *mask = (GdkBitmap*) NULL;
    GdkPixmap *pixmap =
        gdk_pixmap_create_from_xpm_d(wxGetRootWindow()->window,
                                     &mask,
                                     NULL,
                                     (char **)xpm);

    // WARNING: this is very different from a wxMiniButton::CreateButtonWidget;
    //          a GtkPixmap cannot be modified after its creation, a GtkImage
    //          instead can be changed at will... see #UpdatePixmap
    // create a GtkImage widget
    GtkWidget *pw = gtk_image_new_from_pixmap( pixmap, mask );
    gdk_bitmap_unref( mask );
    gdk_pixmap_unref( pixmap );
    gtk_widget_show( pw );

    // create a GtkButton widget containing the GtkImage widget
    m_button = gtk_button_new();
    gtk_container_add( GTK_CONTAINER(m_button), pw );
}

void wxCollapseBox::UpdatePixmap()
{
    bool rolled = isParentRolled();
    if ((rolled && !m_bShowingRolled) ||
        (!rolled && m_bShowingRolled))
        m_bShowingRolled = !m_bShowingRolled;       // update is required
    else
        return;                                     // no update is required

    GtkWidget *image = gtk_bin_get_child((GtkBin*)m_button);
    if (image == NULL || !GTK_IS_IMAGE(image)) return;

    wxEXTMF_LOG(wxT("wxCollapseBox::UpdatePixmap - updating the button pixmap"));
    GdkBitmap *mask = (GdkBitmap*) NULL;
    GdkPixmap *pix =
        gdk_pixmap_create_from_xpm_d(wxGetRootWindow()->window,
                                     &mask,
                                     NULL,
                                     (char **)(m_bShowingRolled ? m_pixmapRolled : m_pixmapUnrolled));

    gtk_image_set_from_pixmap( (GtkImage*)image, pix, mask );
    gdk_bitmap_unref( mask );
    gdk_bitmap_unref( pix );
}

bool wxCollapseBox::Exec()
{
    wxEXTMF_LOG(wxT("wxCollapseBoxGTK::Exec - parent rollstate: %d"), isParentRolled());
    bool ret = wxCollapseBoxBase::Exec();

    // maybe we need to change the GdkPixmap used for this button...
    UpdatePixmap();
    return ret;
}

void wxCollapseBox::Draw(wxDC &, int, int)
{
    // when the minibutton must be updated, this function is often called...
    UpdatePixmap();
}

