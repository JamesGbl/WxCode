/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/minibtn.h
// Purpose:     wxCloseBox, wxMaximizeBox, wxMinimizeBox, wxCollapseBox
// Author:      Francesco Montorsi
// Created:     2004/03/03
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#ifndef _WX_MINIBTN_H_
#define _WX_MINIBTN_H_

#ifdef wxEXTMINIFRM_USE_MINIBTN

// wxCloseBox is a window close button (the classical "x" in the
// top-right border of the window caption) which can be used in
// wxExtMiniFrames to hide the window.
class WXDLLIMPEXP_WXEXTMINIFRAME wxCloseBox : public wxCloseBoxBase
{
protected:
	void Draw(wxDC &, int, int);

public:

	wxCloseBox() {}		// required by wxWidgets RTTI system
	wxCloseBox(wxExtMiniFrameBase *parent, int bSmall = -1) :
	  wxCloseBoxBase(parent, bSmall) {}
	virtual ~wxCloseBox() {}

private:
	DECLARE_DYNAMIC_CLASS( wxCloseBox )
};


// A minibutton which has the same aspect of the standard
// wxMAXIMIZE_BOX; its behaviour is not defined.
// If you want to use it, derive a new class and override
// the Exec() function.
class WXDLLIMPEXP_WXEXTMINIFRAME wxMaximizeBox : public wxMaximizeBoxBase
{
protected:
	void Draw(wxDC &, int, int);

public:

	wxMaximizeBox() {}		// required by wxWidgets RTTI system
	wxMaximizeBox(wxExtMiniFrameBase *parent, int bSmall = -1) :
	  wxMaximizeBoxBase(parent, bSmall) {}
	virtual ~wxMaximizeBox() {}

private:
	DECLARE_DYNAMIC_CLASS( wxMaximizeBox )
};


// A minibutton which has the same aspect of the standard
// wxMINIMIZE_BOX; its behaviour is not defined.
// If you want to use it, derive a new class and override
// the Exec() function.
class WXDLLIMPEXP_WXEXTMINIFRAME wxMinimizeBox : public wxMinimizeBoxBase
{
protected:
	void Draw(wxDC &, int, int);

public:

	wxMinimizeBox() {}		// required by wxWidgets RTTI system
	wxMinimizeBox(wxExtMiniFrameBase *parent, int bSmall = -1) :
	  wxMinimizeBoxBase(parent, bSmall) {}
	virtual ~wxMinimizeBox() {}

private:
	DECLARE_DYNAMIC_CLASS( wxMinimizeBox )
};



// A wxMiniButton which allows the user to roll/unroll the 
// wxExtMiniFrame associated: roll/unroll is useful for
// those miniframes which are quite cumbersome.
//
// wxCollapseBox appears as a triangle pointing right when
// the window is rolled-up and pointing left when the window
// is unrolled...
class WXDLLIMPEXP_WXEXTMINIFRAME wxCollapseBox : public wxCollapseBoxBase
{
protected:

	// wxCollapseBox under MSW uses the same common Draw() function
	// defined in src/common/minibtncmn.cpp
	//  void Draw(wxDC &, int, int);

public:

	wxCollapseBox() {}		// required by wxWidgets RTTI system
	wxCollapseBox(wxExtMiniFrameBase *parent, int bSmall = -1) :
	  wxCollapseBoxBase(parent, bSmall) {}
	virtual ~wxCollapseBox() {}

private:
	DECLARE_DYNAMIC_CLASS( wxCollapseBox )
};


#endif			// wxEXTMINIFRM_USE_MINIBTN

#endif			// _WX_MINIBTN_H_
