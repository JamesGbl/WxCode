/////////////////////////////////////////////////////////////////////////////
// Name:        resizec.cpp
// Purpose:     wxResizeableControl
// Author:      Markus Greither
// Modified by:
// Created:     11/11/02
// RCS-ID:      $Id: resizec.cpp,v 1.5 2005-10-02 19:43:46 frm Exp $
// Copyright:   (c) Markus Greither
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "resizec.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if defined(__BORLANDC__)
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/image.h"
#include "wx/metafile.h"
#include "wx/clipbrd.h"
#include "wx/resizec.h"

#ifdef __WXMSW__
#include <windows.h>
#include "wx/msw/winundef.h"
#include "wx/msw/dib.h"
#include "wx/msw/private.h"
#endif


// list implementation
#include <wx/listimpl.cpp>
WX_DEFINE_LIST(wxResizeableControlList);


// ----------------------------------------------------------------------------
// Events
// ----------------------------------------------------------------------------

DEFINE_EVENT_TYPE(wxEVT_COMMAND_CHILD_CREATED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_CHILD_MOVED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_CHILD_CLOSED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_CHILD_RESIZED)

DEFINE_EVENT_TYPE(wxEVT_COMMAND_SIBLING_CREATED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_SIBLING_MOVED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_SIBLING_CLOSED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_SIBLING_RESIZED)

DEFINE_EVENT_TYPE(wxEVT_COMMAND_SHOW_SIZERS)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_HIDE_SIZERS)

// ----------------------------------------------------------------------------
// wxZoomData
// ----------------------------------------------------------------------------

void wxZoomData::SetSize(int width,int height)
{
    if ((m_currRect.width != width) ||
        (m_currRect.height != height))
    {
        m_orgSize = wxSize(int(width/m_zoom),
                           int(height/m_zoom));
        m_currRect.width = width;
        m_currRect.height = height;
    }
}

void wxZoomData::Move(int xpos,int ypos)
{
    if ((m_currRect.x != xpos) ||
        (m_currRect.x != ypos))
    {
        m_orgPos = wxPoint(int(xpos/m_zoom),
                           int(ypos/m_zoom));
        m_currRect.x = xpos;
        m_currRect.y = ypos;
    }
}

void wxZoomData::SetZoomRect(wxWindow *Window,float zoom,
                 int curxoffs,int curyoffs,
                 int newxoffs,int newyoffs)
{
    m_currRect = wxRect(int((m_orgPos.x+curxoffs-newxoffs)*zoom),
                        int((m_orgPos.y+curyoffs-newyoffs)*zoom),
                        int(m_orgSize.x*zoom),int(m_orgSize.y*zoom));
    m_zoom = zoom;
    Window->SetSize(m_currRect);
}

// ----------------------------------------------------------------------------
// wxResizeableControl
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxResizeableControl, wxWindow)

wxResizeableControl::wxResizeableControl(wxWindow *AParent,
                                         int AnId,const wxPoint &pos,
                                         const wxSize &size,long style,
                                         const wxString &name)
 : wxWindow(AParent,AnId,pos,size,style,name),m_zoomData(size,pos),
   m_curId(-2),m_capt(0),m_movemode(wxResizeableControl::MoveWin),m_hasfocus(false)
{
    wxCommandEvent cevent(wxEVT_COMMAND_CHILD_CREATED,GetId());
    if (GetParent())
        GetParent()->AddPendingEvent(cevent);
}

void wxResizeableControl::FocusRectCoord(wxDC &DC,wxCoord x1,wxCoord y1,
                                         wxCoord w,wxCoord h)
{
    wxPen pen(*wxBLACK,1,wxDOT);
    const wxBrush &OldBr = DC.GetBrush();
    DC.SetBrush(*wxTRANSPARENT_BRUSH);
    DC.SetPen(pen);
    DC.SetLogicalFunction(wxINVERT);
    DC.DrawRectangle(x1,y1,w,h);
    DC.SetBrush(OldBr);
    DC.SetPen(wxNullPen);
    DC.SetLogicalFunction(wxCOPY);
}

// Draws the current window size
void wxResizeableControl::DrawMoveRect(wxPoint hp,int Mode,float Ratio)
{
    wxScreenDC dc;
    wxRect rect = NewRect(hp,Mode,Ratio);
    DrawFocusRect(dc,rect);
}

// FIXME: This doesn't work properly for windows that are
// very small and the size rectangles overlap
int wxResizeableControl::PointInSizeRect(wxPoint hp)
{
    for (int i = 0;i < 8;i++)
    {
        int x = 6+GetSizeX(i)*(GetClientSize().x-11)/2;
        int y = 6+GetSizeY(i)*(GetClientSize().y-11)/2;
        wxRect rect(wxPoint(x-SizeXRad,y-SizeYRad),
                    wxPoint(x+SizeXRad,y+SizeYRad));
        if ((hp.x >= rect.x) && (hp.x <= rect.x+rect.width) &&
            (hp.y >= rect.y) && (hp.y <= rect.y+rect.height))
            return i;
    }
    return MoveWin;
}

// Calculates the current window size (Parent coordinates)
wxRect wxResizeableControl::NewRect(wxPoint hp,int Mode,float Ratio)
{
    wxSize size = GetClientSize();
    wxPoint pt(0,0);
    ClientToScreen(&pt.x,&pt.y);
    wxRect rect(pt,size);
    wxRect rect1(rect);
    wxPoint delta(hp.x-m_curpos.x,hp.y-m_curpos.y);
    switch (Mode)
    {
        case MoveWin : {rect.x += delta.x; rect.y += delta.y;}; break;
        case Top : rect.y += delta.y; rect.height -= delta.y; break;
        case Bottom : rect.height += delta.y; break;
        case Left : rect.x += delta.x; rect.width -= delta.x; break;
        case Right : rect.width += delta.x; break;
        case TopLeft :
            rect.x += delta.x;
            rect.y += delta.y;
            rect.width -= delta.x;
            rect.height -= delta.y;
            if (Ratio)
            {
              if (rect.GetWidth() < rect.GetHeight()*Ratio)
              {
                  int tmp = int(rect.GetHeight()*Ratio);
                  rect.x = rect.GetRight()-tmp+1;
                  rect.width = tmp;
              }
              else
              {
                  int tmp = int(rect.GetWidth()/Ratio);
                  rect.y = rect.GetBottom()-tmp+1;
                  rect.height = tmp;
              }
            }
            break;
        case TopRight :
            rect.width += delta.x;
            rect.y += delta.y;
            rect.height -= delta.y;
            if (Ratio)
            {
                if (rect.GetWidth() < rect.GetHeight()*Ratio)
                    rect.width = int(rect.GetHeight()*Ratio);
                else
                {
                    int tmp = int(rect.GetWidth()/Ratio);
                    rect.y = rect.GetBottom()-tmp+1;
                    rect.height = tmp;
                }
            }
            break;
        case BottomLeft :
            rect.x += delta.x;
            rect.width -= delta.x;
            rect.height += delta.y;
            if (Ratio)
            {
                if (rect.GetWidth() < rect.GetHeight()*Ratio)
                {
                    int tmp = int(rect.GetHeight()*Ratio);
                    rect.x = rect.GetRight()-tmp+1;
                    rect.width = tmp;
                }
                else
                    rect.height = int(rect.GetWidth()/Ratio);
            }
            break;
        case BottomRight :
            rect.width += delta.x;
            rect.height += delta.y;
            if (Ratio)
            {
                if (rect.GetWidth() < rect.GetHeight()*Ratio)
                    rect.width = int(rect.GetHeight()*Ratio);
                else
                    rect.height = int(rect.GetWidth()/Ratio);
            }
            break;
    }
    wxSize size1 = GetParent()->GetClientSize();
    wxPoint pt1(0,0);
    GetParent()->ClientToScreen(&pt1.x,&pt1.y);
    wxRect prect(pt1,size1);
    prect.Inflate(-30,-30);
    if ((((rect.x <= prect.GetRight()) &&
          (prect.x <= rect.GetRight())) &&
         ((rect.y <= prect.GetBottom()) &&
          (prect.y <= rect.GetBottom()))) &&
         (rect.GetWidth() >= 20) && (rect.GetHeight() >= 20))
        return rect;
    else
        return rect1;
}

// Draws the size rectangles
void wxResizeableControl::DrawSizeRect(wxDC &dc)
{
    int AltROP = dc.GetLogicalFunction();
    dc.SetLogicalFunction(wxXOR);
    wxBrush brs(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW),wxSOLID);
    const wxBrush &OldBr = dc.GetBrush();
    dc.SetBrush(brs);
    wxPen pen(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW),1,wxSOLID);
    dc.SetPen(pen);
    for (int i = 0;i < 8;i++)
    {
        int x = 0,y = 0;
        switch (GetSizeX(i))
        {
            case 0 : x = SizeXRad; break;
            case 1 : x = GetClientSize().x/2; break;
            case 2 : x = GetClientSize().x-SizeXRad; break;
        }
        switch (GetSizeY(i))
        {
            case 0 : y = SizeYRad; break;
            case 1 : y = GetClientSize().y/2; break;
            case 2 : y = GetClientSize().y-SizeYRad; break;
        }
        dc.DrawRectangle(x-SizeXRad,y-SizeYRad,2*SizeXRad,2*SizeYRad);
    }
    dc.SetPen(wxNullPen);
    dc.SetBrush(OldBr);
    dc.SetLogicalFunction(AltROP);
}

void wxResizeableControl::OnSetCursor(wxSetCursorEvent &event)
{
    long akcurid = -1;
    if (m_hasfocus)
    {
        wxPoint hp(event.GetX(),event.GetY()),point = hp;
        switch (PointInSizeRect(point))
        {
            case MoveWin :
                akcurid = wxCURSOR_SIZING;
                break;
            case Top :
            case Bottom :
                akcurid = wxCURSOR_SIZENS;
                break;
            case Left :
            case Right :
                akcurid = wxCURSOR_SIZEWE;
                break;
            case TopRight :
            case BottomLeft :
                akcurid = wxCURSOR_SIZENESW;
                break;
            case BottomRight :
            case TopLeft :
                akcurid = wxCURSOR_SIZENWSE;
                break;
            default :
                m_csr = *wxSTANDARD_CURSOR;
        }
    }
    else
        akcurid = wxCURSOR_SIZING;
    if ((akcurid != m_curId) || (akcurid == -1))
    {
        m_curId = akcurid;
        if (m_curId != -1)
            m_csr = wxCursor(m_curId);
#ifdef __WXGTK__
        SetCursor(m_csr);
#endif
    }
#ifdef __WXMSW__
    wxPoint pt(event.GetX(),event.GetY());
    pt = ClientToScreen(pt);
    if (wxFindWindowAtPoint(pt) == this)
        event.SetCursor(m_csr);
#endif
}

void wxResizeableControl::OnLButtonDown(wxMouseEvent &event)
{
    wxPoint hp;
    hp.x = event.GetX();
    hp.y = event.GetY();
    ScreenToClient(hp);
    m_curpos = hp;
    m_lastcurpos = hp;
    if (!m_hasfocus)
    {
        SetFocus();
        m_hasfocus = true;
        m_movemode = MoveWin;
    }
    else
    {
        m_movemode = PointInSizeRect(hp);
    }
    CaptureMouse();
    m_capt = true;
    m_moved = false;
}

void wxResizeableControl::OnMouseMove(wxMouseEvent &event)
{
    wxPoint hp;
    hp.x = event.GetX();
    hp.y = event.GetY();
    ScreenToClient(hp);
    if (hp != m_lastcurpos)
    {
        if (m_capt)
        {
            if (m_moved)
                DrawMoveRect(m_lastcurpos,m_movemode,GetRatio());
            m_moved |= ((abs(m_curpos.x-hp.x) > 10) ||
                        (abs(m_curpos.y-hp.y) > 10));
            if (m_moved)
                DrawMoveRect(hp,m_movemode,GetRatio());
        }
        m_lastcurpos = hp;
    }
#ifdef __WXGTK__
    wxSetCursorEvent evt(hp.x,hp.y);
    OnSetCursor(evt);
#endif
}

void wxResizeableControl::OnLButtonUp(wxMouseEvent &event)
{
    wxPoint point;
    point.x = event.GetX();
    point.y = event.GetY();
    ScreenToClient(point);
    if (m_capt)
    {
        ReleaseMouse();
        m_capt = false;
        if (m_moved)
        {
            DrawMoveRect(m_lastcurpos,m_movemode,GetRatio());
            wxRect rect = NewRect(point,m_movemode,GetRatio());
            wxPoint pt = rect.GetPosition();
            GetParent()->ScreenToClient(&pt.x,&pt.y);
            rect.x = pt.x;
            rect.y = pt.y;
            SetSize(rect);
            Refresh();
        }
    }
    m_movemode = MoveWin;
}

int wxResizeableControl::GetSizeX(int Mode)
{
    int Pos = -1;
    switch (Mode)
    {
        case Bottom :
        case Top : Pos = 1; break;
        case TopLeft :
        case BottomLeft :
        case Left : Pos = 0; break;
        case TopRight :
        case BottomRight :
        case Right : Pos = 2; break;
    }
    return Pos;
}

int wxResizeableControl::GetSizeY(int Mode)
{
  int Pos = -1;
  switch (Mode)
  {
      case TopRight :
      case TopLeft :
      case Top : Pos = 0; break;
      case Left :
      case Right : Pos = 1; break;
      case BottomRight :
      case BottomLeft :
      case Bottom : Pos = 2; break;
  }
  return Pos;
}

bool wxResizeableControl::Destroy()
{
    wxCommandEvent cevent(wxEVT_COMMAND_CHILD_CLOSED,GetId());
    if (GetParent())
        GetParent()->AddPendingEvent(cevent);
    return wxWindow::Destroy();
}

wxResizeableControlCanvas *wxResizeableControl::GetCanvas()
{
    wxResizeableControlCanvas *cnv = wxDynamicCast(GetParent(),wxResizeableControlCanvas);
    return cnv;
}

void wxResizeableControl::ChangeSizerView(bool show)
{
    if (show)
    {
        if (!m_hasfocus)
        {
            Raise();
            Update();
            wxClientDC DC(this);
            DrawSizeRect(DC);
            m_hasfocus = true;
        }
    }
    else
    {
        if (m_hasfocus)
        {
            m_hasfocus = false;
            wxClientDC DC(this);
            DrawSizeRect(DC);
        }
    }
}

void wxResizeableControl::OnKillFocus(wxFocusEvent &)
{
    ChangeSizerView(false);
}

void wxResizeableControl::OnSetFocus(wxFocusEvent &)
{
    ChangeSizerView(true);
}

void wxResizeableControl::OnHideSizers(wxCommandEvent &)
{
    ChangeSizerView(false);
}

void wxResizeableControl::OnShowSizers(wxCommandEvent &)
{
    ChangeSizerView(true);
}

void wxResizeableControl::OnKeyDown(wxKeyEvent &event)
{
    if (event.AltDown())
    {
        event.Skip();
        return;
    }
    switch (event.GetKeyCode())
    {
        /*case WXK_DELETE :
        {
            Destroy();
            break;
        }*/
        case WXK_LEFT :
          if (event.ControlDown())
              if (event.ShiftDown())
                  SetSize(GetPosition().x,GetPosition().y,
                          GetSize().x-1,GetSize().y);
              else
                  Move(GetPosition().x-1,GetPosition().y);
          break;
        case WXK_RIGHT :
          if (event.ControlDown())
              if (event.ShiftDown())
                  SetSize(GetPosition().x,GetPosition().y,
                          GetSize().x+1,GetSize().y);
              else
                  Move(GetPosition().x+1,GetPosition().y);
          break;
        case WXK_UP :
          if (event.ControlDown())
              if (event.ShiftDown())
                  SetSize(GetPosition().x,GetPosition().y,
                          GetSize().x,GetSize().y-1);
              else
                  Move(GetPosition().x,GetPosition().y-1);
          break;
        case WXK_DOWN :
          if (event.ControlDown())
              if (event.ShiftDown())
                  SetSize(GetPosition().x,GetPosition().y,
                          GetSize().x,GetSize().y+1);
              else
                  Move(GetPosition().x,GetPosition().y+1);
          break;
        default:
          event.Skip();
    }
}

void wxResizeableControl::OnEditCut(wxCommandEvent &)
{
    Destroy();
}

void wxResizeableControl::OnSize(wxSizeEvent &event)
{
    wxCommandEvent cevent(wxEVT_COMMAND_CHILD_RESIZED,GetId());
    if (GetParent())
        GetParent()->AddPendingEvent(cevent);
    m_zoomData.SetSize(event.GetSize().x,event.GetSize().y);
}

void wxResizeableControl::OnMove(wxMoveEvent &event)
{
    wxCommandEvent cevent(wxEVT_COMMAND_CHILD_MOVED,GetId());
    if (GetParent())
        GetParent()->AddPendingEvent(cevent);
    m_zoomData.Move(event.GetPosition().x,event.GetPosition().y);
}

BEGIN_EVENT_TABLE(wxResizeableControl,wxWindow)
    EVT_MENU(wxID_CUT,wxResizeableControl::OnEditCut)
    EVT_SET_FOCUS(wxResizeableControl::OnSetFocus)
    EVT_KILL_FOCUS(wxResizeableControl::OnKillFocus)
    EVT_MOTION(wxResizeableControl::OnMouseMove)
    EVT_SET_CURSOR(wxResizeableControl::OnSetCursor)
    EVT_LEFT_UP(wxResizeableControl::OnLButtonUp)
    EVT_LEFT_DOWN(wxResizeableControl::OnLButtonDown)
    EVT_KEY_DOWN(wxResizeableControl::OnKeyDown)
    EVT_SIZE(wxResizeableControl::OnSize)
    EVT_MOVE(wxResizeableControl::OnMove)
    EVT_SHOW_SIZERS(-1,wxResizeableControl::OnShowSizers)
    EVT_HIDE_SIZERS(-1,wxResizeableControl::OnHideSizers)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// wxPictureControl
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxPictureControl,wxResizeableControl)

void wxPictureControl::OnEditCut(wxCommandEvent &event)
{
    OnEditCopy(event);
    event.Skip();
}

void wxPictureControl::OnRightDown(wxMouseEvent &event)
{
    wxMenu *context_menu = new wxMenu;

    context_menu->Append(wxID_COPY, _("Copy"));
    context_menu->Append(wxID_CUT, _("Cut"));
    context_menu->Append(wxID_REVERT, _("Set to original size"));

    wxPoint hp;
    hp = event.GetPosition();
    PopupMenu(context_menu,hp);
    delete context_menu;
}

void wxPictureControl::OnPaletteChanged(wxSysColourChangedEvent &)
{
    Refresh();
}

void wxPictureControl::OnSize(wxSizeEvent &event)
{
    Refresh();
    event.Skip();
}

void wxPictureControl::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);
    wxSize size = GetClientSize();
    Paint(dc,false,size);
    if (m_hasfocus)
        DrawSizeRect(dc);
}

void wxPictureControl::OnRevert(wxCommandEvent &)
{
    wxSize orgs = GetOriginalSize();
    SetClientSize(orgs.x != -1 ? orgs.x : GetClientSize().x,
                  orgs.y != -1 ? orgs.y : GetClientSize().y);
}

BEGIN_EVENT_TABLE(wxPictureControl,wxResizeableControl)
    EVT_MENU(wxID_REVERT,wxPictureControl::OnRevert)
    EVT_PAINT(wxPictureControl::OnPaint)
    EVT_RIGHT_DOWN(wxPictureControl::OnRightDown)
    EVT_SIZE(wxPictureControl::OnSize)
    EVT_UPDATE_UI(wxID_COPY,wxPictureControl::CeEditCopy)
    EVT_UPDATE_UI(wxID_CUT,wxPictureControl::CeEditCut)
    EVT_SYS_COLOUR_CHANGED(wxPictureControl::OnPaletteChanged)
END_EVENT_TABLE()

#if defined(__WXMSW__) && wxUSE_METAFILE && wxUSE_ENH_METAFILE

// ----------------------------------------------------------------------------
// wxMetafileControl
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxMetafileControl, wxPictureControl)

wxMetafileControl::wxMetafileControl(wxWindow *AParent, int AnId,WXHANDLE Meta,
                                     const wxPoint &pos,const wxSize &size,
                                     long style,const wxString &name)
 : wxPictureControl(AParent,AnId,pos,size,style,name)
{
    m_metafile.SetHENHMETAFILE(Meta);
    wxSize msize = m_metafile.GetSize();
    SetSize(msize.x,msize.y);
}

wxMetafileControl::wxMetafileControl(wxWindow *AParent, int AnId,char *Data,
                                     int Size,
                                     const wxPoint &pos,const wxSize &size,
                                     long style,const wxString &name)
 : wxPictureControl(AParent,AnId,pos,size,style,name)
{
    m_metafile.SetHENHMETAFILE((WXHANDLE)
                               ::SetEnhMetaFileBits(Size,(unsigned char *)Data));
}

wxMetafileControl::~wxMetafileControl()
{
}

void wxMetafileControl::OnEditCopy(wxCommandEvent &)
{
    if (wxTheClipboard->Open())
    {
        wxTheClipboard->Clear();
        m_metafile.SetClipboard(1,1);
        wxTheClipboard->Close();
    }
}

void wxMetafileControl::Paint(wxDC &dc,bool,wxSize &size)
{
    wxRect rct(wxPoint(0,0),size);
    if (m_metafile.Ok())
        m_metafile.Play(&dc,&rct);
}

wxSize wxMetafileControl::GetOriginalSize()
{
    return wxSize(m_metafile.GetWidth(),m_metafile.GetHeight());
}

float wxMetafileControl::GetRatio()
{
    if (m_metafile.GetHeight())
        return float(m_metafile.GetWidth())/m_metafile.GetHeight();
    else
        return 0;
}

long wxGetEnhMetaFileBits(HENHMETAFILE metafile,UINT size,LPBYTE data)
{
#ifdef __BORLANDC__
    return ::GetEnhMetaFileBits(metafile,size,(unsigned char *)data);
#else // some MinGW versions do not have GetEnhMetaFileBits in the headers
    HINSTANCE gdi = ::LoadLibrary(wxT("GDI32.DLL"));
    UINT (WINAPI *metab)(HENHMETAFILE,UINT,LPBYTE) = 0;
    long ret = 0;
    if (gdi)
        metab = (UINT (WINAPI *)(HENHMETAFILE,UINT,LPBYTE))
                ::GetProcAddress(gdi,"GetEnhMetaFileBits");
    if (metab)
        ret = metab(metafile,size,(unsigned char *)data);
    if (gdi)
        ::FreeLibrary(gdi);
    return ret;
#endif
}

long wxMetafileControl::GetPictureSize()
{
    return wxGetEnhMetaFileBits((HENHMETAFILE)m_metafile.GetHENHMETAFILE(),0,0);
}

long wxMetafileControl::GetPictureData(char *data,long n)
{
    return wxGetEnhMetaFileBits((HENHMETAFILE)m_metafile.GetHENHMETAFILE(),
                                n,(unsigned char *)data);
}

BEGIN_EVENT_TABLE(wxMetafileControl,wxPictureControl)
    EVT_MENU(wxID_CUT, wxMetafileControl::OnEditCut)
    EVT_MENU(wxID_COPY, wxMetafileControl::OnEditCopy)
END_EVENT_TABLE()

#endif // __WXMSW__

// ----------------------------------------------------------------------------
// wxBitmapControl
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxBitmapControl, wxPictureControl)

wxBitmapControl::wxBitmapControl(wxWindow *AParent, int AnId,
                                 const wxBitmap &bitmap,
                                 const wxPoint &pos,const wxSize &size,
                                 long style,const wxString &name)
 : wxPictureControl(AParent,AnId,pos,size,style,name)
{
    m_bitmap = new wxBitmap(bitmap);
}

#ifdef __WXMSW__

static size_t wxGetNumOfBitmapColors(size_t bitsPerPixel)
{
    switch ( bitsPerPixel )
    {
        case 1:
            // monochrome bitmap, 2 entries
            return 2;

        case 4:
            return 16;

        case 8:
            return 256;

        case 24:
            // may be used with 24bit bitmaps, but we don't use it here - fall
            // through

        case 16:
        case 32:
            // bmiColors not used at all with these bitmaps
            return 0;

        default:
            wxFAIL_MSG( wxT("unknown bitmap format") );
            return 0;
    }
}

wxBitmapControl::wxBitmapControl(wxWindow *AParent, int AnId,char *Data,
                                 int /*Size*/,const wxPoint &pos,
                                 const wxSize &size,
                                 long style,const wxString &name)
 : wxPictureControl(AParent,AnId,pos,size,style,name)
{
    const LPBITMAPINFO pbmi = (const LPBITMAPINFO)Data;
    const BITMAPINFOHEADER *pbmih = &pbmi->bmiHeader;

    // biClrUsed has the number of colors, unless it's 0
    int numColors = pbmih->biClrUsed;
    if (numColors==0)
    {
        numColors = wxGetNumOfBitmapColors(pbmih->biBitCount);
    }

    // offset of image from the beginning of the header
    DWORD ofs = numColors * sizeof(RGBQUAD);
    void *image = (char *)pbmih + sizeof(BITMAPINFOHEADER) + ofs;

    ScreenHDC hdc;
    HBITMAP hbmp = CreateDIBitmap(hdc, pbmih, CBM_INIT,
                                  image, pbmi, DIB_RGB_COLORS);
    if ( !hbmp )
    {
        wxLogLastError(wxT("CreateDIBitmap"));
    }

    wxBitmap bitmap(pbmih->biWidth, pbmih->biHeight, pbmih->biBitCount);
    bitmap.SetHBITMAP((WXHBITMAP)hbmp);

    m_bitmap = new wxBitmap(bitmap);
}

#endif

wxBitmapControl::~wxBitmapControl()
{
    delete m_bitmap;
}

void wxBitmapControl::Paint(wxDC &dc,bool,wxSize &size)
{
    if (m_bitmap)
    {
        wxImage img = m_bitmap->ConvertToImage();
        img.Rescale(size.x,size.y);
        wxBitmap bmp(img);
        dc.DrawBitmap(bmp,0,0);
    }
}

void wxBitmapControl::OnEditCopy(wxCommandEvent &)
{
    if (wxTheClipboard->Open())
    {
        wxTheClipboard->Clear();
        wxBitmapDataObject *bdo = new wxBitmapDataObject;
        wxBitmap *ClBitmap = new wxBitmap(*m_bitmap);
        bdo->SetBitmap(*ClBitmap);
        wxTheClipboard->SetData(bdo);
        wxTheClipboard->Close();
    }
}

#ifdef __WXMSW__

size_t ConvertBitmapToDIB(LPBITMAPINFO pbi, const wxBitmap& bitmap)
{
    wxASSERT_MSG( bitmap.Ok(), wxT("invalid bmp can't be converted to DIB") );

    // shouldn't be selected into a DC or GetDIBits() would fail
    wxASSERT_MSG( !bitmap.GetSelectedInto(),
                  wxT("can't copy bitmap selected into wxMemoryDC") );

    // prepare all the info we need
    BITMAP bm;
    HBITMAP hbmp = (HBITMAP)bitmap.GetHBITMAP();
    if ( !GetObject(hbmp, sizeof(bm), &bm) )
    {
        wxLogLastError(wxT("GetObject(bitmap)"));

        return 0;
    }

    // calculate the number of bits per pixel and the number of items in
    // bmiColors array (whose meaning depends on the bitmap format)
    WORD biBits = bm.bmPlanes * bm.bmBitsPixel;
    WORD biColors = (WORD)wxGetNumOfBitmapColors(biBits);

    BITMAPINFO bi2;

    bool wantSizeOnly = pbi == NULL;
    if ( wantSizeOnly )
        pbi = &bi2;

    // just for convenience
    BITMAPINFOHEADER& bi = pbi->bmiHeader;

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bm.bmWidth;
    bi.biHeight = bm.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = biBits;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // memory we need for BITMAPINFO only
    DWORD dwLen = bi.biSize + biColors * sizeof(RGBQUAD);

    // first get the image size
    ScreenHDC hdc;
    if ( !GetDIBits(hdc, hbmp, 0, bi.biHeight, NULL, pbi, DIB_RGB_COLORS) )
    {
        wxLogLastError(wxT("GetDIBits(NULL)"));

        return 0;
    }

    if ( wantSizeOnly )
    {
        // size of the header + size of the image
        return dwLen + bi.biSizeImage;
    }

    // and now copy the bits
    void *image = (char *)pbi + dwLen;
    if ( !GetDIBits(hdc, hbmp, 0, bi.biHeight, image, pbi, DIB_RGB_COLORS) )
    {
        wxLogLastError(wxT("GetDIBits"));

        return 0;
    }

    return dwLen + bi.biSizeImage;
}

long wxBitmapControl::GetPictureData(char *data,long /*n*/)
{
    ConvertBitmapToDIB((LPBITMAPINFO)data,*m_bitmap);
    return GetPictureSize();
}

long wxBitmapControl::GetPictureSize()
{
    return ConvertBitmapToDIB(0,*m_bitmap);
}

#endif

wxSize wxBitmapControl::GetOriginalSize()
{
    return wxSize(m_bitmap->GetWidth(),m_bitmap->GetHeight());
}

float wxBitmapControl::GetRatio()
{
    if (m_bitmap->GetHeight())
        return float(m_bitmap->GetWidth())/m_bitmap->GetHeight();
    else
        return 0;
}

BEGIN_EVENT_TABLE(wxBitmapControl,wxPictureControl)
    EVT_MENU(wxID_CUT, wxBitmapControl::OnEditCut)
    EVT_MENU(wxID_COPY, wxBitmapControl::OnEditCopy)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// wxResizeableParentControl
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxResizeableParentControl, wxResizeableControl)

void wxResizeableParentControl::OnSize(wxSizeEvent &event)
{
    wxSize size = event.GetSize();
    if (m_child)
        m_child->SetSize(wxResizeableControl::SizeXRad*2,
                         wxResizeableControl::SizeYRad*2,
                         size.x-2*wxResizeableControl::SizeXRad*2,
                         size.y-2*wxResizeableControl::SizeYRad*2);
    Refresh();
    event.Skip();
}

void wxResizeableParentControl::OnPaint(wxPaintEvent &event)
{
    if (m_hasfocus) {
		wxPaintDC dc(this);
        DrawSizeRect(dc);
	}

	event.Skip();
}

void wxResizeableParentControl::OnSiblingChange(wxCommandEvent &event)
{
    if (m_child)
		m_child->ProcessEvent(event);

	event.Skip();
}


BEGIN_EVENT_TABLE(wxResizeableParentControl,wxResizeableControl)
    EVT_SIZE(wxResizeableParentControl::OnSize)
    EVT_PAINT(wxResizeableParentControl::OnPaint)
	EVT_SIBLING_CHANGED(wxID_ANY, wxResizeableParentControl::OnSiblingChange)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// wxResizeableChildTextControl
// ----------------------------------------------------------------------------

void wxResizeableChildTextCtrl::OnKillFocus(wxFocusEvent &event)
{
	wxCommandEvent notification(wxEVT_COMMAND_HIDE_SIZERS);
	if (GetParent())
	    GetParent()->ProcessEvent(notification);
	event.Skip();
}

void wxResizeableChildTextCtrl::OnSetFocus(wxFocusEvent &event)
{
	wxCommandEvent notification(wxEVT_COMMAND_SHOW_SIZERS);
	if (GetParent())
	    GetParent()->ProcessEvent(notification);
	event.Skip();
}

BEGIN_EVENT_TABLE(wxResizeableChildTextCtrl,wxTextCtrl)
    EVT_SET_FOCUS(wxResizeableChildTextCtrl::OnSetFocus)
    EVT_KILL_FOCUS(wxResizeableChildTextCtrl::OnKillFocus)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// wxResizeableControlCanvas
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxResizeableControlCanvas, wxWindow)

void wxResizeableControlCanvas::OnChildWindowChange(wxCommandEvent &ev)
{
    m_scrollflag = true;

	// send events to the resizeable controls which are siblings
	// of the modified one
	wxCommandEvent notification;

	// convert event type from wxEVT_COMMAND_CHILD_XXX to wxEVT_COMMAND_SIBLING_XXX
	if (ev.GetEventType() == wxEVT_COMMAND_CHILD_MOVED)
		notification.SetEventType(wxEVT_COMMAND_SIBLING_MOVED);
	else if (ev.GetEventType() == wxEVT_COMMAND_CHILD_CREATED)
		notification.SetEventType(wxEVT_COMMAND_SIBLING_CREATED);
	else if (ev.GetEventType() == wxEVT_COMMAND_CHILD_RESIZED)
		notification.SetEventType(wxEVT_COMMAND_SIBLING_RESIZED);
	else if (ev.GetEventType() == wxEVT_COMMAND_CHILD_CLOSED)
		notification.SetEventType(wxEVT_COMMAND_SIBLING_CLOSED);
	else
		wxASSERT_MSG(0, wxT("Unknown child window change !"));

	notification.SetId(ev.GetId());
	notification.StopPropagation();		// we don't want that this event propagates upward toward parents

	// send the event to all other siblings
	wxResizeableControlList::Node *node = GetControlList().GetFirst();
	while (node)
	{
		wxResizeableControl *box = node->GetData();
		if (box == ev.GetEventObject()) {
			node = node->GetNext();
			continue;		// skip ourselves !
		}

		box->AddPendingEvent(notification);
		node = node->GetNext();
	}
}

void wxResizeableControlCanvas::UpdateScrollRange()
{
    wxWindowListNode *Child = GetChildren().GetFirst();
    wxSize GesSize;
    int x,y;
    GetViewStart(&x,&y);
    while (Child)
    {
        wxWindow *chw = wxDynamicCast(Child->GetData(),wxResizeableControl);
        if (chw)
        {
            wxRect rect = chw->GetRect();
            rect.x += x;
            rect.y += y;
            if (rect.GetBottom() > GesSize.y)
                GesSize.y = rect.GetBottom();
            if (rect.GetRight() > GesSize.x)
                GesSize.x = rect.GetRight();
        }
        Child = Child->GetNext();
    }
    SetScrollbars(1,1,GesSize.x,GesSize.y,x,y,true);
}

void wxResizeableControlCanvas::OnIdle(wxIdleEvent &event)
{
    if (m_scrollflag)
        UpdateScrollRange();
    m_scrollflag = false;
    event.Skip();
}

BEGIN_EVENT_TABLE(wxResizeableControlCanvas,wxScrolledWindow)
    EVT_IDLE(wxResizeableControlCanvas::OnIdle)
    EVT_CHILD_CHANGED(-1, wxResizeableControlCanvas::OnChildWindowChange)
END_EVENT_TABLE()


