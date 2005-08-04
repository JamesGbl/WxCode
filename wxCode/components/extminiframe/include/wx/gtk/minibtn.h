/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/minibtn.h
// Purpose:     wxCloseBox, wxMaximizeBox, wxMinimizeBox, wxCollapseBox
// Author:      Francesco Montorsi
// Created:     2004/03/03
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#ifndef _WX_MINIBTN_H_
#define _WX_MINIBTN_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "minibtn.h"
#endif


#ifdef wxPALETTEFRM_USE_MINIBTN

// the number of rows in the pixmaps of our minibuttons.
#define wxMINIBNT_PIXMAP_ROWS			22



// wxCloseBox is a window close button (the classical "x" in the
// top-right border of the window caption) which can be used in
// wxPaletteFrames to hide the window.
class WXDLLEXPORT wxCloseBox : public wxCloseBoxBase
{
protected:
	
	static char *m_pixmap[wxMINIBNT_PIXMAP_ROWS];

public:

	wxCloseBox() {}		// required by wxWidgets RTTI system
	wxCloseBox(wxPaletteFrameBase *parent, int bSmall = -1) :
	  wxCloseBoxBase(parent, bSmall) { CreatePixmap(m_pixmap); }
	virtual ~wxCloseBox() {}

private:
	DECLARE_DYNAMIC_CLASS( wxCloseBox )
};


// A minibutton which has the same aspect of the standard
// wxMAXIMIZE_BOX; its behaviour is not defined.
// If you want to use it, derive a new class and override
// the Exec() function.
class WXDLLEXPORT wxMaximizeBox : public wxMaximizeBoxBase
{
protected:
	//void Draw(wxDC &, int, int);
	static char *m_pixmap[wxMINIBNT_PIXMAP_ROWS];

public:

	wxMaximizeBox() {}		// required by wxWidgets RTTI system
	wxMaximizeBox(wxPaletteFrameBase *parent, int bSmall = -1) :
	  wxMaximizeBoxBase(parent, bSmall) { CreatePixmap(m_pixmap); }
	virtual ~wxMaximizeBox() {}

private:
	DECLARE_DYNAMIC_CLASS( wxMaximizeBox )
};


// A minibutton which has the same aspect of the standard
// wxMINIMIZE_BOX; its behaviour is not defined.
// If you want to use it, derive a new class and override
// the Exec() function.
class WXDLLEXPORT wxMinimizeBox : public wxMinimizeBoxBase
{
protected:
	//void Draw(wxDC &, int, int);
	static char *m_pixmap[wxMINIBNT_PIXMAP_ROWS];

public:

	wxMinimizeBox() {}		// required by wxWidgets RTTI system
	wxMinimizeBox(wxPaletteFrameBase *parent, int bSmall = -1) :
	  wxMinimizeBoxBase(parent, bSmall) { CreatePixmap(m_pixmap); }
	virtual ~wxMinimizeBox() {}

private:
	DECLARE_DYNAMIC_CLASS( wxMinimizeBox )
};



// A wxMiniButton which allows the user to roll/unroll the 
// wxPaletteFrame associated: roll/unroll is useful for
// those palettes which are quite cumbersome.
//
// wxCollapseBox appears as a triangle pointing right when
// the window is rolled-up and pointing left when the window
// is unrolled...
class WXDLLEXPORT wxCollapseBox : public wxCollapseBoxBase
{
protected:

	// wxCollapseBox under MSW uses the same common Draw() function
	// defined in src/common/minibtncmn.cpp
	//  void Draw(wxDC &, int, int);
	static char *m_pixmapUnrolled[wxMINIBNT_PIXMAP_ROWS],
				*m_pixmapRolled[wxMINIBNT_PIXMAP_ROWS];

	// TRUE if the #m_pixmapRolled pixmap is currently being show.
	bool m_bShowingRolled;
	
protected:		// some overloads	
	
	bool Exec();
	void Draw(wxDC &, int, int);	
	
	// these two are required to handle the GtkImage we use
	void UpdatePixmap();
	void CreateButtonWidget(char **xpm);

public:

	wxCollapseBox() {}		// required by wxWidgets RTTI system
	wxCollapseBox(wxPaletteFrameBase *parent, int bSmall = -1) :
	  wxCollapseBoxBase(parent, bSmall) 
	  { CreatePixmap(m_pixmapRolled); m_bShowingRolled=TRUE; }
	virtual ~wxCollapseBox() {}

private:
	DECLARE_DYNAMIC_CLASS( wxCollapseBox )
};

#endif			// wxPALETTEFRM_USE_MINIBTN
#endif			// _WX_MINIBTN_H_
