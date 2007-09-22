///////////////////////////////////////////////////////////////////////////////
// Name:        angularregolator.h
// Purpose:     wxIndustrialControls Library
// Author:      Marco Cavallini <m.cavallini AT koansoftware.com>
// Copyright:   (C)2004-2006 Copyright by Koan s.a.s. - www.koansoftware.com
// Licence:     KWIC License http://www.koansoftware.com/kwic/kwic-license.htm
///////////////////////////////////////////////////////////////////////////////
// History:
// Modified by: 20.04.2007 - Andreas Leidig <leidig@wettzell.de>
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef _K_WX_ANG_REG_H_
#define _K_WX_ANG_REG_H_

#ifndef WX_PRECOMP
#  include "wx/wx.h"
#endif

#include "regulatorbase.h"


/// pi
#define PGRECO (4.0 * atan(1.0))


/*!
 * \class  kwxAngularReg
 * \brief  kwxAngularReg - rotary button control widget
 * \note part of kwic - koan wxIndustrialControls \n
 * 	originally known as 'AngularRegulator'
 * \author Marco Cavallini <m.cavallini AT koansoftware.com>
 * \date 24.04.2007 modified by Andreas Leidig <leidig@fs.wettzell.de>
 * <h2>Usage:</h2>
 * <pre>
 * <code>				
 * 	 // event table entry 			
 * 		EVT_KWIC_ANG_REG(CI_ANGULAR_ID, wxFrameDerivedClass::OnAngularReg) 				
 * 	 // example test setup 			
 *		kwxAngularReg * m_pRotary = new kwxAngularReg(
 *				this,						// parent wxWindow
 *				CI_ANGULAR_ID,				// id
 *				0, 100,						// range
 *				wxPoint(iX+20, iY+20 ),		// start point
 *				wxSize(80,80),				// size
 *											// window style
 *				  kwxRegulatorBase::STYLE_DRAW_CURRENT
 *				| kwxRegulatorBase::STYLE_SNAP_DOUBLE_CLK
 *				| kwxRegulatorBase::STYLE_EDITABLE
 *				| kwxRegulatorBase::STYLE_RECT_ON_EDIT 
 *				);	
 * 		m_pRotary->vSetTagsCount(5);
 * 		m_pRotary->vSetPostfix("%");
 * 		m_pRotary->SetValue(0) ;	
 * </code>
 * </pre>
 *	
 * Modification History:
 *	
 *	22.06.2007: -> Added a function vSetRange(const int iMin, const int iMax);
 *				-> Made a few modification in order to avoid compilerwarnings
 *				   (used gcc-3.3.6 compilerflags: -W -Wall -ansi)
 *				-> Added a function to set the limitscolour : SetLimitsColour(wxColour *c);
 *				-> Added a function to set the tagcolour: SetTagsColur(wxColour c);
 *				
 *							Ettl Martin (ettl@fs.wettzell.de)
 */
class WXDLLIMPEXP_KWIC kwxAngularReg :  public kwxRegulatorBase
{
public:
	kwxAngularReg(void) { SetId(-1); };

	kwxAngularReg(wxWindow* parent,
                   const wxWindowID id,
                   const int 		 iMinValue,
				   const int 		 iMaxValue,		
                   const wxPoint& pos,
                   const wxSize& size,
                   const long int iStyle) ;
	                   
	virtual ~kwxAngularReg();

	void	SetValue(int val) ;

	void	SetExtCircleColour(const wxColour *colour) { m_cExtCircle = (wxColour*)colour ; } ;
	void	SetIntCircleColour(const wxColour *colour) { m_cIntCircle = (wxColour*)colour ; } ;

	void	SetKnobBorderColour(const wxColour *colour){ m_cKnobBorderColour = (wxColour*)colour ; } ;
	void	SetKnobColour(const wxColour *colour) 	   { m_cKnobColour = (wxColour*)colour ; } ;

	void	SetLimitsColour(const wxColour *colour)	   { m_cLimitsColour = (wxColour*)colour ; } ;

	void	SetTagsColour(const wxColour colour)	   { m_cTagsColour = colour ; } ;
	
	void	vSetAngle(int iMin, int iMax);
	void    vSetRange(const int iMin, const int iMax);	
	void 	vSetBorderWidth(int iBorderWidth);
	void 	vSetKnobWidth(	int iKnobWidth);

	int 	iType(){ return TYPE_ROTARY; }
	
private:

	DECLARE_DYNAMIC_CLASS(kwxAngularReg)
	DECLARE_EVENT_TABLE()

	void	DrawKnob(wxDC &temp_dc) ;
	void	DrawLimit(wxDC &temp_dc) ;
	void	DrawTags(wxDC &temp_dc) ;
	//void 	DrawCurrent(wxDC &temp_dc) ;

	void	SetPosition() ;
	double	GetAngleFromCord(int cx, int cy) ;

	void    OnPaint(wxPaintEvent& event);
	void	OnEraseBackGround(wxEraseEvent& WXUNUSED(event)) {};
	void	OnMouse(wxMouseEvent& event) ;
	

	
	int m_iClientWidth ;
	int m_iClientHeight ;
	
	int m_iBorderWidth;
	int m_iKnobWidth;

	int m_iAngleStart ;
	int m_iAngleEnd ;

	double m_dOldAng ;
	double m_dxi ;
	double m_dyi ;	


	wxColour *m_cExtCircle ;
	wxColour *m_cIntCircle ;

	wxColour *m_cKnobBorderColour ;
	wxColour *m_cKnobColour ;
	//wxColour *m_cTagsColour ;

	wxColour *m_cLimitsColour ;	
	
	wxColour *m_pValueColour ;
	
	
	
} ;

#endif

