#include "app.h"
#include "frame.h"

// create the app
IMPLEMENT_APP(MyApp);

// initialize the frame
bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

	MyFrame* frame = new MyFrame(0L, _("wxWidgets::wxIndustrial Control"));

	frame->Show();
	return true;
}
