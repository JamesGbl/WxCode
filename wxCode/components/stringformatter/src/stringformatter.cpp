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
	input = DoBlocks(input);
	input = DoFunctions(input);
	input = ReplaceSymbols(input);
	input = DoConcat(input);
	return input;
}

bool wxStringFormatter::CheckSyntax(wxString& input)
{
	int opDelim =0;
	int clDelim = 0;
	int index = input.find_first_of(DELIMS_F_OPEN, 0);
	while(index != wxNOT_FOUND)
	{
		if(index != wxNOT_FOUND)
		{
			opDelim++;
			index = input.find_first_of(DELIMS_F_OPEN, index +1);
		}
	}
	index = input.find_first_of(DELIMS_F_CLOSE, 0);
	while(index != wxNOT_FOUND)
	{
		if(index != wxNOT_FOUND)
		{
			clDelim++;
			index = input.find_first_of(DELIMS_F_CLOSE, index+1);
		}
	}
	if(opDelim < clDelim)
	{
		input = "!ERROR: Missing \"" + DELIMS_F_OPEN + "\"!";
		return true;
	}
	else if(opDelim > clDelim)
	{
		input = "!ERROR: Missing \"" + DELIMS_F_CLOSE + "\"!";
		return true;
	}

	opDelim =0;
	clDelim = 0;
	index = input.find_first_of(DELIMS_B_OPEN, 0);
	while(index != wxNOT_FOUND)
	{
		if(index != wxNOT_FOUND)
		{
			opDelim++;
			index = input.find_first_of(DELIMS_B_OPEN, index +1);
		}
	}
	index = input.find_first_of(DELIMS_B_CLOSE, 0);
	while(index != wxNOT_FOUND)
	{
		if(index != wxNOT_FOUND)
		{
			clDelim++;
			index = input.find_first_of(DELIMS_B_CLOSE, index+1);
		}
	}

	if(opDelim < clDelim)
	{
		input = "!ERROR: Missing \"" + DELIMS_B_OPEN + "\"!";
		return true;
	}
	else if(opDelim > clDelim)
	{
		input = "!ERROR: Missing \"" + DELIMS_B_CLOSE + "\"!";
		return true;
	}
	return false;
}

wxString wxStringFormatter::DoConcat(wxString input)
{
	input.Replace("#", "", true);
	return input;
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
		wxLogDebug("ReplaceSymbols :"+sym);

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
			val = sym;
		}
		tmp.insert(index,val);

		end = index + val.Len() + 1;
		index = FindNextSymbol(tmp);
	}
	return tmp;

}

int wxStringFormatter::FindNextSymbol(wxString input)
{
	return input.find_first_of(m_symbolDelims);
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

wxString wxStringFormatter::DoBlocks(wxString input)
{
	int level = 0;
	int pos = 0;
	while(input.find_first_of(m_blockDelims) != wxNOT_FOUND)
	{
		pos = FindNextBlock(input, 0, level);
	}
	return input;
}

int wxStringFormatter::FindNextBlock(wxString& input, int pos, int& level)
{
	wxString str;
	wxLogDebug(wxString::Format("FindNextBlock: %d ", pos) + input);
	int blockOpen;
	int blockClose;

	blockClose = input.find_first_of(m_blockDelims, pos);

	if(DELIMS_B_CLOSE.Contains(input.GetChar(blockClose)))
	{
		return blockClose;
	}
	else if(DELIMS_B_OPEN.Contains(input.GetChar(blockClose)))
	{
		level++;
		blockOpen = blockClose;
		blockClose = FindNextBlock(input, blockClose+1, level);

		wxString block;
		block = input.SubString(blockOpen + 1, blockClose - 1);
		str.Printf("> %d %d", blockOpen + 1, blockClose - 1);
		wxLogDebug("block > " + block + str);
		block = DoFunctions(block);
		block = ReplaceSymbols(block);
		block = DoConcat(block);
		input.replace(blockOpen, (blockClose + 1) - blockOpen, block);
		level--;
	}
	else
	{
		wxLogDebug("not found");
		return wxNOT_FOUND;
	}

	if(level > 0)
	{
		return FindNextBlock(input, blockOpen, level);
	}
	return -1;
}

wxString wxStringFormatter::DoFunctions(wxString input)
{
	wxLogDebug("DoFunctions: "+ input);
	int startPos = 0, endPos;
	wxString result =FindNextFunction(input, startPos, endPos);
	while(startPos != wxNOT_FOUND)
	{
		input.replace(startPos, endPos - startPos, result);
		result = FindNextFunction(input, startPos, endPos);
	}
	return input;
}

wxString wxStringFormatter::DoFunction(wxString func, wxString block)
{
	wxLogDebug("DoFunction: " + block);
	if(GetFunction(func))
	{
		return GetFunction(func)->Parse(ReplaceSymbols(block));
	}
	return func + block;
}

wxFormatFunction* wxStringFormatter::GetFunction(wxString funcName)
{
	return m_functions[funcName];
}


wxString wxStringFormatter::FindNextFunction(wxString input, int& Startpos, int& Endpos)
{
	wxLogDebug("FindNextFunction: " + input);
	int functBegin;
	int blockOpen = input.find_first_of(DELIMS_F_OPEN);
	if(blockOpen == wxNOT_FOUND)
	{
		Startpos = wxNOT_FOUND;
		return "";
	}

	int blockClose = input.find_first_of(m_funcDelims, blockOpen +1);
	while(!DELIMS_F_CLOSE.Contains(input.GetChar(blockClose)) && blockClose != wxNOT_FOUND)
	{
		if(DELIMS_F_OPEN.Contains(input.GetChar(blockClose)))
		{
			blockOpen = blockClose;
			blockClose = input.find_first_of(m_funcDelims, blockOpen +1);
		}
	}

	if(blockClose == wxNOT_FOUND)
	{
		Startpos = wxNOT_FOUND;
		return "";
	}

	Endpos = blockClose + 1;

	//return input.SubString(blockOpen+1, blockClose-1);

	functBegin = input.find_last_of(DELIMS, blockOpen -1);
	functBegin++;

	wxString tmp = DoFunction(input.SubString(functBegin, blockOpen -1),  input.SubString(blockOpen+1, blockClose-1));
	if(m_concatDelims.Contains(input.GetChar(functBegin -1)))
	{
		functBegin--;
	}
	Startpos = functBegin;
	return tmp;
}

void wxStringFormatter::AddString(wxString key, wxString value)
{
	m_strings[key] = value;
}

void wxStringFormatter::AddString(wxString key, wxString* value)
{
	m_stringVars[key] = value;
}

void wxStringFormatter::AddFormatFunction(wxString key, wxFormatFunction* value)
{
	m_functions[key] = value;
}

