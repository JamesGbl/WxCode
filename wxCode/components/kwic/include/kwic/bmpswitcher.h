/////////////////////////////////////////////////////////////////////////////
// Name:        BmpSwitcher.h
// Purpose:     wxIndustrialControls Library
// Author:      Marco Cavallini <m.cavallini AT koansoftware.com>
// Copyright:   (C)2004-2007 Copyright by Koan s.a.s. - www.koansoftware.com
// Licence:     KWIC License http://www.koansoftware.com/kwic/kwic-license.htm
/////////////////////////////////////////////////////////////////////////////
// History:
// 
//  + 20/06/07 Ported to wxGTK - Version 2.8.4 by Ettl Martin 
// 	   (ettl@fs.wettzell.de)
// 
/////////////////////////////////////////////////////////////////////////////

#include "kwic/kwicdef.h"

WX_DECLARE_LIST(wxBitmap, CBmpList);

class WXDLLIMPEXP_KWIC kwxBmpSwitcher : public wxWindow
{
public:
	kwxBmpSwitcher(wxWindow *parent,
				const wxWindowID id         = -1,
				const wxString&  label      = wxEmptyString,
				const wxPoint&   pos        = wxDefaultPosition,
				const wxSize&    size       = wxDefaultSize,
				const long int   style      = 0);

	virtual ~kwxBmpSwitcher();
	
	//Incrementa stato di avanzamento
	void IncState() ;
	//Imposta stato di avanzamento
	void SetState(int state) ;

	int GetState() { return m_nState ; } ;
	//Aggiunge una bitmap alla lista
	void AddBitmap(wxBitmap *bitmap) ;

	CBmpList m_bmplist;

	wxBitmap *m_pCurrent ;

private:

	long int m_style;
	wxString m_label;
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()

	void    OnPaint(wxPaintEvent& event);
	void	OnEraseBackGround(wxEraseEvent& WXUNUSED(event)) {};

	wxBitmap *membitmap ;

	int m_nCount ;
	int m_nState ;

} ;

