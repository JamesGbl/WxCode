/////////////////////////////////////////////////////////////////////////////
/// @file        wxRubberBand.cpp
/// @brief       Declaration of a Rubber Band class
///				 This is mainly used for drawing a selection box arround something
/// @Author      Lucien Schreiber
//  Modified by
//  Created      12/10/2006
//  RCS-ID:
//  Copyright:   (c) Lucien Schreiber CREALP 2006
//  Licence:     wxWindows licence
//  @version     1.0
/////////////////////////////////////////////////////////////////////////////

#include <wx/rubberband/wxRubberBand.h>

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "wxRubberBand.h"
#endif


#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// Include private header
//#include "wxGISRaster.h"


// default constructor
wxRubberBand::wxRubberBand()
{
	//RubberInitValues (NULL);
}

// constructor
wxRubberBand::wxRubberBand(wxWindow *wnd)
{
	// we init all internal values
	RubberInitValues ();
	// we store the wnd 
	theWnd = wnd;	
}

// destructor
wxRubberBand::~wxRubberBand()
{
	// do nothing for the moment
}

void wxRubberBand::RubberInitValues()
{
	
	// init member data
	theOldRubberRect = wxRect(0,0,0,0);
	SetPen ();


}

void wxRubberBand::SetGeometry(int x, int y, int width, int height)
{
	
		wxClientDC MyDC(theWnd);
		MyDC.SetLogicalFunction (wxINVERT);
		// draw lines
		MyDC.SetPen (thePen);
		if (VerifyRubberLines(x,y,width,height)) // must be not null see documentation
			DrawRectangleLines (&MyDC,x,y,width,height);
}

void wxRubberBand::DrawRectangleLines(wxDC * MyDC,int x, int y, int width, int height)
{
	// dessine new lines
	MyDC->DrawLine (x,y, x+width,y);				// top segment
	MyDC->DrawLine (x+width,y,x+width,y+height);	// right segment
	MyDC->DrawLine (x+width,y+height,x,y+height);	// bottom segment
	MyDC->DrawLine (x,y+height,x,y);				// left segment

	// Erase old line if theOldRubberRect != 0
	if (!theOldRubberRect.IsEmpty())
	{
	MyDC->DrawLine (theOldRubberRect.x,theOldRubberRect.y, 
		theOldRubberRect.x+theOldRubberRect.width,theOldRubberRect.y);	// top segment
	MyDC->DrawLine (theOldRubberRect.x+theOldRubberRect.width,theOldRubberRect.y,
		theOldRubberRect.x+theOldRubberRect.width,theOldRubberRect.y+theOldRubberRect.height);	// right segment
	MyDC->DrawLine (theOldRubberRect.x+theOldRubberRect.width,
		theOldRubberRect.y+theOldRubberRect.height,theOldRubberRect.x,
		theOldRubberRect.y+theOldRubberRect.height);	// bottom segment
	MyDC->DrawLine (theOldRubberRect.x,theOldRubberRect.y+theOldRubberRect.height,
		theOldRubberRect.x,theOldRubberRect.y);				// left segment
	}
	theOldRubberRect.x = x;
	theOldRubberRect.y = y;
	theOldRubberRect.width = width;
	theOldRubberRect.height = height;

}
void wxRubberBand::SetPen()
{
	wxPen pen(*wxBLACK,1,wxDOT);
	thePen = pen;
	
}

bool wxRubberBand::VerifyRubberLines(int x, int y, int width, int height)
{
	if (x < 0 || y < 0)
		return FALSE;
	if (width <= 0 || height <= 0)
		return FALSE;
	return TRUE;

}

void wxRubberBand::SetPen(wxPen &pen)
{
	thePen = pen;
}

void wxRubberBand::ClearOldRubberRect()
{
	theOldRubberRect = wxRect (0,0,0,0);
}
