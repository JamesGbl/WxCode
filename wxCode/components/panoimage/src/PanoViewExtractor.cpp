#include "wx/wx.h"
#include "wx/image.h"

#ifdef new
#undef new
#endif

#include "panorama.h"
#include "filter.h"
#include "PanoViewExtractor.h"

void PV_ExtractStill(TrformStr *TrPtr);
void PrintError (char *fmt, ...);
int SetUpAtan();
int SetUpSqrt();
int SetUpMweights();


//=================================================================================================

PanoViewpoint::PanoViewpoint(
		const int &q,
		const double &h,
		const double &y,
		const double &p,
		const double &r)
{
	SetQuality(q);
	SetHfov(h);
	SetYaw(y);
	SetPitch(p);
	SetRoll(r);
}



PanoViewpoint::PanoViewpoint( const PanoViewpoint &v )
{
	SetViewpoint(v);
}




int PanoViewpoint::GetQuality(void) const
{
	return quality;
}



double PanoViewpoint::GetHfov(void) const
{
	return hfov;
}



double PanoViewpoint::GetYaw(void) const
{
	return yaw;
}



double PanoViewpoint::GetPitch(void) const
{
	return pitch;
}



double PanoViewpoint::GetRoll(void) const
{
	return roll;
}


PanoViewpoint PanoViewpoint::GetViewpoint(void)
{
	return PanoViewpoint(quality, hfov, yaw, pitch, roll);
}



void PanoViewpoint::SetViewpoint ( const PanoViewpoint &v )
{
	quality = v.quality;
	hfov = v.hfov;
	yaw = v.yaw;
	pitch = v.pitch;
	roll = v.roll;
}



void PanoViewpoint::SetQuality ( const int &q)
{
	quality = q;
	if ( quality < 0) quality = 0;
	if ( quality > 100 ) quality = 100;
}



void PanoViewpoint::SetHfov ( const double &h)
{
	hfov = h;
	NORM_ANGLE(hfov);
}



void PanoViewpoint::SetYaw ( const double &y)
{
	yaw = y;
	NORM_ANGLE(yaw);
}



void PanoViewpoint::SetPitch ( const double &p)
{
	pitch = p;
	if ( pitch > 90 ) pitch = 90;
	if ( pitch < -90 ) pitch = -90;
}



void PanoViewpoint::SetRoll ( const double &r)
{
	roll = r;
	NORM_ANGLE(roll);
}



int PanoViewpoint::operator==( const PanoViewpoint &right) const
{
	return ( quality == right.quality && hfov == right.hfov && yaw == right.yaw && pitch == right.pitch && roll == right.roll );
}



//=================================================================================================

PanoView::PanoView(
		const wxBitmap &bmp,
		const PanoViewpoint &v
		) : PanoViewpoint(v)
{
	view = bmp;
}



PanoView::PanoView ( const PanoView &v )
{
}



wxBitmap PanoView::GetView(void) const
{
	return view;
}



wxSize PanoView::GetViewSize(void) const
{
	return wxSize ( view.GetWidth(), view.GetHeight() );
}



void PanoView::SetView( const wxBitmap &b)
{
	view = b;
}



void PanoView::SetViewSize ( const wxSize &sz )
{
	view.Create( sz.GetWidth(), sz.GetHeight() );
}



int PanoView::operator==( PanoView &right )
{
	return ( view == right.view && PanoViewpoint::operator==(right) );
}



//=================================================================================================

ExtractorSlaveWorkspace::ExtractorSlaveWorkspace ( Image * const s, const PanoView &v )
{
	src = s;
	SetBusy(FALSE);
}



Image * ExtractorSlaveWorkspace::GetSourcePano(void) const
{
	return src;
}



void ExtractorSlaveWorkspace::SetSourcePano ( Image * const s)
{
	src = s;
}



bool ExtractorSlaveWorkspace::IsBusy(void) const
{
	return isBusy;
}



void ExtractorSlaveWorkspace::SetBusy(const bool b)
{
	isBusy = b;
}


int ExtractorSlaveWorkspace::operator==(const PanoViewpoint &right) const
{
	return PanoViewpoint::operator==(right);
}



//=================================================================================================

PanoViewExtractorSlave::PanoViewExtractorSlave( ExtractorSlaveWorkspace * const w)
{
	// Make sure our workspace is somewhat valid
	assert ( w != NULL );
	
	// Initialize our workspace pointer
	workspace = w;
}



PanoViewExtractorSlave::~PanoViewExtractorSlave()
{
}



void * PanoViewExtractorSlave::Entry()
{
	Image view;
	TrformStr tr;

	// Init transformation data
   tr.dest = &view;

	// Partially init our intermediate view (the rest will be handled when a view is extracted)
	view.bitsPerPixel = 32;
	view.dataformat = _RGB;
	view.format = _panorama;
	view.name[0] = '\0';    // no image name
	view.data = new unsigned char *;
	if ( view.data == NULL )
	{
		PrintError("Error allocating view transformation pointer. Aborted.");
		exit(1);
	}
	*view.data = NULL;
	
	while ( !TestDestroy() )
	{
		// Look for our sign to start extraction --
		// That is, the IsBusy() flag is set
		if ( workspace->IsBusy() )
		{
			// Make sure we're using the correct source and quality
		   tr.src = workspace->GetSourcePano();
		   switch ( workspace->GetQuality() )
		   {
		   	case 0:
		   		tr.interpolator = _nn;
		   		break;
		   	default:
		   		tr.interpolator = _bilinear;
		   }

		   // Make sure our view is calculated and allocated correctly
	   	if ( workspace->GetViewSize().GetWidth() != view.width
	   		|| workspace->GetViewSize().GetHeight() != view.height
	   		|| *view.data == NULL )
			{
				view.width = workspace->GetViewSize().GetWidth();
				view.height = workspace->GetViewSize().GetHeight();
				view.bytesPerLine = view.width * view.bitsPerPixel/8;
				view.dataSize = view.bytesPerLine * view.height;
				if ( *view.data != NULL) delete [] *view.data;
				*view.data = new unsigned char [ view.dataSize ];
				if ( *view.data == NULL )
				{
					PrintError("Error allocating view data. Aborted.");
					exit(1);
				}
			}

			// Set the view angles
			view.hfov = workspace->GetHfov();
			view.yaw = workspace->GetYaw();
			view.pitch = workspace->GetPitch();
			view.roll = workspace->GetRoll();

			// Perform the extraction
			PV_ExtractStill(&tr);

			// Convert the 4-byte view to the 3-byte wxImage
			wxImage img((int) view.width, (int) view.height);
			int i;
			unsigned char *j = img.GetData();
			unsigned char *d = view.data[0];
			for ( i = 0; i < view.dataSize; i++)
			{
			   if ( i % 4 != 0 ) *(j++) = d[i];
			}

			// Load the new bitmap into the workspace
			workspace->SetView(img.ConvertToBitmap());

			// We're not busy anymore
			workspace->SetBusy(FALSE);
		}

		// Give the rest of the thread time slice to the system
		Yield();
	}

	// We're all done, free the view data
	if ( view.data != NULL )
	{
		if ( *view.data != NULL) delete [] *view.data;
		delete view.data;
	}

	return NULL;
}



void PanoViewExtractorSlave::OnExit()
{
}

//=================================================================================================

PanoViewExtractor::PanoViewExtractor( const wxImage &img )
{
	// Initialize threads, then pause them for later use
	int i;
	xThreads = 2;
	ExtractorSlaveWorkspace *w;
	PanoViewExtractorSlave *s;
	for ( i = 0; i < xThreads; ++i )
	{
		// Create the extractor workspace
		w = new ExtractorSlaveWorkspace ( &s_src );
		if ( w == NULL )
		{
			PrintError("Error allocating workspace for slave %d. Aborting", i);
			exit(1);
		}
		s_work.push_back ( w );
		// Create the slave, and give it the pointer to its workspace
		s = new PanoViewExtractorSlave ( s_work.back() );
		if ( s == NULL )
		{
			PrintError("Error allocating thread for slave %d. Aborting", i);
			exit(1);
		}
		slaves.push_back ( s );
		// Create the slave's thread and run it
		if ( slaves.back()->Create() != wxTHREAD_NO_ERROR )
		{
			PrintError("Error creating extraction thread %d. Aborting.", i);
			exit(1);
		}
		if ( slaves.back()->Run() != wxTHREAD_NO_ERROR )
		{
			PrintError("Error running extraction thread %d. Aborting.", i);
			exit(1);
		}
/*		if ( slaves.back()->Pause() != wxTHREAD_NO_ERROR )
		{
			PrintError("Error pausing extraction thread %d. Aborting.", i);
			exit(1);
		}
*/
	}

	// Partially init our source pano (the rest will be handled when a pano is set)
	s_src.bitsPerPixel = 32;
	s_src.dataformat = _RGB;
	s_src.format = _equirectangular;
	s_src.hfov = 360;
	s_src.yaw = 0;
	s_src.pitch = 0;
	s_src.roll = 0;
	s_src.name[0] = '\0';    // no image name
	s_src.data = new unsigned char *;
	if ( s_src.data == NULL )
	{
		PrintError("Error allocating source data pointer in extraction constructor. Aborting.");
		exit(1);
	}
	if ( s_src.data != NULL ) *s_src.data = NULL;
	SetPano(img);

	// Init our pano library
   SetUpAtan();
   SetUpSqrt();
   SetUpMweights();
}



PanoViewExtractor::~PanoViewExtractor()
{
	// Kill and free all slaves and workspaces (slaves kill themselves)
	int i;
	for ( i = 0; i < xThreads; i++ )
	{
		delete s_work[i];
	}

	// Free the source pano
	if ( s_src.data != NULL )
	{
		if ( *s_src.data != NULL ) delete [] *s_src.data;
		delete s_src.data;
	}
}



void PanoViewExtractor::SetViewSize ( const wxSize &sz )
{
	// The size of the view has changed, so re-compute all slave sizes using same
	// panorama viewpoints. Wait until they're not busy. The worst-case time for
	// waiting for each slave to finish before changing it, rather than moving on
	// to a free one and returning later to the previously busy slave should be
	// negligable.
	int i;
	for(i = 0; i < xThreads; i++)
	{
		// Wait until it's not busy, then change size and re-compute
		while ( s_work[i]->IsBusy() );
		s_work[i]->SetViewSize(sz);
		s_work[i]->SetBusy(TRUE);
	}
}



wxSize PanoViewExtractor::GetViewSize ( void ) const
{
	// Just return the size of the first shared data element. They're all the same.
	// ( see SetViewSize() )
	return s_work[0]->GetViewSize();
}



wxBitmap PanoViewExtractor::GetView ( const PanoViewpoint &v )
{
	// Multithreaded predictive get with caching
	
	static PanoViewpoint last = v, predict;
	static wxBitmap res;
	static int i, j;
	
	//	PrintError("This: %d, %f, %f, %f, %f | Predicted: %d, %f, %f, %f, %f",
	//		v.GetQuality(), v.GetHfov(), v.GetYaw(), v.GetPitch(), v.GetRoll(),
	//		predict.GetQuality(), predict.GetHfov(), predict.GetYaw(), predict.GetPitch(), predict.GetRoll() );

	// Predict the next viewpoint and start calculating on an empty thread
	// Predict by assuming the change between old and new will happen again at all angles
	predict.SetQuality ( v.GetQuality() );
	predict.SetHfov	( v.GetHfov()	+ ( v.GetHfov()	- last.GetHfov()	) );
	predict.SetYaw		( v.GetYaw()	+ ( v.GetYaw()		- last.GetYaw()	) );
	predict.SetPitch	( v.GetPitch()	+ ( v.GetPitch()	- last.GetPitch()	) );
	predict.SetRoll	( v.GetRoll()	+ ( v.GetRoll()	- last.GetRoll()	) );
	last = v;

	// Step through our list, seeing if we predicted the viewpoint desired
	for(i = 0; i < xThreads; i++ )
	{
		// If we have the viewpoint, good. We have it.
		if ( *s_work[i] == v )
		{
			// wxMessageBox("Found cached");
			break;
		}
	}

	// If we did not find the view we needed, compute it now on an empty thread
	if ( i >= xThreads )
	{
		do
		{
			for(i = 0; i < xThreads; i++ ) if ( !s_work[i]->IsBusy() ) break;
		} while (i >= xThreads );
		s_work[i]->SetViewpoint(v);
		s_work[i]->SetBusy();
	}

	// Find an empty thread on which to place the prediction.
	// If the thread contains the view we need, save it first
	for ( j = 0; j < xThreads; j++ )
	{
		if ( !s_work[j]->IsBusy() )
		{
			if ( i == j ) res = s_work[j]->GetView();
			s_work[j]->SetViewpoint(predict);
			s_work[j]->SetBusy();
//			PrintError("Predict put on thread %d", j);
			break;
		}
	}

	// Now, either the prediction was run on the original thread, which finished in time,
	// (j == i), or it was run on a different thread (j != i), which means the view we need
	// now may not be done yet and we must wait for it
	if ( j == i )
	{
		return res;
	} else {
		while ( s_work[i]->IsBusy() );
		return s_work[i]->GetView();
	}
	

/*	
	// Simple implementation -- single thread
	// Give the first thread the new viewpoint to compute, then start it
//	slaves.front()->Resume();
	s_work.front()->SetViewpoint(v);
	s_work.front()->SetBusy(TRUE);
	
	// Wait until the first thread is done computing, then return the result
	while ( s_work.front()->IsBusy() );
//	slaves.front()->Pause();
	return s_work.front()->GetView();
*/
}



void PanoViewExtractor::SetPano ( const wxImage &img )
{
	// Wait for all threads to be done using the source
	SharedData::iterator w;
	for(w = s_work.begin(); w != s_work.end(); ++w)
	{
		while ( (*w)->IsBusy() );
	}
		
	// Resize and reallocate the source pano
	s_src.width = img.GetWidth();
	s_src.height = img.GetHeight();
	s_src.bytesPerLine = s_src.width * s_src.bitsPerPixel/8;
	s_src.dataSize = s_src.bytesPerLine * s_src.height;
	if ( *s_src.data != NULL ) delete [] *s_src.data;
	*s_src.data = new unsigned char[s_src.dataSize];
	if ( *s_src.data == NULL )
	{
		PrintError("Error allocating extraction source data space. Aborting.");
		exit(1);
	}

   // Copy image data and do the 24- to 32-bit conversion by inserting a 0xFF before every set of 3 bytes
   int toIdx;
   unsigned char *i = img.GetData();
   for ( toIdx = 0; toIdx < s_src.dataSize; toIdx++)
   {
      if ( toIdx % 4 == 0 ) s_src.data[0][toIdx] = 0xFF;
         else s_src.data[0][toIdx] = *(i++);
   }	
}