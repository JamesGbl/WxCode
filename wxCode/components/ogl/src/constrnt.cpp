/////////////////////////////////////////////////////////////////////////////
// Name:        constrnt.cpp
// Purpose:     OGL Constraint classes
// Author:      Julian Smart
// Modified by: Troels K
// Created:     12/07/98
// RCS-ID:      $Id: constrnt.cpp,v 1.1 2007/03/28 15:15:57 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include <wx/ogl/ogl.h>

wxList *wxOGLConstraintTypes = NULL;

/*
 * Constraint type
 *
 */

IMPLEMENT_DYNAMIC_CLASS(wxOGLConstraintType, wxObject)

wxOGLConstraintType::wxOGLConstraintType(int theType, const wxString& theName, const wxString& thePhrase)
{
  m_type = theType;
  m_name = theName;
  m_phrase = thePhrase;
}

wxOGLConstraintType::~wxOGLConstraintType()
{
}

void OGLInitializeConstraintTypes()
{
    if (!wxOGLConstraintTypes)
        return;

    wxOGLConstraintTypes = new wxList(wxKEY_INTEGER);

    wxOGLConstraintTypes->Append(gyCONSTRAINT_CENTRED_VERTICALLY,
        new wxOGLConstraintType(gyCONSTRAINT_CENTRED_VERTICALLY, wxT("Centre vertically"), wxT("centred vertically w.r.t.")));

    wxOGLConstraintTypes->Append(gyCONSTRAINT_CENTRED_HORIZONTALLY,
        new wxOGLConstraintType(gyCONSTRAINT_CENTRED_HORIZONTALLY, wxT("Centre horizontally"), wxT("centred horizontally w.r.t.")));

    wxOGLConstraintTypes->Append(gyCONSTRAINT_CENTRED_BOTH,
        new wxOGLConstraintType(gyCONSTRAINT_CENTRED_BOTH, wxT("Centre"), wxT("centred w.r.t.")));

    wxOGLConstraintTypes->Append(gyCONSTRAINT_LEFT_OF,
        new wxOGLConstraintType(gyCONSTRAINT_LEFT_OF, wxT("Left of"), wxT("left of")));

    wxOGLConstraintTypes->Append(gyCONSTRAINT_RIGHT_OF,
        new wxOGLConstraintType(gyCONSTRAINT_RIGHT_OF, wxT("Right of"), wxT("right of")));

    wxOGLConstraintTypes->Append(gyCONSTRAINT_ABOVE,
        new wxOGLConstraintType(gyCONSTRAINT_ABOVE, wxT("Above"), wxT("above")));

    wxOGLConstraintTypes->Append(gyCONSTRAINT_BELOW,
        new wxOGLConstraintType(gyCONSTRAINT_BELOW, wxT("Below"), wxT("below")));

    // Alignment
    wxOGLConstraintTypes->Append(gyCONSTRAINT_ALIGNED_TOP,
        new wxOGLConstraintType(gyCONSTRAINT_ALIGNED_TOP, wxT("Top-aligned"), wxT("aligned to the top of")));

    wxOGLConstraintTypes->Append(gyCONSTRAINT_ALIGNED_BOTTOM,
        new wxOGLConstraintType(gyCONSTRAINT_ALIGNED_BOTTOM, wxT("Bottom-aligned"), wxT("aligned to the bottom of")));

    wxOGLConstraintTypes->Append(gyCONSTRAINT_ALIGNED_LEFT,
        new wxOGLConstraintType(gyCONSTRAINT_ALIGNED_LEFT, wxT("Left-aligned"), wxT("aligned to the left of")));

    wxOGLConstraintTypes->Append(gyCONSTRAINT_ALIGNED_RIGHT,
        new wxOGLConstraintType(gyCONSTRAINT_ALIGNED_RIGHT, wxT("Right-aligned"), wxT("aligned to the right of")));

    // Mid-alignment
    wxOGLConstraintTypes->Append(gyCONSTRAINT_MIDALIGNED_TOP,
        new wxOGLConstraintType(gyCONSTRAINT_MIDALIGNED_TOP, wxT("Top-midaligned"), wxT("centred on the top of")));

    wxOGLConstraintTypes->Append(gyCONSTRAINT_MIDALIGNED_BOTTOM,
        new wxOGLConstraintType(gyCONSTRAINT_MIDALIGNED_BOTTOM, wxT("Bottom-midaligned"), wxT("centred on the bottom of")));

    wxOGLConstraintTypes->Append(gyCONSTRAINT_MIDALIGNED_LEFT,
        new wxOGLConstraintType(gyCONSTRAINT_MIDALIGNED_LEFT, wxT("Left-midaligned"), wxT("centred on the left of")));

    wxOGLConstraintTypes->Append(gyCONSTRAINT_MIDALIGNED_RIGHT,
        new wxOGLConstraintType(gyCONSTRAINT_MIDALIGNED_RIGHT, wxT("Right-midaligned"), wxT("centred on the right of")));
}

void OGLCleanUpConstraintTypes()
{
    if (!wxOGLConstraintTypes)
        return;

    for (wxObjectList::const_iterator it = wxOGLConstraintTypes->begin();
         it != wxOGLConstraintTypes->end();
         it++)
    {
        wxOGLConstraintType* ct = wxStaticCast(*it, wxOGLConstraintType);
        delete ct;
    }
    wxDELETE(wxOGLConstraintTypes);
}

/*
 * Constraint Stuff
 *
 */

IMPLEMENT_DYNAMIC_CLASS(wxOGLConstraint, wxObject)

wxOGLConstraint::wxOGLConstraint(int type, wxShape *constraining, wxList& constrained)
{
  m_xSpacing = 0.0;
  m_ySpacing = 0.0;

  m_constraintType = type;
  m_constrainingObject = constraining;

  m_constraintId = 0;
  m_constraintName = wxT("noname");

  for (wxObjectList::const_iterator it = constrained.begin();
       it != constrained.end();
       it++)
  {
    m_constrainedObjects.Append(*it);
  }
}

wxOGLConstraint::~wxOGLConstraint()
{
}

bool wxOGLConstraint::Equals(double a, double b)
{
  double marg = 0.5;

  bool eq = ((b <= a + marg) && (b >= a - marg));
  return eq;
}

// Return true if anything changed
bool wxOGLConstraint::Evaluate()
{
  double maxWidth, maxHeight, minWidth, minHeight, x, y;
  m_constrainingObject->GetBoundingBoxMax(&maxWidth, &maxHeight);
  m_constrainingObject->GetBoundingBoxMin(&minWidth, &minHeight);
  x = m_constrainingObject->GetX();
  y = m_constrainingObject->GetY();

  wxClientDC dc(m_constrainingObject->GetCanvas());
  m_constrainingObject->GetCanvas()->PrepareDC(dc);

  switch (m_constraintType)
  {
    case gyCONSTRAINT_CENTRED_VERTICALLY:
    {
      int n = m_constrainedObjects.GetCount();
      double totalObjectHeight = 0.0;
      wxObjectList::const_iterator it;

      for (it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);

        double width2, height2;
        constrainedObject->GetBoundingBoxMax(&width2, &height2);
        totalObjectHeight += height2;
      }
      double startY;
      double spacingY;
      // Check if within the constraining object...
      if ((totalObjectHeight + (n + 1)*m_ySpacing) <= minHeight)
      {
        spacingY = (double)((minHeight - totalObjectHeight)/(n + 1));
        startY = (double)(y - (minHeight/2.0));
      }
      // Otherwise, use default spacing
      else
      {
        spacingY = m_ySpacing;
        startY = (double)(y - ((totalObjectHeight + (n+1)*spacingY)/2.0));
      }

      // Now position the objects
      bool changed = false;
      for (it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);
        double width2, height2;
        constrainedObject->GetBoundingBoxMax(&width2, &height2);
        startY += (double)(spacingY + (height2/2.0));
        if (!Equals(startY, constrainedObject->GetY()))
        {
          constrainedObject->Move(dc, constrainedObject->GetX(), startY, false);
          changed = true;
        }
        startY += (double)(height2/2.0);
      }
      return changed;
    }
    case gyCONSTRAINT_CENTRED_HORIZONTALLY:
    {
      int n = m_constrainedObjects.GetCount();
      double totalObjectWidth = 0.0;
      wxObjectList::const_iterator it;

      for (it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);

        double width2, height2;
        constrainedObject->GetBoundingBoxMax(&width2, &height2);
        totalObjectWidth += width2;
      }
      double startX;
      double spacingX;
      // Check if within the constraining object...
      if ((totalObjectWidth + (n + 1)*m_xSpacing) <= minWidth)
      {
        spacingX = (double)((minWidth - totalObjectWidth)/(n + 1));
        startX = (double)(x - (minWidth/2.0));
      }
      // Otherwise, use default spacing
      else
      {
        spacingX = m_xSpacing;
        startX = (double)(x - ((totalObjectWidth + (n+1)*spacingX)/2.0));
      }

      // Now position the objects
      bool changed = false;
      for (it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);
        double width2, height2;
        constrainedObject->GetBoundingBoxMax(&width2, &height2);
        startX += (double)(spacingX + (width2/2.0));
        if (!Equals(startX, constrainedObject->GetX()))
        {
          constrainedObject->Move(dc, startX, constrainedObject->GetY(), false);
          changed = true;
        }
        startX += (double)(width2/2.0);
      }
      return changed;
    }
    case gyCONSTRAINT_CENTRED_BOTH:
    {
      int n = m_constrainedObjects.GetCount();
      double totalObjectWidth = 0.0;
      double totalObjectHeight = 0.0;
      wxObjectList::const_iterator it;
      
      for (it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);

        double width2, height2;
        constrainedObject->GetBoundingBoxMax(&width2, &height2);
        totalObjectWidth += width2;
        totalObjectHeight += height2;
      }
      double startX;
      double spacingX;
      double startY;
      double spacingY;

      // Check if within the constraining object...
      if ((totalObjectWidth + (n + 1)*m_xSpacing) <= minWidth)
      {
        spacingX = (double)((minWidth - totalObjectWidth)/(n + 1));
        startX = (double)(x - (minWidth/2.0));
      }
      // Otherwise, use default spacing
      else
      {
        spacingX = m_xSpacing;
        startX = (double)(x - ((totalObjectWidth + (n+1)*spacingX)/2.0));
      }

      // Check if within the constraining object...
      if ((totalObjectHeight + (n + 1)*m_ySpacing) <= minHeight)
      {
        spacingY = (double)((minHeight - totalObjectHeight)/(n + 1));
        startY = (double)(y - (minHeight/2.0));
      }
      // Otherwise, use default spacing
      else
      {
        spacingY = m_ySpacing;
        startY = (double)(y - ((totalObjectHeight + (n+1)*spacingY)/2.0));
      }

      // Now position the objects
      bool changed = false;
      for (it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);
        double width2, height2;
        constrainedObject->GetBoundingBoxMax(&width2, &height2);
        startX += (double)(spacingX + (width2/2.0));
        startY += (double)(spacingY + (height2/2.0));

        if ((!Equals(startX, constrainedObject->GetX())) || (!Equals(startY, constrainedObject->GetY())))
        {
          constrainedObject->Move(dc, startX, startY, false);
          changed = true;
        }

        startX += (double)(width2/2.0);
        startY += (double)(height2/2.0);
      }
      return changed;
    }
    case gyCONSTRAINT_LEFT_OF:
    {
      bool changed = false;

      for (wxObjectList::const_iterator it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);

        double width2, height2;
        constrainedObject->GetBoundingBoxMax(&width2, &height2);

        double x3 = (double)(x - (minWidth/2.0) - (width2/2.0) - m_xSpacing);
        if (!Equals(x3, constrainedObject->GetX()))
        {
          changed = true;
          constrainedObject->Move(dc, x3, constrainedObject->GetY(), false);
        }
      }
      return changed;
    }
    case gyCONSTRAINT_RIGHT_OF:
    {
      bool changed = false;

      for (wxObjectList::const_iterator it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);

        double width2, height2;
        constrainedObject->GetBoundingBoxMax(&width2, &height2);

        double x3 = (double)(x + (minWidth/2.0) + (width2/2.0) + m_xSpacing);
        if (!Equals(x3, constrainedObject->GetX()))
        {
          changed = true;
          constrainedObject->Move(dc, x3, constrainedObject->GetY(), false);
        }
      }
      return changed;
    }
    case gyCONSTRAINT_ABOVE:
    {
      bool changed = false;

      for (wxObjectList::const_iterator it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);

        double width2, height2;
        constrainedObject->GetBoundingBoxMax(&width2, &height2);

        double y3 = (double)(y - (minHeight/2.0) - (height2/2.0) - m_ySpacing);
        if (!Equals(y3, constrainedObject->GetY()))
        {
          changed = true;
          constrainedObject->Move(dc, constrainedObject->GetX(), y3, false);
        }
      }
      return changed;
    }
    case gyCONSTRAINT_BELOW:
    {
      bool changed = false;

      for (wxObjectList::const_iterator it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);

        double width2, height2;
        constrainedObject->GetBoundingBoxMax(&width2, &height2);

        double y3 = (double)(y + (minHeight/2.0) + (height2/2.0) + m_ySpacing);
        if (!Equals(y3, constrainedObject->GetY()))
        {
          changed = true;
          constrainedObject->Move(dc, constrainedObject->GetX(), y3, false);
        }
      }
      return changed;
    }
    case gyCONSTRAINT_ALIGNED_LEFT:
    {
      bool changed = false;

      for (wxObjectList::const_iterator it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);

        double width2, height2;
        constrainedObject->GetBoundingBoxMax(&width2, &height2);

        double x3 = (double)(x - (minWidth/2.0) + (width2/2.0) + m_xSpacing);
        if (!Equals(x3, constrainedObject->GetX()))
        {
          changed = true;
          constrainedObject->Move(dc, x3, constrainedObject->GetY(), false);
        }
      }
      return changed;
    }
    case gyCONSTRAINT_ALIGNED_RIGHT:
    {
      bool changed = false;

      for (wxObjectList::const_iterator it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);

        double width2, height2;
        constrainedObject->GetBoundingBoxMax(&width2, &height2);

        double x3 = (double)(x + (minWidth/2.0) - (width2/2.0) - m_xSpacing);
        if (!Equals(x3, constrainedObject->GetX()))
        {
          changed = true;
          constrainedObject->Move(dc, x3, constrainedObject->GetY(), false);
        }
      }
      return changed;
      #if 0
      // two returned values ?
      return false;
      #endif
    }
    case gyCONSTRAINT_ALIGNED_TOP:
    {
      bool changed = false;

      for (wxObjectList::const_iterator it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);

        double width2, height2;
        constrainedObject->GetBoundingBoxMax(&width2, &height2);

        double y3 = (double)(y - (minHeight/2.0) + (height2/2.0) + m_ySpacing);
        if (!Equals(y3, constrainedObject->GetY()))
        {
          changed = true;
          constrainedObject->Move(dc, constrainedObject->GetX(), y3, false);
        }
      }
      return changed;
    }
    case gyCONSTRAINT_ALIGNED_BOTTOM:
    {
      bool changed = false;

      for (wxObjectList::const_iterator it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);

        double width2, height2;
        constrainedObject->GetBoundingBoxMax(&width2, &height2);

        double y3 = (double)(y + (minHeight/2.0) - (height2/2.0) - m_ySpacing);
        if (!Equals(y3, constrainedObject->GetY()))
        {
          changed = true;
          constrainedObject->Move(dc, constrainedObject->GetX(), y3, false);
        }
      }
      return changed;
    }
    case gyCONSTRAINT_MIDALIGNED_LEFT:
    {
      bool changed = false;

      for (wxObjectList::const_iterator it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);

        double x3 = (double)(x - (minWidth/2.0));
        if (!Equals(x3, constrainedObject->GetX()))
        {
          changed = true;
          constrainedObject->Move(dc, x3, constrainedObject->GetY(), false);
        }
      }
      return changed;
    }
    case gyCONSTRAINT_MIDALIGNED_RIGHT:
    {
      bool changed = false;

      for (wxObjectList::const_iterator it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);

        double x3 = (double)(x + (minWidth/2.0));
        if (!Equals(x3, constrainedObject->GetX()))
        {
          changed = true;
          constrainedObject->Move(dc, x3, constrainedObject->GetY(), false);
        }
      }
      return changed;
      #if 0
      // two returned values ?
      return false;
      #endif
    }
    case gyCONSTRAINT_MIDALIGNED_TOP:
    {
      bool changed = false;

      for (wxObjectList::const_iterator it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);

        double y3 = (double)(y - (minHeight/2.0));
        if (!Equals(y3, constrainedObject->GetY()))
        {
          changed = true;
          constrainedObject->Move(dc, constrainedObject->GetX(), y3, false);
        }
      }
      return changed;
    }
    case gyCONSTRAINT_MIDALIGNED_BOTTOM:
    {
      bool changed = false;

      for (wxObjectList::const_iterator it = m_constrainedObjects.begin();
           it != m_constrainedObjects.end();
           it++)
      {
        wxShape* constrainedObject = wxStaticCast(*it, wxShape);

        double y3 = (double)(y + (minHeight/2.0));
        if (!Equals(y3, constrainedObject->GetY()))
        {
          changed = true;
          constrainedObject->Move(dc, constrainedObject->GetX(), y3, false);
        }
      }
      return changed;
    }
    #if 0
    // default value handled in main function body
    default:
      return false;
    #endif
  }
  return false;
}

