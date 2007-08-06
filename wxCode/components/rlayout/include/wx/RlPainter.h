//
// Copyright (C) 2007 Arne Steinarson <arst at users dot sourceforge dot net>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must
//    not claim that you wrote the original software. If you use this
//    software in a product, an acknowledgment in the product
//    documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must
//    not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source
//    distribution.
//

#ifndef RLPAINTER_H_INCLUDED
#define RLPAINTER_H_INCLUDED

#include "wx/rlayoutdef.h"

#define wxBORDER_ALL_SIDES      (wxLEFT|wxTOP|wxRIGHT|wxBOTTOM)
#define RLP_MAX_BUTTONS         32      // Could be 64 on 64-bit platform

#define RLP_SZ_BUTTON           14
#define RLP_YSZ_CAPTION         22
#define RLP_XSZ_LAYOUT_GRIPPER  4
#define RLP_SZ_PANE_EDGE        7
#define RLP_SZ_MEDIUM_EDGE      4
#define RLP_SZ_LAYOUT_EDGE      2
#define RLP_SZ_THIN_EDGE        2

// Paints various user interface parts
class WXDLLIMPEXP_RLAYOUT RlPainter{
 	public:
 		virtual ~RlPainter(){ }	// To invoke derived destr
 		
        virtual void Render( wxDC *dc, wxRlItem *item, const wxRect &r ) = 0;
        virtual bool RenderBorder( wxDC *dc, int flag, int size, const wxRect &r );
        virtual bool RenderBorder( wxDC *dc, int flag, int size, int x1, int y1, int x2, int y2 ) = 0;

        virtual int GetMetrics( int type ) = 0;
        virtual int GetMetrics2nd( int type ) = 0;
        virtual int GetMetrics( wxRlItem *item );
        virtual int GetMetrics2nd( wxRlItem *item );

        virtual wxColour GetBackgroundColour( ) = 0;
        virtual void UpdateFrameColour( wxColour &fc ) = 0;
        virtual void UpdateCaptionColour( wxColour &fc ) = 0;

        static int Proportion( int type );  // Grows in primary direction?
        static int Expandable( int type );  // Grows in other direction?
};


// Does no painting. Used before application sets a painter
// to be able to provide metrics for wxRlItems.
class WXDLLIMPEXP_RLAYOUT RlStubPainter : public RlPainter{
    public:
        RlStubPainter(){}
        virtual ~RlStubPainter(){ }

        virtual void Render( wxDC *dc, wxRlItem *item, const wxRect &r );
        virtual bool RenderBorder( wxDC *dc, int flag, int size, int x1, int y1, int x2, int y2 );

        virtual int GetMetrics( int type );
        virtual int GetMetrics2nd( int type );

        virtual wxColour GetBackgroundColour( ){ return wxColour(0,0,0); }
        virtual void UpdateFrameColour( wxColour &fc ){ }
        virtual void UpdateCaptionColour( wxColour &fc ){ }
};


class WXDLLIMPEXP_RLAYOUT RlWinStdPainter : public RlPainter{
    public:
        RlWinStdPainter();
        virtual ~RlWinStdPainter(){ Dtor(); }
        void Dtor();

        virtual void Render( wxDC *dc, wxRlItem *item, const wxRect &r );
        virtual bool RenderBorder( wxDC *dc, int flag, int size, int x1, int y1, int x2, int y2 );

        virtual int GetMetrics( int type );
        virtual int GetMetrics2nd( int type );

        virtual wxColour GetBackgroundColour( ){ return m_col_dk_grey; }
        virtual void UpdateFrameColour( wxColour &fc );
        virtual void UpdateCaptionColour( wxColour &fc );

        bool SetButtonBitmap( int button, const char *bits, int w, int h );
        bool SetButtonBitmap( int button, wxBitmap bm );
        
    protected:
        wxBitmap m_buttons[RLP_MAX_BUTTONS];
        wxColour m_col_lt_grey;
        wxColour m_col_grey;
        wxColour m_col_dk_grey;
        wxColour m_col_dk_shadow;
        wxColour m_col_active;
        wxColour m_col_text;
        wxColour m_col_hilight_text;
        wxColour m_col_active_caption;
        wxColour m_col_inactive_caption;
        wxColour m_col_window_frame;

        wxPen m_pen_lt_grey;
        wxPen m_pen_grey;
        wxPen m_pen_dk_grey;
        wxPen m_pen_dk_shadow;
        wxPen m_pen_layout_edge;
        wxPen m_pen_frame;
        
        wxBrush m_brush_layout_edge;
        wxBrush m_brush_lt_grey;
        wxBrush m_brush_dk_grey;
        wxBrush m_brush_active;
        wxBrush m_brush_frame;
        wxBrush m_brush_move_hint;
};

#endif // RLPAINTER_H_INCLUDED
