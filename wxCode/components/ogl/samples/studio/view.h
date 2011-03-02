/////////////////////////////////////////////////////////////////////////////
// Name:        contrib/samples/ogl/studio/view.h
// Purpose:     View-related classes
// Author:      Julian Smart
// Modified by:
// Created:     12/07/98
// RCS-ID:      $Id: view.h,v 1.1 2007/03/28 15:15:51 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _STUDIO_VIEW_H_
#define _STUDIO_VIEW_H_

#include "doc.h"
#include <wx/ogl/ogl.h>

class WXDLLIMPEXP_FWD_CORE wxDocMDIChildFrame;

class csDiagramView;
class csCanvas : public wxShapeCanvas
{
   typedef wxShapeCanvas base;
DECLARE_CLASS(csCanvas)
 public:

  csCanvas(csDiagramView *view, wxWindowID id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxRETAINED);
  virtual ~csCanvas(void);

  void DrawOutline(wxDC&, double x1, double y1, double x2, double y2);

  void OnMouseEvent(wxMouseEvent&);
  void OnPaint(wxPaintEvent&);

  virtual void OnLeftClick(double x, double y, int keys = 0);
  virtual void OnRightClick(double x, double y, int keys = 0);

  virtual void OnDragLeft(bool draw, double x, double y, int keys=0); // Erase if draw false
  virtual void OnBeginDragLeft(double x, double y, int keys=0);
  virtual void OnEndDragLeft(double x, double y, int keys=0);

  virtual void OnDragRight(bool draw, double x, double y, int keys=0); // Erase if draw false
  virtual void OnBeginDragRight(double x, double y, int keys=0);
  virtual void OnEndDragRight(double x, double y, int keys=0);

  inline csDiagramView*  GetView() const { return m_view; }
  inline void SetView(csDiagramView* view) { m_view = view; }

 protected:
  csDiagramView*    m_view;

DECLARE_EVENT_TABLE()
};

class csDiagramView : public wxView
{
   typedef wxView base;
   DECLARE_DYNAMIC_CLASS(csDiagramView)
public:
   csDiagramView(void) : wxView() { m_canvas = NULL; }
   virtual ~csDiagramView(void);

   virtual bool OnCreate(wxDocument*, long flags);
   virtual void OnDraw(wxDC*);
   virtual void OnUpdate(wxView *sender, wxObject *hint = NULL);
   virtual bool OnClose(bool deleteWindow = true);
   
   csDiagramDocument* GetDocument() const;
   wxFrame* GetFrame() const { return wxStaticCast(base::GetFrame(), wxFrame); }

   wxShape *FindFirstSelectedShape(void);

   // Scans the canvas for selections (doesn't use m_selections)
   void FindSelectedShapes(wxList& selections, wxClassInfo* toFind = NULL);

   // The selections in the order in which they were selected
   inline const wxList& GetSelectionList() const { return m_selections; }

  // Adds or removes shape from m_selections
  void SelectShape(wxShape*, bool select);

  // Apply point size to current shapes
  void ApplyPointSize(int pointSize);

  // Make the point size combobox reflect this
  void ReflectPointSize(int pointSize);

  // Make the arrow toggle button reflect the state of the line
  void ReflectArrowState(wxLineShape*);

  // Do a cut operation for the given list of shapes
  void DoCut(wxList& shapes);

  // Do a general command
  void DoCmd(wxList& shapes, wxList& oldShapes, int cmd, const wxString& op);

  // Select or deselect all
  void SelectAll(bool select = true);

// Event handlers
protected:
  void OnSelectAll(wxCommandEvent&);
  void OnCut(wxCommandEvent&);
  void OnCopy(wxCommandEvent&);
  void OnPaste(wxCommandEvent&);
  void OnDuplicate(wxCommandEvent&);
  void OnClear(wxCommandEvent&);
  void OnChangeBackgroundColour(wxCommandEvent&);
  void OnEditProperties(wxCommandEvent&);
  void OnPointSizeComboSel(wxCommandEvent&);
  void OnPointSizeComboText(wxCommandEvent&);
  void OnToggleArrowTool(wxCommandEvent&);
  void OnZoomSel(wxCommandEvent&);
  void OnAlign(wxCommandEvent&);
  void OnNewLinePoint(wxCommandEvent&);
  void OnCutLinePoint(wxCommandEvent&);
  void OnStraightenLines(wxCommandEvent&);

// UI update handles
  void OnToggleArrowToolUpdate(wxUpdateUIEvent&);
  void OnEditPropertiesUpdate(wxUpdateUIEvent&);
  void OnNeedSelection(wxUpdateUIEvent&);
  void OnPasteUpdate(wxUpdateUIEvent&);
  void OnAlignUpdate(wxUpdateUIEvent&);
  void OnNewLinePointUpdate(wxUpdateUIEvent&);
  void OnCutLinePointUpdate(wxUpdateUIEvent&);
  void OnStraightenLinesUpdate(wxUpdateUIEvent&);
  void OnUndoUpdate(wxUpdateUIEvent&);
  void OnRedoUpdate(wxUpdateUIEvent&);
   DECLARE_EVENT_TABLE()

public:
  csCanvas*     m_canvas;
  wxList        m_selections;
};

#endif
    // _STUDIO_VIEW_H_
