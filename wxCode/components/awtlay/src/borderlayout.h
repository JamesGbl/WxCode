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

#ifndef __BORDERLAYOUT_G__
#define __BORDERLAYOUT_G__

#include "awt_emul.h"

class BorderLayoutConstraint : public Object
{
public:
	BorderLayoutConstraint( int align )
		: mAlign( align ) {}

public:
	int mAlign;
};

class BorderLayout : public LayoutManager2
{
public:
	BorderLayout();

	BorderLayout( int hgap, int vgap );

	~BorderLayout();

	// impl. of LayoutManager

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

public:

	static BorderLayoutConstraint* CENTER;
	static BorderLayoutConstraint* EAST;
	static BorderLayoutConstraint* NORTH;
	static BorderLayoutConstraint* SOUTH;
	static BorderLayoutConstraint* WEST;
	static BorderLayoutConstraint* AFTER_LAST_LINE;
	static BorderLayoutConstraint* AFTER_LINE_ENDS;
	static BorderLayoutConstraint* BEFORE_FIRST_LINE;
	static BorderLayoutConstraint* BEFORE_LINE_BEGINS;

protected:

	static BorderLayoutConstraint mCENTER;
	static BorderLayoutConstraint mEAST;
	static BorderLayoutConstraint mNORTH;
	static BorderLayoutConstraint mSOUTH;
	static BorderLayoutConstraint mWEST;
	static BorderLayoutConstraint mAFTER_LAST_LINE;
	static BorderLayoutConstraint mAFTER_LINE_ENDS;
	static BorderLayoutConstraint mBEFORE_FIRST_LINE;
	static BorderLayoutConstraint mBEFORE_LINE_BEGINS;

protected:

	enum ALIGNMENT_ENUM_INTERNAL
	{
		_CENTER = 0,
		_EAST   = 1,
		_NORTH  = 2,
		_SOUTH  = 3,
		_WEST   = 4,

		// for Western, top-to-bottom, left-to-right orientations

		_AFTER_LAST_LINE    = _SOUTH,
		_AFTER_LINE_ENDS    = _EAST,
		_BEFORE_FIRST_LINE  = _NORTH,
		_BEFORE_LINE_BEGINS = _WEST,
	};

protected:
	void DistributeSizes( int& left, int& center, 
						  int& right, int totalWidth, int gap,
						  bool hasLeft, bool hasCenter, bool hasRight );

	int GetWidthSum( int left, int center, int right, int gap, 
					 bool hasLeft, bool hasCenter, bool hasRight );

	int Hgap( Component* pComp );
	int Vgap( Component* pComp );

protected:
	int mHgap;
	int mVgap;

	Component* mpCenterComp;
	Component* mpEastComp;
	Component* mpNorthComp;
	Component* mpSouthComp;
	Component* mpWestComp;
};

#endif // __BORDERLAYOUT_G__