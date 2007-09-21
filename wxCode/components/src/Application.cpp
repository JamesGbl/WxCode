#include "Application.h"
#include "Frame.h"

// create the app
IMPLEMENT_APP(MyApp);

// initialize the frame
bool MyApp::OnInit()
{

	MyFrame* frame = new MyFrame(0L, _("wxWidgets::wxIndustrial Control"));

	frame->Show();
	return true;
}
