#ifndef PANO_IMAGE_HEADER
#define PANO_IMAGE_HEADER

#include "wx/wx.h"
#include "wx/image.h"
#include "PanoViewExtractor.h"

class PanoImage: public wxPanel
{
public:
	PanoImage ( wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = "panel" );
	~PanoImage();
	void SetPano ( const wxImage &img );

	// Preferences and options
	void ShowOptionsDialog(void);

	// Event handlers
	void OnMouseLeftDown ( wxMouseEvent &event );
	void OnMouseLeftUp ( wxMouseEvent &event );
	void OnMouseMove ( wxMouseEvent &event );
	void OnPaint(wxPaintEvent &event);
	void OnKeyDown(wxKeyEvent &event);
	void OnKeyUp( wxKeyEvent &event);
	void OnIdle ( wxIdleEvent &event );
	void OnSize ( wxSizeEvent &event );

private:
	PanoViewExtractor view;

	bool isPanning, isZoomingIn, isZoomingOut;
	int mx, my;
	double hfov, yaw, pitch, roll;
	int vw, vh;
	int quality;
	wxBitmap currentViewBmp;

	// Mouse speed options
	float accl;

	DECLARE_EVENT_TABLE()
};


#endif /* PANO_IMAGE_HEADER */