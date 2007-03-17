/***************************************************************
 * Name:      wxledfont.h
 * Purpose:   Defines Class wxLEDFont
 * Author:    Christian Gräfe (info@mcs-soft.de)
 * Created:   2007-03-02
 * Copyright: Christian Gräfe (www.mcs-soft.de)
 * License:	  wxWindows licence
 **************************************************************/

#ifndef WXLEDFONT_H
#define WXLEDFONT_H

#include <wx/wx.h>

#include "advancedmatrixobject.h"

WX_DECLARE_HASH_MAP( wxChar , MatrixObject*, wxIntegerHash, wxIntegerEqual, wxLEDFontHashMap );

class wxLEDFont
{
	public:
		wxLEDFont();
		virtual ~wxLEDFont();

		// Get the MatrixObject from a letter of the Font
		// the MatrixObject is deletet, when the font is destroyed
		const MatrixObject* GetLetter(wxChar l);

		// Get the MatrixObject for an Text
		// please delete the MO which you get, if you don't need it anymore
		AdvancedMatrixObject* GetMOForText(const wxString& text, wxAlignment a=wxALIGN_LEFT);

		void SetLetterSpace(int letterspace) {m_letterspace=letterspace;}
		int GetLetterSpace() const {return m_letterspace;}

		int GetMaxLetterWidth() const {return m_maxLetterWidth;}
		int GetMaxLetterHeight() const {return m_maxLetterHeight;}

	private:
		wxLEDFontHashMap m_letters;
		int m_letterspace;
		int m_maxLetterWidth;
		int m_maxLetterHeight;

		static const char ms_standartLettersData[95][35];
		static const wxString ms_standartLettersChar;
		static const int ms_slWidth;
		static const int ms_slHeight;
};

#endif // WXLEDFONT_H
