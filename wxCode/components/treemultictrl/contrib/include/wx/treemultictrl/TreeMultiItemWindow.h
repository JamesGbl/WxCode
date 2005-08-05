//---------------------------------------------------------------------------
// $RCSfile: TreeMultiItemWindow.h,v $
// $Source: /home/ulrich/cvs-wxcode/wxCode/components/treemultictrl/contrib/include/wx/treemultictrl/TreeMultiItemWindow.h,v $
// $Revision: 1.1 $
// $Date: 2005-08-05 19:05:04 $
//---------------------------------------------------------------------------
// Author:      Jorgen Bodde
// Copyright:   (c) Jorgen Bodde
// License:     wxWidgets License
//---------------------------------------------------------------------------

#ifndef __TREEMULTIITEMWINDOW_HPP_
#define __TREEMULTIITEMWINDOW_HPP_

#ifdef __GNUG__
    #pragma interface "TreeMultiItemWindow.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/dynarray.h>

#include "TreeMultiItemBase.h"

class TreeMultiItemWindow : public TreeMultiItemBase
{
private:
	/** The simple control, or complex sizer that represents this window
		in the TreeMultiCtrl */
	wxWindow *_window;

	/** Top and front extra spacings, added to X, Y */
	int _topSpacing, _frontSpacing;

public:
	TreeMultiItemWindow(TreeMultiItemNode *parent, const wxString &name = wxEmptyString);
	virtual ~TreeMultiItemWindow();

	// IsClass method
	virtual TreeMultiItemWindow *IsTreeMultiItemWindow() const {
		return (TreeMultiItemWindow *)this;
	};
	
	/** Assigns the given window to this TreeMultiItemWindow, and also destroys the 
		present window. If the assigned window is 0, this item is not drawn
		and removed from the tree visually */
	void AssignWindow(wxWindow *wnd);

	/** Get window method */
	wxWindow *GetWindow() {
		return _window;
	};

	/* Sets front spacing */
	void SetFrontSpacing(int frontSpacing) {
		_frontSpacing = frontSpacing;
	};
	/* Gets front spacing */
	int GetFrontSpacing() const {
		return _frontSpacing;
	};
	/* Sets top spacing */
	void SetTopSpacing(int topSpacing) {
		_topSpacing = topSpacing;
	};
	/* Gets top spacing */
	int GetTopSpacing() const {
		return _topSpacing;
	};

#if(CHECKBOXVIEW)
	/** Inherited from base, to enable or disable window */
	virtual void SetCheckboxState(int state);
#endif
};

#endif

