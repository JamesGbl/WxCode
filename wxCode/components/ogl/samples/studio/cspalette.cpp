/////////////////////////////////////////////////////////////////////////////
// Name:        contrib/samples/ogl/studio/cspalette.cpp
// Purpose:     OGLEdit palette
// Author:      Julian Smart
// Modified by:
// Created:     12/07/98
// RCS-ID:      $Id: cspalette.cpp,v 1.1 2007/03/28 15:15:49 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "doc.h"
#include "view.h"
#include "app.h"
#include "cspalette.h"
#include "symbols.h"

#include "bitmaps/arrow.xpm"
#include "bitmaps/texttool.xpm"

/*
 * Object editor tool palette
 *
 */

csEditorToolPalette::csEditorToolPalette(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
            long style):
  wxToolBar(parent, id, pos, size, style)
{
  m_currentlySelected = wxNOT_FOUND;

  SetMaxRowsCols(1, 1000);
}

bool csEditorToolPalette::OnLeftClick(int toolIndex, bool toggled)
{
  // BEGIN mutual exclusivity code
  if (toggled && (m_currentlySelected != wxNOT_FOUND) && (toolIndex != m_currentlySelected))
    ToggleTool(m_currentlySelected, false);

  if (toggled)
    m_currentlySelected = toolIndex;
  else if (m_currentlySelected == toolIndex)
    m_currentlySelected = wxNOT_FOUND;
  //  END mutual exclusivity code

  return true;
}

void csEditorToolPalette::OnMouseEnter(int toolIndex)
{
#if wxUSE_STATUSBAR
    wxString msg = wxEmptyString;
    if (toolIndex == PALETTE_ARROW)
        msg = _("Pointer");
    else if (toolIndex != wxNOT_FOUND)
    {
        csSymbol* symbol = wxGetApp().GetSymbolDatabase()->FindSymbol(toolIndex);
        if (symbol)
            msg = symbol->GetName();
    }
    wxStaticCast(wxGetApp().GetTopWindow(), wxFrame)->SetStatusText(msg);
#else
    wxUnusedVar(toolIndex);
#endif // wxUSE_STATUSBAR
}

void csEditorToolPalette::SetSize(wxCoord x, wxCoord y, wxCoord width, wxCoord height, wxCoord sizeFlags)
{
  base::SetSize(x, y, width, height, sizeFlags);
}

void csEditorToolPalette::SetSelection(int sel)
{
    if ((sel != m_currentlySelected) && (m_currentlySelected != wxNOT_FOUND))
    {
        ToggleTool(m_currentlySelected, false);
    }
    m_currentlySelected = sel;
    ToggleTool(m_currentlySelected, true);
}

bool csApp::CreatePalette(wxFrame *parent)
{
    // First create a layout window
    wxSashLayoutWindow* win = new wxSashLayoutWindow(parent, ID_LAYOUT_WINDOW_PALETTE, wxDefaultPosition, wxSize(200, 30), wxNO_BORDER|wxSW_3D|wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(10000, 40));
    win->SetOrientation(wxLAYOUT_HORIZONTAL);
    win->SetAlignment(wxLAYOUT_TOP);
    win->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
    win->SetSashVisible(wxSASH_BOTTOM, true);

    m_diagramPaletteSashWindow = win;

    m_diagramPaletteSashWindow->Show(false);

  // Load palette bitmaps
  wxBitmap PaletteArrow(arrow_xpm);
  wxBitmap TextTool(texttool_xpm);
  wxSize toolBitmapSize(22, 22);

  csEditorToolPalette *palette = new csEditorToolPalette(m_diagramPaletteSashWindow, ID_DIAGRAM_PALETTE, wxPoint(0, 0), wxDefaultSize, wxTB_HORIZONTAL|wxNO_BORDER);

  palette->SetMargins(2, 2);

  palette->SetToolBitmapSize(toolBitmapSize);

  palette->AddCheckTool(PALETTE_ARROW, wxEmptyString, PaletteArrow, wxNullBitmap, _("Pointer"));
  palette->AddCheckTool(PALETTE_TEXT_TOOL, wxEmptyString, TextTool, wxNullBitmap, _("Text"));

  wxString* symbols = new wxString[20];
  int noSymbols = 0;

  symbols[noSymbols] = _("Wide Rectangle");
  noSymbols ++;

  symbols[noSymbols] =  _("Thin Rectangle");
  noSymbols ++;

  symbols[noSymbols] =  _("Triangle");
  noSymbols ++;

  symbols[noSymbols] =  _("Octagon");
  noSymbols ++;

  // For some reason, we're getting Gdk errors with
  // some shapes, such as ones that use DrawEllipse.
#ifndef __WXGTK__
  symbols[noSymbols] =  _("Group");
  noSymbols ++;

  symbols[noSymbols] =  _("Circle");
  noSymbols ++;

  symbols[noSymbols] =  _("Circle shadow");
  noSymbols ++;

  symbols[noSymbols] =  _("SemiCircle");
  noSymbols ++;
#endif

  int i;
  for (i = 0; i < noSymbols; i++)
  {
      csSymbol* symbol = GetSymbolDatabase()->FindSymbol(symbols[i]);
      if (symbol)
      {
           wxBitmap* bitmap = GetSymbolDatabase()->CreateToolBitmap(symbol, toolBitmapSize);
           palette->AddCheckTool(symbol->GetToolId(), wxEmptyString, *bitmap, wxNullBitmap, symbol->GetName());

           delete bitmap;
      }
  }
  delete[] symbols;

#if 0
  wxNode* node = GetSymbolDatabase()->GetSymbols().First();
  while (node)
  {
    csSymbol* symbol = (csSymbol*) node->Data();

    wxBitmap* bitmap = GetSymbolDatabase()->CreateToolBitmap(symbol, toolBitmapSize);
    palette->AddTool(symbol->GetToolId(), *bitmap, wxNullBitmap, true, 0, wxDefaultCoord, NULL, symbol->GetName());

    delete bitmap;

    node = node->Next();
  }
#endif

  palette->Realize();

  palette->SetSelection(PALETTE_ARROW);
  m_diagramPalette = palette;

  return true;
}
