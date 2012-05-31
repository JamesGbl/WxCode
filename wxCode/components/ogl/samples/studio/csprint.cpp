/////////////////////////////////////////////////////////////////////////////
// Name:        contrib/samples/ogl/studio/csprint.cpp
// Purpose:     Printing and clipboard functionality
// Author:      Julian Smart
// Modified by:
// Created:     12/07/98
// RCS-ID:      $Id: csprint.cpp,v 1.1 2007/03/28 15:15:50 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include <wx/ogl/ogl.h> // base header of OGL, includes and adjusts wx/deprecated/setup.h

#include <wx/clipbrd.h>

#ifdef __WXMSW__
#include <wx/metafile.h>
#endif

#include "app.h"
#include "doc.h"
#include "shapes.h"
#include "view.h"

IMPLEMENT_DYNAMIC_CLASS(wxDiagramClipboard, wxDiagram)

// Copy selection
bool wxDiagramClipboard::Copy(wxDiagram* diagram)
{
    DeleteAllShapes();

    return DoCopy(diagram, this, false, NULL);
}

// Copy contents to the diagram, with new ids.

bool wxDiagramClipboard::Paste(wxDiagram* diagram, wxDC* dc, int offsetX, int offsetY)
{
    return DoCopy(this, diagram, true, dc, offsetX, offsetY);
}

// Universal copy function (to or from clipboard).
// TODO:
// Note that this only works for non-composites so far (nested shapes
// don't have their old-to-new object mappings stored).
// Also, lines don't yet get their attachment points moved to the new offset position
// if they have more than 2 points.
bool wxDiagramClipboard::DoCopy(wxDiagram* diagramFrom, wxDiagram* diagramTo, bool newIds,
    wxDC* dc, int offsetX, int offsetY)
{
    OnStartCopy(diagramTo);

    wxHashTable mapping(wxKEY_INTEGER);

    // First copy all node shapes.
    wxList* shapeList = diagramFrom->GetShapeList();
    wxObjectList::compatibility_iterator node;
    for (node = shapeList->GetFirst();
         node;
         node = node->GetNext())
    {
        wxShape* shape = wxStaticCast(node->GetData(), wxShape);
        if (((diagramFrom == this) || shape->Selected()) && !wxDynamicCast(shape, wxLineShape))
        {
            wxShape* newShape = shape->CreateNewCopy();
            newShape->GetLines().Clear();
            if (newIds)
            {
                newShape->AssignNewIds();
            }
            mapping.Put((long) shape, (wxObject*) newShape);

            newShape->SetX(newShape->GetX() + offsetX);
            newShape->SetY(newShape->GetY() + offsetY);

            OnAddShape(diagramTo, newShape, dc);

        }
    }

    for (node = shapeList->GetFirst();
         node;
         node = node->GetNext())
    {
        wxShape* shape = wxStaticCast(node->GetData(), wxShape);
        if (((diagramFrom == this) || shape->Selected()) && wxDynamicCast(shape, wxLineShape))
        {
            wxLineShape* lineShape = wxStaticCast(shape, wxLineShape);
            // Only copy a line if its ends are selected too.
            if ((diagramFrom == this) || (lineShape->GetTo()->Selected() && lineShape->GetFrom()->Selected()))
            {
                wxLineShape* newShape = wxStaticCast(shape->CreateNewCopy(), wxLineShape);
                mapping.Put((long) shape, (wxObject*) newShape);

                if (newIds)
                    newShape->AssignNewIds();

                wxShape* fromShape = wxStaticCast(mapping.Get((long) lineShape->GetFrom()), wxShape);
                wxShape* toShape = wxStaticCast(mapping.Get((long) lineShape->GetTo()), wxShape);

                wxASSERT_MSG( fromShape, wxT("Could not find 'from' shape"));
                wxASSERT_MSG( toShape, wxT("Could not find 'to' shape"));

                fromShape->AddLine(newShape, toShape, newShape->GetAttachmentFrom(),
                  newShape->GetAttachmentTo());

                OnAddShape(diagramTo, newShape, dc);

            }
        }
    }

    // Now make sure line ordering is correct
    for (node = shapeList->GetFirst();
         node;
         node = node->GetNext())
    {
        wxShape* shape = wxStaticCast(node->GetData(), wxShape);
        if (((diagramFrom == this) || shape->Selected()) && !wxDynamicCast(shape, wxLineShape))
        {
            wxShape* newShape = wxStaticCast(mapping.Get((long) shape), wxShape);

            // Make a list of all the new lines, in the same order as the old lines.
            // Then apply the list of new lines to the shape.
            wxList newLines;
            for (wxObjectList::compatibility_iterator lineNode = shape->GetLines().GetFirst();
                 lineNode;
                 lineNode = lineNode->GetNext())
            {
                wxLineShape* lineShape = wxStaticCast(lineNode->GetData(), wxLineShape);
                if ((diagramFrom == this) || (lineShape->GetTo()->Selected() && lineShape->GetFrom()->Selected()))
                {
                    wxLineShape* newLineShape = wxStaticCast(mapping.Get((long) lineShape), wxLineShape);

                    wxASSERT_MSG(newLineShape, wxT("Could not find new line shape"));

                    newLines.Append(newLineShape);
                }
            }

            if (newLines.GetCount() > 0)
                newShape->ApplyAttachmentOrdering(newLines);
        }
    }

    OnEndCopy(diagramTo);

    return true;
}

#ifdef __WXMSW__
// Draw contents to a Windows metafile device context and a bitmap, and copy
// these to the Windows clipboard
bool wxDiagramClipboard::CopyToClipboard(double scale)
{
#if wxUSE_METAFILE
  // Make a metafile DC
  wxMetaFileDC mfDC;
  if (mfDC.IsOk())
  {
    mfDC.SetUserScale(scale, scale);

    // Draw on metafile DC
    Redraw(mfDC);

    // int printWidth = mfDC.MaxX() - mfDC.MinX();
    // int printHeight = mfDC.MaxY() - mfDC.MinY();
    int maxX = (int)mfDC.MaxX();
    int maxY = (int)mfDC.MaxY();
    wxMetaFile *mf = mfDC.Close();

    // Set to a bitmap memory DC
    wxBitmap *newBitmap = new wxBitmap((int)(maxX + 10), (int)(maxY + 10));
    if (!newBitmap->IsOk())
    {
      delete newBitmap;

      wxChar buf[200];
      wxSprintf(buf, _("Sorry, could not allocate clipboard bitmap (%dx%d)"), (maxX+10), (maxY+10));
      wxMessageBox(buf, _("Clipboard copy problem"));
      return false;
    }

    wxMemoryDC memDC;
    memDC.SelectObject(*newBitmap);
    memDC.Clear();

    // Now draw on memory bitmap DC
    Redraw(memDC);

    memDC.SelectObject(wxNullBitmap);

    // Open clipboard and set the data
    if (wxOpenClipboard())
    {
        wxEmptyClipboard();

        // Copy the bitmap to the clipboard
        wxSetClipboardData(wxDF_BITMAP, newBitmap, 0, 0);

#if 0 // TODO: replace this code (wxEnhMetaFile doesn't have SetClipboard)
        if (mf)
        {
            // Copy the metafile to the clipboard
            // Allow a small margin
            bool success = mf->SetClipboard((int)(mfDC.MaxX() + 15), (int)(mfDC.MaxY() + 15));
        }
#endif

        // Close clipboard
        wxCloseClipboard();
    }

    delete newBitmap;
    delete mf;

  }
  return true;
#else
  wxMessageBox("wxUSE_METAFILE in build required to use Clipboard", wxT("Clipboard copy problem"));
  return false;
#endif
}
#endif
    // __WXMSW__

// Override this to e.g. have the shape added through a Do/Undo command system.
// By default, we'll just add it directly to the destination diagram.
bool wxDiagramClipboard::OnAddShape(wxDiagram* diagramTo, wxShape* newShape, wxDC* dc)
{
    diagramTo->AddShape(newShape);

    if (dc && (diagramTo != this))
    {
        newShape->Select(true, dc);
    }

    return true;
}

/*
 * csDiagramClipboard
 */

IMPLEMENT_DYNAMIC_CLASS(csDiagramClipboard, wxDiagramClipboard)

// Start/end copying
bool csDiagramClipboard::OnStartCopy(wxDiagram* diagramTo)
{
    // Do nothing if copying to the clipboard
    if (diagramTo == this)
        return true;

    // Deselect all objects initially.

    csDiagram* diagram = wxStaticCast(diagramTo, csDiagram);
    csDiagramDocument* doc = diagram->GetDocument();
    wxStaticCast(doc->GetFirstView(), csDiagramView)->SelectAll(false);

    m_currentCmd = new csDiagramCommand(_("Paste"), doc);

    return true;
}

bool csDiagramClipboard::OnEndCopy(wxDiagram* diagramTo)
{
    // Do nothing if copying to the clipboard
    if (diagramTo == this)
        return true;

    csDiagram* diagram = wxStaticCast(diagramTo, csDiagram);
    csDiagramDocument* doc = diagram->GetDocument();

    if (m_currentCmd)
    {
        if (m_currentCmd->GetStates().GetCount() == 0)
        {
            delete m_currentCmd;
        }
        else
        {
            doc->GetCommandProcessor()->Submit(m_currentCmd);
            m_currentCmd = NULL;
        }
    }
    return true;
}

// Use the command framework to add the shapes, if we're copying to a diagram and
// not the clipboard.
bool csDiagramClipboard::OnAddShape(wxDiagram* diagramTo, wxShape* newShape, wxDC* WXUNUSED(dc))
{
    if (diagramTo == this)
    {
        diagramTo->AddShape(newShape);
    }
    else
    {
        csDiagram* diagram = wxStaticCast(diagramTo, csDiagram);
        /* csDiagramDocument* doc = */ diagram->GetDocument();

        if (wxDynamicCast(newShape, wxLineShape))
            m_currentCmd->AddState(new csCommandState(ID_CS_ADD_LINE_SELECT, newShape, NULL));
        else
            m_currentCmd->AddState(new csCommandState(ID_CS_ADD_SHAPE_SELECT, newShape, NULL));
    }

    return true;
}
