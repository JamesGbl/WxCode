/* $Id: wxwin.cpp,v 1.1.1.1 2003-12-28 21:06:28 wyo Exp $
 * $Log: not supported by cvs2svn $
 * Revision 1.3  1999/09/03 13:37:13  fhuebner
 * dc->Clear disabled when plotting to a printer
 *
 * Revision 1.2  1999/09/02 14:54:05  fhuebner
 * Added printing facilities
 *
 * Revision 1.1.1.1  1999/09/01 14:01:41  fhuebner
 * Start of wxPlot
 *
 * Revision 1.2  1999/09/01 13:48:36  fhuebner
 * #if -> #ifdef
 *
 * Revision 1.1.1.1  1999/09/01 12:55:12  fhuebner
 * Start of wxPlot
 *
*/

/*	wxwin.cc

	PLplot wxWindows device driver.
*/
#include "plDevs.h"

#ifdef PLD_wxwin
#include "plplotP.h"
#include "drivers.h"
#include <wx/dc.h>
#include <wx/dcprint.h>

static void
fill_polygon(PLStream *pls);

/*--------------------------------------------------------------------------*\
 * plD_init_wxwin()
 *
 * Initialize device (terminal).
\*--------------------------------------------------------------------------*/

void
plD_init_wxwin(PLStream *pls)
{
  //********
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
  pls->dc->DrawLine((long) x1a, -(long) y1a, (long) x2a, -(long) y2a);
}

void
plD_polyline_wxwin(PLStream *pls, short *xa, short *ya, PLINT npts)
{
  // should be changed to use the wxDC::DrawLines function?
  int i;
  wxDC *dc;
  dc=pls->dc;

  dc->BeginDrawing();
  for (i=1;i <npts; i++){
    dc->DrawLine((long) xa[i-1], -(long) ya[i-1], (long) xa[i], -(long) ya[i]);
  }
  dc->EndDrawing();
}

void
plD_eop_wxwin(PLStream *pls)
{
  pls->dc->StartPage();
}

void
plD_bop_wxwin(PLStream *pls)
{
  if (!pls->dc->IsKindOf(CLASSINFO(wxPrinterDC)))
    {
      pls->dc->Clear();
    }
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
  dc=pls->dc;
  switch (op) {

  case PLSTATE_WIDTH:
    dc->GetPen().SetWidth(pls->width);
    break;
  case PLSTATE_COLOR0:
    pen=new wxPen(dc->GetPen());
    pen->SetColour(pls->cmap0[pls->icol0].r,
		   pls->cmap0[pls->icol0].g,
		   pls->cmap0[pls->icol0].b);
    dc->SetPen(*pen);
    delete pen;
    brush=new wxBrush(pls->dc->GetBrush());
    brush->SetStyle(wxSOLID);
    brush->SetColour(pls->cmap0[pls->icol0].r,
		     pls->cmap0[pls->icol0].g,
		     pls->cmap0[pls->icol0].b);
    pls->dc->SetBrush(*brush);
    delete brush;
    break;
  case PLSTATE_COLOR1:
    pen=new wxPen(dc->GetPen());
    pen->SetColour(pls->curcolor.r,
		   pls->curcolor.g,
		   pls->curcolor.b);
    dc->SetPen(*pen);
    delete pen;
    brush=new wxBrush(pls->dc->GetBrush());
    brush->SetStyle(wxSOLID);
    brush->SetColour(pls->curcolor.r,
		     pls->curcolor.g,
		     pls->curcolor.b);
    pls->dc->SetBrush(*brush);
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
  wxPoint *points;
  points = new wxPoint[pls->dev_npts];
  
  for (i=0; i < pls->dev_npts; i++)
    {
      points[i].x=pls->dev_x[i];
      points[i].y=-pls->dev_y[i];
    }

  pls->dc->DrawPolygon(pls->dev_npts, points);
  delete[] points;
}
