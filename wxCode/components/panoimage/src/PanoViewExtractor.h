#ifndef PANO_VEW_EXTRACTOR_HEADER
#define PANO_VEW_EXTRACTOR_HEADER

#include "wx/wx.h"
#include "wx/image.h"
#include "wx/thread.h"
#include "panorama.h"

#include <vector>

// A viewpoint is a view from a certain angle at a certain quality
class PanoViewpoint;
class PanoViewpoint
{
public:
	PanoViewpoint(
		const int &q = 50,
		const double &h = 70,
		const double &y = 0,
		const double &p = 0,
		const double &r = 0);
	PanoViewpoint( const PanoViewpoint &v );

	int GetQuality(void) const;
	double GetHfov(void) const;
	double GetYaw(void) const;
	double GetPitch(void) const;
	double GetRoll(void) const;
	PanoViewpoint GetViewpoint(void);

	void SetViewpoint ( const PanoViewpoint &v );
	void SetQuality ( const int &q = 50 );
	void SetHfov ( const double &h = 70 );
	void SetYaw ( const double &y = 0 );
	void SetPitch ( const double &p = 0 );
	void SetRoll ( const double &r = 0 );
	
	int operator==( const PanoViewpoint &right) const;
private:
	int quality;
	double hfov, yaw, pitch, roll;
};


// A view is a viewpoint of an image (of a certain size)
class PanoView : public PanoViewpoint
{
public:
	PanoView(
		const wxBitmap &bmp = wxBitmap(200,200),
		const PanoViewpoint &v = PanoViewpoint()
		);
	PanoView ( const PanoView &v );

	wxBitmap GetView(void) const;
	wxSize GetViewSize(void) const;
	
	void SetView( const wxBitmap &b = wxBitmap(200,200) );
	void SetViewSize ( const wxSize &sz );

	int operator==( PanoView &right );
private:
	wxBitmap view;
};


// A thread workspace is a pano view with some thread synchronization data
class ExtractorSlaveWorkspace: public PanoView
{
public:
	ExtractorSlaveWorkspace ( Image * const s = NULL, const PanoView &v = PanoView() );

	Image *GetSourcePano(void) const;
	bool IsBusy(void) const;

	void SetSourcePano ( Image * const s = NULL );
	void SetBusy(const bool b = TRUE);

	int operator==(const PanoViewpoint &right) const;
private:
	Image *src;
	bool isBusy;
};


class PanoViewExtractorSlave: public wxThread
{
public:
	PanoViewExtractorSlave( ExtractorSlaveWorkspace * const w);
	~PanoViewExtractorSlave();

	void * Entry();
	void OnExit();
private:
	ExtractorSlaveWorkspace *workspace;
};


typedef std::vector<PanoViewExtractorSlave *> PanoSlave;
typedef std::vector<ExtractorSlaveWorkspace *> SharedData;

class PanoViewExtractor
{
public:
	PanoViewExtractor( const wxImage &img = wxImage(200,200) );
	~PanoViewExtractor();

	// Service functions
	void SetViewSize ( const wxSize &sz = wxSize(200,200) );
	wxSize GetViewSize ( void ) const;
	wxBitmap GetView ( const PanoViewpoint &v = PanoViewpoint() );
	void SetPano ( const wxImage &img = wxImage(200,200) );
private:
	PanoSlave slaves;
	SharedData s_work;
	Image s_src;
	int xThreads;
};

#endif /* PANO_VEW_EXTRACTOR_HEADER */