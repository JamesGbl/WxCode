/////////////////////////////////////////////////////////////////////////////
// Name:        textspan.h
// Purpose:     wxTextStyle, wxTextSpan, wxTextSpanArray
// Author:      Francesco Montorsi
// Created:     2005/8/15
// RCS-ID:      $Id: textspan.h,v 1.4 2005-09-18 10:05:29 frm Exp $
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_TEXTSPAN_H_
#define _WX_TEXTSPAN_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "textspan.h"
#endif

// wxWidgets headers
#include "wx/textctrl.h"		// holds the definition of wxTextAttr
#include "wx/resizec.h"

// in case we need XHTML support for import/export in wxTextboxes...
#if wxUSE_TEXTBOX_XHTML_SUPPORT
	#include "wx/xml/xml.h"
#endif

// defined later
class WXDLLIMPEXP_RESIZEC wxTextStyle;
class WXDLLIMPEXP_RESIZEC wxTextSpan;
class WXDLLIMPEXP_RESIZEC wxTextBox;
class WXDLLIMPEXP_RESIZEC wxTextBoxInputStatus;

// these *must* be pointers because their initialization must happen
// *after* wxWidgets initialization
extern WXDLLIMPEXP_RESIZEC wxTextSpan *wxEmptyTextSpan;
extern WXDLLIMPEXP_RESIZEC wxTextStyle *wxNullTextStyle;

// some data structures which will be used...
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxTextStyle, wxTextStyleArray, WXDLLIMPEXP_RESIZEC);
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxTextSpan, wxTextSpanArrayBase, WXDLLIMPEXP_RESIZEC);


// Defines
// --------

//! The wxTextSpanArray allocation block used when growing the array.
#define wxTSA_ALLOCATION_BLOCK		8

//! The left & right border of each wxTextSpan.
#define wxTS_LEFTRIGHT_BORDER		0

//! The delimiter of a wxTextSpan.
//! Each wxTextSpan can contain a delimiter only as the last character of the span.
#define wxTS_DELIMITER				wxT('\n')

#ifdef __WXDEBUG__

//! When defined, the spans' rectangles will be shown.
#define __WXFULLDEBUG__
#endif




// Enumerations
// -------------

//! A return flag of wxTextSpan::TruncateToSize.
enum wxTextSpanTruncFlag {

	//! The current size of the span is too small: even truncating it to a single
	//! character would require a greater size...
	wxTSTF_TOOSMALL = -1,

	//! The current size is bigger than the minimal required size thus there is no
	//! need to truncate the span in any point.
	wxTSTF_TOOBIG = -2,

	//! The span could be truncated.
	wxTSTF_TRUNCATED = -3
};

enum wxTextStyleVertAlignment {

	wxTSVA_TOP,
	wxTSVA_MIDDLE,
	wxTSVA_BOTTOM
};

//! The truncation mode of a wxTextStyle.
enum wxTextSpanTruncMode {

	//! The spans are truncated so that also words are broken: i.e. the truncation
	//! can happen in any place of the text.
	wxTSTM_CHARWRAP,

	//! The spans are truncated so that the word integrity is preserved.
	wxTSTM_WORDWRAP
};

//! Values which can be used as flag argument when calling
//! wxElement::gui_MoveCaret() function.
enum wxMoveCaretFlag {
	wxMCF_RIGHT,		//!< Cursor must be moved right.
	wxMCF_LEFT,			//!< Cursor must be moved left.
	wxMCF_UP,			//!< Cursor must be moved up.
	wxMCF_DOWN			//!< Cursor must be moved down.
};

//! Values returned by gui_MoveCaret() and gui_MoveCaretUsingPoint() functions.
enum wxMoveCaretRes {
	wxMCR_OKAY,					//!< Everything was okay.
	wxMCR_SETFOCUS_PREVIOUS,	//!< Cursor must be moved on previous element.
	wxMCR_SETFOCUS_NEXT,		//!< Cursor must be moved on next element.
	wxMCR_SETFOCUS_BELOW,		//!< Cursor must be moved on element lying below.
	wxMCR_SETFOCUS_ABOVE,		//!< Cursor must be moved on element lying above.
	wxMCR_CANNOT_SETFOCUS		//!< There is no elements under the given point
								//! (eventually returned by gui_MoveCaretUsingPoint).
};

//! Values which can be returned by gui_Input() functions.
enum wxInputRes {
	wxIR_OKAY,				//!< Everything was okay.
	wxIR_DELETE_THIS,		//!< This element must be deleted.
	wxIR_DELETE_PREVIOUS,	//!< The previous element must be deleted.
	wxIR_DELETE_NEXT,		//!< The next element must be deleted.
	wxIR_INSERT_NEWSPAN_AFTER_THIS,
	wxIR_RELAYOUT
};


//! The style of a wxTextSpan.
class WXDLLIMPEXP_RESIZEC wxTextStyle
{
public:

	//! Contains the following info on this style:
	//! - the horizontal alignment
	//! - the font
	//! - the text background and foreground colours
	wxTextAttr m_style;

	//! The vertical alignment.
	wxTextStyleVertAlignment m_alignVert;

	//! The truncation mode for the spans using this style.
	wxTextSpanTruncMode m_truncMode;

	//! The name for this style.
	wxString m_strName;

	//! The background mode for this style.
	int m_nBkMode;

public:		// some cached data

	int m_nMinCharWidth;
	int m_nAveCharWidth;
	int m_nAveCharHeight;

public:
	wxTextStyle(const wxTextAttr &attr, const wxString &name, 
				wxTextSpanTruncMode tm = wxTSTM_WORDWRAP,
				wxTextStyleVertAlignment va = wxTSVA_MIDDLE,
				int bkmode = wxTRANSPARENT);		
	virtual ~wxTextStyle() {}

public:		// miscellaneous

	void Select(wxDC &) const;
	void CacheData();

	bool operator==(const wxTextStyle &tocomp) const
		{ return (m_strName == tocomp.m_strName); }
	bool operator!=(const wxTextStyle &tocomp) const
		{ return !(*this == tocomp); }

	wxTextStyle *Clone() const
		{ return new wxTextStyle(*this); }

	void SetNewNameFromCurrentName();	

public:		// getters

	int GetAveCharWidth() const
		{ return m_nAveCharWidth; }
	int GetAveCharHeight() const
		{ return m_nAveCharHeight; }
	int GetMinCharWidth() const
		{ return m_nMinCharWidth; }
	int GetBackgroundMode() const
		{ return m_nBkMode; }
	wxString GetName() const
		{ return m_strName; }
	wxTextSpanTruncMode GetTruncMode() const
		{ return m_truncMode; }
	wxFont GetFont() const
		{ return m_style.GetFont(); }
	wxColour GetBackgroundColour() const
		{ return m_style.GetBackgroundColour(); }
	wxColour GetForegroundColour() const
		{ return m_style.GetTextColour(); }

	wxTextAttrAlignment GetHorizAlignment() const {
		wxASSERT(m_style.GetAlignment() != wxTEXT_ALIGNMENT_DEFAULT &&
				m_style.GetAlignment() != wxTEXT_ALIGNMENT_JUSTIFIED);		
		return m_style.GetAlignment();
	}

	wxTextStyleVertAlignment GetVertAlignment() const 
		{ return m_alignVert; }

public:		// setters

	void SetHorizAlignment(wxTextAttrAlignment);
	void SetVertAlignment(wxTextStyleVertAlignment);

public:		// export / import

	wxString ExportRTF() const;
	wxString ExportSelectionToRTF() const;

#if wxUSE_TEXTBOX_XHTML_SUPPORT
	wxXmlNode *ExportXHTML() const;
	wxXmlNode *ExportXHTMLDiffFrom(const wxTextStyle *) const;
#endif
};



//! A container of text inside a wxTextBox.
//! The rules for wxTextSpan are:
//! 1) span can contain the delimiter character wxTS_DELIMITER only
//!    as the last character of the span
//! 2) a span can be "empty"; i.e. containing only the wxTS_DELIMITER.
class WXDLLIMPEXP_RESIZEC wxTextSpan 
{
protected:      // miscellaneous utilities

	//! Recalculates the minimal size of this span and returns it without
	//! storing it. See #RecalcSize() for a function which also stores this
	//! size in the #m_size variable.
	wxSize DoRecalcSize() const;

	//! A #TruncateToSize helper.
	void UpdateCaretPosForTruncateOperation(int tokeep, int *span, int *pos);
	
	//! A #MoveCaret helper.
    void OnMoveCaret(wxMoveCaretFlag flag, long modifiers, bool &extleft, bool &extright);

public:		// core data

	//! The text of this span.
	wxString m_text;

	//! The style used by this span.
	//! This variable should point to one of wxTextBox's registered styles.
	wxTextStyle *m_style;

	//! The current input status.
	wxTextBoxInputStatus *m_input;

public:		// cached data

	//! The size of this span.
	//! This does not represent the <b>minimal size of the span</b> but the
	//! current space which can be used by this span.
	//! To know if the current value matches the minimal size of the span you
	//! can use the #IsTrueSize() function.
	wxSize m_size;

	//! The current position of this span in textbox's coords.
	//! The textbox referred to is the owner of this span.
	wxPoint m_pos;

public:
	wxTextSpan(const wxString &txt = wxT("invalid"), 
				wxTextStyle *attr = wxNullTextStyle,
				wxTextBoxInputStatus *input = NULL,
				const wxPoint &pos = wxDefaultPosition, 
				const wxSize &sz = wxDefaultSize) 
		{ Set(txt, attr, input, pos, sz); }
	wxTextSpan(const wxTextSpan &tocopy)
		{ Copy(tocopy); }

	virtual ~wxTextSpan() {}


public:		// statics

	static void Init();
	static void Cleanup();

public:		// getters
	
	int GetAveCharWidth() const
		{ wxASSERT(m_style); return m_style->GetAveCharWidth(); }
	int GetAveCharHeight() const
		{ wxASSERT(m_style); return m_style->GetAveCharHeight(); }
	int GetMinCharWidth() const
		{ wxASSERT(m_style); return m_style->GetMinCharWidth(); }

	wxTextAttrAlignment GetHorizAlignment() const
		{ wxASSERT(m_style); return m_style->GetHorizAlignment(); }
	wxTextStyleVertAlignment GetVertAlignment() const
		{ wxASSERT(m_style); return m_style->GetVertAlignment(); }
	wxTextStyle *GetStyle() const
		{ return m_style; }

	//! Returns the text of this span without the final delimiter (if present).
	wxString GetTextWithoutDelim(bool trim = FALSE) const;

	bool IsLastOfTheLine() const;
	bool IsTrueSize() const;

	wxSize GetTrueSize() const
		{ return DoRecalcSize(); }
	wxSize GetSize() const
		{ return m_size; }	

	bool IsEmpty() const
		{ return m_text.IsEmpty(); }
	bool IsValid() const
		{ return m_text != wxT("invalid"); }
	bool IsDelimiter() const
		{ return m_text == wxTS_DELIMITER; }
    bool EndsWithDelimiter() const
        { return m_text.Last() == wxTS_DELIMITER; }

public:		// setters

	void SetStyle(wxTextStyle *ts)
		{ wxASSERT(ts); m_style = ts; }

public:		// layout

	void Paint(wxDC &);

	// truncation
	wxTextSpanTruncFlag TruncateToSize(wxTextSpan &);
	wxTextSpanTruncFlag CanBeSplit() const;	
	int GetTruncateSize() const;

	void MergeWith(wxTextSpan &);

	wxRect GetRect() const
		{ return wxRect(m_pos, m_size); }
	void RecalcSize()
		{ m_size = DoRecalcSize(); }

	//! Slow !
	wxArrayInt GetPartialTextExtent() const;

	int GetTextOffsetX() const;
	int GetTextOffsetY() const
		{ return 0; }

	wxPoint ConvertToTextspanCoord(const wxPoint &pt) const
		{ wxPoint p(pt); p -= m_pos; return p; }

public:		// input & caret

	wxInputRes Input(int keycode, wxTextSpan **pnew);

	//! Returns the caret offset position in TEXTBOX coords.
	wxPoint GetCaretOffset();
	void CheckCaretPos();

	virtual bool IsToReject(wxChar) const
		{ return FALSE; }

	wxMoveCaretRes SetCaretPos(int pos, long modifiers);
	void SetCaretPos(int pos)
	   { SetCaretPos(pos, 0); }
	
	wxMoveCaretRes MoveCaret(wxMoveCaretFlag flag, long modifiers);
	wxMoveCaretRes MoveCaretUsingPoint(const wxPoint &p);

	// the point must be in textbox coord.
	int GetCaretPosUsingPoint(const wxPoint &p) const;

	void OnSelChanged();

public:		// miscellaneous

	void Copy(const wxTextSpan &tocopy)
		{ Set(tocopy.m_text, tocopy.m_style, tocopy.m_input, tocopy.m_pos, tocopy.m_size); }

	void Set(const wxString &txt = wxEmptyString, 
			wxTextStyle *attr = wxNullTextStyle, 
			wxTextBoxInputStatus *input = NULL,
			const wxPoint &pos = wxDefaultPosition, 
			const wxSize &sz = wxDefaultSize);

public:		// export / import

	wxString ExportRTF() const;
	wxString ExportSelectionToRTF() const;

#if wxUSE_TEXTBOX_XHTML_SUPPORT
	wxXmlNode *ExportXHTML() const;
	wxXmlNode *ExportSelectionToXHTML() const;
#endif
};


//! A fast array & allocator of wxTextSpan.
class WXDLLIMPEXP_RESIZEC wxTextSpanArray 
{
protected:
	
	wxTextSpan **m_arr;
	int m_nSize;
	int m_nUsed;

public:

#ifdef __WXDEBUG__
	static int s_nMaxUsed;
#endif

public:
	wxTextSpanArray() 
		{ m_arr=NULL; m_nUsed=m_nSize=0; Grow(); }
	virtual ~wxTextSpanArray() { Clear(); }

public:		// wxArray-like functions

	void Empty();
	void Clear();
	void Grow(int increment = wxTSA_ALLOCATION_BLOCK);
	void Alloc(int n);

	//! Removes the pos-th span and shifts left the array.
	void RemoveAt(int pos);

	//! Inserts a new span in the array at the given position, shifting the array.
	void Insert(const wxTextSpan &tocopy, int pos);

	//! Returns a new uninitialized span which is placed at the end of the array.
	wxTextSpan *GetNew();

	//! Returns the n-th span of the array.
	const wxTextSpan *Item(int n) const;
	wxTextSpan *Item(int n);

	//! Returns the last used span of the array.
	wxTextSpan *Last()
		{ return Item(GetCount()-1); }

	//! Returns the number of currently used spans in the array.
	int GetCount() const
		{ return m_nUsed; }

	//! See #Item.
	wxTextSpan *operator[](int item)
		{ return Item(item); }
	const wxTextSpan *operator[](int item) const
		{ return Item(item); }

	//! Finds the given span in the array.
	int Index(const wxTextSpan *tofind) const;

public:		// wxTextSpan-input functions

	int GetCaretEndPos() const;
	int GetCaretOffset(int span) const;
	int GetSpanFromCaretPos(int cp) const;

public:		// wxTextSpan-miscellaneous functions

	//! Concatenates the text strings of all the spans and then returns it.
	//! With this function you can get only the text, not the text style.
	wxString GetAllText() const;

	//! Returns the lenght of the text contained in all spans.
	//! This value is the same of GetAllText().GetTextLen() but
	//! is slightly faster.
	int GetTextLen() const;
	int GetCharIndexOffset(int span) const;
	int GetSpanFromCharIndex(int ci) const;

	void RemoveStyles();
	void Check(wxTextBox *);
	void Paint(wxDC &);
	void Defrag();
	void RecalcSize();

public:		// array export / import

	wxString ExportRTF() const;
	wxString ExportSelectionToRTF() const;

#if wxUSE_TEXTBOX_XHTML_SUPPORT

	wxXmlDocument ExportXHTMLDoc() const;

	wxXmlNode *ExportXHTML() const;
	wxXmlNode *ExportSelectionToXHTML() const;
#endif
};

#endif // _WX_TEXTSPAN_H_
