	
#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

//#include "kwic_prec.h"

#include <wx/event.h>
#include <math.h>

#include "kwic/slider.h"


IMPLEMENT_DYNAMIC_CLASS(kwxLinearReg, wxControl)

BEGIN_EVENT_TABLE(kwxLinearReg,wxControl)
	EVT_MOUSE_EVENTS(kwxLinearReg::OnMouse)
	EVT_PAINT(kwxLinearReg::OnPaint)
	EVT_ERASE_BACKGROUND(kwxLinearReg::OnEraseBackGround)
	EVT_KEY_DOWN(kwxLinearReg::OnKeyDown)
END_EVENT_TABLE()


/*!
 * constructor kwxLinearReg
 * \param pParent 	-> pointer to parent control
 * \param iId	  	-> window id
 * \param iMinValue -> lower bound of displayable range
 * \param iMaxValue -> upper bound of displayable range
 * \param rPos	    -> position of controls' top left corner
 * \param rSize	    -> dimension of control
 * \param iStyle	-> window Style
 * \date 24.04.2007 - modified by Andreas Leidig <leidig@fs.wettzell.de>
 */
kwxLinearReg::kwxLinearReg(	wxWindow	  *  pParent,		                   
						const wxWindowID iId,
						const int 		 iMinValue,
						const int 		 iMaxValue,				
           				const wxPoint &  rPos,
          				const wxSize  &  rSize,
         				const long int 	 iStyle)
	: 
	kwxRegulatorBase(pParent,iId,iMinValue,iMaxValue,rPos,rSize,iStyle)	,
	m_iScaledVal(0),		
	m_iArrowWidth(ARROW_WIDTH),
	m_iArrowHeigth(ARROW_HEIGHT)
{	
	m_ControlSize 	= rSize;					// init size
												
	m_pActiveBarColour 	= (wxColour*)			// init colors
									 wxLIGHT_GREY;  
	m_pPassiveBarColour = (wxColour*)wxWHITE;
	m_pBorderColour 	= (wxColour*)wxBLACK;
	m_pNeedleColour		= (wxColour*)wxRED  ;

	m_pMembitmap = new wxBitmap(rSize.GetWidth(),// init memory bitmap	
							 	rSize.GetHeight());
}


kwxLinearReg::~kwxLinearReg(){
	
	//std::cout << "bye slider " << GetId() << std::endl;			// check destruction
}


void kwxLinearReg::OnPaint(wxPaintEvent &WXUNUSED(event))
{
	wxPaintDC old_dc(this);
	
	int w,h;
	int yPoint, rectHeight ;
	
	GetClientSize(&w,&h);

	wxMemoryDC dc;
	dc.SelectObject(*m_pMembitmap);


	dc.SetBackground(*wxTheBrushList->FindOrCreateBrush(*m_pPassiveBarColour,wxSOLID));
	dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(*m_pPassiveBarColour,wxSOLID));
	dc.Clear();
	///////////////////////////////////////////////////////////////////////////

	dc.SetPen(*wxThePenList->FindOrCreatePen(*m_pBorderColour, 1, wxSOLID));
	dc.DrawRectangle(0, 0, w, h);

	dc.SetPen(*wxThePenList->FindOrCreatePen(*m_pActiveBarColour, 1, wxSOLID));
	dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(*m_pActiveBarColour,wxSOLID));
	
	if((!(m_iStyle&STYLE_VERTICAL)))
		dc.DrawRectangle(1, 1, m_iScaledVal, h - 2);
	else
	{
		yPoint = h - m_iScaledVal ;
		if (m_iScaledVal == 0 )
			rectHeight = m_iScaledVal ;
		else
		{
			if (m_iRealVal == m_iMax)
			{
				rectHeight = m_iScaledVal;
				yPoint -= 1 ;
			}
			else
				rectHeight = m_iScaledVal - 1 ;
		}

		dc.DrawRectangle(1, yPoint, w - 2, rectHeight);
	}
	if (m_iStyle & STYLE_METER) {
		DrawNeedle(dc);		// draw needle
	}
	if (m_iStyle & STYLE_DRAW_CURRENT)
		DrawCurrent(dc);	//valore attuale
	
	if (m_iStyle & STYLE_DRAW_LIMITS)
		DrawLimits(dc);		//valore minimo e massimo
	
	if (m_iTagsCount > 0 )	
		DrawTags(dc) ;		// draw tags

	// We can now draw into the memory DC...
	// Copy from this DC to another DC.
	old_dc.Blit(0, 0, w, h, &dc, 0, 0);

}

void kwxLinearReg::OnMouse(wxMouseEvent& event)
{
	if (m_iStato == 0 && event.Entering())						//dentro nel controllo
		m_iStato = ENTERED ;
	else if (m_iStato >= ENTERED && event.Leaving())			//fuori dal controllo
		m_iStato = LEFT ;
	else if (m_iStato == ENTERED && event.GetWheelRotation())	// process wheel event
	{
		//SetValue(m_iRealVal + (event.GetWheelRotation() * (m_iMax-m_iMin)/kwxLinearReg::MOUSE_STEP / event.GetWheelDelta()) );    	
		SetValue(m_iRealVal + ( m_iBigStep * event.GetWheelRotation()  / event.GetWheelDelta()) );    	
    	vSendEvent();												// send event
		return;
	}
	else if (m_iStato == ENTERED && event.ButtonDClick())		// process double click
	{
//		wxLogTrace("Mouse Premuto") ;
		m_mousePosition = event.GetPosition();
		
		if( (m_iSnapRange<1) && 
			(m_iStyle & STYLE_SNAP_DOUBLE_CLK)	){				// if no snapping was set..
				vSetSnap2Tags(true);							// ..snap to next tag.
				DrawPosition() ;
				vSetSnap2Tags(false);
				return;
		}
		DrawPosition() ;
	}	
	else if (m_iStato == ENTERED && event.LeftDown())			//click o inizio trascinamento
	{
//		wxLogTrace("Mouse Premuto") ;
		m_iStato = DRAGED ;
		if( (m_bEditing) ){										// if control is currently in edit mode 
			if(m_cCurrentRect.Contains(m_mousePosition)){		// mouse click was within text sourrounding rectangle
				m_bEditing 		 = false;						// exit edit mode
				m_iCaretPosition = 0;							// reset edit caret position
				SetValue(iConvert(m_strCurrent));
				Refresh() ;
				vSendEvent() ;
				return;
			}
		}
		m_mousePosition = event.GetPosition();
		DrawPosition() ;
		
		
	}	
	else if (m_iStato == DRAGED && event.LeftIsDown())			//trascinamento
	{
//		wxLogTrace("Mouse trascinato") ;
		m_mousePosition = event.GetPosition();
		DrawPosition() ;
	}
	else if (m_iStato == DRAGED && event.LeftUp())				//fine trascinamento o rilascio click
	{
//		wxLogTrace("Mouse rilasciato") ;
		m_iStato = ENTERED ;
	}
	event.Skip(false);// 
}

void kwxLinearReg::DrawPosition(){
	double coeff = 0 ;
	bool draw = true ;

	if ((!(m_iStyle&STYLE_VERTICAL))){
		m_iScaledVal = m_mousePosition.x ;
		if ( m_iScaledVal < 0 || m_iScaledVal >= m_ControlSize.GetWidth() - 1)
			draw = false ;
		else
			coeff = (m_ControlSize.GetWidth() - 2) / (double)(m_iMax - m_iMin);
	}
	else{
		m_iScaledVal = m_ControlSize.GetHeight() - m_mousePosition.y;
		if ( m_iScaledVal < 0 || m_iScaledVal >= m_ControlSize.GetHeight() - 1)
			draw = false ;
		else
			coeff = (m_ControlSize.GetHeight() - 2) / (double)(m_iMax - m_iMin);
	}

	if (m_iRealVal <= m_iMax && draw){
		m_iRealVal = (int)floor(m_iScaledVal / coeff) ;	// \todo : need floor ?
		if(bSnap(m_iRealVal))							// check for snap
			m_iScaledVal = (int)(m_iRealVal * coeff);
		Refresh() ;
		vSendEvent() ;
	}
}

//per inizializzazione
void kwxLinearReg::SetValue(int val) 
{ 
	int w,h;
	double coeff;
	if(val<m_iMin)									// no wrap over
		val=m_iMin;
	else if(val>m_iMax)
		val=m_iMax;
	
	GetClientSize(&w,&h);

	if((!(m_iStyle&STYLE_VERTICAL)))
		coeff = (w - 2) / (double)(m_iMax - m_iMin);
	else
		coeff = (h - 2) / (double)(m_iMax - m_iMin);
		
	bSnap(val);									 	 // snap 
	m_iScaledVal = (int)ceil((val - m_iMin) * coeff);// \todo: need ceil ?
	 
	m_iRealVal = val;
	
	Refresh();
}

///////////////////////////////
//	FUNZIONI DISEGNO CUSTOM	 //		
///////////////////////////////

//#if 0
//#ifdef _USE_OLD_
////Disegna valore corrente
//void kwxLinearReg::DrawCurrent(wxDC &dc)
//{
//	int w,h;
//	int tw,th;
//	wxString s;
//	
//	GetClientSize(&w, &h);
//
//	//valore attuale
//	if(m_iDivisor<=1){						// no fixed - point presentation
//		s.Printf("%d%s", m_iRealVal, m_strPostfix.c_str() );
//	}	
//	else{
//		wxString format ="%d.%0_d%s";		// format string
//		format[5] = m_iPostPointNumbers+48; // replace '_' by post point number count
//		s.Printf(format.c_str(), 			// create final string
//				m_iRealVal/m_iDivisor, m_iRealVal%m_iDivisor, m_strPostfix.c_str() );
//	}
////	else if(m_iDivisor<=10){// one post - point number
////		s.Printf("%d.%01d%s", m_iRealVal/m_iDivisor, m_iRealVal%m_iDivisor, m_strPostfix.c_str() );
////	}
//
//	dc.GetTextExtent(s, &tw, &th);
//	dc.SetTextForeground(m_cValueColour);
//	dc.DrawText(s, w / 2 - tw / 2 , h / 2 - th / 2);
//}
//#else
/////Disegna valore corrente
//void kwxLinearReg::DrawCurrent(wxDC &dc)
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
//	iy = h / 2 - th / 2;	
//	
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
//
//#endif
//#endif
//Disegna limiti controllo
void kwxLinearReg::DrawLimits(wxDC &dc)
{
	int w,h;
	int tw,th;
	wxString s;

	GetClientSize(&w, &h);
	
	dc.SetFont(m_Font);
	dc.SetTextForeground(m_cLimitColour);

	if((!(m_iStyle&STYLE_VERTICAL)))
	{
		//valore minimo
		s.Printf("%d", m_iMin);	
		dc.GetTextExtent(s, &tw, &th);
		dc.DrawText(s, 5, h / 2 - th / 2);

		//valore massimo
		s.Printf("%d", m_iMax);
		dc.GetTextExtent(s, &tw, &th);
		dc.DrawText(s,w - tw - 5, h / 2 - th / 2);
	}
	else
	{
		//valore minimo
		s.Printf("%d", m_iMin);	
		dc.GetTextExtent(s, &tw, &th);
		dc.DrawText(s, w / 2 - tw / 2, h - th - 5);

		//valore massimo
		s.Printf("%d", m_iMax);
		dc.GetTextExtent(s, &tw, &th);
		dc.DrawText(s, w / 2 - tw / 2, 5);
	}

}

//Disegna i tag impostati
void kwxLinearReg::DrawTags(wxDC &dc)
{	
	int ntag = 0 ;
	int w, h ;
	int tw,th;
	int scalval = 0 ;
	double tcoeff ;

	wxString text ;

	if(m_aTagsVal==NULL)
		return;
	GetClientSize(&w,&h);
	if((!(m_iStyle&STYLE_VERTICAL)))
		tcoeff = (w - 2) / (double)(m_iMax - m_iMin);
	else
		tcoeff = (h - 2) / (double)(m_iMax - m_iMin);

	dc.SetPen(*wxThePenList->FindOrCreatePen(m_cTagsColour, 1, wxSOLID));
	dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(m_cTagsColour,wxSOLID));
	dc.SetTextForeground(m_cTagsColour);

	while (ntag < m_iTagsCount)
	{
		scalval = (int)floor((m_aTagsVal[ ntag] - m_iMin) * tcoeff);// \todo: need floor ?
		text.Printf("%d", m_aTagsVal[ ntag]) ;		
		
		if(m_iStyle & STYLE_VERTICAL)
		{
			dc.DrawLine(w - 2, h - scalval , w - TAGS_LENGTH , h - scalval);
			if(m_iStyle & STYLE_DRAW_TAG_NUMBERS){
				dc.GetTextExtent(text, &tw, &th);
				dc.DrawText(text, w - TAGS_LENGTH - tw, h - scalval - (th / 2) );
			}
		}
		else
		{
			dc.DrawLine(scalval + 1, h - 2 , scalval + 1, h - TAGS_LENGTH);
			if(m_iStyle & STYLE_DRAW_TAG_NUMBERS){
				dc.GetTextExtent(text, &tw, &th);
				dc.DrawText(text, scalval + 1 - (tw / 2 ), h - TAGS_LENGTH - th);
			}
		}
		ntag++ ;
	}
}

// some tuning..

void kwxLinearReg::DrawNeedle(wxDC &dc){
	
	int w, h ;
	int scalval = 0 ;
	double tcoeff ;	
	wxPoint points[3];
	
	GetClientSize(&w,&h);
	
	if(!(m_iStyle&STYLE_VERTICAL))
		tcoeff = (w - 2) / (double)(m_iMax - m_iMin);
	else
		tcoeff = (h - 2) / (double)(m_iMax - m_iMin);

	dc.SetPen(	*wxThePenList->FindOrCreatePen(*m_pNeedleColour, 1, wxSOLID));
	dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(*m_pNeedleColour,wxSOLID));
	
	scalval = (int)floor((m_iRealVal - m_iMin) * tcoeff);// \todo: need floor ?
	
	if(!(m_iStyle&STYLE_VERTICAL))// horizontal style
	{
		if(m_iStyle & STYLE_METER)
			dc.DrawLine(scalval + 1, 2 , scalval + 1, h - 2);
		if(m_iStyle & STYLE_ARROW){
			points[0].x = scalval + 1 ;				  	  //tip
			points[0].y = h - TAGS_LENGTH ;
			points[1].x = scalval + 1 	  + m_iArrowWidth/2;//right
			points[1].y = h - TAGS_LENGTH - m_iArrowHeigth ;
			points[2].x = scalval + 1 	  - m_iArrowWidth/2;//left
			points[2].y = h - TAGS_LENGTH - m_iArrowHeigth ;
			dc.DrawPolygon(3,points);
		}
	}
	else			// vertical style
	{	
		if(m_iStyle & STYLE_METER)
			dc.DrawLine(2, h - scalval , w - 2 , h - scalval);	
		if(m_iStyle & STYLE_ARROW){
			points[0].x = w - TAGS_LENGTH ;
			points[0].y = h - scalval + 1 ;				    //tip
			points[1].x = w - TAGS_LENGTH - m_iArrowWidth;//up
			points[1].y = h - scalval  	  - m_iArrowHeigth/2 ;
			points[2].x = points[1].x;//left
			points[2].y = h - scalval  	  + m_iArrowHeigth/2 ;
			dc.DrawPolygon(3,points);
		}	
	}	
}

/*!
 * vSetArrow set arrow dimensions 
 * \param iWidth  -> width  of arrow
 * \param iHeight -> height of arrow
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	08.05.2007
 * \note useful only when style 0x02=arrow was set
 * \see vSetStyle()
 */
void kwxLinearReg::vSetArrow(const int iWidth,const int iHeight){
	m_iArrowWidth  = iWidth;
	m_iArrowHeigth = iHeight;	
}
