/*! \file kwic_reg_base.h
 *  \brief base for kwic regulator controls  \n 
 * 			kwxRegulatorBase	: extended functionality for kwic regulator-like (user interactive) controls
 * 
 *  \author Andreas Leidig <leidig@fs.wettzell.de>
 *  \date   12.06.2007
 *  \note this application was built using Eclipse CDT, text format on a text editor may lack
 */
#ifndef _KWIC_REGULATOR_
#define _KWIC_REGULATOR_

#ifndef WX_PRECOMP
#  include "wx/wx.h"
#endif

#include "kwic/kwicdef.h"

// event definitions for kwicRotaryButton
const wxEventType kwxEVT_ANGREG_FIRST  = wxEVT_FIRST + 5402;

const wxEventType kwxEVT_ANGREG_CHANGE = kwxEVT_ANGREG_FIRST + 1;

#define EVT_KWIC_ANG_REG(id, fn)	\
		DECLARE_EVENT_TABLE_ENTRY(kwxEVT_ANGREG_CHANGE, id, -1, \
		(wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&fn, \
		(wxObject*)NULL ),


// event definitions for kwicSlider
const wxEventType kwxEVT_LINEARREG_FIRST  = wxEVT_FIRST + 5401;

const wxEventType kwxEVT_LINEARREG_CHANGE = kwxEVT_LINEARREG_FIRST + 1;

#ifndef __SLIDER_EVENT__
#define __SLIDER_EVENT__
	#define EVT_KWIC_LIN_REG(id, fn)	\
			DECLARE_EVENT_TABLE_ENTRY(kwxEVT_LINEARREG_CHANGE, id, -1, \
			(wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&fn, \
			(wxObject*)NULL ),
#endif  //__SLIDER_EVENT__

/*!
 * \class kwxRegulatorBase
 * \brief kwxRegulatorBase - abstract base for koan wxIndustrialControls
 * \note extends functionalty of standard koan wxIndustrialControls regulator by following items \n
 * 	<ul style="list-style-type:square">
 *  <li> fixed-point data presentation </li>
 *  <li> presentation of postfix string </li>
 *  <li> simplified tag handling </li>
 *  <li> adjustable 'snapping' mode </li>
 * 	<li> key induced regulator modification </li>  
 * 	<li> edit mode </li> 
 * \author  Andreas Leidig <leidig@fs.wettzell.de>
 * \date	13.06.2007
 */
class WXDLLIMPEXP_KWIC kwxRegulatorBase: public wxControl{

public:

	kwxRegulatorBase(){};
	kwxRegulatorBase(wxWindow* pParent,
                    const wxWindowID iId,
                    const int iMinValue,
                    const int iMaxValue,
                    const wxPoint& rPos,
                    const wxSize & rSize,
                    const long int iStyle);
	virtual ~kwxRegulatorBase();
		
	int iGetRangeMin();
	int iGetRangeMax();
	
	void vSetTagsCount(const int iCountTags);	
	void vSetTag(const int iIndex,int iValue);
	
	void vSetStyle(const int iStyle);
	
	void vSetSmallStep(int iStep);
	void vSetBigStep(int iStep);
	
	void vSetPostfix(const wxString & rstrPostfix);
	void vSetPointPosition(const unsigned char ucPointPosition);
	
	void vSetRange(int iMin, int iMax)  ;
	
	void vSetSnap2Tags(bool bSnap=true);
		
	int	 iGetValue()  ;
	
	double	dGetValue() ;
	
	void vSetTxtFont(wxFont &rFont);
	
	void vSetTxtFontSize(int iPoints);
	
	void vSetEditColour( const wxColour & rcColour);
	
	void vSetValueColour(const wxColour & rcColour);
	
	void vSetTagsColour(const wxColour & rcColour);
	
	void vSetLimitsColour(const wxColour & rcColour);
		
	
	// overrideables
	virtual void SetValue(int val)=0;
	
	virtual int iType()=0;
	
	/// maskable constants for window style definition \see vSetStyle()
	enum eStyle{
				/// draw curent value
				STYLE_DRAW_CURRENT		= 0x01,		
				/// show limit numbers
				STYLE_DRAW_TAG_NUMBERS	= 0x02,
				/// show limit numbers
				STYLE_DRAW_LIMITS		= 0x04,
				/// show needle
				STYLE_METER				= 0x08,
				/// show arrow
				STYLE_ARROW				= 0x10,
				/// vertical
				STYLE_VERTICAL			= 0x20,
				/// snap on double click
				STYLE_SNAP_DOUBLE_CLK	= 0x40,
				/// value is editable 
				STYLE_EDITABLE			= 0x80,
				/// show rectangle on editing 
				STYLE_RECT_ON_EDIT		= 0x0100,
				/// show rectangle on editing 
				STYLE_TEXT_CENTER		= 0x0200
				};
				
	/// runtime type identification
	static const int TYPE_SLIDER = 0x01;
	static const int TYPE_ROTARY = 0x02;
	
				
protected:		

	void 	OnKeyDown(wxKeyEvent& event);
	void 	DrawCurrent(wxDC &dc);
	void    vSendEvent();
	int 	iNormalizeValue(int iValue);
	
	bool 	bSnap(int & riValue);
	
	int 	iConvert(const wxString & rstrInput);
	
	void 	vCreateCaret();
	
	/// graphical device context buffer
	wxBitmap *m_pMembitmap ;
	
	/// dynamic array for user definable tags 
	int * m_aTagsVal;
	
	/// controls lower / upper bounds
	int m_iMin ;
	int m_iMax ;
	
	/// number of tags to display
	int m_iTagsCount ;
	
	/// window options
	int m_iStyle;
	
	/// current value of control
	int m_iRealVal ;
	
	/// step size for mouse wheel and page up/down cmds						
	int m_iBigStep;
	/// step size for arrow keys
	int m_iSmallStep;
	
	/// internal mouse state
	int m_iStato ;
	
	/// scaled control value
	double	m_dScaledVal;
	
	/// fix point presentation of current value
	int m_iPostPointNumbers;
	int m_iDivisor;
	
	/// manage 'magnetic' tags	
	int m_iSnapRange;	
	
	/// holds caret position
	int m_iCaretPosition;
	
	/// distance of value string from controls lower border
	int m_iTextOffset;
	
	/// manage edit mode
	bool m_bEditing;
	
	/// string to be displayed as numeric value
	wxString m_strCurrent;
	
	/// string to be displayed as postfix value
	wxString m_strPostfix;
	
	/// rectangle surrounding current value
	wxRect	 m_cCurrentRect;
	
	/// position of last mouse click
	wxPoint  m_mousePosition ;
	
	/// font for numeric output
	wxFont 	m_Font ;
	
	/// edit text color
	wxColour m_cEditColour ;
	
	/// numeric value color
	wxColour m_cValueColour ;
	
	/// colour of scale tags
	wxColour m_cTagsColour;
	
	/// colour of limit
	wxColour m_cLimitColour;
	
	/// constants for internal use (concerning mouse event handling)
	enum eMouseState{							
		/// mouse state
		LEFT 		 ,
		ENTERED 	 ,
		DRAGED 		 		
	};
	
};


#endif /*_KWIC_REGULATOR_*/
