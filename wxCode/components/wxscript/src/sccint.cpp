///////////////////////////////////////////////////////////////////////////////
// Name:        sccint.cpp
// Author:      Francesco Montorsi
// Modified by:
// Created:     2004/29/4
// RCS-ID:      
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////



// includes
#include "wx/script.h"
#ifdef wxSCRIPT_USE_CINT				// compile-time choice (see wx/script.h)

// required includes
#include "wx/sccint.h"


// -----------------
// wxCINT
// -----------------

bool wxCINT::Init()
{
	m_bInit = (G__init_cint("cint") != G__INIT_CINT_FAILURE);
	return m_bInit;
}

void wxCINT::GetFunctionList(wxScriptFunctionArray &arr) const
{
	wxScriptTypeInfo arrarg[40];		// 40 = maximum number of function argument for CINT
	G__MethodInfo func;
	
	while (func.Next())	{		// discard first invalid function ....
		
		// discard compiled functions
		long property = func.Property();
		if (property & G__BIT_ISCOMPILED)
			continue;
		
		// discard function without return types
		G__TypeInfo *t = func.Type();
		if (t == NULL)
			continue;
		
		// load argument list
		G__MethodArgInfo arg(func);
		int i=0;
		while(arg.Next())			// discard first invalid argument			
			arrarg[i++].Set(arg.Type()->Name());
		
		// add an entry to the array
		arr.Append(new wxScriptFunctionCINT(func.Name(), t->Name(), arrarg, i));
	}
}




// ---------------------
// wxSCRIPTFUNCTION
// ---------------------

bool wxScriptFunctionCINT::Exec(wxScriptVar &ret, wxScriptVar *arg) const
{
	// create the string with the arguments to give to the function
	wxString cmd = GetCallString(arg);

	// exec the function
	G__value returnvalue = G__calc(cmd);

	// for float & double we need special conversion
	if (m_tReturn.GetGenericType() == wxSTG_FLOAT ||
		m_tReturn.GetGenericType() == wxSTG_DOUBLE) {
		
		ret.Set(m_tReturn.GetGenericType(), 
			wxString::Format("%f", G__double(returnvalue)));
		return TRUE;
	}

	// for all other types, we just get an int...
	long res = G__int(returnvalue);

	// ... and transform it into the correct value
	ret.Set(m_tReturn.GetGenericType(), res);

	return TRUE;
}




// --------------
// wxSCRIPTFILE
// --------------

bool wxScriptFileCINT::Load(const wxString &file)
{
	// remember the script filename
	m_strFileName = file;

	// load it
	if (G__loadfile(file) != G__LOADFILE_SUCCESS) {

		// get a sort of error description
		wxScriptInterpreter::m_strLastErr = 
			wxString::Format("%s [%s]", G__lasterror_filename(), G__lasterror_linenum());

		return FALSE;
	}
	return TRUE;
}

#endif		// wxSCRIPT_USE_CINT


