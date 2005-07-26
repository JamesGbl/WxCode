/*
Based on the work an explanations of Frank Huebner, with some modifications 
and updates.

Copyright (C) 2005 Germán Carrera Corraleche

At the University of Cantabria

german.carrera@unican.es

This file is part of PLplot.

   PLplot is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Library Public License as published
   by the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   PLplot is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with PLplot; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

#include "plDevs.h"

#ifdef PLD_wxwin
#include "plplotP.h"
#include "drivers.h"
#include <wx/dc.h>
#include <wx/dcprint.h>

static void
fill_polygon(PLStream *pls);



char* plD_DEVICE_INFO_wxwin = "PlPlot Driver for wxWidgets";

void plD_dispatch_init_wxwin	( PLDispatchTable *pdt );

void plD_init_wxwin		(PLStream *);
void plD_line_wxwin		(PLStream *, short, short, short, short);
void plD_polyline_wxwin		(PLStream *, short *, short *, PLINT);
void plD_eop_wxwin		(PLStream *);
void plD_bop_wxwin		(PLStream *);
void plD_tidy_wxwin		(PLStream *);
void plD_state_wxwin		(PLStream *, PLINT);
void plD_esc_wxwin		(PLStream *, PLINT, void *);

/*--------------------------------------------------------------------------*\
 * plD_dispatch_init_wxwin()
 *
 * Dispatch table entry.
\*--------------------------------------------------------------------------*/

void plD_dispatch_init_wxwin( PLDispatchTable *pdt )
{
#ifndef ENABLE_DYNDRIVERS
    pdt->pl_MenuStr  = "wxPlot using wxDC";
    pdt->pl_DevName  = "wxwin";
#endif
    pdt->pl_type     = plDevType_Interactive;
    pdt->pl_seq      = 8;
    pdt->pl_init     = (plD_init_fp)     plD_init_wxwin;
    pdt->pl_line     = (plD_line_fp)     plD_line_wxwin;
    pdt->pl_polyline = (plD_polyline_fp) plD_polyline_wxwin;
    pdt->pl_eop      = (plD_eop_fp)      plD_eop_wxwin;
    pdt->pl_bop      = (plD_bop_fp)      plD_bop_wxwin;
    pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_wxwin;
    pdt->pl_state    = (plD_state_fp)    plD_state_wxwin;
    pdt->pl_esc      = (plD_esc_fp)      plD_esc_wxwin;
}

/*--------------------------------------------------------------------------*\
 * plD_init_wxwin()
 *
 * Initialize device (terminal).
\*--------------------------------------------------------------------------*/

void
plD_init_wxwin(PLStream *pls)
{

    pls->color = 1;		/* Is a color device */
    pls->dev_fill0 = 1;		/* Handle solid fills */
    pls->dev_fill1 = 0;		/* Handle pattern fills */    
    
    int xmin = 0;
    int xmax = pls->xlength;
    int ymax = 0;
    int ymin = - pls->ylength;

    float pxlx = ((float) pls->xlength)/600.;
    float pxly = ((float) pls->ylength)/600.;

/* Set up device parameters */

    plP_setpxl(pxlx, pxly);
    plP_setphy(xmin, xmax, ymin, ymax);

}

/*--------------------------------------------------------------------------*\
 * The remaining driver functions are to be filled with code.
\*--------------------------------------------------------------------------*/

void
plD_line_wxwin(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
  wxDC *dc;
  dc=(wxDC *)pls->dc;
  dc->DrawLine((long) x1a, -(long) y1a, (long) x2a, -(long) y2a);
}

void
plD_polyline_wxwin(PLStream *pls, short *xa, short *ya, PLINT npts)
{
  // should be changed to use the wxDC::DrawLines function? - Frank Huebner
  // I probed it without good results - Germán Carrera
  int i;
  wxDC *dc;
  dc=(wxDC *)pls->dc;

  dc->BeginDrawing();
  for (i=1;i <npts; i++){
    dc->DrawLine((long) xa[i-1], -(long) ya[i-1], (long) xa[i], -(long) ya[i]);
  }
  dc->EndDrawing();
}

void
plD_eop_wxwin(PLStream *pls)
{
  wxDC *dc;
  dc=(wxDC *)pls->dc;
 
  dc->StartPage();
}

void
plD_bop_wxwin(PLStream *pls)
{
  wxDC *dc;
  dc=(wxDC *)pls->dc;
#ifdef win32  
  //  if (!dc->IsKindOf(CLASSINFO(wxPrinterDC)))
  //  {
  //    dc->Clear();
  //  }
#endif
}

void
plD_tidy_wxwin(PLStream *pls)
{
}

void 
plD_state_wxwin(PLStream *pls, PLINT op)
{
  wxDC *dc;
  wxPen *pen;
  wxBrush *brush;
  dc=(wxDC *)pls->dc;
  switch (op) {

  case PLSTATE_WIDTH:
     const_cast<wxPen&>(dc->GetPen()).SetWidth(pls->width);
  break;
  case PLSTATE_COLOR0:
    pen=new wxPen(dc->GetPen());
    pen->SetColour(pls->cmap0[pls->icol0].r,
		   pls->cmap0[pls->icol0].g,
		   pls->cmap0[pls->icol0].b);
    dc->SetPen(*pen);
    delete pen;
    
    brush=new wxBrush(dc->GetBrush());
    brush->SetStyle(wxSOLID);
    brush->SetColour(pls->cmap0[pls->icol0].r,
		     pls->cmap0[pls->icol0].g,
		     pls->cmap0[pls->icol0].b);
    
    dc->SetBrush(*brush);
    delete brush;
    break;
  case PLSTATE_COLOR1:
    pen=new wxPen(dc->GetPen());
    pen->SetColour(pls->curcolor.r,
		   pls->curcolor.g,
		   pls->curcolor.b);
    dc->SetPen(*pen);
    delete pen;
    
    brush=new wxBrush(dc->GetBrush());
    brush->SetStyle(wxSOLID);
    brush->SetColour(pls->curcolor.r,
		     pls->curcolor.g,
		     pls->curcolor.b);
    
    dc->SetBrush(*brush);
    delete brush;
    break;
  }
}

void
plD_esc_wxwin(PLStream *pls, PLINT op, void *ptr)
{
  switch (op) {
  case PLESC_FILL:
    fill_polygon(pls);
    break;
  }
}

#else
int 
pldummy_wxwin()
{
    return 0;
}

#endif				/* PLD_wxwin */


/*--------------------------------------------------------------------------*\
 * fill_polygon()
 *
 * Fill polygon described in points pls->dev_x[] and pls->dev_y[].
\*--------------------------------------------------------------------------*/

static void
fill_polygon(PLStream *pls)
{
  int i;
  wxDC *dc;
  wxPoint *points;
  dc=(wxDC *)pls->dc;
  points = new wxPoint[pls->dev_npts];
  
  for (i=0; i < pls->dev_npts; i++)
    {
      points[i].x=pls->dev_x[i];
      points[i].y=-pls->dev_y[i];
    }

  dc->DrawPolygon(pls->dev_npts, points);
  delete[] points;
}
