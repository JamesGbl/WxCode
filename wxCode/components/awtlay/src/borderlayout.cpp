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

#include "borderlayout.h"

/***** Implementation for class BorderLayout *****/

BorderLayoutConstraint BorderLayout::mCENTER( BorderLayout::_CENTER );
BorderLayoutConstraint BorderLayout::mEAST  ( BorderLayout::_EAST   );
BorderLayoutConstraint BorderLayout::mNORTH ( BorderLayout::_NORTH  );
BorderLayoutConstraint BorderLayout::mSOUTH ( BorderLayout::_SOUTH  );
BorderLayoutConstraint BorderLayout::mWEST  ( BorderLayout::_WEST   );

BorderLayoutConstraint BorderLayout::mAFTER_LAST_LINE   ( BorderLayout::_AFTER_LAST_LINE    );
BorderLayoutConstraint BorderLayout::mAFTER_LINE_ENDS   ( BorderLayout::_AFTER_LINE_ENDS    );
BorderLayoutConstraint BorderLayout::mBEFORE_FIRST_LINE ( BorderLayout::_BEFORE_FIRST_LINE  );
BorderLayoutConstraint BorderLayout::mBEFORE_LINE_BEGINS( BorderLayout::_BEFORE_LINE_BEGINS );

BorderLayoutConstraint* BorderLayout::CENTER = &BorderLayout::mCENTER;
BorderLayoutConstraint* BorderLayout::EAST   = &BorderLayout::mEAST;
BorderLayoutConstraint* BorderLayout::NORTH  = &BorderLayout::mNORTH;
BorderLayoutConstraint* BorderLayout::SOUTH  = &BorderLayout::mSOUTH;
BorderLayoutConstraint* BorderLayout::WEST   = &BorderLayout::mWEST;

BorderLayoutConstraint* BorderLayout::AFTER_LAST_LINE    = &BorderLayout::mAFTER_LAST_LINE;
BorderLayoutConstraint* BorderLayout::AFTER_LINE_ENDS    = &BorderLayout::mAFTER_LINE_ENDS;
BorderLayoutConstraint* BorderLayout::BEFORE_FIRST_LINE  = &BorderLayout::mBEFORE_FIRST_LINE;
BorderLayoutConstraint* BorderLayout::BEFORE_LINE_BEGINS = &BorderLayout::mBEFORE_LINE_BEGINS;

BorderLayout::BorderLayout()

	: mpCenterComp( NULL ),
	  mpEastComp  ( NULL ),
	  mpNorthComp ( NULL ),
	  mpSouthComp ( NULL ),
	  mpWestComp  ( NULL ),
	  mHgap( 0 ),
	  mVgap( 0 )

{}

BorderLayout::BorderLayout( int hgap, int vgap )

	: mpCenterComp( NULL ),
	  mpEastComp  ( NULL ),
	  mpNorthComp ( NULL ),
	  mpSouthComp ( NULL ),
	  mpWestComp  ( NULL ),
	  mHgap( hgap ),
	  mVgap( vgap )
{}

BorderLayout::~BorderLayout()
{}

// impl. of LayoutManager

void BorderLayout::removeLayoutComponent(Component* comp)
{
	// TBD::
}

Dimension BorderLayout::preferredLayoutSize(Container* parent)
{
	Dimension center = mpCenterComp ? mpCenterComp->preferredSize() : Dimension(0,0);
	Dimension east   = mpEastComp   ? mpEastComp->preferredSize()   : Dimension(0,0);
	Dimension north  = mpNorthComp  ? mpNorthComp->preferredSize()  : Dimension(0,0);
	Dimension south  = mpSouthComp  ? mpSouthComp->preferredSize()  : Dimension(0,0);
	Dimension west   = mpWestComp   ? mpWestComp->preferredSize()   : Dimension(0,0);

	Dimension pref;

	pref.width  = GetWidthSum( west.width, center.width, east.width, mHgap,
		 					   mpWestComp != NULL, mpCenterComp != NULL, mpEastComp != NULL	);
							  
	pref.height = GetWidthSum( north.height, center.height, south.height, mVgap,
							   mpNorthComp != NULL, mpCenterComp != NULL, mpSouthComp != NULL );

	pref.height = north.height + center.height + south.height;

	return pref;
}

Dimension BorderLayout::minimumLayoutSize( Container* parent )
{
	// FOR NOW::

	return preferredLayoutSize( parent );
}

void BorderLayout::layoutContainer( Container* parent )
{
	Dimension center = mpCenterComp ? mpCenterComp->preferredSize() : Dimension(0,0);
	Dimension east   = mpEastComp   ? mpEastComp->preferredSize()   : Dimension(0,0);
	Dimension north  = mpNorthComp  ? mpNorthComp->preferredSize()  : Dimension(0,0);
	Dimension south  = mpSouthComp  ? mpSouthComp->preferredSize()  : Dimension(0,0);
	Dimension west   = mpWestComp   ? mpWestComp->preferredSize()   : Dimension(0,0);

	int x = 0, y = 0;

	Dimension parentDim = parent->getSize();
	Insets    insets    = parent->getInsets();

	x += insets.left;
	y += insets.top;
	parentDim.width  -= insets.left + insets.right;
	parentDim.height -= insets.top  + insets.bottom;

	DistributeSizes( west.width,   center.width,  east.width,   parentDim.width,  mHgap,
					 mpWestComp != NULL, mpCenterComp != NULL, mpEastComp != NULL
				   );
	DistributeSizes( north.height, center.height, south.height, parentDim.height, mVgap,
					 mpNorthComp != NULL, mpCenterComp != NULL, mpSouthComp != NULL
				   );

	east.height = west.height = center.height;
	north.width = south.width = parentDim.width;

	if ( mpCenterComp )
	{
		mpCenterComp->setLocation( Point( x + west.width   + Hgap( mpWestComp  ), 
										  y + north.height + Vgap( mpNorthComp ) ) );
		mpCenterComp->setSize( center );
	}

	if ( mpEastComp )
	{
		mpEastComp->setLocation( Point( x + west.width + center.width + 
											Hgap( mpWestComp ) + Hgap( mpCenterComp ), 
										y + north.height + Vgap( mpNorthComp ) ) );
		mpEastComp->setSize( east );
	}
	
	if ( mpNorthComp )
	{
		mpNorthComp->setLocation( Point( x, y ) );
		mpNorthComp->setSize( north );
	}
	
	if ( mpSouthComp )

	{
		mpSouthComp->setLocation( Point( x, y + north.height + center.height + 
												Vgap( mpNorthComp ) + Vgap( mpCenterComp ) ) );
		mpSouthComp->setSize( south );
	}

	if ( mpWestComp )

	{
		mpWestComp->setLocation( Point( x, y + north.height + Vgap( mpNorthComp ) ) );
		mpWestComp->setSize( west );
	}
}

// impl. of LayoutManager2

void BorderLayout::addLayoutComponent( Component* comp, Object* constraints )
{
	BorderLayoutConstraint& c = *((BorderLayoutConstraint*)constraints);

	switch( c.mAlign )
	{
		case _CENTER : mpCenterComp = comp; break;
		case _EAST   : mpEastComp   = comp; break;
		case _NORTH  : mpNorthComp  = comp; break;
		case _SOUTH  : mpSouthComp  = comp; break;
		case _WEST   : mpWestComp   = comp; break;
		default: break;
	}
}

Dimension BorderLayout::maximumLayoutSize( Container* target )
{
	// FOR NOW::
	return Dimension(2000,2000);
}

double BorderLayout::getLayoutAlignmentX( Container* target )
{
	return 0;
}

double BorderLayout::getLayoutAlignmentY( Container* target )
{
	return 0;
}

void BorderLayout::invalidateLayout(Container* target)

{ /* nothing */ }

/*** protected methods ***/

void BorderLayout::DistributeSizes( int& left,  int& center, 
								    int& right, int totalWidth, int gap,
									bool hasLeft, bool hasCenter, bool hasRight )
{
	if ( hasLeft && hasCenter ) totalWidth  -= gap;
	if ( hasCenter && hasRight ) totalWidth -= gap;
	if ( hasRight && hasLeft && !hasCenter ) totalWidth -= gap;

	if ( left + center + right <= totalWidth )
	
		center = totalWidth - left - right;
	else
	{
		double sum = (double)( left + center + right );

		left   = (int) ( (double)totalWidth * ( (double)left   / sum ) );
		center = (int) ( (double)totalWidth * ( (double)center / sum ) );

		// compensate round-off errros at the expense of right element

		right  = totalWidth - left - center;
	}
}

int BorderLayout::GetWidthSum( int left, int center, int right, int gap, 
							   bool hasLeft, bool hasCenter, bool hasRight )
{
	int sum = left + center + right;

	if ( hasLeft && hasCenter ) sum += gap;
	if ( hasCenter && hasRight ) sum += gap;
	if ( hasRight && hasLeft && !hasCenter ) sum += gap;

	return sum;

}

int BorderLayout::Hgap( Component* pComp )
{
	if ( pComp ) return mHgap;
			else return 0;
}

int BorderLayout::Vgap( Component* pComp )
{
	if ( pComp ) return mVgap;
			else return 0;
}
