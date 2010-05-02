//
// Name     : mmHyperText
// Purpose  : A clickable text that performs an action when clicked.
//            The default action is to bring up the default browser.
// Author   : Arne Morken
// Copyright: (C) 2000-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence
//

#include "mmHyperText.h"

#ifdef __MMDEBUG__
extern wxTextCtrl* gDebug; // For global debug output
#endif

BEGIN_EVENT_TABLE(mmHyperText,wxWindow)
  EVT_PAINT       (mmHyperText::OnPaint)
  EVT_MOUSE_EVENTS(mmHyperText::OnMouse)
END_EVENT_TABLE()

mmHyperText::mmHyperText(wxWindow* parent,
		         const wxWindowID id,
		         const wxString& str,
		         const wxString& url,
                         const wxPoint& pos,
                         const wxSize& size,
		         const long style)
: wxWindow(parent, id, pos, size, style)
{
  mText = str;
  mStyle = style;
  mURL = url;
  mMargin = 0;
  mIsVisited = FALSE;
  mLeftIsDown = FALSE;
  mUnvisitedColour = new wxColour(  0,0,255); // BLUE
  mVisitedColour   = new wxColour(128,0,128); // PURPLE
  wxFont font = GetFont();
  font.SetUnderlined(TRUE);
  SetFont(font);
  mEnterCursor = new wxCursor(wxCURSOR_HAND);
  mLeaveCursor = new wxCursor(wxCURSOR_ARROW);
} // Constructor

mmHyperText::~mmHyperText(void)
{
} // Destructor

void mmHyperText::OnMouse(wxMouseEvent& event)
{
  if (event.Entering())
  {
    mLeftIsDown = FALSE;
    SetCursor(*mEnterCursor);
    Refresh();
  }
  else
  if (event.Leaving())
  {
    mLeftIsDown = FALSE;
    SetCursor(*mLeaveCursor);
    Refresh();
  }
  else
  if (event.LeftDown() || event.LeftDClick())
  {
    mLeftIsDown = TRUE;
    Refresh();
  }
  else
  if (event.LeftUp() && mLeftIsDown)
  {
    mIsVisited = TRUE;
    mLeftIsDown = FALSE;
    Refresh();
    ::wxSafeYield();

    if (mURL.StartsWith("http://") || mURL.StartsWith("mailto:"))
      HyperExec(TRUE);
    else
      HyperExec(FALSE);
  }

  event.Skip();
} // OnMouse

void mmHyperText::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  dc.SetFont(GetFont());
  if (!(mStyle & mmHT_NO_AUTOSIZE))
  {
    int lw,lh,ext;
    GetTextExtent(mText,&lw,&lh,&ext);
    SetClientSize(wxSize(lw+2*mMargin,lh+ext+2*mMargin));
  }
  if (mIsVisited)
    dc.SetTextForeground(*mVisitedColour);
  else
    dc.SetTextForeground(*mUnvisitedColour);
  dc.SetBackgroundMode(wxTRANSPARENT);
  dc.Clear();
  dc.DrawText(mText,mMargin,mMargin);
  if (mLeftIsDown)
  {
    int w,h;
    GetClientSize(&w,&h);
    dc.SetPen(*wxMEDIUM_GREY_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle(0,0,w,h);
  }
} // OnPaint

void mmHyperText::HyperExec(bool isURL)
{
    wxString ext;
    if (isURL)
      ext = ".htm";
    else
      ext = mURL.AfterLast('.');
    wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(ext);
    if (ft)
    {
      wxString desc;
      if (ft->GetDescription(&desc))
      {
        wxString cmd;
        if (ft->GetOpenCommand(&cmd,wxFileType::MessageParameters(mURL,_T(""))))
        {
	  if (isURL)
            cmd.Replace("file://","");
	  /*
          wxString str;
          str.Printf(_T("To open %s (%s) do '%s'.\n"),
                     mURL.c_str(), desc.c_str(), cmd.c_str());
#ifdef __MMDEBUG__
          *gDebug<<str;
#endif
	  */
          ::wxExecute(cmd);
        }
      } // if (ft->GetDescription)
      delete ft;
    } // if (ft)
} // HyperExec

