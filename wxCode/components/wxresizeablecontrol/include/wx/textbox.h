/////////////////////////////////////////////////////////////////////////////
// Name:        textbox.h
// Purpose:     wxTextBoxLayoutStatus, wxTextBoxInputStatus, wxTextBox
// Author:      Francesco Montorsi
// Created:     2005/8/15
// RCS-ID:      $Id: textbox.h,v 1.5 2005-10-02 19:43:46 frm Exp $
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_TEXTBOX_H_
#define _WX_TEXTBOX_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "textbox.h"
#endif

// wxWidgets headers
#include "wx/textctrl.h"		// holds the definition of wxTextAttr
#include "wx/resizec.h"
#include "wx/textspan.h"
#include <wx/dataobj.h>


// some data structures which will be used...
WX_DECLARE_USER_EXPORTED_OBJARRAY(wxRect, wxRectArray, WXDLLIMPEXP_RESIZEC);

//! The width of the caret of a wxTextBox window.
#define wxTB_CARET_WIDTH			1

//! A wxTextBox::MoveCaret modifier (TBMCM): no modifier on this cursor movement.
#define wxTBMCM_NOMODIFIERS			0

//! A wxTextBox::MoveCaret modifier (TBMCM): user wants to extend the selection
//! while moving the cursor.
#define wxTBMCM_EXTEND_SELECTION	1


// text box cursor positions

//! A wxTextBox::SetCaretPos flag (TBCP): move the caret to the end of the current line.
#define wxTBCP_ENDLINE  		   -1

//! A wxTextBox::SetCaretPos flag (TBCP): move the caret to the beginning of the current line.
#define wxTBCP_STARTLINE		   -2

//! A wxTextBox::SetCaretPos flag (TBCP): move the caret to the end of the textbox.
#define wxTBCP_END        		   -3

//! A wxTextBox::SetCaretPos flag (TBCP): move the caret to the beginning of the textbox.
#define wxTBCP_START    		    0


// context menu IDs

#define wxTBCM_BASEID				wxID_HIGHEST+1

//! The CUT menu item ID.
#define wxTBCM_CUT					wxTBCM_BASEID // won't use wxID_CUT
#define wxTBCM_COPY					wxTBCM_BASEID+1
#define wxTBCM_PASTE				wxTBCM_BASEID+2
#define wxTBCM_SELECTALL			wxTBCM_BASEID+3

#define wxTBCM_FINALID				wxTBCM_BASEID+10

/*
#define wxID_UNDO               5007
#define wxID_REDO               5008

#define wxID_CLEAR              5033
#define wxID_FIND               5034
#define wxID_DUPLICATE          5035
#define wxID_DELETE             5037
#define wxID_REPLACE            5038
#define wxID_REPLACE_ALL        5039
#define wxID_PROPERTIES         5040*/


//! A utility class used to keep the variables used when layouting a wxTextBox.
class WXDLLIMPEXP_RESIZEC wxTextBoxLayoutStatus
{
protected:

	wxTextSpan *m_span;
	int m_idx;

	wxPoint m_curr;
	int m_currheight;

	bool m_bBeginningNewLine;

public:
	wxTextBoxLayoutStatus()
		{ m_span = NULL; m_idx = -1; m_currheight = 0; m_bBeginningNewLine = TRUE; }
	virtual ~wxTextBoxLayoutStatus() {}


	void ClearNewLineFlag()
		{ m_bBeginningNewLine = FALSE; }
	void SetNewLineFlag()
		{ m_bBeginningNewLine = TRUE; }


public:		// getters

	wxTextSpan *GetSpan()
		{ return m_span; }
	int GetSpanIndex()
		{ return m_idx; }
	int GetCurrHeight()
		{ return m_currheight; }

	//! Returns the position of this span in TEXTBOX COORDs.
	wxPoint GetCurrPos()
		{ return m_curr; }

public:		// setters

	void SetSpan(wxTextSpan *ts)
		{ m_span = ts; }
	void SetSpanIdx(int idx)
		{ m_idx = idx; }
	void SetCurrHeight(int ch)
		{ m_currheight = ch; }
	void SetCurrX(int x)
		{ m_curr.x = x; }
	void SetCurrY(int y)
		{ m_curr.y = y; }

	//! Sets the current layouting position in TEXTBOX COORDs.
	void SetCurrPos(const wxPoint &pt)
		{ m_curr = pt; }

public:     // misc

	bool IsBeginningNewLine() const
		{ return m_bBeginningNewLine; }
};


class WXDLLIMPEXP_RESIZEC wxTextBoxInputStatus
{
public:

	//! A reference to the array of spans.
	wxTextSpanArray &m_spans;

	//! The span which contains the caret. Updated from #m_nCaretPos using
	//! #UpdateSpanWithCaret().
	int m_nSpanWithCaret;

	//! The start of the selection. -1 if there is no selection.
	int m_nSpanWithSelStart;

	//! The end of the selection. -1 if there is no selection.
	int m_nSpanWithSelEnd;

public:

	//! The caret position inside the currently focused span.
	//! The caret position in character units. Newlines counts.
	//! If this variable's value is n it means that the caret is placed 
	//! between the n-th and (n+1)-th character of this textbox.
	int m_nCaretPos;

	//! The position of the selection startpoint.
	int m_nSelStart;

	//! The position of the selection endpoint.
	int m_nSelEnd;

public:
	wxTextBoxInputStatus(wxTextSpanArray &arr) : m_spans(arr)
		{ m_nCaretPos = 0; m_nSpanWithCaret = 0; 
			m_nSelStart = -1; m_nSpanWithSelStart = -1;
			m_nSelEnd = -1; m_nSpanWithSelEnd = -1; }
	virtual ~wxTextBoxInputStatus() {}


public:		// getters

	bool HasSelection() const
		{ return (m_nSpanWithSelStart != -1 && m_nSpanWithSelEnd != -1 &&
					m_nSelStart != -1 && m_nSelEnd != -1); }	
	bool IsSpanWithCaret(const wxTextSpan *p) const
		{ return p == GetSpanWithCaret(); }
	bool IsSpanWithSelStart(const wxTextSpan *p) const
		{ return p == GetSpanWithSelStart(); }
	bool IsSpanWithSelEnd(const wxTextSpan *p) const
		{ return p == GetSpanWithSelEnd(); }

	bool IsAllSelected(const wxTextSpan *p) const;


	// caret is placed on the selection endpoint which is being edited
	bool IsExtendingSelStart() const
		{ return (m_nCaretPos == m_nSelStart); }
	bool IsExtendingSelEnd() const
		{ return (m_nCaretPos == m_nSelEnd); }


	int FindSpan(const wxTextSpan *p) const
		{ return m_spans.Index(p); }

	wxTextSpan *GetSpanWithCaret() const
		{ if (m_nSpanWithCaret==-1) return NULL; return m_spans.Item(m_nSpanWithCaret); }
	wxTextSpan *GetSpanWithSelStart() const
		{ if (m_nSpanWithSelStart==-1) return NULL; return m_spans.Item(m_nSpanWithSelStart); }
	wxTextSpan *GetSpanWithSelEnd() const
		{ if (m_nSpanWithSelEnd==-1) return NULL; return m_spans.Item(m_nSpanWithSelEnd); }

    void SetAsSelStartCurrentCaretPos();
    void SetAsSelEndCurrentCaretPos();

/*
	//! Returns TRUE if the last selection rectangle was dragged from
	//! left to right; return FALSE if the selection was dragged from
	//! right to left.
	//! \note Cannot use the m_rcSelection.x & m_rcSelection.width variables
	//!       since they could have been already reset to -1 when this function
	//!       is called.
	bool IsSelectionFromLeftToRight(const wxPoint &pt) const
		{ return m_ptOriginalSelStart.x < pt.x; }*/

	void DeSelect()
		{ m_nSpanWithSelEnd=m_nSpanWithSelStart=m_nSelStart=m_nSelEnd=-1; }


	// char idx

	void SetCaretPosFromCharIdx(int pos) {
		m_nSpanWithCaret = m_spans.GetSpanFromCharIndex(pos);	
		m_nCaretPos = pos - m_spans.GetCharIndexOffset(m_nSpanWithCaret);
	}

	void SetSelStartFromCharIdx(int pos) {
		m_nSpanWithSelStart = m_spans.GetSpanFromCharIndex(pos);	
		m_nSelStart = pos - m_spans.GetCharIndexOffset(m_nSpanWithSelStart);
	}

	void SetSelEndFromCharIdx(int pos) {
		m_nSpanWithSelEnd = m_spans.GetSpanFromCharIndex(pos);	
		m_nSelEnd = pos - m_spans.GetCharIndexOffset(m_nSpanWithSelEnd);
	}
};



//! A resizeable control which acts as an advanced wxTextCtrl.
//! The main features are:
//! - automatic text layouting to avoid overlapping of text with other
//!   resizeable boxes in the same canvas.
//! - handles an arbitrary number of wxTextSpan and an arbitrary number
//!   of wxTextStyle
//! All these features make wxTextBox a powerful window which can be used
//! as a small word processor.
class WXDLLIMPEXP_RESIZEC wxTextBox : public wxWindow
{
protected:		// core

	//! The styles which can be used in this box.
	wxTextStyleArray m_styles;

	//! Our text content.
	wxTextSpanArray m_spans;

protected:		// input & cursor

	//! If TRUE the user can type in the window.
	bool m_bInputEnabled;

	//! If TRUE the user can select text in this window.
	bool m_bSelectionEnabled;

	//! If TRUE the user can use the right-clicks to popup the context menu.
	bool m_bContextMenuEnabled;

protected:		// our current input status

	//! TODO
	wxTextBoxInputStatus m_input;

	//! This must not be saved in undo/redo buffers.
	bool m_bSelecting;
	
	//! The original selection start point when selecting using mouse.	
	wxPoint m_ptOriginalSelStart;

protected:		// regenerated by Layout() & co.

	//! The array of the last clipped box.
	//! Used by Layout()-function helpers.
	wxRectArray m_rc;

	//! An array of indexes to the spans which begin a new line in the textbox.
	wxArrayInt m_lines;


protected:		// layouting-utilities

	//! Clips the other resizeable boxes in the canvas for layouting processing,
	//! and transform them in TEXTBOX coords. The clipped boxes are then stored
	//! in the #m_rc array.
	void ClipOtherBoxes();

	void MoveOnNewLine(wxTextBoxLayoutStatus &curr);
	bool AdjustSpanPosition(wxTextBoxLayoutStatus &p);
	bool AdjustSpanSize(wxTextBoxLayoutStatus &p);
	bool SetToMaxSize(wxTextBoxLayoutStatus &p);
	bool SetToMinSize(wxTextBoxLayoutStatus &p);
	bool IsLastOfTheLine(wxTextBoxLayoutStatus &p) const;
	wxTextSpanTruncFlag TruncateToSize(wxTextBoxLayoutStatus &p);
	
protected:      // miscellaneous utilities

    void OnUserCaretMove(long modifiers);
	
public:
    wxTextBox(wxWindow *parent, int id, const wxPoint &pos = wxDefaultPosition,
              const wxSize &size = wxDefaultSize, long style = 0,
              const wxString &name = wxPanelNameStr);
	virtual ~wxTextBox() {}
	
public:		// layout

	//! Returns the canvas which contains this resizeable control.
	wxResizeableControlCanvas *GetCanvas()
	{ return ((wxResizeableParentControl*)GetParent())->GetCanvas(); }

	bool Layout();

	int GetWidth() const
		{ return GetClientSize().GetWidth(); }
	int GetHeight() const
		{ return GetClientSize().GetHeight(); }
	int GetNumberOfLines() const
		{ return m_lines.GetCount(); }
	int GetLineMaxHeight(int linenum) const;

	wxRect GetRectOfContents() const
		{ return wxRect(GetPosition(), GetVirtualSize()); }

	//! Checks if the given point, which must be in SCREEN coords, is inside
	//! the rect of contents.
	bool IsInsideContents(const wxPoint &pt) const
		{ return GetRectOfContents().Inside(pt); }

	//! Checks if the given point, which must be in SCREEN coords, is inside 
	//! the current selection.
	bool IsInsideSelection(const wxPoint &pt) const;

	//! Converts the given point, which must be in SCREEN coords, to TEXTBOX coords.
	wxPoint ConvertToTextboxCoord(const wxPoint &pt) const
		{ wxPoint p(pt); p -= GetPosition(); return p; }

public:		// input & caret

	void GetContextMenu(wxMenu &, bool onselection);

//    wxMoveCaretRes SetCaretPosByUser(int pos, long modifiers);
	wxMoveCaretRes MoveCaret(wxMoveCaretFlag flag, long modifiers);

	//! Moves the caret to the caret valid position which is nearest to the
	//! given point, which must be SCREEN COORDs.
	wxMoveCaretRes MoveCaretUsingPoint(const wxPoint &pos);
	int GetSpanWithCaretUsingPoint(const wxPoint &p) const;
	wxPoint AdjustPointForCaretMove(const wxPoint &p) const;

	wxInputRes OnInput(const wxKeyEvent &key);
	void OnInputMergeSpan(int idx1, int idx2);

	wxTextBoxInputStatus GetInputStatus() const
		{ return m_input; }
	/*void SetInputStatus(const wxTextBoxInputStatus &s)
		{ m_input = s; }*/

	void AdjustCaretPos(bool moveleft);

	//! Sets the caret to the given position.
	wxMoveCaretRes SetCaretPos(int pos, long modifiers);
	void SetCaretPos(int pos)
	   { SetCaretPos(pos, 0); }

	//! Returns the span with the caret inside.
	wxTextSpan *GetSpanWithCaret() const
		{ return m_input.GetSpanWithCaret(); }

	int GetLineWithCaret() const;

	//! Updates the #m_nSpanWithCaret variable.
	void UpdateSpanWithCaret();

	//! Updates the position of the window's caret and its size.
	void UpdateCaret();

	void EnableInput(bool enable = TRUE)
		{ m_bInputEnabled = enable; UpdateCaret(); }
	void DisableInput()
		{ EnableInput(false); }

public:		// selection

	bool HasSelection() const
		{ return m_input.HasSelection(); }

	void DeSelect()
		{ m_input.DeSelect(); /*Refresh(); don't call - user task */ }
	
	void SetSelection(int from, int to);

	void SelectAll()
		{ SetSelection(-1, -1); }

	void SelectUsingRect(const wxPoint &start, const wxPoint &end);

public:		// import

	bool ImportRTF(const wxString &str);
#if wxUSE_TEXTBOX_XHTML_SUPPORT
	bool ImportXHTML(const wxString &str);
#endif

public:		// export

	wxString ExportRTF() const;
	wxString ExportSelectionToRTF() const;

#if wxUSE_TEXTBOX_XHTML_SUPPORT
	wxXmlNode *ExportXHTML() const;
	wxXmlNode *ExportSelectionToXHTML() const;
#endif

public:		// cut, copy & paste

	void Cut() {}
	void Copy();
	void Paste() {}

public:		// text

	void Clear();

	void SetText(const wxString &txt, const wxTextStyle &attr);
	void AppendText(const wxString &txt, const wxTextStyle &attr);
	
	wxString GetAllText() const
		{ return m_spans.GetAllText(); }
	void SetText(const wxString &txt, const wxString &stylename)
		{ SetText(txt, GetStyle(stylename)); }
	void AppendText(const wxString &txt, const wxString &stylename)
		{ AppendText(txt, GetStyle(stylename)); }

public:		// styles-handling

	wxTextStyle &AddStyle(const wxTextStyle &toadd);

	void RemoveStyles()
		{ m_styles.Empty(); m_spans.RemoveStyles(); }
	bool HasStyle(const wxString &name) const
		{ return GetStyle(name) != *wxNullTextStyle; }

	void InitDefaultStyles();

	const wxTextStyle &GetConstStyle(const wxString &name) const;
	
	wxTextStyle &GetStyle(const wxString &name)
		{ return (wxTextStyle &)GetConstStyle(name); }
	const wxTextStyle &GetStyle(const wxString &name) const
		{ return GetConstStyle(name); }

public:		// events

	void OnPaint(wxPaintEvent &);
	void OnSize(wxSizeEvent &ev);
	void OnMove(wxMoveEvent &ev);
	void OnSiblingChange(wxCommandEvent &);
	void OnChar(wxKeyEvent &);

	// mouse events
	void OnLDown(wxMouseEvent &me);
	void OnLUp(wxMouseEvent &me);
	void OnRUp(wxMouseEvent &me);
	void OnMouseMove(wxMouseEvent &me);

	// menu events
	void OnCtxMenuItem(wxCommandEvent &);

	// required when we are contained into a wxResizeableParentControl
    void OnSetFocus(wxFocusEvent &);
    void OnKillFocus(wxFocusEvent &);

private:
    DECLARE_EVENT_TABLE()
};


//! An RTF data object inside the clipboard.
class wxRTFDataObject : public wxDataObject
{
public:
	char *m_buf;
	size_t m_len;

public:
	wxRTFDataObject() { m_buf=NULL; }
	virtual ~wxRTFDataObject() {}


    virtual wxDataFormat GetPreferredFormat(Direction WXUNUSED(dir) = Get) const
	{ return wxT("Rich Text Format"); }

    virtual size_t GetFormatCount(Direction WXUNUSED(dir) = Get) const
	{ return 1; }

    virtual void GetAllFormats(wxDataFormat *formats, Direction WXUNUSED(dir) = Get) const
	{ formats[0] = wxDataFormat(wxT("Rich Text Format")); }
    
    virtual size_t GetDataSize(const wxDataFormat& WXUNUSED(format)) const
	{ return m_len; }

	bool SetData(const wxDataFormat& WXUNUSED(format), size_t len, const void *buf)
	{ wxDELETEA(m_buf); m_buf = new char[len]; m_len=len; memcpy(m_buf, buf, len); m_buf[len] = 0; return TRUE;}

	virtual bool NeedsVerbatimData(const wxDataFormat& WXUNUSED(format)) const
	{ return TRUE; }

	virtual bool GetDataHere(const wxDataFormat& WXUNUSED(format), void *buf) const
	{ memcpy(buf, m_buf, m_len); return TRUE; }
};


#endif // _WX_TEXTBOX_H_
