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

#ifndef __GRIDBAGLAYOUT_G__
#define __GRIDBAGLAYOUT_G__

#include "awt_emul.h"

class GridBagConstraints : public Object
{
public:
	GridBagConstraints();

	GridBagConstraints( int _gridx,
					    int _gridy,
					    int _gridwidth,
					    int _gridheight,
					    double _weightx,
					    double _weighty,
					    int _anchor,
					    int _fill,
					    Insets _insets,
					    int _ipadx,
					    int _ipady );
public:
	int anchor;
	int fill;
	int gridheight;
	int gridwidth;
	int gridx;
	int gridy;
	Insets insets;
	int ipadx;
	int ipady;
	double weightx;
	double weighty;

	enum Bsdfas
	{
		Afsds  = 5,
			asdfd = 44
	};

#undef RELATIVE
#undef _LEFT
#undef _CENTER

	enum GRID_BAG_CONSTANTS
	{
		RELATIVE   = -1,
		REMAINDER  = 0,

		NONE       = 0,
		BOTH       = 1,
		HORIZONTAL = 2,
		VERTICAL   = 3,

		CENTER    = 10,
		NORTH     = 11,
		NORTHEAST = 12,
		EAST      = 13,
		SOUTHEAST = 14,
		SOUTH     = 15,
		SOUTHWEST = 16,
		WEST      = 17,
		NORTHWEST = 18,
	};

	// used interally  (do not use!)

	enum  GRID_BAG_CONSTANTS_INTERNAL
	{
		_LEFT     = 20,
		_CENTER   = 21,
		_RIGHT    = 22
	};

	// for internal uses

	Component* mpComp;
	Dimension  mPrefCompSize;
};

struct CellInfo
{
	Component* mpComp;

	int    prefSize;	 // actually, it can be calculated on-the-fly
	int    prefCompSize; 

	int    cellSpan;
	int    leftInset;
	int    rightInset;
	int    pad;
	int    fill;
	int    anchor;

	double extraSpace;

	double weight;

	int finalSize;
	int finalPos;
	int finalCompSize;
	int finalCompPos;

};

struct CellHolder
{
	GridBagConstraints* mpConstr;
	double weightx;
	double weighty;

	int    gridwidth;
	int    gridheight;
	int    actualWidth;
	int    actualHeight;

	bool   isFirstCellForComp;
	int x, y;
};

typedef CellInfo& (*CellGetterFnctT)(int,int);

WX_DEFINE_ARRAY( CellHolder*, CellHolderArrayT );
WX_DEFINE_ARRAY( GridBagConstraints*, GridBagConstraintsArrayT );

class GridBagLayout : public LayoutManager2
{
public:
	GridBagLayout();
	~GridBagLayout();

	// impl. of LayoutManager

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
	int CalcPrefSize( CellInfo* cells, int xCnt, int yCnt, int _arrayWidth );
					
	void LayoutCells( CellInfo* cells, int xCnt, int yCnt,
					  int outterSize, int outterPos, int _arrayWidth );

	void InitializeCellArray( CellInfo* cells, int size );
	void InitCellFromHolder( CellHolder& holder );

	void CreateMatrix();

	long GetCellCode( int x, int y );

	void CleanUpConstraints();
	void ClearCachedData();
	bool HasCashedInfo();

	void SetComponentLocations();

protected:

	GridBagConstraintsArrayT mConstraints;

	CellInfo*                mpHorizCellInfos;
	CellInfo*                mpVertCellInfos;

	int                      mColCount;
	int                      mRowCount;
};

#endif // __GRIDBAGLAYOUT_G__