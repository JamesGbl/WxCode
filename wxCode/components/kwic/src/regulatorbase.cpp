
#include "kwic/regulatorbase.h"


kwxRegulatorBase::kwxRegulatorBase(wxWindow* pParent,
		                   const wxWindowID iId,
		                   const int iMinValue,
						   const int iMaxValue,		
                           const wxPoint& pos,
                           const wxSize & size,
		                   const long int iStyle)
	: 
	wxControl(pParent, iId, pos, size, wxBORDER_NONE),		
	m_pMembitmap(NULL),
	m_aTagsVal(NULL),
	m_iMin(iMinValue),
	m_iMax(iMaxValue),
	m_iTagsCount(0),
	m_iStyle(iStyle),
	m_iRealVal(0),					
	m_iBigStep(iMaxValue/10),
	m_iSmallStep(1),
	m_iStato(0),
	m_dScaledVal(0),
	m_iPostPointNumbers(0),
	m_iDivisor(1),
	m_iSnapRange(0),
	m_iCaretPosition(0),
	m_iTextOffset(9),
	m_bEditing(false),
	m_strCurrent(""),
	m_strPostfix(""),
	m_cEditColour(*wxRED),
	m_cValueColour(*wxBLACK),
	m_cTagsColour(*wxBLACK),
	m_cLimitColour(*wxBLACK)
{

}

/*!
 * destructor of kwxRegulatorBase
 * destroys tags array and bitmap buffer
 */
kwxRegulatorBase::~kwxRegulatorBase(){
	if(	m_aTagsVal)
		delete[] m_aTagsVal;
	if (m_pMembitmap)
		delete 	m_pMembitmap;
	//std::cout << "bye regulator base"  << std::endl;			// check destruction
}


#if 1
/// strGetKey - debug function for OnKeyDown()
static wxString strGetKey(long keycode){
	wxString key;
	switch ( keycode )
    {
        case WXK_BACK: key = _T("BACK"); break;
        case WXK_TAB: key = _T("TAB"); break;
        case WXK_RETURN: key = _T("RETURN"); break;
        case WXK_ESCAPE: key = _T("ESCAPE"); break;
        case WXK_SPACE: key = _T("SPACE"); break;
        case WXK_DELETE: key = _T("DELETE"); break;
        case WXK_START: key = _T("START"); break;
        case WXK_LBUTTON: key = _T("LBUTTON"); break;
        case WXK_RBUTTON: key = _T("RBUTTON"); break;
        case WXK_CANCEL: key = _T("CANCEL"); break;
        case WXK_MBUTTON: key = _T("MBUTTON"); break;
        case WXK_CLEAR: key = _T("CLEAR"); break;
        case WXK_SHIFT: key = _T("SHIFT"); break;
        case WXK_ALT: key = _T("ALT"); break;
        case WXK_CONTROL: key = _T("CONTROL"); break;
        case WXK_MENU: key = _T("MENU"); break;
        case WXK_PAUSE: key = _T("PAUSE"); break;
        case WXK_CAPITAL: key = _T("CAPITAL"); break;
        case WXK_END: key = _T("END"); break;
        case WXK_HOME: key = _T("HOME"); break;
        case WXK_LEFT: key = _T("LEFT"); break;
        case WXK_UP: key = _T("UP"); break;
        case WXK_RIGHT: key = _T("RIGHT"); break;
        case WXK_DOWN: key = _T("DOWN"); break;
        case WXK_SELECT: key = _T("SELECT"); break;
        case WXK_PRINT: key = _T("PRINT"); break;
        case WXK_EXECUTE: key = _T("EXECUTE"); break;
        case WXK_SNAPSHOT: key = _T("SNAPSHOT"); break;
        case WXK_INSERT: key = _T("INSERT"); break;
        case WXK_HELP: key = _T("HELP"); break;
        case WXK_NUMPAD0: key = _T("NUMPAD0"); break;
        case WXK_NUMPAD1: key = _T("NUMPAD1"); break;
        case WXK_NUMPAD2: key = _T("NUMPAD2"); break;
        case WXK_NUMPAD3: key = _T("NUMPAD3"); break;
        case WXK_NUMPAD4: key = _T("NUMPAD4"); break;
        case WXK_NUMPAD5: key = _T("NUMPAD5"); break;
        case WXK_NUMPAD6: key = _T("NUMPAD6"); break;
        case WXK_NUMPAD7: key = _T("NUMPAD7"); break;
        case WXK_NUMPAD8: key = _T("NUMPAD8"); break;
        case WXK_NUMPAD9: key = _T("NUMPAD9"); break;
        case WXK_MULTIPLY: key = _T("MULTIPLY"); break;
        case WXK_ADD: key = _T("ADD"); break;
        case WXK_SEPARATOR: key = _T("SEPARATOR"); break;
        case WXK_SUBTRACT: key = _T("SUBTRACT"); break;
        case WXK_DECIMAL: key = _T("DECIMAL"); break;
        case WXK_DIVIDE: key = _T("DIVIDE"); break;
        case WXK_F1: key = _T("F1"); break;
        case WXK_F2: key = _T("F2"); break;
        case WXK_F3: key = _T("F3"); break;
        case WXK_F4: key = _T("F4"); break;
        case WXK_F5: key = _T("F5"); break;
        case WXK_F6: key = _T("F6"); break;
        case WXK_F7: key = _T("F7"); break;
        case WXK_F8: key = _T("F8"); break;
        case WXK_F9: key = _T("F9"); break;
        case WXK_F10: key = _T("F10"); break;
        case WXK_F11: key = _T("F11"); break;
        case WXK_F12: key = _T("F12"); break;
        case WXK_F13: key = _T("F13"); break;
        case WXK_F14: key = _T("F14"); break;
        case WXK_F15: key = _T("F15"); break;
        case WXK_F16: key = _T("F16"); break;
        case WXK_F17: key = _T("F17"); break;
        case WXK_F18: key = _T("F18"); break;
        case WXK_F19: key = _T("F19"); break;
        case WXK_F20: key = _T("F20"); break;
        case WXK_F21: key = _T("F21"); break;
        case WXK_F22: key = _T("F22"); break;
        case WXK_F23: key = _T("F23"); break;
        case WXK_F24: key = _T("F24"); break;
        case WXK_NUMLOCK: key = _T("NUMLOCK"); break;
        case WXK_SCROLL: key = _T("SCROLL"); break;
        case WXK_PAGEUP: key = _T("PAGEUP"); break;
        case WXK_PAGEDOWN: key = _T("PAGEDOWN"); break;
        case WXK_NUMPAD_SPACE: key = _T("NUMPAD_SPACE"); break;
        case WXK_NUMPAD_TAB: key = _T("NUMPAD_TAB"); break;
        case WXK_NUMPAD_ENTER: key = _T("NUMPAD_ENTER"); break;
        case WXK_NUMPAD_F1: key = _T("NUMPAD_F1"); break;
        case WXK_NUMPAD_F2: key = _T("NUMPAD_F2"); break;
        case WXK_NUMPAD_F3: key = _T("NUMPAD_F3"); break;
        case WXK_NUMPAD_F4: key = _T("NUMPAD_F4"); break;
        case WXK_NUMPAD_HOME: key = _T("NUMPAD_HOME"); break;
        case WXK_NUMPAD_LEFT: key = _T("NUMPAD_LEFT"); break;
        case WXK_NUMPAD_UP: key = _T("NUMPAD_UP"); break;
        case WXK_NUMPAD_RIGHT: key = _T("NUMPAD_RIGHT"); break;
        case WXK_NUMPAD_DOWN: key = _T("NUMPAD_DOWN"); break;
        case WXK_NUMPAD_PAGEUP: key = _T("NUMPAD_PAGEUP"); break;
        case WXK_NUMPAD_PAGEDOWN: key = _T("NUMPAD_PAGEDOWN"); break;
        case WXK_NUMPAD_END: key = _T("NUMPAD_END"); break;
        case WXK_NUMPAD_BEGIN: key = _T("NUMPAD_BEGIN"); break;
        case WXK_NUMPAD_INSERT: key = _T("NUMPAD_INSERT"); break;
        case WXK_NUMPAD_DELETE: key = _T("NUMPAD_DELETE"); break;
        case WXK_NUMPAD_EQUAL: key = _T("NUMPAD_EQUAL"); break;
        case WXK_NUMPAD_MULTIPLY: key = _T("NUMPAD_MULTIPLY"); break;
        case WXK_NUMPAD_ADD: key = _T("NUMPAD_ADD"); break;
        case WXK_NUMPAD_SEPARATOR: key = _T("NUMPAD_SEPARATOR"); break;
        case WXK_NUMPAD_SUBTRACT: key = _T("NUMPAD_SUBTRACT"); break;
        case WXK_NUMPAD_DECIMAL: key = _T("NUMPAD_DECIMAL"); break;
        case WXK_NUMPAD_DIVIDE: key = _T("NUMPAD_DIVIDE"); break;
    }
    return key;
}
#endif


/*!
 * OnKeyDown - event handler for key down events
 * \param rEvent -> event object
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	11.06.2007
 */
void kwxRegulatorBase::OnKeyDown(wxKeyEvent& rEvent) { 
	int iKeycode = rEvent.GetKeyCode();	
	int iFaktor  = 1;

	if(rEvent.ControlDown())	// boost arrow keys by meta keys (ctrl, shift or/and alt)
		iFaktor =  100;
	if(rEvent.ShiftDown())
		iFaktor *= 10;
	if(rEvent.MetaDown())
		iFaktor *= 10;
               
    switch(iKeycode){
    	case WXK_RIGHT:  
    					if(m_bEditing ){
    						if(m_iCaretPosition < (int)m_strCurrent.Length()-1){
    							++m_iCaretPosition; 					// in edit mode move caret right til end of value string
    							Refresh(); 								// repaint control 
    						}
    						return;
    					}
    	case WXK_UP: 	 												// arrow right/up 
						SetValue(iNormalizeValue(m_iRealVal + m_iSmallStep * iFaktor));    					 
    					break;    					  					 
    	case WXK_LEFT:    
    					if(m_bEditing){
    						if(m_iCaretPosition){						// in edit mode move caret left
	    						--m_iCaretPosition;
	    						Refresh(); 								// repaint control 	
    						}
    						return;
    					}
    	case WXK_DOWN:	  												// arrow left/down 
						SetValue(iNormalizeValue(m_iRealVal - m_iSmallStep * iFaktor));
						break;
    	case WXK_PAGEUP:  												// page up/down
    					SetValue(iNormalizeValue(m_iRealVal + m_iBigStep * iFaktor)); 
    					break;			
    	case WXK_PAGEDOWN:
						SetValue(iNormalizeValue(m_iRealVal - m_iBigStep * iFaktor)); 
						break;		
    	case WXK_HOME:    
    					if(m_bEditing){
    						m_iCaretPosition=0;
    						Refresh(); 									// repaint control 	
    						return;
    					}
    					SetValue(m_iMin); 				   
    					break;											// home/end
    	case WXK_END:	  
    					if(m_bEditing){
    						m_iCaretPosition=(int)m_strCurrent.Length()-1;
    						Refresh(); 									// repaint control 	
    						return;
    					}
    					SetValue(m_iMax); 				  
    					break;    	
    	case WXK_F2:													// switch to edit mode 
    	case WXK_INSERT:
    					if(m_iStyle & STYLE_EDITABLE)
    						m_bEditing=true;
    					m_iCaretPosition=0;
    					break;    	
    	case WXK_RETURN:
    	case WXK_TAB:
    					SetValue(iConvert(m_strCurrent));
    					vSendEvent();		
    	case WXK_ESCAPE:												// 'esc' - exit edit mode 
    					m_bEditing=false;
    					Refresh(); 										// repaint control
    					return; 		
    	case WXK_BACK:													// 'back' key is pressed
    					if(m_strCurrent.Length()>1){

    						m_strCurrent.Remove(m_iCaretPosition,1);
	    					if(m_iCaretPosition>0)
	    						--m_iCaretPosition;
	    					if(m_iCaretPosition==(int)m_strCurrent.Length())
	    						m_iCaretPosition=m_strCurrent.Length()-1;
    					}
    					else
    						m_strCurrent=" ";
    					break;										
    	case WXK_DELETE:												// 'del' key is pressed in edit mode
    					if(m_strCurrent.Length()>1){
	    						m_strCurrent.Remove(m_iCaretPosition,1);
	    					if(m_iCaretPosition<0)
	    						m_iCaretPosition=0;
	    					if(m_iCaretPosition==(int)m_strCurrent.Length())
	    						m_iCaretPosition=m_strCurrent.Length()-1;
    					}
    					else
    						m_strCurrent=" ";
    					break;	
    	default: 		if(!m_bEditing){
	    					  wxLogDebug("unprocessable key pressed : %d", iKeycode);
	    					  return;
    					}
    					if(isdigit(iKeycode)|| iKeycode=='.'|| iKeycode==','){								// if numeric input process..
    						//std::cout << "max caret " << iMaxCaretPos << " m_iMax " << m_iMax << std::endl;
    						m_strCurrent[m_iCaretPosition++] = iKeycode;	// insert key, move caret right
    																		// if caret at end
    						if( m_iCaretPosition >= (int)m_strCurrent.Length() ){
    								int iMaxCaretPos = (int)log10((double)m_iMax);	// calculate max caret length
    								if(m_iCaretPosition <= iMaxCaretPos)	// if caret not at end..
    									m_strCurrent.Append(" ");			// ..append blank to end of edit str
    								else
    									m_iCaretPosition = m_strCurrent.Length() - 1;// set caret to end position
    						}
    						Refresh();
    					}
    					else {
    						wxLogDebug("key %d %s pressed", iKeycode, strGetKey( iKeycode ).c_str());
    					}
    					return;
    }
    Refresh(); 					// repaint control    	
    vSendEvent();				// process event
}


/*!
 * DrawCurrent - draw controls current numeric value at implementation specific position
 * \param rDc -> graphic device context ref of subclass
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	19.06.2007
 */
void kwxRegulatorBase::DrawCurrent(wxDC &rDc)
{
	int w, h;
	int tw,th;
	int ix,iy;
	int iw,ic;

	const wxBrush& cOldBrush = rDc.GetBackground();	// remember brush
	int iOldMode = rDc.GetBackgroundMode();			// remember mode
	GetClientSize(&w, &h);
													//valore attuale
	if(!m_bEditing){								// create text string only in non edit mode
		if(m_iDivisor<=1){							// no fixed - point presentation
			m_strCurrent.Printf("%d", m_iRealVal );
		}	
		else{										// fixed point presentation
			wxString format ="%d.%0_d";				// format string
			format[5] = m_iPostPointNumbers+'0'; 	// replace '_' by post point number count
			m_strCurrent.Printf(
						format.c_str(), 			// create final string
					 	m_iRealVal / m_iDivisor, 	// pre point part
					 	m_iRealVal % m_iDivisor 	// post point fraction
					 	);
		}
	}
	rDc.SetFont(m_Font);								// \todo adjust underline & text position according to font size
	rDc.GetTextExtent(m_strCurrent+m_strPostfix, &tw, &th);// get text dimensions
	ix = w / 2 - tw / 2 ;
	if(m_iStyle & STYLE_TEXT_CENTER)
		iy = h / 2 - th / 2;
	if(iType()==TYPE_ROTARY)
		iy = h - th - m_iTextOffset;	
	else
		iy = h / 2 - th / 2;
	//printf(" tw = %i , iw = %i\n",tw,iw);
	if(m_bEditing){									// in edit mode edit string m_strCurrent is displayed 
		rDc.SetTextForeground(m_cEditColour );		// set text foreground to edit colour
		const wxPen & cOldPen = rDc.GetPen();		// save current pen
		
		if( STYLE_RECT_ON_EDIT & m_iStyle){			// draw sourrounding rectangle 
			rDc.SetBackground(wxBrush(wxColour(127,127,127)));// edit rectangle back ground	
			rDc.SetBackgroundMode(wxSOLID);
			rDc.SetPen(wxPen(*wxWHITE));				// set pen for rectangle border			
			rDc.DrawRectangle(wxRect(ix-1,iy-1,tw+2,th+2));			
		}											// draw text
					
		rDc.DrawText( m_strCurrent+m_strPostfix, ix ,iy );
		rDc.SetPen(wxPen(m_cEditColour));			// set pen also to edit colour
		
		rDc.GetTextExtent(m_strCurrent.Left(m_iCaretPosition), &iw, NULL);	// get text width before caret 
		rDc.GetTextExtent(m_strCurrent.Left(m_iCaretPosition+1), &ic, NULL);	// get width incl. caret
													// draw edit underline at caret position \todo adjust underline position according to font size												
		rDc.DrawLine( iw + ix , iy+th-1, ic + ix  , iy+th-1 );
		rDc.SetPen(cOldPen);
	}
	else{											// draw current value in non edit mode
		rDc.SetTextForeground(m_cValueColour);
		rDc.DrawText(m_strCurrent+m_strPostfix, ix ,iy );
	}		
	m_cCurrentRect = wxRect(ix,iy,tw,th);  			// save text rectangle
	rDc.SetBackgroundMode(iOldMode);					// restore mode
	rDc.SetBackground(cOldBrush);					// restore brush
}


/*!
 * vSendEvent - send event according to current implementation (subclass) \n
 * original comment : 'evento cambio valore'
 * \author  Marco Cavallini <m.cavallini AT koansoftware.com> \n
 * modified by Andreas Leidig <leidig@fs.wettzell.de>
 * \date	12.06.2007
 */
void kwxRegulatorBase::vSendEvent(){
	wxEventType xEvtType;
	switch(iType()){
		case TYPE_ROTARY:
			xEvtType = kwxEVT_ANGREG_CHANGE;
			break;
		case TYPE_SLIDER:
			xEvtType = kwxEVT_LINEARREG_CHANGE;
			break;
		default:
			return;
	}
	wxCommandEvent event(xEvtType, GetId());	
	event.SetEventObject(this);
	event.SetInt(m_iRealVal);
	ProcessEvent(event) ;
}


/*!
 * vSetRange - set valid range of control
 * \param iMin -> minimal value
 * \param iMax -> maximal value
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	12.06.2007
 */
void kwxRegulatorBase::vSetRange(int iMin, int iMax) { 
	m_iMin = iMin ; 
	m_iMax = iMax ; 
}


/*!
 * iGetValue - get current value of control
 * \returns current value
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	12.06.2007
 */
int	kwxRegulatorBase::iGetValue() { 
	return m_iRealVal ; 
}

/*!
 * dGetValue - get current floating point value of control
 * \returns current value
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	12.06.2007
 */
double	kwxRegulatorBase::dGetValue() { 
	double dRet = m_iRealVal;
	return dRet/m_iDivisor ; 
}

/*!
 * iGetRangeMax range upper bound
 * \returns upper bound of display range
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	12.06.2007
 */
int kwxRegulatorBase::iGetRangeMax(){
	return m_iMax; 
}


/*!
 * iGetRangeMin range lower bound
 * \returns lower bound of display range
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	12.06.2007
 */
int kwxRegulatorBase::iGetRangeMin() {
	return m_iMin; 
}


/*!
 * vSetTagsCount create tags with equidistant values 
 * \param iCountTags -> number of tags with equidistant values to show across whole range of control (exclusive first tag)
 * \note adjusts step size according to controls value range 
 * \warning call this before calling vSetTag(..) !
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	12.06.2007
 */
void kwxRegulatorBase::vSetTagsCount(const int iCountTags){
	m_iTagsCount=0;						// reset from any previous calls
	if(m_aTagsVal){						// free mem
		delete[] m_aTagsVal;
		m_aTagsVal=NULL;
	}
	if(iCountTags<1)					// no tags
		return;
	m_aTagsVal = new int[iCountTags];	// allocate	
	*m_aTagsVal= iGetRangeMin();
	for(int iJ=1; iJ<iCountTags; iJ++)  // preset with equidistant values over controls range
		m_aTagsVal[iJ] = ( iGetRangeMax() * iJ / iCountTags );		
	m_iTagsCount = iCountTags;			
	//vSetBigStep( (iGetRangeMax()-iGetRangeMin()) / iCountTags );	
}


/*!
 * vSetTag set custom value
 * \param iIndex -> tag index
 * \param iValue -> value
 * \warning tags must be created by vSetTagsCount(..) before
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	26.04.2007
 */
void kwxRegulatorBase::vSetTag(const int iIndex,int iValue){
	if(iIndex>=m_iTagsCount)		// check bounds..
		return;
	m_aTagsVal[iIndex] = iValue;	// ..ok,set tag value
}


/*!
 * vSetStyle set window style 
 * \param iStyle -> orable window style  \see eStyle
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	12.06.2007
 */
void kwxRegulatorBase::vSetStyle(const int iStyle){
	m_iStyle = iStyle ;
}


/*!
 * vSetSmallStep adjust step size for arrow keys
 * \param iStep -> step must be <  (m_iMax - m_iMin)
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	12.06.2007
 */
void kwxRegulatorBase::vSetSmallStep(int iStep){
	m_iSmallStep = iStep;
}


/*!
 * vSetBigStep adjust step size for mouse wheel and page up / page down keys 
 * \param iStep -> step must be <  (m_iMax - m_iMin)
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	12.06.2007
 */
void kwxRegulatorBase::vSetBigStep(int iStep){
	m_iBigStep = iStep;
}


/*!
 * vSetPostfix set unit string 
 * \param rstrPostfix -> ref to unit string
 * \see DrawCurrent()
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	12.06.2007
 */
void kwxRegulatorBase::vSetPostfix(const wxString & rstrPostfix){
	m_strPostfix = rstrPostfix;
}


/*!
 * vSetPointPosition - specify number of post point digits
 * \param ucPointPosition -> concerning current value : position (from right) where decimal point will be shown 
 * \see DrawCurrent() in subclass implementation
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	12.06.2007
 */
void kwxRegulatorBase::vSetPointPosition(const unsigned char ucPointPosition) {
	m_iPostPointNumbers = ucPointPosition;
	m_iDivisor = (int)pow(10,ucPointPosition);
}


/*!
 * iNormalizeValue - bound checker for input values
 * \param iValue -> input value to check
 * \returns - iValue if is in valid range of control
 * 			- else min value or max value.
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	10.06.2007
 */
int kwxRegulatorBase::iNormalizeValue(int iValue){
	if(iValue>m_iMax)
		return m_iMax;
	if(iValue<m_iMin)
		return m_iMin;
	return iValue;
}


/*!
 * vSetSnap2Tags set snap range automatically 
 * \param bSnap -> true : adjust snap range according to control range; false : no snap
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	23.04.2007
 * \note vSetTagsCount(..) has to be called previously
 */
void kwxRegulatorBase::vSetSnap2Tags(bool bSnap){
	if(m_iTagsCount<1)					// no division by zero
		return;
	if(bSnap){
		m_iSnapRange =  (iGetRangeMax()-iGetRangeMin() ) / (m_iTagsCount) /2  ;
		return;
	}
	m_iSnapRange = 0;
}


/*!
 * static inline iGetDiff convenience for bSnap(..)
 * \param iVal0 -> param 0 to compare 
 * \param iVal1 -> param 1 to compare
 * \returns difference between params avoiding sign problems
 */
static inline int iGetDiff(int iVal0,int iVal1){
	int iD0 = abs(iVal1-iVal0);
	int iD1 = abs(iVal0-iVal1);
	if(iD0 < iD1)
		return iD0;
	return iD1;
}


/*!
 * vSnap checks and modifies input value 
 * \param 	riValue <-> pulls input value to tag if is in between 'snap' range
 * \author  Andreas Leidig <leidig@fs.wettzell.de> \n
 * \date	23.04.2007
 */
bool kwxRegulatorBase::bSnap(int & riValue){
	if(m_iSnapRange < 1)						 // no snap
		return false;
	for(int iI=0; iI<m_iTagsCount; iI++ ){		// iterate thru tags array
		if(iGetDiff(m_aTagsVal[iI], riValue) <= m_iSnapRange){// if riValue near tag..
			riValue = m_aTagsVal[iI];			 // ..snap !
			//printf ("snap at %i\n",iI);
			return true;
		}
	}
	if(iGetDiff(m_iMin, riValue) < m_iSnapRange){// check min
			riValue = m_iMin;					 // snap !
			//printf ("snap at %i\n",iI);
			return true;
	}
	if(iGetDiff(m_iMax, riValue) < m_iSnapRange){// check max
			riValue = m_iMax;					 // snap !
			//printf ("snap at %i\n",iI);
			return true;
	}
	return false;								 // no snap
}


/*!
 * vSetTxtFont - set controls font
 * \param rFont -> font ref
 * \author  Andreas Leidig <leidig@fs.wettzell.de> \n
 * \date	14.06.2007
 */
void kwxRegulatorBase::vSetTxtFont(wxFont & rFont){
	m_Font = rFont ;
}


/*!
 * vSetTxtFontSize - set controls font point size
 * \param iPoints -> point size 
 * \author  Andreas Leidig <leidig@fs.wettzell.de> \n
 * \date	18.06.2007
 * \note 'point size' is different from 'pixels'
 */
void kwxRegulatorBase::vSetTxtFontSize(int iPoints){
	m_Font.SetPointSize(iPoints);
}


/*!
 * iConvert - convert string from edit buffer to controls integer value
 * \param rstrInput -> ref to edit string
 * \returns converted integer value
 * \author  Andreas Leidig <leidig@fs.wettzell.de> \n
 * \date	14.06.2007
 */
int kwxRegulatorBase::iConvert(const wxString & rstrInput){	
	wxString buf(rstrInput);
	if(buf.Trim() =="")			// remove spaces
		return 0;
	int iPointPos = (buf.Find("."));
	if(iPointPos==wxNOT_FOUND){
		long lNewVal;
		if(buf.ToLong(&lNewVal)){	// if integer
			return (int)lNewVal;// return value
		}
	}
	else{
		buf[iPointPos] = ',';
	}
	double dNewVal;
	//std::cout << " try double " << std::endl;
	if(buf.ToDouble(&dNewVal)){ // if floating point
		//std::cout << " double " << dNewVal << std::endl;
		dNewVal *= m_iDivisor;	// remove point
		return (int)dNewVal;	// return value 
	}							// no conversion performed..
	return m_iRealVal;			// ..return old value
}


/*!
 * vSetEditColour - set text colour used in edit mode
 * \param rcColour -> wxColour ref
 * \author  Andreas Leidig <leidig@fs.wettzell.de> \n
 * \date	14.06.2007
 */
void kwxRegulatorBase::vSetEditColour(const wxColour & rcColour){
	m_cEditColour = rcColour;
}

/*!
 * vSetValueColour - set text colour used to display controls current numeric value
 * \param rcColour -> wxColour ref
 * \author  Andreas Leidig <leidig@fs.wettzell.de> \n
 * \date	19.06.2007
 */
void kwxRegulatorBase::vSetValueColour(const wxColour & rcColour){
	m_cValueColour = rcColour;
}


/*!
 * vSetTagsColour - set colour of range tags
 * \param rcColour -> wxColour ref
 * \author  Andreas Leidig <leidig@fs.wettzell.de> \n
 * \date	19.06.2007
 */
void kwxRegulatorBase::vSetTagsColour(const wxColour & rcColour){
	m_cTagsColour = rcColour;
}


/*!
 * vSetLimitsColour - set colour of range limit tags or text (depends on implementation)
 * \param rcColour -> wxColour ref
 * \author  Andreas Leidig <leidig@fs.wettzell.de> \n
 * \date	19.06.2007
 */
void kwxRegulatorBase::vSetLimitsColour(const wxColour & rcColour){
	m_cLimitColour = rcColour;
}

