/////////////////////////////////////////////////////////////////////////////
// Name:        AWTEL - "AWT Emulation Layer"
// Purpose:     Contrib. demo
// Author:      Aleksandras Gluchovas
// Modified by:
// Created:     2000/01/14
// RCS-ID:      
// Copyright:   (c) Aleksandras Gluchovas
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __AWT_EMUL_G__
#define __AWT_EMUL_G__

#include "wx/dynarray.h"
#include "wx/window.h"
#include "wx/dcmemory.h"
#include "wx/dc.h"

// minimal set of sourogate base-classes for AWT "compatibility"

typedef wxDC Graphics;   // ;D

typedef wxString String;

// eq. for java.awt.peer.ComponentPeer - corresponds to native window 
//										 wrapped by compoent class of AWT

typedef wxWindow ComponentPeer; 

#define LIGHTWEIGHT_COMPONENT_BGCOLOR wxColor(255,255,255)

// eq. for java.lang.Object

class Object : public wxObject
{
public:
	virtual String toString() { return ""; }
};

// eq. for java.awt.Dimension

class Dimension : public Object
{
public:
	Dimension() : width(0), height(0) {}

	Dimension( int w, int h ) : width( w ), height( h ) {}

public:
	int width;
	int height;
};

// eq. for java.awt.Point

class Point : public Object
{
public:
	Point() : x(0), y(0) {}

	Point( int _x, int _y ) : x(_x), y(_y) {}

public:
	int x, y;
};

// eq. for java.awt.Insets

class Insets : public Object
{
public:
	Insets() : top(0), left(0), bottom(0), right(0) {}

	Insets( int _top, int _left, int _bottom, int _right )

		: top(_top), left(_left), bottom(_bottom), right(_right ) {}

public:
	int top;
	int left;
	int bottom;
	int right;
};

class Image;

// eq. for interface java.awt.ImageObserver

class ImageObserver
{ 
public:
	virtual bool imageUpdate( Image& img,
						      int infoflags,
						      int x,
						      int y,
						      int width,
						      int height ) = 0;
};

// eq. for java.awt.Image (used for implementing double-buff. of lightweigh-comp)

class Image : public Object
{
public:
	Image();
	Image( int width, int height );
	~Image();

	virtual void setImageObserver( ImageObserver& observer );

	virtual int getHeight();
	virtual int getWidth();

	virtual Graphics& getGraphics();

	virtual void flush();

protected:
	wxMemoryDC* mpDC;
	wxBitmap*   mpBmp;
	ImageObserver* mpObserver;
};


// non-awt (wrapps up instance of wxPaintDC)

class CanvasImage : public Image, public ImageObserver
{ 
public:
	CanvasImage( Graphics& g, int width, int height );

	virtual bool imageUpdate( Image& img,
							  int infoflags,
							  int x,
							  int y,
							  int width,
							  int height);

protected:
	Graphics* mpGraphics;
	int mWidth, mHeight;
};

// eq. for java.awt.Component

class Component : public Object
{
public:
	Component(); // ctor creates light-weight componenet

	Component( ComponentPeer* pPeer ); // ctor creates heavy-weight component which 
									   // wrapps up the given native peer-window

	~Component();

	virtual void setLocation( Point at );
	virtual void setLocation( int x, int y );
	virtual Point getLocation();
	
	virtual void setSize( Dimension size );
	virtual void setSize( int width, int height );
	virtual Dimension getSize();

	virtual Dimension preferredSize();

	virtual int getX();
	virtual int getY();
	virtual int getWidth();
	virtual int getHeight();

	virtual void setVisible( bool b );
	virtual bool isVisible();

	virtual bool isOpaque();

	virtual void setName( String name );
	virtual String getName();

	virtual void paint( Graphics& g );
	virtual void repaint();

	// HACK:: FOR NOW:: added this to avoid need for IsKindOf(..) in Container class

	virtual void doLayout() {}

	virtual void setParent( Component* parent );
	virtual Component* getParent();

	virtual void dispose();

	virtual void invalidate() {}

	virtual Image* createImage(int width, int height);

	///////////////////////////////////////////////////////
	// three methods for setting/queying of heavy-weight //
	// control of wxWindow for this Component            //
	///////////////////////////////////////////////////////

	virtual void setPeer( ComponentPeer* pPeer );
	virtual ComponentPeer* getPeer();

	virtual bool isLightWeight() { return getPeer() == NULL; }

	virtual bool isDoubleBuffered();

	///////////////////////////////////////////////////////

	/*** not in AWT ***/

	virtual void setMinimumSize( Dimension size );
	virtual Dimension getMinimumSize();

	// hook for Container classes

	virtual bool hasHeavyWeightChildren() { return getPeer() == NULL; }

	// global settings (extentions beyond standard awt)

	enum LAYOUTING_PHASES { PHASE_0 = 0,
							PHASE_1 = 1 };

	static void setDoubleBufferingGlobally( bool on );
	static void setTwoPhaseLayoutingGlobally( bool on );

	virtual int getLayoutingPhase();
	virtual void setLayoutingPhase( int phase );

	virtual Dimension getPhase0Size();

	virtual bool twoPhaseLayoutingEnabled();

	// HACK:: hacky...

	virtual bool needsOwnerDraw();

protected:

	virtual void getAbsoluteLocation( Point& pos );
	virtual bool isTopLevel();
	virtual Component* getTopLevelComponent();

protected:
	Point      mLocation;
	Dimension  mSize;
	bool       mIsShown;
	String     mName;
	Component* mpParent;
	Dimension  mPrefPeerDim;

	ComponentPeer* mpPeer; // referes to actual instance of wxWindow, if any

	static bool mUseDoubleBufferGlobally;
	static bool mUseTwoPhaseLayoutingGlobally;

	static int mCurLayoutingPhase;
};

WX_DEFINE_ARRAY( Component*, ComponentListT );

class LayoutManager;

// eq. for java.awt.Container

class Container : public Component
{
public:
	Container();

	Container( ComponentPeer* pPeer ); // ctor creates container which wrapps
								       // up the given native peer-window

	~Container();

	virtual void add( Component* comp );
	virtual void add( Component* comp, Object* constraints );

	//////////////////////////////////////////////
	// two methods for embedding "heavy-weight" //
	// controls of wxWindows 					//
	//////////////////////////////////////////////

	virtual void add( ComponentPeer* pPeer );
	virtual void add( ComponentPeer* pPeer, Object* constraints );

	virtual bool hasHeavyWeightChildren();

	//////////////////////////////////////////////

	virtual int getComponentCount();
	virtual Component& getComponent( int n );

	virtual Dimension preferredSize();

	virtual void setLayout( LayoutManager* layout );
	virtual LayoutManager* getLayout();

	virtual void setInsets( Insets insets );
	virtual Insets getInsets();

	virtual void paint( Graphics& g );

	virtual void doLayout();

	// mtd. does should really belong to java.awt.Dialog/Frame classes

	virtual void pack();
	virtual void removeAll();

	virtual void dispose();

	virtual void repaint();

	virtual void invalidate();

public: // used interanlly

	virtual void DoRepaintComponent( Component* comp );

	virtual void getAbsoluteLocation( Point& pos );

protected:
	LayoutManager* mpLayout;
	ComponentListT mChildren;
	Insets         mInsets;

	Image*         mpOffSrcImage; // used in double-buffering mode
};

/*** base "interfaces" for custom layout managers ***/

// eq. for java.awt.LayoutManager

class LayoutManager : public Object
{
public:

	// Deprectated in JDK-1.1:
	// virtual void addLayoutComponent(String name,
    //                                Component* comp ) = 0;

	// HACK:: // HACK:: FOR NOW:: added this to avoid need for IsKindOf(..) in Container class

	virtual void addLayoutComponent( Component* comp ) {};


	virtual void removeLayoutComponent( Component* comp ) = 0;

	virtual Dimension preferredLayoutSize( Container* parent ) = 0;

	virtual Dimension minimumLayoutSize( Container* parent ) = 0;

	virtual void layoutContainer( Container* parent ) = 0;

	// HACK:: FOR NOW:: added this to avoid need for IsKindOf(..)

	virtual void addLayoutComponent( Component* comp, Object* constraints ) {}

	virtual void invalidateLayout( Container* target ) {};
};

// eq. for java.awt.LayoutManager2

class LayoutManager2 : public LayoutManager
{
public:

	virtual void addLayoutComponent( Component* comp, Object* constraints ) = 0;

	virtual Dimension maximumLayoutSize( Container* target ) = 0;

	virtual double getLayoutAlignmentX( Container* target ) = 0;
					
	virtual double getLayoutAlignmentY( Container* target ) = 0;

	//virtual void invalidateLayout( Container* target ) = 0;
};

#endif // __AWT_EMUL_G__