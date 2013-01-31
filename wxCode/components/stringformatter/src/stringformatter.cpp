/////////////////////////////////////////////////////////////////////////////
// Name:        stringformatter.cpp
// Purpose:     short description here
// Author:     	nathan shaffer
// Created:     2013/january/29
// Copyright:   (c) 2013 nathan shaffer
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
#endif

#include "wx/stringformatter.h"
wxStringFormatter::~wxStringFormatter()
{

}

wxString wxStringFormatter::Parse(wxString input)
{
	if(CheckSyntax(input)) return input;
	return ReplaceSymbols(DoFunctions(input));
}

bool wxStringFormatter::CheckSyntax(wxString& input)
{
	int opDelim =0;
	int clDelim = 0;
	int index = input.find_first_of(DELIMS_OPEN, 0);
	while(index != wxNOT_FOUND)
	{
		if(index != wxNOT_FOUND)
		{
			opDelim++;
			index = input.find_first_of(DELIMS_OPEN, index +1);
		}
	}
	index = input.find_first_of(DELIMS_CLOSE, 0);
	while(index != wxNOT_FOUND)
	{
		if(index != wxNOT_FOUND)
		{
			clDelim++;
			index = input.find_first_of(DELIMS_CLOSE, index+1);
		}
	}
	if(opDelim == clDelim) {
		return false;
	}
	else if(opDelim < clDelim) {
		input = "!ERROR: Missing \"" + DELIMS_OPEN + "\"!";
		return true;
	}
	else if(opDelim > clDelim) {
		input = "!ERROR: Missing \"" + DELIMS_CLOSE + "\"!";
		return true;
	}
	return true;
}

wxString wxStringFormatter::ReplaceSymbols(wxString input)
{
	wxString tmp = input;
	int index = FindNextSymbol(tmp);

	while(index != wxNOT_FOUND)
	{
		tmp.Remove(index, 1);
		int end = FindEndOfSymbol(tmp.Mid(index));
		wxString sym = tmp.SubString(index, index+end);

		///remove trailing concatination delimeter
		if(m_concatDelims.Contains(tmp.GetChar(index + end + 1)))
		{
			tmp.Remove(index + end + 1, 1);
		}

		///remove symbol
		tmp.Remove(index, sym.Len());

		wxString val;
		if(m_strings.find(sym) != m_strings.end())
		{
			val = m_strings[sym];
		}
		else if(m_stringVars.find(sym) != m_stringVars.end())
		{
			val = *m_stringVars[sym];
		}
		else
		{
			val = "!UNDEF!";
		}
		tmp.insert(index,val);

		end = index + val.Len() + 1;
		index = FindNextSymbol(tmp);
	}
	return tmp;

}


int wxStringFormatter::FindNextSymbol(wxString input)
{
	return input.find_first_of(m_tokenDelims);
}

int wxStringFormatter::FindEndOfSymbol(wxString input)
{
	int pos = input.find_first_of(DELIMS);

	if(pos == wxNOT_FOUND)
	{
		return input.Len()-1;
	}
	return pos - 1;
}

wxString wxStringFormatter::DoFunctions(wxString input)
{
	int pos = 0;
	while(pos != wxNOT_FOUND)
	{
		pos = FindNextFunction(input);
	}
	return input;
}

wxString wxStringFormatter::DoFunction(wxString func, wxString block)
{

	if(GetFunction(func))
	{
		return GetFunction(func)->Parse(block);
	}
	return func + block;
}

wxFormatFunction* wxStringFormatter::GetFunction(wxString funcName)
{
	return m_functions[funcName];
}


int wxStringFormatter::FindNextFunction(wxString& input)
{
	int pos;
	wxString block = FindNextBlock(input, pos);
	if(pos != wxNOT_FOUND)
	{
		pos--;
		int functBegin = input.find_last_of(DELIMS, pos);
		functBegin++;

		wxString tmp = DoFunction(input.SubString(functBegin, pos),  block);
		if(m_concatDelims.Contains(input.GetChar(functBegin -1)))
		{
			functBegin--;
		}
		input.Remove(functBegin, (pos + block.Len() +2) - (functBegin -1));
		input.insert(functBegin, tmp);
	}
	return pos;
}

wxString wxStringFormatter::FindNextBlock(wxString input, int& pos)
{
	int blockOpen = input.find_first_of(DELIMS_OPEN);
	if(blockOpen == wxNOT_FOUND)
	{
		pos = wxNOT_FOUND;
		return "";
	}

	int blockClose = input.find_first_of(m_blockDelims, blockOpen +1);
	while(!DELIMS_CLOSE.Contains(input.GetChar(blockClose)) && input.GetChar(blockClose) != wxNOT_FOUND)
	{
		if(DELIMS_OPEN.Contains(input.GetChar(blockClose)))
		{
			blockOpen = blockClose;
			blockClose = input.find_first_of(m_blockDelims, blockOpen +1);
		}
	}

	if(input.GetChar(blockClose) == wxNOT_FOUND)
	{
		pos = wxNOT_FOUND;
		return "";
	}

	if(DELIMS_CLOSE.Contains(input.GetChar(blockClose)))
		pos = blockOpen;

	return ReplaceSymbols(input.SubString(blockOpen+1, blockClose-1));
}

void wxStringFormatter::AddString(wxString key, wxString value)
{
	m_strings[key] = value;
}

void wxStringFormatter::AddStringVariable(wxString key, wxString* value)
{
	m_stringVars[key] = value;
}

void wxStringFormatter::AddFormatFunction(wxString key, wxFormatFunction* value)
{
	m_functions[key] = value;
}

