/////////////////////////////////////////////////////////////////////////////
// Name:        textspan.cpp
// Purpose:     wxTextStyle, wxTextSpan, wxTextSpanArray
// Author:      Francesco Montorsi
// Created:     2005/8/19
// RCS-ID:      $Id: textspan.cpp,v 1.6 2005-09-18 10:05:29 frm Exp $
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifdef __GNUG__
#pragma implementation "textspan.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
	#include <wx/dcclient.h>
	#include <wx/dcscreen.h>
	#include <wx/log.h>
#endif

#include "wx/textspan.h"
#include "wx/textbox.h"
#include <wx/tokenzr.h>
#include <wx/caret.h>


#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(wxTextStyleArray);

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(wxTextSpanArrayBase);


// global objects
wxTextSpan *wxEmptyTextSpan = NULL;
wxTextStyle *wxNullTextStyle = NULL;

#ifdef __WXDEBUG__
	int wxTextSpanArray::s_nMaxUsed = 0;
#endif




// -----------------
// GLOBALs
// -----------------

bool wxRectInside(const wxRect &bigger, const wxRect &smaller)
{
	if (bigger.Inside(smaller.GetTopLeft()) &&
		bigger.Inside(smaller.GetBottomRight()))
		return TRUE;
	return FALSE;
}

wxXmlNode *wxCreateElemNode(const wxString &name)
{ 
	// create a node which acts as a container
	return new wxXmlNode(NULL, wxXML_ELEMENT_NODE, name, wxEmptyString, NULL, NULL); 
}

wxXmlNode *wxCreateElemTextNode(const wxString &name, const wxString &content = wxEmptyString)
{ 
	wxXmlNode *n = wxCreateElemNode(name); 

	// add to the container node a text node
	n->AddChild(new wxXmlNode(NULL, wxXML_TEXT_NODE, wxEmptyString, content, NULL, NULL));
	return n;
}

wxXmlNode *wxCreateElemTextNode(wxXmlNode *prev, const wxString &name, const wxString &content = wxEmptyString)
{ 
	// links with previous node
	wxXmlNode *n = wxCreateElemTextNode(name, content);
	prev->SetNext(n);

	return n;
}





// -----------------
// wxTEXTSTYLE
// -----------------

wxTextStyle::wxTextStyle(const wxTextAttr &attr, 
						 const wxString &name, 
						 wxTextSpanTruncMode tm,
						 wxTextStyleVertAlignment va,
						 int bkmode)
{
	m_style = attr; 
	m_strName=name; 
	m_truncMode=tm; 
	m_nBkMode=bkmode;
	SetVertAlignment(va); 
	SetHorizAlignment(attr.GetAlignment());
	CacheData();
}

void wxTextStyle::CacheData()
{
	wxArrayInt w;
	wxScreenDC dc; 

	// compute the average char width
	Select(dc); 
	m_nAveCharWidth = dc.GetCharWidth();
	m_nAveCharHeight = dc.GetCharHeight();

	m_nMinCharWidth = 0xFFFFFFF;

	// the first character is not evaluated by our FOR loop so we repeat it twice
	dc.GetPartialTextExtents(wxT("..,-!\\/()'[]{}^ì"), w);
	for (int i=1; i<(int)w.GetCount(); i++)
		m_nMinCharWidth = wxMin(m_nMinCharWidth, w[i]-w[i-1]);
}

void wxTextStyle::Select(wxDC &dc) const
{
	dc.SetTextForeground(m_style.GetTextColour());
	dc.SetTextBackground(m_style.GetBackgroundColour());
	dc.SetFont(m_style.GetFont());
	dc.SetBackgroundMode(m_nBkMode);
}

void wxTextStyle::SetHorizAlignment(wxTextAttrAlignment align)
{
	if (align == wxTEXT_ALIGNMENT_DEFAULT)
		align = wxTEXT_ALIGNMENT_LEFT;
	wxASSERT_MSG(align != wxTEXT_ALIGNMENT_JUSTIFIED, wxT("Not supported !"));
	m_style.SetAlignment(align);
}

void wxTextStyle::SetVertAlignment(wxTextStyleVertAlignment align)
{
	m_alignVert = align;
}

void wxTextStyle::SetNewNameFromCurrentName()
{
	if (!wxString(m_strName.Last(), 1).IsNumber()) {

		m_strName += wxT("2");
		return;
	}		

	// extract the number portion from the end of the name of this string
	int offset = m_strName.Len()-1;
	while (m_strName.Mid(offset).IsNumber())
		offset--;

	long num;
	m_strName.Mid(offset).ToLong(&num);

	wxString newname = m_strName.Left(offset) + wxString::Format(wxT("%d"), num+1);
	m_strName = newname;
}



// -----------------------------------------
// wxTEXTSTYLE - export
// -----------------------------------------

wxString wxTextStyle::ExportRTF() const
{
	wxString ret;

	ret += wxT("");
//	if (EndsWithDelimiter())
//		ret += wxT(" \\line \n");		// the space at the end is important !

	return ret;
}

wxXmlNode *wxTextStyle::ExportXHTML() const
{
	// we "entirely" differ from the wxNullTextStyle
	return ExportXHTMLDiffFrom(wxNullTextStyle);
}

wxXmlNode *wxTextStyle::ExportXHTMLDiffFrom(const wxTextStyle *p) const
{
	wxString value;
#define wxTS_CSS_SEPARATOR			wxT("; ")

	// font facename
	if (p == wxNullTextStyle ||
		p->GetFont().GetFaceName() != GetFont().GetFaceName()) {
		value += wxT("font-family: ");

		// first the generic family
		switch (GetFont().GetFamily()) {
		case wxFONTFAMILY_SCRIPT:
			value += wxT("cursive, ");
			break;
		case wxFONTFAMILY_DECORATIVE:
			value += wxT("fantasy, ");
			break;
		case wxFONTFAMILY_DEFAULT:
		case wxFONTFAMILY_SWISS:
			value += wxT("sans-serif, ");
			break;
		case wxFONTFAMILY_TELETYPE:
			value += wxT("monospace, ");
			break;
		case wxFONTFAMILY_ROMAN:
			value += wxT("serif, ");
			break;
		}

		// then the font family name
		value += GetFont().GetFaceName();
	}

	// font size
	if (p == wxNullTextStyle ||
		p->GetFont().GetPointSize() != GetFont().GetPointSize()) {
		if (!value.IsEmpty()) value += wxTS_CSS_SEPARATOR;
		value += wxString::Format(wxT("font-size: %dpt"), GetFont().GetPointSize());
	}

	// font style
	if (p == wxNullTextStyle ||
		p->GetFont().GetStyle() != GetFont().GetStyle()) {
		if (!value.IsEmpty()) value += wxTS_CSS_SEPARATOR;
		value += wxT("font-style: ");

		switch (GetFont().GetStyle()) {
		case wxFONTSTYLE_NORMAL:
			value += wxT("normal");
			break;
		case wxFONTSTYLE_SLANT:
			value += wxT("???");
			break;
		case wxFONTSTYLE_ITALIC:
			value += wxT("italic");
			break;
		}
	}

	// font decoration
	if (p == wxNullTextStyle ||
		p->GetFont().GetUnderlined() != GetFont().GetUnderlined()) {
		if (!value.IsEmpty()) value += wxTS_CSS_SEPARATOR;

		if (GetFont().GetUnderlined())
			value += wxT("text-decoration: underline");
		else
			value += wxT("text-decoration: none");
	}

	// font weight
	if (p == wxNullTextStyle ||
		p->GetFont().GetWeight() != GetFont().GetWeight()) {
		if (!value.IsEmpty()) value += wxTS_CSS_SEPARATOR;
		value += wxT("font-weight: ");

		switch (GetFont().GetWeight()) {
		case wxFONTWEIGHT_NORMAL:
			value += wxT("normal");
			break;
		case wxFONTWEIGHT_LIGHT:
			value += wxT("200");
			break;
		case wxFONTWEIGHT_BOLD:
			value += wxT("bold");
			break;
		}
	}
	
	// alignment
	if (p == wxNullTextStyle ||
		p->GetHorizAlignment() != GetHorizAlignment()) {
		if (!value.IsEmpty()) value += wxTS_CSS_SEPARATOR;
		value += wxT("text-align: ");

		switch (GetHorizAlignment()) {
		case wxTEXT_ALIGNMENT_LEFT:
			value += wxT("left");
			break;
		case wxTEXT_ALIGNMENT_CENTER:
			value += wxT("center");
			break;
		case wxTEXT_ALIGNMENT_RIGHT:
			value += wxT("right");
			break;
		case wxTEXT_ALIGNMENT_JUSTIFIED:
			value += wxT("justify");
			break;
		}
	}
	
	// text background colour
	if (p == wxNullTextStyle ||
		p->GetBackgroundColour() != GetBackgroundColour()) {
		if (!value.IsEmpty()) value += wxTS_CSS_SEPARATOR;
		value += wxT("background-color: ");

		wxColour col(GetBackgroundColour());

		if (m_nBkMode == wxTRANSPARENT)
			value += wxT("transparent");
		else
			value += wxString::Format(wxT("rgb(%d,%d,%d)"), 
							col.Red(), col.Green(), col.Blue());
	}		
	
	// text foreground colour
	if (p == wxNullTextStyle ||
		p->GetForegroundColour() != GetForegroundColour()) {
		if (!value.IsEmpty()) value += wxTS_CSS_SEPARATOR;
		value += wxT("color: ");

		wxColour col(GetForegroundColour());
		value += wxString::Format(wxT("rgb(%d,%d,%d)"), 
					col.Red(), col.Green(), col.Blue());
	}	
	
	// build a <span> tag with a "style" property
	wxXmlProperty *prop = new wxXmlProperty(wxT("style"), value, NULL);
	return new wxXmlNode(NULL, wxXML_ELEMENT_NODE, 
							wxT("span"), wxEmptyString, prop, NULL);
}






// -----------------
// wxTEXTSPAN
// -----------------

// static
void wxTextSpan::Init()
{
	// NB: the order is important since wxEmptyTextSpan uses wxNullTextStyle
	wxNullTextStyle = new wxTextStyle(wxTextAttr(wxColour(0, 0, 0)), wxT("default"));
	wxEmptyTextSpan = new wxTextSpan(wxT("invalid"));
}

// static
void wxTextSpan::Cleanup()
{
	// NB: the order is important since wxEmptyTextSpan uses wxNullTextStyle
	wxDELETE(wxEmptyTextSpan);
	wxDELETE(wxNullTextStyle);
}

void wxTextSpan::Set(const wxString &txt, wxTextStyle *attr,
					 wxTextBoxInputStatus *input,
					const wxPoint &pos, const wxSize &sz)
{
	wxASSERT_MSG(!txt.IsEmpty(), wxT("An empty span ?!?"));

	m_text=txt; 
	m_style=attr; 
	m_input=input;
	m_pos=pos; 
	m_size=sz; 

	RecalcSize(); 
}

int wxTextSpan::GetTruncateSize() const
{
	wxArrayInt widths = GetPartialTextExtent();
	int i;

	// find the width of the character who crosses the span
	for (i=0; i < (int)widths.GetCount() && widths[i] < m_size.x; i++)
		;

	if (i == (int)widths.GetCount())
		return wxTSTF_TOOBIG;
	int tokeep = (i-1) > 0 ? (i-1) : 0;

	// check again the num of characters to keep
	if (tokeep == 0)
		return wxTSTF_TOOSMALL;
	if (tokeep >= (int)GetTextWithoutDelim().Len())
		return wxTSTF_TOOBIG;
	return tokeep;
}

wxTextSpanTruncFlag wxTextSpan::CanBeSplit() const
{
	// don't perform this check because 
	// 1) this case is handled below
	// 2) wxTSTF_TOOSMALL can be wrong and wxTSTF_TOOBIG could be required
	//if (m_text.Len() == 1)
	//	return wxTSTF_TOOSMALL;				// cannot split single characters !

	// this calculation cannot be completely exact since to be faster
	// we use GetAveCharWidth() as the average character width...
#if 0
	int tokeep = (m_size.x-wxTS_LEFTRIGHT_BORDER*2)/GetAveCharWidth();
	if (tokeep <= 0)					// the current width is too small;
		return wxTSTF_TOOSMALL;			// even a single char takes more space !
	if (tokeep >= (int)m_text.Len())	// the current width is big enough...
		return wxTSTF_TOOBIG;			// cannot split (the second span would be empty)

	return wxTSTF_TRUNCATED;
#else

	int ret = GetTruncateSize();
	return (ret > 0 ? wxTSTF_TRUNCATED : (wxTextSpanTruncFlag)ret);

#endif
}

bool wxTextSpan::IsTrueSize() const
{
	wxSize sz = DoRecalcSize();
	if (sz != m_size)
		return FALSE;
	return TRUE;
}

wxArrayInt wxTextSpan::GetPartialTextExtent() const
{
	wxASSERT(m_style);

	// handle the simplest case...
	wxArrayInt widths;
	if (IsDelimiter())
		return widths;		// an empty array

	wxScreenDC dc;

	// do our slow job
	m_style->Select(dc);
	dc.GetPartialTextExtents(GetTextWithoutDelim(), widths);

	return widths;
}

void wxTextSpan::UpdateCaretPosForTruncateOperation(int tokeep, int *span, int *pos)
{
	// is this the span with the caret ?
	int spanidx = m_input->m_spans.Index(this);
	if (spanidx == *span) {
		if (*pos <= tokeep) {

			// the caret is still in this span and not on its completary 
			// span which will be returned in the given argument.
			// no need to change anything

		} else {

			// the caret will be in the second half of the span generated by
			// this function...
			*pos -= tokeep;
			(*span)++;
		}

	} else if (spanidx < *span) {

		// since we are adding a new span immediately after *this
		// and this span is placed *before* the span with the caret,
		// in order to preserve the correct index of the span with the
		// caret we must simply increment the span-with-caret index
		(*span)++;
	}
}

wxTextSpanTruncFlag wxTextSpan::TruncateToSize(wxTextSpan &ret)
{	
	// OPTIMIZATION: we won't use CanBeSplit() directly, just duplicate
	//               partially its code since we will reuse the 'tokeep'
	//               variable (and #GetTruncateSize() is *slow*).
	int tokeep = GetTruncateSize();
	if (tokeep < 0)
		return (wxTextSpanTruncFlag)tokeep;

	// proceed with truncation
	ret.Copy(*this);

	// then check if we can honour our style's truncation mode
	wxString leftpiece = m_text.Left(tokeep);
	wxASSERT(!leftpiece.IsEmpty());	
	int idx;

	switch (m_style->GetTruncMode()) {
	case wxTSTM_WORDWRAP:

		// find the first word separator
		idx = leftpiece.Find(wxT(' '), TRUE);
		if (idx != wxNOT_FOUND) {

			// we've found a good place where we can break the string
			tokeep = idx+1;
			leftpiece = leftpiece.Left(tokeep);
			wxASSERT(!leftpiece.IsEmpty());	

		} else {

			// fallback to char wrapping 
		}
		break;

	case wxTSTM_CHARWRAP:
		break;

	default:
		wxASSERT_MSG(0, wxT("unknown truncation mode"));
	}

	// update caret positions (required for all operations
	// or addition/remotion of spans and thus also for truncations)
	UpdateCaretPosForTruncateOperation(tokeep,
		&m_input->m_nSpanWithCaret, &m_input->m_nCaretPos);
	UpdateCaretPosForTruncateOperation(tokeep,
		&m_input->m_nSpanWithSelStart, &m_input->m_nSelStart);
	UpdateCaretPosForTruncateOperation(tokeep,
		&m_input->m_nSpanWithSelEnd, &m_input->m_nSelEnd);

	// setup the completary span
	ret.m_text = m_text.Right(m_text.Len() - tokeep);
	wxASSERT(!ret.m_text.IsEmpty());
	ret.RecalcSize();

	// and remove that piece from this span
	m_text = leftpiece;
	int newwidth = DoRecalcSize().GetWidth();
	wxASSERT_MSG(newwidth < m_size.GetWidth(),
				wxT("This span could not be shrinked enough..."));
	wxLogDebug(wxT("wxTextSpan::TruncateToSize - the truncation error was %d pixels (%d%%) when ")
				wxT("breaking the span [") + GetTextWithoutDelim() + ret.GetTextWithoutDelim() + wxT("]"),
				m_size.GetWidth()-newwidth, (m_size.GetWidth()-newwidth)*100/m_size.GetWidth());

	// NOTE: we don't need to adjust our size:
	//            m_size.x = newwidth;
	
	// all other settings (a part the text) must be the same of this span
	return wxTSTF_TRUNCATED;
}

void wxTextSpan::MergeWith(wxTextSpan &s)
{
	wxASSERT(m_input);

	// is this the span with the caret ?
	int idx = m_input->m_spans.Index(&s);
	if (idx == m_input->m_nSpanWithCaret) {

		// adjust the caret position: it was placed on the span we are
		// merging with and thus it needs to take in count the additional
		// text we are prepending to the final span
		m_input->m_nCaretPos += m_text.Len();
		m_input->m_nSpanWithCaret = m_input->m_spans.Index(this);

	} else if (idx < m_input->m_nSpanWithCaret) {

		// since we are adding a new span immediately after *this
		// and this span is placed *before* the span with the caret,
		// in order to preserve the correct index of the span with the
		// caret we must simply increment the span-with-caret index
		m_input->m_nSpanWithCaret--;
	}

	// merge is easier than truncation...
	m_text += s.m_text;
	RecalcSize();
}

wxString wxTextSpan::GetTextWithoutDelim(bool trim) const
{
	wxString ret(m_text);
	
	if (m_text.Last() == wxTS_DELIMITER)
		ret.RemoveLast();
		
	if (trim) {
		ret.Trim(TRUE);
		ret.Trim(FALSE);
	}

	// See rule #1: the delimiter can be only at the end of a span.
	wxASSERT_MSG(!ret.Contains(wxTS_DELIMITER), 
		wxT("This text should have been split in two spans !"));
	return ret;
}

bool wxTextSpan::IsLastOfTheLine() const
{
	wxASSERT(m_style && !m_text.IsEmpty());
	if (EndsWithDelimiter())
		return TRUE;
//	if (GetHorizAlignment() != wxTEXT_ALIGNMENT_LEFT)	// for us, default = left
//		return TRUE;
	return FALSE;
}

wxSize wxTextSpan::DoRecalcSize() const
{
	wxASSERT(m_style && !m_text.IsEmpty());
	wxScreenDC dc;

	// select the style
	m_style->Select(dc);

	int w, h;
	dc.GetTextExtent(GetTextWithoutDelim(), &w, &h);
	if (h == 0 || w == 0) {

		wxASSERT_MSG(GetTextWithoutDelim().IsEmpty(),
					wxT("Couldn't get the text extent of ") + m_text);

		// we cannot have spans with empty text but we can have
		// spans containing just the newline "\n"... in this case
		// we set just the height of the span...
		h = m_style->GetAveCharHeight();
	}

	// add the borders
	return wxSize(w+wxTS_LEFTRIGHT_BORDER*2, h);
}

int wxTextSpan::GetTextOffsetX() const
{
	// honour the current alignment...
	wxSize sz = DoRecalcSize();
	int x = wxTS_LEFTRIGHT_BORDER;		// by default left-aligned

	if (m_style && m_style->GetHorizAlignment() == wxTEXT_ALIGNMENT_CENTER)
		x = (m_size.x - sz.x)/2;
	if (m_style && m_style->GetHorizAlignment() == wxTEXT_ALIGNMENT_RIGHT)
		x = m_size.x - sz.x - wxTS_LEFTRIGHT_BORDER;

	// we won't handle the _JUSTIFY case... too complex !

	return x;
}

void wxTextSpan::Paint(wxDC &dc)
{
	wxASSERT(m_style && !m_text.IsEmpty());

	// select the style
	m_style->Select(dc);

#ifdef __WXFULLDEBUG__
	if (IsDelimiter())
		dc.SetBrush(*wxGREEN_BRUSH);
	else
		dc.SetBrush(*wxLIGHT_GREY_BRUSH);
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.DrawRectangle(m_pos, m_size);
#endif

	// without this the background could be erased...	
	dc.DrawText(GetTextWithoutDelim(TRUE), 
		m_pos.x+GetTextOffsetX(), m_pos.y+GetTextOffsetY());

	// *after* drawing the text, eventually draw also the selection...
	bool hasstart = m_input->IsSpanWithSelStart(this);
	bool hasend = m_input->IsSpanWithSelEnd(this);
	bool isallselected = m_input->IsAllSelected(this);
	if (!hasstart && !hasend && !isallselected)
		return;

	wxRect rc;
	if (isallselected) {

		// just draw the rectangle on all the span space
		rc = wxRect(m_pos.x, m_pos.y, m_size.x, m_size.y);

	} else {
		
		wxArrayInt w = GetPartialTextExtent();
		w.Insert(0, 0);
		
		int x = 0, width = 0;
		if (hasstart && hasend) {
			
			// GetTextOffsetX() is added later...
			x = w[m_input->m_nSelStart];
			width = w[m_input->m_nSelEnd] - x;
			x += GetTextOffsetX();
			
		} else if (hasstart && !hasend) {
			
			x = GetTextOffsetX() + w[m_input->m_nSelStart];
			width = m_size.x;
			
		} else if (!hasstart && hasend) {
			
			x = 0;		// don't add GetTextOffsetX() 
			width = GetTextOffsetX() + w[m_input->m_nSelEnd];
		}
		
		// now we can build the rectangle
		rc = wxRect(m_pos.x+x, m_pos.y+GetTextOffsetY(), width, m_size.y);
	}

	int old = dc.GetLogicalFunction();
	dc.SetLogicalFunction(wxINVERT);
	dc.DrawRectangle(rc);
	dc.SetLogicalFunction(old);
}



// -----------------------------------------
// wxTEXTSPAN - input & cursor
// -----------------------------------------

void wxTextSpan::CheckCaretPos()
{
	// see wxTextSpanArray::GetSpanCaretOffset
	wxASSERT(m_input && m_input->m_nCaretPos >= 0);

	if (!m_input->IsSpanWithCaret(this))
		return;
 
    // if we end with the delimiter, then we allow up to m_text.Len()-1 caret positions:
    // 
	// char index:    0   1   2   3   4   5     (m_text.Len() = 6)
	//              .---.---.---.---.---.---.
	//              | H | e | l | l | o | \n|
	//              ^---^---^---^---^---^---^
	// caret pos:   0   1   2   3   4   5   6 <= this last position is not available!
	//                                 ^^^== last valid caret position is m_text.Len()-1
	//
	// if we do NOT end with the delimiter, then we allow up to m_text.Len() caret positions:
	//
	// char index:    0   1   2   3   4   5     (m_text.Len() = 6)
	//              .---.---.---.---.---.---.
	//              | H | e | l | l | o | ! |
	//              ^---^---^---^---^---^---^
	// caret pos:   0   1   2   3   4   5   6 <= available, in this case
	//                                     ^^^== last valid position is m_text.Len()
    //
	// NB: (each ^ symbol indicates a caret possible position)
    //
    if (EndsWithDelimiter()) 
		wxASSERT_MSG(m_input->m_nCaretPos < (int)m_text.Len(),
                        wxT("wrong caret position"));
    else
		wxASSERT_MSG(m_input->m_nCaretPos <= (int)m_text.Len(),
                        wxT("wrong caret position"));
                        
    // the two rules above can be rewritten as:
	wxASSERT_MSG(m_input->m_nCaretPos <= (int)GetTextWithoutDelim().Len(),
                        wxT("wrong caret position"));
}

wxPoint wxTextSpan::GetCaretOffset()
{
    CheckCaretPos();
    
	// handle the simplest case: the caret is placed at the beginning of this span
	wxString txt(GetTextWithoutDelim());
	if (txt.IsEmpty() || m_input->m_nCaretPos == 0)
		return wxPoint(m_pos.x+GetTextOffsetX(), m_pos.y+GetTextOffsetY());

	// we can also handle faster the case in which the cursor is placed
	// at the end of the span
	if (m_input->m_nCaretPos == (int)txt.Len())
		return wxPoint(m_pos.x+GetTextOffsetX()+DoRecalcSize().x, 
						m_pos.y+GetTextOffsetY());

	// need to get the size of each char
	wxArrayInt widths = GetPartialTextExtent();

	// here we are sure that m_nCaretPos > 0
	// NOTE: we return a point in TEXTSPAN COORD.
	return wxPoint(m_pos.x+GetTextOffsetX()+widths[m_input->m_nCaretPos-1], 
					m_pos.y+GetTextOffsetY());
}

wxInputRes wxTextSpan::Input(int keycode, wxTextSpan **pnew)
{
	wxASSERT(m_input);

	// handle DELETE (i.e. backspace) key
	switch (keycode) {
	case WXK_BACK:

		if (m_input->m_nCaretPos > 0) {

			// remove the character before the focused one
			m_text = m_text.Remove(m_input->m_nCaretPos-1, 1);
			m_input->m_nCaretPos--;

			if (m_text.IsEmpty()) {
				wxASSERT(m_input->m_nCaretPos == 0);
				return wxIR_DELETE_THIS;
			}

		} else {

			// cursor is placed at the beginning of this span
			if (IsDelimiter())
				return wxIR_DELETE_THIS;
			return wxIR_DELETE_PREVIOUS;
		}

		return wxIR_RELAYOUT;

	// handle CANCEL key
	case WXK_DELETE:
	case WXK_CANCEL:

		if (m_input->m_nCaretPos < (int)GetTextWithoutDelim().Len()) {

			// remove the character after the focused one
			m_text = m_text.Remove(m_input->m_nCaretPos, 1);
			// don't touch m_input->m_nCaretPos

			if (m_text.IsEmpty())
				return wxIR_DELETE_THIS;

		} else {

			// cursor is placed at the end of this span
			if (IsDelimiter())
				return wxIR_DELETE_THIS;
			return wxIR_DELETE_NEXT;
		}

		return wxIR_RELAYOUT;

	// handle the RETURN key
	case WXK_RETURN:
		{
			*pnew = new wxTextSpan(*this);

			// remove from this span the text placed after the caret
			m_text.Truncate(m_input->m_nCaretPos);
			m_text += wxTS_DELIMITER;

			// remove from our new span the text placed before the caret
			(*pnew)->m_text.Remove(0, m_input->m_nCaretPos);

			// move caret in the new span
			m_input->m_nCaretPos = 0;
			m_input->m_nSpanWithCaret++;

			// if the new span is an empty span, it is because we had
			// the caret placed in the last available position and we need
			// to create a delimiter span when user hits enter in those cases
			if ((*pnew)->IsEmpty())
				(*pnew)->m_text = wxTS_DELIMITER;
		}
		return wxIR_INSERT_NEWSPAN_AFTER_THIS;

	default:		// handle all other keys

		// discard all invalid inputs
		if (IsToReject((wxChar)keycode)) {
			wxBell();
			return wxIR_OKAY;
		}

		// insert char and advance cursor
		m_text.insert(m_input->m_nCaretPos, wxString((wxChar)keycode, 1));
		m_input->m_nCaretPos++;
	
		{
			// if the new characters make the span bigger than the size which
			// we have currently assigned to it, we need to relayout this textbox
			wxSize currentsz = GetTrueSize();			
			if (currentsz.x > m_size.x || currentsz.y > m_size.y)
				return wxIR_RELAYOUT;
		}	
	}

	return wxIR_OKAY;
}

void wxTextSpan::OnMoveCaret(wxMoveCaretFlag flag, long modifiers, bool &extleft, bool &extright)
{
    extleft = FALSE;
    extright = FALSE;
    
	if (!(modifiers & wxTBMCM_EXTEND_SELECTION))
        return;

	extright = (m_input->IsExtendingSelEnd() && m_input->IsSpanWithSelEnd(this));
	extleft = (m_input->IsExtendingSelStart() && m_input->IsSpanWithSelStart(this));

	// are we starting a new selection by keyboard ?
	if (!extright && !extleft && 
		(flag == wxMCF_LEFT || flag == wxMCF_RIGHT)) {
		wxASSERT_MSG(!m_input->HasSelection(), wxT("Caret is in a bad place..."));

		// place the selection start & end points into this span
		m_input->SetAsSelStartCurrentCaretPos();
		m_input->SetAsSelEndCurrentCaretPos();

		// now set the flag for the right direction
		if (flag == wxMCF_LEFT) extleft = TRUE;
		if (flag == wxMCF_RIGHT) extright = TRUE;
	}
}

void wxTextSpan::OnSelChanged()
{
	if (m_input->m_nSelStart == m_input->m_nSelEnd &&
		m_input->m_nSpanWithSelStart == m_input->m_nSpanWithSelEnd) {

		// the selection has been collapsed to zero chars: e.g. the user selects
		// two chars moving caret right + shift and then moves the caret on the left
		// twice (holding shift down): the selection does not exist anymore.
		m_input->DeSelect();
	}
}

wxMoveCaretRes wxTextSpan::MoveCaret(wxMoveCaretFlag flag, long modifiers)
{
	wxASSERT(m_input);
	bool extendleft = FALSE, extendright = FALSE;	
    OnMoveCaret(flag, modifiers, extendleft, extendright);

	switch (flag) {
	case wxMCF_LEFT:
		if (m_input->m_nCaretPos <= 0)
			return wxMCR_SETFOCUS_PREVIOUS;
		m_input->m_nCaretPos--;
		if (extendleft) { m_input->m_nSelStart--; OnSelChanged(); }
		if (extendright) { m_input->m_nSelEnd--; OnSelChanged(); }
		break;

	case wxMCF_RIGHT:
		if (m_input->m_nCaretPos >= (int)GetTextWithoutDelim().Len())
			return wxMCR_SETFOCUS_NEXT;
		m_input->m_nCaretPos++;
		if (extendleft) { m_input->m_nSelStart++; OnSelChanged(); }
		if (extendright) { m_input->m_nSelEnd++; OnSelChanged(); }
		break;

		// spans are always on a single line, so you cannot
		// move caret up/down remanining in the same span...
	case wxMCF_UP:
		return wxMCR_SETFOCUS_ABOVE;
	case wxMCF_DOWN:
		return wxMCR_SETFOCUS_BELOW;
	}

	return wxMCR_OKAY;
}

wxMoveCaretRes wxTextSpan::SetCaretPos(int pos, long modifiers)
{
    int newpos;
    switch (pos) {

        // start & start-of-line are coincident for us
    case wxTBCP_START:
    case wxTBCP_STARTLINE:
        newpos = 0;
        break;
        
        // end & end-of-line are coincident for us
	case wxTBCP_END:
    case wxTBCP_ENDLINE:
		newpos = GetTextWithoutDelim().Len();
		break;
		
    default:
	   newpos = pos;
    }
    
    // get some selection helper variables (BEFORE updating caret pos)
    wxMoveCaretFlag flag = (newpos > m_input->m_nCaretPos) ? wxMCF_RIGHT : wxMCF_LEFT;
	bool extendleft = FALSE, extendright = FALSE;	
    OnMoveCaret(flag, modifiers, extendleft, extendright);
    
    // finally, update the cursor position
    m_input->m_nCaretPos = newpos;
    
    if (extendleft)
		m_input->SetAsSelStartCurrentCaretPos();
    if (extendright)
		m_input->SetAsSelEndCurrentCaretPos();
    
    return wxMCR_OKAY;
}

wxMoveCaretRes wxTextSpan::MoveCaretUsingPoint(const wxPoint &p)
{
	int res = GetCaretPosUsingPoint(p);

	// caret position not found ?
	if (res == wxNOT_FOUND)
		return wxMCR_CANNOT_SETFOCUS;

	m_input->m_nCaretPos = res;
	return wxMCR_OKAY;
}

int wxTextSpan::GetCaretPosUsingPoint(const wxPoint &textboxpt) const
{
	wxPoint pt = ConvertToTextspanCoord(textboxpt);		// convert to span coords
	wxASSERT_MSG(pt.x >= 0 && pt.x <= m_size.x &&
				pt.y >= 0 && pt.y <= m_size.y, 
				wxT("invalid point"));

	if (IsDelimiter())
		return 0;

	// check if the given point is inside one of the character of our text
	wxArrayInt widths = GetPartialTextExtent();
	int offset = GetTextOffsetX(), lastx = offset;
	for (int i=0; i < (int)widths.GetCount(); i++) {
		if (pt.x >= lastx && pt.x <= offset+widths[i]) {

			// found the char which contains the caret !
			if (pt.x <= lastx+(widths[i]-lastx)/2)
				return i;
			return i+1;
		}

		lastx = offset+widths[i];
	}

	// check if the caret is in our unused borders...
	if (pt.x < offset) {
		
		// the cursor is on the LEFT side of the math content
		return 0;		// thus place it at the beginning of this span
		
	} else if (pt.x > offset+DoRecalcSize().x) {
		
		// the cursor is on the RIGHT side of the string
		return GetTextWithoutDelim().Len();	// thus place it at the end of this span
		
	} /*else if (pt.y < rc.y || pt.y > rc.y+rc.height) {
		
		// the cursor is above/below the math content: ajust
		// the point so that it falls inside the math content
		SetCursorPos(wxTBCP_START);
		pt.y = rc.y+m_mathContent.gui_GetHeight()/2;
		m_mathContent.gui_MoveCursorUsingPoint(dc, 
			wxPoint(pt.x-rc.x, pt.y-rc.y));
	}*/

	return wxNOT_FOUND;
}




// -----------------------------------------
// wxTEXTSPAN - export
// -----------------------------------------

wxString wxTextSpan::ExportRTF() const
{
	wxString ret;

	ret += GetTextWithoutDelim();
//	if (EndsWithDelimiter())
//		ret += wxT(" \\line \n");		// the space at the end is important !

	return ret;
}

wxXmlNode *wxTextSpan::ExportXHTML() const
{
	// we won't export our final newline ! the parent must check that !
	return new wxXmlNode(NULL, wxXML_TEXT_NODE, wxEmptyString, 
						GetTextWithoutDelim(), NULL, NULL);
}





// -----------------------------------------
// wxTEXTSPANARRAY - wxArray-like functions
// -----------------------------------------

void wxTextSpanArray::Grow(int increment)
{
	wxTextSpan **oldarr = m_arr;
	int oldsize = m_nSize;

	// create our new array
	m_nSize += increment;
	m_arr = new wxTextSpan*[m_nSize];
	
	// copy the span memory locations
	int start;
	if (oldarr) {
	
		for (int i=0; i < oldsize; i++)
			m_arr[i] = oldarr[i];
		start = oldsize;

		// delete the old array
		delete [] oldarr;

	} else {

		// this is our first grow... use empty spans for the entire array
		start = 0;
	}

	// init with freshly allocated spans the new portion of the array
	for (int i=start; i < m_nSize; i++)
		m_arr[i] = new wxTextSpan(*wxEmptyTextSpan);
}

void wxTextSpanArray::Alloc(int n)
{
	// create n empty spans and put them at the end of the array
	for (int i=0; i < (int)n; i++)
		Insert(*wxEmptyTextSpan,			// will be copied, not referenced
				GetCount()-1);
}

void wxTextSpanArray::Insert(const wxTextSpan &tocopy, int pos)
{
	wxASSERT_MSG(pos >= 0 && pos <= m_nUsed, wxT("Invalid index"));

	// make space
	if (m_nUsed+1 >= m_nSize)
		Grow(wxTSA_ALLOCATION_BLOCK);
	wxASSERT_MSG(m_nUsed+1 < m_nSize, wxT("Grow() didn't work ?"));	

	// shift the array
	wxTextSpan *newspan = m_arr[m_nUsed];
	for (int i=m_nUsed-1; i >= pos; i--)
		m_arr[i+1] = m_arr[i];

	// fill the hole we have left
	m_arr[pos] = newspan;
	newspan->Copy(tocopy);
	m_nUsed++;

#ifdef __WXDEBUG__
	s_nMaxUsed = wxMax(s_nMaxUsed, m_nUsed);
#endif
}

void wxTextSpanArray::RemoveAt(int pos)
{
	// shift the array
	wxTextSpan *toremove = m_arr[pos];
	for (int i=pos; i < m_nUsed-1; i++)
		m_arr[i] = m_arr[i+1];

	// reset the span we removed
	m_arr[m_nUsed-1] = toremove;
	m_arr[m_nUsed-1]->m_text = wxT("invalid");
	m_nUsed--;
}

wxTextSpan *wxTextSpanArray::GetNew()
{
	if (m_nUsed+1 >= m_nSize)
		Grow();
	wxASSERT_MSG(m_nUsed+1 < m_nSize, wxT("Grow() didn't work ?"));
	
	// the item already exist; the only thing is that it is uninitialized...
	m_nUsed++;
#ifdef __WXDEBUG__
	s_nMaxUsed = wxMax(s_nMaxUsed, m_nUsed);
#endif
	return Item(m_nUsed-1);
}

const wxTextSpan *wxTextSpanArray::Item(int n) const
{
	wxASSERT_MSG(n >= 0 && n < m_nUsed, wxT("Invalid index"));
	return m_arr[n];
}

wxTextSpan *wxTextSpanArray::Item(int n)
{
	wxASSERT_MSG(n >= 0 && n < m_nUsed, wxT("Invalid index"));
	return m_arr[n];
}

void wxTextSpanArray::Empty()
{
	// we simply set the number of used spans to zero...
	m_nUsed=0;
}

void wxTextSpanArray::Clear()
{
	for (int i=0; i<m_nSize; i++)
		delete m_arr[i];
	delete [] m_arr;

	m_nSize = m_nUsed = 0;
	m_arr = NULL;
}

int wxTextSpanArray::Index(const wxTextSpan *tofind) const
{
	for (int i=0; i < m_nUsed; i++)
		if (Item(i) == tofind)
			return i;
	return wxNOT_FOUND;
}




// -----------------------------------------
// wxTEXTSPANARRAY - specialized functions
// -----------------------------------------

void wxTextSpanArray::Defrag()
{
	// merge together the adjacent spans which have the same settings
	for (int i=0; i < GetCount()-1; i++) {
		wxTextSpan *s1 = Item(i);
		wxTextSpan *s2 = Item(i+1);

		// we can merge these two spans only if they have the same style
		// NB: here we do a pointer-comparison which is faster than a
		//     wxTextStyle comparison.
		if (s1->m_style == s2->m_style && 
			!s1->EndsWithDelimiter()) {  /* && !s2->IsDelimiter() no cond on s2 */

			// merge the spans
			s1->MergeWith(*s2);			
			RemoveAt(i+1);

			// check again the i-th span with the (i+1)-th span
			// since now the (i+1)-th span has been changed
			i--;
		}
	}
}

void wxTextSpanArray::RemoveStyles()
{
	for (int i=0; i < GetCount(); i++)
		Item(i)->m_style = NULL;
}

void wxTextSpanArray::RecalcSize()
{
	for (int i=0; i < GetCount(); i++)
		Item(i)->RecalcSize();
}

void wxTextSpanArray::Paint(wxDC &dc)
{
	for (int i=0; i < GetCount(); i++)		
		Item(i)->Paint(dc);
}

void wxTextSpanArray::Check(wxTextBox *owner)
{
#ifdef __WXDEBUG__
	wxRect parent(owner->GetRectOfContents());

	// this is because our span rects are in textbox coords not
	// in the canvas coord (i.e. a span placed at (0,0) is in
	// the topleft angle of the textbox which owns it not in
	// the topleft angle of the canvas which contains it)
	parent.SetTopLeft(wxPoint(0, 0));

	for (int i=0; i < GetCount(); i++) {

		// do some checks		
		wxTextSpan *child = Item(i);
		wxASSERT_MSG(child != NULL && child->IsValid(), wxT("invalid span !"));
		child->CheckCaretPos();
		
		bool inside = wxRectInside(parent, child->GetRect());
		wxASSERT_MSG(inside || !child->CanBeSplit(), 
							wxT("This span is outside the box !"));
	}
#endif
}

int wxTextSpanArray::GetTextLen() const
{
	// just get the lenght of all spans	
	int len = 0;
	for (int i=0; i < GetCount(); i++)
		len += Item(i)->m_text.Len();
	wxASSERT(len == (int)GetAllText().Len());
	return len;
}

int wxTextSpanArray::GetSpanFromCharIndex(int cp) const
{
	wxASSERT(cp >= 0 && cp <= GetTextLen());

	// FIXME: needs testing !
	int i, len = 0;
	for (i=0; i < GetCount() && cp >= len; i++)
		len += Item(i)->m_text.Len();		
	
	return i-1;
}

wxString wxTextSpanArray::GetAllText() const
{
	wxString tot;
	for (int i=0; i < GetCount(); i++)
		tot += Item(i)->m_text;
	return tot;
}

int wxTextSpanArray::GetCharIndexOffset(int span) const
{
	wxASSERT(span >= 0 && span <= GetCount());

	int offset = 0;
	for (int i=0; i < span; i++)	
		offset += Item(i)->m_text.Len();
	return offset;
}
int wxTextSpanArray::GetCaretEndPos() const
{
	// just get the offset of the (last+1)-th span 
	return GetCaretOffset(GetCount());
}

int wxTextSpanArray::GetSpanFromCaretPos(int cp) const
{
	wxASSERT(cp >= 0 && cp <= GetCaretEndPos());

	int i, offset = 0;
	for (i=0; i < GetCount(); i++) {
		offset += Item(i)->GetTextWithoutDelim().Len();
		//if (!Item(i)->IsDelimiter() && i < GetCount()-1) offset++;
		if (cp < offset)
			return i;
	}

	// for the last there is a little exception...
	if (cp == offset)
		return i-1;

	wxASSERT_MSG(0, wxT("This point should never be reached !"));
	return wxNOT_FOUND;
}

int wxTextSpanArray::GetCaretOffset(int span) const // TODO change in GetCaretPosFromSpan
{
	wxASSERT(span >= 0 && span <= GetCount());

	int offset = 0;

	// each span of n characters has n+1 possible positions for the caret.
	// E.g. the span "hello\n" has 6 characters but 7 possible caret positions:
	//
	// char index:    0   1   2   3   4   5
	//              .---.---.---.---.---.---.
	//              | H | e | l | l | o | \n|
	//              ^---^---^---^---^---^---^
	// caret pos:   0   1   2   3   4   5   6
	//
	// (each ^ symbol indicates a caret possible position)
	// 
	for (int i=0; i < span; i++) {		
		offset += Item(i)->GetTextWithoutDelim().Len();
	}
	return offset;
}

wxString wxTextSpanArray::ExportRTF() const
{
	wxString ret; 
	for (int i=0; i < GetCount(); i++)		
		ret += Item(i)->ExportRTF();
	return ret;
}


wxXmlNode *wxTextSpanArray::ExportXHTML() const
{
	if (GetCount() == 0)
		return NULL;

	wxXmlNode *html = wxCreateElemNode(wxT("html"));
	wxXmlNode *body = wxCreateElemNode(wxT("body"));
	wxXmlNode *root = wxCreateElemNode(wxT("p"));
	html->AddChild(body);
	body->AddChild(root);
	
	// this is the first node we're going to output:
	// we need to wrap it into the XHTML for its style.	
	wxXmlNode *firstcontainer = Item(0)->GetStyle()->ExportXHTML();
	root->AddChild(firstcontainer);
	firstcontainer->AddChild(Item(0)->ExportXHTML());
	if (Item(0)->EndsWithDelimiter())
		firstcontainer->AddChild(wxCreateElemNode(wxT("br")));

	// export all the remaining nodes...
	wxXmlNode *container = firstcontainer;
	for (int i=1; i < GetCount(); i++) {

		// if the style for this span is different from the style for the previous
		// span, then we need to create a new container
		if (Item(i)->GetStyle() != Item(i-1)->GetStyle()) {
			
			//container = Item(i)->GetStyle()->ExportXHTMLDiffFrom(previous->GetStyle());
			container = Item(i)->GetStyle()->ExportXHTML();			
			root->AddChild(container);
		}

		do {
			// add to the current container the XHTML for the i-th span
			container->AddChild(Item(i)->ExportXHTML());
			if (Item(i)->EndsWithDelimiter())
				container->AddChild(wxCreateElemNode(wxT("br")));
			i++;

		} while (i < GetCount() && Item(i)->GetStyle() == Item(i-1)->GetStyle());

		// the i-th span has not been exported yet... avoid that the 
		// for loop increment make us lost a span...
		i--;
	}

	return html;
}

wxXmlDocument wxTextSpanArray::ExportXHTMLDoc() const
{
	wxXmlDocument doc;
	doc.SetRoot(ExportXHTML());

	return doc;
}

