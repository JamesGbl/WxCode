/***************************************************************
 * Name:      sampel.h
 * Purpose:   Header of the sampel-code for the use of wxLEDPanel
 * Author:    Christian Gräfe (info@mcs-soft.de)
 * Created:   2007-03-16
 * Copyright: Christian Gräfe (www.mcs-soft.de)
 * License:
 **************************************************************/

#ifndef SAMPEL_H_INCLUDED
#define SAMPEL_H_INCLUDED

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "wx/wxledpanel.h"

class ledpanelApp : public wxApp
{
	public:
		virtual bool OnInit();
};


class ledpanelFrame: public wxFrame
{
	public:
		ledpanelFrame(wxFrame *frame, const wxString& title);
		~ledpanelFrame();
};



#endif // SAMPEL_H_INCLUDED
