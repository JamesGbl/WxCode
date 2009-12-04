/////////////////////////////////////////////////////////////////////////////
// Name:        samples/ogl/ogledit/palette.cpp
// Purpose:     OGLEdit palette
// Author:      Julian Smart
// Modified by:
// Created:     12/07/98
// RCS-ID:      $Id: palette.cpp,v 1.1 2007/03/28 15:15:49 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "doc.h"
#include "view.h"
#include "app.h"
#include "palette.h"

// Include pixmaps
#include "bitmaps/arrow.xpm"
#include "bitmaps/tool1.xpm"
#include "bitmaps/tool2.xpm"
#include "bitmaps/tool3.xpm"
#include "bitmaps/tool4.xpm"
#include "bitmaps/texttool.xpm"

/*
 * Object editor tool palette
 *
 */

EditorToolPalette::EditorToolPalette(wxWindow* parent, const wxPoint& pos, const wxSize& size,
            long style):
  TOOLPALETTECLASS(parent, wxID_ANY, pos, size, style)
{
    currentlySelected = wxNOT_FOUND;
}

bool EditorToolPalette::OnLeftClick(int toolIndex, bool toggled)
{
  // BEGIN mutual exclusivity code
  if (toggled && (currentlySelected != wxNOT_FOUND) && (toolIndex != currentlySelected))
    ToggleTool(currentlySelected, false);

  if (toggled)
    currentlySelected = toolIndex;
  else if (currentlySelected == toolIndex)
    currentlySelected = wxNOT_FOUND;
  //  END mutual exclusivity code

  return true;
}

void EditorToolPalette::OnMouseEnter(int WXUNUSED(toolIndex))
{
}

void EditorToolPalette::SetSize(int x, int y, int width, int height, int sizeFlags)
{
  TOOLPALETTECLASS::SetSize(x, y, width, height, sizeFlags);
}

EditorToolPalette *MyApp::CreatePalette(wxFrame *parent)
{
  // Load palette bitmaps.
  wxBitmap PaletteTool1(tool1_xpm);
  wxBitmap PaletteTool2(tool2_xpm);
  wxBitmap PaletteTool3(tool3_xpm);
  wxBitmap PaletteTool4(tool4_xpm);
  wxBitmap PaletteTool5(texttool_xpm);
  wxBitmap PaletteArrow(arrow_xpm);

  EditorToolPalette *palette = new EditorToolPalette(parent, wxPoint(0, 0), wxDefaultSize,
      wxTB_VERTICAL);

  palette->SetMargins(2, 2);
  palette->SetToolBitmapSize(wxSize(22, 22));

  palette->AddCheckTool(PALETTE_ARROW, wxEmptyString, PaletteArrow, wxNullBitmap, _("Pointer"));
  palette->AddCheckTool(PALETTE_TOOL1, wxEmptyString, PaletteTool1, wxNullBitmap, _("Tool 1"));
  palette->AddCheckTool(PALETTE_TOOL2, wxEmptyString, PaletteTool2, wxNullBitmap, _("Tool 2"));
  palette->AddCheckTool(PALETTE_TOOL3, wxEmptyString, PaletteTool3, wxNullBitmap, _("Tool 3"));
  palette->AddCheckTool(PALETTE_TOOL4, wxEmptyString, PaletteTool4, wxNullBitmap, _("Tool 4"));
  palette->AddCheckTool(PALETTE_TOOL5, wxEmptyString, PaletteTool5, wxNullBitmap, _("Tool 5"));

  palette->Realize();

  palette->ToggleTool(PALETTE_ARROW, true);
  palette->currentlySelected = PALETTE_ARROW;
  return palette;
}
