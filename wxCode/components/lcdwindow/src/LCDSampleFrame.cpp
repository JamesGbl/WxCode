#include "LCDSampleApp.h"
#include "LCDSampleFrame.h"
#include "ClockThread.h"

BEGIN_EVENT_TABLE( CLCDSampleFrame, wxFrame )
	EVT_CLOSE( CLCDSampleFrame::OnClose )
END_EVENT_TABLE()

CLCDSampleFrame::CLCDSampleFrame( const wxString& title, const wxPoint& pos, const wxSize& size ) : wxFrame( NULL, -1, title, pos, size )
{
	SetIcon( wxICON( LCDSample ) );
}


void CLCDSampleFrame::OnClose( wxCloseEvent &event )
{
	Show( false );
	wxGetApp().mUpdThread->Delete();
	wxSleep( 1 );
	event.Skip();
	return;
}
