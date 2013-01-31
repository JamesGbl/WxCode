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


/********************************************
wxFormatFunction create new inherited classes of wxFormatFunction to provide custom functionality. ...
example:

	class expandNumberFunc : public wxFormatFunction
	{
	public:
		expandNumberFunc();
		~expandNumberFunc();
		wxString Parse(wxString input)
		{
			...
			change numerals to written numbers
			...
		}
	};

	wxStringFormatter SF;
	SF.AddFormatFunction("EXPAND", new expandNumberFunc());

	wxString sample = "I have EXPAND(1) apple."

	SF.Parse(sample);

	output = I have one apple.

***************************************
	Text is evaluated left to right parentheses first
	in the string: " FUNC($sym1) blah $sym5 (blah ($sym2)(blah3))

***************************************/
class wxFormatFunction
{
	public:
		wxFormatFunction(){}
		~wxFormatFunction(){}
/**Override Parse() in inherited classes to provide custom text formatting ...
*/
		virtual wxString Parse(wxString input) = 0;
};

WX_DECLARE_STRING_HASH_MAP( wxString, Strings );
WX_DECLARE_STRING_HASH_MAP( wxString*, StringVariables );
WX_DECLARE_STRING_HASH_MAP( wxFormatFunction*, Functions );



class wxStringFormatter
{
	public:

		wxStringFormatter(wxString concatDelims = "#", wxString blockDelims = "()", wxString tokenDelims = "$")
		{
			m_concatDelims = concatDelims;
			m_blockDelims = blockDelims;
			m_tokenDelims = tokenDelims;
		}

		~wxStringFormatter();

		/*********************************
		Initializing functions
		**********************************/

/**concatination delimeters ...
 *default is "#"
 */
		void SetConcatDelims(wxString concatDelims = "#")
		{
			m_concatDelims = concatDelims;
		}

/**delimeters for blocks and function parameters ...
 *ex: "(){}[]"
 *pass string containing pairs of opening and closing characters.
 */
		void SetBlockDelims(wxString blockDelims = "()")
		{
			m_blockDelims = blockDelims;
		}
		void SetTokenDelims(wxString tokenDelims = "$")
		{
			m_tokenDelims = tokenDelims;
		}

		void AddString(wxString key, wxString value);
		void AddStringVariable(wxString key, wxString* value);
		void AddFormatFunction(wxString key, wxFormatFunction* value);
		/*********************************/


		/**
		 *This is the function that does all the work.  Call parse supplying a wxString containing
		 *formatting symbols.
		 */
		wxString Parse(wxString input);

	private:

		wxString ReplaceSymbols(wxString input);

		int FindNextSymbol(wxString input);
		int FindEndOfSymbol(wxString input);

		wxString DoFunctions(wxString input);

		int FindNextFunction(wxString& input);
		wxString DoFunction(wxString func, wxString block);
		wxString FindNextBlock(wxString input, int& pos);
		wxFormatFunction* GetFunction(wxString funcName);

		wxString GetOpeningDelims()
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
		wxString GetClosingDelims()
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
		wxString m_concatDelims, m_blockDelims, m_tokenDelims;

#define DELIMS wxString(m_concatDelims + m_blockDelims + m_tokenDelims + " ")
#define DELIMS_OPEN GetOpeningDelims()
#define DELIMS_CLOSE GetClosingDelims()

};



#endif // _WX_STRING_FORMATTER_H_
