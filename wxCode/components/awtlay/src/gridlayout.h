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

#ifndef __GRIDLAYOUT_G__
#define __GRIDLAYOUT_G__

#include "awt_emul.h"

class GridLayout : public LayoutManager
{
public:
	GridLayout();

	GridLayout( int rows, int cols );

	GridLayout( int rows, int cols, int hgap, int vgap );

	virtual int getColumns();
	virtual int getHgap();
	virtual int getRows();
	virtual int getVgap();

	virtual void setColumns( int columns );
	virtual void setHgap( int hgap );
	virtual void setRows( int rows );
	virtual void setVgap( int vgap );

	// impl. of LayoutManager interface

	virtual void addLayoutComponent(String name,
                                    Component* comp );

	virtual void removeLayoutComponent( Component* comp );

	virtual Dimension preferredLayoutSize( Container* parent );

	virtual Dimension minimumLayoutSize( Container* parent );

	virtual void layoutContainer( Container* parent );

protected:
	int mRows;
	int mCols;
	int mHgap;
	int mVgap;
};

#endif // __GRIDLAYOUT_G__