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

#include "cardlayout.h"

/***** Implementation for class CardLayout *****/

CardLayout::CardLayout()
	: mShownCardIdx( 0 )
{}

CardLayout::~CardLayout()
{
	for( size_t i = 0; i != mCards.Count(); ++i )
	{
		delete mCards[i]->mpName;
		delete mCards[i];
	}
}

int CardLayout::getHgap() { return mHgap; }
int CardLayout::getVgap() { return mVgap; }

void CardLayout::setHgap( int hgap ) { mHgap = hgap; }
void CardLayout::setVgap( int vgap ) { mVgap = vgap; }

void CardLayout::first( Container* parent ) 
{
	DoShowCard( parent, 0 );
}

void CardLayout::next( Container* parent )
{
	if ( mShownCardIdx + 1 < mCards.Count() )

		DoShowCard( parent, mShownCardIdx + 1 );
	else
		DoShowCard( parent, 0 );
}

void CardLayout::previous( Container* parent )
{
	if ( mShownCardIdx > 0 );
	
		DoShowCard( parent, mShownCardIdx - 1 );
}

void CardLayout::last( Container* parent )
{
	if ( mCards.Count() )

		DoShowCard( parent, mCards.Count() - 1 );
}

void CardLayout::show( Container* parent, String name )
{
	for( size_t i = 0; i != mCards.Count(); ++i )

		if ( *mCards[i]->mpName == name )

			DoShowCard( parent, i );
}

// impl. of LayoutManager

void CardLayout::addLayoutComponent(String name,
                                      Component* comp)
{
	// Deprecated. replaced by addLayoutComponent(Component, Object)


	addLayoutComponent( comp, (Object*)&name );
}

void CardLayout::removeLayoutComponent(Component* comp)
{
	for( size_t i = 0; i != mCards.Count(); ++i )

		if ( mCards[i]->mpComp == comp )
		{
			delete mCards[i];
			mCards.Remove( i );

			break;
		}
}

Dimension CardLayout::preferredLayoutSize(Container* parent)
{
	if ( mCards.Count() == 0 || mShownCardIdx >= mCards.Count() ) 
		
		return Dimension(0,0);

	Dimension dim =	mCards[mShownCardIdx]->mpComp->preferredSize();
	Insets    insets    = parent->getInsets();

	return Dimension( dim.width  + insets.left + insets.right,
		              dim.height + insets.top  + insets.bottom );
}

Dimension CardLayout::minimumLayoutSize( Container* parent )
{
	// FOR NOW::

	return preferredLayoutSize( parent );
}

void CardLayout::layoutContainer( Container* parent )
{
	if ( mCards.Count() == 0 || mShownCardIdx >= mCards.Count() ) return;

	Dimension parentDim = parent->getSize();
	Insets    insets    = parent->getInsets();

	int x = 0, y =0;
	x += insets.left;
	y += insets.top;
	parentDim.width  -= insets.left + insets.right;
	parentDim.height -= insets.top  + insets.bottom;

	Component& c = *mCards[mShownCardIdx]->mpComp;

	c.setLocation( Point(x,y) );
	c.setSize( parentDim );

	for( size_t i = 0; i != mCards.Count(); ++i )

		if ( i == (size_t)mShownCardIdx )
		{
			if ( !mCards[i]->mpComp->isVisible() )
				
				mCards[i]->mpComp->setVisible( TRUE );
		}
		else
		{
			if ( mCards[i]->mpComp->isVisible() )

				mCards[i]->mpComp->setVisible( FALSE );
		}
}

// impl. of LayoutManager2

void CardLayout::addLayoutComponent( Component* comp, Object* constraints )
{
	CardInfo* pCard = new CardInfo();

	pCard->mpComp = comp;
	pCard->mpName = (String*)constraints;

	mCards.Add( pCard );
}

Dimension CardLayout::maximumLayoutSize( Container* target )
{
	// FOR NOW::
	return Dimension(2000,2000);
}

double CardLayout::getLayoutAlignmentX( Container* target )
{
	return 0;
}

double CardLayout::getLayoutAlignmentY( Container* target )
{
	return 0;
}

void CardLayout::invalidateLayout(Container* target)

{ /* nothing */ }

/*** protected methods ***/

void CardLayout::DoShowCard( Container* parent, size_t index )
{
	if ( mCards.Count() == 0 ) return;

	size_t prevCard = mShownCardIdx;

	if ( index > mCards.Count() ) mShownCardIdx = 0;
						     else mShownCardIdx = index;

	if ( prevCard < mCards.Count() && 
		 prevCard != mShownCardIdx &&
		 mCards[prevCard]->mpComp->isVisible() )

		 mCards[prevCard]->mpComp->setVisible( FALSE );

	mCards[mShownCardIdx]->mpComp->setVisible( TRUE );

	this->layoutContainer( parent );

	parent->repaint();
}