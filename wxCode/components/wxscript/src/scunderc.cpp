///////////////////////////////////////////////////////////////////////////////
// Name:        scunderc.cpp
// Author:      Francesco Montorsi
// Modified by:
// Created:     2004/29/4
// RCS-ID:      
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////



// includes
#include <wx/tokenzr.h>
#include "wx/script.h"

#ifdef wxSCRIPT_USE_UNDERC				// compile-time choice (see wx/script.h)



// -----------------
// wxUNDERC
// -----------------

bool wxUnderC::Init()
{
	// init UnderC
	m_bInit = (uc_init(NULL, TRUE) == 0);

	// fill the std array
	m_arrStd = GetFunctionListComplete();

	return m_bInit;
}

void wxUnderC::GetFunctionListComplete(wxScriptFunctionArray &arr) const
{
	// get the function list from UnderC
	// DO NOT USE uc_eval() BECAUSE IT WOULD STRIP THE '\n'...
	char buff[16384];	
	uc_exec("#funs");
	uc_result(buff, 16384);

	wxStringTokenizer tknzr(buff);
	while (tknzr.HasMoreTokens()) {

		// get name
		wxString name = tknzr.GetNextToken();

		// ask detailed info
		// they're returned in the format:
		//
		//     n  rettype  name(args)
		//
		// where
		//
		//     n = the overload ID
		//     rettype = the returned variable type (may contain spaces)
		//     name = the name of the function
		//     args = the argument lists (only types separed by commas)
		// 
		uc_exec(wxStringBuffer(name + ";"));
		uc_result(buff, 16384);
		wxString tmp = wxString(buff);

		// sometimes, maybe "name" is the name of an overload operator...
		// in this case the tmp string is empty and we must skip it.
		if (tmp.IsEmpty())
			continue;
		
		int total = tmp.Freq('\n')+1;
		int overload = 0;

		// parse each overload
		for (; overload < total; overload++) {
			
			// choose right overload
			wxString toparse = tmp;
			if (toparse.Contains('\n')) {
				
				// remove useless lines
				for (int i=0; i < overload; i++)
					toparse = toparse.AfterFirst('\n');
				for (int j=0, max=total-overload-1; j < max; j++)
					toparse = toparse.BeforeLast('\n');
			}
			
			toparse = toparse.AfterFirst(' ');		// remove overload ID
			
			// get return type
			wxString ret = toparse.BeforeFirst('(');
			ret = ret.BeforeLast(' ');

			// remove return type (cannot use AfterFirst(' ') because 
			// rettype may contain spaces)
			toparse	= toparse.Right(toparse.Len()-ret.Len());
			toparse.Trim(FALSE);
			toparse.Trim(TRUE);
			
			// prepare everything for arg parsing
			wxASSERT(toparse.BeforeFirst('(') == name);
			toparse = toparse.AfterFirst('(');		// remove func name
			toparse = toparse.BeforeLast(')');
			
			wxScriptTypeInfo arg[16];
			int narg = 0;
			
			// parse arg list
			wxStringTokenizer argtknzr(toparse, ",");
			while (argtknzr.HasMoreTokens())
				arg[narg++] = wxScriptTypeInfo(argtknzr.GetNextToken());
			
			// finally, create this new function entry
			arr.Add(new wxScriptFunctionUnderC(name, ret, arg, narg));
		}
	}
}

void wxUnderC::GetFunctionList(wxScriptFunctionArray &arr) const
{
    arr.Clear();
	wxScriptFunctionArray arr = GetFunctionListComplete();
	
	// be sure that this is not a standard function
	for (int i=0; i < arr.GetCount(); i++) {

		// compare this function with all the std functions
		for (int j=0; j < m_arrStd.GetCount(); j++) {
			if (m_arrStd.GetName(j) == arr.GetName(i)) {

				// this function is a standard one; remove it because
				// we want to keep only user-loaded functions
				arr.Remove(i);
				i--;		// recheck this entry
				break;
			}
		}
	}
}



// -------------------------
// wxSCRIPTFUNCTIONUNDERC
// -------------------------

bool wxScriptFunctionUnderC::Exec(wxScriptVar &ret, wxScriptVar *arg) const
{
	wxString cmd = GetCallString(arg);

	// exec this function
	uc_exec(wxStringBuffer(cmd, cmd.Len()));

	// get returned value...

	return TRUE;
}



// --------------------
// wxSCRIPTFILEUNDERC
// --------------------

bool wxScriptFileUnderC::Load(const wxString &file)
{
	// remember the script filename
	m_strFileName = file;

	// load it
	//wxStringBuffer(m_strFileName, 64);
	char buff[256];
	strcpy(buff, m_strFileName);
	uc_load(buff);

	return TRUE;
}


#endif		// wxSCRIPT_USE_UNDERC

