/////////////////////////////////////////////////////////////////////////////
// Name:        textbox.cpp
// Purpose:     wxTextBoxLayoutStatus, wxTextBox
// Author:      Francesco Montorsi
// Created:     2005/8/16
// RCS-ID:      $Id: textbox.cpp,v 1.9 2005-10-02 19:43:46 frm Exp $
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifdef __GNUG__
#pragma implementation "textbox.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
	#include <wx/settings.h>
	#include <wx/dcclient.h>
	#include <wx/menu.h>
	#include <wx/msgdlg.h>
	#include <wx/log.h>
#endif

#include "wx/textbox.h"
#include <wx/tokenzr.h>
#include <wx/caret.h>
#include <wx/clipbrd.h>
#include <wx/sstream.h>

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(wxRectArray);

BEGIN_EVENT_TABLE(wxTextBox, wxWindow)
    EVT_PAINT(wxTextBox::OnPaint)

	// these events force a re-layouting of the box
    EVT_SIZE(wxTextBox::OnSize)
    EVT_MOVE(wxTextBox::OnMove)
	EVT_SIBLING_CHANGED(wxID_ANY, wxTextBox::OnSiblingChange)

	// for input
    EVT_CHAR(wxTextBox::OnChar)
	EVT_KEY_DOWN(wxTextBox::OnChar) 

	// mouse events
	EVT_RIGHT_UP(wxTextBox::OnRUp)				// used for context menu
	EVT_LEFT_DOWN(wxTextBox::OnLDown)			// used to start selections
	EVT_LEFT_UP(wxTextBox::OnLUp)				// used to end selections
	EVT_MOTION(wxTextBox::OnMouseMove)			// used to select

	// context menu events
	EVT_MENU_RANGE(wxTBCM_BASEID,
					wxTBCM_FINALID,
					wxTextBox::OnCtxMenuItem)


	// required when we are contained into a wxResizeableParentControl
    EVT_SET_FOCUS(wxTextBox::OnSetFocus)
    EVT_KILL_FOCUS(wxTextBox::OnKillFocus)

END_EVENT_TABLE()





// ---------------------
// wxTEXTBOXINPUTSTATUS
// ---------------------

void wxTextBoxInputStatus::SetAsSelStartCurrentCaretPos()
{
	m_nSelStart = m_nCaretPos;
	m_nSpanWithSelStart = m_nSpanWithCaret;
}

void wxTextBoxInputStatus::SetAsSelEndCurrentCaretPos()
{
	m_nSelEnd = m_nCaretPos;
	m_nSpanWithSelEnd = m_nSpanWithCaret;
}




// -----------------
// wxTEXTBOX
// -----------------

wxTextBox::wxTextBox(wxWindow *parent, int id, const wxPoint &pos,
					const wxSize &size, long style,
					const wxString &name)
			  : wxWindow(parent, id, pos, size, style, name), m_input(m_spans)
{
	// the height will be changed by SetCaretPos...
	SetCaret(new wxCaret(this, wxTB_CARET_WIDTH, 10));
	GetCaret()->Show();

	m_bInputEnabled = TRUE;
	m_bSelectionEnabled = TRUE;
	m_bContextMenuEnabled = TRUE;

	m_bSelecting = FALSE;

	// we want to emulate textctrl
	SetCursor(wxCursor(wxCURSOR_IBEAM));
}

wxTextSpanTruncFlag wxTextBox::TruncateToSize(wxTextBoxLayoutStatus &p)
{
	wxTextSpan next;

	// check if the given span can be truncated with the current size...
	wxTextSpanTruncFlag f = p.GetSpan()->TruncateToSize(next);

	// ...and produce a valid complementar span
	if (f != wxTSTF_TRUNCATED)
		return f;

	// insert this new span *after* the given one
	m_spans.Insert(next, p.GetSpanIndex()+1);
	return wxTSTF_TRUNCATED;
}

bool wxTextBox::AdjustSpanPosition(wxTextBoxLayoutStatus &p)
{
	wxTextSpan *span = p.GetSpan();
	bool restart, changed = FALSE;

	do {
		restart = FALSE;

		for (int i=0; i < (int)m_rc.GetCount() && !restart; i++) {			

			wxPoint topleft(span->m_pos), 
					bottomleft(span->m_pos.x, span->m_pos.y+span->m_size.y);
			if (m_rc[i].Inside(topleft) || m_rc[i].Inside(bottomleft)) {

				// ok, the current pos is not good since a box is overlapping
				// with this span... so move the span to the end of this box
				span->m_pos.x += m_rc[i].GetWidth();
				changed = TRUE;

				// and recheck this new pos also against the positions
				// of the previously-checked boxes
				restart = TRUE;
   			}
  		}
	} while (restart);

	// did we change the position ?
	return changed;
}

bool wxTextBox::AdjustSpanSize(wxTextBoxLayoutStatus &sp)
{
	wxTextSpan *p = sp.GetSpan();
	bool restart, changed = FALSE;

	do {
		restart = FALSE;
		
		wxRect span(p->GetRect());
		for (int i=0; i < (int)m_rc.GetCount() && !restart; i++) {

			// is this size okay ?
			if (m_rc[i].Intersects(span)) {

				// no, it's not since it overlaps with another box...
				// try to set this span so that it ends *before* the
				// beginning of this inteersecting box
				p->m_size.x = m_rc[i].x - p->m_pos.x;

				// we adapted the size of this span... set the flag
				changed = TRUE;

				// check again this size against other boxes previously-checked
				restart = TRUE;
			}
  		}
	
	} while (restart);	

	// does TruncateToSize needs to be called by our caller ?
	return changed;
}

bool wxTextBox::SetToMaxSize(wxTextBoxLayoutStatus &p)
{
	// set the span to take the max size
	//int oldw = p.GetSpan()->m_size.x;
	p.GetSpan()->m_size.x = GetWidth() - p.GetSpan()->m_pos.x;
	AdjustSpanSize(p);
	
	// AdjustSpanSize maye have not called TruncateToSize and since
	// we set the span size as hoc to make it fill the textbox
	// probably the span needs to be truncated.
	// Since both in the case AdjustSpanSize asks to call TruncateToSize
	// (returning TRUE) and in the case it does not (returning FALSE),
	// a call to TruncateToSize is required in order to be sure that the
	// width we set is okay, we *always* return TRUE here
	return TRUE;
}

bool wxTextBox::SetToMinSize(wxTextBoxLayoutStatus &p)
{
	p.GetSpan()->RecalcSize();
	
	// Differently from #SetToMaxSize() here we won't return always TRUE.
	// forcing the caller to call
	// TruncateToSize; infact, we set the size of the span to the
	// minimal span width and thus we are sure that it does not need
	// to be truncated with that size... it could need to be truncated
	// only if #AdjustSpanSize requires it
	return AdjustSpanSize(p);
}

void wxTextBox::MoveOnNewLine(wxTextBoxLayoutStatus &curr)
{
	// set our current position on a new line
	wxASSERT(curr.GetCurrHeight() > 0);
	curr.SetCurrX(0);
	curr.SetCurrY(curr.GetCurrPos().y+curr.GetCurrHeight());
	curr.SetCurrHeight(0);
	curr.SetNewLineFlag();
}

bool wxTextBox::IsLastOfTheLine(wxTextBoxLayoutStatus &p) const
{
	if (p.GetSpan()->IsLastOfTheLine())
		return TRUE;

	// there is also another case which makes this span the last of the line:
	// it's the last span of the array.
	// NOTE: this makes it easier the task of GetSpanWithCaretUsingPoint since
	//       with this simple rule we can be sure that all the usable space of
	//       this textbox is taken from a span, except for the space after the
	//       last span.
	//       Without this rule, GetSpanWithCaretUsingPoint should also check
	//       if the given point is at the right/left of the last span since the
	//       last span would not take all the available space.
	if (p.GetSpanIndex() == m_spans.GetCount()-1)
		return TRUE;

	return FALSE;
}

bool wxTextBox::Layout()
{
	wxLogDebug(wxT("wxTextBox::Layout - starting"));

	// some preparatory operation first...
	m_spans.Defrag();
	//m_spans.RecalcSize();		// NOT REQUIRED !
	m_lines.Empty();
	ClipOtherBoxes();	// update our clipped box array
	
	// begin the layout cycle
	bool restart;
	wxTextBoxLayoutStatus status;
	for (int i=0; i<(int)m_spans.GetCount(); i++) {
	
		status.SetSpan(m_spans.Item(i));
		status.SetSpanIdx(i);
		wxTextSpan *p = status.GetSpan();

		// is this the first span of the line ?
		if (status.IsBeginningNewLine()) {
			m_lines.Add(status.GetSpanIndex());
			status.ClearNewLineFlag();
		}

		do {
			do {
				restart = FALSE;

				// IMPORTANT: this needs to be in the innermost "do" cycle
				// since each call to #MoveOnNewLine would reset this var
				// to zero and we must avoid it
				status.SetCurrHeight(wxMax(status.GetCurrHeight(), p->m_size.y));
				
				// begin to place this span on the current "line": i.e. set its "y"
				switch (p->GetVertAlignment()) {
				case wxTSVA_TOP:
					p->m_pos.y = status.GetCurrPos().y;
					break;

				case wxTSVA_MIDDLE:
					p->m_pos.y = status.GetCurrPos().y + (status.GetCurrHeight() - p->m_size.y) / 2;
					break;

				case wxTSVA_BOTTOM:
					p->m_pos.y = status.GetCurrPos().y + status.GetCurrHeight() - p->m_size.y;
					break;
				}
				
				// try to set this span in the leftmost position available on this line
				p->m_pos.x = status.GetCurrPos().x;
				AdjustSpanPosition(status);

				// check if position we found is outside this textbox...
				if (p->m_pos.x >= GetWidth()) {

					// need to move on a new line & restart the layout of this span agaistatus...
					MoveOnNewLine(status);
					restart = TRUE;
				}
				
			} while (restart);
		
			// now adjust the size
			bool needtrunc;
			if (IsLastOfTheLine(status))
				needtrunc = SetToMaxSize(status);		// expand it to take all the size of this box
			else
				needtrunc = SetToMinSize(status);		// keep it min-sized so that other spans can be placed in this same line

			if (needtrunc) {

				if (TruncateToSize(status) == wxTSTF_TOOSMALL) {

					// try to place this span more to the right
					status.SetCurrX(p->m_pos.x + p->m_size.x);
					restart = TRUE;
					continue;
				}
			}
					
			// now we are sure that		
			// this span does not intersect any extern box with the current
			// position & size... now check the size against this box's bounds
			if (p->m_pos.x+p->m_size.x > GetWidth()) {		// not >= !
							
				// need to split this span because it would exceed our
				// rightmost bound
				int oldw = p->m_size.x;
				p->m_size.x = GetWidth() - p->m_pos.x;
				
				if (TruncateToSize(status) == wxTSTF_TOOSMALL) {

					// cannot make this span even smaller... start a new line
					MoveOnNewLine(status);
					p->m_size.x = oldw;
					restart = TRUE;
				}
			}
			
		} while (restart);
				
		status.SetCurrX(p->m_pos.x + p->m_size.x);
		wxASSERT_MSG(status.GetCurrPos().x <= GetWidth(), wxT("this span exceed this box"));

		bool enoughspace = (GetWidth() - status.GetCurrPos().x) > p->GetMinCharWidth();
		if (IsLastOfTheLine(status) || !enoughspace)
			MoveOnNewLine(status);
 	}

	wxASSERT_MSG(m_lines[0] == 0, wxT("The first span is not the leader of the first line ?"));

	// update our virtual height
	SetVirtualSize(GetWidth(), 
		status.GetCurrPos().y);		// no need to add the height of the last row of spans;
									// this was already done by MoveOnNewLine()

	// update also our caret position
	UpdateSpanWithCaret();		// we have changed also the number/position/lenght of spans
	UpdateCaret();

	wxLogDebug(wxT("wxTextBox::Layout - ended; %d lines and %d spans in total"),
				m_lines.GetCount(), m_spans.GetCount());
	return TRUE;
}

wxTextStyle &wxTextBox::AddStyle(const wxTextStyle &toadd)
{
	if (!HasStyle(toadd.GetName()))
		m_styles.Add(toadd);
	return GetStyle(toadd.GetName());
}

void wxTextBox::AppendText(const wxString &txt, const wxTextStyle &attr)
{
	// add the given style to this box style list (if not present)
	// and/or retrieve it
	wxTextStyle &style = AddStyle(attr);

	// do we need to break the string ?
	wxString toparse(txt);	
	while (toparse.Len() > 0)
	{
		wxString token = toparse.BeforeFirst(wxTS_DELIMITER);
		toparse = toparse.Right(toparse.Len() - token.Len());

		// eventually add the delimiter to this span
		if (toparse[0] == wxTS_DELIMITER) {
            token += wxTS_DELIMITER;
			toparse.Remove(0, 1);
		}
		
		// process token here
		if (!token.IsEmpty())
			m_spans.GetNew()->Set(token, &style, &m_input);
	}

	// we won't call Layout() here since the user could use
	// AppendText() more times in sequence before showing
	// the text box to the user; thus to avoid flickering
	// it's task of the user of wxTextBox call Layout() after
	// this function
}

void wxTextBox::SetText(const wxString &txt, const wxTextStyle &attr)
{
	m_spans.Empty();
	AppendText(txt, attr);
}

void wxTextBox::ClipOtherBoxes()
{
	wxPoint ourpt(GetParent()->GetPosition());
	wxRect ourrc(this->GetRect());
	ourrc.Offset(ourpt);

	// fist, clean old array
	m_rc.Empty();

	wxResizeableControlList &lst = GetCanvas()->GetControlList();
	wxResizeableControlList::Node *node = lst.GetFirst();
	while (node)
	{
		wxResizeableControl *box = node->GetData();
		if (box == GetParent()) {
			node = node->GetNext();
			continue;		// skip ourselves !
		}
		
		wxRect rc(box->GetRect());
		if (rc.Intersects(ourrc)) {

			// convert from canvas coord to this textbox coords...
			rc.SetX(rc.GetX() - ourpt.x);
			rc.SetY(rc.GetY() - ourpt.y);

			// normalize them
			if (rc.GetX() < 0) {
				rc.SetWidth(rc.GetWidth() + rc.GetX());
				rc.SetX(0);
			}

			if (rc.GetY() < 0) {
				rc.SetHeight(rc.GetHeight() + rc.GetY());
				rc.SetY(0);
			}

			// these should always be true if we are intersecting the
			// rect of this window
			wxASSERT(rc.GetHeight() > 0 && rc.GetWidth() > 0);
			m_rc.Add(rc);
		}

		node = node->GetNext();
	}
}

void wxTextBox::InitDefaultStyles()
{
	// like HTML <h1>
	wxTextStyle h1(wxTextAttr(*wxBLACK, wxNullColour, 
					wxFont(15, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_ITALIC, 
						wxFONTWEIGHT_BOLD, FALSE, wxEmptyString),
						wxTEXT_ALIGNMENT_CENTER), wxT("h1"));
	
	// like HTML <h2> 
	wxTextStyle h2(wxTextAttr(*wxBLACK, wxNullColour, 
					wxFont(13, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_ITALIC, 
						wxFONTWEIGHT_BOLD, FALSE, wxEmptyString),
						wxTEXT_ALIGNMENT_CENTER), wxT("h2"));
	
	// like HTML <p> 
	wxTextStyle normal(wxTextAttr(*wxBLACK, wxNullColour, 
					wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT)), wxT("normal"));

	// add the just-created styles to our array
	AddStyle(h1);
	AddStyle(h2);
	AddStyle(normal);
}

const wxTextStyle &wxTextBox::GetConstStyle(const wxString &name) const
{
	for (int i=0; i < (int)m_styles.GetCount(); i++)
		if (m_styles.Item(i).GetName() == name)
			return m_styles.Item(i);
	return *wxNullTextStyle;
}

int wxTextBox::GetLineMaxHeight(int linenum) const
{
	int idx = m_lines[linenum];

	if (linenum == (int)m_lines.GetCount()-1)
		return m_spans[idx]->GetAveCharHeight();
	
	int idxfollowing = m_lines[linenum+1];
	return m_spans[idxfollowing]->m_pos.y - m_spans[idx]->m_pos.y;
}





// -------------------
// wxTEXTBOX - events
// -------------------

void wxTextBox::OnPaint(wxPaintEvent &ev)
{
	wxPaintDC dc(this);

	m_spans.Check(this);
	m_spans.Paint(dc);

	ev.Skip();
}

void wxTextBox::OnSize(wxSizeEvent &ev)
{
	wxLogDebug(wxT("wxTextBox::OnSize"));

	Layout();
	Refresh();
	ev.Skip();
}

void wxTextBox::OnMove(wxMoveEvent &ev)
{
	wxLogDebug(wxT("wxTextBox::OnMove"));

	Layout();
	Refresh();
	ev.Skip();
}

void wxTextBox::OnSiblingChange(wxCommandEvent &ev)
{
	wxLogDebug(wxT("wxTextBox::OnSiblingChange"));

	Layout();
	Refresh();
	ev.Skip();
}

void wxTextBox::OnKillFocus(wxFocusEvent &event)
{
	wxCommandEvent notification(wxEVT_COMMAND_HIDE_SIZERS);
	if (GetParent())
	    GetParent()->ProcessEvent(notification);
	event.Skip();
}

void wxTextBox::OnSetFocus(wxFocusEvent &event)
{
	wxCommandEvent notification(wxEVT_COMMAND_SHOW_SIZERS);
	if (GetParent())
	    GetParent()->ProcessEvent(notification);
	event.Skip();
}

void wxTextBox::OnChar(wxKeyEvent &ke)
{	
	int r, mod=wxTBMCM_NOMODIFIERS;

	// block here the ALT+xxxx keypresses.
	if (ke.AltDown()) {
		ke.Skip();		// otherwise menu shortcut won't work
		return;
	}

	// block here some other special keypresses which we are sure are not used
	// (like the Windows' START keypress)
	//
	// VERY IMPORTANT: we cannot simply discard *all* special keys (like F1, F2...)
	//                 because some of them could have been set by the user as
	//                 creation hooks for something
	// 
	if (ke.GetKeyCode() == WXK_START
#ifdef __WXMSW__
		|| ke.GetKeyCode() == 397	// on winXP at least, start button has keycode = 397
#endif		
		) {
		ke.Skip();
		return;
	}

	// is input enabled ?
	if (!m_bInputEnabled) {
		wxBell();			// cannot accept any input... sorry
		return;
	}

	// are we moving the cursor and selecting ?
	if (ke.ShiftDown())	
		mod = wxTBMCM_EXTEND_SELECTION;	// maybe: pass to the gui_MoveCaret a "modifier"


#define HandleMoveCaretFlag(flag)		\
	if (flag != wxMCR_OKAY) wxBell();

	switch (ke.m_keyCode) {

		// cursor movement keys handlers
	case WXK_LEFT:
		r = MoveCaret(wxMCF_LEFT, mod);
		HandleMoveCaretFlag(r);
		break;
	case WXK_RIGHT:
		r = MoveCaret(wxMCF_RIGHT, mod);
		HandleMoveCaretFlag(r);
		break;
	case WXK_UP:
		r = MoveCaret(wxMCF_UP, mod);
		HandleMoveCaretFlag(r);
		break;
	case WXK_DOWN:
		r = MoveCaret(wxMCF_DOWN, mod);
		HandleMoveCaretFlag(r);
		break;
	case WXK_HOME:
        if (ke.ControlDown())
    		r = SetCaretPos(wxTBCP_START, mod);
        else
            r = SetCaretPos(wxTBCP_STARTLINE, mod);
        HandleMoveCaretFlag(r);
		break;
	case WXK_END:
        if (ke.ControlDown())
    		r = SetCaretPos(wxTBCP_END, mod);
        else
            r = SetCaretPos(wxTBCP_ENDLINE, mod);
		HandleMoveCaretFlag(r);
		break;

	default:
		
		if (ke.GetEventType() != wxEVT_CHAR) {

			ke.Skip();		// wait this event as wxEVT_CHAR instead of processing
							// it now as wxEVT_KEY_DOWN
			return;
		}

		wxLogDebug(wxT("wxTextBox::OnChar - handling [%d]"), ke.GetKeyCode());
		
		// one of ()0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ
		// or abcdefghijklmnopqrstuvwxyz key was pressed
		OnInput(ke);
		//Refresh();	only the modified span is redrawn by OnInput
	}	
}

void wxTextBox::OnLDown(wxMouseEvent &me)
{
	// if the selection system is not enabled, then this event handler's
	// code should not be executed since it only handles the SELECTION
	if (!m_bSelectionEnabled)
		return;
	
	// the first thing to do, is to delete the old selection...
	bool needsupdate = this->HasSelection();
	DeSelect();
	if (needsupdate) 
		Refresh();

	// this is used by isSelectionFromLeftToRight
	m_ptOriginalSelStart = me.GetPosition()+GetPosition();

	// capture the mouse (so, if it exits the window
	// the OnLUP function will still be called)
	CaptureMouse();
	m_bSelecting = TRUE;

	// hide caret
	UpdateCaret();
}

void wxTextBox::OnLUp(wxMouseEvent &me)
{
	// VERY IMPORTANT: without this line everything works as usual
	// when mgMathBox is the only child of a wxFrame or a wxPanel;
	// try to remove this line and you won't be able to set the
	// focus on a mgSymPropPanel !!
	SetFocus();

	// when this event handler is called, then it is because
	// the user should be selecting something...
	// check if the drag point was in this same mgMathBox
	bool havecapture = GetCapture() == this;
	if (!havecapture && m_bSelectionEnabled)
		return;
	
	// user has just finished to create a selection rectangle...
	// all the text must have been already selected by
	// OnMouseMove... just reset selection system
	//ResetSelectionRect();
	
	// free the cursor
	if (havecapture) ReleaseMouse();
	m_bSelecting = FALSE;
	
	// move the cursor inside the math data
	wxClientDC dc(this);
	wxPoint pt = me.GetPosition(); // returns a point in TEXTBOX coords !
	pt += GetPosition();			// this converts it to SCREEN coords

	// NOTE: we won't use here the IsInsideContents function since it would
	//       discard those clicks that are *below* the last span but still
	//       inside the textbox (and we want to allow them)
	if (!GetRect().Inside(pt)) {
		UpdateCaret();
		return;
	}
	
	// user released button over the textbox; move the cursor
	// to the nearest available position
#ifdef __WXDEBUG__
	bool oldselstate = HasSelection();
#endif
	MoveCaretUsingPoint(pt);
#ifdef __WXDEBUG__
	if (oldselstate)
		wxASSERT_MSG(HasSelection(), wxT("MoveCaretUsingPoint removed the selection ?"));
#endif
	
	// restore caret & cursor
	UpdateCaret();
}

void wxTextBox::OnRUp(wxMouseEvent &me)
{
	// this event handler is used only for context menu handling
	if (!m_bContextMenuEnabled)
		return;
	
	// discard boring events
	wxPoint tmp = me.GetPosition() + GetPosition();

	// NOTE: we won't use here the IsInsideContents function since it would
	//       discard those clicks that are *below* the last span but still
	//       inside the textbox (and we want to allow them)
	if (!GetRect().Inside(tmp))
		return;	

	// check if the user right-clicked on the selection or not	
	bool onsel = IsInsideSelection(tmp) && m_bSelectionEnabled;
	if (!onsel)	{
		DeSelect();
		Refresh();
	}

	wxMenu m;		// create the context menu
	GetContextMenu(m, onsel);

	// pop up the context menu
	PopupMenu(&m, me.GetPosition());

	// for debugging
	wxLogDebug(wxT("wxTextBox::OnRUp() - popped the context menu"));
}

void wxTextBox::OnCtxMenuItem(wxCommandEvent &ce)
{
	// exec the command chosen by the user
	switch (ce.GetId()) {
	case wxTBCM_CUT:
		Cut();
		break;

	case wxTBCM_COPY:
		Copy();
		break;

	case wxTBCM_PASTE:
		Paste();
		break;

	case wxTBCM_SELECTALL:
		SelectAll();
		break;

	default:
		break;
	}

	UpdateCaret();
	Refresh();

	// for debugging
	wxLogDebug(wxT("wxTextBox::OnCtxMenuItem - context menu click handled"));
}

void wxTextBox::Copy()
{
/*	wxString str;
	if (HasSelection())
		str = ExportSelectionToRTF();
	else
		str = ExportRTF();*/


	wxXmlDocument doc(m_spans.ExportXHTMLDoc());
	wxStringOutputStream str;
	doc.Save(str);

	// export to clipboard
	if (wxTheClipboard->Open())
	{
		// This data objects are held by the clipboard,
		// so do not delete them in the app.
		wxString mystr(str.GetString());
		wxTheClipboard->SetData( new wxTextDataObject(mystr) );
		wxTheClipboard->Close();
	}
		//pdo->SetData(str.Len()*sizeof(wxChar) + 1, str.c_str());

/*
	str = wxT("{\\rtf1\\ansi ciao }");
	if (wxTheClipboard->Open())
	{
		// create an RTF data object
		wxRTFDataObject *pdo = new wxRTFDataObject();
		
		// copy RTF string as UTF8
		wxCharBuffer cb = str.mb_str(wxConvUTF8);
		const char *s = (const char *)cb;
		pdo->SetData(wxT("Rich Text Format"),
					strlen(s)+1, s);	// the +1 is used to force copy of the \0 character

		// tell clipboard about our RTF
		wxTheClipboard->SetData( pdo );
		wxTheClipboard->Close();
	}	

	// Read some text
	if (wxTheClipboard->Open())
	{
		if (wxTheClipboard->IsSupported( wxT("Rich Text Format") ))
		{
			wxRTFDataObject data;
			wxTheClipboard->GetData( data );
			wxMessageBox( wxString(data.m_buf, wxConvUTF8) );
		}  
		wxTheClipboard->Close();
	}


  /*
	str = wxT("{\\rtf1\\ansi\\pard ciao\\par }");
		wxCharBuffer cb = str.mb_str(wxConvUTF8);
		const char *s = (const char *)cb;

                int width = strlen(s) + 1;
                int height = 1;
                DWORD l = (width * height);
                HANDLE hGlobalMemory = GlobalAlloc(GHND, l);
                if ( hGlobalMemory )
                {
                    LPSTR lpGlobalMemory = (LPSTR)GlobalLock(hGlobalMemory);

                    memcpy(lpGlobalMemory, s, l);

                    GlobalUnlock(hGlobalMemory);
                }

	
				OpenClipboard(NULL);
                SetClipboardData(::RegisterClipboardFormat(wxT("Rich Text Format")), hGlobalMemory);
				CloseClipboard();
*/
}

void wxTextBox::OnMouseMove(wxMouseEvent &me)
{
	if (GetCapture() != this)
		return;

	wxASSERT(m_bSelecting);

	// we are not just moving the cursor into the window:
	// we are dragging a selection rectangle...

	//wxClientDC dc(this);
	wxPoint startsel = m_ptOriginalSelStart,
			endsel = me.GetPosition()+GetPosition();	// don't use GetLogicalPosition
	startsel = AdjustPointForCaretMove(startsel);
	endsel = AdjustPointForCaretMove(endsel);
	
#define	wxSWAP(t,a,b)	{ t x; x = (a); (a) = (b); (b) = x; }

	// the START point must be the one with the lower y
	if (startsel.y > endsel.y) {
		wxSWAP(wxPoint, startsel, endsel);
/*
		// this flag tells OnLUp to move the caret in the position which
		// 
		m_bMoveAtSelStart = TRUE;*/

	} else {


	}
	
	// do not allow coincident start & end point
	if (startsel == endsel)
		return;
	
	// clip start & end point
	//if (startsel.x < 
	{
		SelectUsingRect(startsel, endsel);
	
	//Refresh();
	}
}



// -----------------------------
// wxTEXTBOX - selection
// -----------------------------

void wxTextBox::SetSelection(int from, int to)
{
	if (from == -1)
		from = 0;
	if (to == -1)
		to = m_spans.GetTextLen();

	m_input.SetSelStartFromCharIdx(from);
	m_input.SetSelEndFromCharIdx(to);
}

bool wxTextBoxInputStatus::IsAllSelected(const wxTextSpan *p) const
{
	int idx = m_spans.Index(p);

	if (idx > m_nSpanWithSelStart && idx < m_nSpanWithSelEnd)
		return TRUE;
	return FALSE;
}

bool wxTextBox::IsInsideSelection(const wxPoint &pt) const
{
	int sp = GetSpanWithCaretUsingPoint(pt);

	// the span with the current point is inside the selected spans
	if (m_input.m_nSpanWithSelStart < sp &&
		m_input.m_nSpanWithSelEnd > sp)
		return TRUE;

	// handle the case in which the sp span is the span with
	// sel start or the span with sel end
	if (m_input.m_nSpanWithSelStart == sp ||
		m_input.m_nSpanWithSelEnd == sp) {

		bool inside = TRUE;
		wxPoint pos = ConvertToTextboxCoord(pt);

		if (m_input.m_nSpanWithSelStart == sp) {
			int cp = m_input.GetSpanWithSelStart()->GetCaretPosUsingPoint(pos);
			inside &= (m_input.m_nSelStart < cp);
		}
		if (m_input.m_nSpanWithSelEnd == sp) {
			int cp = m_input.GetSpanWithSelEnd()->GetCaretPosUsingPoint(pos);
			inside &= (m_input.m_nSelEnd > cp);
		}

		return inside;
	}

	return FALSE;
}

void wxTextBox::GetContextMenu(wxMenu &m, bool onselection)
{
	m.Append(wxTBCM_CUT, wxT("Cut"), 
			wxT("Cuts the selection and moves it to the clipboard"));
	m.Append(wxTBCM_COPY, wxT("Copy"), 
			wxT("Copies the selection to the clipboard"));
	
	if (!onselection) {

		// disable cut & copy
		m.Enable(wxTBCM_CUT, FALSE);
		//m.Enable(wxTBCM_COPY, FALSE);
	}

	m.Append(wxTBCM_PASTE, wxT("Paste"), 
		wxT("Inserts the clipboard contents at the insertion point"));
	m.AppendSeparator();

	m.Append(wxTBCM_SELECTALL, wxT("Select all"),
		wxT("Selects all the text"));
}




// -----------------------------
// wxTEXTBOX - input & caret
// -----------------------------

void wxTextBox::UpdateCaret()
{
	wxASSERT_MSG(GetCaret(), wxT("caret should have been already created"));

	// do we have to update its show status ?
	if ((m_bInputEnabled && !m_bSelecting) && !GetCaret()->IsVisible()) {
		wxLogDebug(wxT("wxTextBox::UpdateCaret - showing the caret"));
		GetCaret()->Show();
	}
	if ((!m_bInputEnabled || m_bSelecting) && GetCaret()->IsVisible()) {
		wxLogDebug(wxT("wxTextBox::UpdateCaret - hiding the caret"));
		GetCaret()->Hide();
	}

	// move the caret
	//int spanoffset = m_spans.GetCaretOffset(m_nSpanWithCaret);
	wxPoint pos = GetSpanWithCaret()->GetCaretOffset();
	GetCaret()->Move(pos.x, pos.y);

	// and update its size
	GetCaret()->SetSize(wxTB_CARET_WIDTH, 
			GetSpanWithCaret()->m_style->GetAveCharHeight());
}

void wxTextBox::UpdateSpanWithCaret()
{
	// update the cached index of the span with the caret inside
	//m_nSpanWithCaret = m_spans.GetSpanFromCaretPos(m_nCaretPos);
}

wxMoveCaretRes wxTextBox::SetCaretPos(int pos, long modifiers)
{
	bool extendselection = (modifiers & wxTBMCM_EXTEND_SELECTION);
	bool needsupdate = HasSelection() || extendselection;
	
	// deselect the current text ?
    OnUserCaretMove(modifiers);

    switch (pos) {

        // this operation does not change the current span with caret
    case wxTBCP_STARTLINE:
		GetSpanWithCaret()->SetCaretPos(wxTBCP_START, modifiers);
        break;
    case wxTBCP_ENDLINE:
		GetSpanWithCaret()->SetCaretPos(wxTBCP_END, modifiers);
        break;
        
        // these affect also the current span with caret        
	case wxTBCP_START:
		m_input.m_nSpanWithCaret = 0;
		GetSpanWithCaret()->SetCaretPos(wxTBCP_START, modifiers);
		break;
	case wxTBCP_END:
		m_input.m_nSpanWithCaret = m_spans.GetCount()-1;
		GetSpanWithCaret()->SetCaretPos(wxTBCP_END, modifiers);
		break;

	default:

		// update caret position
		m_input.SetCaretPosFromCharIdx(pos);
	}

	// update our cached index
	UpdateSpanWithCaret();

	// and caret pos
	UpdateCaret();
	if (needsupdate)
	   Refresh();
	
	return wxMCR_OKAY;
}

void wxTextBox::OnInputMergeSpan(int idx1, int idx2)
{
	wxASSERT(idx1 >= 0 && idx2 < m_spans.GetCount());
	wxASSERT(idx1 < idx2);

	// we have the following different possibilities:
	// 1) the spans have different alignment
	// 2) the first span ends with a delimiter
	// 3) both
	if (m_spans[idx1]->GetHorizAlignment() != m_spans[idx2]->GetHorizAlignment()) {

		// case #1: adapt the alignment of the second span to the alignment
		//          of the first span
		wxTextStyle s(*m_spans[idx2]->GetStyle());
		s.SetHorizAlignment(m_spans[idx1]->GetHorizAlignment());

		s.SetNewNameFromCurrentName();
		wxTextStyle &newstyle = AddStyle(s);
		m_spans[idx2]->SetStyle(&newstyle);
	}

	if (m_spans[idx1]->EndsWithDelimiter()) {

		// case #2: 
		m_spans[idx1]->m_text.RemoveLast();//RemoveDelimiter();
	}
}

wxInputRes wxTextBox::OnInput(const wxKeyEvent &key)
{
	bool needslayout = FALSE, needsrefresh = FALSE;

	// if there was a selection, remove it
	// TODO: overwrite is better
	if (HasSelection()) {
		DeSelect();
		needsrefresh = TRUE;
	}

	// ask the focused span to handle this input
	wxTextSpan *newspan = NULL;
	wxInputRes r = GetSpanWithCaret()->Input(key.GetKeyCode(), &newspan);

	// did it ask to create a new span ?
	switch (r) {
	case wxIR_OKAY:
		break;			// this is easy to handle :-)

	case wxIR_RELAYOUT:
		needslayout = TRUE;
		break;

	case wxIR_INSERT_NEWSPAN_AFTER_THIS:
		wxASSERT(newspan != NULL);
		m_spans.Insert(*newspan, m_input.m_nSpanWithCaret);
		delete newspan;
		needslayout = TRUE;
		break;

	case wxIR_DELETE_THIS:
		// check if we can remove this span
		if (m_spans.GetCount() == 1)
			wxBell();		// we can't be left without any span !
		else {

			// remove this span from the array
			m_spans.RemoveAt(m_input.m_nSpanWithCaret);

			// move caret
			if (key.GetKeyCode() == WXK_BACK && m_input.m_nSpanWithCaret > 0) {

				// move caret on previous span
				m_input.m_nSpanWithCaret--;
				m_input.GetSpanWithCaret()->SetCaretPos(wxTBCP_END);
				UpdateCaret();

			} else if (key.GetKeyCode() == WXK_DELETE) {
				
				if (m_input.m_nSpanWithCaret >= m_spans.GetCount()) {
					
					// the span we have just deleted was the last span of the array...
					// we must move the caret in the previous span
					m_input.m_nSpanWithCaret--;
					m_input.GetSpanWithCaret()->SetCaretPos(wxTBCP_END);

				} else {

					// don't touch m_input.m_nSpanWithCaret	
					m_input.GetSpanWithCaret()->SetCaretPos(wxTBCP_START);
				}

				UpdateCaret();
			}

			m_input.GetSpanWithCaret()->CheckCaretPos();
			needslayout = TRUE;
		}
		break;

	case wxIR_DELETE_PREVIOUS:
		if (m_input.m_nSpanWithCaret > 0) {

			// move the caret in the previous span
			m_input.m_nSpanWithCaret--;
			m_input.GetSpanWithCaret()->SetCaretPos(wxTBCP_END);

			if (m_input.GetSpanWithCaret()->IsLastOfTheLine()) {

				// let's merge these two spans together, eventually removing the \n from
				// the first and handling all style issues...
				OnInputMergeSpan(m_input.m_nSpanWithCaret, m_input.m_nSpanWithCaret+1);
				needslayout = TRUE;

			} else {

				// simulate another keypress of this key (backspace probably)
				// however, this time the keypress will be handled on the 
				// span where we have just moved the caret on
				return OnInput(key);
			}

		} else {

			// cannot delete in the previous span since this is the first span
			wxBell();
		}
		break;

	case wxIR_DELETE_NEXT:
		if (m_input.m_nSpanWithCaret < m_spans.GetCount()-1) {

			// move the caret in the previous span
			int oldspan = m_input.m_nSpanWithCaret;
			m_input.m_nSpanWithCaret++;
			m_input.GetSpanWithCaret()->SetCaretPos(wxTBCP_START);

			if (m_spans[oldspan]->IsLastOfTheLine()) {

				// let's merge these two spans together, eventually removing the \n from
				// the first and handling all style issues...
				OnInputMergeSpan(oldspan, m_input.m_nSpanWithCaret);
				needslayout = TRUE;

			} else {

				// simulate another keypress of this key (backspace probably)
				// however, this time the keypress will be handled on the 
				// span where we have just moved the caret on
				return OnInput(key);
			}

		} else {

			// cannot delete in the previous span since this is the first span
			wxBell();
		}
		break;

	default:
		wxASSERT_MSG(0, wxT("Unknown return flag"));
	}

#ifdef __WXDEBUG__
	if (r != wxIR_DELETE_THIS)
		wxASSERT(!GetSpanWithCaret()->IsEmpty());
#endif

	// update cursor position
	UpdateCaret();

	if (needslayout) {

		Layout();
		Refresh();

	} else if (needsrefresh) {

		Refresh();

	} else {
		
		// redraw the modified span
		// NOTE: this must be done *after* updating the caret to avoid
		//       ugly artifacts
		wxClientDC our(this);
		m_input.GetSpanWithCaret()->Paint(our);
	}

	return wxIR_OKAY;
}

int wxTextBox::GetLineWithCaret() const
{
    int i;

	// cycle until we found that line which contains the index of
	// the span with the caret
	for (i=0; i < (int)m_lines.GetCount() && 
		m_input.m_nSpanWithCaret > m_lines[i]; i++)
		;
	
	return i;
}

void wxTextBox::OnUserCaretMove(long modifiers)
{
	bool extendselection = (modifiers & wxTBMCM_EXTEND_SELECTION);
	
	// deselect if required
	if (HasSelection() && !extendselection)
		m_input.DeSelect();
}

wxMoveCaretRes wxTextBox::MoveCaret(wxMoveCaretFlag flag, long modifiers)
{
	/*wxASSERT_MSG(!GetSpanWithCaret()->IsDelimiter(),
		wxT("The caret cannot be placed on a delimiter span !"));*/

	bool extendselection = (modifiers & wxTBMCM_EXTEND_SELECTION),
		wasselected = HasSelection(),
		needsupdate = HasSelection() || extendselection;
	int linewithcaret = GetLineWithCaret();

	// deselect if required
    OnUserCaretMove(modifiers);
    
	// drop this move caret request to the focused caret
	wxMoveCaretRes r = GetSpanWithCaret()->MoveCaret(flag, modifiers);
	switch (r) {
	case wxMCR_SETFOCUS_PREVIOUS:
		if (m_input.m_nSpanWithCaret <= 0)
			return wxMCR_SETFOCUS_PREVIOUS;

		// update span with sel start 
		if (m_input.IsExtendingSelStart() && extendselection &&
			m_input.m_nSpanWithCaret == m_input.m_nSpanWithSelStart)
			m_input.m_nSpanWithSelStart--;
		if (m_input.IsExtendingSelEnd() && extendselection &&
			m_input.m_nSpanWithCaret == m_input.m_nSpanWithSelEnd)
			m_input.m_nSpanWithSelEnd--;

		m_input.m_nSpanWithCaret--;		
		GetSpanWithCaret()->SetCaretPos(wxTBCP_END, modifiers);
		break;

	case wxMCR_SETFOCUS_NEXT:
		if (m_input.m_nSpanWithCaret >= m_spans.GetCount()-1)
			return wxMCR_SETFOCUS_NEXT;

		// update span with sel end
		if (m_input.IsExtendingSelStart() && extendselection &&
			m_input.m_nSpanWithCaret == m_input.m_nSpanWithSelStart)
			m_input.m_nSpanWithSelStart++;
		if (m_input.IsExtendingSelEnd() && extendselection &&
			m_input.m_nSpanWithCaret == m_input.m_nSpanWithSelEnd)
			m_input.m_nSpanWithSelEnd++;

		m_input.m_nSpanWithCaret++;
		GetSpanWithCaret()->SetCaretPos(wxTBCP_START, modifiers);
		break;

	// the only easy way to move caret up/down is to use our MoveCaretUsingPoint
	// function since we cannot be sure that a span do exist in the space immediately
	// above the current span: it could be space unavailable...
	case wxMCR_SETFOCUS_ABOVE:
	case wxMCR_SETFOCUS_BELOW:
		{
			// handle simplest cases
			if (GetLineWithCaret() == 0 && r == wxMCR_SETFOCUS_ABOVE)
				return wxMCR_SETFOCUS_ABOVE;
			if (GetLineWithCaret() == (int)m_lines.GetCount()-1 && r == wxMCR_SETFOCUS_BELOW)
				return wxMCR_SETFOCUS_BELOW;

			// get old data (well, this is the current data but it will replaced soon)
			int oldspanpos = m_input.m_nSpanWithCaret;
			int oldcaretpos = m_input.m_nCaretPos;
			wxPoint pt(GetPosition() + GetCaret()->GetPosition());

			// the caret y position is always the y position of the line with the caret;
			// so to move caret above, we just "emulate a mouse click"...
			if (r == wxMCR_SETFOCUS_ABOVE)
				pt.y--;				// a pixel above
			else
				pt.y += GetLineMaxHeight(linewithcaret)+1;		// a line below

			// move the caret in a simple way
			MoveCaretUsingPoint(pt);

			// honour selection system
			if (extendselection) {

				if (wasselected) {

					// a selection already exist... extend it
					bool isstart = m_input.m_nSpanWithSelStart > m_input.m_nSpanWithCaret;
					if (isstart) {
						m_input.m_nSpanWithSelStart = m_input.m_nSpanWithCaret;
						m_input.m_nSelStart = m_input.m_nCaretPos;
					} else {
						m_input.m_nSpanWithSelEnd = m_input.m_nSpanWithCaret;
						m_input.m_nSelEnd = m_input.m_nCaretPos;
					}

				} else {

					// we already saved the old span & caret position before
					bool oldisstart = oldspanpos < m_input.m_nSpanWithCaret;
					wxASSERT((oldisstart && r == wxMCR_SETFOCUS_BELOW) ||
							(!oldisstart && r == wxMCR_SETFOCUS_ABOVE));

					// set the new selection endpoints
					m_input.m_nSpanWithSelStart = oldisstart ? oldspanpos : m_input.m_nSpanWithCaret;
					m_input.m_nSelStart = oldisstart ? oldcaretpos : m_input.m_nCaretPos;
					m_input.m_nSpanWithSelEnd = oldisstart ? m_input.m_nSpanWithCaret : oldspanpos;
					m_input.m_nSelEnd = oldisstart ? m_input.m_nCaretPos : oldcaretpos;
				}
			}
		}
		break;
	}

	// move the cursor away from delimiter spans
	//AdjustCaretPos(flag == wxMCF_LEFT || flag == wxMCF_UP);
	UpdateCaret();

	// update window
	if (needsupdate) {

		//if (wasselected && !HasSelection()) {
			Refresh();
	/*	} else {
			wxWindowDC dc(this);
			GetSpanWithCaret()->Paint(dc);
		}*/
	}

	return wxMCR_OKAY;
}

void wxTextBox::AdjustCaretPos(bool moveleft)
{
	// avoid delimiter spans
	while (GetSpanWithCaret()->IsDelimiter()) {

		// move caret on the right/left to avoid this delimiter...
		if (moveleft) {

			wxASSERT_MSG(m_input.m_nSpanWithCaret > 0, 
				wxT("A delimiter span is our first span ?"));

			m_input.m_nSpanWithCaret--;
			GetSpanWithCaret()->SetCaretPos(wxTBCP_END);

		} else {

			wxASSERT_MSG(m_input.m_nSpanWithCaret < m_spans.GetCount()-1, 
				wxT("xxxx ?"));

			m_input.m_nSpanWithCaret++;
			GetSpanWithCaret()->SetCaretPos(wxTBCP_START);
		}
	}

	wxASSERT(!GetSpanWithCaret()->IsDelimiter());
}

wxPoint wxTextBox::AdjustPointForCaretMove(const wxPoint &p) const
{
	wxPoint newpt(p);		// it should be in SCREEN coords
	wxPoint ourpos = GetPosition();
	wxSize contentsize = GetVirtualSize();

	// check if the given point is after our last span
	if (newpt.y < ourpos.y) {

		newpt.y = ourpos.y;

	} else if (newpt.y >= ourpos.y+contentsize.y) {

		// in this case, adjust the y of the point and call ourselves recursively...
		newpt.y = ourpos.y + contentsize.y - GetLineMaxHeight(m_lines.GetCount()-1)/2;
		wxASSERT(newpt.y < ourpos.y + contentsize.y);
	}

	// now check x
	if (newpt.x < ourpos.x) 
		newpt.x = ourpos.x;
	if (newpt.x >= ourpos.x+contentsize.x) 
		newpt.x = ourpos.x+contentsize.x - 1;

	wxASSERT_MSG(IsInsideContents(newpt), wxT("Invalid SCREEN point coordinates"));
	return newpt;
}

wxMoveCaretRes wxTextBox::MoveCaretUsingPoint(const wxPoint &p)
{
	wxPoint newpt = AdjustPointForCaretMove(p);
	int res = GetSpanWithCaretUsingPoint(newpt);
	if (res == wxNOT_FOUND) 
		return wxMCR_CANNOT_SETFOCUS;
		
	// found the span with the given point; 
	// before storing the new caret position,
	// check that a valid caret position can be found
	// in the res-th span !
	wxMoveCaretRes r = m_spans[res]->MoveCaretUsingPoint(ConvertToTextboxCoord(newpt));
	if (r != wxMCR_OKAY)
		return r;

	// we now have a new valid caret position to save
	m_input.m_nSpanWithCaret = res;
	//AdjustCaretPos(TRUE);
	return wxMCR_OKAY;
}

int wxTextBox::GetSpanWithCaretUsingPoint(const wxPoint &pt) const
{
	wxPoint p = AdjustPointForCaretMove(pt);
	p = ConvertToTextboxCoord(p);

	int h, y = 0;
	for (int i=0; i < (int)m_lines.GetCount(); y+=h, i++) {
		h = GetLineMaxHeight(i);
		
		// is the given point in this line ?
		if (p.y >= y && p.y <= y+h) {

			// if this is the last line of this textbox, we must
			// get as final idx of the line the end of our span array
			int finalidxoftheline;
			if (i == (int)m_lines.GetCount()-1)
				finalidxoftheline = m_spans.GetCount();
			else
				finalidxoftheline = m_lines[i+1];

			// find the span which contains it (if any)
			for (int x=m_lines[i]; x < finalidxoftheline; x++)
				if (m_spans[x]->GetRect().Inside(p))
					return x;
   		}
	}

	// we could not find a row containing the given point...
	return wxNOT_FOUND;
}

void wxTextBox::SelectUsingRect(const wxPoint &start, const wxPoint &end)
{
	// check that everything's okay
	wxASSERT_MSG(IsInsideContents(start) &&
				IsInsideContents(end), wxT("invalid points"));
	wxASSERT_MSG(start.y <= end.y, wxT("invalid points"));
	
	int oldspanwithselstart = m_input.m_nSpanWithSelStart;
	int oldspanwithselend = m_input.m_nSpanWithSelEnd;
	if (oldspanwithselstart == -1)
		oldspanwithselstart = m_spans.GetCount()-1;
	if (oldspanwithselend == -1)
		oldspanwithselend = 0;

	// set the startpoint for this selection
	int newstart = GetSpanWithCaretUsingPoint(start);
	if (newstart != wxNOT_FOUND) {

		m_input.m_nSpanWithSelStart = newstart;
		m_input.m_nSelStart = m_input.GetSpanWithSelStart()->
				GetCaretPosUsingPoint(ConvertToTextboxCoord(start));

	} else {

		// if we cannot find a good selection startpoint then the user cannot begin
		// a selection there... abort
		return;
	}

	// set the endpoint for this selection
	int newend = GetSpanWithCaretUsingPoint(end);
	if (newend != wxNOT_FOUND) {

		m_input.m_nSpanWithSelEnd = newend;
		m_input.m_nSelEnd = m_input.GetSpanWithSelEnd()->
				GetCaretPosUsingPoint(ConvertToTextboxCoord(end));

	} else {

		// leave old selection, if it was present
		if (m_input.m_nSpanWithSelEnd == -1 ||
			m_input.m_nSelEnd == -1)
			m_input.DeSelect();
	}

	wxLogDebug(wxT("wxTextBox::SelectUsingRect - rect: %d;%d - %d;%d; ")
				wxT("new startpoint: %d/%d; new endpoint: %d/%d"),
				start.x, start.y, end.x, end.y,
				m_input.m_nSpanWithSelStart, m_input.m_nSelStart,
				m_input.m_nSpanWithSelEnd, m_input.m_nSelEnd);

	// redraw all modified spans
	wxWindowDC dc(this);
	for (int i=wxMin(m_input.m_nSpanWithSelStart, oldspanwithselstart),
			max=wxMax(m_input.m_nSpanWithSelEnd, oldspanwithselend); 
			i <= max; i++)
		m_spans[i]->Paint(dc);
}




// -----------------------------
// wxTEXTBOX - export
// -----------------------------

wxString wxTextBox::ExportRTF() const
{
	//wxString ret = wxT("{\\rtf1\\ansi\\par\n");
	//ret += m_spans.ExportRTF();
	//return ret + wxT(" }");

	wxString text = wxT("{\\rtf1\\ansi\\pard ciao\\par }");
	return text;
}

wxString wxTextBox::ExportSelectionToRTF() const
{
	wxString ret;

	return ret;
}


