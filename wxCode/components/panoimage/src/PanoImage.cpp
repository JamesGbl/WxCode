#include "wx/wx.h"
#include "wx/image.h"
#include "wx/utils.h"

#ifdef new
#undef new
#endif

#include "filter.h"
#include "PanoViewExtractor.h"
#include "PanoImage.h"

BEGIN_EVENT_TABLE(PanoImage, wxPanel)
    EVT_PAINT(PanoImage::OnPaint)
    EVT_KEY_DOWN(PanoImage::OnKeyDown)
    EVT_KEY_UP(PanoImage::OnKeyUp)
    EVT_LEFT_DOWN(PanoImage::OnMouseLeftDown)
    EVT_LEFT_UP(PanoImage::OnMouseLeftUp)
//    EVT_MOTION(PanoImage::OnMouseMove)
    EVT_IDLE(PanoImage::OnIdle)
    EVT_SIZE(PanoImage::OnSize)
END_EVENT_TABLE()


PanoImage::PanoImage ( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxPanel(parent, id, pos, size, style, name)
{
	// Set up view angles and quality
	hfov = 70;
	yaw = 0;
	pitch = 0;
	roll = 0;
	quality = 1;

   // Init action flags
   isPanning = isZoomingIn = isZoomingOut = FALSE;

   // Set the width and height
   GetClientSize(&vw, &vh);
   view.SetViewSize( wxSize(vw, vh) );

	// Init current cached bitmap to a blank bitmap
   currentViewBmp.Create(vw, vh);

   // Init mouse panning speed
   accl = 0.05;
}



PanoImage::~PanoImage()
{
}


void PanoImage::SetPano ( const wxImage &img )
{
	view.SetPano(img);
	currentViewBmp = view.GetView( PanoViewpoint(quality, hfov, yaw, pitch) );
	Refresh();
}


void PanoImage::OnKeyDown(wxKeyEvent &event)
{
	if ( event.ShiftDown() ) isZoomingIn = TRUE;
	if ( event.ControlDown() ) isZoomingOut = TRUE;
}


void PanoImage::OnKeyUp( wxKeyEvent &event )
{
	if ( !event.ShiftDown() ) isZoomingIn = FALSE;
	if ( !event.ControlDown() ) isZoomingOut = FALSE;
}


void PanoImage::OnIdle ( wxIdleEvent &event )
{
	if ( isZoomingIn ) hfov -= 10;
	if ( isZoomingOut ) hfov +=10;
	
	if ( isPanning )
	{
		int x, y, w, h, dx, dy;
		GetPosition(&x, &y);
		ClientToScreen(&x, &y);
		GetClientSize (&w, &h);
		wxGetMousePosition(&mx, &my);

		x += w/2;
		y += h/2;
		dx = x-mx;
		dy = y-my;

		if ( abs(dx) > 15 ) yaw -= ( dx * accl );
		if ( abs(dy) > 15 ) pitch += ( dy * accl );
	}

	// Draw the updated image
	if ( isZoomingIn || isZoomingOut || isPanning )
	{
		// Make sure angles are acceptable, because they changed
		if ( hfov < 10 ) hfov = 10;
		if ( hfov > 150 ) hfov = 150;
		NORM_ANGLE (yaw);
		if ( pitch > 90 ) pitch = 90;
		if ( pitch < -90 ) pitch = -90;

		// Draw a low-quality image
		wxClientDC dc(this);
		PrepareDC(dc);
		quality = 0;
		currentViewBmp = view.GetView( PanoViewpoint(quality, hfov, yaw, pitch) );
		dc.DrawBitmap( currentViewBmp, 0, 0);
	} else {
		// Draw only if we have not already settled on high quality
		if ( quality == 0 )
		{
			quality = 1;
			wxClientDC dc(this);
			PrepareDC(dc);
			currentViewBmp = view.GetView( PanoViewpoint(quality, hfov, yaw, pitch) );
			dc.DrawBitmap( currentViewBmp, 0, 0);
		}
	}

	// Request more idle events. No more will be posted until another event causes one
	// unless we request more now.
	event.RequestMore();
}


void PanoImage::OnMouseLeftDown ( wxMouseEvent &event )
{
   CaptureMouse();
	isPanning = TRUE;
}

void PanoImage::OnMouseLeftUp ( wxMouseEvent & event )
{
	ReleaseMouse();
	isPanning = FALSE;
}

void PanoImage::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);
	PrepareDC(dc);
	dc.DrawBitmap( currentViewBmp, 0, 0);
}


void PanoImage::OnSize( wxSizeEvent &event )
{
	GetClientSize(&vw, &vh);
	view.SetViewSize ( wxSize(vw, vh) );
	currentViewBmp = view.GetView( PanoViewpoint(quality, hfov, yaw, pitch) );
}

void PanoImage::ShowOptionsDialog ( void )
{
	wxDialog *dlog = new wxDialog(NULL, -1, "Panorama Viewer Options", wxPoint(-1,-1));
	
	wxSlider slider(dlog, -1, 500, 0, 10000);

	dlog->ShowModal();

	accl = slider.GetValue()/10000.0;

	delete dlog;
}