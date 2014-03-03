/////////////////////////////////////////////////////////////////////////////
// Name:        basic.cpp
// Purpose:     Basic OGL classes
// Author:      Julian Smart
// Modified by: Troels K
// Created:     12/07/98
// RCS-ID:      $Id: basic.cpp,v 1.1 2007/03/28 15:15:56 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include <wx/ogl/ogl.h>

// Control point types
// Rectangle and most other shapes
#define CONTROL_POINT_VERTICAL   1
#define CONTROL_POINT_HORIZONTAL 2
#define CONTROL_POINT_DIAGONAL   3

// Line
#define CONTROL_POINT_ENDPOINT_TO 4
#define CONTROL_POINT_ENDPOINT_FROM 5
#define CONTROL_POINT_LINE       6

IMPLEMENT_DYNAMIC_CLASS(wxShapeTextLine, wxObject)
IMPLEMENT_DYNAMIC_CLASS(wxAttachmentPoint, wxObject)

wxShapeTextLine::wxShapeTextLine(double the_x, double the_y, const wxString& the_line)
{
  m_x = the_x; m_y = the_y; m_line = the_line;
}

wxShapeTextLine::~wxShapeTextLine()
{
}

IMPLEMENT_ABSTRACT_CLASS(wxShapeEvtHandler, wxObject)

wxShapeEvtHandler::wxShapeEvtHandler(wxShapeEvtHandler *prev, wxShape *shape)
{
  m_previousHandler = prev;
  m_handlerShape = shape;
}

wxShapeEvtHandler::~wxShapeEvtHandler()
{
}

// Creates a copy of this event handler.
wxShapeEvtHandler* wxShapeEvtHandler::CreateNewCopy()
{
  wxShapeEvtHandler* newObject = wxStaticCast(GetClassInfo()->CreateObject(), wxShapeEvtHandler);

  wxASSERT(newObject);
  wxASSERT(wxDynamicCast(newObject, wxShapeEvtHandler));

  newObject->m_previousHandler = newObject;

  CopyData(*newObject);

  return newObject;
}


void wxShapeEvtHandler::OnDelete()
{
  if (this != GetShape())
    delete this;
}

void wxShapeEvtHandler::OnDraw(wxDC& dc)
{
  if (m_previousHandler)
    m_previousHandler->OnDraw(dc);
}

void wxShapeEvtHandler::OnMoveLinks(wxDC& dc)
{
  if (m_previousHandler)
    m_previousHandler->OnMoveLinks(dc);
}

void wxShapeEvtHandler::OnMoveLink(wxDC& dc, bool moveControlPoints)
{
  if (m_previousHandler)
    m_previousHandler->OnMoveLink(dc, moveControlPoints);
}

void wxShapeEvtHandler::OnDrawContents(wxDC& dc)
{
  if (m_previousHandler)
    m_previousHandler->OnDrawContents(dc);
}

void wxShapeEvtHandler::OnDrawBranches(wxDC& dc, bool erase)
{
  if (m_previousHandler)
    m_previousHandler->OnDrawBranches(dc, erase);
}

void wxShapeEvtHandler::OnSize(double x, double y)
{
  if (m_previousHandler)
    m_previousHandler->OnSize(x, y);
}

bool wxShapeEvtHandler::OnMovePre(wxDC& dc, double x, double y, double old_x, double old_y, bool display)
{
  if (m_previousHandler)
    return m_previousHandler->OnMovePre(dc, x, y, old_x, old_y, display);
  else
    return true;
}

void wxShapeEvtHandler::OnMovePost(wxDC& dc, double x, double y, double old_x, double old_y, bool display)
{
  if (m_previousHandler)
    m_previousHandler->OnMovePost(dc, x, y, old_x, old_y, display);
}

void wxShapeEvtHandler::OnErase(wxDC& dc)
{
  if (m_previousHandler)
    m_previousHandler->OnErase(dc);
}

void wxShapeEvtHandler::OnEraseContents(wxDC& dc)
{
  if (m_previousHandler)
    m_previousHandler->OnEraseContents(dc);
}

void wxShapeEvtHandler::OnHighlight(wxDC& dc)
{
  if (m_previousHandler)
    m_previousHandler->OnHighlight(dc);
}

void wxShapeEvtHandler::OnLeftClick(double x, double y, int keys, int attachment)
{
  if (m_previousHandler)
    m_previousHandler->OnLeftClick(x, y, keys, attachment);
}

void wxShapeEvtHandler::OnLeftDoubleClick(double x, double y, int keys, int attachment)
{
  if (m_previousHandler)
    m_previousHandler->OnLeftDoubleClick(x, y, keys, attachment);
}

void wxShapeEvtHandler::OnRightClick(double x, double y, int keys, int attachment)
{
  if (m_previousHandler)
    m_previousHandler->OnRightClick(x, y, keys, attachment);
}

void wxShapeEvtHandler::OnDragLeft(bool draw, double x, double y, int keys, int attachment)
{
  if (m_previousHandler)
    m_previousHandler->OnDragLeft(draw, x, y, keys, attachment);
}

void wxShapeEvtHandler::OnBeginDragLeft(double x, double y, int keys, int attachment)
{
  if (m_previousHandler)
    m_previousHandler->OnBeginDragLeft(x, y, keys, attachment);
}

void wxShapeEvtHandler::OnEndDragLeft(double x, double y, int keys, int attachment)
{
  if (m_previousHandler)
    m_previousHandler->OnEndDragLeft(x, y, keys, attachment);
}

void wxShapeEvtHandler::OnDragRight(bool draw, double x, double y, int keys, int attachment)
{
  if (m_previousHandler)
    m_previousHandler->OnDragRight(draw, x, y, keys, attachment);
}

void wxShapeEvtHandler::OnBeginDragRight(double x, double y, int keys, int attachment)
{
  if (m_previousHandler)
    m_previousHandler->OnBeginDragRight(x, y, keys, attachment);
}

void wxShapeEvtHandler::OnEndDragRight(double x, double y, int keys, int attachment)
{
  if (m_previousHandler)
    m_previousHandler->OnEndDragRight(x, y, keys, attachment);
}

// Control points ('handles') redirect control to the actual shape, to make it easier
// to override sizing behaviour.
void wxShapeEvtHandler::OnSizingDragLeft(wxControlPoint* pt, bool draw, double x, double y, int keys, int attachment)
{
  if (m_previousHandler)
    m_previousHandler->OnSizingDragLeft(pt, draw, x, y, keys, attachment);
}

void wxShapeEvtHandler::OnSizingBeginDragLeft(wxControlPoint* pt, double x, double y, int keys, int attachment)
{
  if (m_previousHandler)
    m_previousHandler->OnSizingBeginDragLeft(pt, x, y, keys, attachment);
}

void wxShapeEvtHandler::OnSizingEndDragLeft(wxControlPoint* pt, double x, double y, int keys, int attachment)
{
  if (m_previousHandler)
    m_previousHandler->OnSizingEndDragLeft(pt, x, y, keys, attachment);
}

void wxShapeEvtHandler::OnDrawOutline(wxDC& dc, double x, double y, double w, double h)
{
  if (m_previousHandler)
    m_previousHandler->OnDrawOutline(dc, x, y, w, h);
}

void wxShapeEvtHandler::OnDrawControlPoints(wxDC& dc)
{
  if (m_previousHandler)
    m_previousHandler->OnDrawControlPoints(dc);
}

void wxShapeEvtHandler::OnEraseControlPoints(wxDC& dc)
{
  if (m_previousHandler)
    m_previousHandler->OnEraseControlPoints(dc);
}

// Can override this to prevent or intercept line reordering.
void wxShapeEvtHandler::OnChangeAttachment(int attachment, wxLineShape* line, wxList& ordering)
{
  if (m_previousHandler)
    m_previousHandler->OnChangeAttachment(attachment, line, ordering);
}

IMPLEMENT_ABSTRACT_CLASS(wxShape, wxShapeEvtHandler)

wxShape::wxShape(wxShapeCanvas *can)
{
  m_eventHandler = this;
  SetShape(this);
  m_id = 0;
  m_formatted = false;
  m_canvas = can;
  m_xpos = 0.0; m_ypos = 0.0;
  m_pen = g_oglBlackPen;
  m_brush = wxWHITE_BRUSH;
  m_font = g_oglNormalFont;
  m_textColour = wxT("BLACK");
  m_textColourName = wxT("BLACK");
  m_visible = false;
  m_selected = false;
  m_attachmentMode = ATTACHMENT_MODE_NONE;
  m_spaceAttachments = true;
  m_disableLabel = false;
  m_fixedWidth = false;
  m_fixedHeight = false;
  m_drawHandles = true;
  m_sensitivity = OP_ALL;
  m_draggable = true;
  m_parent = NULL;
  m_formatMode = FORMAT_CENTRE_HORIZ | FORMAT_CENTRE_VERT;
  m_shadowMode = SHADOW_NONE;
  m_shadowOffsetX = 6;
  m_shadowOffsetY = 6;
  m_shadowBrush = wxBLACK_BRUSH;
  m_textMarginX = 5;
  m_textMarginY = 5;
  m_regionName = wxT("0");
  m_centreResize = true;
  m_maintainAspectRatio = false;
  m_highlighted = false;
  m_rotation = 0.0;
  m_branchNeckLength = 10;
  m_branchStemLength = 10;
  m_branchSpacing = 10;
  m_branchStyle = BRANCHING_ATTACHMENT_NORMAL;

  // Set up a default region. Much of the above will be put into
  // the region eventually (the duplication is for compatibility)
  wxShapeRegion *region = new wxShapeRegion;
  m_regions.Append(region);
  region->SetName(wxT("0"));
  region->SetFont(g_oglNormalFont);
  region->SetFormatMode(FORMAT_CENTRE_HORIZ | FORMAT_CENTRE_VERT);
  region->SetColour(wxT("BLACK"));
}

wxShape::~wxShape()
{
  if (m_parent)
    m_parent->GetChildren().DeleteObject(this);

  ClearText();
  ClearRegions();
  ClearAttachments();

  if (m_canvas)
    m_canvas->RemoveShape(this);

  GetEventHandler()->OnDelete();
}

void wxShape::SetHighlight(bool hi, bool recurse)
{
  m_highlighted = hi;
  if (recurse)
  {
    for (wxObjectList::const_iterator it = m_children.begin();
         it != m_children.end();
         it++)
    {
      wxShape* child = wxStaticCast(*it, wxShape);
      child->SetHighlight(hi, recurse);
    }
  }
}

void wxShape::SetSensitivityFilter(int sens, bool recursive)
{
  if (sens & OP_DRAG_LEFT)
    m_draggable = true;
  else
    m_draggable = false;

  m_sensitivity = sens;
  if (recursive)
  {
    for (wxObjectList::const_iterator it = m_children.begin();
         it != m_children.end();
         it++)
    {
      wxShape* obj = wxStaticCast(*it, wxShape);
      obj->SetSensitivityFilter(sens, true);
    }
  }
}

void wxShape::SetDraggable(bool drag, bool recursive)
{
  m_draggable = drag;
  if (m_draggable)
    m_sensitivity |= OP_DRAG_LEFT;
  else
    if (m_sensitivity & OP_DRAG_LEFT)
      m_sensitivity = m_sensitivity - OP_DRAG_LEFT;

  if (recursive)
  {
    for (wxObjectList::const_iterator it = m_children.begin();
         it != m_children.end();
         it++)
    {
      wxShape* obj = wxStaticCast(*it, wxShape);
      obj->SetDraggable(drag, true);
    }
  }
}

void wxShape::SetDrawHandles(bool drawH)
{
  m_drawHandles = drawH;
  for (wxObjectList::const_iterator it = m_children.begin();
       it != m_children.end();
       it++)
  {
    wxShape* obj = wxStaticCast(*it, wxShape);
    obj->SetDrawHandles(drawH);
  }
}

void wxShape::SetShadowMode(int mode, bool redraw)
{
  if (redraw && GetCanvas())
  {
    wxClientDC dc(GetCanvas());
    GetCanvas()->PrepareDC(dc);
    Erase(dc);

    m_shadowMode = mode;

    Draw(dc);
  }
  else
  {
    m_shadowMode = mode;
  }
}

void wxShape::SetCanvas(wxShapeCanvas *theCanvas)
{
  m_canvas = theCanvas;
  for (wxObjectList::const_iterator it = m_children.begin();
       it != m_children.end();
       it++)
  {
    wxShape* child = wxStaticCast(*it, wxShape);
    child->SetCanvas(theCanvas);
  }
}

void wxShape::AddToCanvas(wxShapeCanvas *theCanvas, wxShape *addAfter)
{
  theCanvas->AddShape(this, addAfter);
  wxShape *lastImage = this;
  for (wxObjectList::const_iterator it = m_children.begin();
       it != m_children.end();
       it++)
  {
    wxShape* object = wxStaticCast(*it, wxShape);
    object->AddToCanvas(theCanvas, lastImage);
    lastImage = object;
  }
}

// Insert at front of canvas
void wxShape::InsertInCanvas(wxShapeCanvas *theCanvas)
{
  theCanvas->InsertShape(this);
  wxShape *lastImage = this;
  for (wxObjectList::const_iterator it = m_children.begin();
       it != m_children.end();
       it++)
  {
    wxShape* object = wxStaticCast(*it, wxShape);
    object->AddToCanvas(theCanvas, lastImage);
    lastImage = object;
  }
}

void wxShape::RemoveFromCanvas(wxShapeCanvas *theCanvas)
{
  if (Selected())
    Select(false);
  theCanvas->RemoveShape(this);
  for (wxObjectList::const_iterator it = m_children.begin();
       it != m_children.end();
       it++)
  {
    wxShape* object = wxStaticCast(*it, wxShape);
    object->RemoveFromCanvas(theCanvas);
  }
}

void wxShape::ClearAttachments()
{
  for (wxObjectList::const_iterator it = m_attachmentPoints.begin();
       it != m_attachmentPoints.end();
       it++)
  {
    wxAttachmentPoint* point = wxStaticCast(*it, wxAttachmentPoint);
    delete point;
  }
  m_attachmentPoints.Clear();
}

void wxShape::ClearText(int regionId)
{
  if (regionId == 0)
  {
    m_text.DeleteContents(true);
    m_text.Clear();
    m_text.DeleteContents(false);
  }
  wxObjectList::compatibility_iterator node = m_regions.Item(regionId);
  if (!node)
    return;
  wxShapeRegion* region = wxStaticCast(node->GetData(), wxShapeRegion);
  region->ClearText();
}

void wxShape::ClearRegions()
{
  for (wxObjectList::iterator it = m_regions.begin();
       it != m_regions.end();
       )
  {
    wxShapeRegion* region = wxStaticCast(*it, wxShapeRegion);
    delete region;
    m_regions.erase(it);
    it = m_regions.begin();
  }
}

void wxShape::AddRegion(wxShapeRegion *region)
{
  m_regions.Append(region);
}

void wxShape::SetDefaultRegionSize()
{
  wxObjectList::compatibility_iterator node = m_regions.GetFirst();
  if (!node) return;
  wxShapeRegion* region = wxStaticCast(node->GetData(), wxShapeRegion);
  double w, h;
  GetBoundingBoxMin(&w, &h);
  region->SetSize(w, h);
}

bool wxShape::HitTest(double x, double y, int *attachment, double *distance)
{
//  if (!sensitive)
//    return false;

  double width = 0.0, height = 0.0;
  GetBoundingBoxMin(&width, &height);
  if (fabs(width) < 4.0) width = 4.0;
  if (fabs(height) < 4.0) height = 4.0;

  width += (double)4.0; height += (double)4.0; // Allowance for inaccurate mousing

  double left = (double)(m_xpos - (width/2.0));
  double top = (double)(m_ypos - (height/2.0));
  double right = (double)(m_xpos + (width/2.0));
  double bottom = (double)(m_ypos + (height/2.0));

  int nearest_attachment = 0;

  // If within the bounding box, check the attachment points
  // within the object.

  if (x >= left && x <= right && y >= top && y <= bottom)
  {
    int n = GetNumberOfAttachments();
    double nearest = 999999.0;

    // GetAttachmentPosition[Edge] takes a logical attachment position,
    // i.e. if it's rotated through 90%, position 0 is East-facing.

    for (int i = 0; i < n; i++)
    {
      double xp, yp;
      if (GetAttachmentPositionEdge(i, &xp, &yp))
      {
        double l = (double)sqrt(((xp - x) * (xp - x)) +
                   ((yp - y) * (yp - y)));

        if (l < nearest)
        {
          nearest = l;
          nearest_attachment = i;
        }
      }
    }
    *attachment = nearest_attachment;
    *distance = nearest;
    return true;
  }
  else return false;
}

// Format a text string according to the region size, adding
// strings with positions to region text list

static bool GraphicsInSizeToContents = false; // Infinite recursion elimination
void wxShape::FormatText(wxDC& dc, const wxString& s, int i)
{
  double w, h;
  ClearText(i);

  if (m_regions.GetCount() < 1)
    return;
  wxObjectList::compatibility_iterator node = m_regions.Item(i);
  if (!node)
    return;

  wxShapeRegion* region = wxStaticCast(node->GetData(), wxShapeRegion);
  // region->SetText(s);  // don't set the formatted text yet, it will be done below
  region->m_regionText = s;
  dc.SetFont(* region->GetFont());

  region->GetSize(&w, &h);

  wxStringList *stringList = oglFormatText(dc, s, (w-2*m_textMarginX), (h-2*m_textMarginY), region->GetFormatMode());
  for (wxStringList::const_iterator it = stringList->begin();
       it != stringList->end();
       it++)
  {
    wxString s = *it;
    wxShapeTextLine* line = new wxShapeTextLine(0.0, 0.0, s);
    region->GetFormattedText().Append((wxObject*)line);
  }
  delete stringList;
  double actualW = w;
  double actualH = h;
  // Don't try to resize an object with more than one image (this case should be dealt
  // with by overriden handlers)
  if ((region->GetFormatMode() & FORMAT_SIZE_TO_CONTENTS) &&
      (region->GetFormattedText().GetCount() > 0) &&
      (m_regions.GetCount() == 1) && !GraphicsInSizeToContents)
  {
    oglGetCentredTextExtent(dc, &(region->GetFormattedText()), m_xpos, m_ypos, w, h, &actualW, &actualH);
    if ((actualW+2*m_textMarginX != w ) || (actualH+2*m_textMarginY != h))
    {
      // If we are a descendant of a composite, must make sure the composite gets
      // resized properly
      wxShape *topAncestor = GetTopAncestor();

      if (topAncestor != this)
      {
        // Make sure we don't recurse infinitely
        GraphicsInSizeToContents = true;

        wxCompositeShape *composite = wxStaticCast(topAncestor, wxCompositeShape);
        composite->Erase(dc);
        SetSize(actualW+2*m_textMarginX, actualH+2*m_textMarginY);
        Move(dc, m_xpos, m_ypos);
        composite->CalculateSize();
        if (composite->Selected())
        {
          composite->DeleteControlPoints(& dc);
          composite->MakeControlPoints();
          composite->MakeMandatoryControlPoints();
        }
        // Where infinite recursion might happen if we didn't stop it
        composite->Draw(dc);

        GraphicsInSizeToContents = false;
      }
      else
      {
        Erase(dc);
        SetSize(actualW+2*m_textMarginX, actualH+2*m_textMarginY);
        Move(dc, m_xpos, m_ypos);
      }
      SetSize(actualW+2*m_textMarginX, actualH+2*m_textMarginY);
      Move(dc, m_xpos, m_ypos);
      EraseContents(dc);
    }
  }
  oglCentreText(dc, &(region->GetFormattedText()), m_xpos, m_ypos, actualW-2*m_textMarginX, actualH-2*m_textMarginY, region->GetFormatMode());
  m_formatted = true;
}

void wxShape::Recentre(wxDC& dc)
{
  double w, h;
  GetBoundingBoxMin(&w, &h);

  int noRegions = m_regions.GetCount();
  for (int i = 0; i < noRegions; i++)
  {
    wxObjectList::compatibility_iterator node = m_regions.Item(i);
    if (node)
    {
      wxShapeRegion* region = wxStaticCast(node->GetData(), wxShapeRegion);
      oglCentreText(dc, &(region->GetFormattedText()), m_xpos, m_ypos, w-2*m_textMarginX, h-2*m_textMarginY, region->GetFormatMode());
    }
  }
}

bool wxShape::GetPerimeterPoint(double WXUNUSED(x1), double WXUNUSED(y1),
                                     double WXUNUSED(x2), double WXUNUSED(y2),
                                     double *WXUNUSED(x3), double *WXUNUSED(y3))
{
  return false;
}

void wxShape::SetPen(const wxPen *the_pen)
{
  m_pen = the_pen;
}

void wxShape::SetBrush(const wxBrush *the_brush)
{
  m_brush = the_brush;
}

// Get the top-most (non-division) ancestor, or self
wxShape *wxShape::GetTopAncestor()
{
  if (!GetParent())
    return this;

  if (wxDynamicCast(GetParent(), wxDivisionShape))
    return this;
  else return GetParent()->GetTopAncestor();
}

/*
 * Region functions
 *
 */
void wxShape::SetFont(wxFont *the_font, int regionId)
{
  m_font = the_font;
  wxObjectList::compatibility_iterator node = m_regions.Item(regionId);
  if (!node)
    return;
  wxShapeRegion* region = wxStaticCast(node->GetData(), wxShapeRegion);
  region->SetFont(the_font);
}

wxFont *wxShape::GetFont(int n) const
{
  wxObjectList::compatibility_iterator node = m_regions.Item(n);
  if (!node)
    return NULL;
  wxShapeRegion* region = wxStaticCast(node->GetData(), wxShapeRegion);
  return region->GetFont();
}

void wxShape::SetFormatMode(int mode, int regionId)
{
  wxObjectList::compatibility_iterator node = m_regions.Item(regionId);
  if (!node)
    return;
  wxShapeRegion* region = wxStaticCast(node->GetData(), wxShapeRegion);
  region->SetFormatMode(mode);
}

int wxShape::GetFormatMode(int regionId) const
{
  wxObjectList::compatibility_iterator node = m_regions.Item(regionId);
  if (!node)
    return 0;
  wxShapeRegion* region = wxStaticCast(node->GetData(), wxShapeRegion);
  return region->GetFormatMode();
}

void wxShape::SetTextColour(const wxString& the_colour, int regionId)
{
  m_textColour = wxTheColourDatabase->Find(the_colour);
  m_textColourName = the_colour;

  wxObjectList::compatibility_iterator node = m_regions.Item(regionId);
  if (!node)
    return;
  wxShapeRegion* region = wxStaticCast(node->GetData(), wxShapeRegion);
  region->SetColour(the_colour);
}

wxString wxShape::GetTextColour(int regionId) const
{
  wxObjectList::compatibility_iterator node = m_regions.Item(regionId);
  if (!node)
    return wxEmptyString;
  wxShapeRegion* region = wxStaticCast(node->GetData(), wxShapeRegion);
  return region->GetColour();
}

void wxShape::SetRegionName(const wxString& name, int regionId)
{
  wxObjectList::compatibility_iterator node = m_regions.Item(regionId);
  if (!node)
    return;
  wxShapeRegion* region = wxStaticCast(node->GetData(), wxShapeRegion);
  region->SetName(name);
}

wxString wxShape::GetRegionName(int regionId)
{
  wxObjectList::compatibility_iterator node = m_regions.Item(regionId);
  if (!node)
    return wxEmptyString;
  wxShapeRegion* region = wxStaticCast(node->GetData(), wxShapeRegion);
  return region->GetName();
}

int wxShape::GetRegionId(const wxString& name)
{
  int i = 0;
  for (wxObjectList::const_iterator it = m_regions.begin();
       it != m_regions.end();
       it++, i++)
  {
    wxShapeRegion* region = wxStaticCast(*it, wxShapeRegion);
    if (region->GetName() == name)
      return i;
  }
  return wxNOT_FOUND;
}

// Name all m_regions in all subimages recursively.
void wxShape::NameRegions(const wxString& parentName)
{
  int n = GetNumberOfTextRegions();
  wxString buff;
  for (int i = 0; i < n; i++)
  {
    if (!parentName.empty())
      buff << parentName << wxT(".") << i;
    else
      buff << i;
    SetRegionName(buff, i);
  }
  int j = 0;
  for (wxObjectList::const_iterator it = m_children.begin();
       it != m_children.end();
       it++)
  {
    buff.Empty();
    wxShape* child = wxStaticCast(*it, wxShape);
    if (!parentName.empty())
      buff << parentName << wxT(".") << j;
    else
      buff << j;
    child->NameRegions(buff);
    j ++;
  }
}

// Get a region by name, possibly looking recursively into composites.
wxShape *wxShape::FindRegion(const wxString& name, int *regionId)
{
  int id = GetRegionId(name);
  if (id > -1)
  {
    *regionId = id;
    return this;
  }

  for (wxObjectList::const_iterator it = m_children.begin();
       it != m_children.end();
       it++)
  {
    wxShape* child = wxStaticCast(*it, wxShape);
    wxShape *actualImage = child->FindRegion(name, regionId);
    if (actualImage)
      return actualImage;
  }
  return NULL;
}

// Finds all region names for this image (composite or simple).
// Supply empty string list.
void wxShape::FindRegionNames(wxStringList& list)
{
  int n = GetNumberOfTextRegions();
  for (int i = 0; i < n; i++)
  {
    wxString name(GetRegionName(i));
    list.Add(name.wx_str());
  }

  for (wxObjectList::const_iterator it = m_children.begin();
       it != m_children.end();
       it++)
  {
    wxShape* child = wxStaticCast(*it, wxShape);
    child->FindRegionNames(list);
  }
}

void wxShape::AssignNewIds()
{
//  if (m_id == 0)
  m_id = wxNewId();
  for (wxObjectList::const_iterator it = m_children.begin();
       it != m_children.end();
       it++)
  {
    wxShape* child = wxStaticCast(*it, wxShape);
    child->AssignNewIds();
  }
}

void wxShape::OnDraw(wxDC& WXUNUSED(dc))
{
}

void wxShape::OnMoveLinks(wxDC& dc)
{
  // Want to set the ends of all attached links
  // to point to/from this object

  for (wxObjectList::const_iterator it = m_lines.begin();
       it != m_lines.end();
       it++)
  {
    wxLineShape* line = wxStaticCast(*it, wxLineShape);
    line->GetEventHandler()->OnMoveLink(dc);
  }
}


void wxShape::OnDrawContents(wxDC& dc)
{
  double bound_x, bound_y;
  GetBoundingBoxMin(&bound_x, &bound_y);
    if (m_regions.GetCount() < 1) return;

    if (m_pen) dc.SetPen(* m_pen);

    wxShapeRegion* region = wxStaticCast(m_regions.GetFirst()->GetData(), wxShapeRegion);
    if (region->GetFont()) dc.SetFont(* region->GetFont());

    dc.SetTextForeground(region->GetActualColourObject());
    dc.SetBackgroundMode(wxTRANSPARENT);
    if (!m_formatted)
    {
      oglCentreText(dc, &(region->GetFormattedText()), m_xpos, m_ypos, bound_x-2*m_textMarginX, bound_y-2*m_textMarginY, region->GetFormatMode());
      m_formatted = true;
    }
    if (!GetDisableLabel())
    {
      oglDrawFormattedText(dc, &(region->GetFormattedText()), m_xpos, m_ypos, bound_x-2*m_textMarginX, bound_y-2*m_textMarginY, region->GetFormatMode());
    }
}

void wxShape::DrawContents(wxDC& dc)
{
  GetEventHandler()->OnDrawContents(dc);
}

void wxShape::OnSize(double WXUNUSED(x), double WXUNUSED(y))
{
}

bool wxShape::OnMovePre(wxDC& WXUNUSED(dc), double WXUNUSED(x), double WXUNUSED(y), double WXUNUSED(old_x), double WXUNUSED(old_y), bool WXUNUSED(display))
{
  return true;
}

void wxShape::OnMovePost(wxDC& WXUNUSED(dc), double WXUNUSED(x), double WXUNUSED(y), double WXUNUSED(old_x), double WXUNUSED(old_y), bool WXUNUSED(display))
{
}

void wxShape::OnErase(wxDC& dc)
{
  if (!m_visible)
    return;

  // Erase links
  for (wxObjectList::const_iterator it = m_lines.begin();
       it != m_lines.end();
       it++)
  {
    wxLineShape* line = wxStaticCast(*it, wxLineShape);
    line->GetEventHandler()->OnErase(dc);
  }
  GetEventHandler()->OnEraseContents(dc);
}

void wxShape::OnEraseContents(wxDC& dc)
{
  if (!m_visible)
    return;

  double maxX, maxY, minX, minY;
  double xp = GetX();
  double yp = GetY();
  GetBoundingBoxMin(&minX, &minY);
  GetBoundingBoxMax(&maxX, &maxY);
  double topLeftX = (double)(xp - (maxX / 2.0) - 2.0);
  double topLeftY = (double)(yp - (maxY / 2.0) - 2.0);

    int penWidth = 0;
    if (m_pen)
      penWidth = m_pen->GetWidth();

    dc.SetPen(GetBackgroundPen());
    dc.SetBrush(GetBackgroundBrush());

    dc.DrawRectangle(WXROUND(topLeftX - penWidth), WXROUND(topLeftY - penWidth),
                      WXROUND(maxX + penWidth*2.0 + 4.0), WXROUND(maxY + penWidth*2.0 + 4.0));
}

void wxShape::EraseLinks(wxDC& dc, int attachment, bool recurse)
{
  wxObjectList::const_iterator it;

  if (!m_visible)
    return;

  for (it = m_lines.begin();
       it != m_lines.end();
       it++)
  {
    wxLineShape* line = wxStaticCast(*it, wxLineShape);
    if (attachment == -1 || ((line->GetTo() == this && line->GetAttachmentTo() == attachment) ||
                             (line->GetFrom() == this && line->GetAttachmentFrom() == attachment)))
    {
      line->GetEventHandler()->OnErase(dc);
    }
  }
  if (recurse)
  {
    for (it = m_children.begin();
         it != m_children.end();
         it++)
    {
      wxShape* child = wxStaticCast(*it, wxShape);
      child->EraseLinks(dc, attachment, recurse);
    }
  }
}

void wxShape::DrawLinks(wxDC& dc, int attachment, bool recurse)
{
  wxObjectList::const_iterator it;

  if (!m_visible)
    return;

  for (it = m_lines.begin();
       it != m_lines.end();
       it++)
  {
    wxLineShape* line = wxStaticCast(*it, wxLineShape);
    if (attachment == -1 ||
        (line->GetTo() == this && line->GetAttachmentTo() == attachment) ||
        (line->GetFrom() == this && line->GetAttachmentFrom() == attachment))
    {
      line->Draw(dc);
    }
  }
  if (recurse)
  {
    for (it = m_children.begin();
         it != m_children.end();
         it++)
    {
      wxShape* child = wxStaticCast(*it, wxShape);
      child->DrawLinks(dc, attachment, recurse);
    }
  }
}

// Returns true if pt1 <= pt2 in the sense that one point comes before another on an
// edge of the shape.
// attachmentPoint is the attachment point (= side) in question.

// This is the default, rectangular implementation.
bool wxShape::AttachmentSortTest(int attachmentPoint, const wxRealPoint& pt1, const wxRealPoint& pt2)
{
    int physicalAttachment = LogicalToPhysicalAttachment(attachmentPoint);
    switch (physicalAttachment)
    {
        case 0:
        case 2:
        {
          return (pt1.x <= pt2.x) ;
        }
        case 1:
        case 3:
        {
          return (pt1.y <= pt2.y) ;
        }
    }

    return false;
}

bool wxShape::MoveLineToNewAttachment(wxDC& dc, wxLineShape *to_move,
                                       double x, double y)
{
  if (GetAttachmentMode() == ATTACHMENT_MODE_NONE)
      return false;

  int newAttachment, oldAttachment;
  double distance;

  // Is (x, y) on this object? If so, find the new attachment point
  // the user has moved the point to
  bool hit = HitTest(x, y, &newAttachment, &distance);
  if (!hit)
    return false;

  EraseLinks(dc);

  if (to_move->GetTo() == this)
    oldAttachment = to_move->GetAttachmentTo();
  else
    oldAttachment = to_move->GetAttachmentFrom();

  // The links in a new ordering.
  wxList newOrdering;

  // First, add all links to the new list.
  wxObjectList::const_iterator it;
  for (it = m_lines.begin();
       it != m_lines.end();
       it++)
  {
    newOrdering.Append(*it);
  }

  // Delete the line object from the list of links; we're going to move
  // it to another position in the list
  newOrdering.DeleteObject(to_move);

  double old_x = (double) -99999.9;
  double old_y = (double) -99999.9;

  bool found = false;
  size_t pos = 0;

  for (it = newOrdering.begin();
       (!found) && (it != newOrdering.end());
       it++, pos++)
  {
    wxLineShape* line = wxStaticCast(*it, wxLineShape);
    if ((line->GetTo() == this && oldAttachment == line->GetAttachmentTo()) ||
        (line->GetFrom() == this && oldAttachment == line->GetAttachmentFrom()))
    {
      double startX, startY, endX, endY;
      double xp, yp;
      line->GetEnds(&startX, &startY, &endX, &endY);
      if (line->GetTo() == this)
      {
        xp = endX;
        yp = endY;
      } else
      {
        xp = startX;
        yp = startY;
      }

      wxRealPoint thisPoint(xp, yp);
      wxRealPoint lastPoint(old_x, old_y);
      wxRealPoint newPoint(x, y);

      if (AttachmentSortTest(newAttachment, newPoint, thisPoint) && AttachmentSortTest(newAttachment, lastPoint, newPoint))
      {
        found = true;
        newOrdering.Insert(pos, to_move);
      }

      old_x = xp;
      old_y = yp;
    }
  }

  if (!found)
    newOrdering.Append(to_move);

  GetEventHandler()->OnChangeAttachment(newAttachment, to_move, newOrdering);

  return true;
}

void wxShape::OnChangeAttachment(int attachment, wxLineShape* line, wxList& ordering)
{
    if (line->GetTo() == this)
        line->SetAttachmentTo(attachment);
    else
        line->SetAttachmentFrom(attachment);

    ApplyAttachmentOrdering(ordering);

    wxClientDC dc(GetCanvas());
    GetCanvas()->PrepareDC(dc);

    MoveLinks(dc);

    if (!GetCanvas()->GetQuickEditMode()) GetCanvas()->Redraw(dc);
}

// Reorders the lines according to the given list.
void wxShape::ApplyAttachmentOrdering(wxList& linesToSort)
{
  // This is a temporary store of all the lines.
  wxList linesStore;

  wxObjectList::const_iterator it;
  for (it = m_lines.begin();
       it != m_lines.end();
       it++)
  {
    wxLineShape* line = wxStaticCast(*it, wxLineShape);
    linesStore.Append(line);
  }

  m_lines.Clear();

  for (it = linesToSort.begin();
       it != linesToSort.end();
       it++)
  {
    wxLineShape* line = wxStaticCast(*it, wxLineShape);
    if (linesStore.Member(line))
    {
      // Done this one
      linesStore.DeleteObject(line);
      m_lines.Append(line);
    }
  }

  // Now add any lines that haven't been listed in linesToSort.
  for (it = linesStore.begin();
       it != linesStore.end();
       it++)
  {
    wxLineShape* line = wxStaticCast(*it, wxLineShape);
    m_lines.Append(line);
  }
}

// Reorders the lines coming into the node image at this attachment
// position, in the order in which they appear in linesToSort.
// Any remaining lines not in the list will be added to the end.
void wxShape::SortLines(int attachment, wxList& linesToSort)
{
  // This is a temporary store of all the lines at this attachment
  // point. We'll tick them off as we've processed them.
  wxList linesAtThisAttachment;
  wxObjectList::compatibility_iterator node;
  wxObjectList::const_iterator it;

  for (node = m_lines.GetFirst();
       node;
       )
  {
    wxLineShape* line = wxStaticCast(node->GetData(), wxLineShape);
    wxObjectList::compatibility_iterator next = node->GetNext();
    if ((line->GetTo() == this && line->GetAttachmentTo() == attachment) ||
        (line->GetFrom() == this && line->GetAttachmentFrom() == attachment))
    {
      linesAtThisAttachment.Append(line);
      delete node->GetData();
      node = next;
    }
    else
    {
       node = node->GetNext();
    }
  }

  for (it = linesToSort.begin();
       it != linesToSort.end();
       it++)
  {
    wxLineShape* line = wxStaticCast(*it, wxLineShape);
    if (linesAtThisAttachment.Member(line))
    {
      // Done this one
      linesAtThisAttachment.DeleteObject(line);
      m_lines.Append(line);
    }
  }

  // Now add any lines that haven't been listed in linesToSort.
  for (it = linesAtThisAttachment.begin();
       it != linesAtThisAttachment.end();
       it++)
  {
    wxLineShape* line = wxStaticCast(*it, wxLineShape);
    m_lines.Append(line);
  }
}

void wxShape::OnHighlight(wxDC& WXUNUSED(dc))
{
}

void wxShape::OnLeftClick(double x, double y, int keys, int attachment)
{
  if ((m_sensitivity & OP_CLICK_LEFT) != OP_CLICK_LEFT)
  {
    attachment = 0;
    double dist;
    if (m_parent)
    {
      m_parent->HitTest(x, y, &attachment, &dist);
      m_parent->GetEventHandler()->OnLeftClick(x, y, keys, attachment);
    }
    return;
  }
}

void wxShape::OnRightClick(double x, double y, int keys, int attachment)
{
  if ((m_sensitivity & OP_CLICK_RIGHT) != OP_CLICK_RIGHT)
  {
    attachment = 0;
    double dist;
    if (m_parent)
    {
      m_parent->HitTest(x, y, &attachment, &dist);
      m_parent->GetEventHandler()->OnRightClick(x, y, keys, attachment);
    }
    return;
  }
}

double DragOffsetX = 0.0;
double DragOffsetY = 0.0;

void wxShape::OnDragLeft(bool draw, double x, double y, int keys, int attachment)
{
  if ((m_sensitivity & OP_DRAG_LEFT) != OP_DRAG_LEFT)
  {
    attachment = 0;
    double dist;
    if (m_parent)
    {
      m_parent->HitTest(x, y, &attachment, &dist);
      m_parent->GetEventHandler()->OnDragLeft(draw, x, y, keys, attachment);
    }
    return;
  }

  wxClientDC dc(GetCanvas());
  GetCanvas()->PrepareDC(dc);

  dc.SetLogicalFunction(OGLRBLF);

  wxPen dottedPen(*wxBLACK, 1, wxPENSTYLE_DOT);
  dc.SetPen(dottedPen);
  dc.SetBrush(* wxTRANSPARENT_BRUSH);

  double xx, yy;
  xx = x + DragOffsetX;
  yy = y + DragOffsetY;

  m_canvas->Snap(&xx, &yy);
//  m_xpos = xx; m_ypos = yy;
  double w, h;
  GetBoundingBoxMax(&w, &h);
  GetEventHandler()->OnDrawOutline(dc, xx, yy, w, h);
}

void wxShape::OnBeginDragLeft(double x, double y, int keys, int attachment)
{
  if ((m_sensitivity & OP_DRAG_LEFT) != OP_DRAG_LEFT)
  {
    attachment = 0;
    double dist;
    if (m_parent)
    {
      m_parent->HitTest(x, y, &attachment, &dist);
      m_parent->GetEventHandler()->OnBeginDragLeft(x, y, keys, attachment);
    }
    return;
  }

  DragOffsetX = m_xpos - x;
  DragOffsetY = m_ypos - y;

  wxClientDC dc(GetCanvas());
  GetCanvas()->PrepareDC(dc);

  // New policy: don't erase shape until end of drag.
//  Erase(dc);

  double xx, yy;
  xx = x + DragOffsetX;
  yy = y + DragOffsetY;
  m_canvas->Snap(&xx, &yy);
//  m_xpos = xx; m_ypos = yy;
  dc.SetLogicalFunction(OGLRBLF);

  wxPen dottedPen(*wxBLACK, 1, wxPENSTYLE_DOT);
  dc.SetPen(dottedPen);
  dc.SetBrush((* wxTRANSPARENT_BRUSH));

  double w, h;
  GetBoundingBoxMax(&w, &h);
  GetEventHandler()->OnDrawOutline(dc, xx, yy, w, h);
  m_canvas->CaptureMouse();
}

void wxShape::OnEndDragLeft(double x, double y, int keys, int attachment)
{
  if (!m_draggable)
    return;
  m_canvas->ReleaseMouse();
  if ((m_sensitivity & OP_DRAG_LEFT) != OP_DRAG_LEFT)
  {
    attachment = 0;
    double dist;
    if (m_parent)
    {
      m_parent->HitTest(x, y, &attachment, &dist);
      m_parent->GetEventHandler()->OnEndDragLeft(x, y, keys, attachment);
    }
    return;
  }

  wxClientDC dc(GetCanvas());
  GetCanvas()->PrepareDC(dc);

  dc.SetLogicalFunction(wxCOPY);

  double xx = x + DragOffsetX;
  double yy = y + DragOffsetY;
  m_canvas->Snap(&xx, &yy);
//  canvas->Snap(&m_xpos, &m_ypos);

  // New policy: erase shape at end of drag.
  Erase(dc);

  Move(dc, xx, yy);
  if (m_canvas && !m_canvas->GetQuickEditMode()) m_canvas->Redraw(dc);
}

void wxShape::OnDragRight(bool draw, double x, double y, int keys, int attachment)
{
  if ((m_sensitivity & OP_DRAG_RIGHT) != OP_DRAG_RIGHT)
  {
    attachment = 0;
    double dist;
    if (m_parent)
    {
      m_parent->HitTest(x, y, &attachment, &dist);
      m_parent->GetEventHandler()->OnDragRight(draw, x, y, keys, attachment);
    }
    return;
  }
}

void wxShape::OnBeginDragRight(double x, double y, int keys, int attachment)
{
  if ((m_sensitivity & OP_DRAG_RIGHT) != OP_DRAG_RIGHT)
  {
    attachment = 0;
    double dist;
    if (m_parent)
    {
      m_parent->HitTest(x, y, &attachment, &dist);
      m_parent->GetEventHandler()->OnBeginDragRight(x, y, keys, attachment);
    }
    return;
  }
}

void wxShape::OnEndDragRight(double x, double y, int keys, int attachment)
{
  if ((m_sensitivity & OP_DRAG_RIGHT) != OP_DRAG_RIGHT)
  {
    attachment = 0;
    double dist;
    if (m_parent)
    {
      m_parent->HitTest(x, y, &attachment, &dist);
      m_parent->GetEventHandler()->OnEndDragRight(x, y, keys, attachment);
    }
    return;
  }
}

void wxShape::OnDrawOutline(wxDC& dc, double x, double y, double w, double h)
{
  double top_left_x = (double)(x - w/2.0);
  double top_left_y = (double)(y - h/2.0);
  double top_right_x = (double)(top_left_x + w);
  double top_right_y = (double)top_left_y;
  double bottom_left_x = (double)top_left_x;
  double bottom_left_y = (double)(top_left_y + h);
  double bottom_right_x = (double)top_right_x;
  double bottom_right_y = (double)bottom_left_y;

  wxPoint points[5];
  points[0].x = WXROUND(top_left_x); points[0].y = WXROUND(top_left_y);
  points[1].x = WXROUND(top_right_x); points[1].y = WXROUND(top_right_y);
  points[2].x = WXROUND(bottom_right_x); points[2].y = WXROUND(bottom_right_y);
  points[3].x = WXROUND(bottom_left_x); points[3].y = WXROUND(bottom_left_y);
  points[4].x = WXROUND(top_left_x); points[4].y = WXROUND(top_left_y);

  dc.DrawLines(5, points);
}

void wxShape::Attach(wxShapeCanvas *can)
{
  m_canvas = can;
}

void wxShape::Detach()
{
  m_canvas = NULL;
}

void wxShape::Move(wxDC& dc, double x, double y, bool display)
{
  double old_x = m_xpos;
  double old_y = m_ypos;

  if (!GetEventHandler()->OnMovePre(dc, x, y, old_x, old_y, display))
  {
//    m_xpos = old_x;
//    m_ypos = old_y;
    return;
  }

  m_xpos = x; m_ypos = y;

  ResetControlPoints();

  if (display)
    Draw(dc);

  MoveLinks(dc);

  GetEventHandler()->OnMovePost(dc, x, y, old_x, old_y, display);
}

void wxShape::MoveLinks(wxDC& dc)
{
  GetEventHandler()->OnMoveLinks(dc);
}


void wxShape::Draw(wxDC& dc)
{
  if (m_visible)
  {
    GetEventHandler()->OnDraw(dc);
    GetEventHandler()->OnDrawContents(dc);
    GetEventHandler()->OnDrawControlPoints(dc);
    GetEventHandler()->OnDrawBranches(dc);
  }
}

void wxShape::Flash()
{
    if (GetCanvas())
    {
        wxClientDC dc(GetCanvas());
        GetCanvas()->PrepareDC(dc);

        dc.SetLogicalFunction(OGLRBLF);
        Draw(dc);
        dc.SetLogicalFunction(wxCOPY);
        Draw(dc);
    }
}

void wxShape::Show(bool show)
{
  m_visible = show;
  for (wxObjectList::const_iterator it = m_children.begin();
       it != m_children.end();
       it++)
  {
    wxShape* image = wxStaticCast(*it, wxShape);
    image->Show(show);
  }
}

void wxShape::Erase(wxDC& dc)
{
  GetEventHandler()->OnErase(dc);
  GetEventHandler()->OnEraseControlPoints(dc);
  GetEventHandler()->OnDrawBranches(dc, true);
}

void wxShape::EraseContents(wxDC& dc)
{
  GetEventHandler()->OnEraseContents(dc);
}

void wxShape::AddText(const wxString& string)
{
  wxObjectList::compatibility_iterator node = m_regions.GetFirst();
  if (!node)
    return;
  wxShapeRegion* region = wxStaticCast(node->GetData(), wxShapeRegion);
  region->ClearText();
  wxShapeTextLine *new_line =
      new wxShapeTextLine(0.0, 0.0, string);
  region->GetFormattedText().Append(new_line);

  m_formatted = false;
}

void wxShape::SetSize(double x, double y, bool WXUNUSED(recursive))
{
  SetAttachmentSize(x, y);
  SetDefaultRegionSize();
}

void wxShape::SetAttachmentSize(double w, double h)
{
  double scaleX;
  double scaleY;
  double width, height;
  GetBoundingBoxMin(&width, &height);
  if (width == 0.0)
    scaleX = 1.0;
  else scaleX = w/width;
  if (height == 0.0)
    scaleY = 1.0;
  else scaleY = h/height;

  for (wxObjectList::const_iterator it = m_attachmentPoints.begin();
       it != m_attachmentPoints.end();
       it++)
  {
    wxAttachmentPoint* point = wxStaticCast(*it, wxAttachmentPoint);
    point->m_x = (double)(point->m_x * scaleX);
    point->m_y = (double)(point->m_y * scaleY);
  }
}

// Add line FROM this object
void wxShape::AddLine(wxLineShape *line, wxShape *other,
                            int attachFrom, int attachTo,
                            // The line ordering
                            int positionFrom, int positionTo)
{
    if (positionFrom == -1)
    {
        if (!m_lines.Member(line))
            m_lines.Append(line);
    }
    else
    {
        // Don't preserve old ordering if we have new ordering instructions
        m_lines.DeleteObject(line);
        if (positionFrom < (int) m_lines.GetCount())
        {
            wxObjectList::compatibility_iterator node = m_lines.Item(positionFrom);
            m_lines.Insert(node, line);
        }
        else
            m_lines.Append(line);
    }

    if (positionTo == -1)
    {
        if (!other->m_lines.Member(line))
            other->m_lines.Append(line);
    }
    else
    {
        // Don't preserve old ordering if we have new ordering instructions
        other->m_lines.DeleteObject(line);
        if (positionTo < (int) other->m_lines.GetCount())
        {
            wxObjectList::compatibility_iterator node = other->m_lines.Item(positionTo);
            other->m_lines.Insert(node, line);
        }
        else
            other->m_lines.Append(line);
    }
#if 0
    // Wrong: doesn't preserve ordering of shape already linked
    m_lines.DeleteObject(line);
    other->m_lines.DeleteObject(line);

    if (positionFrom == -1)
        m_lines.Append(line);
    else
    {
        if (positionFrom < m_lines.GetCount())
        {
            wxNode* node = m_lines.Item(positionFrom);
            m_lines.Insert(node, line);
        }
        else
            m_lines.Append(line);
    }

    if (positionTo == -1)
        other->m_lines.Append(line);
    else
    {
        if (positionTo < other->m_lines.GetCount())
        {
            wxNode* node = other->m_lines.Item(positionTo);
            other->m_lines.Insert(node, line);
        }
        else
            other->m_lines.Append(line);
    }
#endif

    line->SetFrom(this);
    line->SetTo(other);
    line->SetAttachments(attachFrom, attachTo);
}

void wxShape::RemoveLine(wxLineShape *line)
{
  if (line->GetFrom() == this)
    line->GetTo()->m_lines.DeleteObject(line);
  else
   line->GetFrom()->m_lines.DeleteObject(line);

  m_lines.DeleteObject(line);
}

void wxShape::WriteAttributes(wxXmlNode*clause) const
{
  clause->AddAttribute(wxT("type"), GetClassInfo()->GetClassName());
  clause->AddAttribute(wxT("id"), wxString::Format(wxT("%d"), (int)m_id));

  if (m_pen)
  {
    int penWidth = m_pen->GetWidth();
    int penStyle = m_pen->GetStyle();
    if (penWidth != 1)
      clause->AddAttribute(wxT("pen_width"), wxString::Format(wxT("%d"), penWidth));
    if (penStyle != wxSOLID)
      clause->AddAttribute(wxT("pen_style"), wxString::Format(wxT("%d"), penStyle));

    wxString penColour = wxTheColourDatabase->FindName(m_pen->GetColour());
    if (penColour == wxEmptyString)
    {
      wxString hex(oglColourToHex(m_pen->GetColour()));
      hex = wxString(wxT("#")) + hex;
      clause->AddAttribute(wxT("pen_colour"), hex);
    }
    else if (penColour != wxT("BLACK"))
      clause->AddAttribute(wxT("pen_colour"), penColour);
  }

  if (m_brush)
  {
    wxString brushColour = wxTheColourDatabase->FindName(m_brush->GetColour());

    if (brushColour == wxEmptyString)
    {
      wxString hex(oglColourToHex(m_brush->GetColour()));
      hex = wxString(wxT("#")) + hex;
      clause->AddAttribute(wxT("brush_colour"), hex);
    }
    else if (brushColour != wxT("WHITE"))
      clause->AddAttribute(wxT("brush_colour"), brushColour);

    if (m_brush->GetStyle() != wxBRUSHSTYLE_SOLID)
      clause->AddAttribute(wxT("brush_style"), wxString::Format(wxT("%d"), m_brush->GetStyle()));
  }

  // Output line ids

  size_t n_lines = m_lines.GetCount();
  if (n_lines)
  {
    wxXmlNode* list = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("arcs"));
    int i = 0;
    for (wxObjectList::const_iterator it = m_lines.begin();
         it != m_lines.end();
         it++, i++)
    {
      wxShape* line = wxStaticCast(*it, wxShape);
      //wxExpr *id_expr = new wxExpr(line->GetId());
      list->AddAttribute(wxString::Format(wxT("%d"), i), wxString::Format(wxT("%d"), (int)line->GetId()));
    }
    clause->AddChild(list);
  }

  // Miscellaneous members
  if (m_attachmentMode != 0)
    clause->AddAttribute(wxT("use_attachments"), wxString::Format(wxT("%d"), m_attachmentMode));
  if (m_sensitivity != OP_ALL)
    clause->AddAttribute(wxT("sensitivity"), wxString::Format(wxT("%d"), m_sensitivity));
  if (!m_spaceAttachments)
    clause->AddAttribute(wxT("space_attachments"), wxString::Format(wxT("%d"), m_spaceAttachments));
  if (m_fixedWidth)
    clause->AddAttribute(wxT("fixed_width"), wxString::Format(wxT("%d"), m_fixedWidth));
  if (m_fixedHeight)
    clause->AddAttribute(wxT("fixed_height"), wxString::Format(wxT("%d"), m_fixedHeight));
  if (m_shadowMode != SHADOW_NONE)
    clause->AddAttribute(wxT("shadow_mode"), wxString::Format(wxT("%d"), m_shadowMode));
  if (m_centreResize != true)
    clause->AddAttribute(wxT("centre_resize"), wxString::Format(wxT("%d"), 0));
  clause->AddAttribute(wxT("maintain_aspect_ratio"), wxString::Format(wxT("%d"), m_maintainAspectRatio));
  if (m_highlighted != false)
    clause->AddAttribute(wxT("hilite"), wxString::Format(wxT("%d"), m_highlighted));

  if (m_parent) // For composite objects
    clause->AddAttribute(wxT("parent"), wxString::Format(wxT("%d"), (int)m_parent->GetId()));

  if (m_rotation != 0.0)
    clause->AddAttribute(wxT("rotation"), wxString::Format(wxT("%g"), m_rotation));

  if (!wxDynamicCast(this, wxLineShape))
  {
    clause->AddAttribute(wxT("neck_length"), wxString::Format(wxT("%d"), m_branchNeckLength));
    clause->AddAttribute(wxT("stem_length"), wxString::Format(wxT("%d"), m_branchStemLength));
    clause->AddAttribute(wxT("branch_spacing"), wxString::Format(wxT("%d"), m_branchSpacing));
    clause->AddAttribute(wxT("branch_style"), wxString::Format(wxT("%d"), (int)m_branchStyle));
  }

  // Write user-defined attachment points, if any
  if (m_attachmentPoints.GetCount())
  {
    wxXmlNode* attachmentList = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("user_attachments"));
    for (wxObjectList::const_iterator it = m_attachmentPoints.begin();
         it != m_attachmentPoints.end();
         it++)
    {
      wxAttachmentPoint* point = wxStaticCast(*it, wxAttachmentPoint);
      wxXmlNode* pointExpr = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("point"));
      pointExpr->AddAttribute(wxT("id"), wxString::Format(wxT("%d"), point->m_id));
      pointExpr->AddAttribute(wxT("x"), wxString::Format(wxT("%g"), point->m_x));
      pointExpr->AddAttribute(wxT("y"), wxString::Format(wxT("%g"), point->m_y));
      attachmentList->AddChild(pointExpr);
    }
    clause->AddChild(attachmentList);
  }

  // Write text regions
  WriteRegions(clause);
}

void wxShape::WriteRegions(wxXmlNode*clause) const
{
  // Output regions as region1 = (...), region2 = (...), etc
  // and formatted text as text1 = (...), text2 = (...) etc.
  int regionNo = 1;
  wxChar regionNameBuf[20];
  wxChar textNameBuf[20];
  for (wxObjectList::const_iterator it = m_regions.begin();
       it != m_regions.end();
       it++, regionNo++)
  {
    wxShapeRegion* region = wxStaticCast(*it, wxShapeRegion);
    wxSprintf(regionNameBuf, wxT("region%d"), regionNo);
    wxSprintf(textNameBuf, wxT("text%d"), regionNo);

    // Original text and region attributes:
    // region1 = (regionName regionText x y width height minWidth minHeight proportionX proportionY
    //            formatMode fontSize fontFamily fontStyle fontWeight textColour)
    wxXmlNode* regionExpr = new wxXmlNode(wxXML_ELEMENT_NODE, regionNameBuf);
    regionExpr->AddAttribute(wxT("name"), region->m_regionName);
    regionExpr->AddAttribute(wxT("text"), region->m_regionText);

    regionExpr->AddAttribute(wxT("x"), wxString::Format(wxT("%g"), region->m_x));
    regionExpr->AddAttribute(wxT("y"), wxString::Format(wxT("%g"), region->m_y));
    regionExpr->AddAttribute(wxT("w"), wxString::Format(wxT("%g"), region->GetWidth()));
    regionExpr->AddAttribute(wxT("h"), wxString::Format(wxT("%g"), region->GetHeight()));

    regionExpr->AddAttribute(wxT("minw"), wxString::Format(wxT("%g"), region->m_minWidth));
    regionExpr->AddAttribute(wxT("minh"), wxString::Format(wxT("%g"), region->m_minHeight));
    regionExpr->AddAttribute(wxT("propx"), wxString::Format(wxT("%g"), region->m_regionProportionX));
    regionExpr->AddAttribute(wxT("propy"), wxString::Format(wxT("%g"), region->m_regionProportionY));

    regionExpr->AddAttribute(wxT("mode"), wxString::Format(wxT("%d"), region->m_formatMode));

    regionExpr->AddAttribute(wxT("pt"), wxString::Format(wxT("%d"), region->m_font ? region->m_font->GetPointSize() : 10));
    regionExpr->AddAttribute(wxT("family"), wxString::Format(wxT("%d"), region->m_font ? region->m_font->GetFamily() : wxDEFAULT));
    regionExpr->AddAttribute(wxT("style"), wxString::Format(wxT("%d"), region->m_font ? region->m_font->GetStyle() : wxDEFAULT));
    regionExpr->AddAttribute(wxT("weight"), wxString::Format(wxT("%d"), region->m_font ? region->m_font->GetWeight() : wxNORMAL));
    regionExpr->AddAttribute(wxT("clr"), region->m_textColour);

    // New members for pen colour/style
    regionExpr->AddAttribute(wxT("penclr"), region->m_penColour);
    regionExpr->AddAttribute(wxT("penstyle"), wxString::Format(wxT("%d"), region->m_penStyle));

    // Formatted text:
    // text1 = ((x y string) (x y string) ...)
    wxXmlNode* textExpr = new wxXmlNode(wxXML_ELEMENT_NODE, textNameBuf);

    for (wxObjectList::const_iterator text_it = region->m_formattedText.begin();
         text_it != region->m_formattedText.end();
         text_it++)
    {
      wxShapeTextLine* line = wxStaticCast(*text_it, wxShapeTextLine);
      wxXmlNode* list2 = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("text"));
      list2->AddAttribute(wxT("x"), wxString::Format(wxT("%g"), line->GetX()));
      list2->AddAttribute(wxT("y"), wxString::Format(wxT("%g"), line->GetY()));
      list2->AddAttribute(wxT("text"), line->GetText());
      textExpr->AddChild(list2);
    }

    // Now add both attributes to the clause
    clause->AddChild(regionExpr);
    clause->AddChild(textExpr);
  }
}

wxXmlNode* xmlFindChild(wxXmlNode* root, const wxString& childname, int id = wxNOT_FOUND)
{
   for (wxXmlNode* node = root->GetChildren(); node; node = node->GetNext())
   {
      if (node->GetName() == childname)
      {
         long iVal = 0;
         if ( (id != wxNOT_FOUND) && node->GetAttribute(wxT("id"), wxEmptyString).ToLong(&iVal))
         {
            if (id != iVal) continue;
         }
         return node;
      }
   }
   return NULL;
}

void wxShape::ReadAttributes(wxXmlNode*clause)
{
  long iVal;
  if (clause->GetAttribute(wxT("id"), wxEmptyString).ToLong(&iVal)) m_id = iVal;
  wxRegisterId(m_id);

  if (clause->GetAttribute(wxT("x"), wxEmptyString).ToLong(&iVal)) m_xpos = iVal;
  if (clause->GetAttribute(wxT("y"), wxEmptyString).ToLong(&iVal)) m_ypos = iVal;

  wxString pen_string = wxEmptyString;
  wxString brush_string = wxEmptyString;
  int pen_width = 1;
  wxPenStyle pen_style = wxPENSTYLE_SOLID;
  wxBrushStyle brush_style = wxBRUSHSTYLE_SOLID;
  m_attachmentMode = ATTACHMENT_MODE_NONE;

  pen_string = clause->GetAttribute(wxT("pen_colour"), wxEmptyString);
  m_textColourName = clause->GetAttribute(wxT("text_colour"), wxEmptyString);

  SetTextColour(m_textColourName);

  m_regionName = clause->GetAttribute(wxT("region_name"), wxEmptyString);

  brush_string = clause->GetAttribute(wxT("brush_colour"), wxEmptyString);
  if (clause->GetAttribute(wxT("pen_width"), wxEmptyString).ToLong(&iVal)) pen_width = iVal;
  if (clause->GetAttribute(wxT("pen_style"), wxEmptyString).ToLong(&iVal)) pen_style = (wxPenStyle)iVal;
  if (clause->GetAttribute(wxT("brush_style"), wxEmptyString).ToLong(&iVal)) brush_style = (wxBrushStyle)iVal;
  if (clause->GetAttribute(wxT("use_attachments"), wxEmptyString).ToLong(&iVal)) m_attachmentMode = iVal;
  if (clause->GetAttribute(wxT("sensitivity"), wxEmptyString).ToLong(&iVal)) m_sensitivity = iVal;
  if (clause->GetAttribute(wxT("space_attachments"), wxEmptyString).ToLong(&iVal)) m_spaceAttachments = (iVal != 0);
  if (clause->GetAttribute(wxT("fixed_width"), wxEmptyString).ToLong(&iVal)) m_fixedWidth = (iVal != 0);
  if (clause->GetAttribute(wxT("fixed_height"), wxEmptyString).ToLong(&iVal)) m_fixedHeight = (iVal != 0);
  if (clause->GetAttribute(wxT("format_mode"), wxEmptyString).ToLong(&iVal)) m_formatMode = iVal;
  if (clause->GetAttribute(wxT("shadow_mode"), wxEmptyString).ToLong(&iVal)) m_shadowMode = iVal;
  if (clause->GetAttribute(wxT("neck_length"), wxEmptyString).ToLong(&iVal)) m_branchNeckLength = iVal;
  if (clause->GetAttribute(wxT("stem_length"), wxEmptyString).ToLong(&iVal)) m_branchStemLength = iVal;
  if (clause->GetAttribute(wxT("branch_spacing"), wxEmptyString).ToLong(&iVal)) m_branchSpacing = iVal;
  if (clause->GetAttribute(wxT("branch_style"), wxEmptyString).ToLong(&iVal)) m_branchStyle = iVal;
  if (clause->GetAttribute(wxT("centre_resize"), wxEmptyString).ToLong(&iVal)) m_centreResize = (iVal != 0);
  if (clause->GetAttribute(wxT("maintain_aspect_ratio"), wxEmptyString).ToLong(&iVal)) m_maintainAspectRatio = (iVal != 0);
  if (clause->GetAttribute(wxT("hilite"), wxEmptyString).ToLong(&iVal)) m_highlighted = (iVal != 0);
  if (clause->GetAttribute(wxT("rotation"), wxEmptyString).ToLong(&iVal)) m_rotation = iVal;

  if (pen_string == wxEmptyString)
    pen_string = wxT("BLACK");
  if (brush_string == wxEmptyString)
    brush_string = wxT("WHITE");

  if (pen_string.GetChar(0) == '#')
  {
    wxColour col(oglHexToColour(pen_string.After('#')));
    m_pen = wxThePenList->FindOrCreatePen(col, pen_width, pen_style);
  }
  else
    m_pen = wxThePenList->FindOrCreatePen(pen_string, pen_width, pen_style);

  if (!m_pen)
    m_pen = wxBLACK_PEN;

  if (brush_string.GetChar(0) == '#')
  {
    wxColour col(oglHexToColour(brush_string.After('#')));
    m_brush = wxTheBrushList->FindOrCreateBrush(col, brush_style);
  }
  else
    m_brush = wxTheBrushList->FindOrCreateBrush(brush_string, brush_style);

  if (!m_brush)
    m_brush = wxWHITE_BRUSH;

  iVal = 10;
  clause->GetAttribute(wxT("point_size"), wxEmptyString).ToLong(&iVal);
  SetFont(oglMatchFont(iVal));

  // Read user-defined attachment points, if any
  wxXmlNode *attachmentList = ::xmlFindChild(clause, wxT("user_attachments"));
  if (attachmentList)
  {
    for (wxXmlNode* node = attachmentList->GetChildren(); node; node = node->GetNext())
    {
      wxString idExpr = node->GetAttribute(wxT("id"), wxEmptyString);
      wxString xExpr = node->GetAttribute(wxT("x"), wxEmptyString);
      wxString yExpr = node->GetAttribute(wxT("y"), wxEmptyString);
      if (idExpr.Length() && xExpr.Length() && yExpr.Length())
      {
        wxAttachmentPoint* point = new wxAttachmentPoint();
        idExpr.ToLong(&iVal);
        point->m_id = iVal;
        xExpr.ToLong(&iVal);
        point->m_x = iVal;
        yExpr.ToLong(&iVal);
        point->m_y = iVal;
        m_attachmentPoints.Append(point);
      }
    }
  }

  // Read text regions
  ReadRegions(clause);
}

void wxShape::ReadRegions(wxXmlNode*clause)
{
  ClearRegions();

  // region1 = (regionName regionText x y width height minWidth minHeight proportionX proportionY
  //            formatMode fontSize fontFamily fontStyle fontWeight textColour)
  int regionNo = 1;
  wxChar regionNameBuf[20];
  wxChar textNameBuf[20];

  wxXmlNode *regionExpr;
  wxXmlNode *textExpr = NULL;
  wxSprintf(regionNameBuf, wxT("region%d"), regionNo);
  wxSprintf(textNameBuf, wxT("text%d"), regionNo);

  m_formatted = true;  // Assume text is formatted unless we prove otherwise

  for (;;)
  {
    regionExpr = ::xmlFindChild(clause, regionNameBuf);
    if (NULL == regionExpr)
    {
       break;
    }
    /*
     * Get the region information
     *
     */

    wxString regionName = wxEmptyString;
    wxString regionText = wxEmptyString;
    double x = 0.0;
    double y = 0.0;
    double width = 0.0;
    double height = 0.0;
    double minWidth = 5.0;
    double minHeight = 5.0;
    double m_regionProportionX = -1.0;
    double m_regionProportionY = -1.0;
    int formatMode = FORMAT_NONE;
    int fontSize = 10;
    wxFontFamily fontFamily = wxFONTFAMILY_SWISS;
    wxFontStyle fontStyle = wxFONTSTYLE_NORMAL;
    wxFontWeight fontWeight = wxFONTWEIGHT_NORMAL;
    wxString regionTextColour = wxEmptyString;
    wxString penColour = wxEmptyString;
    wxPenStyle penStyle = wxPENSTYLE_SOLID;

    //if (regionExpr->Type() == wxExprList)
    {
      wxString nameExpr = regionExpr->GetAttribute(wxT("name"), wxEmptyString);
      wxString textExpr = regionExpr->GetAttribute(wxT("text"), wxEmptyString);
      wxString xExpr =  regionExpr->GetAttribute(wxT("x"), wxEmptyString);
      wxString yExpr =  regionExpr->GetAttribute(wxT("y"), wxEmptyString);
      wxString widthExpr = regionExpr->GetAttribute(wxT("w"), wxEmptyString);
      wxString heightExpr = regionExpr->GetAttribute(wxT("h"), wxEmptyString);
      wxString minWidthExpr = regionExpr->GetAttribute(wxT("minw"), wxEmptyString);
      wxString minHeightExpr = regionExpr->GetAttribute(wxT("minh"), wxEmptyString);
      wxString propXExpr = regionExpr->GetAttribute(wxT("propx"), wxEmptyString);
      wxString propYExpr = regionExpr->GetAttribute(wxT("propy"), wxEmptyString);
      wxString formatExpr = regionExpr->GetAttribute(wxT("fmt"), wxEmptyString);
      wxString sizeExpr = regionExpr->GetAttribute(wxT("size"), wxEmptyString);
      wxString familyExpr = regionExpr->GetAttribute(wxT("family"), wxEmptyString);
      wxString styleExpr = regionExpr->GetAttribute(wxT("style"), wxEmptyString);
      wxString weightExpr = regionExpr->GetAttribute(wxT("weight"), wxEmptyString);
      wxString colourExpr = regionExpr->GetAttribute(wxT("clr"), wxEmptyString);;
      wxString penColourExpr = regionExpr->GetAttribute(wxT("penclr"), wxEmptyString);
      wxString penStyleExpr = regionExpr->GetAttribute(wxT("pen_style"), wxEmptyString);

      regionName = nameExpr;
      regionText = textExpr;

      xExpr.ToDouble(&x);
      yExpr.ToDouble(&y);

      widthExpr.ToDouble(&width);
      heightExpr.ToDouble(&height);

      minWidthExpr.ToDouble(&minWidth);
      minHeightExpr.ToDouble(&minHeight);

      propXExpr.ToDouble(&m_regionProportionX);
      propYExpr.ToDouble(&m_regionProportionY);

      long iVal;
      if (formatExpr.ToLong(&iVal)) formatMode = iVal;
      if (sizeExpr.ToLong(&iVal)) fontSize = iVal;
      if (familyExpr.ToLong(&iVal)) fontFamily = (wxFontFamily)iVal;
      if (styleExpr.ToLong(&iVal)) fontStyle = (wxFontStyle)iVal;
      if (weightExpr.ToLong(&iVal)) fontWeight = (wxFontWeight)iVal;

      if (colourExpr.Length())
      {
        regionTextColour = colourExpr;
      }
      else
        regionTextColour = wxT("BLACK");

      if (penColourExpr.Length()) penColour = penColourExpr;
      if (penStyleExpr.ToLong(&iVal)) penStyle = (wxPenStyle)iVal;
    }
    wxFont *font = wxTheFontList->FindOrCreateFont(fontSize, fontFamily, fontStyle, fontWeight);

    wxShapeRegion* region = new wxShapeRegion();
    region->SetProportions(m_regionProportionX, m_regionProportionY);
    region->SetFont(font);
    region->SetSize(width, height);
    region->SetPosition(x, y);
    region->SetMinSize(minWidth, minHeight);
    region->SetFormatMode(formatMode);
    region->SetPenStyle(penStyle);
    if (penColour != wxEmptyString)
      region->SetPenColour(penColour);

    region->m_textColour = regionTextColour;
    region->m_regionText = regionText;
    region->m_regionName = regionName;

    m_regions.Append(region);

    /*
     * Get the formatted text strings
     *
     */
    textExpr = ::xmlFindChild(clause, textNameBuf);
    if (textExpr)
    {
      for (wxXmlNode* node = textExpr->GetChildren(); node; node = node->GetNext())
      {
        wxXmlNode*string_expr = node;
        double the_x = 0.0;
        double the_y = 0.0;
        wxString the_string = string_expr->GetAttribute(wxT("text"), wxEmptyString);
        m_formatted = string_expr->GetAttribute(wxT("x"), wxEmptyString).ToDouble(&the_x)
                   && string_expr->GetAttribute(wxT("y"), wxEmptyString).ToDouble(&the_y);

        if (!the_string.empty())
        {
          wxShapeTextLine *line = new wxShapeTextLine(the_x, the_y, the_string);
          region->m_formattedText.Append(line);
        }
      }
    }

    regionNo ++;
    wxSprintf(regionNameBuf, wxT("region%d"), regionNo);
    wxSprintf(textNameBuf, wxT("text%d"), regionNo);
  }

  // Compatibility: check for no regions (old file).
  // Lines and divided rectangles must deal with this compatibility
  // theirselves. Composites _may_ not have any regions anyway.
  if ((m_regions.GetCount() == 0) &&
      !wxDynamicCast(this, wxLineShape) && !wxDynamicCast(this, wxDividedShape) &&
      !wxDynamicCast(this, wxCompositeShape))
  {
    wxShapeRegion* newRegion = new wxShapeRegion();
    newRegion->SetName(wxT("0"));
    m_regions.Append((wxObject *)newRegion);
    if (m_text.GetCount() > 0)
    {
      newRegion->ClearText();
      for (wxObjectList::compatibility_iterator node = m_text.GetFirst();
           node;
           )
      {
        wxShapeTextLine* textLine = wxStaticCast(node->GetData(), wxShapeTextLine);
        wxObjectList::compatibility_iterator next = node->GetNext();
        newRegion->GetFormattedText().Append((wxObject *)textLine);
        delete node->GetData();
        node = next;
      }
    }
  }
}

void wxShape::Copy(wxShape& copy)
{
  copy.m_id = m_id;
  copy.m_xpos = m_xpos;
  copy.m_ypos = m_ypos;
  copy.m_pen = m_pen;
  copy.m_brush = m_brush;
  copy.m_textColour = m_textColour;
  copy.m_centreResize = m_centreResize;
  copy.m_maintainAspectRatio = m_maintainAspectRatio;
  copy.m_attachmentMode = m_attachmentMode;
  copy.m_spaceAttachments = m_spaceAttachments;
  copy.m_highlighted = m_highlighted;
  copy.m_rotation = m_rotation;
  copy.m_textColourName = m_textColourName;
  copy.m_regionName = m_regionName;

  copy.m_sensitivity = m_sensitivity;
  copy.m_draggable = m_draggable;
  copy.m_fixedWidth = m_fixedWidth;
  copy.m_fixedHeight = m_fixedHeight;
  copy.m_formatMode = m_formatMode;
  copy.m_drawHandles = m_drawHandles;

  copy.m_visible = m_visible;
  copy.m_shadowMode = m_shadowMode;
  copy.m_shadowOffsetX = m_shadowOffsetX;
  copy.m_shadowOffsetY = m_shadowOffsetY;
  copy.m_shadowBrush = m_shadowBrush;

  copy.m_branchNeckLength = m_branchNeckLength;
  copy.m_branchStemLength = m_branchStemLength;
  copy.m_branchSpacing = m_branchSpacing;

  // Copy text regions
  copy.ClearRegions();
  wxObjectList::const_iterator it;
  for (it = m_regions.begin();
       it != m_regions.end();
       it++)
  {
    wxShapeRegion* region = wxStaticCast(*it, wxShapeRegion);
    wxShapeRegion *newRegion = new wxShapeRegion(*region);
    copy.m_regions.Append(newRegion);
  }

  // Copy attachments
  copy.ClearAttachments();
  for (it = m_attachmentPoints.begin();
       it != m_attachmentPoints.end();
       it++)
  {
    wxAttachmentPoint* point = wxStaticCast(*it, wxAttachmentPoint);
    wxAttachmentPoint *newPoint = new wxAttachmentPoint;
    newPoint->m_id = point->m_id;
    newPoint->m_x = point->m_x;
    newPoint->m_y = point->m_y;
    copy.m_attachmentPoints.Append((wxObject *)newPoint);
  }

  // Copy lines
  copy.m_lines.Clear();
  for (it = m_lines.begin();
       it != m_lines.end();
       it++)
  {
    wxLineShape* line = wxStaticCast(*it, wxLineShape);
    copy.m_lines.Append(line);
  }
}

// Create and return a new, fully copied object.
wxShape *wxShape::CreateNewCopy(bool resetMapping, bool recompute)
{
  if (resetMapping)
    oglObjectCopyMapping.Clear();

  wxShape* newObject = wxStaticCast(GetClassInfo()->CreateObject(),wxShape);

  wxASSERT(newObject);
  wxASSERT(wxDynamicCast(newObject, wxShape));

  Copy(*newObject);

  if (GetEventHandler() != this)
  {
    wxShapeEvtHandler* newHandler = GetEventHandler()->CreateNewCopy();
    newObject->SetEventHandler(newHandler);
    newObject->SetPreviousHandler(NULL);
    newHandler->SetPreviousHandler(newObject);
    newHandler->SetShape(newObject);
  }

  if (recompute)
    newObject->Recompute();
  return newObject;
}

// Does the copying for this object, including copying event
// handler data if any. Calls the virtual Copy function.
void wxShape::CopyWithHandler(wxShape& copy)
{
    Copy(copy);

    if (GetEventHandler() != this)
    {
        wxASSERT( copy.GetEventHandler() != NULL );
        wxASSERT( copy.GetEventHandler() != (&copy) );
        wxASSERT( GetEventHandler()->GetClassInfo() == copy.GetEventHandler()->GetClassInfo() );
        GetEventHandler()->CopyData(* (copy.GetEventHandler()));
    }
}


// Default - make 6 control points
void wxShape::MakeControlPoints()
{
  double maxX, maxY, minX, minY;

  GetBoundingBoxMax(&maxX, &maxY);
  GetBoundingBoxMin(&minX, &minY);

  double widthMin = (double)(minX + CONTROL_POINT_SIZE + 2);
  double heightMin = (double)(minY + CONTROL_POINT_SIZE + 2);

  // Offsets from main object
  double top = (double)(- (heightMin / 2.0));
  double bottom = (double)(heightMin / 2.0 + (maxY - minY));
  double left = (double)(- (widthMin / 2.0));
  double right = (double)(widthMin / 2.0 + (maxX - minX));

  wxControlPoint *control = new wxControlPoint(m_canvas, this, CONTROL_POINT_SIZE, left, top,
                                           CONTROL_POINT_DIAGONAL);
  m_canvas->AddShape(control);
  m_controlPoints.Append(control);

  control = new wxControlPoint(m_canvas, this, CONTROL_POINT_SIZE, 0, top,
                                           CONTROL_POINT_VERTICAL);
  m_canvas->AddShape(control);
  m_controlPoints.Append(control);

  control = new wxControlPoint(m_canvas, this, CONTROL_POINT_SIZE, right, top,
                                           CONTROL_POINT_DIAGONAL);
  m_canvas->AddShape(control);
  m_controlPoints.Append(control);

  control = new wxControlPoint(m_canvas, this, CONTROL_POINT_SIZE, right, 0,
                                           CONTROL_POINT_HORIZONTAL);
  m_canvas->AddShape(control);
  m_controlPoints.Append(control);

  control = new wxControlPoint(m_canvas, this, CONTROL_POINT_SIZE, right, bottom,
                                           CONTROL_POINT_DIAGONAL);
  m_canvas->AddShape(control);
  m_controlPoints.Append(control);

  control = new wxControlPoint(m_canvas, this, CONTROL_POINT_SIZE, 0, bottom,
                                           CONTROL_POINT_VERTICAL);
  m_canvas->AddShape(control);
  m_controlPoints.Append(control);

  control = new wxControlPoint(m_canvas, this, CONTROL_POINT_SIZE, left, bottom,
                                           CONTROL_POINT_DIAGONAL);
  m_canvas->AddShape(control);
  m_controlPoints.Append(control);

  control = new wxControlPoint(m_canvas, this, CONTROL_POINT_SIZE, left, 0,
                                           CONTROL_POINT_HORIZONTAL);
  m_canvas->AddShape(control);
  m_controlPoints.Append(control);

}

void wxShape::MakeMandatoryControlPoints()
{
  for (wxObjectList::const_iterator it = m_children.begin();
       it != m_children.end();
       it++)
  {
    wxShape* child = wxStaticCast(*it, wxShape);
    child->MakeMandatoryControlPoints();
  }
}

void wxShape::ResetMandatoryControlPoints()
{
  for (wxObjectList::const_iterator it = m_children.begin();
       it != m_children.end();
       it++)
  {
    wxShape* child = wxStaticCast(*it, wxShape);
    child->ResetMandatoryControlPoints();
  }
}

void wxShape::ResetControlPoints()
{
  ResetMandatoryControlPoints();

  if (m_controlPoints.GetCount() < 1)
    return;

  double maxX, maxY, minX, minY;

  GetBoundingBoxMax(&maxX, &maxY);
  GetBoundingBoxMin(&minX, &minY);

  double widthMin = (double)(minX + CONTROL_POINT_SIZE + 2);
  double heightMin = (double)(minY + CONTROL_POINT_SIZE + 2);

  // Offsets from main object
  double top = (double)(- (heightMin / 2.0));
  double bottom = (double)(heightMin / 2.0 + (maxY - minY));
  double left = (double)(- (widthMin / 2.0));
  double right = (double)(widthMin / 2.0 + (maxX - minX));

  wxObjectList::compatibility_iterator node = m_controlPoints.GetFirst();
  wxControlPoint* control = wxStaticCast(node->GetData(), wxControlPoint);
  control->m_xoffset = left; control->m_yoffset = top;

  node = node->GetNext(); control = wxStaticCast(node->GetData(), wxControlPoint);
  control->m_xoffset = 0; control->m_yoffset = top;

  node = node->GetNext(); control = wxStaticCast(node->GetData(), wxControlPoint);
  control->m_xoffset = right; control->m_yoffset = top;

  node = node->GetNext(); control = wxStaticCast(node->GetData(), wxControlPoint);
  control->m_xoffset = right; control->m_yoffset = 0;

  node = node->GetNext(); control = wxStaticCast(node->GetData(), wxControlPoint);
  control->m_xoffset = right; control->m_yoffset = bottom;

  node = node->GetNext(); control = wxStaticCast(node->GetData(), wxControlPoint);
  control->m_xoffset = 0; control->m_yoffset = bottom;

  node = node->GetNext(); control = wxStaticCast(node->GetData(), wxControlPoint);
  control->m_xoffset = left; control->m_yoffset = bottom;

  node = node->GetNext(); control = wxStaticCast(node->GetData(), wxControlPoint);
  control->m_xoffset = left; control->m_yoffset = 0;
}

void wxShape::DeleteControlPoints(wxDC *dc)
{
  wxObjectList::iterator it;
  for (it = m_controlPoints.begin();
       it != m_controlPoints.end();
       )
  {
    wxControlPoint* control = wxStaticCast(*it, wxControlPoint);
    if (dc)
    {
        control->GetEventHandler()->OnErase(*dc);
    }
    m_canvas->RemoveShape(control);
    delete control;
    m_controlPoints.erase(it);
    it = m_controlPoints.begin();
  }
  // Children of divisions are contained objects,
  // so stop here
  if (!wxDynamicCast(this, wxDivisionShape))
  {
    for (it = m_children.begin();
         it != m_children.end();
         it++)
    {
      wxShape* child = wxStaticCast(*it, wxShape);
      child->DeleteControlPoints(dc);
    }
  }
}

void wxShape::OnDrawControlPoints(wxDC& dc)
{
  if (!m_drawHandles)
    return;

  dc.SetBrush(* wxBLACK_BRUSH);
  dc.SetPen(* wxBLACK_PEN);

  wxObjectList::const_iterator it;
  for (it = m_controlPoints.begin();
       it != m_controlPoints.end();
       it++)
  {
    wxControlPoint* control = wxStaticCast(*it, wxControlPoint);
    control->Draw(dc);
  }
  // Children of divisions are contained objects,
  // so stop here.
  // This test bypasses the type facility for speed
  // (critical when drawing)
  if (!wxDynamicCast(this, wxDivisionShape))
  {
    for (it = m_children.begin();
         it != m_children.end();
         it++)
    {
      wxShape* child = wxStaticCast(*it, wxShape);
      child->GetEventHandler()->OnDrawControlPoints(dc);
    }
  }
}

void wxShape::OnEraseControlPoints(wxDC& dc)
{
  wxObjectList::const_iterator it;
  for (it = m_controlPoints.begin();
       it != m_controlPoints.end();
       it++)
  {
    wxControlPoint* control = wxStaticCast(*it, wxControlPoint);
    control->Erase(dc);
  }
  if (!wxDynamicCast(this, wxDivisionShape))
  {
    for (it = m_children.begin();
         it != m_children.end();
         it++)
    {
      wxShape* child = wxStaticCast(*it, wxShape);
      child->GetEventHandler()->OnEraseControlPoints(dc);
    }
  }
}

void wxShape::Select(bool select, wxDC* dc)
{
  m_selected = select;
  if (select)
  {
    MakeControlPoints();
    // Children of divisions are contained objects,
    // so stop here
    if (!wxDynamicCast(this, wxDivisionShape))
    {
      for (wxObjectList::const_iterator it = m_children.begin();
           it != m_children.end();
           it++)
      {
        wxShape* child = wxStaticCast(*it, wxShape);
        child->MakeMandatoryControlPoints();
      }
    }
    if (dc)
        GetEventHandler()->OnDrawControlPoints(*dc);
  }
  if (!select)
  {
    DeleteControlPoints(dc);
    if (!wxDynamicCast(this, wxDivisionShape))
    {
      for (wxObjectList::const_iterator it = m_children.begin();
           it != m_children.end();
           it++)
      {
        wxShape* child = wxStaticCast(*it, wxShape);
        child->DeleteControlPoints(dc);
      }
    }
  }
}

bool wxShape::Selected() const
{
  return m_selected;
}

bool wxShape::AncestorSelected() const
{
  if (m_selected) return true;
  if (!GetParent())
    return false;
  else
    return GetParent()->AncestorSelected();
}

int wxShape::GetNumberOfAttachments() const
{
  // Should return the MAXIMUM attachment point id here,
  // so higher-level functions can iterate through all attachments,
  // even if they're not contiguous.
  if (m_attachmentPoints.GetCount() == 0)
    return 4;
  else
  {
    int maxN = 3;
    for (wxObjectList::const_iterator it = m_attachmentPoints.begin();
         it != m_attachmentPoints.end();
         it++)
    {
      wxAttachmentPoint* point = wxStaticCast(*it, wxAttachmentPoint);
      if (point->m_id > maxN)
        maxN = point->m_id;
    }
    return maxN+1;;
  }
}

bool wxShape::AttachmentIsValid(int attachment) const
{
  if (m_attachmentPoints.GetCount() == 0)
  {
    return ((attachment >= 0) && (attachment < 4)) ;
  }

  for (wxObjectList::const_iterator it = m_attachmentPoints.begin();
       it != m_attachmentPoints.end();
       it++)
  {
    wxAttachmentPoint* point = wxStaticCast(*it, wxAttachmentPoint);
    if (point->m_id == attachment)
      return true;
  }
  return false;
}

bool wxShape::GetAttachmentPosition(int attachment, double *x, double *y,
                                         int nth, int no_arcs, wxLineShape *line)
{
    if (m_attachmentMode == ATTACHMENT_MODE_NONE)
    {
        *x = m_xpos; *y = m_ypos;
        return true;
    }
    else if (m_attachmentMode == ATTACHMENT_MODE_BRANCHING)
    {
        wxRealPoint pt, stemPt;
        GetBranchingAttachmentPoint(attachment, nth, pt, stemPt);
        *x = pt.x;
        *y = pt.y;
        return true;
    }
    else if (m_attachmentMode == ATTACHMENT_MODE_EDGE)
    {
        if (m_attachmentPoints.GetCount() > 0)
        {
            for (wxObjectList::const_iterator it = m_attachmentPoints.begin();
                 it != m_attachmentPoints.end();
                 it++)
            {
                wxAttachmentPoint* point = wxStaticCast(*it, wxAttachmentPoint);
                if (point->m_id == attachment)
                {
                    *x = (double)(m_xpos + point->m_x);
                    *y = (double)(m_ypos + point->m_y);
                    return true;
                }
            }
            *x = m_xpos; *y = m_ypos;
            return false;
        }
        else
        {
            // Assume is rectangular
            double w, h;
            GetBoundingBoxMax(&w, &h);
            double top = (double)(m_ypos + h/2.0);
            double bottom = (double)(m_ypos - h/2.0);
            double left = (double)(m_xpos - w/2.0);
            double right = (double)(m_xpos + w/2.0);

#if 0
            /* bool isEnd = */ (line && line->IsEnd(this));
#endif

            int physicalAttachment = LogicalToPhysicalAttachment(attachment);

            // Simplified code
            switch (physicalAttachment)
            {
                case 0:
                {
                    wxRealPoint pt = CalcSimpleAttachment(wxRealPoint(left, bottom), wxRealPoint(right, bottom),
                            nth, no_arcs, line);

                    *x = pt.x; *y = pt.y;
                    break;
                }
                case 1:
                {
                    wxRealPoint pt = CalcSimpleAttachment(wxRealPoint(right, bottom), wxRealPoint(right, top),
                            nth, no_arcs, line);

                    *x = pt.x; *y = pt.y;
                    break;
                }
                case 2:
                {
                    wxRealPoint pt = CalcSimpleAttachment(wxRealPoint(left, top), wxRealPoint(right, top),
                            nth, no_arcs, line);

                    *x = pt.x; *y = pt.y;
                    break;
                }
                case 3:
                {
                    wxRealPoint pt = CalcSimpleAttachment(wxRealPoint(left, bottom), wxRealPoint(left, top),
                            nth, no_arcs, line);

                    *x = pt.x; *y = pt.y;
                    break;
                }
                default:
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

void wxShape::GetBoundingBoxMax(double *w, double *h)
{
  double ww, hh;
  GetBoundingBoxMin(&ww, &hh);
  if (m_shadowMode != SHADOW_NONE)
  {
    ww += m_shadowOffsetX;
    hh += m_shadowOffsetY;
  }
  *w = ww;
  *h = hh;
}

// Returns true if image is a descendant of this composite
bool wxShape::HasDescendant(wxShape *image)
{
  if (image == this)
    return true;
  for (wxObjectList::const_iterator it = m_children.begin();
       it != m_children.end();
       it++)
  {
    wxShape* child = wxStaticCast(*it, wxShape);
    bool ans = child->HasDescendant(image);
    if (ans)
      return true;
  }
  return false;
}

// Clears points from a list of wxRealPoints, and clears list
void wxShape::ClearPointList(wxList& list)
{
    for (wxObjectList::const_iterator it = list.begin();
         it != list.end();
         it++)
    {
        wxRealPoint* pt = (wxRealPoint*)*it;
        delete pt;
    }
    list.Clear();
}

// Assuming the attachment lies along a vertical or horizontal line,
// calculate the position on that point.
wxRealPoint wxShape::CalcSimpleAttachment(const wxRealPoint& pt1, const wxRealPoint& pt2,
    int nth, int noArcs, wxLineShape* line)
{
    bool isEnd = (line && line->IsEnd(this));

    // Are we horizontal or vertical?
    bool isHorizontal = (oglRoughlyEqual(pt1.y, pt2.y) == true);

    double x, y;

    if (isHorizontal)
    {
        wxRealPoint firstPoint, secondPoint;
        if (pt1.x > pt2.x)
        {
            firstPoint = pt2;
            secondPoint = pt1;
        }
        else
        {
            firstPoint = pt1;
            secondPoint = pt2;
        }

        if (m_spaceAttachments)
        {
          if (line && (line->GetAlignmentType(isEnd) == LINE_ALIGNMENT_TO_NEXT_HANDLE))
          {
            // Align line according to the next handle along
            wxRealPoint *point = line->GetNextControlPoint(this);
            if (point->x < firstPoint.x)
              x = firstPoint.x;
            else if (point->x > secondPoint.x)
              x = secondPoint.x;
            else
              x = point->x;
          }
          else
            x = firstPoint.x + (nth + 1)*(secondPoint.x - firstPoint.x)/(noArcs + 1);
        }
        else x = (secondPoint.x - firstPoint.x)/2.0; // Midpoint

        y = pt1.y;
    }
    else
    {
        wxASSERT( oglRoughlyEqual(pt1.x, pt2.x) == true );

        wxRealPoint firstPoint, secondPoint;
        if (pt1.y > pt2.y)
        {
            firstPoint = pt2;
            secondPoint = pt1;
        }
        else
        {
            firstPoint = pt1;
            secondPoint = pt2;
        }

        if (m_spaceAttachments)
        {
          if (line && (line->GetAlignmentType(isEnd) == LINE_ALIGNMENT_TO_NEXT_HANDLE))
          {
            // Align line according to the next handle along
            wxRealPoint *point = line->GetNextControlPoint(this);
            if (point->y < firstPoint.y)
              y = firstPoint.y;
            else if (point->y > secondPoint.y)
              y = secondPoint.y;
            else
              y = point->y;
          }
          else
            y = firstPoint.y + (nth + 1)*(secondPoint.y - firstPoint.y)/(noArcs + 1);
        }
        else y = (secondPoint.y - firstPoint.y)/2.0; // Midpoint

        x = pt1.x;
    }

    return wxRealPoint(x, y);
}

// Return the zero-based position in m_lines of line.
int wxShape::GetLinePosition(wxLineShape* line)
{
    for (size_t i = 0; i < m_lines.GetCount(); i++)
        if (wxStaticCast(m_lines.Item(i)->GetData(), wxLineShape) == line)
            return i;

    return 0;
}

//
//             |________|
//                 | <- root
//                 | <- neck
// shoulder1 ->---------<- shoulder2
//             | | | | |
//                      <- branching attachment point N-1

// This function gets information about where branching connections go.
// Returns false if there are no lines at this attachment.
bool wxShape::GetBranchingAttachmentInfo(int attachment, wxRealPoint& root, wxRealPoint& neck,
    wxRealPoint& shoulder1, wxRealPoint& shoulder2)
{
    int physicalAttachment = LogicalToPhysicalAttachment(attachment);

    // Number of lines at this attachment.
    int lineCount = GetAttachmentLineCount(attachment);

    if (lineCount == 0)
        return false;

    int totalBranchLength = m_branchSpacing * (lineCount - 1);

    root = GetBranchingAttachmentRoot(attachment);

    // Assume that we have attachment points 0 to 3: top, right, bottom, left.
    switch (physicalAttachment)
    {
        case 0:
        {
            neck.x = GetX();
            neck.y = root.y - m_branchNeckLength;

            shoulder1.x = root.x - (totalBranchLength/2.0) ;
            shoulder2.x = root.x + (totalBranchLength/2.0) ;

            shoulder1.y = neck.y;
            shoulder2.y = neck.y;
            break;
        }
        case 1:
        {
            neck.x = root.x + m_branchNeckLength;
            neck.y = root.y;

            shoulder1.x = neck.x ;
            shoulder2.x = neck.x ;

            shoulder1.y = neck.y - (totalBranchLength/2.0) ;
            shoulder2.y = neck.y + (totalBranchLength/2.0) ;
            break;
        }
        case 2:
        {
            neck.x = GetX();
            neck.y = root.y + m_branchNeckLength;

            shoulder1.x = root.x - (totalBranchLength/2.0) ;
            shoulder2.x = root.x + (totalBranchLength/2.0) ;

            shoulder1.y = neck.y;
            shoulder2.y = neck.y;
            break;
        }
        case 3:
        {
            neck.x = root.x - m_branchNeckLength;
            neck.y = root.y ;

            shoulder1.x = neck.x ;
            shoulder2.x = neck.x ;

            shoulder1.y = neck.y - (totalBranchLength/2.0) ;
            shoulder2.y = neck.y + (totalBranchLength/2.0) ;
            break;
        }
        default:
        {
            wxFAIL_MSG( wxT("Unrecognised attachment point in GetBranchingAttachmentInfo.") );
            break;
        }
    }
    return true;
}

// n is the number of the adjoining line, from 0 to N-1 where N is the number of lines
// at this attachment point.
// Get the attachment point where the arc joins the stem, and also the point where the
// the stem meets the shoulder.
bool wxShape::GetBranchingAttachmentPoint(int attachment, int n, wxRealPoint& pt, wxRealPoint& stemPt)
{
    int physicalAttachment = LogicalToPhysicalAttachment(attachment);

    wxRealPoint root, neck, shoulder1, shoulder2;
    GetBranchingAttachmentInfo(attachment, root, neck, shoulder1, shoulder2);

    // Assume that we have attachment points 0 to 3: top, right, bottom, left.
    switch (physicalAttachment)
    {
        case 0:
        {
            pt.y = neck.y - m_branchStemLength;
            pt.x = shoulder1.x + n*m_branchSpacing;

            stemPt.x = pt.x;
            stemPt.y = neck.y;
            break;
        }
        case 2:
        {
            pt.y = neck.y + m_branchStemLength;
            pt.x = shoulder1.x + n*m_branchSpacing;

            stemPt.x = pt.x;
            stemPt.y = neck.y;
            break;
        }
        case 1:
        {
            pt.x = neck.x + m_branchStemLength;
            pt.y = shoulder1.y + n*m_branchSpacing;

            stemPt.x = neck.x;
            stemPt.y = pt.y;
            break;
        }
        case 3:
        {
            pt.x = neck.x - m_branchStemLength;
            pt.y = shoulder1.y + n*m_branchSpacing;

            stemPt.x = neck.x;
            stemPt.y = pt.y;
            break;
        }
        default:
        {
            wxFAIL_MSG( wxT("Unrecognised attachment point in GetBranchingAttachmentPoint.") );
            break;
        }
    }

    return true;
}

// Get the number of lines at this attachment position.
int wxShape::GetAttachmentLineCount(int attachment) const
{
    int count = 0;
    for (wxObjectList::const_iterator it = m_lines.begin();
         it != m_lines.end();
         it++)
    {
        wxLineShape* lineShape = wxStaticCast(*it, wxLineShape);
        if ((lineShape->GetFrom() == this) && (lineShape->GetAttachmentFrom() == attachment))
            count ++;
        else if ((lineShape->GetTo() == this) && (lineShape->GetAttachmentTo() == attachment))
            count ++;
    }
    return count;
}

// This function gets the root point at the given attachment.
wxRealPoint wxShape::GetBranchingAttachmentRoot(int attachment)
{
    int physicalAttachment = LogicalToPhysicalAttachment(attachment);

    wxRealPoint root;

    double width, height;
    GetBoundingBoxMax(& width, & height);

    // Assume that we have attachment points 0 to 3: top, right, bottom, left.
    switch (physicalAttachment)
    {
        case 0:
        {
            root.x = GetX() ;
            root.y = GetY() - height/2.0;
            break;
        }
        case 1:
        {
            root.x = GetX() + width/2.0;
            root.y = GetY() ;
            break;
        }
        case 2:
        {
            root.x = GetX() ;
            root.y = GetY() + height/2.0;
            break;
        }
        case 3:
        {
            root.x = GetX() - width/2.0;
            root.y = GetY() ;
            break;
        }
        default:
        {
            wxFAIL_MSG( wxT("Unrecognised attachment point in GetBranchingAttachmentRoot.") );
            break;
        }
    }
    return root;
}

// Draw or erase the branches (not the actual arcs though)
void wxShape::OnDrawBranches(wxDC& dc, int attachment, bool erase)
{
    int count = GetAttachmentLineCount(attachment);
    if (count == 0)
        return;

    wxRealPoint root, neck, shoulder1, shoulder2;
    GetBranchingAttachmentInfo(attachment, root, neck, shoulder1, shoulder2);

    if (erase)
    {
        dc.SetPen(*wxWHITE_PEN);
        dc.SetBrush(*wxWHITE_BRUSH);
    }
    else
    {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(*wxBLACK_BRUSH);
    }

    // Draw neck
    dc.DrawLine((long) root.x, (long) root.y, (long) neck.x, (long) neck.y);

    if (count > 1)
    {
        // Draw shoulder-to-shoulder line
        dc.DrawLine((long) shoulder1.x, (long) shoulder1.y, (long) shoulder2.x, (long) shoulder2.y);
    }
    // Draw all the little branches
    int i;
    for (i = 0; i < count; i++)
    {
        wxRealPoint pt, stemPt;
        GetBranchingAttachmentPoint(attachment, i, pt, stemPt);
        dc.DrawLine((long) stemPt.x, (long) stemPt.y, (long) pt.x, (long) pt.y);

        if ((GetBranchStyle() & BRANCHING_ATTACHMENT_BLOB) && (count > 1))
        {
            long blobSize=6;
//            dc.DrawEllipse((long) (stemPt.x + 0.5 - (blobSize/2.0)), (long) (stemPt.y + 0.5 - (blobSize/2.0)), blobSize, blobSize);
            dc.DrawEllipse((long) (stemPt.x - (blobSize/2.0)), (long) (stemPt.y - (blobSize/2.0)), blobSize, blobSize);
        }
    }
}

// Draw or erase the branches (not the actual arcs though)
void wxShape::OnDrawBranches(wxDC& dc, bool erase)
{
    if (m_attachmentMode != ATTACHMENT_MODE_BRANCHING)
        return;

    int count = GetNumberOfAttachments();
    int i;
    for (i = 0; i < count; i++)
        OnDrawBranches(dc, i, erase);
}

// Only get the attachment position at the _edge_ of the shape, ignoring
// branching mode. This is used e.g. to indicate the edge of interest, not the point
// on the attachment branch.
bool wxShape::GetAttachmentPositionEdge(int attachment, double *x, double *y,
                                     int nth, int no_arcs, wxLineShape *line)
{
    int oldMode = m_attachmentMode;

    // Calculate as if to edge, not branch
    if (m_attachmentMode == ATTACHMENT_MODE_BRANCHING)
        m_attachmentMode = ATTACHMENT_MODE_EDGE;
    bool success = GetAttachmentPosition(attachment, x, y, nth, no_arcs, line);
    m_attachmentMode = oldMode;

    return success;
}

// Rotate the standard attachment point from physical (0 is always North)
// to logical (0 -> 1 if rotated by 90 degrees)
int wxShape::PhysicalToLogicalAttachment(int physicalAttachment) const
{
    const double pi = M_PI ;
    int i;
    if (oglRoughlyEqual(GetRotation(), 0.0))
    {
        i = physicalAttachment;
    }
    else if (oglRoughlyEqual(GetRotation(), (pi/2.0)))
    {
        i = physicalAttachment - 1;
    }
    else if (oglRoughlyEqual(GetRotation(), pi))
    {
        i = physicalAttachment - 2;
    }
    else if (oglRoughlyEqual(GetRotation(), (3.0*pi/2.0)))
    {
        i = physicalAttachment - 3;
    }
    else
        // Can't handle -- assume the same.
        return physicalAttachment;

    if (i < 0)
      i += 4;

    return i;
}

// Rotate the standard attachment point from logical
// to physical (0 is always North)
int wxShape::LogicalToPhysicalAttachment(int logicalAttachment) const
{
    const double pi = M_PI ;
    int i;
    if (oglRoughlyEqual(GetRotation(), 0.0))
    {
        i = logicalAttachment;
    }
    else if (oglRoughlyEqual(GetRotation(), (pi/2.0)))
    {
        i = logicalAttachment + 1;
    }
    else if (oglRoughlyEqual(GetRotation(), pi))
    {
        i = logicalAttachment + 2;
    }
    else if (oglRoughlyEqual(GetRotation(), (3.0*pi/2.0)))
    {
        i = logicalAttachment + 3;
    }
    else
        // Can't handle -- assume the same.
        return logicalAttachment;

    if (i > 3)
      i -= 4;

    return i;
}

void wxShape::Rotate(double WXUNUSED(x), double WXUNUSED(y), double theta)
{
    const double pi = M_PI ;
    m_rotation = theta;
    if (m_rotation < 0.0)
    {
        m_rotation += 2*pi;
    }
    else if (m_rotation > 2*pi)
    {
        m_rotation -= 2*pi;
    }
}


wxPen wxShape::GetBackgroundPen()
{
    if (GetCanvas())
    {
        wxColour c = GetCanvas()->GetBackgroundColour();
        return wxPen(c, 1, wxPENSTYLE_SOLID);
    }
    return * g_oglWhiteBackgroundPen;
}


wxBrush wxShape::GetBackgroundBrush()
{
    if (GetCanvas())
    {
        wxColour c = GetCanvas()->GetBackgroundColour();
        return wxBrush(c, wxBRUSHSTYLE_SOLID);
    }
    return * g_oglWhiteBackgroundBrush;
}
