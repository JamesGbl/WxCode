/////////////////////////////////////////////////////////////////////////////
// Name:        ogldiag.h
// Purpose:     OGL - wxDiagram class
// Author:      Julian Smart
// Modified by:
// Created:     12/07/98
// RCS-ID:      $Id: ogldiag.h,v 1.1 2007/03/28 15:15:47 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _OGL_OGLDIAG_H_
#define _OGL_OGLDIAG_H_

#include <wx/ogl/basic.h>

class WXDLLIMPEXP_OGL wxDiagram: public wxObject
{
 DECLARE_DYNAMIC_CLASS(wxDiagram)

public:

  wxDiagram();
  virtual ~wxDiagram();

  void SetCanvas(wxShapeCanvas *can);

  inline wxShapeCanvas *GetCanvas() const { return m_diagramCanvas; }

  virtual void Redraw(wxDC&);
  virtual void Clear(wxDC&);
  virtual void DrawOutline(wxDC&, double x1, double y1, double x2, double y2);

  // Add object to end of object list (if addAfter is NULL)
  // or just after addAfter.
  virtual void AddShape(wxShape *object, wxShape *addAfter = NULL);

  // Add object to front of object list
  virtual void InsertShape(wxShape *object);

  void SetSnapToGrid(bool snap);
  void SetGridSpacing(double spacing);
  inline double GetGridSpacing() const { return m_gridSpacing; }
  inline bool GetSnapToGrid() const { return m_snapToGrid; }
  void Snap(double *x, double *y);

  inline void SetQuickEditMode(bool qem) { m_quickEditMode = qem; }
  inline bool GetQuickEditMode() const { return m_quickEditMode; }

  virtual void RemoveShape(wxShape *object);
  virtual void RemoveAllShapes();
  virtual void DeleteAllShapes();
  virtual void ShowAll(bool show);

  // Find a shape by its id
  wxShape* FindShape(long id) const;

  inline void SetMouseTolerance(int tol) { m_mouseTolerance = tol; }
  inline int GetMouseTolerance() const { return m_mouseTolerance; }
  inline wxList *GetShapeList() const { return m_shapeList; }
  inline int GetCount() const { return m_shapeList->GetCount(); }

  // Make sure all text that should be centred, is centred.
  void RecentreAll(wxDC&);

  virtual bool SaveFile(const wxString& filename);
  virtual bool LoadFile(const wxString& filename);

  virtual void ReadNodes(wxXmlNode*);
  virtual void ReadLines(wxXmlNode*);
  virtual void ReadContainerGeometry(wxXmlNode*);

  virtual bool OnShapeLoad(wxXmlNode*, wxShape*);
  virtual bool OnShapeSave(wxXmlNode*, const wxShape&);

protected:
  wxShapeCanvas*        m_diagramCanvas;
  bool                  m_quickEditMode;
  bool                  m_snapToGrid;
  double                m_gridSpacing;
  int                   m_mouseTolerance;
  wxList*               m_shapeList;
};

class WXDLLIMPEXP_OGL wxLineCrossing: public wxObject
{
public:
    wxLineCrossing() { m_lineShape1 = NULL; m_lineShape2 = NULL; }
    wxRealPoint     m_pt1; // First line
    wxRealPoint     m_pt2;
    wxRealPoint     m_pt3; // Second line
    wxRealPoint     m_pt4;
    wxRealPoint     m_intersect;
    wxLineShape*    m_lineShape1;
    wxLineShape*    m_lineShape2;
};

class WXDLLIMPEXP_OGL wxLineCrossings : public wxObject
{
public:
    wxLineCrossings();
    virtual ~wxLineCrossings();

    void FindCrossings(wxDiagram& diagram);
    void DrawCrossings(wxDiagram& diagram, wxDC&);
    void ClearCrossings();

public:
    wxList  m_crossings;
};

#endif
 // _OGL_OGLDIAG_H_
