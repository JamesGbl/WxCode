/////////////////////////////////////////////////////////////////////////////
// Name:        gtk/palettefrm.cpp
// Purpose:     wxExtMiniFrame, wxExtMiniFrame, wxMiniButton
//              wxCloseBox, wxMaximizeBox, wxMinimizeBox, wxCollapseBox
// Author:      Francesco Montorsi (mostly based on wxMiniFrame by Robert Roebling)
// Created:     2004/03/03
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////




#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "palettefrm.h"
#endif

// Include some wxWidgets headers
#include "wx/frame.h"
#include "wx/log.h"
#include "wx/dcclient.h"
#include "wx/palettefrm.h"
#include "wx/minibtn.h"
#include "wx/msgdlg.h"

// include GTK headers
#include "gtk/gtk.h"
#include "wx/gtk/win_gtk.h"
//#include "wx/gtk/private.h"
#include <gdk/gdk.h>
#include <gdk/gdkprivate.h>
#include <gdk/gdkx.h>




// wxWidgets RTTI system
IMPLEMENT_DYNAMIC_CLASS(wxExtMiniFrame, wxFrame)
IMPLEMENT_ABSTRACT_CLASS(wxMiniButton, wxObject)

// event table
BEGIN_EVENT_TABLE(wxExtMiniFrame, wxFrame)

    // for some reason, we need to specify these handlers here too
    // (see the event table of wxExtMiniFrameBase)
    EVT_CLOSEBOX_CLICKED( -1, wxExtMiniFrameBase::OnCloseBox )
    EVT_COLLAPSEBOX_CLICKED( -1, wxExtMiniFrameBase::OnCollapseBox )

END_EVENT_TABLE()




//-----------------------------------------------------------------------------
// idle system
//-----------------------------------------------------------------------------

extern void wxapp_install_idle_handler();
extern bool g_isIdle;

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

extern bool        g_blockEventsOnDrag;
extern bool        g_blockEventsOnScroll;
extern GtkWidget  *wxGetRootWindow();

//-----------------------------------------------------------------------------
// local functions
//-----------------------------------------------------------------------------

/* draw XOR rectangle when moving mine frame around */
static void DrawFrame( GtkWidget *widget, int x, int y, int w, int h )
{
    int org_x = 0;
    int org_y = 0;
    gdk_window_get_origin( widget->window, &org_x, &org_y );
    x += org_x;
    y += org_y;

    GdkGC *gc = gdk_gc_new( GDK_ROOT_PARENT() );
    gdk_gc_set_subwindow( gc, GDK_INCLUDE_INFERIORS );
    gdk_gc_set_function( gc, GDK_INVERT );

    gdk_draw_rectangle( GDK_ROOT_PARENT(), gc, FALSE, x, y, w, h );
    gdk_gc_unref( gc );
}

//-----------------------------------------------------------------------------
// "expose_event" of m_mainWidget
//-----------------------------------------------------------------------------

static void gtk_window_own_expose_callback( GtkWidget *widget,
                                           GdkEventExpose *gdk_event, wxExtMiniFrame *win )
{
    wxEXTMF_LOG(wxT("gtk_window_own_expose_callback"));
    if (g_isIdle) wxapp_install_idle_handler();

    if (!win->m_hasVMT) return;
    if (gdk_event->count > 0) return;

    GtkPizza *pizza = GTK_PIZZA(widget);

    // draw the borders
    gtk_draw_shadow( widget->style,
        pizza->bin_window,
        GTK_STATE_NORMAL,
        GTK_SHADOW_OUT,
        0, 0,
        win->m_width, win->m_height );

    if (!win->GetTitle().IsEmpty() &&
        ((win->GetWindowStyle() & wxCAPTION) ||
        (win->GetWindowStyle() & wxTINY_CAPTION_HORIZ) ||
        (win->GetWindowStyle() & wxTINY_CAPTION_VERT)))
    {
        wxClientDC dc(win);
        dc.SetFont( *wxSMALL_FONT );

        // get the metrics of the paletteframe involved:
        // some adjustments are hardcoded to make the extminiframe looks better
        int height = win->GetTitleHeight(), bs = win->GetBorderSize();

        GdkGC *gc = gdk_gc_new( pizza->bin_window );
        gdk_gc_set_foreground( gc, &widget->style->bg[GTK_STATE_SELECTED] );
        gdk_draw_rectangle( pizza->bin_window, gc, TRUE,
            bs*2, bs+1,
            win->m_width - bs*4, height );      // gdk_draw_rectangle requires the width & height of rect here
        gdk_gc_unref( gc );

        // Hack alert
        dc.m_window = pizza->bin_window;
        dc.SetTextForeground( *wxWHITE );
        dc.DrawText( win->GetTitle(), bs*3, bs*2 );
    }
}

//-----------------------------------------------------------------------------
// "draw" of m_mainWidget
//-----------------------------------------------------------------------------

#ifndef __WXGTK20__
static void gtk_window_own_draw_callback( GtkWidget *widget,
                                         GdkRectangle *WXUNUSED(rect), wxFrame *win )
{
    wxEXTMF_LOG(wxT("gtk_window_own_draw_callback"));
    if (g_isIdle) wxapp_install_idle_handler();

    if (!win->m_hasVMT) return;

    GtkPizza *pizza = GTK_PIZZA(widget);

    gtk_draw_shadow( widget->style,
        pizza->bin_window,
        GTK_STATE_NORMAL,
        GTK_SHADOW_OUT,
        0, 0,
        win->m_width, win->m_height );

    if (!win->m_title.IsEmpty() &&
        ((win->GetWindowStyle() & wxCAPTION) ||
        (win->GetWindowStyle() & wxTINY_CAPTION_HORIZ) ||
        (win->GetWindowStyle() & wxTINY_CAPTION_VERT)))
    {
        wxClientDC dc(win);
        dc.SetFont( *wxSMALL_FONT );
        int height = dc.GetCharHeight();

        GdkGC *gc = gdk_gc_new( pizza->bin_window );
        gdk_gc_set_foreground( gc, &widget->style->bg[GTK_STATE_SELECTED] );
        gdk_draw_rectangle( pizza->bin_window, gc, TRUE,
            3,
            3,
            win->m_width - 7,
            height+1 );
        gdk_gc_unref( gc );

        // Hack alert
        dc.m_window = pizza->bin_window;
        dc.SetTextForeground( *wxWHITE );
        dc.DrawText( win->GetTitle(), 6, 3 );
    }
}
#endif

//-----------------------------------------------------------------------------
// "button_press_event" of m_mainWidget
//-----------------------------------------------------------------------------

static gint gtk_window_button_press_callback( GtkWidget *widget,
                                             GdkEventButton *gdk_event, wxExtMiniFrame *win )
{
    wxEXTMF_LOG(wxT("gtk_window_button_press_callback"));
    if (g_isIdle) wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return TRUE;
    if (g_blockEventsOnScroll) return TRUE;

    if (win->m_isDragging) return TRUE;

#ifdef wxEXTMINIFRM_USE_EXTMINIFRM
    // right button clicks are used to pop up the menu context
    if (gdk_event->button == 3) {

        wxEXTMF_LOG(wxT("gtk_window_button_press_callback - handling a right click at [%d;%d] for [%d]; ")
            wxT("palettefrm label window is [%d]"),
            (int)gdk_event->x, (int)gdk_event->y, gdk_event->window, win->m_gdkLabelWnd);
        wxPoint pt((int)gdk_event->x, (int)gdk_event->y);// + win->GetTitleHeight());

        if (win &&
            win->m_gdkLabelWnd != gdk_event->window &&
            gdk_event->window != NULL) {

            wxExtMiniFrameHitCode res = win->HitTest(pt);
            if (res == wxPFHT_TOPLEVEL_TITLEBAR)
                win->OnPosRightUp(pt);

        } else
            wxEXTMF_LOG(wxT("gtk_window_button_press_callback - skipping this right click"));
        return TRUE;
    }
#endif

    GtkPizza *pizza = GTK_PIZZA(widget);
    if (gdk_event->window != pizza->bin_window) return TRUE;

    wxClientDC dc(win);
    dc.SetFont( *wxSMALL_FONT );
    int height = dc.GetCharHeight() + 1;

    if (gdk_event->y > height) return TRUE;

    gdk_window_raise( win->m_widget->window );

    gdk_pointer_grab( widget->window, FALSE,
        (GdkEventMask)
        (GDK_BUTTON_PRESS_MASK |
        GDK_BUTTON_RELEASE_MASK |
        GDK_POINTER_MOTION_MASK        |
        GDK_POINTER_MOTION_HINT_MASK  |
        GDK_BUTTON_MOTION_MASK        |
        GDK_BUTTON1_MOTION_MASK),
        (GdkWindow *) NULL,
        (GdkCursor *) NULL,
        (unsigned int) GDK_CURRENT_TIME );

    win->m_diffX = (int)gdk_event->x;
    win->m_diffY = (int)gdk_event->y;
    DrawFrame( widget, 0, 0, win->m_width, win->m_height );
    win->m_oldX = 0;
    win->m_oldY = 0;

    win->m_isDragging = TRUE;

    return TRUE;
}

//-----------------------------------------------------------------------------
// "button_release_event" of m_mainWidget
//-----------------------------------------------------------------------------

static gint gtk_window_button_release_callback( GtkWidget *widget, GdkEventButton *gdk_event, wxExtMiniFrame *win )
{
    wxEXTMF_LOG(wxT("gtk_window_button_release_callback"));
    if (g_isIdle) wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return TRUE;
    if (g_blockEventsOnScroll) return TRUE;

    if (!win->m_isDragging) return TRUE;

    win->m_isDragging = FALSE;

    int x = (int)gdk_event->x;
    int y = (int)gdk_event->y;

    DrawFrame( widget, win->m_oldX, win->m_oldY, win->m_width, win->m_height );
    gdk_pointer_ungrab ( (guint32)GDK_CURRENT_TIME );
    int org_x = 0;
    int org_y = 0;
    gdk_window_get_origin( widget->window, &org_x, &org_y );
    x += org_x - win->m_diffX;
    y += org_y - win->m_diffY;
    win->m_x = x;
    win->m_y = y;
    gtk_widget_set_uposition( win->m_widget, x, y );

    return TRUE;
}

//-----------------------------------------------------------------------------
// "motion_notify_event" of m_mainWidget
//-----------------------------------------------------------------------------

static gint gtk_window_motion_notify_callback( GtkWidget *widget, GdkEventMotion *gdk_event, wxExtMiniFrame *win )
{
    if (g_isIdle) wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;
    if (g_blockEventsOnDrag) return TRUE;
    if (g_blockEventsOnScroll) return TRUE;

    if (!win->m_isDragging) return TRUE;

    if (gdk_event->is_hint)
    {
        int x = 0;
        int y = 0;
        GdkModifierType state;
        gdk_window_get_pointer(gdk_event->window, &x, &y, &state);
        gdk_event->x = x;
        gdk_event->y = y;
        gdk_event->state = state;
    }

    DrawFrame( widget, win->m_oldX, win->m_oldY, win->m_width, win->m_height );
    win->m_oldX = (int)gdk_event->x - win->m_diffX;
    win->m_oldY = (int)gdk_event->y - win->m_diffY;
    DrawFrame( widget, win->m_oldX, win->m_oldY, win->m_width, win->m_height );

    return TRUE;
}






//-----------------------------------------------------------------------------
// wxEXTMINIFRAME
//-----------------------------------------------------------------------------

bool wxExtMiniFrame::Create( wxMainFrame *parent, wxWindowID id, const wxString &title,
                            const wxPoint &pos, const wxSize &size,
                            long style, const wxString &name )
{
    if ((style & wxCAPTION) || (style & wxTINY_CAPTION_HORIZ) || (style & wxTINY_CAPTION_VERT))
        m_miniTitle = GetTitleHeight();

    m_miniEdge = GetBorderSize();
    m_isDragging = FALSE;
    m_oldX = -1;
    m_oldY = -1;
    m_diffX = 0;
    m_diffY = 0;

    // create our base version
    wxEXTMF_LOG(wxT("wxExtMiniFrame::Create - creating the wxExtMiniFrameBase"));
    if (!wxExtMiniFrameBase::Create( parent, id, title, pos, size, style, name ))
        return FALSE;

    // this let us to be placed over the parent...
    if (m_parent && (GTK_IS_WINDOW(m_parent->m_widget))) {
        gtk_window_set_transient_for( GTK_WINDOW(m_widget), GTK_WINDOW(parent->m_widget) );
#ifdef __WXGTK20__
        gtk_window_set_skip_taskbar_hint(GTK_WINDOW(m_widget), TRUE);
#endif
    }

    // these are called when the borders are drawn
    gtk_signal_connect( GTK_OBJECT(m_mainWidget), "expose_event",
        GTK_SIGNAL_FUNC(gtk_window_own_expose_callback), (gpointer)this );

#ifndef __WXGTK20__
    gtk_signal_connect( GTK_OBJECT(m_mainWidget), "draw",
        GTK_SIGNAL_FUNC(gtk_window_own_draw_callback), (gpointer)this );
#endif

    // these are required for dragging the mini frame around
    gtk_signal_connect( GTK_OBJECT(m_mainWidget), "button_press_event",
        GTK_SIGNAL_FUNC(gtk_window_button_press_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(m_mainWidget), "button_release_event",
        GTK_SIGNAL_FUNC(gtk_window_button_release_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(m_mainWidget), "motion_notify_event",
        GTK_SIGNAL_FUNC(gtk_window_motion_notify_callback), (gpointer)this );

    // this is the GdkWindow used to recognize the mouse signals sent to
    // our label... see gtk_window_button_press_callback
    m_gdkLabelWnd = gtk_widget_get_root_window(m_widget);

    return TRUE;
}

wxExtMiniFrame::wxExtMiniFrame(wxMainFrameBase* parent,
                               wxWindowID id,
                               const wxString &title,
                               const wxPoint& pos,
                               const wxSize& size,
                               long style,
                               const wxString& name)
{
    // just create the window...
    Create(parent, id, title, pos, size, style, name);
}

wxExtMiniFrame::~wxExtMiniFrame()
{
    wxEXTMF_LOG(wxT("wxExtMiniFrameGTK - destroyed"));
}





//-----------------------------------------------------------------------------
// wxMINIBUTTON
//-----------------------------------------------------------------------------

static void gtk_button_clicked_callback( GtkWidget *WXUNUSED(widget), wxMiniButton *mb )
{
    wxEXTMF_LOG(wxT("gtk_button_clicked_callback"));
    if (g_isIdle) wxapp_install_idle_handler();

    // this minibtn has been clicked...
    mb->OnExec();
}

void wxMiniButton::CreateButtonWidget(char **xpm)
{
    // create the pixmap & its mask
    GdkBitmap *mask = (GdkBitmap*) NULL;
    GdkPixmap *pixmap = gdk_pixmap_create_from_xpm_d(
        wxGetRootWindow()->window,
        &mask,
        NULL,
        (char **)xpm);

    // create a GtkPixmap widget
    GtkWidget *pw = gtk_pixmap_new( pixmap, mask );
    gdk_bitmap_unref( mask );
    gdk_pixmap_unref( pixmap );
    gtk_widget_show( pw );

    // create a GtkButton widget containing the GtkPixmap widget
    m_button = gtk_button_new();
    gtk_container_add( GTK_CONTAINER(m_button), pw );
}

void wxMiniButton::CreatePixmap(char **xpm)
{
    // init the m_button widget
    CreateButtonWidget(xpm);

    wxExtMiniFrameBase *parent = GetParent();

    // attach this GtkButton to our parent
    wxEXTMF_LOG(wxT("wxMiniButton::CreatePixmap - creating a pixmap placed at [%d;%d]"),
                        GetPos().x, GetPos().y);
    gtk_pizza_put( GTK_PIZZA(parent->m_mainWidget),
        m_button,
        GetPos().x, GetPos().y, GetSize().GetWidth(), GetSize().GetHeight() );
    gtk_widget_show( m_button );

    // handle the click sent to the widget
    gtk_signal_connect( GTK_OBJECT(m_button), "clicked",
        GTK_SIGNAL_FUNC(gtk_button_clicked_callback), (gpointer*)this );
}

void wxMiniButton::Move(int x, int y)
{
    wxEXTMF_LOG(wxT("wxMiniButton::Move - moving at %d;%d"), x, y);
    wxASSERT(GetParent() != NULL);

    // move the button widget
    //gtk_widget_set_uposition(m_button, x, y);
    gtk_pizza_move(GTK_PIZZA(GetParent()->m_mainWidget), m_button, x, y);

    // update also our internal variables
    wxMiniButtonBase::Move(x, y);
}




