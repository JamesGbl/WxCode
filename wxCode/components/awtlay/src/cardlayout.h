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

#ifndef __CARDLAYOUT_G__
#define __CARDLAYOUT_G__

#include "awt_emul.h"
#include "wx/dynarray.h"

class CardInfo
{
public:
	Component* mpComp;
	String*    mpName;
};

WX_DEFINE_ARRAY( CardInfo*, CardInfoListT );

class CardLayout : public LayoutManager
{
public:
	CardLayout();
	~CardLayout();

	virtual int getHgap();
	virtual int getVgap();

	virtual void setHgap( int hgap );
	virtual void setVgap( int vgap );

	virtual void first( Container* parent );
	virtual void next( Container* parent );
	virtual void previous( Container* parent );
	virtual void last( Container* parent );
	virtual void show( Container* parent, String name );

	// impl. of LayoutManager

	// Deprecated. replaced by addLayoutComponent(Component, Object)

	virtual void addLayoutComponent(String name,
                                    Component* comp);

	virtual void removeLayoutComponent(Component* comp);

	virtual Dimension preferredLayoutSize(Container* parent);

	virtual Dimension minimumLayoutSize(Container* parent);

	virtual void layoutContainer(Container* parent);

	// impl. of LayoutManager2

	virtual void addLayoutComponent(Component* comp, Object* constraints);

	virtual Dimension maximumLayoutSize(Container* target);

	virtual double getLayoutAlignmentX(Container* target);

	virtual double getLayoutAlignmentY(Container* target);

	virtual void invalidateLayout(Container* target);

protected:
	void DoShowCard( Container* parent, size_t index );

protected:
	int mHgap;
	int mVgap;

	CardInfoListT mCards;
	size_t        mShownCardIdx;
	Container*    mpContainer;
};

#endif // __CARDLAYOUT_G__