/***************************************************************
 * Name:      wxSTC.h
 * Purpose:   Defines class for wxStyledTextControl with custom colors and text highlighters
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef WXSTC_H
#define WXSTC_H

#include <wx/file.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/stc/stc.h>

#include "styleLexer.h"

typedef struct {
    wxColour backColor, lineNrBackColor, lineNrTextColor, selTextColor, selBackColor;
    int fontSize;
} wxSTCStyle;

/*! \brief Class represents wxStyledTextControl with custom colors and text highlighters. */
class wxSTC: public wxStyledTextCtrl
{
    public:
        /*!
         * \brief Default contructor.
         * \param parent Pointer to a parent window
         * \param id Window identifier
         * \param style wxStyledTextControl style
         */
        wxSTC(wxWindow *parent, wxWindowID id, wxSTCStyle style);

        /*! \brief Destructor */
        ~wxSTC();

        /*!
         * \brief Searches the text for next occurrence of entered text.
         * \param text Searched text
         */
        void SearchNext(wxString text);

        /*!
         * \brief Searches the text for previous occurrence of entered text.
         * \param text Searched text
         */
        void SearchPrev(wxString text);

        /*! \brief Set the search starting position to start of the text. */
        void ResetSearch();

        /*! \brief Returns the word under cursor. */
        wxString GetWordUnderCursor();

        /*!
         * \brief Applies selected lexer on text in control.
         * \param style Highlighter - lexer style
         */
        bool InitializeStyle(styleLexer *style);

        /*!
         * \brief Loads content of a file to control and applies selected lexer.
         * \param filename Path to input filename
         * \param style Highlighter - lexer style
         */
        bool LoadFile(const wxString &filename, styleLexer *style);

    private:
        enum { m_LineNrID, m_DividerID, m_FoldingID };

        /*! \brief Default highlighter - lexer style. */
        styleLexer defaultStyle;

        /*! \brief wxStyledTextControl style. */
        wxSTCStyle style;
};

#endif // WXSTC_H
