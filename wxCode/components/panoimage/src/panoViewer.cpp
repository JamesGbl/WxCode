#include "wx/wx.h"
#include "wx/image.h"

#ifdef new
#undef new
#endif

#include "PanoImage.h"

class MyApp: public wxApp
{
public:
	virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
};


IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	MyFrame *frame = new MyFrame("wxWindows Panorama Viewer, v1.0",
	                           wxPoint(50, 50), wxSize(450, 340));

	frame->Show(TRUE);
	//frame->ShowFullScreen(TRUE);
	SetTopWindow(frame);

	return TRUE;
}


MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
   wxInitAllImageHandlers();

   // Convert our panorama into the proper format for the libarary
   wxImage p;
   p.LoadFile("pano.jpg");

   PanoImage *pano = new PanoImage(this, -1);
   pano->SetPano(p);
   pano->Show(TRUE);

//	pano->ShowOptionsDialog();
}