/////////////////////////////////////////////////////////////////////////////
/// @file        wxRubberBand.h
/// @brief       Definition of a Rubber Band class
///				 This is mainly used for drawing a selection box arround something
/// @Author      Lucien Schreiber
//  Modified by
//  Created      12/10/2006
//  RCS-ID:
//  Copyright:   (c) Lucien Schreiber CREALP 2006
//  Licence:     wxWindows licence
//  @version     1.0
/////////////////////////////////////////////////////////////////////////////

#ifndef __WXRUBBERBAND_H__
#define __WXRUBBERBAND_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "wxRubberBand.h"
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/wxprec.h>
#include <wx/dcclient.h>
///\brief Class used for drawing a rubber band.
/** This class was created for drawing a selection box 
* it is a very simple one. 
* We draw a rectangle using lines despite rectangle to avoid flickering
* \warning For the moment only positive rectangle are supported
*/
class wxRubberBand
{
	private:
		/** \brief [PRIVATE MEMBER] used for storing last rectangle coordinates */
		wxRect theOldRubberRect;
		/** \brief [PRIVATE FUNCTION] used during wxRubberBand initialisation */
		void RubberInitValues ();
		/** \brief [PRIVATE MEMBER] used for window storing */
		wxWindow * theWnd;
		/** \brief [PRIVATE FUNCTION] used for lines drawing 
		*
		* We draw lines and not rectangle directely to avoid filckering. This function is called
		* from the SetGeometry One*/
		void DrawRectangleLines (wxDC *MyDC,int x, int y, int width, int height);
		/** \brief [PRIVATE MEMBER] we store the pen */
		wxPen thePen;
		/** \brief [PRIVATE FUNCTION] Line verification against inverse 
		*selection rectangle 
		*
		* The following verification are made : x et y must not be < 0
		* width and height must be > 0 
		* \return TRUE if all verifications are good*/
		bool VerifyRubberLines (int x, int y, int width, int height);



	public:
		/** \brief default constructor */
		wxRubberBand();
		/**\brief wxRubberBand Constructor */
		wxRubberBand(wxWindow * wnd);
		/**\brief wxRubberBand Destructor */
		~wxRubberBand();
		/** \brief Resize the rubber band */
		void SetGeometry (int x, int y, int width, int height);
		/** \brief Set the default pen */
		void SetPen ();
		/** \brief Set an specific pen */
		void SetPen (wxPen &pen);
		/** \brief Clear theOldRubberRect needed each time
		* the mouse button is released
		*
		* This function is needed when used try to change 
		* the pen for exemple. */
		void ClearOldRubberRect ();


};


#endif
