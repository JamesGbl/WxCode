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

#include "boxlayout.h"

/***** Implementation for class BoxLayout *****/

BoxLayout::BoxLayout( Container* target, int axis )

	: mAxis( axis )
{}

BoxLayout::BoxLayout( int axis )

	: mAxis( axis )
{}

// impl. of LayoutManager interface

void BoxLayout::addLayoutComponent( String name,
                                    Component* comp )
{ /* nothing */ }

void BoxLayout::removeLayoutComponent( Component* comp )

{ /* nothing */ }


Dimension BoxLayout::preferredLayoutSize( Container* parent )
{
	int cnt = parent->getComponentCount();
	int width = 0;
	int maxHeight = 0; 

	for( int i = 0; i != cnt; ++i )
	{
		Dimension dim = parent->getComponent( i ).preferredSize();

		if ( mAxis == X_AXIS ) 
		{
			width += dim.width;

			if ( dim.height > maxHeight ) maxHeight = dim.height;
		}
		else
		{
			width += dim.height;

			if ( dim.width > maxHeight ) maxHeight = dim.width;
		}
	}

	Insets insets = parent->getInsets();

	if ( mAxis == X_AXIS )
	
		return Dimension( width     + insets.left + insets.right, 
						  maxHeight + insets.top  + insets.bottom );
	else
		return Dimension( maxHeight + insets.left + insets.right, 
						  width     + insets.top  + insets.bottom );

	return Dimension(); // never reached
}

Dimension BoxLayout::minimumLayoutSize( Container* parent )
{
	// FOR NOW::
	return preferredLayoutSize( parent );
}

void BoxLayout::layoutContainer( Container* parent )
{
	// fragment form preferedLayoutSize()...

	int cnt = parent->getComponentCount();

	if ( cnt == 0 ) return;

	int prefTotalWidth = 0;

	int i = 0;
	for( ; i != cnt; ++i )
	
		if ( mAxis == X_AXIS )
		
			prefTotalWidth += parent->getComponent( i ).preferredSize().width;
		else
			prefTotalWidth += parent->getComponent( i ).preferredSize().height;


	Dimension parentDim = parent->getSize();
	Insets    insets    = parent->getInsets();

	int x = 0, y =0;
	x += insets.left;
	y += insets.top;
	parentDim.width  -= insets.left + insets.right;
	parentDim.height -= insets.top  + insets.bottom;

	int widthUsed = 0;

	for( i = 0; i != cnt; ++i )
	{
		Component& c = parent->getComponent(i);

		Dimension prefDim = c.preferredSize();

		// squeez it if there is less space then preferred

		if ( mAxis == X_AXIS )
		{
			if ( parentDim.width < prefTotalWidth )
			{
				prefDim.width  = (int)( (double)parentDim.width * 
										( (double)prefDim.width / (double)prefTotalWidth ) 
									  );

				if ( i == cnt - 1 ) prefDim.width = parentDim.width - widthUsed;
			}

			prefDim.height = parentDim.height;

			widthUsed += prefDim.width;
		}
		else
		{
			if ( parentDim.height < prefTotalWidth )
			{
				prefDim.height  = (int)( (double)parentDim.height * 
										 ( (double)prefDim.height / (double)prefTotalWidth ) 
									   );

				if ( i == cnt - 1 ) prefDim.height = parentDim.height - widthUsed;
			}

			prefDim.width  = parentDim.width;

			widthUsed += prefDim.height;
		}

		c.setLocation( x,y );
		c.setSize( prefDim );

		if ( mAxis == X_AXIS ) x += prefDim.width;
						  else y += prefDim.height;
	}
}