/////////////////////////////////////////////////////////////////////////////
// Name:        wxAdvTable.cpp
// Purpose:     wxAdvTable classes implementation
// Author:      Moskvichev Andrey V.
// Created:     29/10/2008
// RCS-ID:      $Id: wxAdvTable.cpp,v 1.4 2008/10/29 16:42:58 frm Exp $
// Copyright:   (c) 2008-2010 Moskvichev A.V.
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxadvtable.h"

#include <wx/renderer.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/colordlg.h>

#include "checkMark.xpm"
#include "rowSortAscend.xpm"
#include "rowSortDescend.xpm"
#include "colSortAscend.xpm"
#include "colSortDescend.xpm"

#include <wx/arrimpl.cpp>
WX_DEFINE_EXPORTED_OBJARRAY(wxAdvHdrCellArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxAdvHdrCellPtrArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxAdvRangeArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxCoordArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxIndexArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxArrayArrayString);
WX_DEFINE_EXPORTED_OBJARRAY(wxIntArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxIntIntArray);

DEFINE_EVENT_TYPE(wxEVT_ADVTABLE_HDRCELL_LEFT_CLICK)
DEFINE_EVENT_TYPE(wxEVT_ADVTABLE_HDRCELL_RIGHT_CLICK)
DEFINE_EVENT_TYPE(wxEVT_ADVTABLE_HDRCELL_LEFT_DCLICK)
DEFINE_EVENT_TYPE(wxEVT_ADVTABLE_HDRCELL_RIGHT_DCLICK)
DEFINE_EVENT_TYPE(wxEVT_ADVTABLE_HDRCELL_MOVE)
DEFINE_EVENT_TYPE(wxEVT_ADVTABLE_HDRCELL_SIZE)
DEFINE_EVENT_TYPE(wxEVT_ADVTABLE_HDRCELL_SORT)

static bool g_bitmapsLoaded = false;
static wxBitmap g_bmpCheckMark;
static wxBitmap g_bmpRowSortAscend;
static wxBitmap g_bmpRowSortDescend;
static wxBitmap g_bmpColSortAscend;
static wxBitmap g_bmpColSortDescend;

void LoadBitmaps()
{
	if (g_bitmapsLoaded) {
		return ;
	}

	g_bmpCheckMark = wxBitmap(checkMark_xpm);
	g_bmpRowSortAscend = wxBitmap(rowSortAscend_xpm);
	g_bmpRowSortDescend = wxBitmap(rowSortDescend_xpm);
	g_bmpColSortAscend = wxBitmap(colSortAscend_xpm);
	g_bmpColSortDescend = wxBitmap(colSortDescend_xpm);

	g_bitmapsLoaded = true;
}

const int wxAdvHdrCell::defaultSpacing         = 5;
const int wxAdvHdrCell::defaultAlignVertical   = wxALIGN_CENTER_VERTICAL;
const int wxAdvHdrCell::defaultAlignHorizontal = wxALIGN_CENTER_HORIZONTAL;

static const wxChar *modelDateFormat = wxT("%X %x");

size_t wxAdvHdrCell::GetDecompCellCount(wxAdvHdrCell *cells, size_t numCells)
{
	size_t count = 0;

	for (size_t n = 0; n < numCells; n++) {
		count += cells[n].GetDecompCellCount();
	}
	return count;
}

wxAdvHdrCell *wxAdvHdrCell::RightSibling()
{
	if (m_parent == NULL) {
		if (m_isRow) {
			return m_table->GetRow(m_subIndex + 1);
		}
		else {
			return m_table->GetCol(m_subIndex + 1);
		}
	}
	else {
		if (m_subIndex + 1 >= m_parent->m_subCells.Count()) {
			return NULL;
		}

		return &m_parent->m_subCells[m_subIndex + 1];
	}
	return NULL;
}

IMPLEMENT_DYNAMIC_CLASS(wxAdvHdrCellEvent, wxNotifyEvent)

wxAdvHdrCellEvent::wxAdvHdrCellEvent(int id, wxEventType type, wxObject* obj,
		wxAdvHdrCell *hdrCell, int x, int y,
		bool control, bool shift, bool alt, bool meta)
: wxNotifyEvent(type, id)
{
	m_hdrCell = hdrCell;
	m_x = x;
	m_y = y;
	m_control = control;
	m_shift = shift;
	m_alt = alt;
	m_meta = meta;

	SetEventObject(obj);
}

//
// Drawing helper functions.
//

void ResizeBitmap(wxBitmap &bmp, wxCoord newWidth, wxCoord newHeight)
{
	if (bmp.GetWidth() != newWidth || bmp.GetHeight() != newHeight) {
		bmp.Create(newWidth, newHeight);
	}
}

/**
 * Make colour darker.
 * @param colour input colour
 * @return modified colour
 */
wxColour Darker(wxColour colour)
{
	const double mult = 0.8;

	double red = colour.Red() * mult;
	double green = colour.Green() * mult;
	double blue = colour.Blue() * mult;

	return wxColour((unsigned char) red,
			(unsigned char) green,
			(unsigned char) blue);

}

/**
 * Make colour brighter.
 * @param colour input colour
 * @return modified colour
 */
wxColour Brighter(wxColour colour)
{
	const double mult = 1.2;

	double red = colour.Red() * mult;
	double green = colour.Green() * mult;
	double blue = colour.Blue() * mult;

	if (red > 255) {
		red = 255;
	}
	if (green > 255) {
		green = 255;
	}
	if (blue > 255) {
		blue = 255;
	}

	return wxColour((unsigned char) red,
			(unsigned char) green,
			(unsigned char) blue);

}

/**
 * Draws bevel.
 * @param dc device context
 * @param rc rectangle where to draw
 * @param bgColour bevel background colour
 * @param colour1 colour of left and top edges
 * @param colour2 colour of right and bottom edges
 * @param edgeWidth edgeWidth
 * @param gradient true to draw gradient bevel
 */
void DrawBevel(wxDC &dc, const wxRect &rc, wxColour bgColour, wxColour colour1, wxColour colour2, int edgeWidth, bool gradient)
{
	// draw background
	dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(bgColour));
	dc.SetPen(*wxThePenList->FindOrCreatePen(bgColour, 1, wxSOLID));
	if (gradient) {
		dc.GradientFillLinear(rc, bgColour, Brighter(bgColour), wxNORTH);
	}
	else {
		dc.DrawRectangle(rc);
	}

	// draw light edge
	dc.SetPen(*wxThePenList->FindOrCreatePen(colour1, edgeWidth, wxSOLID));
	dc.DrawLine(rc.x, rc.y,
			rc.x, rc.y + rc.height);
	dc.DrawLine(rc.x, rc.y,
			rc.x + rc.width, rc.y);

	// draw shadow
	dc.SetPen(*wxThePenList->FindOrCreatePen(colour2, edgeWidth, wxSOLID));
	dc.DrawLine(rc.x + rc.width, rc.y,
			rc.x + rc.width, rc.y + rc.height);
	dc.DrawLine(rc.x, rc.y + rc.height,
			rc.x + rc.width, rc.y + rc.height);
}

void AlignAdjust(const wxRect &rc, wxCoord *x, wxCoord *y, wxCoord widthText, wxCoord heightText, int align, bool vertical)
{
	wxCoord wT, hT;

	if (vertical) {
		wT = heightText;
		hT = widthText;
	}
	else {
		wT = widthText;
		hT = heightText;
	}

	wxCoord dx, dy;

	if (align & wxALIGN_RIGHT) {
		dx = rc.width - wT;
	}
	else if (align & wxALIGN_CENTRE_HORIZONTAL) {
		dx = (rc.width - wT) / 2;
	}
	else { // wxALIGN_LEFT
		dx = 0;
	}

	if (align & wxALIGN_BOTTOM) {
		dy = rc.height - hT;
	}
	else if (align & wxALIGN_CENTRE_VERTICAL) {
		dy = (rc.height - hT) / 2;
	}
	else { // wxALIGN_TOP
		dy = 0;
	}

	*x = rc.x + dx;
	*y = rc.y + dy;
}

/**
 * Draws multiline text, using current font and text colour.
 * @param dc device context
 * @param text text to draw
 * @param rc rectangle where to draw text
 * @param align horizontal and vertical alignment
 * @param vertical true if you want draw draw vertical text
 */
void DrawMultilineText(wxDC &dc, const wxString &text, const wxRect &rc, int align, bool vertical)
{
	wxCoord textWidth, textHeight, lineHeight;
	dc.GetMultiLineTextExtent(text, &textWidth, &textHeight, &lineHeight);

	wxCoord x, y;

	AlignAdjust(rc, &x, &y, textWidth, textHeight, align, vertical);

	wxString curLine;
	for (const wxChar *pc = text; ; pc++) {
		if (*pc == wxT('\n') || *pc == wxT('\0')) { // end of line
			if (vertical) {
				wxCoord lineWidth;
				dc.GetTextExtent(curLine, &lineWidth, NULL);

				wxCoord yLine = y;

				if (align & wxALIGN_BOTTOM) {
					yLine = y + textWidth;
				}
				else if (align & wxALIGN_CENTRE_VERTICAL) {
					yLine = y + lineWidth / 2 + textWidth / 2;
				}
				else {// wxALIGN_TOP
					yLine = y + lineWidth;
				}

				if (!curLine.empty()) {
					dc.DrawRotatedText(curLine, x, yLine, 90);
				}

				x += lineHeight;
			}
			else {
				wxCoord xLine = x;

				if (align & (wxALIGN_RIGHT | wxALIGN_CENTRE_HORIZONTAL)) {
					wxCoord lineWidth;
					dc.GetTextExtent(curLine, &lineWidth, NULL);

					if (align & wxALIGN_RIGHT) {
						xLine = x + textWidth - lineWidth;
					}
					else if (align & wxALIGN_CENTRE_HORIZONTAL) {
						xLine = x + (textWidth - lineWidth) / 2;
					}
				}
				else { // wxALIGN_LEFT
					xLine = x;
				}

				if (!curLine.empty()) {
					dc.DrawText(curLine, xLine, y);
				}

				y += lineHeight;
			}

			if (*pc == wxT('\0')) {
				break;
			}

			curLine.clear();
		}
		else {
			curLine += *pc;
		}
	}
}

//
// wxAdvTableDataModelObserver
//
wxAdvTableDataModelObserver::wxAdvTableDataModelObserver()
{
}

wxAdvTableDataModelObserver::~wxAdvTableDataModelObserver()
{
}


//
// Table data models implementation.
//

//
// wxAdvTableDataModel
//
wxAdvTableDataModel::wxAdvTableDataModel()
{
}

wxAdvTableDataModel::~wxAdvTableDataModel()
{
}

double wxAdvTableDataModel::GetCellValueAsDouble(size_t WXUNUSED(row), size_t WXUNUSED(col))
{
	return 0;
}

void wxAdvTableDataModel::SetCellValueAsDateTime(size_t row, size_t col, wxDateTime &dt)
{
	SetCellValue(row, col, dt.Format(modelDateFormat));
}

wxDateTime wxAdvTableDataModel::GetCellValueAsDateTime(size_t WXUNUSED(row), size_t WXUNUSED(col))
{
	return wxDateTime();
}

void wxAdvTableDataModel::StringToChoices(const wxString &value, wxString &selection, wxArrayString &choices)
{
	wxChar prevCh = wxT('0');

	size_t wordStart = 0;
	int nWord = 0;

	for (size_t n = 0; n < value.Length(); n++) {
		wxChar ch = value[n];

		if (ch == wxT(';') && prevCh != wxT('\\')) {
			wxString word = value.SubString(wordStart, n - 1);
			word.Replace(wxT("\\;"), wxT(";"));

			if (nWord == 0) {
				selection = word;
			}
			else {
				choices.Add(word);
			}

			nWord++;
			wordStart = n + 1;
		}

		prevCh = ch;
	}

	// parse string tail
	wxString word = value.SubString(wordStart, value.Length() - 1);
	word.Replace(wxT("\\;"), wxT(";"));

	if (nWord == 0) {
		selection = word;
	}
	else {
		choices.Add(word);
	}
}

wxString wxAdvTableDataModel::ChoicesToString(const wxString &selection, wxArrayString &choices)
{
	wxString value;

	value += selection;
	value.Replace(wxT(";"), wxT("\\;"));

	for (size_t n = 0; n < choices.Count(); n++) {
		wxString choice = choices[n];

		choice.Replace(wxT(";"), wxT("\\;"));

		value += wxT(";");
		value += choice;
	}
	return value;
}

wxString wxAdvTableDataModel::ChoicesSelection(const wxString &value)
{
	wxChar prevCh = wxT('\0');

	for (size_t n = 0; n < value.Length(); n++) {
		wxChar ch = value[n];

		if (ch == wxT(';') && prevCh != wxT('\\')) {
			wxString word = value.SubString(0, n - 1);
			word.Replace(wxT("\\;"), wxT(";"));

			return word;
		}

		prevCh = ch;
	}
	return value;
}

//
// wxAdvDefaultTableDataModel
//
wxAdvDefaultTableDataModel::wxAdvDefaultTableDataModel(size_t numRows, size_t numCols, bool readOnly)
{
	m_readOnly = readOnly;

	wxArrayString row;
	row.Add(wxEmptyString, numCols);
	m_data.Add(row, numRows);

	wxIntArray rowFormat;
	rowFormat.Add(wxStringFormat, numCols);
	m_formats.Add(rowFormat, numRows);
}

wxAdvDefaultTableDataModel::~wxAdvDefaultTableDataModel()
{
}

wxString wxAdvDefaultTableDataModel::GetCellValue(size_t row, size_t col)
{
	wxCHECK(row < m_data.Count(), wxEmptyString);
	wxCHECK(col < m_data[row].Count(), wxEmptyString);
	return m_data[row][col];
}

bool wxAdvDefaultTableDataModel::IsCellEditable(size_t WXUNUSED(row), size_t WXUNUSED(col))
{
	return !m_readOnly;
}

int wxAdvDefaultTableDataModel::GetCellFormat(size_t row, size_t col)
{
	wxCHECK(row < m_formats.Count(), wxStringFormat);
	wxCHECK(col < m_formats[row].Count(), wxStringFormat);
	return  m_formats[row][col];
}

void wxAdvDefaultTableDataModel::SetCellFormat(size_t row, size_t col, int format)
{
	wxCHECK(row < (size_t) m_formats.Count(), );
	wxCHECK(col < (size_t) m_formats[row].Count(), );
	m_formats[row][col] = format;
}

bool wxAdvDefaultTableDataModel::SetCellValue(size_t row, size_t col, wxString value)
{
	if (m_readOnly) {
		return false;
	}

	wxCHECK(row < (size_t) m_data.Count(), false);
	wxCHECK(col < (size_t) m_data[row].Count(), false);

	m_data[row][col] = value;
	FireCellChanged(row, col);
	return true;
}

wxAdvCellAttribute *wxAdvDefaultTableDataModel::GetCellAttribute(size_t WXUNUSED(row), size_t WXUNUSED(col))
{
	return NULL;
}

double wxAdvDefaultTableDataModel::GetCellValueAsDouble(size_t WXUNUSED(row), size_t WXUNUSED(col))
{
	return 0;
}

wxDateTime wxAdvDefaultTableDataModel::GetCellValueAsDateTime(size_t row, size_t col)
{
	wxString value = GetCellValue(row, col);

	wxDateTime dt;
	dt.ParseDateTime(value);
	return dt;
}

void wxAdvDefaultTableDataModel::SetRowFormat(size_t row, int format)
{
	wxCHECK(row < (size_t) m_formats.Count(), );

	const size_t colCount = m_formats[0].Count();
	for (size_t col = 0; col < colCount; col++) {
		m_formats[row][col] = format;
	}
	FireTableChanged();
}

void wxAdvDefaultTableDataModel::SetColFormat(size_t col, int format)
{
	wxCHECK(col < (size_t) m_formats[0].Count(), );

	const size_t rowCount = m_formats.Count();
	for (size_t row = 0; row < rowCount; row++) {
		m_formats[row][col] = format;
	}
	FireTableChanged();
}

void wxAdvDefaultTableDataModel::InsertRows(size_t before, size_t count, int format)
{
	const size_t colCount = m_data[0].Count();

	wxArrayString row;
	row.Add(wxEmptyString, colCount);
	m_data.Insert(row, before, count);

	wxIntArray rowFormat;
	rowFormat.Add(format, colCount);
	m_formats.Insert(rowFormat, before, count);

	FireTableChanged();
}

void wxAdvDefaultTableDataModel::InsertCols(size_t before, size_t count, int format)
{
	const size_t rowCount = m_data.Count();

	for (size_t row = 0; row < rowCount; row++) {
		m_data[row].Insert(wxEmptyString, before, count);
		m_formats[row].Insert(format, before, count);
	}

	FireTableChanged();
}

void wxAdvDefaultTableDataModel::RemoveRows(size_t first, size_t count)
{
	m_data.RemoveAt(first, count);
	m_formats.RemoveAt(first, count);

	FireTableChanged();
}

void wxAdvDefaultTableDataModel::RemoveCols(size_t first, size_t count)
{
	const size_t rowCount = m_data.Count();
	for (size_t row = 0; row < rowCount; row++) {
		m_data[row].RemoveAt(first, count);
		m_formats[row].RemoveAt(first, count);
	}

	FireTableChanged();
}

//
// wxAdvFilterTableDataModel
//
wxAdvFilterTableDataModel::wxAdvFilterTableDataModel(wxAdvTableDataModel *model, size_t numRows, size_t numCols)
{
	m_underlayingModel = model;
	m_underlayingModel->AddObserver(this);

	wxArrayString row;
	row.Add(wxEmptyString, numCols);
	m_data.Add(row, numRows);

	m_needUpdate = true;
}

wxAdvFilterTableDataModel::~wxAdvFilterTableDataModel()
{
	m_underlayingModel->RemoveObserver(this);
	delete m_underlayingModel;
}

wxString wxAdvFilterTableDataModel::GetCellValue(size_t row, size_t col)
{
	if (m_needUpdate) {
		UpdateValues();
	}

	wxCHECK(row < m_data.Count(), wxEmptyString);
	wxCHECK(col < m_data[row].Count(), wxEmptyString);
	return m_data[row][col];
}

int wxAdvFilterTableDataModel::GetCellFormat(size_t row, size_t col)
{
	return m_underlayingModel->GetCellFormat(row, col);
}

bool wxAdvFilterTableDataModel::IsCellEditable(size_t row, size_t col)
{
	return m_underlayingModel->IsCellEditable(row, col);
}

void wxAdvFilterTableDataModel::CellChanged(size_t row, size_t col)
{
	m_data[row][col] = m_underlayingModel->GetCellValue(row, col);
	FireCellChanged(row, col);
}

bool wxAdvFilterTableDataModel::SetCellValue(size_t row, size_t col, wxString value)
{
	return m_underlayingModel->SetCellValue(row, col, value);
}

wxAdvCellAttribute *wxAdvFilterTableDataModel::GetCellAttribute(size_t row, size_t col)
{
	return m_underlayingModel->GetCellAttribute(row, col);
}

double wxAdvFilterTableDataModel::GetCellValueAsDouble(size_t row, size_t col)
{
	if (GetCellFormat(row, col) != wxDoubleFormat) {
		return 0;
	}

	wxString value = GetCellValue(row, col);
	double dvalue;
	if (value.ToDouble(&dvalue)) {
		return dvalue;
	}
	return 0;
}

wxDateTime wxAdvFilterTableDataModel::GetCellValueAsDateTime(size_t row, size_t col)
{
	wxString value = GetCellValue(row, col);

	wxDateTime dt;
	dt.ParseDateTime(value);
	return dt;
}

void wxAdvFilterTableDataModel::TableChanged()
{
	UpdateValues();
	FireTableChanged();
}

void wxAdvFilterTableDataModel::UpdateValues()
{
	for (size_t row = 0; row < m_data.Count(); row++) {
		for (size_t col = 0; col < m_data[row].Count(); col++) {
			m_data[row][col] = m_underlayingModel->GetCellValue(row, col);
		}
	}
	m_needUpdate = false;
}

wxAdvTableDataModel *wxAdvFilterTableDataModel::GetUnderlayingModel()
{
	return m_underlayingModel;
}

//
// Renderers
//


//
// wxAdvHdrCellRenderer
//
wxAdvHdrCellRenderer::wxAdvHdrCellRenderer()
{
}

wxAdvHdrCellRenderer::~wxAdvHdrCellRenderer()
{
}

//
// wxAdvDefaultHdrCellRenderer
//
wxAdvDefaultHdrCellRenderer::wxAdvDefaultHdrCellRenderer()
{
	m_edgeWidth = 3;
	m_drawGradient = true;
}

wxAdvDefaultHdrCellRenderer::~wxAdvDefaultHdrCellRenderer()
{
}

void wxAdvDefaultHdrCellRenderer::Draw(wxAdvTable *WXUNUSED(table), wxDC &dc, wxAdvHdrCell *hdrCell, bool selected, bool pressed, int sortDirection)
{
	DrawBackground(dc, hdrCell, selected, pressed);

	wxColour textColour = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT);

	dc.SetFont(*wxNORMAL_FONT);
	dc.SetTextForeground(textColour);

	wxRect rc = hdrCell->m_rc;
	rc.Deflate(m_edgeWidth, m_edgeWidth);

	if (sortDirection != wxAdvTable::SortDirNoSorting) {
		// draw sort arrow
		wxBitmap *bmpArrow = NULL; // : warning C4701: local variable 'bmpArrow' may be used without having been initialized
		LoadBitmaps();

		if (hdrCell->IsRow()) {
			if (sortDirection == wxAdvTable::SortDirAscending) {
				bmpArrow = &g_bmpRowSortAscend;
			}
			else if (sortDirection == wxAdvTable::SortDirDescending) {
				bmpArrow = &g_bmpRowSortDescend;
			}
		}
		else {
			if (sortDirection == wxAdvTable::SortDirAscending) {
				bmpArrow = &g_bmpColSortAscend;
			}
			else if (sortDirection == wxAdvTable::SortDirDescending) {
				bmpArrow = &g_bmpColSortDescend;
			}
		}

		wxCoord xArrow  = rc.x + rc.width - bmpArrow->GetWidth() - hdrCell->m_spacing;
		wxCoord yCenter = rc.y + rc.height / 2;

		dc.DrawBitmap(*bmpArrow, xArrow, yCenter - bmpArrow->GetHeight() / 2, true);

		//rc.width = xArrow - rc.x - hdrCell->m_spacing;
	}

	DrawMultilineText(dc, hdrCell->m_label, rc,
			hdrCell->m_alignHorizontal | hdrCell->m_alignVertical,
			hdrCell->IsVerticalText());
}

void wxAdvDefaultHdrCellRenderer::DrawBackground(wxDC &dc, wxAdvHdrCell *hdrCell, bool selected, bool pressed)
{
	wxColour colour1, colour2, bgColour;
	bgColour = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);

	if (pressed) {
		colour1 = wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW);
		colour2 = wxSystemSettings::GetColour(wxSYS_COLOUR_3DHIGHLIGHT);
	}
	else {
		colour1 = wxSystemSettings::GetColour(wxSYS_COLOUR_3DHIGHLIGHT);
		colour2 = wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW);
	}

	if (selected) {
		bgColour = Darker(bgColour);
		colour1 = Darker(colour1);
		colour2 = Darker(colour2);
	}

	DrawBevel(dc, hdrCell->m_rc, bgColour, colour1, colour2, m_edgeWidth, m_drawGradient);
}

wxAdvHdrCellRenderer *wxAdvDefaultHdrCellRenderer::Clone()
{
	return new wxAdvDefaultHdrCellRenderer();
}

//
// wxAdvCellRenderer
//
wxAdvCellRenderer::wxAdvCellRenderer()
{
}

wxAdvCellRenderer::~wxAdvCellRenderer()
{
}

//
// wxAdvStringCellRenderer
//
wxAdvStringCellRenderer::wxAdvStringCellRenderer()
{
}

wxAdvStringCellRenderer::~wxAdvStringCellRenderer()
{
}

void wxAdvStringCellRenderer::Draw(wxAdvTable *WXUNUSED(table), wxDC &dc, wxRect rc, wxString value, bool WXUNUSED(selected), bool WXUNUSED(focused), wxAdvCellAttribute *attr)
{
	wxDCClipper clip(dc, rc);

	if (attr->BgBrush().GetStyle() != wxTRANSPARENT) {
		// blend brush colors.
		wxColour clr1 = dc.GetBrush().GetColour();
		wxColour clr2 = attr->BgBrush().GetColour();

		wxColour blendedClr((clr1.Red() + clr2.Red()) / 2,
			(clr1.Green() + clr2.Green()) / 2,
			(clr1.Blue() + clr2.Blue()) / 2);

		wxBrush brush = *wxTheBrushList->FindOrCreateBrush(blendedClr, attr->BgBrush().GetStyle());
		dc.SetBrush(brush);
	}

	dc.DrawRectangle(rc);
	dc.SetFont(attr->Font());
	dc.SetTextForeground(attr->TextColour());
	dc.DrawLabel(value, rc, attr->Alignment());
}

//
// wxAdvBoolCellRenderer
//
wxAdvBoolCellRenderer::wxAdvBoolCellRenderer()
{
}

wxAdvBoolCellRenderer::~wxAdvBoolCellRenderer()
{
}

void wxAdvBoolCellRenderer::Draw(wxAdvTable *WXUNUSED(table), wxDC &dc, wxRect rc, wxString value, bool WXUNUSED(selected), bool WXUNUSED(focused), wxAdvCellAttribute *WXUNUSED(attr))
{
	bool checked = false;
	if (value.Cmp(boolTrue) == 0) {
		checked = true;
	}

	if (checked) {
		LoadBitmaps();

		wxDCClipper clipper(dc, rc);

		dc.DrawBitmap(g_bmpCheckMark,
				rc.x + (rc.width - g_bmpCheckMark.GetWidth()) / 2,
				rc.y + (rc.height - g_bmpCheckMark.GetHeight()) / 2,
				true);
	}
}

//
// wxAdvColourCellRenderer
//
wxAdvColourCellRenderer::wxAdvColourCellRenderer()
{
}

wxAdvColourCellRenderer::~wxAdvColourCellRenderer()
{
}

void wxAdvColourCellRenderer::Draw(wxAdvTable *WXUNUSED(table), wxDC &dc, wxRect rc, wxString value, bool WXUNUSED(selected), bool WXUNUSED(focused), wxAdvCellAttribute *WXUNUSED(attr))
{
	wxColour colour(value);

	dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(colour));
	dc.DrawRectangle(rc);
}

//
// wxAdvDateTimeCellRenderer
//

wxAdvDateTimeCellRenderer::wxAdvDateTimeCellRenderer(wxChar *format)
{
	m_format = format;
}

wxAdvDateTimeCellRenderer::~wxAdvDateTimeCellRenderer()
{
}

void wxAdvDateTimeCellRenderer::Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool selected, bool focused, wxAdvCellAttribute *attr)
{
	// XXX overhead
	wxDateTime dt;
	dt.ParseFormat(value.c_str(), modelDateFormat);

	value = dt.Format(m_format.c_str());

	wxAdvStringCellRenderer::Draw(table, dc, rc, value, selected, focused, attr);
}

void wxAdvDateTimeCellRenderer::SetFormat(const wxString &format)
{
	m_format = format;
}

//
// wxAdvChoicesCellRenderer
//
wxAdvChoicesCellRenderer::wxAdvChoicesCellRenderer()
{
}

wxAdvChoicesCellRenderer::~wxAdvChoicesCellRenderer()
{
}

void wxAdvChoicesCellRenderer::Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool selected, bool focused, wxAdvCellAttribute *attr)
{
	wxAdvStringCellRenderer::Draw(table, dc, rc, wxAdvTableDataModel::ChoicesSelection(value), selected, focused, attr);
}

//
// Editors
//

wxAdvCellEditor::wxAdvCellEditor(wxAdvTable *table)
{
	m_table = table;
	m_active = false;
}

wxAdvCellEditor::~wxAdvCellEditor()
{
}

bool wxAdvCellEditor::OneClickActivate()
{
	return false;
}

void wxAdvCellEditor::HandleKeyEvent(wxKeyEvent &WXUNUSED(ev))
{
	// do nothing by default - method is for override.
}

void wxAdvCellEditor::Activate(size_t row, size_t col)
{
	m_cell.Set(row, col);
	m_active = true;

	DoActivate();
}

void wxAdvCellEditor::Deactivate()
{
	if (m_active) {
		m_active = false;

		DoDeactivate();

		m_cell.Unset();
	}
}

bool wxAdvCellEditor::IsActive()
{
	return m_active;
}

wxAdvTable *wxAdvCellEditor::GetTable()
{
	return m_table;
}

size_t wxAdvCellEditor::GetRow()
{
	wxCHECK_MSG(m_cell.IsSet(), UNDEF_SIZE, wxT("wxAdvCellEditor::GetValue: m_cell is not set"));

	return m_cell.Row();
}

size_t wxAdvCellEditor::GetCol()
{
	wxCHECK_MSG(m_cell.IsSet(), UNDEF_SIZE, wxT("wxAdvCellEditor::GetValue: m_cell is not set"));

	return m_cell.Col();
}

wxRect wxAdvCellEditor::GetCellRect()
{
	wxCHECK_MSG(m_cell.IsSet(), wxRect(0, 0, 0, 0), wxT("wxAdvCellEditor::GetValue: m_cell is not set"));

	return m_table->GetCellRect(m_cell.Row(), m_cell.Col());
}

wxString wxAdvCellEditor::GetValue()
{
	wxCHECK_MSG(m_cell.IsSet(), wxEmptyString, wxT("wxAdvCellEditor::GetValue: m_cell is not set"));

	return m_table->GetCellValue(m_cell.Row(), m_cell.Col());
}

void wxAdvCellEditor::SetNewValue(wxString newValue)
{
	wxCHECK_RET(m_cell.IsSet(), wxT("wxAdvCellEditor::SetNewValue: m_cell is not set"));
	wxCHECK_RET(m_table != NULL, wxT("wxAdvCellEditor::SetNewValue: m_table != NULL"));

	size_t row, col;
	m_table->ToModelCellCoord(m_cell.Row(), m_cell.Col(), row, col);
	m_table->ChangeCellValue(row, col, newValue);
}

void wxAdvCellEditor::EndEditing()
{
	if (m_active) {
		wxCHECK_RET(m_table != NULL, wxT("wxAdvCellEditor::EndEditing: m_table != NULL"));
		m_table->StopEditing();
		m_active = false;
	}
}

//
// wxAdvStringCellEditor
//
BEGIN_EVENT_TABLE(wxAdvStringCellEditor, wxEvtHandler)
	EVT_TEXT_ENTER(wxID_ANY, wxAdvStringCellEditor::OnTextEnter)
	EVT_KILL_FOCUS(wxAdvStringCellEditor::OnKillFocus)
	EVT_KEY_DOWN(wxAdvStringCellEditor::OnTextKeydown)
END_EVENT_TABLE()

wxAdvStringCellEditor::wxAdvStringCellEditor(wxAdvTable *table)
 : wxAdvCellEditor(table)
{
	m_textCtrl = new wxTextCtrl(table, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
			wxTE_PROCESS_ENTER | wxBORDER_NONE);
	m_textCtrl->Show(false);
	m_textCtrl->SetNextHandler(this);
}

wxAdvStringCellEditor::~wxAdvStringCellEditor()
{
	m_textCtrl->Destroy();
}

bool wxAdvStringCellEditor::OneClickActivate()
{
	return false;
}

void wxAdvStringCellEditor::HandleKeyEvent(wxKeyEvent &ev)
{
	int keyCode = ev.GetKeyCode();

	if (wxIsalnum(keyCode)) {
		m_textCtrl->EmulateKeyPress(ev);
	}
}

void wxAdvStringCellEditor::DoActivate()
{
	wxRect rc = GetCellRect();
	rc.Deflate(1, 1);
	GetTable()->CalcScrolledPosition(rc.x, rc.y, &rc.x, &rc.y);

	m_textCtrl->ChangeValue(GetValue());
	m_textCtrl->SetInsertionPointEnd();

	m_textCtrl->SetSize(rc.x, rc.y, rc.width, rc.height);
	m_textCtrl->Enable();
	m_textCtrl->Show();

	m_textCtrl->SetFocus();

	m_escPressed = false;
}

void wxAdvStringCellEditor::DoDeactivate()
{
	m_textCtrl->Disable();
	m_textCtrl->Show(false);

	if (!m_escPressed) {
		SetNewValue(m_textCtrl->GetValue());
	}
}

void wxAdvStringCellEditor::OnTextEnter(wxCommandEvent &WXUNUSED(ev))
{
	EndEditing();
}

void wxAdvStringCellEditor::OnKillFocus(wxFocusEvent &WXUNUSED(ev))
{
	EndEditing();
}

void wxAdvStringCellEditor::OnTextKeydown(wxKeyEvent &ev)
{
	if (ev.GetKeyCode() == WXK_ESCAPE) {
		m_escPressed = true;
		EndEditing();
		return ;
	}
	ev.Skip();
}

//
// wxAdvBoolCellEditor
//

wxAdvBoolCellEditor::wxAdvBoolCellEditor(wxAdvTable *table)
: wxAdvCellEditor(table)
{
}

wxAdvBoolCellEditor::~wxAdvBoolCellEditor()
{
}

bool wxAdvBoolCellEditor::OneClickActivate()
{
	return true;
}

void wxAdvBoolCellEditor::DoActivate()
{
	bool checked = ! (GetValue().Cmp(boolTrue) == 0); // invert value on activation
	SetNewValue((checked) ? boolTrue : boolFalse);

	EndEditing();
}

void wxAdvBoolCellEditor::DoDeactivate()
{
}

//
// wxAdvColourCellEditor
//
wxAdvColourCellEditor::wxAdvColourCellEditor(wxAdvTable *table)
: wxAdvCellEditor(table)
{
}

wxAdvColourCellEditor::~wxAdvColourCellEditor()
{
}

bool wxAdvColourCellEditor::OneClickActivate()
{
	return true;
}

void wxAdvColourCellEditor::DoActivate()
{
	wxString strColor = GetValue();

	wxColourData clrData;
	clrData.SetColour(wxColour(strColor));

	wxColourDialog dlg(GetTable(), &clrData);
	if (dlg.ShowModal() == wxID_OK) {
		wxString newColourValue = dlg.GetColourData().GetColour().GetAsString();

		SetNewValue(newColourValue);
		EndEditing();
	}
}

void wxAdvColourCellEditor::DoDeactivate()
{
}

//
// wxAdvIntervalCellEditor
//
wxAdvIntervalCellEditor::wxAdvIntervalCellEditor(wxAdvTable *table)
: wxAdvCellEditor(table)
{
	m_spinCtrl = new wxSpinCtrl(table, wxID_ANY);
	m_spinCtrl->Show(false);
}

wxAdvIntervalCellEditor::~wxAdvIntervalCellEditor()
{
	m_spinCtrl->Close();
}

bool wxAdvIntervalCellEditor::OneClickActivate()
{
	return false;
}

void wxAdvIntervalCellEditor::SetRange(int minValue, int maxValue)
{
	m_spinCtrl->SetRange(minValue, maxValue);
}

void wxAdvIntervalCellEditor::DoActivate()
{
	wxRect rc = GetCellRect();
	GetTable()->CalcScrolledPosition(rc.x, rc.y, &rc.x, &rc.y);

	wxString value = GetValue();

	m_spinCtrl->SetValue(value);

	m_spinCtrl->SetSize(rc.x, rc.y, rc.width, rc.height);
	m_spinCtrl->Show();
}

void wxAdvIntervalCellEditor::DoDeactivate()
{
	m_spinCtrl->Show(false);

	SetNewValue(wxString::Format(wxT("%i"), m_spinCtrl->GetValue()));
}

#if wxUSE_DATEPICKCTRL
//
// wxAdvDateTimeCellEditor
//
BEGIN_EVENT_TABLE(wxAdvDateTimeCellEditor, wxEvtHandler)
	EVT_DATE_CHANGED(wxID_ANY, wxAdvDateTimeCellEditor::OnDateChanged)
END_EVENT_TABLE()

wxAdvDateTimeCellEditor::wxAdvDateTimeCellEditor(wxAdvTable *table, bool dropDown)
: wxAdvCellEditor(table)
{
	m_datePicker = new wxDatePickerCtrl(table, wxID_ANY, wxDefaultDateTime,
			wxDefaultPosition, wxDefaultSize, wxDP_SHOWCENTURY | (dropDown ? wxDP_DROPDOWN : wxDP_SPIN));
	m_datePicker->Show(false);
}

wxAdvDateTimeCellEditor::~wxAdvDateTimeCellEditor()
{
	m_datePicker->Close();
}

bool wxAdvDateTimeCellEditor::OneClickActivate()
{
	return false;
}

void wxAdvDateTimeCellEditor::DoActivate()
{
	wxRect rc = GetCellRect();
	GetTable()->CalcScrolledPosition(rc.x, rc.y, &rc.x, &rc.y);

	wxString value = GetValue();

	wxDateTime dtValue;
	wxCHECK_RET(dtValue.ParseFormat(value.c_str(), modelDateFormat) != NULL, wxT("Error parsing datetime value"));

	m_datePicker->SetValue(dtValue);

	m_datePicker->SetSize(rc.x, rc.y, rc.width, rc.height);
	m_datePicker->Show();
	m_datePicker->SetFocus();
}

void wxAdvDateTimeCellEditor::DoDeactivate()
{
	m_datePicker->Show(false);

	wxString value = m_datePicker->GetValue().Format(modelDateFormat);
	SetNewValue(value);
}

void wxAdvDateTimeCellEditor::OnDateChanged(wxDateEvent &WXUNUSED(ev))
{
	wxString value = m_datePicker->GetValue().Format(modelDateFormat);
	SetNewValue(value);
}

#endif /* wxUSE_DATEPICKCTRL */

#define ID_COMBO 5000

//
// wxAdvChoicesCellEditor
//
BEGIN_EVENT_TABLE(wxAdvChoicesCellEditor, wxEvtHandler)
	EVT_COMBOBOX(ID_COMBO, wxAdvChoicesCellEditor::OnCombobox)
	EVT_TEXT(wxID_ANY, wxAdvChoicesCellEditor::OnText)
END_EVENT_TABLE()

wxAdvChoicesCellEditor::wxAdvChoicesCellEditor(wxAdvTable *table, bool editable)
: wxAdvCellEditor(table)
{
	m_comboBox = new wxComboBox(table, ID_COMBO, wxEmptyString,
			wxDefaultPosition, wxDefaultSize, 0, NULL, wxTE_PROCESS_ENTER | (editable ? 0 : wxCB_READONLY));
	m_comboBox->Show(false);
}

wxAdvChoicesCellEditor::~wxAdvChoicesCellEditor()
{
	m_comboBox->Close();
}

bool wxAdvChoicesCellEditor::OneClickActivate()
{
	return false;
}

void wxAdvChoicesCellEditor::DoActivate()
{
	wxRect rc = GetCellRect();
	GetTable()->CalcScrolledPosition(rc.x, rc.y, &rc.x, &rc.y);

	wxString value = GetValue();
	wxArrayString choices;
	wxString selection;

	wxAdvTableDataModel::StringToChoices(value, selection, choices);
	m_comboBox->Clear();
	m_comboBox->Append(choices);
	m_comboBox->SetStringSelection(selection);

	m_comboBox->SetSize(rc.x, rc.y, rc.width, rc.height);
	m_comboBox->Show();
	m_comboBox->SetFocus();
}

void wxAdvChoicesCellEditor::DoDeactivate()
{
	m_comboBox->Show(false);

	UpdateValue();
}

void wxAdvChoicesCellEditor::UpdateValue()
{
	wxArrayString choices = m_comboBox->GetStrings();

	SetNewValue(wxAdvTableDataModel::ChoicesToString(
			m_comboBox->GetStringSelection(),
			choices));
	EndEditing();
}

void wxAdvChoicesCellEditor::OnCombobox(wxCommandEvent &WXUNUSED(ev))
{
	UpdateValue();
	//EndEditing();
}

void wxAdvChoicesCellEditor::OnText(wxCommandEvent &WXUNUSED(ev))
{
	UpdateValue();
}

//
// wxAdvCellAttribute
//

wxAdvCellAttribute::wxAdvCellAttribute()
{
	// default values
	m_font = *wxNORMAL_FONT;
	m_alignment = wxALIGN_CENTER | wxALIGN_RIGHT;
	m_bgBrush = *wxTheBrushList->FindOrCreateBrush(*wxBLACK, wxTRANSPARENT);
}

wxAdvCellAttribute::~wxAdvCellAttribute()
{
}

//
// Sorters
//

/**
 * Internal class. Don't use it in applications.
 */
class SortHelper // used to implement sorters
{
public:
	SortHelper(const SortHelper &o)
	{
		m_sorter = o.m_sorter;
		m_table = o.m_table;
		m_row = o.m_row;
		m_col = o.m_col;
		m_sortRows = o.m_sortRows;
		m_sortDirection = o.m_sortDirection;
	}

	SortHelper(wxAdvTable *table, wxAdvTableSorter *sorter, size_t row, size_t col, bool sortRows, int sortDirection)
	{
		m_table = table;
		m_sorter = sorter;
		m_row = row;
		m_col = col;
		m_sortRows = sortRows;
		m_sortDirection = sortDirection;
	}

	virtual ~SortHelper()
	{
	}

	size_t Index()
	{
		return (m_sortRows) ? m_row : m_col;
	}

	wxAdvTableSorter *m_sorter;
	wxAdvTable *m_table;
	size_t m_row;
	size_t m_col;
	int m_sortDirection;

	bool m_sortRows;
};

WX_DEFINE_SORTED_ARRAY(SortHelper *, wxSortHelperArray);

int SorterFunc(SortHelper *first, SortHelper *second)
{
	wxAdvTableSorter *sorter = first->m_sorter;
	int res = sorter->Compare(first->m_table, first->m_row, first->m_col, second->m_row, second->m_col);
	if (first->m_sortDirection == wxAdvTable::SortDirDescending) {
		return -1 * res;
	}
	return res;
}


//
// Sorters.
//

wxAdvTableSorter::wxAdvTableSorter()
{
}

wxAdvTableSorter::~wxAdvTableSorter()
{
}

//
// wxAdvTableStringSorter
//
wxAdvTableStringSorter::wxAdvTableStringSorter()
{
}

wxAdvTableStringSorter::~wxAdvTableStringSorter()
{
}

int wxAdvTableStringSorter::Compare(wxAdvTable *table, size_t row1, size_t col1, size_t row2, size_t col2)
{
	wxAdvTableDataModel *dataModel = table->GetModel();

	wxString value1 = dataModel->GetCellValue(row1, col1);
	wxString value2 = dataModel->GetCellValue(row2, col2);

	return Compare(value1, value2);
}

int wxAdvTableStringSorter::Compare(wxString value1, wxString value2)
{
	return value1.Cmp(value2);
}

//
// wxAdvTableDoubleSorter
//
wxAdvTableDoubleSorter::wxAdvTableDoubleSorter()
{
}

wxAdvTableDoubleSorter::~wxAdvTableDoubleSorter()
{
}

int wxAdvTableDoubleSorter::Compare(wxAdvTable *table, size_t row1, size_t col1, size_t row2, size_t col2)
{
	wxAdvTableDataModel *dataModel = table->GetModel();

	// if values are not in double format, fallback to string compare.
	if ((dataModel->GetCellFormat(row1, col1) != wxDoubleFormat)
			|| (dataModel->GetCellFormat(row2, col2) != wxDoubleFormat)) {
		return wxAdvTableStringSorter::Compare(table, row1, col1, row2, col2);
	}

	double value1 = dataModel->GetCellValueAsDouble(row1, col1);
	double value2 = dataModel->GetCellValueAsDouble(row2, col2);

	return Compare(value1, value2);
}

int wxAdvTableDoubleSorter::Compare(double value1, double value2)
{
	if (value1 == value2) {
		return 0;
	}
	else if (value1 < value2) {
		return -1;
	}
	else {
		return 1;
	}
}

//
// wxAdvTableDoubleSorter
//
wxAdvTableDateTimeSorter::wxAdvTableDateTimeSorter()
{
}

wxAdvTableDateTimeSorter::~wxAdvTableDateTimeSorter()
{
}

int wxAdvTableDateTimeSorter::Compare(wxAdvTable *table, size_t row1, size_t col1, size_t row2, size_t col2)
{
	wxAdvTableDataModel *dataModel = table->GetModel();

	// if values are not in datetime format, fallback to string compare.
	if ((dataModel->GetCellFormat(row1, col1) != wxDateTimeFormat)
			|| (dataModel->GetCellFormat(row2, col2) != wxDateTimeFormat)) {
		return wxAdvTableStringSorter::Compare(table, row1, col1, row2, col2);
	}

	wxDateTime value1 = dataModel->GetCellValueAsDateTime(row1, col1);
	wxDateTime value2 = dataModel->GetCellValueAsDateTime(row2, col2);

	return Compare(value1, value2);
}

int wxAdvTableDateTimeSorter::Compare(const wxDateTime &value1, const wxDateTime &value2)
{
	if (value1.IsEarlierThan(value2)) {
		return -1;
	}
	else if (value1.IsLaterThan(value2)) {
		return 1;
	}
	else {
		return 0;
	}
}

//
// wxAdvTable
//

BEGIN_EVENT_TABLE(wxAdvTable, wxScrolledWindow)
	EVT_PAINT(wxAdvTable::OnPaint)
	EVT_MOUSE_EVENTS(wxAdvTable::OnMouseEvents)
	EVT_KEY_DOWN(wxAdvTable::OnKeyDown)
	//EVT_CHAR(wxAdvTable::OnChar)
	EVT_KILL_FOCUS(wxAdvTable::OnKillFocus)
	EVT_SIZE(wxAdvTable::OnSize)
	EVT_SCROLLWIN(wxAdvTable::OnScrollWin)
END_EVENT_TABLE()

IMPLEMENT_CLASS(wxAdvTable, wxScrolledWindow)

wxAdvTable::wxAdvTable(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
: wxScrolledWindow(parent, id, pos, size, wxHSCROLL | wxVSCROLL | wxFULL_REPAINT_ON_RESIZE | wxWANTS_CHARS)
{
	SetInitialSize(size);

	m_tableCreated = false;
	m_model = NULL;
	m_defaultRenderer = new wxAdvStringCellRenderer();
	m_defaultHdrRenderer = new wxAdvDefaultHdrCellRenderer();

	m_sorter = NULL;
	m_sortMode = SortDisabled;
	m_highlightMode = HighlightNone;
	m_pressedHdrCell = NULL;
	m_currentHdrCell = NULL;
	m_resizingHdrCell = NULL;
	m_sortDirection = SortDirAscending;

	m_activeEditor = NULL;

	m_selectMode = SelectBlock;

	m_showRows = true;
	m_showCols = true;
	m_showCorner = true;

	m_resizeAllRowsAllowed = false;
	m_resizeAllColsAllowed = false;

	m_sortByAnyRow = false;
	m_sortByAnyCol = false;

	// Graphic objects initialization
	m_bgBrush = *wxWHITE_BRUSH;
	m_selectedBgBrush = *wxTheBrushList->FindOrCreateBrush(wxColour(220, 220, 220));
	m_gridPen = *wxThePenList->FindOrCreatePen(wxColour(200, 200, 200), 1, wxSOLID);
	m_focusedPen = *wxThePenList->FindOrCreatePen(wxColour(80, 80, 250), 1, wxSOLID);
	m_focusedBgBrush = *wxTheBrushList->FindOrCreateBrush(wxColour(180, 180, 250));
	m_highlightedBgBrush = *wxTheBrushList->FindOrCreateBrush(wxColour(225, 225, 250));

	ClearSelection();

	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	SetScrollRate(10, 10);

	// set default editors/renderers for formats
	SetEditorForFormat(wxStringFormat, new wxAdvStringCellEditor(this));
	SetEditorForFormat(wxColourFormat, new wxAdvColourCellEditor(this));
	SetEditorForFormat(wxBoolFormat, new wxAdvBoolCellEditor(this));

	SetRendererForFormat(wxColourFormat, new wxAdvColourCellRenderer());
	SetRendererForFormat(wxBoolFormat, new wxAdvBoolCellRenderer());
}

wxAdvTable::~wxAdvTable()
{
	wxAdvCellRendererMap::iterator it;
	for (it = m_renderers.begin(); it != m_renderers.end(); it++) {
		delete it->second;
	}

	wxAdvCellEditorMap::iterator it2;
	for (it2 = m_editors.begin(); it2 != m_editors.end(); it2++) {
		delete it2->second;
	}

	wxDELETE(m_model);
	wxDELETE(m_defaultRenderer);

	wxDELETE(m_defaultHdrRenderer);
}

//
// Table creation functions.
//

void wxAdvTable::Create(wxAdvHdrCell *rows, size_t numRows, wxAdvHdrCell *cols, size_t numCols, const wxString &cornerLabel, wxAdvTableDataModel *model)
{
	if (m_tableCreated) {
		// destroy old data and structures if any
		DestroyTable();
	}

	m_cornerCell.Label(cornerLabel);

	AddHdrCells(m_rows, rows, numRows, UNDEF_SIZE, true);
	AddHdrCells(m_cols, cols, numCols, UNDEF_SIZE, false);

	// recalculate entire geometry
	CalcTableGeometry();

	// assign new model for table data
	m_model = model;
	m_model->AddObserver(this);

	// reset sorting and selection
	m_sortingIndex = UNDEF_SIZE;
	ClearSelection();

	m_tableCreated = true;
	ResizeBackBitmaps();
}

void wxAdvTable::Create(wxAdvHdrCellArray &rows, wxAdvHdrCellArray &cols, const wxString &cornerLabel, wxAdvTableDataModel *model)
{
	wxAdvHdrCell *rowsA = new wxAdvHdrCell[rows.Count()];
	wxAdvHdrCell *colsA = new wxAdvHdrCell[cols.Count()];

	FOREACH_HDRCELL(n, rows) {
		rowsA[n] = rows[n];
	}
	FOREACH_HDRCELL(n, cols) {
		colsA[n] = cols[n];
	}

	Create(rowsA, rows.Count(), colsA, cols.Count(), cornerLabel, model);

	wxDELETEA(rowsA);
	wxDELETEA(colsA);
}

void wxAdvTable::Create(size_t numRows, wxAdvHdrCell *cols, size_t numCols, const wxString &cornerLabel, wxAdvTableDataModel *model)
{
	Create(NULL, numRows, cols, numCols, cornerLabel, model);
}

void wxAdvTable::Create(size_t numRows, size_t numCols, const wxString &cornerLabel, wxAdvTableDataModel *model)
{
	Create(NULL, numRows, NULL, numCols, cornerLabel, model);
}

void wxAdvTable::Create(wxAdvHdrCell *rows, size_t numRows, size_t numCols, const wxString &cornerLabel, wxAdvTableDataModel *model)
{
	Create(rows, numRows, NULL, numCols, cornerLabel, model);
}

void wxAdvTable::DestroyTable()
{
	m_rows.Clear();
	m_cols.Clear();
	m_cornerCell.Label(wxEmptyString);
	m_cornerCell.m_rc = wxRect(0, 0, 0, 0);

	m_decompRows.Clear();
	m_decompCols.Clear();

	m_rowLayerWidths.Clear();
	m_colLayerHeights.Clear();

	if (m_model != NULL) {
		m_model->RemoveObserver(this);
		wxDELETE(m_model);
	}

	ClearSelection();

	m_tableCreated = false;

	SetVirtualSize(0, 0);

	// destroy backbuffers
	m_backBitmap.Create(0, 0);
	m_backBitmapRows.Create(0, 0);
	m_backBitmapCols.Create(0, 0);
	RedrawAll();
}

//
// Rows/columns dynamic add/remove functions.
//

void wxAdvTable::AddRows(wxAdvHdrCell *rows, size_t numRows, size_t before)
{
	wxCHECK_RET(m_tableCreated, wxT("wxAdvTable::AddRows: Table is not created"));

	AddHdrCells(m_rows, rows, numRows, before, true);
	UpdateGeometry();
}

void wxAdvTable::AddCols(wxAdvHdrCell *cols, size_t numCols, size_t before)
{
	wxCHECK_RET(m_tableCreated, wxT("wxAdvTable::AddCols: Table is not created"));

	AddHdrCells(m_cols, cols, numCols, before, false);
	UpdateGeometry();
}

void wxAdvTable::RemoveRows(size_t from, size_t count)
{
	RemoveHdrCells(from, count, true);
}

void wxAdvTable::RemoveCols(size_t from, size_t count)
{
	RemoveHdrCells(from, count, false);
}

void wxAdvTable::RemoveHdrCells(size_t from, size_t count, bool isRows)
{
	wxCHECK_RET(m_tableCreated, wxT("wxAdvTable::RemoveHdrCells: Table is not created"));

	wxAdvHdrCellArray *hdrCells;

	if (isRows) {
		hdrCells = &m_rows;
	}
	else {
		hdrCells = &m_cols;
	}

	if (m_pressedHdrCell != NULL || m_currentHdrCell != NULL || m_resizingHdrCell != NULL) {
		for (size_t index = from; index < (from + count); index++) {
			wxAdvHdrCell *hdrCell = &(*hdrCells)[index];

			if (hdrCell == m_pressedHdrCell) {
				m_pressedHdrCell = NULL;
			}
			if (hdrCell == m_currentHdrCell) {
				m_currentHdrCell = NULL;
			}
			if (hdrCell == m_resizingHdrCell) {
				m_resizingHdrCell = NULL;
			}
		}
	}

	hdrCells->RemoveAt(from, count);
	UpdateGeometry();
}

void wxAdvTable::RemoveHdrCell(wxAdvHdrCell *hdrCell)
{
	if (hdrCell == m_pressedHdrCell) {
		m_pressedHdrCell = NULL;
	}
	if (hdrCell == m_currentHdrCell) {
		m_currentHdrCell = NULL;
	}
	if (hdrCell == m_resizingHdrCell) {
		m_resizingHdrCell = NULL;
	}

	if (hdrCell->m_parent != NULL) {
		hdrCell->m_parent->RemoveSubCell(hdrCell->m_subIndex);
		UpdateGeometry();
	}
	else {
		RemoveHdrCells(hdrCell->m_subIndex, hdrCell->m_subIndex, hdrCell->m_isRow);
	}
}

//
// Table geometry functions.
//

void wxAdvTable::UpdateGeometry()
{
	if (!m_tableCreated) {
		return ;
	}

	m_sortOrder.Empty();

	CalcTableGeometry();
	ResizeBackBitmaps();
	UpdateSortOrder();
}

void wxAdvTable::CalcTableGeometry()
{
	size_t numRowLayers = GetLayerCount(m_rows);
	size_t numColLayers = GetLayerCount(m_cols);

	// update indexes
    {
	    FOREACH_HDRCELL(n, m_rows) {
		    m_rows[n].m_subIndex = n;
	    }
    }

    {
	    FOREACH_HDRCELL(n, m_cols) {
		    m_cols[n].m_subIndex = n;
	    }
    }

	// Layer sizes
	m_rowLayerWidths.Clear();
	m_rowLayerWidths.Alloc(numRowLayers);
	m_rowLayerWidths.Add(0, numRowLayers);

	m_colLayerHeights.Clear();
	m_colLayerHeights.Alloc(numColLayers);
	m_colLayerHeights.Add(0, numColLayers);

	// real rows/columns
	m_decompRows.Clear();
	m_decompCols.Clear();

	DecomposeHdrCells(m_rows, m_decompRows);
	DecomposeHdrCells(m_cols, m_decompCols);

	// update header geometry
	wxClientDC dc(this);
	CalcHeaderGeometry(dc);
}

void wxAdvTable::CalcHeaderGeometry(wxDC &dc)
{
	// setup sizes for all rows and columns
    {
	    FOREACH_HDRCELL(n, m_rows) {
		    FitForSubCells(dc, m_rows[n], true);
	    }
    }

    {
	    FOREACH_HDRCELL(n, m_cols) {
		    FitForSubCells(dc, m_cols[n], false);
	    }
    }

	// calculate layer sizes from row/column maximum heights/widths
    {
	    FOREACH_HDRCELL(n, m_rows) {
		    CalcLayerSizes(m_rows[n], m_rowLayerWidths, true, 0);
	    }
    }

	FOREACH_HDRCELL(n, m_cols) {
		CalcLayerSizes(m_cols[n], m_colLayerHeights, false, 0);
	}

    {
	    // set row/columns heights/widths from layer sizes
	    FOREACH_HDRCELL(n, m_rows) {
		    UpdateHeaderSizes(m_rows[n], m_rowLayerWidths, true, 0);
	    }
    }


    {
	    FOREACH_HDRCELL(n, m_cols) {
		    UpdateHeaderSizes(m_cols[n], m_colLayerHeights, false, 0);
	    }
    }

	// setup positions for all rows and columns
	wxCoord totalColHeight = SumLayerSizes(m_colLayerHeights);
	wxCoord totalRowWidth = SumLayerSizes(m_rowLayerWidths);

	wxCoord x0 = 0;
	wxCoord y0 = 0;

	x0 = 0;
	y0 = (m_showCols || m_showCorner) ? totalColHeight : 0;

    {
	    FOREACH_HDRCELL(n, m_rows) {
		    UpdateHeaderPositions(m_rows[n], x0, y0, m_rowLayerWidths, 0, true);
		    y0 += m_rows[n].m_rc.height;
	    }
    }

	x0 = (m_showRows || m_showCorner) ? totalRowWidth : 0;
	y0 = 0;

    {
	    FOREACH_HDRCELL(n, m_cols) {
		    UpdateHeaderPositions(m_cols[n], x0, y0, m_colLayerHeights, 0, false);
		    x0 += m_cols[n].m_rc.width;
	    }
    }

	m_cornerCell.m_rc = wxRect(0, 0, totalRowWidth, totalColHeight);

	UpdateVirtualSize();
}

void wxAdvTable::UpdateVirtualSize()
{
	wxCoord totalColHeight = SumLayerSizes(m_colLayerHeights);
	wxCoord totalRowWidth = SumLayerSizes(m_rowLayerWidths);

	wxCoord virtWidth = CalcTotalColsWidth();
	wxCoord virtHeight = CalcTotalRowsHeight();

	if (m_showRows || m_showCorner) {
		virtWidth += totalRowWidth;
	}
	if (m_showCols || m_showCorner) {
		virtHeight += totalColHeight;
	}

	SetVirtualSize(virtWidth, virtHeight);
}

wxSize wxAdvTable::FitForSubCells(wxDC &dc, wxAdvHdrCell &hdrCell, bool isRows)
{
	wxSize size;

	if (!hdrCell.IsComposite()) {
		size = hdrCell.CalcExtent(dc);
		hdrCell.m_rc.SetPosition(wxPoint(0, 0));
		hdrCell.m_rc.SetSize(size);
		return size;
	}

	FOREACH_HDRCELL(n, hdrCell.m_subCells) {
		wxAdvHdrCell &subHdrCell = hdrCell.m_subCells[n];

		wxSize subSize;
		if (subHdrCell.IsComposite()) {
			subSize = FitForSubCells(dc, subHdrCell, isRows);
		}
		else {
			subSize = subHdrCell.CalcExtent(dc);
		}

		if (isRows) {
			size.x = wxMax(size.x, subSize.x);
			size.y += subSize.y;
		}
		else {
			size.x += subSize.x;
			size.y = wxMax(size.y, subSize.y);
		}

		subHdrCell.m_rc.SetPosition(wxPoint(0, 0));
		subHdrCell.m_rc.SetSize(subSize);
	}

	hdrCell.m_rc.SetPosition(wxPoint(0, 0));
	hdrCell.m_rc.SetSize(size);
	return size;
}

void wxAdvTable::CalcLayerSizes(wxAdvHdrCell &hdrCell, wxCoordArray &layerSizes, bool isRows, size_t nLayer)
{
	FOREACH_HDRCELL(n, hdrCell.m_subCells) {
		wxAdvHdrCell &subHdrCell = hdrCell.m_subCells[n];
		CalcLayerSizes(subHdrCell, layerSizes, isRows, nLayer + 1);
	}

	wxCoord size;
	if (isRows) {
		size = hdrCell.m_rc.width;
	}
	else {
		size = hdrCell.m_rc.height;
	}

	if (nLayer == LastLayer(hdrCell, layerSizes)) {
		// update layer size if row/column occupies only one layer.
		layerSizes[nLayer] = wxMax(layerSizes[nLayer], size);
	}
}

void wxAdvTable::UpdateHeaderPositions(wxAdvHdrCell &hdrCell, wxCoord x0, wxCoord y0, wxCoordArray &layerSizes, size_t nLayer, bool isRows)
{
	hdrCell.m_rc.x = x0;
	hdrCell.m_rc.y = y0;

	FOREACH_HDRCELL(n, hdrCell.m_subCells) {
		wxAdvHdrCell &subHdrCell = hdrCell.m_subCells[n];

		wxCoord x, y;
		if (isRows) {
			x = x0 + layerSizes[nLayer];
			y = y0;
		}
		else {
			x = x0;
			y = y0 + layerSizes[nLayer];
		}

		UpdateHeaderPositions(subHdrCell, x, y, layerSizes, nLayer + 1, isRows);

		if (isRows) {
			y0 += subHdrCell.m_rc.height;
		}
		else {
			x0 += subHdrCell.m_rc.width;
		}
	}
}

void wxAdvTable::UpdateHeaderSizes(wxAdvHdrCell &hdrCell, wxCoordArray &layerSizes, bool isRows, size_t nLayer)
{
	FOREACH_HDRCELL(n, hdrCell.m_subCells) {
		wxAdvHdrCell &subHdrCell = hdrCell.m_subCells[n];
		UpdateHeaderSizes(subHdrCell, layerSizes, isRows, nLayer + 1);
	}

	wxCoord size = SumLayerSizes(layerSizes, nLayer, LastLayer(hdrCell, layerSizes));
	if (isRows) {
		hdrCell.m_rc.width = size;
	}
	else {
		hdrCell.m_rc.height = size;
	}
}

void wxAdvTable::DecomposeHdrCells(wxAdvHdrCellArray &hdrCells, wxAdvHdrCellPtrArray &decompHdrCells)
{
	FOREACH_HDRCELL(n, hdrCells) {
		wxAdvHdrCell &hdrCell = hdrCells[n];
		DecomposeHdrCell(hdrCell, decompHdrCells);
	}
}

void wxAdvTable::DecomposeHdrCell(wxAdvHdrCell &hdrCell, wxAdvHdrCellPtrArray &decompHdrCells)
{
	if (!hdrCell.IsComposite()) {
		hdrCell.m_decompIndex = decompHdrCells.Count();
		decompHdrCells.Add(&hdrCell);
	}
	else {
		FOREACH_HDRCELL(n, hdrCell.m_subCells) {
			DecomposeHdrCell(hdrCell.m_subCells[n], decompHdrCells);
		}
	}
}

wxCoord wxAdvTable::SumDimensions(wxAdvHdrCellPtrArray &hdrCells, bool widths)
{
	wxCoord sum = 0;
	if (widths) {
		FOREACH_HDRCELL(n, hdrCells) {
			sum += hdrCells[n]->m_rc.width;
		}
	}
	else {
		FOREACH_HDRCELL(n, hdrCells) {
			sum += hdrCells[n]->m_rc.height;
		}
	}
	return sum;
}

wxCoord wxAdvTable::SumLayerSizes(wxCoordArray &layerSizes, size_t first, size_t last)
{
	wxCoord size = 0;
	for (size_t n = first; n <= last && n < layerSizes.Count(); n++) {
		size += layerSizes[n];
	}
	return size;
}

size_t wxAdvTable::GetLayerCount(wxAdvHdrCellArray &hdrCells)
{
	size_t numLayers = 0;
	FOREACH_HDRCELL(n, hdrCells) {
		if (hdrCells[n].Layers() > numLayers)
			numLayers = hdrCells[n].Layers();
	}
	return numLayers;
}

void wxAdvTable::AddHdrCells(wxAdvHdrCellArray &arr, wxAdvHdrCell *hdrCells, size_t numCells, size_t before, bool isRows)
{
	arr.Alloc(arr.Count() + numCells);

	size_t number = arr.Count() + 1;
	for (size_t n = 0; n < numCells; n++) {
		wxAdvHdrCell *cell;

		if (hdrCells != NULL) {
			cell = &hdrCells[n];
		}
		else {
			cell = (isRows) ? &m_defaultRow : &m_defaultCol;
			cell->Label(wxString::Format(wxT("%i"), number++));
		}

		if (isRows) {
			cell->MarkAsRow();
		}
		else {
			cell->MarkAsCol();
		}

		if (before != UNDEF_SIZE) {
			arr.Insert(*cell, before);
		}
		else {
			arr.Add(*cell);
		}
	}

    {
	    FOREACH_HDRCELL(n, arr) {
		    arr[n].m_table = this;
	    }
    }
}

//
// Sorting functions.
//

void wxAdvTable::SetSorter(wxAdvTableSorter *sorter)
{
	wxREPLACE(m_sorter, sorter);
	UpdateSortOrder();
}

wxAdvTableSorter *wxAdvTable::GetSorter()
{
	return m_sorter;
}

void wxAdvTable::SetSortMode(SortMode sortMode)
{
	if (m_sortMode != sortMode) {
		m_sortMode = sortMode;
		m_sortingIndex = UNDEF_SIZE;
		UpdateSortOrder();
	}
}

wxAdvTable::SortMode wxAdvTable::GetSortMode()
{
	return m_sortMode;
}

void wxAdvTable::UpdateSortOrder()
{
	m_sortOrder.Empty();

	if (m_sorter == NULL || m_sortDirection == SortDirNoSorting) {
		RedrawAll();
		return ;
	}

	wxSortHelperArray helpers(SorterFunc);

	// XXX need normal sorting method, this code is very ugly!
	if (m_sortMode == SortRows) {
		if (m_sortingIndex > MaxCol()) {
			RedrawAll();
			return ;
		}

		if (!CanSortByHdrCell(m_decompCols[m_sortingIndex])) {
			RedrawAll();
			return ;
		}

		size_t rowCount = m_decompRows.Count();
		helpers.Alloc(rowCount);

		for (size_t row = 0; row < rowCount; row++) {
			helpers.Add(new SortHelper(this, m_sorter, row, m_sortingIndex, true, m_sortDirection));
		}
	}
	else if (m_sortMode == SortCols) {
		if (m_sortingIndex > MaxRow()) {
			RedrawAll();
			return ;
		}

		if (!CanSortByHdrCell(m_decompRows[m_sortingIndex])) {
			RedrawAll();
			return ;
		}

		size_t colCount = m_decompCols.Count();
		helpers.Alloc(colCount);

		for (size_t col = 0; col < colCount; col++) {
			helpers.Add(new SortHelper(this, m_sorter, m_sortingIndex, col, false, m_sortDirection));
		}
	}
	else {
		RedrawAll();
		return ;
	}

	m_sortOrder.Alloc(helpers.Count());

	for (size_t n = 0; n < helpers.Count(); n++) {
		m_sortOrder.Add(helpers[n]->Index());
	}

    {
	    // destroy helpers
	    for (size_t n = 0; n < helpers.Count(); n++) {
		    delete helpers[n];
	    }
    }

	RedrawAll();
}

bool wxAdvTable::IsSorting()
{
	if (m_sorter == NULL) {
		return false;
	}

	if (m_sortMode == SortRows && m_sortingIndex <= MaxCol()) {
		return true;
	}
	else if (m_sortMode == SortCols && m_sortingIndex <= MaxRow()) {
		return true;
	}
	return false;
}

void wxAdvTable::SetAllowSortByAnyRow(bool sortByAnyRow)
{
	if (m_sortByAnyRow != sortByAnyRow) {
		m_sortByAnyRow = sortByAnyRow;
		UpdateSortOrder();
	}
}

bool wxAdvTable::IsAllowSortByAnyRow()
{
	return m_sortByAnyRow;
}

void wxAdvTable::SetAllowSortByAnyCol(bool sortByAnyCol)
{
	if (m_sortByAnyCol != sortByAnyCol) {
		m_sortByAnyCol = sortByAnyCol;
		UpdateSortOrder();
	}
}

bool wxAdvTable::IsAllowSortByAnyCol()
{
	return m_sortByAnyCol;
}

void wxAdvTable::SetSortingIndex(size_t sortingIndex, bool invert)
{
	if (sortingIndex != UNDEF_SIZE) {
		if (invert && m_sortingIndex == sortingIndex) {
			switch (m_sortDirection) {
			case SortDirAscending:
				m_sortDirection = SortDirDescending;
				break;
			case SortDirDescending:
				m_sortDirection = SortDirNoSorting;
				break;
			case SortDirNoSorting:
				m_sortDirection = SortDirAscending;
				break;
			}
		}
		else {
			m_sortDirection = SortDirAscending;
		}
	}
	else {
		m_sortDirection = SortDirNoSorting;
	}

	m_sortingIndex = sortingIndex;
	UpdateSortOrder();
}

void wxAdvTable::SetSortDirection(SortDirection sortDirection)
{
	if (m_sortDirection != sortDirection) {
		m_sortDirection = sortDirection;
		UpdateSortOrder();
	}
}

wxAdvTable::SortDirection wxAdvTable::GetSortDirection()
{
	return m_sortDirection;
}

bool wxAdvTable::CanSortByHdrCell(wxAdvHdrCell *hdrCell)
{
	if (hdrCell->IsComposite()) {
		return false;
	}
	return (hdrCell->m_isRow && m_sortMode == SortCols && (hdrCell->IsSortable() || m_sortByAnyRow))
			|| (!hdrCell->m_isRow && m_sortMode == SortRows && (hdrCell->IsSortable() || m_sortByAnyCol));
}

//
// Selection, highlight, etc functions.
//

void wxAdvTable::SetSelectMode(SelectMode selectMode)
{
	if (m_selectMode != selectMode) {
		m_selectMode = selectMode;
		ClearSelection();
	}
}

int wxAdvTable::GetSelectMode()
{
	return m_selectMode;
}

void wxAdvTable::SetHighlightMode(HighlightMode highlightMode)
{
	if (m_highlightMode != highlightMode) {
		m_highlightMode = highlightMode;

		RedrawAll();
	}
}

void wxAdvTable::ClearSelection()
{
	m_focused.Unset();
	m_pressedCell.Unset();
	m_selected.Unset();

	RedrawAll();
}

void wxAdvTable::SetShowHeaders(bool showRows, bool showCols, bool showCorner)
{
	m_showRows = showRows;
	m_showCols = showCols;
	m_showCorner = showCorner;

	wxClientDC dc(this);
	CalcHeaderGeometry(dc);

	RedrawAll();
}

//
// Drawing functions.
//

void wxAdvTable::DrawHdrCells(wxDC &dc, wxAdvHdrCellArray &hdrCells)
{
	FOREACH_HDRCELL(n, hdrCells) {
		DrawHdrCell(dc, &hdrCells[n]);
	}
}

void wxAdvTable::DrawHdrCell(wxDC &dc, wxAdvHdrCell *hdrCell)
{
	DrawHdrCellSelf(dc, hdrCell);

	// draw subcells
	FOREACH_HDRCELL(n, hdrCell->m_subCells) {
		DrawHdrCell(dc, &hdrCell->m_subCells[n]);
	}
}

void wxAdvTable::DrawHdrCellSelf(wxDC &dc, wxAdvHdrCell *hdrCell)
{
	wxDCClipper clip(dc, hdrCell->m_rc);

	int sortDirection = SortDirNoSorting;
	bool pressed = false;
	bool selected = false;

	if (IsSorting() && m_sortingIndex == hdrCell->m_decompIndex) {
		if (CanSortByHdrCell(hdrCell)) {
			sortDirection = m_sortDirection;
		}
	}

	if (hdrCell == m_pressedHdrCell) {
		pressed = true;
	}
	if (hdrCell == m_currentHdrCell) {
		selected = true;
	}

	wxAdvHdrCellRenderer *renderer = NULL;
	if (hdrCell->m_renderer != NULL) {
		renderer = hdrCell->m_renderer;
	}
	else {
		renderer = m_defaultHdrRenderer;
	}
	renderer->Draw(this, dc, hdrCell, selected, pressed, sortDirection);
}

void wxAdvTable::DrawTable(wxDC &dc)
{
	FOREACH_HDRCELL(row, m_decompRows) {
		FOREACH_HDRCELL(col, m_decompCols) {
			DrawCell(dc, row, col);
		}
	}
}

void wxAdvTable::DrawCell(wxDC &dc, size_t row, size_t col)
{
	bool highlighed = IsCellHighlighted(row, col);
	bool selected = IsCellSelected(row, col);
	bool focused = IsCellFocused(row, col);

	if (selected) {
		dc.SetBrush(m_selectedBgBrush);
	}
	else {
		dc.SetBrush(m_bgBrush);
	}

	if (highlighed) {
		dc.SetBrush(m_highlightedBgBrush);
	}

	if (focused) {
		dc.SetBrush(m_focusedBgBrush);
		dc.SetPen(m_focusedPen);
	}
	else {
		dc.SetPen(m_gridPen);
	}

	size_t modelRow, modelCol;
	ToModelCellCoord(row, col, modelRow, modelCol);

	wxRect rc = GetCellRect(row, col);
	wxString value = m_model->GetCellValue(modelRow, modelCol);

	wxAdvCellAttribute *attr = m_model->GetCellAttribute(modelRow, modelCol);
	if (attr == NULL) {
		attr = &m_defaultCellAttribute;
	}

	dc.DrawRectangle(rc);
	GetRendererForCell(row, col)->Draw(this, dc, rc, value, selected, focused, attr);
}

void wxAdvTable::DrawBackground(wxDC &dc, int width, int height)
{
	wxColour bgColour = GetBackgroundColour();

	dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(bgColour));
	dc.SetPen(*wxThePenList->FindOrCreatePen(bgColour, 1, wxSOLID));
	dc.DrawRectangle(wxRect(0, 0, width, height));
}

wxPoint wxAdvTable::ToViewportPosition(const wxPoint &pt, bool transVert, bool transHoriz)
{
	int dx, dy;
	int xunit, yunit;

	GetViewStart(&dx, &dy);
	GetScrollPixelsPerUnit(&xunit, &yunit);

	int tx = (transHoriz) ? (dx * xunit) : 0;
	int ty = (transVert) ? (dy * yunit) : 0;

	return wxPoint(pt.x + tx, pt.y + ty);
}

bool wxAdvTable::GetCellAt(const wxPoint &pt, size_t &cellRow, size_t &cellCol)
{
	wxPoint ptVP = ToViewportPosition(pt);

	FOREACH_HDRCELL(row, m_decompRows) {
		wxAdvHdrCell *rowCell = m_decompRows[row];

		if (ptVP.y > rowCell->m_rc.y && ptVP.y < (rowCell->m_rc.y + rowCell->m_rc.height)) {

			FOREACH_HDRCELL(col, m_decompCols) {
				wxAdvHdrCell *colCell = m_decompCols[col];

				if (ptVP.x > colCell->m_rc.x && ptVP.x < (colCell->m_rc.x + colCell->m_rc.width)) {
					cellRow = row;
					cellCol = col;
					return true;
				}
			}
		}
	}
	return false;
}

wxAdvHdrCell *wxAdvTable::GetHdrCellAt(const wxPoint &pt)
{
	wxAdvHdrCell *cell;
	if (m_showRows) {
		wxPoint ptRow = ToViewportPosition(pt, true, false);

		FOREACH_HDRCELL(row, m_rows) {
			if ((cell = GetHdrCellAtRecursive(&m_rows[row], ptRow)) != NULL) {
				return cell;
			}
		}
	}

	if (m_showCols) {
		wxPoint ptCol = ToViewportPosition(pt, false, true);

		FOREACH_HDRCELL(col, m_cols) {
			if ((cell = GetHdrCellAtRecursive(&m_cols[col], ptCol)) != NULL) {
				return cell;
			}
		}
	}
	return NULL;
}

wxAdvHdrCell *wxAdvTable::GetHdrCellAtRecursive(wxAdvHdrCell *cell, wxPoint &pt)
{
	if (cell->m_rc.Contains(pt)) {
		return cell;
	}
	else if (cell->IsComposite()) {
		FOREACH_HDRCELL(n, cell->m_subCells) {
			wxAdvHdrCell *subCell = GetHdrCellAtRecursive(&cell->m_subCells[n], pt);
			if (subCell != NULL) {
				return subCell;
			}
		}
	}
	return NULL;
}

wxString wxAdvTable::GetCellValue(size_t row, size_t col)
{
	ToModelCellCoord(row, col, row, col);
	return m_model->GetCellValue(row, col);
}

void wxAdvTable::ChangeCellValue(size_t row, size_t col, wxString value)
{
	if (SendEvent(wxEVT_GRID_CELL_CHANGE, row, col)) {
		GetModel()->SetCellValue(row, col, value);
		UpdateSortOrder();
	}
}

bool wxAdvTable::IsCellEditable(size_t row, size_t col)
{
	ToModelCellCoord(row, col, row, col);
	return m_model->IsCellEditable(row, col);
}


void wxAdvTable::ToModelCellCoord(size_t row, size_t col, size_t &modelRow, size_t &modelCol)
{
	if (!IsSorting() || m_sortOrder.Count() == 0) {
		modelRow = row;
		modelCol = col;
	}
	else if (m_sortMode == SortRows) {
		modelRow = m_sortOrder[row];
		modelCol = col;
	}
	else if (m_sortMode == SortCols) {
		modelRow = row;
		modelCol = m_sortOrder[col];
	}
}

void wxAdvTable::ToCellCoord(size_t row, size_t col, size_t &tableRow, size_t &tableCol)
{
	if (!IsSorting() || m_sortOrder.Count() == 0) {
		tableRow = row;
		tableCol = col;
	}
	else if (m_sortMode == SortRows) {
		for (size_t n = 0; n < m_sortOrder.Count(); n++) {
			if (m_sortOrder[n] == row) {
				tableRow = n;
				break;
			}
		}

		tableCol = col;
	}
	else if (m_sortMode == SortCols) {
		tableRow = row;

		for (size_t n = 0; n < m_sortOrder.Count(); n++) {
			if (m_sortOrder[n] == row) {
				tableCol = n;
				break;
			}
		}
	}
}

wxAdvCellEditor *wxAdvTable::GetEditorForCell(size_t row, size_t col)
{
	ToModelCellCoord(row, col, row, col);
	int format = m_model->GetCellFormat(row, col);

	if (m_editors.find(format) == m_editors.end()) {
		return NULL;
	}
	return m_editors[format];
}

void wxAdvTable::SetEditorForFormat(int format, wxAdvCellEditor *editor)
{
	if (m_editors.find(format) != m_editors.end()) {
		delete m_editors[format];
	}
	m_editors[format] = editor;
}

wxAdvCellRenderer *wxAdvTable::GetRendererForCell(size_t row, size_t col)
{
	ToModelCellCoord(row, col, row, col);
	int format = m_model->GetCellFormat(row, col);

	if (m_renderers.find(format) == m_renderers.end()) {
		return m_defaultRenderer;
	}
	return m_renderers[format];
}

void wxAdvTable::SetRendererForFormat(int format, wxAdvCellRenderer *renderer)
{
	if (m_renderers.find(format) != m_renderers.end()) {
		delete m_renderers[format];
	}
	m_renderers[format] = renderer;
	RedrawAll();
}

void wxAdvTable::SetDefaultRenderer(wxAdvCellRenderer *renderer)
{
	wxREPLACE(m_defaultRenderer, renderer);
	RedrawAll();
}

bool wxAdvTable::GetCellRect(wxRect &rc, size_t row, size_t col)
{
	if (row < m_decompRows.Count() && col < m_decompCols.Count()) {
		rc.x = m_decompCols[col]->m_rc.x;
		rc.y = m_decompRows[row]->m_rc.y;
		rc.width = m_decompCols[col]->m_rc.width;
		rc.height = m_decompRows[row]->m_rc.height;
		return true;
	}
	return false;
}

wxRect wxAdvTable::GetCellRect(size_t row, size_t col)
{
	wxRect rc;
	GetCellRect(rc, row, col);
	return rc;
}

wxRect wxAdvTable::GetRangeRect(size_t row1, size_t col1, size_t row2, size_t col2)
{
	wxRect rcCell1;
	wxRect rcCell2;

	GetCellRect(rcCell1, row1, col1);
	GetCellRect(rcCell2, row2, col2);

	wxRect rc;
	rc.x = rcCell1.x;
	rc.y = rcCell1.y;

	rc.width = rcCell2.x - rcCell1.x + rcCell2.width;
	rc.height = rcCell2.y - rcCell1.y + rcCell2.height;
	return rc;
}

wxRect wxAdvTable::GetRangeRect(wxAdvRange &range)
{
	return GetRangeRect(range.Row1(), range.Col1(), range.Row2(), range.Col2());
}

//
// Graphical objects functions.
//

void wxAdvTable::SetBgBrush(const wxBrush &bgBrush)
{
	m_bgBrush = bgBrush;
	RedrawAll();
}

const wxBrush &wxAdvTable::GetBgBrush()
{
	return m_bgBrush;
}

void wxAdvTable::SetSelectedBgBrush(const wxBrush &selectedBgBrush)
{
	m_selectedBgBrush = selectedBgBrush;
	RedrawRange(&m_selected);
}

const wxBrush &wxAdvTable::GetSelectedBgBrush()
{
	return m_selectedBgBrush;
}

void wxAdvTable::SetGridPen(const wxPen &gridPen)
{
	m_gridPen = gridPen;
	RedrawAll();
}

const wxPen &wxAdvTable::GetGridPen()
{
	return m_gridPen;
}

void wxAdvTable::SetFocusedPen(const wxPen &focusedPen)
{
	m_focusedPen = focusedPen;

	if (m_focused.IsSet()) {
		RedrawRange(m_focused.Row(), m_focused.Col(), m_focused.Row(), m_focused.Col());
	}
}

const wxPen &wxAdvTable::GetFocusedPen()
{
	return m_focusedPen;
}

void wxAdvTable::SetFocusedBgBrush(const wxBrush &focusedBgBrush)
{
	m_focusedBgBrush = focusedBgBrush;

	if (m_focused.IsSet()) {
		RedrawRange(m_focused.Row(), m_focused.Col(), m_focused.Row(), m_focused.Col());
	}
}

const wxBrush &wxAdvTable::GetFocusedBgBrush()
{
	return m_focusedBgBrush;
}

void wxAdvTable::SetHighlightedBgBrush(const wxBrush &highlightedBgBrush)
{
	m_highlightedBgBrush = highlightedBgBrush;
	RedrawAll();
}

const wxBrush &wxAdvTable::GetHighlightedBgBrush()
{
	return m_highlightedBgBrush;
}

//
// Selection functions
//

void wxAdvTable::SetSelection(wxAdvRange &range)
{
	SetSelection(range.Row1(), range.Col1(), range.Row2(), range.Col2());
}

void wxAdvTable::SetSelection(size_t row, size_t col)
{
	SetSelection(row, col, row, col);
}

void wxAdvTable::SetSelection(size_t row1, size_t col1, size_t row2, size_t col2)
{
	wxAdvRange oldSelected = m_selected;

	m_selected.Set(row1, col1, row2, col2);

	RedrawRange(&oldSelected);
	RedrawRange(&m_selected);
}


//
// Selection internal functions.
//

void wxAdvTable::SelectCells(const wxMouseEvent &ev, size_t row1, size_t col1, size_t row2, size_t col2)
{
	SelectCells(row1, col1, row2, col2,
			ev.ControlDown(),
			ev.ShiftDown(),
			ev.AltDown(),
			ev.MetaDown());
}

void wxAdvTable::SelectCells(const wxKeyEvent &ev, size_t row1, size_t col1, size_t row2, size_t col2)
{
	SelectCells(row1, col1, row2, col2,
			ev.ControlDown(),
			ev.ShiftDown(),
			ev.AltDown(),
			ev.MetaDown());
}

void wxAdvTable::SelectCells(size_t row1, size_t col1, size_t row2, size_t col2, bool controlDown, bool shiftDown, bool altDown, bool metaDown)
{
	if (m_selected.Equals(row1, col1, row2, col2)) {
		// selection not changed
		return ;
	}

	wxAdvRange oldSelected = m_selected; // save old selection to redraw

	switch (m_selectMode) {
	case SelectCell:
		m_selected.Set(row2, col2);
		break;
	case SelectRows:
		m_selected.Set(row1, 0, row2, MaxCol());
		break;
	case SelectCols:
		m_selected.Set(0, col1, MaxRow(), col2);
		break;
	case SelectBlock:
		m_selected.Set(row1, col1, row2, col2);
		break;
	}

	if (m_selected.IsSingleCell()) {
		SendEvent(wxEVT_GRID_SELECT_CELL,
				m_selected.TopRow(), m_selected.LeftCol());
	}
	else {
		SendRangeEvent(wxEVT_GRID_RANGE_SELECT,
				m_selected.TopRow(), m_selected.LeftCol(),
				m_selected.BottomRow(), m_selected.RightCol(),
				controlDown, shiftDown, altDown, metaDown);
	}

	// redraw old selection
	RedrawRange(&oldSelected);
	// redraw new selection
	RedrawRange(&m_selected);
}

void wxAdvTable::SetFocusedCell(size_t row, size_t col)
{
	if (row > MaxRow() || col > MaxCol()) {
		return ;
	}

	if (m_focused.Equal(row, col)) {
		return ;
	}

	wxAdvCoord oldFocused = m_focused;

	m_focused.Set(row, col);

	// redraw old focused
	RedrawHighlighted(&oldFocused);

	StopEditing();

	// redraw new focused
	RedrawHighlighted(&m_focused);
}

void wxAdvTable::SetPressedHdrCell(wxAdvHdrCell *cell)
{
	if (m_pressedHdrCell == cell) {
		return ;
	}

	// redraw old pressed header cell
	if (m_pressedHdrCell != NULL) {
		wxAdvHdrCell *oldPressedHdrCell = m_pressedHdrCell;
		m_pressedHdrCell = NULL;
		RedrawHdrCell(oldPressedHdrCell);
	}

	m_pressedHdrCell = cell;
	if (m_pressedHdrCell != NULL) {
		RedrawHdrCell(m_pressedHdrCell);
	}
}

void wxAdvTable::SetCurrentHdrCell(wxAdvHdrCell *cell)
{
	if (m_currentHdrCell == cell) {
		return ;
	}

	if (m_currentHdrCell != NULL) {
		wxAdvHdrCell *oldCurrentHdrCell = m_currentHdrCell;
		m_currentHdrCell = NULL;
		RedrawHdrCell(oldCurrentHdrCell);
	}

	m_currentHdrCell = cell;
	if (m_currentHdrCell != NULL) {
		RedrawHdrCell(m_currentHdrCell);
	}
}

void wxAdvTable::StopEditing()
{
	if (m_activeEditor != NULL) {
		wxAdvCellEditor *editor = m_activeEditor;

		m_activeEditor = NULL;
		editor->Deactivate();

		SendEvent(wxEVT_GRID_EDITOR_HIDDEN, UNDEF_SIZE, UNDEF_SIZE);

		SetFocus();
	}
}

void wxAdvTable::ScrollToCell(size_t row, size_t col)
{
	wxAdvRange visibleRange;

	if (!GetVisibleRange(visibleRange)) {
		return ;
	}

	if (row > MaxRow() || col > MaxCol()) {
		return ;
	}

	bool scrolled = false;

	int x, y;
	int xUnit, yUnit;

	GetViewStart(&x, &y);
	GetScrollPixelsPerUnit(&xUnit, &yUnit);

	// scroll row
	wxAdvHdrCell *rowCell = GetDecompRow(row);

	if (row <= visibleRange.TopRow()) {
		wxAdvHdrCell *firstRow = GetDecompRow(0);

		y = (rowCell->m_rc.y - firstRow->m_rc.y) / yUnit;
		scrolled = true;
	}
	else if (row >= visibleRange.BottomRow()) {
		wxAdvHdrCell *boundRow = GetDecompRow(visibleRange.BottomRow());

		y += ((rowCell->m_rc.y + rowCell->m_rc.height) - boundRow->m_rc.y) / yUnit;
		scrolled = true;
	}

	// scroll column
	wxAdvHdrCell *colCell = GetDecompCol(col);

	if (col <= visibleRange.LeftCol()) {
		wxAdvHdrCell *firstCol = GetDecompCol(0);

		x = (colCell->m_rc.x - firstCol->m_rc.x) / xUnit;
		scrolled = true;
	}
	else if (col >= visibleRange.RightCol()) {
		wxAdvHdrCell *boundCol = GetDecompCol(visibleRange.RightCol());

		x += ((colCell->m_rc.x + colCell->m_rc.width) - boundCol->m_rc.x) / xUnit;
		scrolled = true;
	}

	if (scrolled) {
		Scroll(x, y);
		RedrawAll();
	}
}

//
// Redraw functions
//

void wxAdvTable::ResizeBackBitmaps()
{
	if (!m_tableCreated) {
		// destroy backbuffers
		m_backBitmap.Create(0, 0);
		m_backBitmapCols.Create(0, 0);
		m_backBitmapRows.Create(0, 0);
		return ;
	}

	wxSize size = GetClientSize();

	wxSize dataSize = size;

	wxCoord rowsHeight = CalcTotalRowsHeight();
	wxCoord colsWidth = CalcTotalColsWidth();

	if (m_showRows) {
		wxCoord rowsWidth = CalcTotalRowLayersWidth();

		if (rowsWidth <= dataSize.x) {
			dataSize.x -= rowsWidth;
		}
		else {
			dataSize.x = 0;
		}

		ResizeBitmap(m_backBitmapRows, rowsWidth, wxMin(rowsHeight, size.GetHeight()));
	}
	else {
		m_backBitmapRows.Create(0, 0);
	}

	if (m_showCols) {
		wxCoord colsHeight = CalcTotalColLayersHeight();

		if (colsHeight <= dataSize.y) {
			dataSize.y -= colsHeight;
		}
		else {
			dataSize.y = 0;
		}

		ResizeBitmap(m_backBitmapCols, wxMin(colsWidth, size.GetWidth()), colsHeight);
	}
	else {
		m_backBitmapCols.Create(0, 0);
	}

	if ((!m_showCols && !m_showRows) && m_showCorner) {
		wxCoord rowsWidth = CalcTotalRowLayersWidth();
		wxCoord colsHeight = CalcTotalColLayersHeight();

		if (rowsWidth <= dataSize.x) {
			dataSize.x -= rowsWidth;
		}
		else {
			dataSize.x = 0;
		}

		if (colsHeight <= dataSize.y) {
			dataSize.y -= colsHeight;
		}
		else {
			dataSize.y = 0;
		}
	}

	ResizeBitmap(m_backBitmap, wxMin(colsWidth, dataSize.GetWidth()), wxMin(rowsHeight, dataSize.GetHeight()));

	RedrawAll();
}

void wxAdvTable::RedrawRange(wxAdvRange *range)
{
	RedrawRange(range->TopRow(), range->LeftCol(), range->BottomRow(), range->RightCol());
}

void wxAdvTable::RedrawRange(size_t row1, size_t col1, size_t row2, size_t col2)
{
	if (!m_tableCreated) {
		return ;
	}

	wxAdvRange visibleRange;
	if (!GetVisibleRange(visibleRange)) {
		return ;
	}

	wxPoint viewStart = ToViewportPosition(wxPoint(0, 0));

	row1 = wxMax(row1, visibleRange.TopRow());
	col1 = wxMax(col1, visibleRange.LeftCol());
	row2 = wxMin(row2, visibleRange.BottomRow());
	col2 = wxMin(col2, visibleRange.RightCol());

	// redraw range cells
	wxMemoryDC dc(m_backBitmap);

	wxRect rcData = CalcTableRect();
	dc.SetDeviceOrigin(-viewStart.x - rcData.x, -viewStart.y - rcData.y);

	for (size_t row = row1; row <= row2; row++) {
		for (size_t col = col1; col <= col2; col++) {
			DrawCell(dc, row, col);
		}
	}

	// refresh range rectangle
	wxRect rc = GetRangeRect(row1, col1, row2, col2);
	rc.x -= viewStart.x;
	rc.y -= viewStart.y;

	RefreshRect(rc);
}

void wxAdvTable::RedrawAll()
{
	if (!m_tableCreated) {
		return ;
	}

	wxPoint viewStart = ToViewportPosition(wxPoint(0, 0));
	wxRect rcData = CalcTableRect();

	// draw table
	wxMemoryDC dataDc(m_backBitmap);

	DrawBackground(dataDc, m_backBitmap.GetWidth(), m_backBitmap.GetHeight());
	dataDc.SetDeviceOrigin(-viewStart.x - rcData.x, -viewStart.y - rcData.y);
	DrawTable(dataDc);

	// draw rows
	if (m_showRows) {
		wxMemoryDC rowsDc(m_backBitmapRows);

		DrawBackground(rowsDc, m_backBitmapRows.GetWidth(), m_backBitmapRows.GetHeight());

		rowsDc.SetDeviceOrigin(0, -viewStart.y - rcData.y);
		DrawHdrCells(rowsDc, m_rows);
	}

	// draw columns
	if (m_showCols) {
		wxMemoryDC colsDc(m_backBitmapCols);

		DrawBackground(colsDc, m_backBitmapCols.GetWidth(), m_backBitmapCols.GetHeight());

		colsDc.SetDeviceOrigin(-viewStart.x - rcData.x, 0);
		DrawHdrCells(colsDc, m_cols);
	}

	Refresh();
}

void wxAdvTable::RedrawHdrCell(wxAdvHdrCell *cell)
{
	wxRect rc = cell->m_rc;

	wxRect rcData = CalcTableRect();

	wxPoint viewStart = ToViewportPosition(wxPoint(0, 0));

	if (cell->m_isRow) {
		wxMemoryDC dc(m_backBitmapRows);
		dc.SetDeviceOrigin(0, -viewStart.y - rcData.y);

		DrawHdrCell(dc, cell);

		rc.y -= viewStart.y;
	}
	else {
		wxMemoryDC dc(m_backBitmapCols);
		dc.SetDeviceOrigin(-viewStart.x - rcData.x, 0);

		DrawHdrCell(dc, cell);

		rc.x -= viewStart.x;
	}

	RefreshRect(rc);
}

void wxAdvTable::RedrawHighlighted(wxAdvCoord *coord)
{
	if (!coord->IsSet()) {
		return ;
	}

	switch (m_highlightMode) {
		case HighlightNone:
			RedrawRange(coord->Row(), coord->Col(), coord->Row(), coord->Col());
			break;
		case HighlightRows:
			RedrawRange(coord->Row(), 0, coord->Row(), MaxCol());
			break;
		case HighlightCols:
			RedrawRange(0, coord->Col(), MaxRow(), coord->Col());
			break;
		case HighlightBoth:
			RedrawRange(coord->Row(), 0, coord->Row(), MaxCol());
			RedrawRange(0, coord->Col(), MaxRow(), coord->Col());
			break;
	}
}

bool wxAdvTable::GetVisibleRange(wxAdvRange &range)
{
	size_t row1 = UNDEF_SIZE;
	size_t col1 = UNDEF_SIZE;
	size_t row2 = UNDEF_SIZE;
	size_t col2 = UNDEF_SIZE;

	wxPoint viewStart = ToViewportPosition(wxPoint(0, 0));

	wxRect tableRc = CalcTableRect();
	wxCoord x0 = tableRc.x + viewStart.x;
	wxCoord y0 = tableRc.y + viewStart.y;
	wxCoord x1 = tableRc.x + viewStart.x + m_backBitmap.GetWidth();
	wxCoord y1 = tableRc.y + viewStart.y + m_backBitmap.GetHeight();

	for (size_t row = 0; row < m_decompRows.Count(); row++) {
		wxAdvHdrCell *rowCell = m_decompRows[row];

		if (y0 >= rowCell->m_rc.y &&
			y0 <= (rowCell->m_rc.y + rowCell->m_rc.height)) {
				row1 = row;
				break;
		}
	}
	for (size_t col = 0; col < m_decompCols.Count(); col++) {
		wxAdvHdrCell *colCell = m_decompCols[col];

		if (x0 >= colCell->m_rc.x &&
			x0 <= (colCell->m_rc.x + colCell->m_rc.width)) {
				col1 = col;
				break;
		}
	}

	if (row1 == UNDEF_SIZE || col1 == UNDEF_SIZE) {
		return false;
	}

    {
	    for (size_t row = row1; row < m_decompRows.Count(); row++) {
		    wxAdvHdrCell *rowCell = m_decompRows[row];

		    row2 = row;

		    if (y1 > rowCell->m_rc.y &&
			    y1 <= (rowCell->m_rc.y + rowCell->m_rc.height)) {
				    break;
		    }
	    }
    }

    {
	    for (size_t col = col1; col < m_decompCols.Count(); col++) {
		    wxAdvHdrCell *colCell = m_decompCols[col];

		    col2 = col;
		    if (x1 > colCell->m_rc.x &&
			    x1 <= (colCell->m_rc.x + colCell->m_rc.width)) {
				    break;
		    }
	    }
    }

	range.Set(row1, col1, row2, col2);
	return true;
}

//
// Table area calculation functions.
//

wxRect wxAdvTable::CalcTableRect()
{
	wxCoord x = 0;
	wxCoord y = 0;

	if (m_showCorner || m_showRows) {
		x = CalcTotalRowLayersWidth();
	}
	if (m_showCorner || m_showCols) {
		y = CalcTotalColLayersHeight();
	}

	return wxRect(x, y,
			CalcTotalColsWidth(), CalcTotalRowsHeight());
}

wxRect wxAdvTable::CalcRowsAreaRect()
{
	if (m_showRows) {
		wxCoord y = (m_showCols || m_showCorner) ? CalcTotalColLayersHeight() : 0;
		return wxRect(0, y,
				CalcTotalRowLayersWidth(), CalcTotalRowsHeight());
	}
	else {
		return wxRect(0, 0, 0, 0);
	}
}

wxRect wxAdvTable::CalcColsAreaRect()
{
	if (m_showCols) {
		wxCoord x = (m_showRows || m_showCorner) ? SumLayerSizes(m_rowLayerWidths) : 0;
		return wxRect(x, 0,
				CalcTotalColsWidth(), SumLayerSizes(m_colLayerHeights));
	}
	else {
		return wxRect(0, 0, 0, 0);
	}
}

wxRect wxAdvTable::CalcRowRect(size_t row, size_t col)
{
	wxRect rcTable = CalcTableRect();
	wxRect rc;

	rc = GetCellRect(row, col);
	rc.x = rcTable.x;
	rc.width = rcTable.width;
	return rc;
}

wxRect wxAdvTable::CalcColRect(size_t row, size_t col)
{
	wxRect rcTable = CalcTableRect();
	wxRect rc;

	rc = GetCellRect(row, col);
	rc.y = rcTable.y;
	rc.height = rcTable.height;
	return rc;
}

bool wxAdvTable::IsCellHighlighted(size_t row, size_t col)
{
	switch (m_highlightMode) {
		case HighlightNone:
			return false;
		case HighlightRows:
			return m_focused.Row() == row;
		case HighlightCols:
			return m_focused.Col() == col;
		case HighlightBoth:
			return (m_focused.Row() == row) || (m_focused.Col() == col);
		default:
			return false;
	}
}

void wxAdvTable::EditCell(wxAdvCellEditor *editor, size_t row, size_t col)
{
	if (m_activeEditor != NULL) {
		m_activeEditor->Deactivate();
		SendEvent(wxEVT_GRID_EDITOR_HIDDEN, UNDEF_SIZE, UNDEF_SIZE);
	}

	editor->Activate(row, col);
	m_activeEditor = editor;

	SendEvent(wxEVT_GRID_EDITOR_SHOWN, UNDEF_SIZE, UNDEF_SIZE);

	if (!m_activeEditor->IsActive()) {
		m_activeEditor = NULL;
		SendEvent(wxEVT_GRID_EDITOR_HIDDEN, UNDEF_SIZE, UNDEF_SIZE);
	}
}

bool wxAdvTable::ResizeHdrCellSelf(wxAdvHdrCell *hdrCell, int d, int shift)
{
	wxCoord newSize;

	if (hdrCell->IsRow()) {
		newSize = hdrCell->m_rc.height + d;
	}
	else {
		newSize = hdrCell->m_rc.width + d;
	}

	bool canResizeMore = true;

	if (newSize < hdrCell->MinSize()) {
		canResizeMore = false;
		newSize = hdrCell->MinSize();
	}
	else if (hdrCell->MaxSize() != (wxCoord) UNDEF_SIZE && newSize > hdrCell->MaxSize()) {
		canResizeMore = false;
		newSize = hdrCell->MaxSize();
	}

	if (hdrCell->IsRow()) {
		hdrCell->m_rc.height = newSize;
		if (canResizeMore) {
			hdrCell->m_rc.x += shift;
		}
	}
	else {
		hdrCell->m_rc.width = newSize;
		if (canResizeMore) {
			hdrCell->m_rc.y += shift;
		}
	}

	hdrCell->m_size = newSize;
	return canResizeMore;
}

void wxAdvTable::ResizeHdrCell(const wxMouseEvent &ev, wxAdvHdrCell *hdrCell)
{
	int d;
	if (hdrCell->m_isRow) {
		wxPoint pt = ToViewportPosition(ev.GetPosition(), true, false);
		d = pt.y - hdrCell->m_rc.y - hdrCell->m_rc.height;
	}
	else {
		wxPoint pt = ToViewportPosition(ev.GetPosition(), false, true);
		d = pt.x - hdrCell->m_rc.x - hdrCell->m_rc.width;
	}

	wxAdvHdrCell *resizeCell = hdrCell;
	while (resizeCell->IsComposite()) {
		resizeCell = &resizeCell->m_subCells.Last();
	}

	// TODO: optimize
	if (ResizeHdrCellSelf(resizeCell, d, 0)) {
		UpdateGeometry();

		SendHdrCellEvent(wxEVT_ADVTABLE_HDRCELL_SIZE, resizeCell, ev);
	}
}

//
// Event send functions.
//

bool wxAdvTable::SendEvent(const wxEventType type, size_t row, size_t col, const wxMouseEvent &mouseEv)
{
	wxGridEvent ev(GetId(), type, this,
               (int) row, (int) col,
               mouseEv.GetX(),
               mouseEv.GetY(),
               false/*XXX FIXME*/,
               mouseEv.ControlDown(),
               mouseEv.ShiftDown(),
               mouseEv.AltDown(),
               mouseEv.MetaDown());

	GetEventHandler()->ProcessEvent(ev);
	if (!ev.IsAllowed()) {
		return false;
	}
	return true;
}

bool wxAdvTable::SendEvent(const wxEventType type, size_t row, size_t col)
{
	wxGridEvent ev(GetId(), type, this,
	               (int) row, (int) col);
	GetEventHandler()->ProcessEvent(ev);
	if (!ev.IsAllowed()) {
		return false;
	}
	return true;
}

bool wxAdvTable::SendRangeEvent(const wxEventType type, size_t row1, size_t col1, size_t row2, size_t col2, const wxMouseEvent &mouseEv)
{
	return SendRangeEvent(type, row1, col1, row2, col2,
               mouseEv.ControlDown(),
               mouseEv.ShiftDown(),
               mouseEv.AltDown(),
               mouseEv.MetaDown());
}

bool wxAdvTable::SendRangeEvent(const wxEventType type, size_t row1, size_t col1, size_t row2, size_t col2,
		bool controlDown, bool shiftDown, bool altDown,	bool metaDown)
{
	wxGridRangeSelectEvent ev(GetId(), type, this,
			wxGridCellCoords((int) row1, (int) col1),
			wxGridCellCoords((int) row2, (int) col2),
			false/*XXX FIXME*/,
			controlDown,
			shiftDown,
			altDown,
			metaDown);

	GetEventHandler()->ProcessEvent(ev);
	if (!ev.IsAllowed()) {
		return false;
	}
	return true;
}

bool wxAdvTable::SendHdrCellEvent(const wxEventType type, wxAdvHdrCell *hdrCell, const wxMouseEvent &mouseEv)
{
	return SendHdrCellEvent(type, hdrCell, mouseEv.GetX(), mouseEv.GetY(),
			mouseEv.ControlDown(),
			mouseEv.ShiftDown(),
			mouseEv.AltDown(),
			mouseEv.MetaDown());
}

bool wxAdvTable::SendHdrCellEvent(const wxEventType type, wxAdvHdrCell *hdrCell, wxCoord x, wxCoord y,
		bool controlDown, bool shiftDown, bool altDown,	bool metaDown)
{
	wxAdvHdrCellEvent ev(GetId(), type, this,
			hdrCell,
			x, y,
			controlDown,
			shiftDown,
			altDown,
			metaDown);

	GetEventHandler()->ProcessEvent(ev);
	if (!ev.IsAllowed()) {
		return false;
	}
	return true;
}

//
// Event handlers.
//

void wxAdvTable::OnPaint(wxPaintEvent &WXUNUSED(ev))
{
	wxAutoBufferedPaintDC dc(this);

	// draw window background
	dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(GetBackgroundColour()));
	dc.SetPen(*wxThePenList->FindOrCreatePen(GetBackgroundColour(), 1, wxSOLID));
	dc.DrawRectangle(GetClientRect());

	if (!m_tableCreated) {
		return ;
	}

	// draw row hdrCells
	if (m_showRows && m_backBitmapRows.IsOk()) {
		wxRect rcRows = CalcRowsAreaRect();
		dc.DrawBitmap(m_backBitmapRows, rcRows.x, rcRows.y);
	}

	// draw column hdrCells
	if (m_showCols && m_backBitmapCols.IsOk()) {
		wxRect rcCols = CalcColsAreaRect();
		dc.DrawBitmap(m_backBitmapCols, rcCols.x, rcCols.y);
	}

	// draw table corner
	if (m_showCorner) {
		DrawHdrCell(dc, &m_cornerCell);
	}

	// draw data
	if (m_backBitmap.IsOk()) {
		wxRect rcTable = CalcTableRect();
		dc.DrawBitmap(m_backBitmap, rcTable.x, rcTable.y);
	}
}

void wxAdvTable::OnMouseEvents(wxMouseEvent &ev)
{
	wxPoint pt = ev.GetPosition();

	size_t row, col;
	wxAdvHdrCell *hdrCell = GetHdrCellAt(pt);

	if (hdrCell != NULL) {
		// mouse event on header cell
		HandleHdrCellMouseEvent(ev, hdrCell);
	}
	else {
		if (ev.Moving()) {
			SetCursor(wxNullCursor);
		}

		if (GetCellAt(pt, row, col)) {
			// mouse event on data cell
			HandleCellMouseEvent(ev, row, col);
		}
	}

	if (ev.ButtonUp()) {
		// user released mouse button - so resize finished
		m_resizingHdrCell = NULL;
		SetCursor(wxNullCursor);
	}

	// resize hdrCell
	if (ev.Dragging()) {
		if (m_resizingHdrCell != NULL) {
			ResizeHdrCell(ev, m_resizingHdrCell);
		}
		else if (m_pressedHdrCell != NULL) {
			// TODO drag hdrCell move
		}
	}

	if (ev.Leaving() || hdrCell == NULL) {
		if (m_currentHdrCell != NULL) {
			SetCurrentHdrCell(NULL);
			SetPressedHdrCell(NULL);
		}
	}

	if (ev.ButtonUp() || ev.Leaving()) {
		SetPressedHdrCell(NULL);
	}

	if (ev.ButtonDown()) {
		SetFocus();
	}

	ev.Skip();
}

bool wxAdvTable::IsHdrCellResizeEvent(const wxMouseEvent &ev, wxAdvHdrCell *cell)
{
	const int resizeEdgeDistance = 5;

	if (cell->m_isRow) {
		if (!m_resizeAllRowsAllowed && !cell->m_resizeAllowed) {
			return false;
		}

		wxPoint pt = ToViewportPosition(ev.GetPosition(), true, false);
		return (pt.y > (cell->m_rc.y + cell->m_rc.height - resizeEdgeDistance));
	}
	else {
		if (!m_resizeAllColsAllowed && !cell->m_resizeAllowed) {
			return false;
		}

		wxPoint pt = ToViewportPosition(ev.GetPosition(), false, true);
		return (pt.x > (cell->m_rc.x + cell->m_rc.width - resizeEdgeDistance));
	}
}

void wxAdvTable::HandleHdrCellMouseEvent(const wxMouseEvent &ev, wxAdvHdrCell *hdrCell)
{
	if (ev.LeftUp()) {
		if (hdrCell == m_pressedHdrCell && m_pressedHdrCell != NULL) {
			if (m_sorter != NULL && CanSortByHdrCell(m_pressedHdrCell)) {
				SetSortingIndex(m_pressedHdrCell->m_decompIndex);

				// fire sort event
				SendHdrCellEvent(wxEVT_ADVTABLE_HDRCELL_SORT, m_pressedHdrCell, ev);
			}
		}

		if (hdrCell == m_pressedHdrCell) {
			SendHdrCellEvent(wxEVT_ADVTABLE_HDRCELL_LEFT_CLICK, hdrCell, ev);
		}

		SetPressedHdrCell(NULL);
	}
	else if (ev.LeftDown()) {
		if (IsHdrCellResizeEvent(ev, hdrCell)) {
			m_resizingHdrCell = hdrCell;
		}
		else {
			SetPressedHdrCell(hdrCell);
		}
	}
	else if (ev.RightUp()) {
		if (hdrCell == m_pressedHdrCell) {
			SendHdrCellEvent(wxEVT_ADVTABLE_HDRCELL_RIGHT_CLICK, hdrCell, ev);
		}
	}
	else if (ev.RightDown()) {
		if (m_pressedHdrCell == NULL) {
			SetPressedHdrCell(hdrCell);
		}
	}
	else if (ev.LeftDClick()) {
		SendHdrCellEvent(wxEVT_ADVTABLE_HDRCELL_LEFT_DCLICK, hdrCell, ev);
	}
	else if (ev.RightDClick()) {
		SendHdrCellEvent(wxEVT_ADVTABLE_HDRCELL_RIGHT_DCLICK, hdrCell, ev);
	}
	else if (ev.Moving()) {
		if (IsHdrCellResizeEvent(ev, hdrCell)) {
			int cursorId;

			if (hdrCell->m_isRow) {
				cursorId = wxCURSOR_SIZENS;
			}
			else {
				cursorId = wxCURSOR_SIZEWE;
			}
			SetCursor(wxCursor(cursorId)); // XXX: can leak resources?!
			return ;
		}
		else {
			SetCursor(wxNullCursor);
		}
	}

	SetCurrentHdrCell(hdrCell);
}

void wxAdvTable::HandleCellMouseEvent(const wxMouseEvent &ev, size_t row, size_t col)
{
	if (ev.LeftUp()) {
		if (m_pressedCell.Equal(row, col)) {
			if (IsCellEditable(row, col)) {
				wxAdvCellEditor *editor = GetEditorForCell(row, col);

				if (editor != NULL && editor->OneClickActivate()) {
					EditCell(editor, row, col);
				}
			}

			SendEvent(wxEVT_GRID_CELL_LEFT_CLICK, row, col, ev);
		}
	}
	else if (ev.LeftDown()) {
		m_pressedCell.Set(row, col);

		SetFocusedCell(row, col);
		SelectCells(ev, m_pressedCell.Row(), m_pressedCell.Col(), row, col);
	}
	else if (ev.RightUp()) {
		SendEvent(wxEVT_GRID_CELL_RIGHT_CLICK, row, col, ev);
	}
	else if (ev.LeftDClick()) {
		if (IsCellEditable(row, col)) {
			wxAdvCellEditor *editor = GetEditorForCell(row, col);
			if (editor != NULL) {
				EditCell(editor, row, col);
			}
		}

		SendEvent(wxEVT_GRID_CELL_LEFT_DCLICK, row, col, ev);
	}
	else if (ev.RightDClick()) {
		SendEvent(wxEVT_GRID_CELL_RIGHT_DCLICK, row, col, ev);
	}
	else if (ev.Dragging() && ev.LeftIsDown()) {
		SetFocusedCell(row, col);
		SelectCells(ev, m_pressedCell.Row(), m_pressedCell.Col(), row, col);
	}
}

void wxAdvTable::OnKeyDown(wxKeyEvent &ev)
{
	int keyCode = ev.GetKeyCode();

	// cell select/focus shift
	int colShift = 0;
	int rowShift = 0;

	switch (keyCode) {
	case WXK_LEFT:
	case WXK_NUMPAD_LEFT:
		colShift = -1;
		break;
	case WXK_RIGHT:
	case WXK_NUMPAD_RIGHT:
		colShift = 1;
		break;
	case WXK_UP:
	case WXK_NUMPAD_UP:
		rowShift = -1;
		break;
	case WXK_DOWN:
	case WXK_NUMPAD_DOWN:
		rowShift = 1;
		break;
	default:
		if (/*wxIsalnum(keyCode) || */
				keyCode == WXK_RETURN ||
				keyCode == WXK_SPACE ||
				keyCode == WXK_BACK) {
			if (m_focused.IsSet()) {
				size_t row = m_focused.Row();
				size_t col = m_focused.Col();

				if (IsCellEditable(row, col)) {
					wxAdvCellEditor *editor = GetEditorForCell(row, col);
					if (editor != NULL) {
						EditCell(editor, row, col);

						editor->HandleKeyEvent(ev);
					}
				}
			}
		}
		else {
			ev.Skip();
		}
	}

	if (colShift != 0 || rowShift != 0) {
		// selected range
		size_t row1, col1;
		size_t row2, col2;

		// new selected row/column coordinate
		size_t newRow, newCol;

		if (m_selected.IsSet()) {
			row1 = m_selected.Row1();
			col1 = m_selected.Col1();
			row2 = m_selected.Row2();
			col2 = m_selected.Col2();

			if (!m_focused.IsSet()) {
				newRow = IncrRow(row2, rowShift);
				newCol = IncrCol(col2, colShift);
			}
			else {
				newRow = IncrRow(m_focused.Row(), rowShift);
				newCol = IncrCol(m_focused.Col(), colShift);
			}
		}
		else {
			// if no cell selected, start selection from first cell
			row1 = row2 = 0;
			col1 = col2 = 0;
			newRow = 0;
			newCol = 0;
		}

		// update focused cell
		SetFocusedCell(newRow, newCol);

		// scroll to new focused cell
		ScrollToCell(newRow, newCol);

		// update selection
		if (ev.ShiftDown()) {
			SelectCells(ev, row1, col1, newRow, newCol);
		}
		else {
			SelectCells(ev, newRow, newCol, newRow, newCol);
		}
	}
}

void wxAdvTable::OnChar(wxKeyEvent &ev)
{
	int keyCode = ev.GetKeyCode();

	if (wxIsalnum(keyCode) || keyCode == WXK_RETURN || keyCode == WXK_SPACE) {
		if (m_focused.IsSet()) {
			size_t row = m_focused.Row();
			size_t col = m_focused.Col();

			if (IsCellEditable(row, col)) {
				wxAdvCellEditor *editor = GetEditorForCell(row, col);
				if (editor != NULL) {
					EditCell(editor, row, col);

					editor->HandleKeyEvent(ev);
				}
			}
		}
	}
	else {
		ev.Skip();
	}
}

void wxAdvTable::SetFocus()
{
	wxWindow::SetFocus();
}

void wxAdvTable::OnKillFocus(wxFocusEvent &WXUNUSED(ev))
{
	StopEditing();
}

void wxAdvTable::OnSize(wxSizeEvent &ev)
{
	HandleOnSize(ev); // make scrollbars adjustment before redrawing

	ResizeBackBitmaps();
}

void wxAdvTable::OnScrollWin(wxScrollWinEvent &ev)
{
	int inc = CalcScrollInc(ev);
	if (inc == 0) {
		ev.Skip();
		return ;
	}

	int x, y;
	GetViewStart(&x, &y);

	switch (ev.GetOrientation()) {
	case wxHORIZONTAL:
		x += inc;
		y = -1;
		break;
	case wxVERTICAL:
		x = -1;
		y += inc;
		break;
	}

	Scroll(x, y);

	RedrawAll();
}

void wxAdvTable::ScrollWindow(int WXUNUSED(dx), int WXUNUSED(dy), const wxRect* WXUNUSED(rect))
{
}

void wxAdvTable::CellChanged(size_t row, size_t col)
{
	if (IsSorting()) {
		UpdateSortOrder();
	}
	else {
		wxRect rc = GetCellRect(row, col);
		RedrawRange(row, col, row, col);
	}
}

void wxAdvTable::TableChanged()
{
	if (IsSorting()) {
		UpdateSortOrder();
	}
	else {
		RedrawAll();
	}
}
