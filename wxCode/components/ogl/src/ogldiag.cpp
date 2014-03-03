/////////////////////////////////////////////////////////////////////////////
// Name:        ogldiag.cpp
// Purpose:     wxDiagram
// Author:      Julian Smart
// Modified by: Troels K
// Created:     12/07/98
// RCS-ID:      $Id: ogldiag.cpp,v 1.1 2007/03/28 15:15:57 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include <wx/ogl/ogl.h>

IMPLEMENT_DYNAMIC_CLASS(wxDiagram, wxObject)

// Object canvas
wxDiagram::wxDiagram() : wxObject()
{
  m_diagramCanvas = NULL;
  m_quickEditMode = false;
  m_snapToGrid = true;
  m_gridSpacing = 5.0;
  m_shapeList = new wxList;
  m_mouseTolerance = DEFAULT_MOUSE_TOLERANCE;
}

wxDiagram::~wxDiagram()
{
  if (m_shapeList)
    delete m_shapeList;
}

void wxDiagram::SetSnapToGrid(bool snap)
{
  m_snapToGrid = snap;
}

void wxDiagram::SetGridSpacing(double spacing)
{
  m_gridSpacing = spacing;
}

void wxDiagram::Snap(double *x, double *y)
{
  if (m_snapToGrid)
  {
    *x = m_gridSpacing * ((int)(*x/m_gridSpacing + 0.5));
    *y = m_gridSpacing * ((int)(*y/m_gridSpacing + 0.5));
  }
}


void wxDiagram::Redraw(wxDC& dc)
{
  if (m_shapeList)
  {
    if (GetCanvas())
      GetCanvas()->SetCursor(* wxHOURGLASS_CURSOR);

    for (wxObjectList::const_iterator it = m_shapeList->begin();
         it != m_shapeList->end();
         it++)
    {
      wxShape* object = wxStaticCast(*it, wxShape);
      if (!object->GetParent())
        object->Draw(dc);
    }
    if (GetCanvas())
      GetCanvas()->SetCursor(* wxSTANDARD_CURSOR);
  }
}

void wxDiagram::Clear(wxDC& dc)
{
  dc.Clear();
}

// Insert object after addAfter, or at end of list.
void wxDiagram::AddShape(wxShape *object, wxShape *addAfter)
{
  wxObjectList::compatibility_iterator nodeAfter = NULL;
  if (addAfter)
    nodeAfter = m_shapeList->Find(addAfter);

  if (!m_shapeList->Member(object))
  {
    if (nodeAfter)
    {
      if (nodeAfter->GetNext())
        m_shapeList->Insert(nodeAfter->GetNext(), object);
      else
        m_shapeList->Append(object);
    }
    else
      m_shapeList->Append(object);
    object->SetCanvas(GetCanvas());
  }
}

void wxDiagram::InsertShape(wxShape *object)
{
  m_shapeList->Insert(object);
  object->SetCanvas(GetCanvas());
}

void wxDiagram::RemoveShape(wxShape *object)
{
  m_shapeList->DeleteObject(object);
}

// Should this delete the actual objects too? I think not.
void wxDiagram::RemoveAllShapes()
{
  m_shapeList->Clear();
}

void wxDiagram::DeleteAllShapes()
{
  for (wxObjectList::compatibility_iterator node = m_shapeList->GetFirst();
       node;
       )
  {
    wxShape* shape = wxStaticCast(node->GetData(), wxShape);
    if (!shape->GetParent())
    {
      RemoveShape(shape);
      delete shape;
      node = m_shapeList->GetFirst();
    }
    else
    {
      node = node->GetNext();
    }
  }
}

void wxDiagram::ShowAll(bool show)
{
  for (wxObjectList::const_iterator it = m_shapeList->begin();
       it != m_shapeList->end();
       it++)
  {
    wxShape* object = wxStaticCast(*it, wxShape);
    object->Show(show);
  }
}

void wxDiagram::DrawOutline(wxDC& dc, double x1, double y1, double x2, double y2)
{
  wxPen dottedPen(*wxBLACK, 1, wxPENSTYLE_DOT);
  dc.SetPen(dottedPen);
  dc.SetBrush((* wxTRANSPARENT_BRUSH));

  wxPoint points[5];

  points[0].x = (int) x1;
  points[0].y = (int) y1;

  points[1].x = (int) x2;
  points[1].y = (int) y1;

  points[2].x = (int) x2;
  points[2].y = (int) y2;

  points[3].x = (int) x1;
  points[3].y = (int) y2;

  points[4].x = (int) x1;
  points[4].y = (int) y1;
  dc.DrawLines(5, points);
}

// Make sure all text that should be centred, is centred.
void wxDiagram::RecentreAll(wxDC& dc)
{
  for (wxObjectList::const_iterator it = m_shapeList->begin();
       it != m_shapeList->end();
       it++)
  {
    wxShape* obj = wxStaticCast(*it, wxShape);
    obj->Recentre(dc);
  }
}

// Input/output
bool wxDiagram::SaveFile(const wxString& filename)
{
  wxBeginBusyCursor();

  wxXmlDocument doc;

  wxXmlNode *root = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("diagram"));
  doc.SetRoot(root);

  for (wxObjectList::const_iterator it = m_shapeList->begin();
       it != m_shapeList->end();
       it++)
  {
    wxShape* shape = wxStaticCast(*it, wxShape);

    if (!wxDynamicCast(shape, wxControlPoint))
    {
      wxString expr;
      if (wxDynamicCast(shape, wxLineShape))
        expr = wxT("line");
       else
        expr = wxT("shape");

      wxXmlNode* item = new wxXmlNode(wxXML_ELEMENT_NODE, expr);
      OnShapeSave(item, *shape);
      root->AddChild(item);
    }
  }
  bool ok = doc.Save(filename);
  wxEndBusyCursor();
  return ok;
}

extern wxXmlNode* xmlFindChild(wxXmlNode* root, const wxString& childname, int id = wxNOT_FOUND);

bool wxDiagram::LoadFile(const wxString& filename)
{
  wxBeginBusyCursor();

  wxXmlDocument doc;
  bool ok = doc.Load(filename);
  if (ok)
  {
     DeleteAllShapes();

     wxXmlNode* diagram = doc.GetRoot();
     bool ok = diagram && (diagram->GetName() == wxT("diagram"));
     if (ok)
     {
        for (wxXmlNode* node = diagram->GetChildren(); node; node = node->GetNext())
        {
          long id;
          if (node->GetAttribute(wxT("id"), wxEmptyString).ToLong(&id)) ::wxRegisterId(id);
        }

        ReadNodes(doc.GetRoot());
        ReadContainerGeometry(doc.GetRoot());
        ReadLines(doc.GetRoot());
     }
  }
  wxEndBusyCursor();
  return ok;
}

void wxDiagram::ReadNodes(wxXmlNode* node)
{
  // Find and create the node images
  for (wxXmlNode* clause = ::xmlFindChild(node, wxT("shape"));
       clause;
       clause = clause->GetNext())
  {
    long parentId = wxNOT_FOUND;

    wxString type = clause->GetAttribute(wxT("type"), wxEmptyString);
    clause->GetAttribute(wxT("parent"), wxEmptyString).ToLong(&parentId);
    wxClassInfo *classInfo = wxClassInfo::FindClass(type);
    if (classInfo)
    {
      wxShape* shape = wxStaticCast(classInfo->CreateObject(), wxShape);
      OnShapeLoad(clause, shape);

      shape->SetCanvas(GetCanvas());
      shape->Show(true);

      m_shapeList->Append(shape);

      // If child of composite, link up
      if (parentId > wxNOT_FOUND)
      {
         wxXmlNode* parentExpr = ::xmlFindChild(node, wxT("shape"), parentId);
        if (parentExpr)
        {
          //wxShape *parent = (wxShape *)parentExpr->GetClientData();
          //shape->SetParent(parent);
          //parent->GetChildren().Append(shape);
        }
      }
      //clause->SetClientData(shape);
    }
  }
}

void wxDiagram::ReadLines(wxXmlNode* node)
{
  for (wxXmlNode* clause = ::xmlFindChild(node, wxT("line"));
       clause;
       clause = clause->GetNext())
  {
    wxString type;
    long parentId = wxNOT_FOUND;

    type = clause->GetAttribute(wxT("type"), wxEmptyString);
    clause->GetAttribute(wxT("parent"), wxEmptyString).ToLong(&parentId);
    wxClassInfo *classInfo = wxClassInfo::FindClass(type);
    if (classInfo)
    {
      wxLineShape* shape = wxStaticCast(classInfo->CreateObject(), wxLineShape);
      shape->Show(true);

      OnShapeLoad(clause, shape);
      shape->SetCanvas(GetCanvas());

      long image_to = wxNOT_FOUND; long image_from = wxNOT_FOUND;
      clause->GetAttribute(wxT("to"), wxEmptyString).ToLong(&image_to);
      clause->GetAttribute(wxT("from"), wxEmptyString).ToLong(&image_from);

      wxXmlNode*image_to_expr = xmlFindChild(node, wxT("shape"), image_to);
      if (!image_to_expr)
      {
        // Error
      }
      wxXmlNode*image_from_expr = xmlFindChild(node, wxT("shape"), image_from);
      if (!image_from_expr)
      {
        // Error
      }

      if (image_to_expr && image_from_expr)
      {
        //wxShape *image_to_object = (wxShape *)image_to_expr->GetClientData();
        //wxShape *image_from_object = (wxShape *)image_from_expr->GetClientData();
        //if (image_to_object && image_from_object)
        {
          //image_from_object->AddLine(shape, image_to_object, shape->GetAttachmentFrom(), shape->GetAttachmentTo());
        }
      }
      //clause->SetClientData(shape);
      m_shapeList->Append(shape);
    }
  }
}

// Containers have divisions that reference adjoining divisions,
// so we need a separate pass to link everything up.
// Also used by Symbol Library.
void wxDiagram::ReadContainerGeometry(wxXmlNode* node)
{
  for (wxXmlNode* clause = ::xmlFindChild(node, wxT("shape"));
       clause;
       clause = clause->GetNext())
  {
    wxShape *image = NULL;//(wxShape *)clause->GetClientData();

    if (wxDynamicCast(image, wxCompositeShape))
    {
      //wxCompositeShape *composite = (wxCompositeShape *)image;

      // Find the list of divisions in the composite
      wxXmlNode* divisionExpr = xmlFindChild(clause, wxT("divisions"));
      if (divisionExpr)
      {
         /*
        int i = 0;
        wxExpr *idExpr = divisionExpr->Nth(i);
        while (idExpr)
        {
          long divisionId = idExpr->IntegerValue();
          wxExpr *childExpr = database.HashFind(wxT("shape"), divisionId);
          if (childExpr && childExpr->GetClientData())
          {
            wxDivisionShape *child = (wxDivisionShape *)childExpr->GetClientData();
            composite->GetDivisions().Append(child);

            // Find the adjoining shapes
            long leftSideId = -1;
            long topSideId = -1;
            long rightSideId = -1;
            long bottomSideId = -1;
            childExpr->GetAttributeValue(wxT("left_side"), leftSideId);
            childExpr->GetAttributeValue(wxT("top_side"), topSideId);
            childExpr->GetAttributeValue(wxT("right_side"), rightSideId);
            childExpr->GetAttributeValue(wxT("bottom_side"), bottomSideId);
            if (leftSideId > -1)
            {
              wxExpr *leftExpr = database.HashFind(wxT("shape"), leftSideId);
              if (leftExpr && leftExpr->GetClientData())
              {
                wxDivisionShape *leftSide = (wxDivisionShape *)leftExpr->GetClientData();
                child->SetLeftSide(leftSide);
              }
            }
            if (topSideId > -1)
            {
              wxExpr *topExpr = database.HashFind(wxT("shape"), topSideId);
              if (topExpr && topExpr->GetClientData())
              {
                wxDivisionShape *topSide = (wxDivisionShape *)topExpr->GetClientData();
                child->SetTopSide(topSide);
              }
            }
            if (rightSideId > -1)
            {
              wxExpr *rightExpr = database.HashFind(wxT("shape"), rightSideId);
              if (rightExpr && rightExpr->GetClientData())
              {
                wxDivisionShape *rightSide = (wxDivisionShape *)rightExpr->GetClientData();
                child->SetRightSide(rightSide);
              }
            }
            if (bottomSideId > -1)
            {
              wxExpr *bottomExpr = database.HashFind(wxT("shape"), bottomSideId);
              if (bottomExpr && bottomExpr->GetClientData())
              {
                wxDivisionShape *bottomSide = (wxDivisionShape *)bottomExpr->GetClientData();
                child->SetBottomSide(bottomSide);
              }
            }
          }
          i ++;
          idExpr = divisionExpr->Nth(i);
        }
          */
      }
    }
  }
}

bool wxDiagram::OnShapeSave(wxXmlNode*expr, const wxShape& shape)
{
  shape.WriteAttributes(expr);

  if (wxDynamicCast(&shape, wxCompositeShape))
  {
    for (wxObjectList::const_iterator it = shape.GetChildren().begin();
         it != shape.GetChildren().end();
         it++)
    {
      wxShape* childShape = wxStaticCast(*it, wxShape);
      wxXmlNode* childExpr = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("shape"));
      OnShapeSave(childExpr, *childShape);
      expr->AddChild(childExpr);
    }
  }
  return true;
}

bool wxDiagram::OnShapeLoad(wxXmlNode*expr, wxShape* shape)
{
  shape->ReadAttributes(expr);
  return true;
}

void wxDiagram::SetCanvas(wxShapeCanvas *can)
{
  m_diagramCanvas = can;
}

// Find a shape by its id
wxShape* wxDiagram::FindShape(long id) const
{
    for (wxObjectList::const_iterator it = GetShapeList()->begin();
         it != GetShapeList()->end();
         it++)
    {
        wxShape* shape = wxStaticCast(*it, wxShape);
        if (shape->GetId() == id)
        {
            return shape;
        }
    }
    return NULL;
}


//// Crossings classes

wxLineCrossings::wxLineCrossings()
{
}

wxLineCrossings::~wxLineCrossings()
{
    ClearCrossings();
}

void wxLineCrossings::FindCrossings(wxDiagram& diagram)
{
    ClearCrossings();
    for (wxObjectList::const_iterator it1 = diagram.GetShapeList()->begin();
         it1 != diagram.GetShapeList()->end();
         it1++)
    {
        wxShape* shape1 = wxStaticCast(*it1, wxShape);

        if (wxDynamicCast(shape1, wxLineShape))
        {
            wxLineShape* lineShape1 = wxStaticCast(shape1, wxLineShape);
            // Iterate through the segments
            wxList* pts1 = lineShape1->GetLineControlPoints();
            size_t i;
            for (i = 0; i < (pts1->GetCount() - 1); i++)
            {
                wxRealPoint* pt1_a = (wxRealPoint*) (pts1->Item(i)->GetData());
                wxRealPoint* pt1_b = (wxRealPoint*) (pts1->Item(i+1)->GetData());

                // Now we iterate through the segments again

                for (wxObjectList::const_iterator it2 = diagram.GetShapeList()->begin();
                     it2 != diagram.GetShapeList()->end();
                     it2++)
                {
                    wxShape* shape2 = wxStaticCast(*it2, wxShape);

                    // Assume that the same line doesn't cross itself
                    if (wxDynamicCast(shape2, wxLineShape) && (shape1 != shape2))
                    {
                        wxLineShape* lineShape2 = wxStaticCast(shape2, wxLineShape);
                        // Iterate through the segments
                        wxList* pts2 = lineShape2->GetLineControlPoints();
                        int j;
                        for (j = 0; j < (int) (pts2->GetCount() - 1); j++)
                        {
                            wxRealPoint* pt2_a = (wxRealPoint*) (pts2->Item(j)->GetData());
                            wxRealPoint* pt2_b = (wxRealPoint*) (pts2->Item(j+1)->GetData());

                            // Now let's see if these two segments cross.
                            double ratio1, ratio2;
                            oglCheckLineIntersection(pt1_a->x, pt1_a->y, pt1_b->x, pt1_b->y,
                               pt2_a->x, pt2_a->y, pt2_b->x, pt2_b->y,
                             & ratio1, & ratio2);

                            if ((ratio1 < 1.0) && (ratio1 > -1.0))
                            {
                                // Intersection!
                                wxLineCrossing* crossing = new wxLineCrossing;
                                crossing->m_intersect.x = (pt1_a->x + (pt1_b->x - pt1_a->x)*ratio1);
                                crossing->m_intersect.y = (pt1_a->y + (pt1_b->y - pt1_a->y)*ratio1);

                                crossing->m_pt1 = * pt1_a;
                                crossing->m_pt2 = * pt1_b;
                                crossing->m_pt3 = * pt2_a;
                                crossing->m_pt4 = * pt2_b;

                                crossing->m_lineShape1 = lineShape1;
                                crossing->m_lineShape2 = lineShape2;

                                m_crossings.Append(crossing);
                            }
                        }
                    }
                }
            }
        }
    }
}

void wxLineCrossings::DrawCrossings(wxDiagram& WXUNUSED(diagram), wxDC& dc)
{
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    long arcWidth = 8;

    for (wxObjectList::const_iterator it = m_crossings.begin();
         it != m_crossings.end();
         it++)
    {
        wxLineCrossing* crossing = (wxLineCrossing*)*it;
//        dc.DrawEllipse((long) (crossing->m_intersect.x - (arcWidth/2.0) + 0.5), (long) (crossing->m_intersect.y - (arcWidth/2.0) + 0.5),
//           arcWidth, arcWidth);


        // Let's do some geometry to find the points on either end of the arc.
/*

(x1, y1)
    |\
    | \
    |  \
    |   \
    |    \
    |    |\ c    c1
    |  a | \
         |  \
    |     -  x <-- centre of arc
 a1 |     b  |\
    |        | \ c2
    |     a2 |  \
    |         -  \
    |         b2  \
    |              \
    |_______________\ (x2, y2)
          b1

*/

        double a1 = wxMax(crossing->m_pt1.y, crossing->m_pt2.y) - wxMin(crossing->m_pt1.y, crossing->m_pt2.y) ;
        double b1 = wxMax(crossing->m_pt1.x, crossing->m_pt2.x) - wxMin(crossing->m_pt1.x, crossing->m_pt2.x) ;
        double c1 = sqrt( (a1*a1) + (b1*b1) );

        double c = arcWidth / 2.0;
        double a = c * a1/c1 ;
        double b = c * b1/c1 ;

        // I'm not sure this is right, since we don't know which direction we should be going in - need
        // to know which way the line slopes and choose the sign appropriately.
        double arcX1 = crossing->m_intersect.x - b;
        double arcY1 = crossing->m_intersect.y - a;

        double arcX2 = crossing->m_intersect.x + b;
        double arcY2 = crossing->m_intersect.y + a;

        dc.SetPen(*wxBLACK_PEN);
        dc.DrawArc( (long) arcX1, (long) arcY1, (long) arcX2, (long) arcY2,
            (long) crossing->m_intersect.x, (long) crossing->m_intersect.y);

        dc.SetPen(*wxWHITE_PEN);
        dc.DrawLine( (long) arcX1, (long) arcY1, (long) arcX2, (long) arcY2 );
    }
}

void wxLineCrossings::ClearCrossings()
{
    for (wxObjectList::const_iterator it = m_crossings.begin();
         it != m_crossings.end();
         it++)
    {
        wxLineCrossing* crossing = (wxLineCrossing*)*it;
        delete crossing;
    }
    m_crossings.Clear();
}

