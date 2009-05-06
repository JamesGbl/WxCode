/***************************************************************
 * Name:      wxSTC.cpp
 * Purpose:   Implements class for wxStyledTextControl with custom colors and text highlighters
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wxSTC.h"

wxSTC::wxSTC(wxWindow *parent, wxWindowID id, wxSTCStyle style) : wxStyledTextCtrl(parent, id, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxVSCROLL)
{
    this->style = style;

    wxFont font(style.fontSize, wxMODERN, wxNORMAL, wxNORMAL);
    StyleSetFont(wxSTC_STYLE_DEFAULT, font);
    StyleSetForeground(wxSTC_STYLE_DEFAULT, *wxBLACK);
    StyleSetBackground(wxSTC_STYLE_DEFAULT, style.backColor);
    StyleSetBold(wxSTC_STYLE_DEFAULT, 0);
    StyleSetItalic(wxSTC_STYLE_DEFAULT, 0);
    StyleSetUnderline(wxSTC_STYLE_DEFAULT, 0);
    StyleSetVisible(wxSTC_STYLE_DEFAULT, 1);
    StyleSetCase(wxSTC_STYLE_DEFAULT, wxSTC_CASE_MIXED);

    SetSelForeground(true, style.selTextColor);
    SetSelBackground(true, style.selBackColor);

    InitializeStyle(&defaultStyle);

    SetVisiblePolicy(wxSTC_VISIBLE_STRICT | wxSTC_VISIBLE_SLOP, 1);
    SetXCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);
    SetYCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);

    // markers
    MarkerDefine(wxSTC_MARKNUM_FOLDER,        wxSTC_MARK_DOTDOTDOT, *wxBLACK, *wxBLACK);
    MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN,    wxSTC_MARK_ARROWDOWN, *wxBLACK, *wxBLACK);
    MarkerDefine(wxSTC_MARKNUM_FOLDERSUB,     wxSTC_MARK_EMPTY,     *wxBLACK, *wxBLACK);
    MarkerDefine(wxSTC_MARKNUM_FOLDEREND,     wxSTC_MARK_DOTDOTDOT, *wxBLACK, *wxBLACK);
    MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN, *wxBLACK, *wxBLACK);
    MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY,     *wxBLACK, *wxBLACK);
    MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL,    wxSTC_MARK_EMPTY,     *wxBLACK, *wxBLACK);
}

wxSTC::~wxSTC()
{
    ;
}

void wxSTC::SearchNext(wxString text)
{
    int index, start, end;

    GetSelection(&start, &end);
    SetSelection(end, end);
    SearchAnchor();

    index = wxStyledTextCtrl::SearchNext(0, text);

    if(index!=-1) SetSelection(index, index + text.Length());
}

void wxSTC::SearchPrev(wxString text)
{
    int index, start, end;

    GetSelection(&start, &end);
    SetSelection(start, start);
    SearchAnchor();

    index = wxStyledTextCtrl::SearchPrev(0, text);

    if(index!=-1) SetSelection(index, index + text.Length());
}

void wxSTC::ResetSearch()
{
    SetSelection(0, 0);
}

wxString wxSTC::GetWordUnderCursor()
{
    int start, end;

    GetSelection(&start, &end);
    if(start!=end) return GetSelectedText();

    start = WordStartPosition(start, true);
    end = WordEndPosition(end, true);

    return GetTextRange(start, end);
}

bool wxSTC::InitializeStyle(styleLexer *style)
{
    StyleClearAll();

    SetTabWidth(4);
    SetUseTabs(false);
    SetTabIndents(false);
    SetBackSpaceUnIndents(true);
    SetIndent(0);

    SetViewEOL(false);
    SetIndentationGuides(false);
    SetEdgeColumn(80);
    SetEdgeMode(wxSTC_EDGE_NONE);
    SetViewWhiteSpace(wxSTC_WS_INVISIBLE);
    SetOvertype(false);
    SetReadOnly(false);
    SetWrapMode(wxSTC_WRAP_NONE);

    SetMarginType(m_LineNrID, wxSTC_MARGIN_NUMBER);
    SetMarginWidth(m_LineNrID, 40);
    StyleSetForeground(wxSTC_STYLE_LINENUMBER, this->style.lineNrTextColor);
    StyleSetBackground(wxSTC_STYLE_LINENUMBER, this->style.lineNrBackColor);

    SetMarginType(m_DividerID, wxSTC_MARGIN_SYMBOL);
    SetMarginWidth(m_DividerID, 0);
    SetMarginSensitive(m_DividerID, false);

    SetMarginType(m_FoldingID, wxSTC_MARGIN_SYMBOL);
    SetMarginMask(m_FoldingID, wxSTC_MASK_FOLDERS);
    StyleSetBackground(m_FoldingID, *wxWHITE);
    SetMarginWidth(m_FoldingID, 0);
    SetMarginSensitive(m_FoldingID, false);

    StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColour(128, 128, 128));

    SetLexer(style->lexerID);

    int keywordnr = 0;
    for(unsigned int n = 0; n < style->styleCount; n++)
    {
        StyleSetForeground(n, style->styles[n].textColor);
        StyleSetBackground(n, style->styles[n].backColor);

        StyleSetBold(n, style->styles[n].bold);
        StyleSetUnderline(n, style->styles[n].underline);
        StyleSetItalic(n, style->styles[n].italic);

        if(!style->styles[n].keywords.IsEmpty())
        {
            SetKeyWords(keywordnr, style->styles[n].keywords);
            keywordnr += 1;
        }
    }

    return true;
}

bool wxSTC::LoadFile(const wxString &filename, styleLexer *style)
{
    wxFileName fn(filename, wxPATH_UNIX);

    if(filename.IsEmpty()) return false;
    if(fn.FileExists())
    {
        SetReadOnly(false);
        ClearAll();

        // On Linux loads empty text, if file has non-linux line ends
        //wxStyledTextCtrl::LoadFile(filename);

        // Slow, but works on all plaftorms
        wxFileInputStream fin(fn.GetFullPath());
        wxTextInputStream tin(fin, wxEmptyString);

        wxString text;
        while(fin.CanRead())
        {
            text += tin.ReadLine();
            text += wxT('\n');
        }
        wxStyledTextCtrl::SetText(text);

        EmptyUndoBuffer();

        if(style==NULL) InitializeStyle(&defaultStyle);
        else InitializeStyle(style);

        SetReadOnly(true);
        return true;
    }
    else
    {
        SetReadOnly(false);
        SetText(wxT("File not found!"));
        InitializeStyle(&defaultStyle);
        SetReadOnly(true);
        wxLogWarning(wxT("File '%s' not found"), fn.GetFullPath().c_str());
    }

    return false;
}
