/////////////////////////////////////////////////////////////////////////////
// Name:        contrib/samples/ogl/ogledit/view.cpp
// Purpose:     Implements view functionality in OGLEdit
// Author:      Julian Smart
// Modified by:
// Created:     12/07/98
// RCS-ID:      $Id: view.cpp,v 1.1 2007/03/28 15:15:49 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "app.h"
#include "doc.h"
#include "view.h"
#include "palette.h"

IMPLEMENT_DYNAMIC_CLASS(DiagramView, wxView)

BEGIN_EVENT_TABLE(DiagramView, wxView)
    EVT_MENU(wxID_CUT, DiagramView::OnCut)
    EVT_MENU(OGLEDIT_CHANGE_BACKGROUND_COLOUR, DiagramView::OnChangeBackgroundColour)
    EVT_MENU(OGLEDIT_EDIT_LABEL, DiagramView::OnEditLabel)
END_EVENT_TABLE()

// What to do when a view is created. Creates actual
// windows for displaying the view.
bool DiagramView::OnCreate(wxDocument* doc, long flags)
{
    bool ok = wxView::OnCreate(doc, flags);
    if (ok)
    {
        m_canvas = wxGetApp().GetMainFrame()->canvas;
        m_canvas->m_view = this;
        m_canvas->Enable(true);

        SetFrame(wxGetApp().GetMainFrame());

        // Initialize the edit menu Undo and Redo items
        doc->GetCommandProcessor()->SetEditMenu(wxStaticCast(GetFrame(), MyFrame)->editMenu);
        doc->GetCommandProcessor()->Initialize();

        wxShapeCanvas *shapeCanvas = m_canvas;
        DiagramDocument* diagramDoc = wxStaticCast(doc, DiagramDocument);
        shapeCanvas->SetDiagram(diagramDoc->GetDiagram());
        diagramDoc->GetDiagram()->SetCanvas(shapeCanvas);
    }
    return ok;
}

DiagramDocument* DiagramView::GetDocument() const
{
   return wxStaticCast(wxView::GetDocument(), DiagramDocument);
}

#define CENTER  false // Place the drawing to the center of the page


// Sneakily gets used for default print/preview
// as well as drawing on the screen.
void DiagramView::OnDraw(wxDC *dc)
{

  /* You might use THIS code if you were scaling
   * graphics of known size to fit on the page.
   */
  wxCoord w, h;

  // We need to adjust for the graphic size, a formula will be added
  float maxX = 900;
  float maxY = 700;
  // A better way of find the maximum values would be to search through
  // the linked list

  // Let's have at least 10 device units margin
  float marginX = 10;
  float marginY = 10;

  // Add the margin to the graphic size
  maxX += (2 * marginX);
  maxY += (2 * marginY);

  // Get the size of the DC in pixels
  dc->GetSize (&w, &h);

  // Calculate a suitable scaling factor
  float scaleX = (float) (w / maxX);
  float scaleY = (float) (h / maxY);

  // Use x or y scaling factor, whichever fits on the DC
  float actualScale = wxMin (scaleX, scaleY);

  float posX, posY;
  // Calculate the position on the DC for centring the graphic
  #if 0
     // center the drawing
      posX = (float) ((w - (200 * actualScale)) / 2.0);
      posY = (float) ((h - (200 * actualScale)) / 2.0);
  #else
     // Use defined presets
      posX = 10;
      posY = 35;
  #endif


  // Set the scale and origin
  dc->SetUserScale (actualScale, actualScale);
  dc->SetDeviceOrigin ((long) posX, (long) posY);

  // This part was added to preform the print preview and printing functions

  wxDiagram *diagram_p= GetDocument()->GetDiagram();  // Get the current diagram
  if (diagram_p->GetShapeList())
  {
    /* wxCursor *old_cursor = NULL; */
    for (wxObjectList::compatibility_iterator current = diagram_p->GetShapeList()->GetFirst();
         current;
         current = current->GetNext())
    {
        wxShape* object = wxStaticCast(current->GetData(), wxShape);
        if (!object->GetParent())
        {
            object->Draw(* dc); // Draw the shape onto our printing dc
        }
    }
  }
}

void DiagramView::OnUpdate(wxView* sender, wxObject* hint)
{
    wxView::OnUpdate(sender, hint);
    if (m_canvas)
        m_canvas->Refresh();
}

void DiagramView::OnChangeFilename()
{
    wxView::OnChangeFilename();
    wxWindow *win = GetFrame();
    if (!win) return;

    win->SetLabel(wxString::Format(wxT("%s - %s"), 
       win->GetLabel().wx_str(),
       wxGetApp().GetAppDisplayName().wx_str()
       ));
}

// Clean up windows used for displaying the view.
bool DiagramView::OnClose(bool deleteWindow)
{
  if (!wxView::OnClose(deleteWindow))
    return false;

  DiagramDocument *diagramDoc = GetDocument();
  diagramDoc->GetDiagram()->SetCanvas(NULL);

  m_canvas->ClearBackground();
  m_canvas->SetDiagram(NULL);
  m_canvas->m_view = NULL;
  m_canvas->Enable(false);
  m_canvas = NULL;

  //wxString s = wxGetApp().GetAppDisplayName();
  //if (frame)
  //  frame->SetTitle(s);

  Activate(false);
  return true;
}

wxShape *DiagramView::FindSelectedShape(void)
{
  DiagramDocument *doc = GetDocument();
  for (wxObjectList::compatibility_iterator node = doc->GetDiagram()->GetShapeList()->GetFirst();
       node;
       )
  {
    wxShape* eachShape = wxStaticCast(node->GetData(), wxShape);
    if ((eachShape->GetParent() == NULL) && eachShape->Selected())
    {
      return eachShape;
    }
    else
    {
       node = node->GetNext();
    }
  }
  return NULL;
}

void DiagramView::OnCut(wxCommandEvent& WXUNUSED(event))
{
  DiagramDocument *doc = GetDocument();

  wxShape *theShape = FindSelectedShape();
  if (theShape)
    doc->GetCommandProcessor()->Submit(new DiagramCommand(_("Cut"), wxID_CUT, doc, NULL, 0.0, 0.0, true, theShape));
}

void DiagramView::OnChangeBackgroundColour(wxCommandEvent& WXUNUSED(event))
{
      DiagramDocument *doc = GetDocument();

      wxShape *theShape = FindSelectedShape();
      if (theShape)
      {
        wxColourData data;
        data.SetChooseFull(true);
        data.SetColour(theShape->GetBrush()->GetColour());

        wxColourDialog *dialog = new wxColourDialog(GetFrame(), &data);
        wxBrush *theBrush = NULL;
        if (dialog->ShowModal() == wxID_OK)
        {
          wxColourData retData = dialog->GetColourData();
          wxColour col = retData.GetColour();
          theBrush = wxTheBrushList->FindOrCreateBrush(col, wxBRUSHSTYLE_SOLID);
        }
        dialog->Close();

        if (theBrush)
          doc->GetCommandProcessor()->Submit(new DiagramCommand(_("Change colour"), OGLEDIT_CHANGE_BACKGROUND_COLOUR, doc,
            theBrush, theShape));
      }
}

void DiagramView::OnEditLabel(wxCommandEvent& WXUNUSED(event))
{
      wxShape *theShape = FindSelectedShape();
      if (theShape)
      {
        wxString newLabel = wxGetTextFromUser(_("Enter new label"), _("Shape Label"), ((MyEvtHandler *)theShape->GetEventHandler())->label);
        GetDocument()->GetCommandProcessor()->Submit(new DiagramCommand(_("Edit label"), OGLEDIT_EDIT_LABEL, GetDocument(), newLabel, theShape));
      }
}


/*
 * Window implementations
 */

BEGIN_EVENT_TABLE(MyCanvas, wxShapeCanvas)
    EVT_MOUSE_EVENTS(MyCanvas::OnMouseEvent)
    EVT_PAINT(MyCanvas::OnPaint)
END_EVENT_TABLE()

// Define a constructor for my canvas
MyCanvas::MyCanvas(wxView *v, wxWindow *parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style):
 wxShapeCanvas(parent, id, pos, size, style)
{
  SetBackgroundColour(*wxWHITE);
  m_view = v;
}

MyCanvas::~MyCanvas(void)
{
}

void MyCanvas::OnLeftClick(double x, double y, int WXUNUSED(keys))
{
  EditorToolPalette *palette = wxGetApp().GetMainFrame()->palette;
  wxClassInfo *info = NULL;
  switch (palette->currentlySelected)
  {
    case PALETTE_TOOL1:
    {
      info = wxCLASSINFO(wxRectangleShape);
      break;
    }
    case PALETTE_TOOL2:
    {
      info = wxCLASSINFO(wxRoundedRectangleShape);
      break;
    }
    case PALETTE_TOOL3:
    {
      info = wxCLASSINFO(wxEllipseShape);
      break;
    }
    case PALETTE_TOOL4:
    {
      info = wxCLASSINFO(wxDiamondShape);
      break;
    }
    default:
      break;
  }
  if (info)
  {
    GetDocument()->GetCommandProcessor()->Submit(
      new DiagramCommand( info->GetClassName(), OGLEDIT_ADD_SHAPE, wxStaticCast(GetDocument(), DiagramDocument), info,
         x, y));
  }
}

void MyCanvas::OnRightClick(double WXUNUSED(x), double WXUNUSED(y), int WXUNUSED(keys))
{
}

void MyCanvas::OnDragLeft(bool WXUNUSED(draw), double WXUNUSED(x), double WXUNUSED(y), int WXUNUSED(keys))
{
}

void MyCanvas::OnBeginDragLeft(double WXUNUSED(x), double WXUNUSED(y), int WXUNUSED(keys))
{
}

void MyCanvas::OnEndDragLeft(double WXUNUSED(x), double WXUNUSED(y), int WXUNUSED(keys))
{
}

void MyCanvas::OnDragRight(bool WXUNUSED(draw), double WXUNUSED(x), double WXUNUSED(y), int WXUNUSED(keys))
{
}

void MyCanvas::OnBeginDragRight(double WXUNUSED(x), double WXUNUSED(y), int WXUNUSED(keys))
{
}

void MyCanvas::OnEndDragRight(double WXUNUSED(x), double WXUNUSED(y), int WXUNUSED(keys))
{
}

void MyCanvas::OnMouseEvent(wxMouseEvent& event)
{
    wxShapeCanvas::OnMouseEvent(event);
}

void MyCanvas::OnPaint(wxPaintEvent& event)
{
//  if (GetDiagram())
    wxShapeCanvas::OnPaint(event);
}
