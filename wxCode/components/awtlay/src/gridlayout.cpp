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

#include "gridlayout.h"
#include "wx/hash.h"

/***** Implementation for class GridLayout *****/

GridLayout::GridLayout()

	: mRows( 0 ), mCols( 0 ), mHgap( 0 ), mVgap( 0 )
{}

GridLayout::GridLayout( int rows, int cols )

	: mRows( rows ), mCols( cols ), mHgap( 0 ), mVgap( 0 )
{}

GridLayout::GridLayout( int rows, int cols, int hgap, int vgap )

	: mRows( rows ), mCols( cols ), mHgap( hgap ), mVgap( vgap )
{}

int GridLayout::getColumns() { return mCols; }
int GridLayout::getHgap() { return mHgap; }
int GridLayout::getRows() { return mRows; }
int GridLayout::getVgap() { return mVgap; }

void GridLayout::setColumns( int columns ) { mCols = columns; }
void GridLayout::setHgap( int hgap ) { mHgap = hgap; }
void GridLayout::setRows( int rows ) { mRows = rows; }
void GridLayout::setVgap( int vgap ) { mVgap = vgap; }

// impl. of LayoutManager interface

void GridLayout::addLayoutComponent( String name,
                                     Component* comp )
{ /* nothing */ }

void GridLayout::removeLayoutComponent( Component* comp )

{ /* nothing */ }


Dimension GridLayout::preferredLayoutSize( Container* parent )
{
	int cnt = parent->getComponentCount();

	int maxHeight = 0;
	int maxWidth  = 0;

	if ( cnt == 0 ) return Dimension(0,0);

	for( int i = 0; i != cnt; ++i )
	{
		Dimension dim = parent->getComponent(i).preferredSize();

		if ( dim.width  > maxWidth  ) maxWidth  = dim.width;
		if ( dim.height > maxHeight ) maxHeight = dim.height;
	}

	int nRows = cnt / mCols + ( ( cnt % mCols ) ? 1 : 0 );

	int nCols = ( nRows != 0 ) ? mCols : cnt;

	if ( nRows > mRows ) nRows = mRows;

	int hgaps = ( nCols > 1 ) ? ( nCols - 1 ) * mHgap : 0;
	int vgaps = ( nRows > 1 ) ? ( nRows - 1 ) * mVgap : 0;

	Insets insets = parent->getInsets();

	return Dimension( maxWidth * nCols  + hgaps + insets.left + insets.right, 
		              maxHeight* nRows  + vgaps + insets.top  + insets.bottom );
}

Dimension GridLayout::minimumLayoutSize( Container* parent )
{
	// FOR NOW::
	return preferredLayoutSize( parent );
}

void GridLayout::layoutContainer( Container* parent )
{
	// fragment form preferedLayoutSize()...

	int cnt = parent->getComponentCount();

	if ( cnt == 0 ) return;

	int nRows = cnt / mCols + ( ( cnt % mCols ) ? 1 : 0 );

	int nCols = ( nRows != 0 ) ? mCols : cnt;

	if ( nRows > mRows ) nRows = mRows;

	int hgaps = ( nCols > 1 ) ? ( nCols - 1 ) * mHgap : 0;
	int vgaps = ( nRows > 1 ) ? ( nRows - 1 ) * mVgap : 0;

	// actual layouting

	int x = 0, y = 0;

	Dimension dim = parent->getSize();
	Insets insets = parent->getInsets();

	x += insets.left;
	y += insets.top;

	if ( dim.width < 0  ) { dim.width = 0;  hgaps = 0; }
	if ( dim.height < 0 ) { dim.height = 0; vgaps = 0; }

	dim.width  -= insets.left + insets.right;
	dim.height -= insets.top  + insets.bottom;

	int colWidth  = ( dim.width  - hgaps ) / nCols;
	int rowHeight = ( dim.height - vgaps ) / nRows;

	int i = 0;

	for( int row = 0; row != nRows; ++row, 
									y += ( rowHeight + ( ( row == 0 ) ? 0 : mVgap ) ) )
	
		for( int col = 0; col != nCols; ++col )
		{
			if ( i < cnt )
			{
				Component& c = parent->getComponent( i );

				c.setLocation( Point( x + col*colWidth + col * mHgap, y ) );
				c.setSize( Dimension( colWidth, rowHeight ) );

				++i;
			}
		}
}
