/////////////////////////////////////////////////////////////////////////////
// Name:        contrib/samples/ogl/studio/view.cpp
// Purpose:     Implements view functionality
// Author:      Julian Smart
// Modified by:
// Created:     12/07/98
// RCS-ID:      $Id: view.cpp,v 1.1 2007/03/28 15:15:51 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "app.h"
#include "doc.h"
#include "view.h"
#include "cspalette.h"
#include "symbols.h"
#include "dialogs.h"
#include <wx/ogl/basicp.h>
#include <wx/ogl/linesp.h>

IMPLEMENT_DYNAMIC_CLASS(csDiagramView, wxView)

BEGIN_EVENT_TABLE(csDiagramView, wxView)
    EVT_MENU(wxID_CUT, csDiagramView::OnCut)
    EVT_MENU(wxID_COPY, csDiagramView::OnCopy)
    EVT_MENU(wxID_CLEAR, csDiagramView::OnClear)
    EVT_MENU(wxID_PASTE, csDiagramView::OnPaste)
    EVT_MENU(wxID_DUPLICATE, csDiagramView::OnDuplicate)
    EVT_MENU(ID_CS_CHANGE_BACKGROUND_COLOUR, csDiagramView::OnChangeBackgroundColour)
    EVT_MENU(wxID_PROPERTIES, csDiagramView::OnEditProperties)
    EVT_MENU(wxID_SELECTALL, csDiagramView::OnSelectAll)
    EVT_TOOL(DIAGRAM_TOOLBAR_LINE_ARROW, csDiagramView::OnToggleArrowTool)
    EVT_COMBOBOX(ID_WINDOW_POINT_SIZE_COMBOBOX, csDiagramView::OnPointSizeComboSel)
    EVT_COMBOBOX(ID_WINDOW_ZOOM_COMBOBOX, csDiagramView::OnZoomSel)
    EVT_TEXT(ID_WINDOW_POINT_SIZE_COMBOBOX, csDiagramView::OnPointSizeComboText)
    EVT_TOOL(DIAGRAM_TOOLBAR_ALIGNL, csDiagramView::OnAlign)
    EVT_TOOL(DIAGRAM_TOOLBAR_ALIGNR, csDiagramView::OnAlign)
    EVT_TOOL(DIAGRAM_TOOLBAR_ALIGNB, csDiagramView::OnAlign)
    EVT_TOOL(DIAGRAM_TOOLBAR_ALIGNT, csDiagramView::OnAlign)
    EVT_TOOL(DIAGRAM_TOOLBAR_ALIGN_HORIZ, csDiagramView::OnAlign)
    EVT_TOOL(DIAGRAM_TOOLBAR_ALIGN_VERT, csDiagramView::OnAlign)
    EVT_TOOL(DIAGRAM_TOOLBAR_COPY_SIZE, csDiagramView::OnAlign)
    EVT_TOOL(DIAGRAM_TOOLBAR_NEW_POINT, csDiagramView::OnNewLinePoint)
    EVT_TOOL(DIAGRAM_TOOLBAR_CUT_POINT, csDiagramView::OnCutLinePoint)
    EVT_TOOL(DIAGRAM_TOOLBAR_STRAIGHTEN, csDiagramView::OnStraightenLines)
    EVT_UPDATE_UI(DIAGRAM_TOOLBAR_ALIGNL, csDiagramView::OnAlignUpdate)
    EVT_UPDATE_UI(DIAGRAM_TOOLBAR_ALIGNR, csDiagramView::OnAlignUpdate)
    EVT_UPDATE_UI(DIAGRAM_TOOLBAR_ALIGNB, csDiagramView::OnAlignUpdate)
    EVT_UPDATE_UI(DIAGRAM_TOOLBAR_ALIGNT, csDiagramView::OnAlignUpdate)
    EVT_UPDATE_UI(DIAGRAM_TOOLBAR_ALIGN_HORIZ, csDiagramView::OnAlignUpdate)
    EVT_UPDATE_UI(DIAGRAM_TOOLBAR_ALIGN_VERT, csDiagramView::OnAlignUpdate)
    EVT_UPDATE_UI(DIAGRAM_TOOLBAR_COPY_SIZE, csDiagramView::OnAlignUpdate)
    EVT_UPDATE_UI(DIAGRAM_TOOLBAR_NEW_POINT, csDiagramView::OnNewLinePointUpdate)
    EVT_UPDATE_UI(DIAGRAM_TOOLBAR_CUT_POINT, csDiagramView::OnCutLinePointUpdate)
    EVT_UPDATE_UI(DIAGRAM_TOOLBAR_STRAIGHTEN, csDiagramView::OnStraightenLinesUpdate)
    EVT_UPDATE_UI(DIAGRAM_TOOLBAR_LINE_ARROW, csDiagramView::OnToggleArrowToolUpdate)
    EVT_UPDATE_UI(wxID_CUT, csDiagramView::OnNeedSelection)
    EVT_UPDATE_UI(wxID_COPY, csDiagramView::OnNeedSelection)
    EVT_UPDATE_UI(wxID_CLEAR, csDiagramView::OnNeedSelection)
    EVT_UPDATE_UI(wxID_PASTE, csDiagramView::OnPasteUpdate)
    EVT_UPDATE_UI(wxID_DUPLICATE, csDiagramView::OnNeedSelection)
    EVT_UPDATE_UI(wxID_PROPERTIES, csDiagramView::OnEditPropertiesUpdate)
    EVT_UPDATE_UI(wxID_UNDO, csDiagramView::OnUndoUpdate)
    EVT_UPDATE_UI(wxID_REDO, csDiagramView::OnRedoUpdate)
END_EVENT_TABLE()

// What to do when a view is created. Creates actual
// windows for displaying the view.
bool csDiagramView::OnCreate(wxDocument* doc, long flags)
{
    bool ok = wxView::OnCreate(doc, flags);
    if (ok)
    {
        wxMenu* editMenu;
        wxDocMDIChildFrame* frame = wxGetApp().CreateChildFrame(doc, &editMenu);
        wxASSERT(frame == GetFrame());
        m_canvas = wxGetApp().CreateCanvas(this);

        // Initialize the edit menu Undo and Redo items
        doc->GetCommandProcessor()->SetEditMenu(editMenu);
        doc->GetCommandProcessor()->Initialize();

        wxShapeCanvas *shapeCanvas = m_canvas;
        csDiagramDocument *diagramDoc = wxStaticCast(doc, csDiagramDocument);
        shapeCanvas->SetDiagram(diagramDoc->GetDiagram());
        diagramDoc->GetDiagram()->SetCanvas(shapeCanvas);

        diagramDoc->GetDiagram()->SetGridSpacing((double) wxGetApp().GetGridSpacing());

        switch (wxGetApp().GetGridStyle())
        {
            case csGRID_STYLE_NONE:
                diagramDoc->GetDiagram()->SetSnapToGrid(false);
                break;
            case csGRID_STYLE_INVISIBLE:
                diagramDoc->GetDiagram()->SetSnapToGrid(true);
                break;
            case csGRID_STYLE_DOTTED:
                // TODO (not implemented in OGL)
                break;
        }
    }
    return ok;
}

csDiagramView::~csDiagramView(void)
{
}

// Sneakily gets used for default print/preview
// as well as drawing on the screen.
void csDiagramView::OnDraw(wxDC *WXUNUSED(dc))
{
}

void csDiagramView::OnUpdate(wxView* sender, wxObject* hint)
{
    base::OnUpdate(sender, hint);
    if (m_canvas)
        m_canvas->Refresh();
}

// Clean up windows used for displaying the view.
bool csDiagramView::OnClose(bool deleteWindow)
{
  if (!wxView::OnClose(deleteWindow))
    return false;

  csDiagramDocument* diagramDoc = GetDocument();
  diagramDoc->GetDiagram()->SetCanvas(NULL);

  m_canvas->ClearBackground();
  m_canvas->SetDiagram(NULL);
  m_canvas->SetView(NULL);
  m_canvas = NULL;

  wxMenu* fileMenu = GetFrame()->GetMenuBar()->GetMenu(0);

  // Remove file menu from those managed by the command history
  diagramDoc->GetDocumentManager()->FileHistoryRemoveMenu(fileMenu);

  Activate(false);
  GetFrame()->Show(false);

  if (deleteWindow && GetFrame())
  {
    GetFrame()->Destroy();
    SetFrame(NULL);
  }
  return true;
}

// Adds or removes shape from m_selections
void csDiagramView::SelectShape(wxShape* shape, bool select)
{
    if (select && !m_selections.Member(shape))
        m_selections.Append(shape);
    else if (!select)
        m_selections.DeleteObject(shape);
}

void csDiagramView::OnSelectAll(wxCommandEvent& WXUNUSED(event))
{
    SelectAll(true);
}

wxShape *csDiagramView::FindFirstSelectedShape(void)
{
  csDiagramDocument* doc = GetDocument();
  for (wxObjectList::compatibility_iterator node = doc->GetDiagram()->GetShapeList()->GetFirst();
       node;
       )
  {
    wxShape* eachShape = wxStaticCast(node->GetData(), wxShape);
    if ((eachShape->GetParent() == NULL) && !wxDynamicCast(eachShape, wxLabelShape) && eachShape->Selected())
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

void csDiagramView::FindSelectedShapes(wxList& selections, wxClassInfo* toFind)
{
  csDiagramDocument* doc = GetDocument();
  for (wxObjectList::compatibility_iterator node = doc->GetDiagram()->GetShapeList()->GetFirst();
       node;
       node = node->GetNext())
  {
    wxShape* eachShape = wxStaticCast(node->GetData(), wxShape);
    if ((eachShape->GetParent() == NULL) && !wxDynamicCast(eachShape, wxLabelShape) && eachShape->Selected() && ((toFind == NULL) || (eachShape->IsKindOf(toFind))))
    {
      selections.Append(eachShape);
    }
  }
}

void csDiagramView::OnUndoUpdate(wxUpdateUIEvent& event)
{
    csDiagramDocument* doc = GetDocument();
    event.Enable(doc->GetCommandProcessor()->CanUndo());
}

void csDiagramView::OnRedoUpdate(wxUpdateUIEvent& event)
{
    csDiagramDocument* doc = GetDocument();
    event.Enable(doc->GetCommandProcessor()->CanRedo());
}

void csDiagramView::OnCut(wxCommandEvent& WXUNUSED(event))
{
    csDiagramDocument* doc = GetDocument();

    // Copy the shapes to the clipboard
    wxGetApp().GetDiagramClipboard().Copy(doc->GetDiagram());

    wxList selections;
    FindSelectedShapes(selections);

    DoCut(selections);
}

void csDiagramView::OnClear(wxCommandEvent& WXUNUSED(event))
{
    wxList selections;
    FindSelectedShapes(selections);

    DoCut(selections);
}

void csDiagramView::OnCopy(wxCommandEvent& WXUNUSED(event))
{
    csDiagramDocument* doc = GetDocument();

    // Copy the shapes to the clipboard
    if (wxGetApp().GetDiagramClipboard().Copy(doc->GetDiagram()))
    {
#ifdef __WXMSW__
        // Copy to the Windows clipboard
        wxGetApp().GetDiagramClipboard().CopyToClipboard(1.0);
#endif
    }
}

void csDiagramView::OnPaste(wxCommandEvent& WXUNUSED(event))
{
    csDiagramDocument* doc = GetDocument();

    wxGetApp().GetDiagramClipboard().Paste(doc->GetDiagram());
}

void csDiagramView::OnDuplicate(wxCommandEvent& WXUNUSED(event))
{
    csDiagramDocument* doc = GetDocument();

    // Do a copy, then a paste
    wxGetApp().GetDiagramClipboard().Copy(doc->GetDiagram());

    // Apply an offset. Really, this offset should keep being incremented,
    // but where do we reset it again?
    wxGetApp().GetDiagramClipboard().Paste(doc->GetDiagram(), NULL, 20, 20);
}

void csDiagramView::OnPasteUpdate(wxUpdateUIEvent& event)
{
    int n = wxGetApp().GetDiagramClipboard().GetCount();

    event.Enable(n > 0);
}

void csDiagramView::OnNeedSelection(wxUpdateUIEvent& event)
{
    event.Enable(m_selections.GetCount() > 0);
}

csDiagramDocument* csDiagramView::GetDocument() const
{
   return wxStaticCast(base::GetDocument(), csDiagramDocument);
}

void csDiagramView::DoCut(wxList& shapes)
{
    csDiagramDocument* doc = GetDocument();

    if (shapes.GetCount() > 0)
    {
        csDiagramCommand* cmd = new csDiagramCommand(_("Cut"), doc);

        for (wxObjectList::compatibility_iterator node = shapes.GetFirst();
             node;
             node = node->GetNext())
        {
            wxShape* theShape = wxStaticCast(node->GetData(), wxShape);
            csCommandState* state = new csCommandState(wxID_CUT, NULL, theShape);

            // Insert lines at the front, so they are cut first.
            // Otherwise we may try to remove a shape with a line still
            // attached.
            if (wxDynamicCast(theShape, wxLineShape))
                cmd->InsertState(state);
            else
                cmd->AddState(state);
        }
        cmd->RemoveLines(); // Schedule any connected lines, not already mentioned,
                            // to be removed first

        doc->GetCommandProcessor()->Submit(cmd);
    }
}

// Generalised command
void csDiagramView::DoCmd(wxList& shapes, wxList& oldShapes, int cmd, const wxString& op)
{
    csDiagramDocument* doc = GetDocument();

    if (shapes.GetCount() > 0)
    {
        csDiagramCommand* command = new csDiagramCommand(op, doc);

        wxObjectList::compatibility_iterator node = shapes.GetFirst();
        wxObjectList::compatibility_iterator node1 = oldShapes.GetFirst();
        for (;
             node && node1;
             node = node->GetNext(), node1 = node1->GetNext())
        {
            wxShape* theShape = wxStaticCast(node->GetData(), wxShape);
            wxShape* oldShape = wxStaticCast(node1->GetData(), wxShape);
            csCommandState* state = new csCommandState(cmd, theShape, oldShape);
            command->AddState(state);
        }
        doc->GetCommandProcessor()->Submit(command);
    }
}

void csDiagramView::OnChangeBackgroundColour(wxCommandEvent& WXUNUSED(event))
{
    csDiagramDocument* doc = GetDocument();

    wxList selections;
    FindSelectedShapes(selections);

    if (selections.GetCount() > 0)
    {
        wxColourData data;
        data.SetChooseFull(true);
        if (selections.GetCount() == 1)
        {
            wxShape* firstShape = wxStaticCast(selections.GetFirst()->GetData(), wxShape);
            data.SetColour(firstShape->GetBrush()->GetColour());
        }

        wxColourDialog *dialog = new wxColourDialog(GetFrame(), &data);
        wxBrush *theBrush = NULL;
        if (dialog->ShowModal() == wxID_OK)
        {
          wxColourData retData = dialog->GetColourData();
          wxColour col = retData.GetColour();
          theBrush = wxTheBrushList->FindOrCreateBrush(col, wxBRUSHSTYLE_SOLID);
        }
        dialog->Close(true);
        if (!theBrush)
            return;

        csDiagramCommand* cmd = new csDiagramCommand(_("Change colour"), doc);

        for (wxObjectList::compatibility_iterator node = selections.GetFirst();
             node;
             node = node->GetNext())
        {
            wxShape* theShape = wxStaticCast(node->GetData(), wxShape);
            wxShape* newShape = theShape->CreateNewCopy();
            newShape->SetBrush(theBrush);

            csCommandState* state = new csCommandState(ID_CS_CHANGE_BACKGROUND_COLOUR, newShape, theShape);
            cmd->AddState(state);
        }
        doc->GetCommandProcessor()->Submit(cmd);
    }
}

void csDiagramView::OnEditProperties(wxCommandEvent& WXUNUSED(event))
{
      wxShape *theShape = FindFirstSelectedShape();
      if (theShape)
        wxStaticCast(theShape->GetEventHandler(), csEvtHandler)->EditProperties();
}

void csDiagramView::OnEditPropertiesUpdate(wxUpdateUIEvent& event)
{
    wxList selections;
    FindSelectedShapes(selections);
    event.Enable( (selections.GetCount() > 0) );
}

void csDiagramView::OnPointSizeComboSel(wxCommandEvent& event)
{
    wxComboBox* combo = wxStaticCast(event.GetEventObject(), wxComboBox);
    wxASSERT(combo);

    int newPointSize = (combo->GetSelection() + 1);

    ApplyPointSize(newPointSize);

}

// TODO: must find out how to intercept the Return key, rather than
// every key stroke. But for now, do every key stroke.
void csDiagramView::OnPointSizeComboText(wxCommandEvent& event)
{
    wxComboBox* combo = wxStaticCast(event.GetEventObject(), wxComboBox);
    wxASSERT(combo);

    wxString str(combo->GetValue());
    long newPointSize;
    str.ToLong( &newPointSize );

    if (newPointSize < 2)
        return;

    ApplyPointSize(newPointSize);
}

void csDiagramView::ApplyPointSize(int pointSize)
{
    csDiagramDocument* doc = GetDocument();

    wxList selections;
    FindSelectedShapes(selections);

    if (selections.GetCount() > 0)
    {
        csDiagramCommand* cmd = new csDiagramCommand(_("Point size"), doc);

        for (wxObjectList::compatibility_iterator node = selections.GetFirst();
             node;
             node = node->GetNext())
        {
            wxShape* theShape = wxStaticCast(node->GetData(), wxShape);
            wxShape* newShape = theShape->CreateNewCopy();

            wxFont* newFont = wxTheFontList->FindOrCreateFont(pointSize,
                theShape->GetFont()->GetFamily(),
                theShape->GetFont()->GetStyle(),
                theShape->GetFont()->GetWeight(),
                theShape->GetFont()->GetUnderlined(),
                theShape->GetFont()->GetFaceName());

            newShape->SetFont(newFont);

            csCommandState* state = new csCommandState(ID_CS_FONT_CHANGE, newShape, theShape);

            cmd->AddState(state);
        }
        doc->GetCommandProcessor()->Submit(cmd);
    }
}

void csDiagramView::OnZoomSel(wxCommandEvent& event)
{
    int maxZoom = 200;
    int minZoom = 5;
    int inc = 5;
    int noStrings = (maxZoom - minZoom)/inc ;

    wxComboBox* combo = wxStaticCast(event.GetEventObject(), wxComboBox);
    wxASSERT(combo);

    int scale = (int) ((noStrings - combo->GetSelection() - 1)*inc + minZoom);

    m_canvas->SetScale((double) (scale/100.0), (double) (scale/100.0));
    m_canvas->Refresh();
}

// Select or deselect all
void csDiagramView::SelectAll(bool select)
{
    wxClientDC dc(m_canvas);
    m_canvas->PrepareDC(dc);

    if (!select)
    {
        wxList selections;
        FindSelectedShapes(selections);

        for (wxObjectList::compatibility_iterator node = selections.GetFirst();
             node;
             node = node->GetNext())
        {
            wxShape* theShape = wxStaticCast(node->GetData(), wxShape);
            theShape->Select(false, &dc);
            SelectShape(theShape, false);
        }
    }
    else
    {
        csDiagramDocument* doc = GetDocument();
        for (wxObjectList::compatibility_iterator node = doc->GetDiagram()->GetShapeList()->GetFirst();
             node;
             node = node->GetNext())
        {
            wxShape* eachShape = wxStaticCast(node->GetData(), wxShape);
            if (eachShape->GetParent() == NULL &&
                !wxDynamicCast(eachShape, wxControlPoint) &&
                !wxDynamicCast(eachShape, wxLabelShape))
            {
                eachShape->Select(true, &dc);
                SelectShape(eachShape, true);
            }
        }
    }
}


void csDiagramView::OnToggleArrowTool(wxCommandEvent& WXUNUSED(event))
{
    csDiagramDocument* doc = GetDocument();

    bool state = wxGetApp().GetDiagramToolBar()->GetToolState(DIAGRAM_TOOLBAR_LINE_ARROW);
    wxString stateName;
    if (state)
        stateName = wxT("Arrow on");
    else
        stateName = wxT("Arrow off");

    wxList selections;
    FindSelectedShapes(selections, wxCLASSINFO(wxLineShape));

    if (selections.GetCount() > 0)
    {
        csDiagramCommand* cmd = new csDiagramCommand(stateName, doc);

        for (wxObjectList::compatibility_iterator node = selections.GetFirst();
             node;
             node = node->GetNext())
        {
            wxLineShape* theShape = wxStaticCast(node->GetData(), wxLineShape);
            wxLineShape *newShape = NULL;

            if (state)
            {
                // Add arrow
                if (theShape->GetArrows().GetCount() == 0)
                {
                    newShape = wxStaticCast(theShape->CreateNewCopy(), wxLineShape);
                    newShape->AddArrow(ARROW_ARROW, ARROW_POSITION_MIDDLE, 10.0, 0.0, wxT("Normal arrowhead"));
                }
            }
            else
            {
                if (theShape->GetArrows().GetCount() > 0)
                {
                    newShape = wxStaticCast(theShape->CreateNewCopy(), wxLineShape);
                    newShape->ClearArrowsAtPosition();
                }
            }

            // If the new state is the same as the old, don't bother adding it to the command state.
            if (newShape)
            {
                csCommandState* state = new csCommandState(ID_CS_ARROW_CHANGE, newShape, theShape);
                cmd->AddState(state);
            }
        }
        doc->GetCommandProcessor()->Submit(cmd);
    }
}

void csDiagramView::OnToggleArrowToolUpdate(wxUpdateUIEvent& event)
{
    wxList selections;

    FindSelectedShapes(selections, wxCLASSINFO(wxLineShape));
    event.Enable( (selections.GetCount() > 0) );
}

// Make the point size combobox reflect this
void csDiagramView::ReflectPointSize(int pointSize)
{
    wxComboBox* comboBox = wxGetApp().GetPointSizeComboBox();
    comboBox->SetSelection(pointSize -1);
}

// Make the arrow toggle button reflect the state of the line
void csDiagramView::ReflectArrowState(wxLineShape* lineShape)
{
    bool haveArrow = false;

    for (wxObjectList::compatibility_iterator node = lineShape->GetArrows().GetFirst();
         node;
         node = node->GetNext())
    {
      wxArrowHead* arrow = wxStaticCast(node->GetData(), wxArrowHead);
      if (ARROW_POSITION_MIDDLE == arrow->GetArrowEnd())
      {
        haveArrow = true;
      }
    }
    wxGetApp().GetDiagramToolBar()->ToggleTool(DIAGRAM_TOOLBAR_LINE_ARROW, haveArrow);
}

void csDiagramView::OnAlign(wxCommandEvent& event)
{
    // Make a copy of the selections, keeping only those shapes
    // that are top-level non-line shapes.
    wxList selections;
    wxObjectList::compatibility_iterator node;

    for (node = GetSelectionList().GetFirst();
         node;
         node = node->GetNext())
    {
        wxShape* shape = wxStaticCast(node->GetData(), wxShape);

        if ((shape->GetParent() == NULL) && !wxDynamicCast(shape, wxLineShape))
        {
            selections.Append(shape);
        }
    }

    if (selections.GetCount() == 0)
        return;

    csDiagramDocument* doc = GetDocument();
    csDiagramCommand* cmd = new csDiagramCommand(_("Align"), doc);

    node = selections.GetFirst();
    wxShape* firstShape = wxStaticCast(node->GetData(), wxShape);

    double x = firstShape->GetX();
    double y = firstShape->GetY();
    double width, height;
    firstShape->GetBoundingBoxMax(&width, &height);

    for (node = selections.GetFirst();
         node;
         node = node->GetNext())
    {
        wxShape* shape = wxStaticCast(node->GetData(), wxShape);
        if (shape != firstShape)
        {
            /* double x1 = */ shape->GetX();
            /* double y1 = */ shape->GetY();
            double width1, height1;
            shape->GetBoundingBoxMax(& width1, & height1);

            wxShape* newShape = shape->CreateNewCopy();

            switch (event.GetId())
            {
                case DIAGRAM_TOOLBAR_ALIGNL:
                {
                    double x2 = (double)(x - (width/2.0) + (width1/2.0));
                    newShape->SetX(x2);
                    break;
                }
                case DIAGRAM_TOOLBAR_ALIGNR:
                {
                    double x2 = (double)(x + (width/2.0) - (width1/2.0));
                    newShape->SetX(x2);
                    break;
                }
                case DIAGRAM_TOOLBAR_ALIGNB:
                {
                    double y2 = (double)(y + (height/2.0) - (height1/2.0));
                    newShape->SetY(y2);
                    break;
                }
                case DIAGRAM_TOOLBAR_ALIGNT:
                {
                    double y2 = (double)(y - (height/2.0) + (height1/2.0));
                    newShape->SetY(y2);
                    break;
                }
                case DIAGRAM_TOOLBAR_ALIGN_HORIZ:
                {
                    newShape->SetX(x);
                    break;
                }
                case DIAGRAM_TOOLBAR_ALIGN_VERT:
                {
                    newShape->SetY(y);
                    break;
                }
                case DIAGRAM_TOOLBAR_COPY_SIZE:
                {
                    newShape->SetSize(width, height);
                    break;
                }
            }
            csCommandState* state = new csCommandState(ID_CS_ALIGN, newShape, shape);
            cmd->AddState(state);
        }
    }
    doc->GetCommandProcessor()->Submit(cmd);
}

void csDiagramView::OnAlignUpdate(wxUpdateUIEvent& event)
{
    // This is an approximation, since there may be lines
    // amongst the selections.
    event.Enable( (m_selections.GetCount() > 1) ) ;
}

void csDiagramView::OnNewLinePoint(wxCommandEvent& WXUNUSED(event))
{
    csDiagramDocument* doc = GetDocument();
    csDiagramCommand* cmd = new csDiagramCommand(_("New line point"), doc);

    for (wxObjectList::compatibility_iterator node = m_selections.GetFirst();
         node;
         node = node->GetNext())
    {
        wxShape* shape = wxStaticCast(node->GetData(), wxShape);

        if (wxDynamicCast(shape, wxLineShape))
        {
            wxShape* newShape = shape->CreateNewCopy();

            wxStaticCast(newShape, wxLineShape)->InsertLineControlPoint(NULL);
            csCommandState* state = new csCommandState(ID_CS_NEW_POINT, newShape, shape);
            cmd->AddState(state);
        }
    }
    doc->GetCommandProcessor()->Submit(cmd);
}

void csDiagramView::OnCutLinePoint(wxCommandEvent& WXUNUSED(event))
{
    csDiagramDocument* doc = GetDocument();
    csDiagramCommand* cmd = new csDiagramCommand(_("Cut line point"), doc);

    for (wxObjectList::compatibility_iterator node = m_selections.GetFirst();
         node;
         node = node->GetNext())
    {
        wxShape* shape = wxStaticCast(node->GetData(), wxShape);

        if (wxDynamicCast(shape, wxLineShape))
        {
            wxShape* newShape = shape->CreateNewCopy();
            wxStaticCast(newShape, wxLineShape)->DeleteLineControlPoint();
            csCommandState* state = new csCommandState(ID_CS_CUT_POINT, newShape, shape);
            cmd->AddState(state);
        }
    }
    doc->GetCommandProcessor()->Submit(cmd);
}

void csDiagramView::OnStraightenLines(wxCommandEvent& WXUNUSED(event))
{
    csDiagramDocument* doc = GetDocument();
    csDiagramCommand* cmd = new csDiagramCommand(_("Straighten lines"), doc);

    for (wxObjectList::compatibility_iterator node = m_selections.GetFirst();
         node;
         node = node->GetNext())
    {
        wxShape* shape = wxStaticCast(node->GetData(), wxShape);

        if (wxDynamicCast(shape, wxLineShape))
        {
            wxShape* newShape = shape->CreateNewCopy();
            wxStaticCast(newShape, wxLineShape)->Straighten();
            csCommandState* state = new csCommandState(ID_CS_STRAIGHTEN, newShape, shape);
            cmd->AddState(state);
        }
    }
    doc->GetCommandProcessor()->Submit(cmd);
}

void csDiagramView::OnNewLinePointUpdate(wxUpdateUIEvent& event)
{
    wxList selections;

    FindSelectedShapes(selections, wxCLASSINFO(wxLineShape));
    event.Enable( (selections.GetCount() > 0) );
}

void csDiagramView::OnCutLinePointUpdate(wxUpdateUIEvent& event)
{
    wxList selections;

    FindSelectedShapes(selections, wxCLASSINFO(wxLineShape));
    event.Enable( (selections.GetCount() > 0) );
}

void csDiagramView::OnStraightenLinesUpdate(wxUpdateUIEvent& event)
{
    wxList selections;

    FindSelectedShapes(selections, wxCLASSINFO(wxLineShape));
    event.Enable( (selections.GetCount() > 0) );
}

/*
 * Window implementations
 */

IMPLEMENT_CLASS(csCanvas, wxShapeCanvas)

BEGIN_EVENT_TABLE(csCanvas, wxShapeCanvas)
    EVT_MOUSE_EVENTS(csCanvas::OnMouseEvent)
    EVT_PAINT(csCanvas::OnPaint)
END_EVENT_TABLE()

// Define a constructor for my m_canvas
csCanvas::csCanvas(csDiagramView* view, wxWindowID id, const wxPoint& pos, const wxSize& size, long style):
    wxShapeCanvas(view->GetFrame(), id, pos, size, style)
{
    m_view = view;
}

csCanvas::~csCanvas(void)
{
}

void csCanvas::DrawOutline(wxDC& dc, double x1, double y1, double x2, double y2)
{
    wxPen dottedPen(*wxBLACK, 1, wxPENSTYLE_DOT);
    dc.SetPen(dottedPen);
    dc.SetBrush(* wxTRANSPARENT_BRUSH);

    dc.DrawRectangle((wxCoord) x1, (wxCoord) y1, (wxCoord) (x2 - x1), (wxCoord) (y2 - y1));
}

void csCanvas::OnLeftClick(double x, double y, int WXUNUSED(keys))
{
    csEditorToolPalette *palette = wxGetApp().GetDiagramPalette();

    switch (palette->GetSelection())
    {
        case PALETTE_ARROW:
        {
           GetView()->SelectAll(false);

           wxClientDC dc(this);
           PrepareDC(dc);

           Redraw(dc);
           return;
        }
        case PALETTE_TEXT_TOOL:
        {
           wxString newLabel;

           // Ask for a label and create a new free-floating text region
           csLabelEditingDialog* dialog = new csLabelEditingDialog(GetParent());

           dialog->SetShapeLabel( wxEmptyString );
           dialog->SetTitle(_("New text box"));
           if (dialog->ShowModal() == wxID_CANCEL)
           {
               delete dialog;
               return;
           }

           newLabel = dialog->GetShapeLabel();
           delete dialog;
 
           wxShape* shape = new csTextBoxShape;
           shape->AssignNewIds();
           shape->SetEventHandler(new csEvtHandler(shape, shape, newLabel));

           wxComboBox* comboBox = wxGetApp().GetPointSizeComboBox();
           wxString str(comboBox->GetValue());
           long pointSize;
           str.ToLong( &pointSize );

           wxFont* newFont = wxTheFontList->FindOrCreateFont(pointSize,
                   shape->GetFont()->GetFamily(),
                   shape->GetFont()->GetStyle(),
                   shape->GetFont()->GetWeight(),
                   shape->GetFont()->GetUnderlined(),
                   shape->GetFont()->GetFaceName());

           shape->SetFont(newFont);

           shape->SetX(x);
           shape->SetY(y);

           csDiagramCommand* cmd = new csDiagramCommand(_("Text box"),
               wxStaticCast(GetView()->GetDocument(), csDiagramDocument),
               new csCommandState(ID_CS_ADD_SHAPE, shape, NULL));
           GetView()->GetDocument()->GetCommandProcessor()->Submit(cmd);

           palette->SetSelection(PALETTE_ARROW);

           return;
        }
    }

    csSymbol* symbol = wxGetApp().GetSymbolDatabase()->FindSymbol(palette->GetSelection());
    if (symbol)
    {
        wxShape* theShape = symbol->GetShape()->CreateNewCopy();

        wxComboBox* comboBox = wxGetApp().GetPointSizeComboBox();
        wxString str(comboBox->GetValue());
        long pointSize;
        str.ToLong( &pointSize );

        wxFont* newFont = wxTheFontList->FindOrCreateFont(pointSize,
                symbol->GetShape()->GetFont()->GetFamily(),
                symbol->GetShape()->GetFont()->GetStyle(),
                symbol->GetShape()->GetFont()->GetWeight(),
                symbol->GetShape()->GetFont()->GetUnderlined(),
                symbol->GetShape()->GetFont()->GetFaceName());

        theShape->SetFont(newFont);

        theShape->AssignNewIds();
        theShape->SetX(x);
        theShape->SetY(y);

        csDiagramCommand* cmd = new csDiagramCommand(symbol->GetName(),
            GetView()->GetDocument(),
            new csCommandState(ID_CS_ADD_SHAPE, theShape, NULL));
        GetView()->GetDocument()->GetCommandProcessor()->Submit(cmd);

        palette->SetSelection(PALETTE_ARROW);
    }
}

void csCanvas::OnRightClick(double WXUNUSED(x), double WXUNUSED(y), int WXUNUSED(keys))
{
}

// Initial point
static double sg_initialX, sg_initialY;

void csCanvas::OnDragLeft(bool WXUNUSED(draw), double x, double y, int WXUNUSED(keys))
{
    wxClientDC dc(this);
    PrepareDC(dc);

    dc.SetLogicalFunction(OGLRBLF);
    DrawOutline(dc, sg_initialX, sg_initialY, x, y);
}

void csCanvas::OnBeginDragLeft(double x, double y, int WXUNUSED(keys))
{
    sg_initialX = x;
    sg_initialY = y;

    wxClientDC dc(this);
    PrepareDC(dc);

    dc.SetLogicalFunction(OGLRBLF);
    DrawOutline(dc, sg_initialX, sg_initialY, x, y);
    CaptureMouse();
}

void csCanvas::OnEndDragLeft(double x, double y, int WXUNUSED(keys))
{
    ReleaseMouse();

    wxClientDC dc(this);
    PrepareDC(dc);

    // Select all images within the rectangle
    float min_x, max_x, min_y, max_y;
    min_x = wxMin(x, sg_initialX);
    max_x = wxMax(x, sg_initialX);
    min_y = wxMin(y, sg_initialY);
    max_y = wxMax(y, sg_initialY);

    for (wxObjectList::compatibility_iterator node = GetDiagram()->GetShapeList()->GetFirst();
         node;
         node = node->GetNext())
    {
        wxShape *shape = wxStaticCast(node->GetData(), wxShape);

        if (shape->GetParent() == NULL && !wxDynamicCast(shape, wxControlPoint))
        {
            float image_x = shape->GetX();
            float image_y = shape->GetY();
            if (image_x >= min_x && image_x <= max_x &&
                image_y >= min_y && image_y <= max_y)
            {
                shape->Select(true, &dc);
                GetView()->SelectShape(shape, true);
            }
        }
    }
}

void csCanvas::OnDragRight(bool WXUNUSED(draw), double WXUNUSED(x), double WXUNUSED(y), int WXUNUSED(keys))
{
}

void csCanvas::OnBeginDragRight(double WXUNUSED(x), double WXUNUSED(y), int WXUNUSED(keys))
{
}

void csCanvas::OnEndDragRight(double WXUNUSED(x), double WXUNUSED(y), int WXUNUSED(keys))
{
}

void csCanvas::OnMouseEvent(wxMouseEvent& event)
{
    wxShapeCanvas::OnMouseEvent(event);
}

void csCanvas::OnPaint(wxPaintEvent& event)
{
//  if (GetDiagram())
    wxShapeCanvas::OnPaint(event);
}
