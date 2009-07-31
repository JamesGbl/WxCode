/////////////////////////////////////////////////////////////////////////////
// Name:        wxAdvTable.cpp
// Purpose:     wxAdvTable classes implementation
// Author:      Moskvichev Andrey V.
// Created:     29/10/2008
// RCS-ID:      $Id: wxAdvTable.cpp,v 1.4 2008/10/29 16:42:58 frm Exp $
// Copyright:   (c) 2008-2009 Moskvichev A.V.
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxadvtable.h"

#include <wx/renderer.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/colordlg.h>

#include <wx/arrimpl.cpp>
WX_DEFINE_EXPORTED_OBJARRAY(wxAdvHdrCellArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxAdvHdrCellPtrArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxAdvRangeArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxCoordArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxIndexArray);
WX_DEFINE_EXPORTED_OBJARRAY(wxArrayArrayString);

size_t wxAdvHdrCell::GetDecompCellCount(wxAdvHdrCell *cells, size_t numCells)
{
	size_t count = 0;

	for (size_t n = 0; n < numCells; n++) {
		count += cells[n].GetDecompCellCount();
	}
	return count;
}

//
// Drawing helper functions.
//

/**
 * Draws bevel.
 * @param dc device context
 * @param rc rectangle where to draw
 * @param bgColour bevel background colour
 * @param colour1 colour of left and top edges
 * @param colour2 colour of right and bottom edges
 * @param edgeWidth edgeWidth
 */
void DrawBevel(wxDC &dc, const wxRect &rc, wxColour bgColour, wxColour colour1, wxColour colour2, int edgeWidth)
{
	// draw background
	dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(bgColour));
	dc.SetPen(*wxThePenList->FindOrCreatePen(bgColour, 1, wxSOLID));
	dc.DrawRectangle(rc);

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

double wxAdvTableDataModel::GetCellValueDouble(size_t WXUNUSED(row), size_t WXUNUSED(col))
{
	return 0;
}

//
// wxAdvStringTableDataModel
//
wxAdvStringTableDataModel::wxAdvStringTableDataModel(size_t numRows, size_t numCols, bool readOnly)
{
	m_readOnly = readOnly;

	wxArrayString row;
	row.Add(wxEmptyString, numCols);
	m_data.Add(row, numRows);
}

wxAdvStringTableDataModel::~wxAdvStringTableDataModel()
{
}

wxString wxAdvStringTableDataModel::GetCellValue(size_t row, size_t col)
{
	wxCHECK(row < m_data.Count(), wxEmptyString);
	wxCHECK(col < m_data[row].Count(), wxEmptyString);
	return m_data[row][col];
}

bool wxAdvStringTableDataModel::IsCellEditable(size_t WXUNUSED(row), size_t WXUNUSED(col))
{
	return !m_readOnly;
}

int wxAdvStringTableDataModel::GetCellFormat(size_t WXUNUSED(row), size_t WXUNUSED(col))
{
	return wxStringFormat;
}

bool wxAdvStringTableDataModel::SetCellValue(size_t row, size_t col, wxString value)
{
	if (m_readOnly) {
		return false;
	}

	wxCHECK(row < m_data.Count(), false);
	wxCHECK(col < m_data[row].Count(), false);

	m_data[row][col] = value;
	FireCellChanged(row, col);
	return true;
}

wxAdvCellAttribute *wxAdvStringTableDataModel::GetCellAttribute(size_t WXUNUSED(row), size_t WXUNUSED(col))
{
	return NULL;
}

double wxAdvStringTableDataModel::GetCellValueDouble(size_t WXUNUSED(row), size_t WXUNUSED(col))
{
	return 0;
}

//
// wxAdvFilterTableDataModel
//
wxAdvFilterTableDataModel::wxAdvFilterTableDataModel(wxAdvTableDataModel *model, size_t numRows, size_t numCols)
{
	m_undelayingModel = model;
	m_undelayingModel->AddObserver(this);

	wxArrayString row;
	row.Add(wxEmptyString, numCols);
	m_data.Add(row, numRows);

	m_needUpdate = true;
	//UpdateValues();
}

wxAdvFilterTableDataModel::~wxAdvFilterTableDataModel()
{
	delete m_undelayingModel;
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
	return m_undelayingModel->GetCellFormat(row, col);
}

bool wxAdvFilterTableDataModel::IsCellEditable(size_t row, size_t col)
{
	return m_undelayingModel->IsCellEditable(row, col);
}

void wxAdvFilterTableDataModel::CellChanged(size_t row, size_t col)
{
	m_data[row][col] = m_undelayingModel->GetCellValue(row, col);
	FireCellChanged(row, col);
}

bool wxAdvFilterTableDataModel::SetCellValue(size_t row, size_t col, wxString value)
{
	return m_undelayingModel->SetCellValue(row, col, value);
}

wxAdvCellAttribute *wxAdvFilterTableDataModel::GetCellAttribute(size_t row, size_t col)
{
	return m_undelayingModel->GetCellAttribute(row, col);
}

double wxAdvFilterTableDataModel::GetCellValueDouble(size_t row, size_t col)
{
	return m_undelayingModel->GetCellValueDouble(row, col);
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
			m_data[row][col] = m_undelayingModel->GetCellValue(row, col);
		}
	}
	m_needUpdate = false;
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
}

wxAdvDefaultHdrCellRenderer::~wxAdvDefaultHdrCellRenderer()
{
}

void wxAdvDefaultHdrCellRenderer::Draw(wxAdvTable *table, wxDC &dc, wxAdvHdrCell *hdrCell, bool selected, bool pressed, int sortDirection)
{
	wxColour colour1, colour2, bgColour, textColour;
	textColour = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT);
	bgColour = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);

	if (pressed) {
		colour1 = wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW);
		colour2 = wxSystemSettings::GetColour(wxSYS_COLOUR_3DHIGHLIGHT);
	}
	/*else if (selected) {
		colour1 = wxSystemSettings::GetColour(wxSYS_COLOUR_3DHIGHLIGHT);
		colour2 = wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW);
	}*/
	else {
		colour1 = wxSystemSettings::GetColour(wxSYS_COLOUR_3DHIGHLIGHT);
		colour2 = wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW);
	}

	const int edgeWidth = 4;

	DrawBevel(dc, hdrCell->m_rc, bgColour, colour1, colour2, edgeWidth);

	dc.SetFont(*wxNORMAL_FONT);
	dc.SetTextForeground(textColour);

	wxRect rc = hdrCell->m_rc;
	rc.Deflate(edgeWidth, edgeWidth);

	DrawMultilineText(dc, hdrCell->m_label, rc,
			hdrCell->m_alignHorizontal | hdrCell->m_alignVertical,
			hdrCell->IsVerticalText());

	// TODO draw sort arrow
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
// wxAdvBoolCellRenderer
//
wxAdvBoolCellRenderer::wxAdvBoolCellRenderer()
{
}

wxAdvBoolCellRenderer::~wxAdvBoolCellRenderer()
{
}

void wxAdvBoolCellRenderer::Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool WXUNUSED(selected), bool WXUNUSED(focused), wxAdvCellAttribute *WXUNUSED(attr))
{
	int flags = 0;
	if (value.Cmp(boolTrue) == 0) {
		flags |= wxCONTROL_CHECKED;
	}

	wxRendererNative::Get().DrawCheckBox(table, dc, rc, flags);
}

wxAdvStringCellRenderer::wxAdvStringCellRenderer()
{
}

wxAdvStringCellRenderer::~wxAdvStringCellRenderer()
{
}

void wxAdvStringCellRenderer::Draw(wxAdvTable *WXUNUSED(table), wxDC &dc, wxRect rc, wxString value, bool WXUNUSED(selected), bool WXUNUSED(focused), wxAdvCellAttribute *attr)
{
	wxDCClipper clip(dc, rc);

	if (attr->Brush().GetStyle() !=	wxTRANSPARENT) {
		// blend brush colors.
		wxColour clr1 = dc.GetBrush().GetColour();
		wxColour clr2 = attr->Brush().GetColour();

		wxColour blendedClr((clr1.Red() + clr2.Red()) / 2,
			(clr1.Green() + clr2.Green()) / 2,
			(clr1.Blue() + clr2.Blue()) / 2);

		wxBrush brush = *wxTheBrushList->FindOrCreateBrush(blendedClr, attr->Brush().GetStyle());
		dc.SetBrush(brush);
	}

	dc.DrawRectangle(rc);
	dc.SetFont(attr->Font());
	dc.SetTextForeground(attr->TextColour());
	dc.DrawLabel(value, rc, attr->Alignment());
}

wxAdvColourCellRenderer::wxAdvColourCellRenderer()
{
}

wxAdvColourCellRenderer::~wxAdvColourCellRenderer()
{
}

void wxAdvColourCellRenderer::Draw(wxAdvTable *table, wxDC &dc, wxRect rc, wxString value, bool WXUNUSED(selected), bool WXUNUSED(focused), wxAdvCellAttribute *WXUNUSED(attr))
{
	wxColour colour(value);

	dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(colour));
	dc.DrawRectangle(rc);
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

void wxAdvCellEditor::HandleKeyEvent(wxKeyEvent &ev)
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
	wxCHECK_MSG(m_cell.IsSet(), UNDEF_SIZE, wxT("wxAdvCellEditor::GetValue: m_cell.GetRow()"));

	return m_cell.Row();
}

size_t wxAdvCellEditor::GetCol()
{
	wxCHECK_MSG(m_cell.IsSet(), UNDEF_SIZE, wxT("wxAdvCellEditor::GetValue: m_cell.GetCol()"));

	return m_cell.Col();
}

wxRect wxAdvCellEditor::GetCellRect()
{
	wxCHECK_MSG(m_cell.IsSet(), wxRect(0, 0, 0, 0), wxT("wxAdvCellEditor::GetValue: m_cell.GetCol()"));

	return m_table->GetCellRect(m_cell.Row(), m_cell.Col());
}

wxString wxAdvCellEditor::GetValue()
{
	wxCHECK_MSG(m_cell.IsSet(), wxEmptyString, wxT("wxAdvCellEditor::GetValue: m_cell.IsSet()"));

	return m_table->GetModel()->GetCellValue(m_cell.Row(), m_cell.Col());
}

void wxAdvCellEditor::SetNewValue(wxString newValue)
{
	wxCHECK_RET(m_cell.IsSet(), wxT("wxAdvCellEditor::SetNewValue: m_cell.IsSet()"));
	wxCHECK_RET(m_table != NULL, wxT("wxAdvCellEditor::SetNewValue: m_table != NULL"));

	m_table->GetModel()->SetCellValue(m_cell.Row(), m_cell.Col(), newValue);
}

void wxAdvCellEditor::EndEditing()
{
	if (m_active) {
		wxCHECK_RET(m_table != NULL, wxT("wxAdvCellEditor::EndEditing"));
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
	//EVT_KEY_DOWN(wxAdvStringCellEditor::OnTextKeydown)
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
	m_textCtrl->Close();
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
	m_textCtrl->Show();

	m_textCtrl->SetFocus();

	m_escPressed = false;
}

void wxAdvStringCellEditor::DoDeactivate()
{
	m_textCtrl->Show(false);

	if (!m_escPressed) {
		SetNewValue(m_textCtrl->GetValue());
	}
}

void wxAdvStringCellEditor::OnTextEnter(wxCommandEvent &ev)
{
	EndEditing();
}

void wxAdvStringCellEditor::OnKillFocus(wxFocusEvent &WXUNUSED(ev))
{
	EndEditing();
}

void wxAdvStringCellEditor::OnTextKeydown(wxKeyEvent &ev)
{
	if (!IsActive()) {
		// bugfix: editor is not active, but keydown events
		// go here, so we forward them to table
		GetTable()->ProcessEvent(ev);
		return ;
	}

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
BEGIN_EVENT_TABLE(wxAdvBoolCellEditor, wxEvtHandler)
	EVT_CHECKBOX(wxID_ANY, wxAdvBoolCellEditor::OnCheckbox)
	//	EVT_KILL_FOCUS(wxAdvBoolCellEditor::OnKillFocus)
END_EVENT_TABLE()

wxAdvBoolCellEditor::wxAdvBoolCellEditor(wxAdvTable *table)
: wxAdvCellEditor(table)
{
	m_checkBox = new wxCheckBox(table, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
	m_checkBox->Show(false);
	m_checkBox->SetNextHandler(this);
}

wxAdvBoolCellEditor::~wxAdvBoolCellEditor()
{
	m_checkBox->Close();
}

bool wxAdvBoolCellEditor::OneClickActivate()
{
	return true;
}

void wxAdvBoolCellEditor::DoActivate()
{
	wxRect rc = GetCellRect();
	GetTable()->CalcScrolledPosition(rc.x, rc.y, &rc.x, &rc.y);

	bool checked = ! (GetValue().Cmp(boolTrue) == 0); // invert value on activation
	m_checkBox->SetValue(checked);
	SetNewValue((checked) ? boolTrue : boolFalse);

	// rearrange checkbox to cell
	m_checkBox->SetSize(rc.x, rc.y, rc.width, rc.height);
	m_checkBox->Show();

	m_checkBox->SetFocus();
}

void wxAdvBoolCellEditor::DoDeactivate()
{
	m_checkBox->Show(false);

	bool checked = m_checkBox->IsChecked();
	SetNewValue((checked) ? boolTrue : boolFalse);
}

void wxAdvBoolCellEditor::OnKillFocus(wxFocusEvent &WXUNUSED(ev))
{
	EndEditing();
}

void wxAdvBoolCellEditor::OnCheckbox(wxCommandEvent &WXUNUSED(ev))
{
	bool checked = m_checkBox->IsChecked();
	SetNewValue((checked) ? boolTrue : boolFalse);
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

//
// wxAdvCellAttribute
//
wxAdvCellAttribute::wxAdvCellAttribute()
{
	// default values
	m_font = *wxNORMAL_FONT;
	m_alignment = wxALIGN_CENTER | wxALIGN_RIGHT;
	m_brush = *wxTheBrushList->FindOrCreateBrush(*wxBLACK, wxTRANSPARENT);
}

wxAdvCellAttribute::~wxAdvCellAttribute()
{
}


//
// Sorters
//
class SortHelper // XXX dirty hack! used to implement sorters
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
		int i = 0;
		i++;
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

	double value1 = dataModel->GetCellValueDouble(row1, col1);
	double value2 = dataModel->GetCellValueDouble(row2, col2);

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
// wxAdvTable
//

BEGIN_EVENT_TABLE(wxAdvTable, wxScrolledWindow)
	EVT_PAINT(wxAdvTable::OnPaint)
	EVT_MOUSE_EVENTS(wxAdvTable::OnMouseEvents)
	EVT_KEY_DOWN(wxAdvTable::OnKeyDown)
	EVT_CHAR(wxAdvTable::OnChar)
	EVT_KILL_FOCUS(wxAdvTable::OnKillFocus)
	EVT_SIZE(wxAdvTable::OnSize)
	EVT_SCROLLWIN(wxAdvTable::OnScrollWin)
END_EVENT_TABLE()

IMPLEMENT_CLASS(wxAdvTable, wxScrolledWindow)

wxAdvTable::wxAdvTable(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &m_size)
: wxScrolledWindow(parent, id, pos, m_size, wxHSCROLL | wxVSCROLL | wxFULL_REPAINT_ON_RESIZE | wxWANTS_CHARS)
{
	m_tableCreated = false;
	m_model = NULL;
	m_defaultRenderer = new wxAdvStringCellRenderer();
	m_defaultHdrRenderer = new wxAdvDefaultHdrCellRenderer();

	m_sorter = NULL;
	m_sortMode = SortRows;
	m_highlightMode = HighlightNone;
	m_pressedHdrCell = NULL;
	m_currentHdrCell = NULL;
	m_sortDirection = SortDirAscending;

	m_activeEditor = NULL;

	m_selectMode = SelectBlock;

	m_showRows = true;
	m_showCols = true;
	m_showCorner = true;

	// Graphic objects initialization
	m_bgBrush = *wxWHITE_BRUSH;
	m_selectedBgBrush = *wxTheBrushList->FindOrCreateBrush(wxColour(220, 220, 220));
	m_gridPen = *wxThePenList->FindOrCreatePen(wxColour(200, 200, 200), 1, wxSOLID);
	m_focusedPen = *wxThePenList->FindOrCreatePen(wxColour(80, 80, 250), 1, wxSOLID);
	m_focusedBgBrush = *wxTheBrushList->FindOrCreateBrush(wxColour(180, 180, 250));
	m_highlightedBgBrush = *wxTheBrushList->FindOrCreateBrush(wxColour(225, 225, 250));

	ClearSelection();

	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	SetScrollRate(1, 1);
	EnableScrolling(false, false);
}

wxAdvTable::~wxAdvTable()
{
	wxAdvCellRendererMap::iterator it;

	for (it = m_renderers.begin(); it != m_renderers.end(); it++) {
		delete it->second;
	}

	SAFE_DELETE(m_model);
	SAFE_DELETE(m_defaultRenderer);

	SAFE_DELETE(m_defaultHdrRenderer);
}

//
// Table creation functions.
//

void wxAdvTable::Create(wxAdvHdrCell *rows, size_t numRows, wxAdvHdrCell *cols, size_t numCols, const wxString &cornerLabel, wxAdvTableDataModel *_model)
{
	if (m_tableCreated) {
		DestroyTable();
	}

	m_cornerCell.Label(cornerLabel);

	AddHdrCells(m_rows, rows, numRows, true);
	AddHdrCells(m_cols, cols, numCols, false);

	// recalculate entire geometry
	CalcTableGeometry();

	// assign new model for table data
	SAFE_REPLACE(m_model, _model);
	m_model->AddObserver(this);

	// reset sorting and selection
	m_sortingIndex = UNDEF_SIZE;
	ClearSelection();

	m_tableCreated = true;
	ResizeBackBitmaps();
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

	SAFE_DELETE(m_model);

	ClearSelection();
}

void wxAdvTable::AddRows(wxAdvHdrCell *rows, size_t numRows)
{
	wxCHECK_RET(m_tableCreated, wxT("wxAdvTable::AddRows: Table is not created"));

	AddHdrCells(m_rows, rows, numRows, true);
	CalcTableGeometry();
}

void wxAdvTable::AddCols(wxAdvHdrCell *cols, size_t numCols)
{
	wxCHECK_RET(m_tableCreated, wxT("wxAdvTable::AddCols: Table is not created"));

	AddHdrCells(m_cols, cols, numCols, false);
	CalcTableGeometry();
}

//
// Table geometry functions.
//
void wxAdvTable::CalcTableGeometry()
{
	size_t numRowLayers = GetLayerCount(m_rows);
	size_t numColLayers = GetLayerCount(m_cols);

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
	FOREACH_HDRCELL(n, m_rows) {
		FitForSubCells(dc, m_rows[n], true);
	}
	FOREACH_HDRCELL(n, m_cols) {
		FitForSubCells(dc, m_cols[n], false);
	}

	// calculate layer sizes from row/column maximum heights/widths
	FOREACH_HDRCELL(n, m_rows) {
		CalcLayerSizes(m_rows[n], m_rowLayerWidths, true, 0);
	}
	FOREACH_HDRCELL(n, m_cols) {
		CalcLayerSizes(m_cols[n], m_colLayerHeights, false, 0);
	}

	// set row/columns heights/widths from layer sizes
	FOREACH_HDRCELL(n, m_rows) {
		UpdateHeaderSizes(m_rows[n], m_rowLayerWidths, true, 0);
	}
	FOREACH_HDRCELL(n, m_cols) {
		UpdateHeaderSizes(m_cols[n], m_colLayerHeights, false, 0);
	}

	// setup positions for all rows and columns
	wxCoord totalColHeight = SumLayerSizes(m_colLayerHeights);
	wxCoord totalRowWidth = SumLayerSizes(m_rowLayerWidths);

	wxCoord x0 = 0;
	wxCoord y0 = 0;

	x0 = 0;
	y0 = (m_showCols || m_showCorner) ? totalColHeight : 0;
	FOREACH_HDRCELL(n, m_rows) {
		UpdateHeaderPositions(m_rows[n], x0, y0, m_rowLayerWidths, 0, true);
		y0 += m_rows[n].m_rc.height;
	}

	x0 = (m_showRows || m_showCorner) ? totalRowWidth : 0;
	y0 = 0;
	FOREACH_HDRCELL(n, m_cols) {
		UpdateHeaderPositions(m_cols[n], x0, y0, m_colLayerHeights, 0, false);
		x0 += m_cols[n].m_rc.width;
	}

	m_cornerCell.m_rc = wxRect(0, 0, totalRowWidth, totalColHeight);

	wxCoord virtWidth = CalcTotalColsWidth();
	wxCoord virtHeight = CalcTotalRowsHeight();

	if (m_showRows || m_showCorner) {
		virtWidth += totalRowWidth;
	}
	if (m_showCols || m_showCorner) {
		virtHeight += totalColHeight;
	}

	//SetScrollbars(1, 1, virtWidth, virtHeight);
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
		hdrCell.m_isDecomp = true;
		hdrCell.m_index = decompHdrCells.Count();
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

void wxAdvTable::AddHdrCells(wxAdvHdrCellArray &arr, wxAdvHdrCell *hdrCells, size_t numCells, bool isRows)
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

		arr.Add(*cell);
	}
}

//
// Sorting, selection, highlight, etc functions.
//
void wxAdvTable::SetSorter(wxAdvTableSorter *sorter)
{
	SAFE_REPLACE(m_sorter, sorter);
	UpdateSortOrder();
}

void wxAdvTable::SetSortMode(SortMode sortMode)
{
	if (m_sortMode != sortMode) {
		m_sortMode = sortMode;
		UpdateSortOrder();
	}
}

void wxAdvTable::UpdateSortOrder()
{
	if (m_sorter == NULL) {
		return ;
	}

	wxSortHelperArray helpers(SorterFunc);

	m_sortOrder.Empty();

	// XXX need normal sorting method, this code is very ugly.
	if (m_sortMode == SortRows) {
		if (m_sortingIndex > MaxCol()) {
			return ;
		}

		size_t rowCount = m_decompRows.Count();
		helpers.Alloc(rowCount);

		for (size_t row = 0; row < rowCount; row++) {
			helpers.Add(new SortHelper(this, m_sorter, row, m_sortingIndex, true, m_sortDirection));
		}
	}
	else if (m_sortMode == SortCols) {
		if (m_sortingIndex >= MaxRow()) {
			return ;
		}

		size_t colCount = m_decompCols.Count();
		helpers.Alloc(colCount);

		for (size_t col = 0; col < colCount; col++) {
			helpers.Add(new SortHelper(this, m_sorter, m_sortingIndex, col, false, m_sortDirection));
		}
	}
	else {
		return ;
	}

	m_sortOrder.Alloc(helpers.Count());

	for (size_t n = 0; n < helpers.Count(); n++) {
		m_sortOrder.Add(helpers[n]->Index());
	}

	// destroy helpers
	for (size_t n = 0; n < helpers.Count(); n++) {
		delete helpers[n];
	}

	RedrawAll();
}

void wxAdvTable::SetSortingIndex(size_t sortingIndex, bool invert)
{
	if (sortingIndex != UNDEF_SIZE) {
		if (m_sortingIndex == sortingIndex && invert) {
			m_sortDirection = (m_sortDirection == SortDirAscending) ?
					SortDirDescending : SortDirAscending;
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

	if (m_sortingIndex == hdrCell->m_index) {
		if ((hdrCell->m_isRow && m_selectMode == SelectCols)
			|| (!hdrCell->m_isRow && m_selectMode == SelectRows)) {
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
	wxString value = m_model->GetCellValue(row, col);
	wxAdvCellAttribute *attr = m_model->GetCellAttribute(row, col);
	if (attr == NULL) {
		attr = &m_defaultCellAttribute;
	}

	dc.DrawRectangle(rc);
	GetRendererForCell(row, col)->Draw(this, dc, rc, value, selected, focused, attr);
}

wxPoint wxAdvTable::ToViewportPosition(wxPoint &pt, bool transVert, bool transHoriz)
{
	int dx, dy;
	int xunit, yunit;

	GetViewStart(&dx, &dy);
	GetScrollPixelsPerUnit(&xunit, &yunit);

	int tx = (transHoriz) ? (dx * xunit) : 0;
	int ty = (transVert) ? (dy * yunit) : 0;

	return wxPoint(pt.x + tx, pt.y + ty);
}

bool wxAdvTable::GetCellAt(wxPoint pt, size_t &cellRow, size_t &cellCol)
{
	pt = ToViewportPosition(pt);

	FOREACH_HDRCELL(row, m_decompRows) {
		wxAdvHdrCell *rowCell = m_decompRows[row];

		if (pt.y > rowCell->m_rc.y && pt.y < (rowCell->m_rc.y + rowCell->m_rc.height)) {

			FOREACH_HDRCELL(col, m_decompCols) {
				wxAdvHdrCell *colCell = m_decompCols[col];

				if (pt.x > colCell->m_rc.x && pt.x < (colCell->m_rc.x + colCell->m_rc.width)) {
					cellRow = row;
					cellCol = col;
					return true;
				}
			}
		}
	}
	return false;
}

wxAdvHdrCell *wxAdvTable::GetHdrCellAt(wxPoint pt)
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

bool wxAdvTable::IsCellEditable(size_t row, size_t col)
{
	ToModelCellCoord(row, col, row, col);
	return m_model->IsCellEditable(row, col);
}


void wxAdvTable::ToModelCellCoord(size_t row, size_t col, size_t &modelRow, size_t &modelCol)
{
	if (m_sorter == NULL || m_sortingIndex == UNDEF_SIZE) {
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
	else {
		modelRow = row;
		modelCol = col;
	}
}

void wxAdvTable::ToCellCoord(size_t row, size_t col, size_t &tableRow, size_t &tableCol)
{
	if (m_sorter == NULL || m_sortingIndex == UNDEF_SIZE) {
		tableRow = row;
		tableCol = col;
	}
	else if (m_sortMode == SortRows && m_sortingIndex < m_sortOrder.Count()) {
		for (size_t n = 0; n < m_sortOrder.Count(); n++) {
			if (m_sortOrder[n] == row) {
				tableRow = n;
				break;
			}
		}

		tableCol = col;
	}
	else if (m_sortMode == SortCols && m_sortingIndex < m_sortOrder.Count()) {
		tableRow = row;

		for (size_t n = 0; n < m_sortOrder.Count(); n++) {
			if (m_sortOrder[n] == row) {
				tableCol = n;
				break;
			}
		}
	}
	else {
		tableRow = row;
		tableCol = col;
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
	SAFE_REPLACE(m_defaultRenderer, renderer);
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

void wxAdvTable::GetRangeRect(wxRect &rc, size_t row1, size_t col1, size_t row2, size_t col2)
{
	wxRect rcCell1;
	wxRect rcCell2;

	GetCellRect(rcCell1, row1, col1);
	GetCellRect(rcCell2, row2, col2);

	rc.x = rcCell1.x;
	rc.y = rcCell1.y;

	rc.width = rcCell2.x - rcCell1.x + rcCell2.width;
	rc.height = rcCell2.y - rcCell1.y + rcCell2.height;
}

//
// Graphical objects functions.
//

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

void wxAdvTable::SelectCells(wxMouseEvent &ev, size_t row1, size_t col1, size_t row2, size_t col2)
{
	SelectCells(row1, col1, row2, col2,
			ev.ControlDown(),
			ev.ShiftDown(),
			ev.AltDown(),
			ev.MetaDown());
}

void wxAdvTable::SelectCells(wxKeyEvent &ev, size_t row1, size_t col1, size_t row2, size_t col2)
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

	SendRangeEvent(wxEVT_GRID_RANGE_SELECT,
			m_selected.TopRow(), m_selected.LeftCol(),
			m_selected.BottomRow(), m_selected.RightCol(),
			controlDown, shiftDown, altDown, metaDown);

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
		m_activeEditor->Deactivate();
		m_activeEditor = NULL;

		SetFocus();
	}
}


//
// Redraw functions
//

void wxAdvTable::ResizeBackBitmaps()
{
	if (!m_tableCreated) {
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

		m_backBitmapRows.Create(rowsWidth, wxMin(rowsHeight, size.GetHeight()));
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

		m_backBitmapCols.Create(wxMin(colsWidth, size.GetWidth()), colsHeight);
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

	m_backBitmap.Create(wxMin(colsWidth, dataSize.GetWidth()), wxMin(rowsHeight, dataSize.GetHeight()));

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

	int xViewStart, yViewStart;
	CalcUnscrolledPosition(0, 0, &xViewStart, &yViewStart);

	row1 = wxMax(row1, visibleRange.TopRow());
	col1 = wxMax(col1, visibleRange.LeftCol());
	row2 = wxMin(row2, visibleRange.BottomRow());
	col2 = wxMin(col2, visibleRange.RightCol());

	wxMemoryDC dc(m_backBitmap);

	wxRect rcData = CalcTableRect();
	dc.SetDeviceOrigin(-xViewStart -rcData.x, -yViewStart -rcData.y);

	for (size_t row = row1; row <= row2; row++) {
		for (size_t col = col1; col <= col2; col++) {
			DrawCell(dc, row, col);
		}
	}

	wxRect rc;
	GetRangeRect(rc, row1, col1, row2, col2);
	rc.x -= xViewStart;
	rc.y -= yViewStart;

	RefreshRect(rc);
}

void wxAdvTable::RedrawAll()
{
	if (!m_tableCreated) {
		return ;
	}

	int xViewStart, yViewStart;
	CalcUnscrolledPosition(0, 0, &xViewStart, &yViewStart);

	wxRect rcData = CalcTableRect();

	// draw table
	wxMemoryDC dataDc(m_backBitmap);
	dataDc.SetDeviceOrigin(-xViewStart -rcData.x, -yViewStart -rcData.y);
	DrawTable(dataDc);

	// draw rows
	if (m_showRows) {
		wxMemoryDC rowsDc(m_backBitmapRows);
		rowsDc.SetDeviceOrigin(0, -yViewStart -rcData.y);
		DrawHdrCells(rowsDc, m_rows);
	}

	// draw columns
	if (m_showCols) {
		wxMemoryDC colsDc(m_backBitmapCols);
		colsDc.SetDeviceOrigin(-xViewStart -rcData.x, 0);
		DrawHdrCells(colsDc, m_cols);
	}

	Refresh();
}

void wxAdvTable::RedrawHdrCell(wxAdvHdrCell *cell)
{
	wxRect rc = cell->m_rc;

	wxRect rcData = CalcTableRect();

	int xViewStart, yViewStart;
	CalcUnscrolledPosition(0, 0, &xViewStart, &yViewStart);

	if (cell->m_isRow) {
		wxMemoryDC dc(m_backBitmapRows);
		dc.SetDeviceOrigin(0, -yViewStart -rcData.y);

		DrawHdrCell(dc, cell);

		rc.y -= yViewStart;
	}
	else {
		wxMemoryDC dc(m_backBitmapCols);
		dc.SetDeviceOrigin(-xViewStart -rcData.x, 0);

		DrawHdrCell(dc, cell);

		rc.x -= xViewStart;
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

	int xViewStart, yViewStart;
	CalcUnscrolledPosition(0, 0, &xViewStart, &yViewStart);

	wxRect tableRc = CalcTableRect();
	wxCoord x0 = tableRc.x + xViewStart;
	wxCoord y0 = tableRc.y + yViewStart;
	wxCoord x1 = tableRc.x + m_backBitmap.GetWidth() + xViewStart;
	wxCoord y1 = tableRc.y + m_backBitmap.GetHeight() + yViewStart;

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

	for (size_t row = row1; row < m_decompRows.Count(); row++) {
		wxAdvHdrCell *rowCell = m_decompRows[row];

		row2 = row;

		if (y1 > rowCell->m_rc.y &&
			y1 <= (rowCell->m_rc.y + rowCell->m_rc.height)) {
				break;
		}
	}
	for (size_t col = col1; col < m_decompCols.Count(); col++) {
		wxAdvHdrCell *colCell = m_decompCols[col];

		col2 = col;
		if (x1 > colCell->m_rc.x &&
			x1 <= (colCell->m_rc.x + colCell->m_rc.width)) {
				break;
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

//
// Event send functions.
//

void wxAdvTable::SendEvent(const wxEventType type, size_t row, size_t col, wxMouseEvent &mouseEv)
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
}

void wxAdvTable::SendRangeEvent(const wxEventType type, size_t row1, size_t col1, size_t row2, size_t col2, wxMouseEvent &mouseEv)
{
	SendRangeEvent(type, row1, col1, row2, col2,
               mouseEv.ControlDown(),
               mouseEv.ShiftDown(),
               mouseEv.AltDown(),
               mouseEv.MetaDown());
}

void wxAdvTable::SendRangeEvent(const wxEventType type, size_t row1, size_t col1, size_t row2, size_t col2,
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
}

void wxAdvTable::EditCell(wxAdvCellEditor *editor, size_t row, size_t col)
{
	// TODO send editor shown/hidden events
	if (m_activeEditor != NULL) {
		m_activeEditor->Deactivate();
	}

	editor->Activate(row, col);
	m_activeEditor = editor;
}

//
// Event handlers
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

	if (m_showRows) {
		wxRect rcRows = CalcRowsAreaRect();
		dc.DrawBitmap(m_backBitmapRows, rcRows.x, rcRows.y);
	}

	if (m_showCols) {
		wxRect rcCols = CalcColsAreaRect();
		dc.DrawBitmap(m_backBitmapCols, rcCols.x, rcCols.y);
	}

	if (m_showCorner) {
		DrawHdrCell(dc, &m_cornerCell);
	}

	// draw data
	wxRect rcTable = CalcTableRect();
	dc.DrawBitmap(m_backBitmap, rcTable.x, rcTable.y);
}

void wxAdvTable::OnMouseEvents(wxMouseEvent &ev)
{
	wxPoint pt = ev.GetPosition();

	size_t row, col;
	wxAdvHdrCell *cell = GetHdrCellAt(pt);

	if (cell != NULL) {
		// mouse event on header cell
		HandleHdrCellMouseEvent(ev, cell);
	}
	else if (GetCellAt(pt, row, col)) {
		// mouse event on data cell
		HandleCellMouseEvent(ev, row, col);
	}

	if (ev.Leaving() || cell == NULL) {
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

void wxAdvTable::HandleHdrCellMouseEvent(wxMouseEvent &ev, wxAdvHdrCell *cell)
{
	if (ev.LeftDown()) {
		SetPressedHdrCell(cell);
	}
	else if (ev.LeftUp()) {
		if (cell == m_pressedHdrCell && m_pressedHdrCell != NULL) {
			if ((m_sorter != NULL && m_pressedHdrCell->m_sortable && m_pressedHdrCell->m_isDecomp) &&
				((!m_pressedHdrCell->m_isRow && m_sortMode == SortRows) ||
				(m_pressedHdrCell->m_isRow && m_sortMode == SortCols))) {
				SetSortingIndex(m_pressedHdrCell->m_index);
			}
		}

		SetPressedHdrCell(NULL);
	}
	else if (ev.LeftDClick()) {
	}

	SetCurrentHdrCell(cell);
}

void wxAdvTable::HandleCellMouseEvent(wxMouseEvent &ev, size_t row, size_t col)
{
	if (ev.LeftDown()) {
		m_pressedCell.Set(row, col);

		SetFocusedCell(row, col);
		SelectCells(ev, m_pressedCell.Row(), m_pressedCell.Col(), row, col);

		//Refresh();
	}
	else if (ev.Dragging() && ev.m_leftDown) {
		SetFocusedCell(row, col);
		SelectCells(ev, m_pressedCell.Row(), m_pressedCell.Col(), row, col);

		//Refresh();
	}
	else if (ev.LeftUp()) {
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
		ev.Skip();
		return ;
#if 0
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
#endif
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

void wxAdvTable::OnKillFocus(wxFocusEvent &WXUNUSED(ev))
{
	StopEditing();
}

void wxAdvTable::OnSize(wxSizeEvent &WXUNUSED(ev))
{
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

void wxAdvTable::ScrollWindow(int dx, int dy, const wxRect* rect)
{
	// do nothing. Turn off physical scrolling, for smooth scroll.
}

void wxAdvTable::CellChanged(size_t row, size_t col)
{
	wxRect rc = GetCellRect(row, col);
	RedrawRange(row, col, row, col);
}

void wxAdvTable::TableChanged()
{
	RedrawAll();
}
