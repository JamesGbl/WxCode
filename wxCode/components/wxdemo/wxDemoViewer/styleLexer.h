/***************************************************************
 * Name:      styleLexer.h
 * Purpose:   Defines class encapsulating a text highlighter - lexer for wxSTC control
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef STYLELEXER_H
#define STYLELEXER_H

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <wx/filename.h>
#include <wx/list.h>

/*! \brief Class encapsulating a text highlighter - lexer for wxSTC control. */
class styleLexer
{
    public:
        /*! \brief Styles for different text areas. */
        struct {
            wxColour backColor, textColor;
            bool bold, underline, italic;
            wxString keywords;
        } styles[32];

        /*! \brief Lexer identifier. */
        int lexerID;

        /*! \brief Style count. */
        unsigned int styleCount;

        /*! \brief Default contructor. */
        styleLexer();

        /*! \brief Destructor */
        ~styleLexer();

        /*!
         * \brief Loads styles from xml file.
         * \param filename Path to xml file
         */
        bool loadLexterStyle(wxString filename);

    private:
        /*! \brief Restores all styles to default values. */
        void clearAllStyles();
};

WX_DECLARE_LIST(styleLexer, styleLexerList);

#endif // STYLELEXER_H
