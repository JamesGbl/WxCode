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

#ifndef __ABSOLUTELAYOUT_G__
#define __ABSOLUTELAYOUT_G__

#include "awt_emul.h"

class AbsoluteLayout : public LayoutManager
{
public:

	AbsoluteLayout();
	// impl. of LayoutManager interface

	virtual void addLayoutComponent(String name,
                                    Component* comp );

	virtual void removeLayoutComponent( Component* comp );

	virtual Dimension preferredLayoutSize( Container* parent );

	virtual Dimension minimumLayoutSize( Container* parent );

	virtual void layoutContainer( Container* parent );

};

#endif // __ABSOLUTELAYOUT_G__