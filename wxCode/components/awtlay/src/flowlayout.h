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

#ifndef __FLOWLAYOUT_G__
#define __FLOWLAYOUT_G__

#include "awt_emul.h"

class FlowLayout : public LayoutManager
{
public:
	FlowLayout();

	FlowLayout( int align );

	FlowLayout( int align, int hgap, int vgap );

	int getAlignment();
	int getHgap();
	int getVgap();

	void setAlignment( int align );
	void setHgap( int hgap );
	void setVgap( int vgap );

	// impl. of LayoutManager interface

	virtual void addLayoutComponent(String name,
                                    Component* comp );

	virtual void removeLayoutComponent( Component* comp );

	virtual Dimension preferredLayoutSize( Container* parent );

	virtual Dimension minimumLayoutSize( Container* parent );

	virtual void layoutContainer( Container* parent );

public: 

	enum ALIGNMNET_ENUM
	{
		CENTER   = 0,
		LEFT     = 1,
		RIGHT    = 2,

		LEADING  = LEFT,  // for Western, Eruopien text-orientation
		TRAILING = RIGHT  // -/-
	};

protected:
	int mAlign;
	int mHgap;
	int mVgap;

	Dimension mPrefDimOfPhase1;
};

#endif // __FLOWLAYOUT_G__