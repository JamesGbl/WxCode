/////////////////////////////////////////////////////////////////////////////
// Name:        No names yet.
// Purpose:     Contrib. demo
// Author:      Aleksandras Gluchovas
// Modified by:
// Created:     2000/01/14
// RCS-ID:      
// Copyright:   (c) Aleksandras Gluchovas
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "dynamic.cpp"
#pragma interface "dynamic.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <memory.h>
#include "awt_emul.h"

#include "flowlayout.h" // default layout manager for Contaienr

// HACK::... (is needed on my wxMSW-2.0.1)

#if defined(wxMSW) || defined(__WINDOWS__)

	#define NON_CLIENT_WIDTH  4
	#define NON_CLIENT_HEIGHT 20

#else

	#define NON_CLIENT_WIDTH  0
	#define NON_CLIENT_HEIGHT 0

#endif


/***** Implementation for class Image ******/

Image::Image()

	: mpBmp( NULL ),
	  mpDC( NULL ),
	  mpObserver( NULL )
{}

Image::Image( int width, int height )

	: mpBmp( NULL ),
	  mpDC( NULL ),
	  mpObserver( NULL )
{
	mpBmp = new wxBitmap( width, height );
	mpDC  = new wxMemoryDC();

	mpDC->SelectObject( *mpBmp );
}

Image::~Image()
{
	if ( mpDC )  
	{
		mpDC->SelectObject( wxNullBitmap );
		delete mpDC;
	}

	if ( mpBmp ) delete mpBmp;
}

int Image::getHeight()
{
	return mpBmp->GetHeight();
}

int Image::getWidth()
{
	return mpBmp->GetWidth();
}

Graphics& Image::getGraphics()
{
	return *mpDC;
}

void Image::setImageObserver( ImageObserver& observer )
{
	mpObserver = &observer;
}

void Image::flush()
{
	if ( mpObserver )

		mpObserver->imageUpdate( *this, 0, 0,0, getWidth(), getHeight() );
}

/***** Implementation for class CanvasImage ******/

CanvasImage::CanvasImage( Graphics& g, int width, int height )

	: mpGraphics( &g ),
	  mWidth( width ),
	  mHeight( height )
{}

bool CanvasImage::imageUpdate( Image& img,
							   int infoflags,
							   int x,
							   int y,
							   int width,
							   int height)
{
	mpGraphics->Blit( x,y, mWidth, mHeight, 
					  &img.getGraphics(), 0,0, wxCOPY, FALSE );

	return TRUE;
}


/***** Implementation for class Component ******/

#define DEFAUTL_COMP_WIDTH  120
#define DEFAUTL_COMP_HEIGHT 50

// global settigns

bool Component::mUseDoubleBufferGlobally      = TRUE;
bool Component::mUseTwoPhaseLayoutingGlobally = TRUE;
int  Component::mCurLayoutingPhase            = Component::PHASE_1;

Component::Component() // ctor creates light-weight componenet

	: mIsShown( TRUE ), // FIXME:: really?
	  mpParent( NULL ),
	  mSize( DEFAUTL_COMP_WIDTH, DEFAUTL_COMP_HEIGHT ),
	  mPrefPeerDim( -1, -1 ),
	  mpPeer( NULL )
	  
{}

Component::Component( ComponentPeer* pPeer )

	: mIsShown( TRUE ), // FIXME:: really?
	  mpParent( NULL ),
	  mSize( DEFAUTL_COMP_WIDTH, DEFAUTL_COMP_HEIGHT ),
	  mPrefPeerDim( -1, -1 ),
	  mpPeer( pPeer )
{}

Component::~Component()
{}

void Component::dispose()
{
	if ( isLightWeight() == FALSE )
	{
		getPeer()->Show( FALSE ); // avoid flicker
		getPeer()->Destroy(); // destroy assocaiated peer
	}
}

Dimension Component::preferredSize()
{
	if ( isLightWeight() )
	{
		if ( mPrefPeerDim.width == -1 )

			mPrefPeerDim = Dimension( DEFAUTL_COMP_WIDTH, DEFAUTL_COMP_HEIGHT );

		return mPrefPeerDim;
	}

	if ( mPrefPeerDim.width == -1 )
	{
		
		wxFont fnt = mpPeer->GetFont();
		wxWindow* p = mpPeer;
		// FIXME:: creating any window-dc for sub-windows
		//         on wxgtk causes "invalide window dc" assert,
		//         only for top-level window client-dc can be
		//         created at any time??

		while( p->GetParent() ) p = p->GetParent();
		wxClientDC dc(p);
		mpPeer->PrepareDC( dc );
		dc.SetFont( fnt );

		long w = 0, h = 0;

#define DEFAULT_TEXT_X_PAD 2
#define DEFAULT_TEXT_Y_PAD 2

		//////////////////////////////////////////////////////////////////////////////////////
		// FOR NOW:: instead of using new featured "GetMin/MaxSize()" of wxWindow			//
		//           we do some guesswork here... to get it compile on other wxWin versions //
		//////////////////////////////////////////////////////////////////////////////////////

		if ( mpPeer->IsKindOf( CLASSINFO(wxButton) ) )
		{
			wxString label = ((wxButton*)mpPeer)->GetLabel();
			dc.GetTextExtent( label, &w, &h );

			mPrefPeerDim =  Dimension( w+ 10, h + 10 );
		}
		else
		if ( mpPeer->IsKindOf( CLASSINFO(wxStaticText) ) )
		{
			wxString label = ((wxStaticText*)mpPeer)->GetLabel();
			dc.GetTextExtent( label, &w, &h );

			mPrefPeerDim =  Dimension( w + DEFAULT_TEXT_X_PAD, h + DEFAULT_TEXT_Y_PAD );
		}
		else
		if ( mpPeer->IsKindOf( CLASSINFO(wxTextCtrl) ) )
		{
			dc.GetTextExtent( "MINIMAL-TEXT", &w, &h );

			mPrefPeerDim =  Dimension( w + 5 + DEFAULT_TEXT_X_PAD, h + 5 + DEFAULT_TEXT_Y_PAD );
		}
		else
		if ( mpPeer->IsKindOf( CLASSINFO(wxRadioBox) ) )
		{
			wxString label = ((wxRadioBox*)mpPeer)->GetLabel(0);
			dc.GetTextExtent( label, &w, &h );

			mPrefPeerDim =  Dimension( w + 30 + DEFAULT_TEXT_X_PAD, h + DEFAULT_TEXT_Y_PAD );
		}
		else
		if ( mpPeer->IsKindOf( CLASSINFO(wxCheckBox) ) )
		{
			wxString label = ((wxCheckBox*)mpPeer)->GetLabel();
			dc.GetTextExtent( label, &w, &h );

			mPrefPeerDim =  Dimension( w + 20 + DEFAULT_TEXT_X_PAD, h + DEFAULT_TEXT_Y_PAD );
		}
		else
		if ( mpPeer->IsKindOf( CLASSINFO(wxListBox) ) )
		{
			dc.GetTextExtent( "MINIM-LIST", &w, &h );

			mPrefPeerDim =  Dimension( w + 20 + DEFAULT_TEXT_X_PAD, h*3 + DEFAULT_TEXT_Y_PAD );
		}
		else
		if ( mpPeer->IsKindOf( CLASSINFO(wxStaticBox) ) )
		{
			wxString label = ((wxStaticBox*)mpPeer)->GetName();
			dc.GetTextExtent( label, &w, &h );

			mPrefPeerDim =  Dimension( w + 20 + DEFAULT_TEXT_X_PAD, h + DEFAULT_TEXT_Y_PAD );
		}
		else
		{
			// FOR NOW:: the initial size of other heavy-weight components
			//           is treated as it's prefered size 

			// FOR NOW:: to real client size is taken only for
			//           top-level native-windows

			int width = 0, height = 0;

			if ( isTopLevel() )

				getPeer()->GetClientSize( &width, &height );
			else
				getPeer()->GetSize( &width, &height );

			mPrefPeerDim =  Dimension( (int)width, (int)height );
		}
	}
	

	return mPrefPeerDim;
}

void Component::setLocation( Point at )
{
	mLocation = at;

	if ( isLightWeight() == FALSE &&
		 getLayoutingPhase() == Component::PHASE_1 )
	{
		Point p = at;

		if ( getParent() )

			getParent()->getAbsoluteLocation( p );

		mpPeer->Move( p.x, p.y );
	}
}

void Component::setLocation( int x, int y )
{
	setLocation( Point( x, y ) );
}

Point Component::getLocation()
{
	if ( isLightWeight() == FALSE )
	{
		int x = 0, y = 0;
		getPeer()->GetPosition( &x, &y );

		return Point( x, y );
	}
	else
		return mLocation;
}

void Component::setSize( Dimension size )
{
	mSize = size;

	if ( isLightWeight() == FALSE && 
		 getLayoutingPhase() == Component::PHASE_1 )
	{
		// only in PHASE_1 sizes are actually set for heavy-weight components

		if ( isTopLevel() )

			// HACK::...

			getPeer()->SetClientSize( size.width  + NON_CLIENT_WIDTH, 
									  size.height + NON_CLIENT_HEIGHT );
		else
			getPeer()->SetSize( size.width, size.height );
	}
}

Component* Component::getTopLevelComponent()
{
	Component* pParent = this;

	while( pParent )
	{
		if ( pParent->getParent() == NULL ) break;

		pParent = pParent->getParent();
	}

	return pParent;
}

void Component::setSize( int width, int height )
{
	setSize( Dimension( width, height ) );
}

Dimension Component::getSize()
{
	if ( isLightWeight() == FALSE 
		//&& getLayoutingPhase() == Component::PHASE_1 
		)
	{
		int w = 0, h = 0;

		// FOR NOW:: to real client size is taken only for
		//           top-level native-windows

		if ( isTopLevel() )

			getPeer()->GetClientSize( &w, &h );
		else
			getPeer()->GetSize( &w, &h );

		return Dimension( (int)w, (int)h );
	}
	else
		return mSize;
}

int Component::getX()
{
	return getLocation().y;
}

int Component::getY()
{
	return getLocation().y;
}

int Component::getWidth()
{
	return getSize().width;
}

int Component::getHeight()
{
	return getSize().height;
}

void Component::setVisible( bool b )
{
	if ( isLightWeight() ) 
		
		mIsShown = b;
	else 
		getPeer()->Show( b );
}

bool Component::isVisible()
{
	if ( isLightWeight() ) return mIsShown;
					  else return getPeer()->IsShown();
}

bool Component::isOpaque()
{
	if ( isLightWeight() == FALSE ) return TRUE; // heavy-weights are always opaque
							   else return FALSE; 
}

void Component::setName( String name )
{
	mName = name;
}

String Component::getName()
{
	return mName;
}

void Component::paint( Graphics& g )
{
	if ( needsOwnerDraw() == FALSE ) return; 

	if ( isLightWeight() == FALSE )
	
		mSize = getSize();

	// default paiting code for leight-weight components

	wxBrush br( wxColour( 255, 255, 255 ), wxSOLID );
	wxPen pen ( wxColour( 255,   0,   0 ), 1, wxSOLID );

	g.SetBrush( br );
	g.SetPen( pen );

	g.DrawRectangle( 0,0, mSize.width, mSize.height );

	if ( mName != "" ) 
	{
		g.SetClippingRegion( 2,2, mSize.width-4, mSize.height-4 );

		long w = 0, h = 0;
		g.GetTextExtent( mName, &w, &h );

		g.DrawText( mName, ( mSize.width  - w ) / 2, 
						   ( mSize.height - h ) / 2  );

		g.DestroyClippingRegion();
	}
}

void Component::repaint()
{
	Container* pParent = (Container*)this->getParent();

	// find top-level heavy-weight container,
	// and request it to repaint this object

	while( pParent )
	{
		if ( pParent->isLightWeight() == FALSE )
		{
			pParent->DoRepaintComponent( this );

			break;
		}

		pParent = (Container*)pParent->getParent();
	}
}

void Component::setMinimumSize( Dimension size )
{
	// FOR NOW::

	mPrefPeerDim = size;
}

Dimension Component::getMinimumSize()
{
	// FOR NOW::...

	return Component::preferredSize();
}

void Component::setDoubleBufferingGlobally( bool on )
{
	Component::mUseDoubleBufferGlobally = on;
}

void Component::setTwoPhaseLayoutingGlobally( bool on )
{
	Component::mUseTwoPhaseLayoutingGlobally = on;
}

int  Component::getLayoutingPhase()
{
	return Component::mCurLayoutingPhase;
}

void Component::setLayoutingPhase( int phase )
{
	Component::mCurLayoutingPhase = phase;
}

Dimension Component::getPhase0Size()
{
	return mSize; // TBD:: comments
}

bool Component::twoPhaseLayoutingEnabled()
{
	return Component::mUseTwoPhaseLayoutingGlobally;
}

void Component::getAbsoluteLocation( Point& pos )
{
	// convert relative coordinates to
	// absolute with respect to the first
	// heavy-weight parent container (Swing's method)

	Point myPos = getLocation();

	pos.x += myPos.x;
	pos.y += myPos.y;

	if ( this->getParent() )

		getParent()->getAbsoluteLocation( pos );
}

bool Component::isTopLevel()
{
	bool sre = mpPeer->IsKindOf( CLASSINFO(wxDialog) );

	if ( mpPeer != NULL && 
		 ( mpPeer->GetParent() == NULL ||
		   mpPeer->IsKindOf( CLASSINFO(wxDialog) ) 
		 )
	   )

	    return TRUE;
	else
		return FALSE;
}

void Component::setParent( Component* parent )
{
	mpParent = parent;
}

Component* Component::getParent()
{
	return mpParent;
}

Image* Component::createImage(int width, int height)
{
	return new Image( width, height );
}

void Component::setPeer( ComponentPeer* pPeer )
{
	mpPeer = pPeer;
}

ComponentPeer* Component::getPeer()
{
	return mpPeer;
}

bool Component::isDoubleBuffered()
{
	// FOR NOW:: no extra bool per each component

	return mUseDoubleBufferGlobally;
}

bool Component::needsOwnerDraw()
{
	wxColor bg = getTopLevelComponent()->getPeer()->GetBackgroundColour();

	int r = bg.Red(), g = bg.Green(), b = bg.Blue();

	// FOR NOW:: the need for owner-draw is triggered by detecting
	//           white background of the heavy-weight component

	return r == 255 && g == 255 && b == 255;
}

/*** helper class used for delegating events from window to the corresponding container ***/

class PeerEventSpy : public wxEvtHandler
{
public:
	PeerEventSpy() {}
	PeerEventSpy( Container* pContainer, wxWindow* pWnd );

	void OnSize( wxSizeEvent& evt );
	void OnPaint( wxPaintEvent& evt );
	void OnErase( wxEraseEvent& evt );

	DECLARE_EVENT_TABLE()
	DECLARE_DYNAMIC_CLASS( PeerEventSpy )

protected:
	Container* mpContainer;
	wxWindow*  mpWnd;
};

IMPLEMENT_DYNAMIC_CLASS( PeerEventSpy, wxEvtHandler )

BEGIN_EVENT_TABLE( PeerEventSpy, wxEvtHandler )

	EVT_SIZE ( PeerEventSpy::OnSize  )
	EVT_PAINT( PeerEventSpy::OnPaint )
	EVT_ERASE_BACKGROUND( PeerEventSpy::OnErase )

END_EVENT_TABLE()

PeerEventSpy::PeerEventSpy( Container* pContainer, wxWindow* pWnd )

	: wxEvtHandler(),
	  mpContainer( pContainer ), 
	  mpWnd( pWnd )
{}

void PeerEventSpy::OnSize( wxSizeEvent& evt )
{
	mpContainer->doLayout();

	// FIXME:: how to avoid (sometimes) unnecessary refreshes ??

	mpContainer->getPeer()->Refresh();

	evt.Skip();
}

void PeerEventSpy::OnPaint( wxPaintEvent& evt )
{
	wxPaintDC dc( mpWnd );

	dc.SetFont( mpWnd->GetFont() );

	mpContainer->paint( dc );

//	evt.Skip();
}

void PeerEventSpy::OnErase( wxEraseEvent& evt )
{
	if ( mpContainer->needsOwnerDraw() == FALSE )

		evt.Skip();
}

/***** Implementation for class Container ******/

Container::Container()

	: Component(),
	  mpLayout( NULL ),
	  mpOffSrcImage( NULL )
{}

Container::Container( ComponentPeer* pPeer )

	: Component( pPeer ),
	  mpLayout( NULL ),
	  mpOffSrcImage( NULL )
{
	if ( isTopLevel() )
	{
		// FOR NOW:: size and paint events are delegated
		//           only from top-level native windows

		PeerEventSpy* pSpy = new PeerEventSpy( this, pPeer );

		//pPeer->GetEventHandler()->SetNextHandler( pSpy );

		pPeer->PushEventHandler( pSpy );

		// FXIME:: TBD:: removing and releaseing memory of PeerEventSpy
	}
}

Container::~Container()
{
	// FIXME:: TBD:: removing container when corresponding peer-window is destroyed

	// TBD:: remove children...

	// FOR NOW:: layout-objects cannot be shared among different containers

	if ( mpLayout ) delete mpLayout;

	if ( mpOffSrcImage ) delete mpOffSrcImage;
}

void Container::add( Component* comp )
{
	mChildren.Add( comp );

	comp->setParent( this );

	if ( mpLayout ) mpLayout->addLayoutComponent( comp );
}

void Container::add( Component* comp, Object* constraints )
{
	mChildren.Add( comp );

	comp->setParent( this );

	if ( mpLayout ) mpLayout->addLayoutComponent( comp, constraints );
}

bool Container::hasHeavyWeightChildren()
{
	for( size_t i = 0; i != mChildren.Count(); ++i )
	
		if ( mChildren[i]->hasHeavyWeightChildren() ) return TRUE;

	return FALSE;
}

void Container::add( ComponentPeer* pPeer )
{
	add( new Component( pPeer ) );
}

void Container::add( ComponentPeer* pPeer, Object* constraints )
{
	add( new Component( pPeer ), constraints );
}

int Container::getComponentCount()
{
	return mChildren.Count();
}

Component& Container::getComponent( int n )
{
	return *mChildren[n];
}

Dimension Container::preferredSize()
{
	if ( mpLayout == NULL ) 
		
		// default layout manager for java.awt.Contaienr
		setLayout( new FlowLayout() );

	if ( mpLayout ) 
		
		return mpLayout->preferredLayoutSize( this );
	else 
		return Component::preferredSize();
}

void Container::setLayout( LayoutManager* layout )
{
	mpLayout = layout;
}

LayoutManager* Container::getLayout()
{
	return mpLayout;
}

void Container::paint( Graphics& g )
{
	if ( !needsOwnerDraw() )

		  return;

	Point thisPos(0,0);
	getAbsoluteLocation(thisPos);

	Dimension dim = getSize();

	CanvasImage observer( g, dim.width, dim.height );

	Graphics* canvas = &g;

	if ( isDoubleBuffered() && getParent() == NULL )
	{

		if ( mpOffSrcImage && 
			 mpOffSrcImage->getWidth()  >= dim.width && 
			 mpOffSrcImage->getHeight() >= dim.height )

		 	 canvas = &mpOffSrcImage->getGraphics();
		else
		{
			if ( mpOffSrcImage ) delete mpOffSrcImage;

			mpOffSrcImage = createImage( dim.width, dim.height );

			canvas = &mpOffSrcImage->getGraphics();
		}

		mpOffSrcImage->setImageObserver( observer );
	}

	canvas->SetDeviceOrigin( thisPos.x, thisPos.y );
	Component::paint( *canvas );

	// draw this container and it's children

	for( size_t i = 0; i != mChildren.Count(); ++i )
	{
		Component& c = *mChildren[i];

		if ( c.needsOwnerDraw() && c.isVisible() )
		{
			Point pos = c.getLocation();
			Dimension dim = c.getSize();
				
			canvas->SetDeviceOrigin( thisPos.x + pos.x, thisPos.y + pos.y );

			canvas->SetClippingRegion( 0,0,
									   dim.width, dim.height );

			c.paint( *canvas );

			canvas->DestroyClippingRegion();

		}
	}

	if ( isDoubleBuffered() && getParent() == NULL )
	{
		canvas->SetDeviceOrigin( 0,0 );

		mpOffSrcImage->flush();
	}
}

void Container::doLayout()
{
	if ( mpLayout == NULL ) 
		
		// default layout manager for java.awt.Contaienr
		setLayout( new FlowLayout() );

	if ( twoPhaseLayoutingEnabled() && getParent() == NULL )
	{
		for( int n = Component::PHASE_0; n <= Component::PHASE_1; ++n )
		{
			if ( n == Component::PHASE_1 ) invalidate(); // TBD:: comment

			setLayoutingPhase( n );

			mpLayout->layoutContainer( this );

			for( size_t i = 0; i != mChildren.Count(); ++i )

				mChildren[i]->doLayout();
		}
	}
	else
	{
		mpLayout->layoutContainer( this );

		for( size_t i = 0; i != mChildren.Count(); ++i )

			mChildren[i]->doLayout();
	}
}

void Container::setInsets( Insets insets )
{
	mInsets = insets;
}

Insets Container::getInsets()
{
	return mInsets;
}

void Container::pack()
{
	Container* pParent = this;

	// find top-level heavy-weight container and 
	// set it's actual size to match it's prefered one

	while( pParent )
	{
		if ( pParent->isLightWeight() == FALSE )
		{
			Dimension prefDim = pParent->preferredSize();
			
			// HACK::... needed at least on v.2.0.1

			pParent->getPeer()->SetClientSize( prefDim.width  + NON_CLIENT_WIDTH, 
											   prefDim.height + NON_CLIENT_HEIGHT );

			break;
		}

		pParent = (Container*)pParent->getParent();
	}
}

void Container::removeAll()
{
	for( size_t i = 0; i != mChildren.Count(); ++i )
	{
		mChildren[i]->dispose();
		delete mChildren[i];
	}

	mChildren.Clear();
}

void Container::dispose()
{
	removeAll();

	if ( isLightWeight() == FALSE && getPeer()->GetParent() == NULL )

		getPeer()->PopEventHandler( TRUE ); // remove attached instance
										    // of PeerEventSpy (only for top-level windows)

	// TBD:: should we destroy associated top-level peer for 
	//       heavy-weight container here?

	// HACK::

	if ( isLightWeight() == FALSE && getPeer()->GetParent() != NULL )

		getPeer()->Destroy();
}

void Container::repaint()
{
	Container* pParent = this;

	// find top-level heavy-weight container,
	// and request it to repaint this object

	while( pParent )
	{
		if ( pParent->isLightWeight() == FALSE )
		{
			pParent->DoRepaintComponent( this );

			break;
		}

		pParent = (Container*)pParent->getParent();
	}
}

void Container::invalidate()
{
	if ( mpLayout ) mpLayout->invalidateLayout( this );

	for( size_t i = 0; i != mChildren.Count(); ++i )

		mChildren[i]->invalidate();
}

void Container::DoRepaintComponent( Component* comp )
{
	wxClientDC dc( getPeer() );

	comp->paint( dc );
}

void Container::getAbsoluteLocation( Point& pos )
{
	if ( isLightWeight() == FALSE ) return;

	Point myPos = getLocation();

	pos.x += myPos.x;
	pos.y += myPos.y;
}
