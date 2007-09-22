//////////////////////////////////////////////////////////////////////////////
// Name:        kwic_lin_reg.h
// Purpose:     wxIndustrialControls Library
// Author:      Marco Cavallini <m.cavallini AT koansoftware.com>
// Copyright:   (C)2004-2006 Copyright by Koan s.a.s. - www.koansoftware.com
// Licence:     KWIC License http://www.koansoftware.com/kwic/kwic-license.htm
// Modified by: 23.04.2007 - Andreas Leidig <leidig@fs.wettzell.de>
//////////////////////////////////////////////////////////////////////////////

#ifndef _KWIC_LINREG_H_
#define _KWIC_LINREG_H_

#ifndef WX_PRECOMP
#  include "wx/wx.h"
#endif


#include "kwic/regulatorbase.h"


/*!
 * \class kwxLinearReg
 * \brief kwxLinearReg - linear regulator control widget
 * \author      Marco Cavallini <m.cavallini AT koansoftware.com>
 * \note part of kwic - koan wxIndustrialControls \n
 *  originally known as 'LinearRegulator'			\n
 * \date 12.06.07 modifications by Andreas Leidig <leidig@wettzell.ifag.de>
 * <h2>Usage:</h2>
 * <pre>
 * <code>							
 * 	 // event table entry 			
 * 		EVT_KWIC_LIN_REG(CI_LIN_REG_ID, wxFrameDerivedClass::OnLinReg) 				
 * 	 // example test setup 			
 *		kwxLinearReg * m_pSlider = new kwxLinearReg(wxFrameDerivedClass,// parent
 *  							CI_LIN_REG_ID,		// id							     									
 *  							0,10000,			// range
 *  				   			wxPoint(iX, iY+25),	// start point
 *  				   			wxSize(80,80),		// size
 *  				   				kwxRegulatorBase::STYLE_DRAW_CURRENT 		// window style options
 * 								| kwxRegulatorBase::STYLE_SNAP_DOUBLE_CLK 
 *  				   			| kwxRegulatorBase::STYLE_EDITABLE 		 
 *  				   			| kwxRegulatorBase::STYLE_RECT_ON_EDIT	 
 *  				   			);
 *		m_pSlider->vSetTagsCount(10);				// specify scale tags count
 *		m_pSlider->vSetPostfix("V");				// specify postfix
 *		m_pSlider->vSetPointPosition(3);			// specify number of post point digits
 *		m_pSlider->vSetTxtFontSize(10) ;			// specifiy font size in 'points'
 *		m_pSlider->SetValue(0) ;		
 * </code>
 * </pre>
 *
 * Modification History:
 *	22.06.2007: --> Added a function SetRangeVal(const int iMinValue,const int iMaxValue)
 *				--> Modified the code, to avoid compilerwarnings ( used g++-3.3.6 flags: -W -Wall -ansi)
 *				--> Readded the function SetTxtValueColour(const wxColour *colour)
 *									
 *										Ettl Martin (ettl@fs.wettzell.de)
 *
 */

 
 




		
class WXDLLIMPEXP_KWIC kwxLinearReg : public kwxRegulatorBase
{
public:
	kwxLinearReg(void) { SetId(-1); }
	
	kwxLinearReg(wxWindow *parent,
				const wxWindowID iId        = -1,
				const int 		 iMinValue 	= 0,
				const int 		 iMaxValue 	= 100,
				const wxPoint&   rPos       = wxDefaultPosition,
				const wxSize&    rSize      = wxDefaultSize,
				const long int   iStyle     = STYLE_DRAW_CURRENT);

	virtual ~kwxLinearReg();

	void SetActiveBarColour(const wxColour *colour){ m_pActiveBarColour = (wxColour*)colour ; } 
	void SetPassiveBarColour(const wxColour *colour){m_pPassiveBarColour = (wxColour*)colour ; } 
	void SetBorderColour(const wxColour *colour)   { m_pBorderColour = (wxColour*)colour ; } 

	void SetTxtLimitColour(const wxColour *colour) { m_pLimitColour = (wxColour*)colour ; } 
	void SetTxtValueColour(const wxColour *colour) { m_pValueColour = (wxColour*)colour ; } 
	void SetTagsColour(const wxColour *colour) 	   { m_pTagsColour = (wxColour*)colour ; } 
	void SetNeedleColour(const wxColour *colour)   { m_pNeedleColour = (wxColour*)colour ; } 
	
	void SetRangeVal(const int iMinValue,const int iMaxValue){m_iMin = iMinValue; m_iMax = iMaxValue;};
	void SetValue(int val) ;
		

	void vSetArrow(const int iWidth,const int iHeight);
				
	int iType(){ return TYPE_SLIDER; }				
				
private:

	DECLARE_DYNAMIC_CLASS(kwxLinearReg)
	
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()

	void	OnMouse(wxMouseEvent& event) ;
	void	OnEraseBackGround(wxEraseEvent& WXUNUSED(event)) {}
	void	OnPaint(wxPaintEvent& event);
	//void 	OnKeyDown(wxKeyEvent& event);
	void	DrawPosition() ;
	void	Change();

	void 	DrawLimits(wxDC &dc);
	//void 	DrawCurrent(wxDC &dc);
	void 	DrawTags(wxDC &dc);
	void 	DrawNeedle(wxDC &dc);	
	
	int m_iScaledVal ;
	
	/// arrow dimensions
	int m_iArrowWidth;
	int m_iArrowHeigth;
		
	wxSize 		m_ControlSize ;

	wxColour 	* m_pActiveBarColour ;
	wxColour 	* m_pPassiveBarColour ;
	wxColour 	* m_pBorderColour ;
	wxColour 	* m_pLimitColour ;
	wxColour 	* m_pValueColour ;
	wxColour 	* m_pTagsColour ;
	wxColour 	* m_pNeedleColour ;
	
	enum eLinReg{							
		
		/// arrow dimensions
		ARROW_WIDTH  = 12, 
		ARROW_HEIGHT = 12, 
		
		/// lenght of tag lines
		TAGS_LENGTH  = 6
	};

} ;






#endif


