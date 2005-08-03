
#include <wx\wxprec.h>
#include "LCDSampleApp.h"
#include "LCDSampleFrame.h"
#include "clockthread.h"

#include "wx/lcdwindow.h"

// Creation of the application instance
IMPLEMENT_APP( CLCDSampleApp )

bool CLCDSampleApp::OnInit( void )
{
	CLCDSampleFrame *frame = new CLCDSampleFrame( wxT("wxLCDWindow Sample"), wxPoint( 50, 50 ), wxSize( 200, 100 ) );
	mClock = new wxLCDWindow( frame, wxDefaultPosition, wxDefaultSize );
	mClock->SetNumberDigits( 8 );

	mUpdThread = new CClockThread();
	mUpdThread->Run();
	frame->Show(TRUE);

	return TRUE;
}


int CLCDSampleApp::OnExit( void )
{
	return wxApp::OnExit();
}


void CLCDSampleApp::UpdateClock( void )
{
	static wxString now;
	if( now != ::wxNow() )
	{
		now = ::wxNow();
		mClock->SetValue( now.Mid( 11, 8 ) );
	}

	return;
}
