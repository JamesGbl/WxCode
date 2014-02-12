/////////////////////////////////////////////////////////////////////////////
// Name:        stringformatter.h
// Purpose:     short description here
// Author:     	nathan shaffer
// Created:     2013/january/29
// Copyright:   (c) 2013 nathan shaffer
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_STRING_FORMATTER_H_
#define _WX_STRING_FORMATTER_H_

#include <wx/string.h>
#include <wx/hashmap.h>
#include <wx/log.h>


/**
 * wxFormatFunction create new inherited classes of wxFormatFunction to provide custom functionality. ...
 * example:
 *
 * 	class expandNumberFunc : public wxFormatFunction
 * 	{
 * 	public:
 * 		 expandNumberFunc();
 * 		~expandNumberFunc();
 * 		wxString Parse(wxString input)
 * 		{
 * 			...
 * 			change numerals to written numbers
 * 			...
 * 		}
 *	};
 *
 * 	wxStringFormatter SF;
 * 	SF.AddFormatFunction("EXPAND", new expandNumberFunc());
 *
 * 	wxString sample = "I have EXPAND(1) apple.";
 *
 * 	SF.Parse(sample);
 *
 * ***************************************
 *
 *	output = I have one apple.
 * ***************************************/
class wxFormatFunction
{
	public:
		wxFormatFunction(){}
		~wxFormatFunction(){}
        /** \brief Override Parse() in inherited classes to provide custom text formatting.
         * Takes a string as an input and performs some custom text processing and returns the result.
         * \param input wxString
         * \return virtual wxString
         *
         */
		virtual wxString Parse(wxString input) = 0;
};

WX_DECLARE_STRING_HASH_MAP( wxString, Strings );
WX_DECLARE_STRING_HASH_MAP( wxString*, StringVariables );
WX_DECLARE_STRING_HASH_MAP( wxFormatFunction*, Functions );



/** \brief This is the main class to use for text formatting.
 *
 *  Order of evaluation is:\n \t left to right\n \t Block > Function parameter > Function > Symbol\n
 * 	The string:\n\t " FUNC($sym3) blah $sym4 [blah [$sym1][$sym2]]\n "
 *  would be evaluated: \n\t sym1 > sym2 > sym3 > FUNC > sym4
 */
class wxStringFormatter
{
	public:

        /** \brief
         *
         * \param concatDelims wxString
         * \param funcDelims wxString
         * \param blockDelims wxString
         * \param symbolDelims wxString
         *
         */
		wxStringFormatter(wxString concatDelims = "#", wxString funcDelims = "()", wxString blockDelims = "[]", wxString symbolDelims = "$")
		{
			m_concatDelims = concatDelims;
			m_funcDelims = funcDelims;
			m_blockDelims = blockDelims;
			m_symbolDelims = symbolDelims;
		}

		~wxStringFormatter();

		/*********************************
		Initializing functions
		**********************************/

        /** \brief concatination delimeters
         *
         * \param concatDelims wxString
         * \return void
         *
         */
		void SetConcatDelims(wxString concatDelims = "#")
		{
			m_concatDelims = concatDelims;
		}

        /** \brief delimeters for blocks and function parameters.\n
         * the string is evaluated in pairs and the first character in the pair is considered to be an opening character
         * and the second is a closing.  Order matters.
         *
         * \param funcDelims wxString
         * \return void
         *
         */
		void SetFuncDelims(wxString funcDelims = "()")
		{
			m_funcDelims = funcDelims;
		}

        /** \brief delimeters for blocks and function parameters\n
         * The string is evaluated in pairs and the first character in the pair is considered to be an opening character
         * and the second is a closing.  Order matters.
         *
         * \param blockDelims wxString
         * \return void
         *
         */
		void SetBlockDelims(wxString blockDelims = "[]")
		{
			m_blockDelims = blockDelims;
		}

        /** \brief Sets the deliminators for named symbols
         *
         * \param SymbolDelims wxString
         * \return void
         *
         */
		void SetSymbolDelims(wxString SymbolDelims = "$")
		{
			m_symbolDelims = SymbolDelims;
		}

        /** \brief Adds named symbol with a key/value pair
         *
         * \param key wxString
         * \param value wxString
         * \return void
         *
         */
		void AddString(wxString key, wxString value);
        /** \brief Add a wxString pointer as a named symbol.
		 *
         *
         * \param key wxString
         * \param value wxString*
         * \return void
         *
         */
		void AddString(wxString key, wxString* value);
        /** \brief Adds a wxFormatFunction class to the list of available functions.
         *
         * \param key wxString
         * \param value wxFormatFunction*
         * \return void
         *
         */
		void AddFormatFunction(wxString key, wxFormatFunction* value);

        /** \brief
		 *This is the function that does all the work.  Call parse supplying a wxString containing
		 *formatting symbols.
         *
         * \param input wxString
         * \return wxString
         *
         */
		wxString Parse(wxString input);

	private:

		wxString ReplaceSymbols(wxString input);

		wxString DoConcat(wxString);

		int FindNextSymbol(wxString input);
		int FindEndOfSymbol(wxString input);

		wxString DoFunctions(wxString input);

		wxString FindNextFunction(wxString input, int& Startpos, int& Endpos);
		wxString DoFunction(wxString func, wxString block);

		wxString DoBlocks(wxString input);
		int FindNextBlock(wxString& input, int pos, int& level);

		wxFormatFunction* GetFunction(wxString funcName);

		wxString GetOpeningFuncDelims()
		{
			wxString delims;
			int index=0;
			while(index < m_funcDelims.Len())
			{
				delims.Append(m_funcDelims.GetChar(index));
				index = index +2;
			}
			return delims;
		}
		wxString GetClosingFuncDelims()
		{
			wxString delims;
			int index=1;
			while(index < m_funcDelims.Len())
			{
				delims.Append(m_funcDelims.GetChar(index));
				index = index +2;
			}
			return delims;
		}
		wxString GetOpeningBlockDelims()
		{
			wxString delims;
			int index=0;
			while(index < m_blockDelims.Len())
			{
				delims.Append(m_blockDelims.GetChar(index));
				index = index +2;
			}
			return delims;
		}
		wxString GetClosingBlockDelims()
		{
			wxString delims;
			int index=1;
			while(index < m_blockDelims.Len())
			{
				delims.Append(m_blockDelims.GetChar(index));
				index = index +2;
			}
			return delims;
		}

		bool CheckSyntax(wxString& input);


		Strings m_strings;
		StringVariables m_stringVars;
		Functions m_functions;
		wxString m_concatDelims, m_funcDelims, m_blockDelims, m_symbolDelims;

#define DELIMS wxString(m_concatDelims +m_funcDelims + m_blockDelims + m_symbolDelims + " ")
#define DELIMS_B_OPEN GetOpeningBlockDelims()
#define DELIMS_B_CLOSE GetClosingBlockDelims()
#define DELIMS_F_OPEN GetOpeningFuncDelims()
#define DELIMS_F_CLOSE GetClosingFuncDelims()

};



#endif // _WX_STRING_FORMATTER_H_
