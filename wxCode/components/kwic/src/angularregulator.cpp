///////////////////////////////////////////////////////////////////////////////
// Name:        kwic_rotary.cpp
// Purpose:     wxIndustrialControls Library
// Author:      Marco Cavallini <m.cavallini AT koansoftware.com>
// Copyright:   (C)2004-2007 Copyright by Koan s.a.s. - www.koansoftware.com
// Licence:     KWIC License http://www.koansoftware.com/kwic/kwic-license.htm
///////////////////////////////////////////////////////////////////////////////
// History:
// 
///////////////////////////////////////////////////////////////////////////////

//#include "kwic_prec.h"		

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include <math.h>
#include "kwic/angularregulator.h"

IMPLEMENT_DYNAMIC_CLASS(kwxAngularReg, wxControl)

BEGIN_EVENT_TABLE(kwxAngularReg, wxControl)
	EVT_MOUSE_EVENTS(kwxAngularReg::OnMouse)
	EVT_KEY_DOWN(kwxAngularReg::OnKeyDown)
	EVT_PAINT(kwxAngularReg::OnPaint)
	EVT_ERASE_BACKGROUND(kwxAngularReg::OnEraseBackGround)
END_EVENT_TABLE()


kwxAngularReg::kwxAngularReg(wxWindow* pParent,
		                   		const wxWindowID iId,
		                   		const int iMinValue,
						   		const int iMaxValue,		
                           		const wxPoint& rPos,
                           		const wxSize & rSize,
		                   		const long int iStyle)
	: kwxRegulatorBase(pParent,iId,iMinValue,iMaxValue,rPos,rSize,iStyle)
{
	if (pParent)
		SetBackgroundColour(pParent->GetBackgroundColour());
	else
		SetBackgroundColour(*wxLIGHT_GREY);    

	m_iClientWidth  = rSize.GetWidth() ;
	m_iClientHeight = rSize.GetHeight() ;
			
	m_iBorderWidth  = m_iClientWidth/10;		// width of outer rotary buttons border 
	
	m_iKnobWidth = m_iClientWidth/20;			// width of rotary buttons knob
	
	m_iAngleStart= -45;							// usable angular range of control, starting at left border
	m_iAngleEnd	 = 225;
	
	m_cExtCircle = (wxColour*)wxLIGHT_GREY ;	// init colors
	m_cIntCircle = (wxColour*)wxBLUE ;

	//m_cLimitsColour = (wxColour*)wxBLACK ;

	m_cKnobBorderColour = (wxColour*)wxLIGHT_GREY ;
	m_cKnobColour = (wxColour*)wxBLACK ;;
	
	m_cValueColour= *wxLIGHT_GREY ;
	
												// init bitmap buffer
	m_pMembitmap = new wxBitmap(rSize.GetWidth(), rSize.GetHeight()) ;
	
    SetAutoLayout(TRUE);
	Refresh();
}

kwxAngularReg::~kwxAngularReg(){	
		
	//std::cout << "bye angular reg " << GetId() << std::endl;			// check destruction
}

void kwxAngularReg::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxPaintDC dc(this);

							// create a memory DC
	wxMemoryDC temp_dc;
	temp_dc.SelectObject(*m_pMembitmap);

	temp_dc.SetBackground(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour(),wxSOLID));
	temp_dc.Clear();

//////////////////////////////////////////////////////
	temp_dc.SetPen(  *wxThePenList->FindOrCreatePen(	*wxBLACK, 1,  wxSOLID));
	temp_dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(*m_cExtCircle,wxSOLID));
	
	
	temp_dc.DrawCircle(m_iClientWidth / 2 , m_iClientHeight / 2, m_iClientHeight / 2 -1) ;
//////////////////////////////////////////////////////
	temp_dc.SetPen(*wxThePenList->FindOrCreatePen(*wxBLACK, 1, wxSOLID));
	temp_dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(*m_cIntCircle, wxSOLID));

	temp_dc.DrawCircle(m_iClientWidth / 2, m_iClientHeight / 2, m_iClientHeight / 2 - m_iBorderWidth) ;

	DrawLimit(temp_dc) ;	//limiti

	if (m_iTagsCount >0 )
		DrawTags(temp_dc);	//tags

	DrawKnob(temp_dc) ;		//knob
	
	if (m_iStyle & STYLE_DRAW_CURRENT)
		DrawCurrent(temp_dc);

							// copy from this DC to another DC.
	dc.Blit(0, 0, m_iClientWidth, m_iClientHeight, &temp_dc, 0, 0);

}

void kwxAngularReg::SetValue(int iVal)
{
	iVal = iNormalizeValue(iVal);						// check bounds
	int deltaRange = m_iMax - m_iMin;
	int deltaAngle = m_iAngleEnd - m_iAngleStart;

	double coeff = (double)deltaAngle / (double)deltaRange;

	double ang = 360 + ((iVal * coeff) + m_iAngleStart);
	
	ang = (ang * PGRECO) / 180;
	m_dxi = cos(ang) * ((m_iClientHeight / 2) - 22) ; 	//coordinate centro knob
	m_dyi = sin(ang) * ((m_iClientHeight / 2) - 22) ;
	
	m_iRealVal = iVal;
	Refresh();
}


void kwxAngularReg::OnMouse(wxMouseEvent& event)
{
	if (m_iStato == LEFT && event.Entering())			//dentro nel controllo
		m_iStato = ENTERED ;
	else if (m_iStato >= ENTERED && event.Leaving())	//fuori dal controllo
		m_iStato = LEFT ;
	else if (m_iStato == ENTERED && event.ButtonDClick()){// process double click	
		m_mousePosition = event.GetPosition();
		if( (m_iSnapRange<1) && 
			(m_iStyle & STYLE_SNAP_DOUBLE_CLK)	){		// if no snapping was set..
				vSetSnap2Tags(true);					// ..snap to next tag.
				SetPosition() ;
				vSetSnap2Tags(false);
				vSendEvent();
				return;
		}
		SetPosition() ;
		return;
	}	
	else if (m_iStato == ENTERED && event.LeftDown()){	//click o inizio trascinamento	
		m_iStato = DRAGED;
		m_mousePosition = event.GetPosition();
		if( (m_iStyle & STYLE_EDITABLE) ){				// if control is editable 
			if(m_cCurrentRect.Contains(m_mousePosition)){// mouse click was within text sourrounding rectangle
				m_bEditing = !m_bEditing;				// toggle edit mode
				m_iCaretPosition = 0;					// reset edit caret position
			}
			else if(m_bEditing){
				m_bEditing = false;						// any click on non text area -> no edit mode
														// set value
				SetValue(iConvert(m_strCurrent));
			}
		}
		SetPosition() ;
		vSendEvent();
	}
	else if (m_iStato == ENTERED && event.GetWheelRotation())// process wheel event
	{
		int iVal = m_iRealVal + ( (m_iMax-m_iMin) * event.GetWheelRotation() / (event.GetWheelDelta() * m_iBigStep ) );
		SetValue( iNormalizeValue(iVal) );  
		vSendEvent();
	}
	else if (m_iStato == DRAGED && event.LeftIsDown())	//trascinamento
	{
		m_mousePosition = event.GetPosition();
		SetPosition() ;
		vSendEvent();
	}
	else if (m_iStato == DRAGED && event.LeftUp())		//fine trascinamento o rilascio click
		m_iStato = ENTERED ;
}


void kwxAngularReg::DrawKnob(wxDC &temp_dc)
{
	double x, y ;

	x = ((m_iClientHeight ) / 2) - m_dxi ;
	y = ((m_iClientHeight ) / 2) - m_dyi ; 
	

	temp_dc.SetPen(	 *wxThePenList->FindOrCreatePen(	*m_cKnobBorderColour, 1, wxSOLID));
	temp_dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(*m_cKnobColour,wxSOLID));

	temp_dc.DrawCircle( (int)x, (int)y, m_iKnobWidth) ;
}

void kwxAngularReg::DrawLimit(wxDC &temp_dc)
{
	double dLength = m_iBorderWidth * 0.8;
	int anglestart = m_iAngleStart ;
	//Start
	if (anglestart < 0 )
		anglestart = 360 + anglestart ;

	double dAng = (anglestart * PGRECO) / 180 ; 				//radianti parametro angolo

	double sx = cos(dAng) * ((m_iClientHeight / 2) ) ; 			//coordinate limite start
	double sy = sin(dAng) * ((m_iClientHeight / 2) ) ;

	double dx = cos(dAng) * ((m_iClientHeight / 2) - dLength) ; //coordinate limite start (7)
	double dy = sin(dAng) * ((m_iClientHeight / 2) - dLength) ;

	
	temp_dc.SetPen(*wxThePenList->FindOrCreatePen(m_cLimitColour, 2, wxSOLID));

	temp_dc.DrawLine((m_iClientHeight / 2) - (int)sx , (m_iClientHeight / 2) - (int)sy ,
					 (m_iClientHeight / 2) - (int)dx , (m_iClientHeight / 2) - (int)dy ) ;
					 
	

	dAng = (m_iAngleEnd * PGRECO) / 180 ; 				 //radianti parametro angolo

	sx = cos(dAng) * ((m_iClientHeight / 2) ) ; 		 //coordinate limite start
	sy = sin(dAng) * ((m_iClientHeight / 2) ) ;

	dx = cos(dAng) * ((m_iClientHeight / 2) - dLength) ; //coordinate limite start
	dy = sin(dAng) * ((m_iClientHeight / 2) - dLength) ;

	temp_dc.DrawLine((m_iClientHeight / 2) - (int)sx , (m_iClientHeight / 2) - (int)sy ,
					 (m_iClientHeight / 2) - (int)dx , (m_iClientHeight / 2) - (int)dy ) ;
					 
}

void kwxAngularReg::DrawTags(wxDC &temp_dc)
{
	int iTagCount ;

	int deltaRange = m_iMax - m_iMin;
	int deltaAngle = m_iAngleEnd - m_iAngleStart;

	double sx, sy, dx, dy ;
	
	double dLength= m_iBorderWidth * 0.6;
	double dCoeff  = (double)deltaAngle / (double)deltaRange;

	double dAngle ;

	temp_dc.SetPen(*wxThePenList->FindOrCreatePen(m_cTagsColour, 1, wxSOLID));

	for (iTagCount = 1 ; iTagCount < m_iTagsCount ; iTagCount++){
		dAngle = 360 + ((m_aTagsVal[iTagCount] * dCoeff) + m_iAngleStart);	
		dAngle = (dAngle * PGRECO) / 180;

		sx = cos(dAngle) * ((m_iClientHeight / 2) ) ; 			//coordinate limite start
		sy = sin(dAngle) * ((m_iClientHeight / 2) ) ;

		dx = cos(dAngle) * ((m_iClientHeight / 2) - dLength) ; 	//coordinate limite start
		dy = sin(dAngle) * ((m_iClientHeight / 2) - dLength) ;

		temp_dc.DrawLine( (m_iClientHeight / 2) - (int)sx , (m_iClientHeight / 2) - (int)sy ,
						  (m_iClientHeight / 2) - (int)dx , (m_iClientHeight / 2) - (int)dy ) ;
	}
}

//#if 0
/////Disegna valore corrente
//void kwxAngularReg::DrawCurrent(wxDC &dc)
//{
//	int w, h;
//	int tw,th;
//	int ix,iy;
//	int iw,ic;
//
//	const wxBrush& cOldBrush = dc.GetBackground();	// remember brush
//	int iOldMode = dc.GetBackgroundMode();			// remember mode
//	GetClientSize(&w, &h);
//													//valore attuale
//	if(!m_bEditing){								// create text string only in non edit mode
//		if(m_iDivisor<=1){							// no fixed - point presentation
//			m_strCurrent.Printf("%d", m_iRealVal );
//		}	
//		else{										// fixed point presentation
//			wxString format ="%d.%0_d";				// format string
//			format[5] = m_iPostPointNumbers+'0'; 	// replace '_' by post point number count
//			m_strCurrent.Printf(
//						format.c_str(), 			// create final string
//					 	m_iRealVal / m_iDivisor, 	// pre point part
//					 	m_iRealVal % m_iDivisor 	// post point fraction
//					 	);
//		}
//	}
//	dc.SetFont(m_Font);								// \todo adjust underline & text position according to font size
//	dc.GetTextExtent(m_strCurrent+m_strPostfix, &tw, &th);// get text dimensions
//	ix = w / 2 - tw / 2 ;
//	if(m_iStyle & STYLE_TEXT_CENTER)
//		iy = h / 2 - th / 2;
//	if(iType()==TYPE_ROTARY)
//		iy = h - th - m_iTextOffset;	
//	else
//		iy = h / 2 - th / 2;
//	//printf(" tw = %i , iw = %i\n",tw,iw);
//	if(m_bEditing){									// in edit mode edit string m_strCurrent is displayed 
//		dc.SetTextForeground(m_cEditColour );		// set text foreground to edit colour
//		const wxPen & cOldPen = dc.GetPen();		// save current pen
//		
//		if( STYLE_RECT_ON_EDIT & m_iStyle){			// draw sourrounding rectangle 
//			dc.SetBackground(wxBrush(wxColour(127,127,127)));// edit rectangle back ground	
//			dc.SetBackgroundMode(wxSOLID);
//			dc.SetPen(wxPen(*wxWHITE));				// set pen for rectangle border			
//			dc.DrawRectangle(wxRect(ix-1,iy-1,tw+2,th+2));			
//		}											// draw text
//					
//		dc.DrawText( m_strCurrent+m_strPostfix, ix ,iy );
//		dc.SetPen(wxPen(m_cEditColour));			// set pen also to edit colour
//		
//		dc.GetTextExtent(m_strCurrent.Left(m_iCaretPosition), &iw, NULL);	// get text width before caret 
//		dc.GetTextExtent(m_strCurrent.Left(m_iCaretPosition+1), &ic, NULL);	// get width incl. caret
//													// draw edit underline at caret position \todo adjust underline position according to font size												
//		dc.DrawLine( iw + ix , iy+th-1, ic + ix  , iy+th-1 );
//		dc.SetPen(cOldPen);
//	}
//	else{											// draw current value in non edit mode
//		dc.SetTextForeground(m_cValueColour);
//		dc.DrawText(m_strCurrent+m_strPostfix, ix ,iy );
//	}		
//	m_cCurrentRect = wxRect(ix,iy,tw,th);  			// save text rectangle
//	dc.SetBackgroundMode(iOldMode);					// restore mode
//	dc.SetBackground(cOldBrush);					// restore brush
//}
//#endif

void kwxAngularReg::SetPosition()
{
	int x, y ;
	double ang ;
	int deltaRange, deltaAngle ;
	double val ;

	x = m_mousePosition.x ;
	y = m_mousePosition.y ;
	
	ang = GetAngleFromCord(x, y) ; //IN RADIANTI

	val = (ang * 180) / PGRECO; //radianti parametro angolo

	deltaRange = m_iMax - m_iMin;
	deltaAngle = m_iAngleEnd - m_iAngleStart;

	double coeff = (double)deltaAngle / (double)deltaRange;

	//OK
	if( m_iAngleStart < 0 && val >= (360 + m_iAngleStart) )
		m_dScaledVal = (double)(val  - (360 + m_iAngleStart)) / coeff;
	else
		m_dScaledVal = (double)(val  - m_iAngleStart) / coeff;
	
	//vale sempre fuori dall'intervallo ma non si sa da dove si arriva
	//salviamo ultima posizione e ricarichiamo
	if(m_dScaledVal > m_iMax || m_dScaledVal < m_iMin)	{
		ang = m_dOldAng ;
		
	}
	else
	{
		m_iRealVal = (int)(ceil(m_dScaledVal)) ;
		if(bSnap(m_iRealVal)){							// check for snap
			SetValue(m_iRealVal);
			//std::cout << "corrected\n";
		}
		else{
			m_dxi = cos(ang) * ((m_iClientHeight / 2) - 22) ; //coordinate centro knob 2
			m_dyi = sin(ang) * ((m_iClientHeight / 2) - 22) ;
		}
	}	
	
//	wxLogTrace("angolo: %f scalato: %f",val, m_nScaledVal);

	m_dOldAng = ang ;

	Refresh() ;

}

///Ritorna l'angolo in radianti della posizione del mouse
double kwxAngularReg::GetAngleFromCord(int cx, int cy)
{

	double x, y, ang = 0 ;

	y = ((m_iClientHeight / 2) - (double)cy)  / (m_iClientHeight / 2); 
	x = ((double)cx - (m_iClientWidth / 2)) / (m_iClientHeight / 2);

	ang -= (atan2(-y, -x)) ; // IN RADIANTI

	if (ang < 0 )
		ang += 2 * PGRECO ;

	return  ang ;
}


/*!
 * 	vSetAngle - set usable angular range of control
 * \param iMin -> start angle in degrees
 * \param iMax -> end   angle in degrees 
 * \note intersection of left border of control with its horizontal axis means zero degree. \n
 * deegres count clockwise.
 * \author Marco Cavallini <m.cavallini AT koansoftware.com> 
 * \date 13.06.2007 modified Andreas Leidig <leidig@fs.wettzell.de>
 */
void kwxAngularReg::vSetAngle(int iMin, int iMax) {
	m_iAngleStart = iMin ; 
	m_iAngleEnd   = iMax ; 
}


/*!
 * vSetBorderWidth - modify width of rotary buttons outer border
 * \param iBorderWidth -> the border width in pixels
 * \author Andreas Leidig <leidig@fs.wettzell.de>
 * \date 05.06.2007
 */
void kwxAngularReg::vSetBorderWidth(int iBorderWidth){
	m_iBorderWidth = iBorderWidth;
}


/*!
 * vSetKnobWidth - modify width of rotary buttons knob
 * \param iBorderWidth -> the knob width in pixels
 * \author Andreas Leidig <leidig@fs.wettzell.de>
 * \date 05.06.2007
 */
void kwxAngularReg::vSetKnobWidth(int iKnobWidth){
	m_iKnobWidth = iKnobWidth;
}

/*!
* with this function the range of can be set
*/
void kwxAngularReg::vSetRange(const int iMin, const int iMax)
{
	m_iMin = iMin;
	m_iMax = iMax;

}
