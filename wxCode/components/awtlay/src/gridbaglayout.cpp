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

#include "gridbaglayout.h"

#include "wx/hash.h"
#include <memory.h>

/***** Implementation for class GridBagConstraints ******/

GridBagConstraints::GridBagConstraints()

	: gridx( RELATIVE ),
	  gridy( RELATIVE ),
	  gridwidth( 1 ),
	  gridheight( 1 ),
	  weightx( 0 ),
	  weighty( 0 ),
	  anchor ( CENTER ),
	  fill   ( NONE ),
	  insets ( 0,0,0,0 ),
	  ipadx  ( 0 ),
	  ipady  ( 0 )
{}

GridBagConstraints::GridBagConstraints( int _gridx,
										int _gridy,
										int _gridwidth,
										int _gridheight,
										double _weightx,
										double _weighty,
										int _anchor,
										int _fill,
										Insets _insets,
										int _ipadx,
										int _ipady )
	: gridx( _gridx ),
	  gridy( _gridy ),
	  gridwidth ( _gridwidth  ),
	  gridheight( _gridheight ),
	  weightx( _weightx ),
	  weighty( _weighty ),
	  anchor ( _anchor ),
	  fill   ( _fill ),
	  insets ( _insets ),
	  ipadx  ( _ipadx ),
	  ipady  ( _ipady )
{}


/***** Implementation for class GridBagLayout ******/

GridBagLayout::GridBagLayout()

	: mpHorizCellInfos( NULL ),
	  mpVertCellInfos( NULL )
{}

GridBagLayout::~GridBagLayout()
{
	ClearCachedData();
	CleanUpConstraints();
}

// impl. of LayoutManager

void GridBagLayout::addLayoutComponent(String name,
                                       Component* comp)
{
	// cannot add components without constraints
	// TBD:: assertion
}

void GridBagLayout::removeLayoutComponent(Component* comp)
{
	// TBD::
}

Dimension GridBagLayout::preferredLayoutSize(Container* parent)
{
	if ( !HasCashedInfo() ) CreateMatrix();

	int width  = CalcPrefSize( mpHorizCellInfos, mColCount, mRowCount, mColCount );
	int height = CalcPrefSize( mpVertCellInfos,  mRowCount, mColCount, mRowCount );

	Insets insets = parent->getInsets();

	return Dimension( width  + insets.left + insets.right,
					  height + insets.top  + insets.bottom );
}

Dimension GridBagLayout::minimumLayoutSize( Container* parent )
{
	// FOR NOW::

	return preferredLayoutSize( parent );
}

void GridBagLayout::layoutContainer( Container* parent )
{
	Dimension parentDim = parent->getSize();
	Point     pos(0,0);

	Insets insets = parent->getInsets();

	pos.x += insets.left;
	pos.y += insets.top;

	parentDim.width  -= insets.left + insets.right;
	parentDim.height -= insets.top  + insets.bottom;

	if ( !HasCashedInfo() ) CreateMatrix();

	// calculate horizontal extents and sizes of all cells

	LayoutCells( mpHorizCellInfos, mColCount, mRowCount, 
								   parentDim.width,  pos.x, mColCount );

	// now the same alg. is applied independently for vertical positions/sizes

	LayoutCells( mpVertCellInfos,  mRowCount, mColCount, 
								   parentDim.height, pos.y, mRowCount );

	SetComponentLocations();
}

// impl. of LayoutManager2

void GridBagLayout::addLayoutComponent( Component* comp, Object* constraints )
{
	GridBagConstraints* pConstr = new GridBagConstraints();

	*pConstr = *((GridBagConstraints*)constraints);

	pConstr->mpComp = comp;

	mConstraints.Add( pConstr );
}

Dimension GridBagLayout::maximumLayoutSize( Container* target )
{
	// FOR NOW::
	return Dimension(2000,2000);
}

double GridBagLayout::getLayoutAlignmentX( Container* target )
{
	return 0;
}

double GridBagLayout::getLayoutAlignmentY( Container* target )
{
	return 0;
}

void GridBagLayout::invalidateLayout( Container* target )
{
	ClearCachedData();
}

/*** protected mehtods ***/

#define CELL_AT(x,y) cells[ (y) * _arrayWidth + (x) ]

int GridBagLayout::CalcPrefSize( CellInfo* cells, int xCnt, int yCnt,
								 int _arrayWidth )
{
	int prefered = 0;

	for( int x = 0; x != xCnt; ++x )
	{
		int maxColSize = 0;

		for( int y = 0; y != yCnt; ++y )
		
			if ( CELL_AT(x,y).prefSize > maxColSize )

				maxColSize = CELL_AT(x,y).prefSize;
		
		prefered += maxColSize;
	}

	return prefered;
}

void GridBagLayout::LayoutCells( CellInfo* cells, int xCnt, int yCnt,
							     int outterSize, int outterPos,
								 int _arrayWidth )
{
	int actualSpaceUsed = 0;

	int x = 0, y = 0;

	// calculate prefered size of each column

	double* prefColSizes = new double[ xCnt ];

	for( x = 0; x != xCnt; ++x )
	{
		int prefColSize = 0;

		for( y = 0; y != yCnt; ++y )

			if ( CELL_AT(x,y).prefSize > prefColSize )

				prefColSize = CELL_AT(x,y).prefSize;

		prefColSizes[x] = prefColSize;
	}

	double totalPrefSize = 0;

	for( x = 0; x != xCnt; ++x ) totalPrefSize += prefColSizes[x];

	int extraSpace = outterSize - totalPrefSize;

	if ( extraSpace <= 0 )
	{
		// redistribute spaces to fit (i.e. shrink to) given outter-size

		int spaceUsed = 0;

		for( x = 0; x != xCnt; ++x )
		{
			int colSize = (int)( (double)outterSize * ( prefColSizes[x] / totalPrefSize ) );

			// elimniate round-off errors at the expence of last column

			if ( x == xCnt - 1 ) colSize = outterSize - spaceUsed;

			for( int y = 0; y != yCnt; ++y )
			
				CELL_AT(x,y).finalSize = colSize;

			spaceUsed += colSize;
		}

		actualSpaceUsed = outterSize; // all space is cosumed when 
								      // extra-space is absent
	}
	else
	{
		// defferent alg. if there is extra space available

		int hasStrechedCells = 0;

		for( y = 0; y != yCnt; ++y )
		{
			// calc weight-sum

			double totalWeight = 0;

			int x = 0;
			for( ; x != xCnt; ++x )
			
				totalWeight += CELL_AT(x,y).weight;

			// distribute extraSpace in a row according to cell weights

			if ( totalWeight > 0 )
			{
				hasStrechedCells = 1;

				for( x = 0; x != xCnt; ++x )
			
					CELL_AT(x,y).extraSpace = extraSpace * 
											  ( CELL_AT(x,y).weight / totalWeight );
			}
			else
				for( x = 0; x != xCnt; ++x )
			
					CELL_AT(x,y).extraSpace = 0;
		}

		// calc total extra space consumed by each column
		
		double* colSpaces = new double[ xCnt ];

		double sum = 0;

		for( x = 0; x != xCnt; ++x )
		{
			double total = 0;

			for( y = 0; y != yCnt; ++y )

				total += CELL_AT(x,y).extraSpace;

			colSpaces[x] = total;

			sum += total;
		}

		// equation should hold true : "extraSpace * yCnt == sum"
		// now redistribute extra space among all cesll in each columns using, 
		// giving each column a fraction of space which corresponds to column's 
		// "totalSpace" value's relation with the "sum"

		int spaceUsed = 0; 
		actualSpaceUsed = 0;

		for( x = 0; x != xCnt; ++x )
		{
			int extraSpaceForCol = (int)( extraSpace * ( colSpaces[x] / sum ) );

			// elimniate round-off errors at the expence of last column

			if ( x == xCnt - 1 && hasStrechedCells ) 
				
				extraSpaceForCol = extraSpace - spaceUsed;

			int spaceForCol = prefColSizes[x] + extraSpaceForCol;

			actualSpaceUsed += spaceForCol;

			for( y = 0; y != yCnt; ++y )
			
				CELL_AT(x,y).finalSize = spaceForCol;


			spaceUsed += extraSpaceForCol;
		}

		delete [] colSpaces;

	} // end of else {....}

	// now do positioning of cells and components inside of them
	// (at this point, "actualSpaceUsed <= outterSpace" should be true)

	int curPos = ( outterSize - actualSpaceUsed ) / 2; // center grid w/respect
													   // to bounds of outter component

	for( x = 0; x != xCnt; ++x )
	{
		for( y = 0; y != yCnt; ++y )
		{
			CellInfo& cell = CELL_AT(x,y);

			if ( cell.mpComp != NULL )
			{
				cell.finalPos = curPos;

				int compSize = 0;
				int compPos  = curPos;

				// account for spaning multiple cells

				for( int i = 0; i != cell.cellSpan && x + i < xCnt; ++i )

					compSize += CELL_AT(x+i,y).finalSize;

				// align compoenent within cell's display area

				compPos  += cell.leftInset;
				compSize -= cell.leftInset + cell.rightInset;

				cell.finalCompPos  = compPos;
				cell.finalCompSize = compSize;

				if ( !cell.fill && cell.prefCompSize < compSize )
				{
					cell.finalCompSize = cell.prefCompSize;

					if ( cell.anchor == GridBagConstraints::_LEFT )
					
						cell.finalCompPos  = compPos;
					else
					if ( cell.anchor == GridBagConstraints::_CENTER )
					
						cell.finalCompPos = compPos + ( compSize - cell.prefCompSize ) / 2;
					else
					if ( cell.anchor == GridBagConstraints::_RIGHT )
					
						cell.finalCompPos = compPos + ( compSize - cell.prefCompSize );
				}

				cell.finalCompPos += outterPos;

			} // end of if(...)
		} // end of for(...)

		curPos += CELL_AT(x,0).finalSize;

	} // end of for(..)

	delete [] prefColSizes;
}

void GridBagLayout::CleanUpConstraints()
{
	for( size_t i = 0; i != mConstraints.Count(); ++i )

		delete mConstraints[i];
}

void GridBagLayout::InitializeCellArray( CellInfo* cells, int size )
{
	for( int i = 0; i != size; ++i )
	{
		memset( cells+i, 0, sizeof(CellInfo) );

		cells[i].weight = 0; // doubles cannot be zero'ed with memset(..)
	}
}

void GridBagLayout::InitCellFromHolder( CellHolder& holder )
{
	int pos = holder.y * mColCount + holder.x;

	CellInfo& hCell = mpHorizCellInfos[ holder.y * mColCount + holder.x ]; // note the trick...
	CellInfo& vCell = mpVertCellInfos [ holder.x * mRowCount + holder.y ]; // -/-

	GridBagConstraints& c = *holder.mpConstr;

	hCell.mpComp = c.mpComp;
	vCell.mpComp = c.mpComp;

	if ( c.fill == GridBagConstraints::BOTH || 
		 c.fill == GridBagConstraints::HORIZONTAL ) hCell.fill = 1;

	if ( c.fill == GridBagConstraints::BOTH || 
		 c.fill == GridBagConstraints::VERTICAL )   vCell.fill = 1;

	hCell.leftInset  = c.insets.left;
	hCell.rightInset = c.insets.right;
	vCell.leftInset  = c.insets.top;
	vCell.rightInset = c.insets.bottom;

	hCell.pad = c.ipadx;
	vCell.pad = c.ipady;

	hCell.prefCompSize = c.mPrefCompSize.width  + 2*hCell.pad;
	vCell.prefCompSize = c.mPrefCompSize.height + 2*vCell.pad;

	hCell.prefSize = hCell.prefCompSize + hCell.leftInset + hCell.rightInset;
	vCell.prefSize = vCell.prefCompSize + hCell.leftInset + hCell.rightInset;

	if ( holder.isFirstCellForComp )
	{
		hCell.cellSpan = holder.actualWidth;
		vCell.cellSpan = holder.actualHeight;

		// non-zero weights are applied to the last 
		// cell of the area covered by component 
		// (this is how AWT's GridBagLayout behaves!)

		int x = holder.x + holder.actualWidth  - 1;
		int y = holder.y + holder.actualHeight - 1;

		CellInfo& hCell1 = mpHorizCellInfos[ y * mColCount + x ]; // note the trick...
		CellInfo& vCell1 = mpVertCellInfos [ x * mRowCount + y ]; // -/-

		hCell1.weight = holder.weightx;
		vCell1.weight = holder.weighty;
	}
	// otherwise cellSpan are 0 for intermediate cells

	// adjust alginment for the horizontal-info carrying cell's

	if ( c.anchor == GridBagConstraints::NORTHWEST ||
		 c.anchor == GridBagConstraints::WEST      ||
		 c.anchor == GridBagConstraints::SOUTHWEST ) 
	
		 hCell.anchor = GridBagConstraints::_LEFT;

	if ( c.anchor == GridBagConstraints::NORTH  ||
		 c.anchor == GridBagConstraints::CENTER ||
		 c.anchor == GridBagConstraints::SOUTH  ) 
		 
		 hCell.anchor = GridBagConstraints::_CENTER;

	if ( c.anchor == GridBagConstraints::NORTHEAST ||
		 c.anchor == GridBagConstraints::EAST      ||
		 c.anchor == GridBagConstraints::SOUTHEAST ) 
		 
		 hCell.anchor = GridBagConstraints::_RIGHT;

	// adjust alginment for the vertical-info carrying cell's

	if ( c.anchor == GridBagConstraints::NORTHWEST ||
		 c.anchor == GridBagConstraints::NORTH     ||
		 c.anchor == GridBagConstraints::NORTHEAST ) 
	
		 vCell.anchor = GridBagConstraints::_LEFT;

	if ( c.anchor == GridBagConstraints::WEST   ||
		 c.anchor == GridBagConstraints::CENTER ||
		 c.anchor == GridBagConstraints::EAST   ) 
		 
		 vCell.anchor = GridBagConstraints::_CENTER;

	if ( c.anchor == GridBagConstraints::SOUTHWEST ||
		 c.anchor == GridBagConstraints::SOUTH     ||
		 c.anchor == GridBagConstraints::SOUTHEAST ) 
		 
		 vCell.anchor = GridBagConstraints::_RIGHT;
}

long GridBagLayout::GetCellCode( int x, int y )
{
	return (long)( x << 8 | y );
}

void GridBagLayout::CreateMatrix()
{
	#define MAX_GRID_WIDTH  100
	#define MAX_GRID_HEIGHT 100

	ClearCachedData();

	CellHolderArrayT holders;
	wxHashTable      usedCellsHash;

	mColCount = 0;
	mRowCount = 0;

	int nextX = 0;
	int nextY = 0;

	// creating cells for all added components according
	// to the info in their constraints

	size_t i = 0;
	for( ; i != mConstraints.Count(); ++i )
	{
		GridBagConstraints& c = *mConstraints[i];

		c.mPrefCompSize = c.mpComp->preferredSize();

		if ( c.gridx != GridBagConstraints::RELATIVE ) nextX = c.gridx;
		if ( c.gridy != GridBagConstraints::RELATIVE ) nextY = c.gridy;

		// TBD:: handle situations when grix - give, but gridy is RELATIVE, 
		//       and v.v. (should use vert/horiz scanning for not-used cells)

		while( usedCellsHash.Get( GetCellCode(nextX,nextY) ) != NULL )

			++nextX;

		int width  = c.gridwidth;
		int height = c.gridheight;

		// add this stage, we treat relavtive/REMAINDER
		// sizes as equal to 1, because we cannot yet
		// predict the actual expansion of the coponent

		if ( width == GridBagConstraints::RELATIVE ||
			 width == GridBagConstraints::REMAINDER )

			 width = 1;

		if ( height == GridBagConstraints::RELATIVE ||
			 height == GridBagConstraints::REMAINDER )

			 height = 1;

		/*
		// OLD STUF::
		// distribute weigths uniformly among all
		// cells which are covered by current compoenent

		//double weightx = c.weightx / (double)width;
		//double weighty = c.weighty / (double)height;
		*/

		// create cells for the area covered by the component

		CellHolder* pHolder = NULL;

		for( int xofs = 0; xofs != width; ++xofs )

			for( int yofs = 0; yofs != height; ++yofs )
			{
				pHolder = new CellHolder();
				pHolder->mpConstr = &c;
				pHolder->isFirstCellForComp = ( xofs == 0 && yofs == 0 );

				pHolder->x = nextX + xofs;
				pHolder->y = nextY + yofs;

				pHolder->weightx = c.weightx;
				pHolder->weighty = c.weighty;

				pHolder->gridwidth  = c.gridwidth;
				pHolder->gridheight = c.gridheight;

				holders.Add( pHolder );

				usedCellsHash.Put( GetCellCode( nextX + xofs, nextY + yofs ), 
								   (wxObject*)pHolder );
			}

		if ( c.gridwidth == GridBagConstraints::REMAINDER )
		
			// mark all possible reminding cells in the row as used

			for( int x = nextX+width; x < MAX_GRID_WIDTH; ++x )

				usedCellsHash.Put( GetCellCode(x, nextY), (wxObject*)pHolder );

		if ( c.gridheight == GridBagConstraints::REMAINDER )

			// mark all possible eminding cells in the column as used
		
			for( int y = nextY+height; y < MAX_GRID_HEIGHT; ++y )

				usedCellsHash.Put( GetCellCode(nextX, y), (wxObject*)pHolder );

		// adjust estimated dimensions of the matrix (grid)

		if ( nextX + width  > mColCount ) 
			
			mColCount = nextX + width;

		if ( nextY + height > mRowCount ) 
			
			mRowCount = nextY + height;

		// move on to next cell

		if ( c.gridwidth == GridBagConstraints::REMAINDER ) 
		{ 
			nextX = 0; ++nextY; 
		}
		else
		{
			if ( c.gridwidth == GridBagConstraints::RELATIVE ) nextX += 1;
														  else nextX += width;
		}

	}  // end of for(...)

	// now actually create matrix

	int sz = mColCount*mRowCount;

	mpHorizCellInfos = new CellInfo[ sz ];
	mpVertCellInfos  = new CellInfo[ sz ];
	InitializeCellArray( mpHorizCellInfos, sz );
	InitializeCellArray( mpVertCellInfos,  sz );

	// and fill in cells with info

	for( i = 0; i != holders.Count(); ++i )
	{	
		CellHolder& h = *holders[i];

		if ( h.isFirstCellForComp )
		{
			// now set-up actual gridwidth, and gridheigh parameters

 			int actualWidth  = h.mpConstr->gridwidth;
			int actualHeight = h.mpConstr->gridheight;
			int x = h.x, y = h.y;

			// extend widths/heights if given as relative/REMAINDER by 
			// traversing grid until end is reached or cell occupied by 
			// anther component is encountered

			if ( h.mpConstr->gridwidth == GridBagConstraints::RELATIVE ||
				 h.mpConstr->gridwidth == GridBagConstraints::REMAINDER )
			{
				// TBD:: comments.. this is how AWT's GridBagLayout behaves...

				++x;

				while( x < mColCount )
				{
					CellHolder* pHolder = (CellHolder*)usedCellsHash.Get( GetCellCode(x,y) );

					if ( !pHolder ||
						 ( pHolder && pHolder->mpConstr != h.mpConstr ) ) break;

					++x;
				}

				actualWidth = x - h.x;
			}

			if ( h.mpConstr->gridheight == GridBagConstraints::RELATIVE ||
				 h.mpConstr->gridheight == GridBagConstraints::REMAINDER )
			{
				++y;

				while( y < mRowCount )
				{
					CellHolder* pHolder = (CellHolder*)usedCellsHash.Get( GetCellCode(x,y) );

					if ( !pHolder ||
						 ( pHolder && pHolder->mpConstr != h.mpConstr ) ) break;

					++y;
				}

				actualHeight = y - h.y;
			}

			h.actualWidth  = actualWidth;
			h.actualHeight = actualHeight;

			// split info contained in holder and constraints among two cell objects:
			// one - carrying vertical info, another -carrying horizontal info

			InitCellFromHolder( *holders[i] );
		}

	}

	// as the actual matrix is set up, get rid of holders now

	for( i = 0; i != holders.Count(); ++i )

		delete holders[i];
}

void GridBagLayout::ClearCachedData()
{
	if ( mpHorizCellInfos ) delete mpHorizCellInfos;
	if ( mpVertCellInfos  ) delete mpVertCellInfos;

	mpHorizCellInfos = NULL;
	mpVertCellInfos  = NULL;
}

bool GridBagLayout::HasCashedInfo()
{
	return mpHorizCellInfos != NULL;
}

void GridBagLayout::SetComponentLocations()
{
	for( int y = 0; y != mRowCount; ++y )

		for( int x = 0; x != mColCount; ++x )
		{
		 	CellInfo& hCell = mpHorizCellInfos[ y * mColCount + x ]; // note the trick...
			CellInfo& vCell = mpVertCellInfos [ x * mRowCount + y ]; // -/-

			if ( hCell.mpComp != NULL )
			{
				hCell.mpComp->setLocation( hCell.finalCompPos, vCell.finalCompPos );

				hCell.mpComp->setSize( hCell.finalCompSize, vCell.finalCompSize );
			}
		}
} 