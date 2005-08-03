#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#include <wx\wxprec.h>

class CLCDSampleFrame : public wxFrame
{
protected:
	void OnClose( wxCloseEvent &event );

public:
	CLCDSampleFrame( const wxString& title, const wxPoint& pos, const wxSize& size );

DECLARE_EVENT_TABLE()
};

#endif // __MAINFRAME_H__
