/////////////////////////////////////////////////////////////////////////////
// Name:        scbasic.h
// Purpose:     wxScriptFunctionBasic, wxScriptFileBasic, wxBasic
// Author:      Francesco Montorsi
// Modified by: 
// Created:     2004/6/5
// RCS-ID:      
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#if defined(wxSCRIPT_USE_BASIC) && !defined(__SCBASIC_H__)
#define __SCBASIC_H__


// includes
#include "wx/script.h"		// base classes
/*


//! A Basic interpreted function.
class wxScriptFunctionBasic : public wxScriptFunction
{
public:

	//! Creates the object; no info about return value and
	//! arguments are required since wxBasic does not store them.
	wxScriptFunctionBasic(const wxString &name = wxEmptyString) { 
		Set(name, "", NULL, 0);
	}


	virtual wxScriptFunction *Clone() const {
		wxScriptFunction *newf = new wxScriptFunctionBasic();
		newf->DeepCopy(this);
		return newf;
	}

	virtual bool Exec(wxScriptVar &ret, wxScriptVar *arg) const;
};




//! A wxBasic file script.
class wxScriptFileBasic : public wxScriptFile
{
public:
	wxScriptFileBasic(const wxString &toload = wxEmptyString) { 
		m_tScriptFile = wxBASIC_SCRIPTFILE;
		if (!toload.IsEmpty())
			Load(toload);
	}

	virtual bool Load(const wxString &filename);
};




//! The wxBasic interpreter.
class wxBasic : public wxScriptInterpreter
{
protected:
	
	//! TRUE if the script interpreter is ready to work.
	bool m_bInit;

	//! The Basic standard libraries loaded by wxBasic.
	static BasicL_reg BasicLibs[16];

	//! The array of the standard functions of UnderC.
	wxScriptFunctionArray m_arrStd;

	//! Returns the array containing ALL currently interpreted functions.
	//! This functions is different from #GetFunctionList() because
	//! it doesn't exclude functions loaded by default by UnderC.
	wxScriptFunctionArray GetFunctionListComplete() const;

public:
	wxBasic() : m_bInit(FALSE) {}
	virtual ~wxBasic() { Basic_close(m_state); }

	//! Returns the global instance of this class.
	static wxBasic *Get() { return m_pBasic; }

	//! Inits UnderC.
	virtual bool Init();

	//! Returns TRUE if UnderC is ready.
	virtual bool isReady() const { return m_bInit; }

	//! Returns the list of the functions currently recognized by the interpreter.
	virtual wxScriptFunctionArray GetFunctionList() const;
};

*/

#endif		// wxSCRIPT_USE_BASIC


