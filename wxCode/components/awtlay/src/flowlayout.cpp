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

#include "flowlayout.h"

/***** Implementation for class FlowLayout *****/

FlowLayout::FlowLayout()

	: mAlign( CENTER ), mHgap( 5 ), mVgap( 5 )
{}

FlowLayout::FlowLayout( int align )

	: mAlign( align ), mHgap( 5 ), mVgap( 5 )
{}

FlowLayout::FlowLayout( int align, int hgap, int vgap )

	: mAlign( align ), mHgap( hgap ), mVgap( vgap )
{}

int FlowLayout::getAlignment()
{
	return mAlign;
}

int FlowLayout::getHgap()
{
	return mHgap;
}

int FlowLayout::getVgap()
{
	return mVgap;
}

void FlowLayout::setAlignment( int align )
{
	mAlign = align;
}

void FlowLayout::setHgap( int hgap )
{
	mHgap = hgap;
}

void FlowLayout::setVgap( int vgap )
{
	mVgap = vgap;
}

// impl. of LayoutManager interface

void FlowLayout::addLayoutComponent( String name,
                                     Component* comp )
{ /* nothing */ }

void FlowLayout::removeLayoutComponent( Component* comp )

{ /* nothing */ }

Dimension FlowLayout::preferredLayoutSize( Container* parent )
{
	if ( parent->twoPhaseLayoutingEnabled() &&
		 parent->getLayoutingPhase() == Component::PHASE_1 )

		 return mPrefDimOfPhase1; // TBD:: comments...


	int width = 0;
	int maxHeight = 0;

	int cnt = parent->getComponentCount();

	for( int i = 0; i != cnt; ++i )
	{
		if ( i != 0 ) width += mHgap;

		Dimension dim = parent->getComponent(i).preferredSize();

		width += dim.width;

		if ( dim.height > maxHeight ) maxHeight = dim.height;
	}

	Insets insets = parent->getInsets();

	return Dimension( width     + insets.left + insets.right,
		              maxHeight + insets.top  + insets.bottom );
}

Dimension FlowLayout::minimumLayoutSize( Container* parent )
{
	// FOR NOW::
	return preferredLayoutSize( parent );
}

void FlowLayout::layoutContainer( Container* parent )
{
	int cnt = parent->getComponentCount();

	if ( cnt == 0 ) return;

	int x = 0, y =0;

	Dimension parentDim = parent->getSize();
	Insets    insets    = parent->getInsets();

	x += insets.left;
	y += insets.top;
	parentDim.width  -= insets.left + insets.right;
	parentDim.height -= insets.top  + insets.bottom;

	// used later in 2nd phase of 2-phase layouing

	mPrefDimOfPhase1 = Dimension( 0,0 );

	int i = 0;
	int row = 0;

	while( i < cnt )
	{
		int rowWidth = 0;
		int col = 0;
		int i1 = i;
		int maxHeight = 0;

		// estimate # of items in current row, and set their sizes

		// FIXME::
		Dimension dims[400];
		
		while( i < cnt && rowWidth < parentDim.width )
		{
			if ( col != 0 ) rowWidth += mHgap;

			// FIXME:: getPreferedSize() is called twice for the
			//         compnent which wraps around the current row

			Dimension prefDim = parent->getComponent(i).preferredSize();

			if ( rowWidth + prefDim.width <= parentDim.width )
			{
				parent->getComponent(i).setSize( prefDim );

				dims[i - i1] = prefDim;

				rowWidth += prefDim.width;

				if ( prefDim.height > maxHeight ) 
					
					maxHeight = prefDim.height;

				++i; ++col;
			}
			else
				break;
		}

		if ( mPrefDimOfPhase1.width < rowWidth ) mPrefDimOfPhase1.width = rowWidth; 

		if ( col == 0 ) break; // cannot fit component into the row

		// posion items in current row

		int pos = x; // left-alignend

		if ( mAlign == CENTER ) pos = x + ( parentDim.width - rowWidth ) / 2;
		else
		if ( mAlign == RIGHT  ) pos = x + ( parentDim.width - rowWidth );

		col = 0;
		int rowStart = i1;

		if ( row != 0 ) 
		{
			y += mVgap;
			mPrefDimOfPhase1.height += mVgap;
		}

		while( i1 < i )
		{
			Dimension dim = dims[i1-rowStart];

			if ( col != 0 ) pos += mHgap;

			parent->getComponent(i1).setLocation( 
				Point( pos, y + ( maxHeight - dim.height ) / 2 ) ); // centered vertically

			pos += dim.width; ++col; ++i1;
		}

		y += maxHeight; ++row;

		mPrefDimOfPhase1.height += maxHeight; 

	} // end of while(...)

	mPrefDimOfPhase1.width  += insets.left + insets.right;
	mPrefDimOfPhase1.height += insets.top  + insets.bottom;
}