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

#include "absolutelayout.h"
#include "wx/hash.h"

/***** Implementation for class AbsoluteLayout *****/

AbsoluteLayout::AbsoluteLayout() {}
// impl. of LayoutManager interface

void AbsoluteLayout::addLayoutComponent( String name,
                                     Component* comp )
{ /* nothing */ }

void AbsoluteLayout::removeLayoutComponent( Component* comp )

{ /* nothing */ }

Dimension AbsoluteLayout::preferredLayoutSize( Container* parent )
{
	int cnt = parent->getComponentCount();

	int width  = 0;
	int height = 0;

	for( int i = 0; i != cnt; ++i )
	{
		Point pos = parent->getComponent(i).getLocation();
		Dimension dim = parent->getComponent(i).preferredSize();

		if ( width < pos.x + dim.width ) width = pos.x + dim.width;
		if ( height < pos.y + dim.height ) height = pos.y + dim.height;
	}

	return Dimension( width, height );
}

Dimension AbsoluteLayout::minimumLayoutSize( Container* parent )
{
	// FOR NOW::
	return preferredLayoutSize( parent );
}

void AbsoluteLayout::layoutContainer( Container* parent )
{
	// no layouting is done of absolute-layouts,
	// positions of components are left unchanged (i.e. initial)
}
