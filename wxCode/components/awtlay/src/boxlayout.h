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

#ifndef __BOXLAYOUT_G__
#define __BOXLAYOUT_G__

#include "awt_emul.h"

class BoxLayout : public LayoutManager
{
public:
	BoxLayout( Container* target, int axis );

	BoxLayout( int axis );

	// impl. of LayoutManager interface

	virtual void addLayoutComponent(String name,
                                    Component* comp );

	virtual void removeLayoutComponent( Component* comp );

	virtual Dimension preferredLayoutSize( Container* parent );

	virtual Dimension minimumLayoutSize( Container* parent );

	virtual void layoutContainer( Container* parent );

	enum AXIS_ORIENTATIONS
	{
		X_AXIS = 0,
		Y_AXIS = 1
	};

protected:
	int mAxis;
};

#endif // __BOXLAYOUT_G__