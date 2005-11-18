/////////////////////////////////////////////////////////////////////////////
// Name:        scperl.h
// Purpose:     wxScriptFunctionPerl, wxScriptFilePerl, wxPerl
// Author:      Jerry Fath
// Modified by: 
// Created:     2005/13/11
// RCS-ID:      
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#if defined(wxSCRIPT_USE_PERL) && !defined(__SCPERL_H__)
#define __SCPERL_H__


// includes
#include "wx/script.h"		// base classes


//! An interpreted function.
class WXDLLIMPEXP_WXSCRIPT wxScriptFunctionPerl : public wxScriptFunction
{
public:

	//! Creates & set the properties of the object.
	wxScriptFunctionPerl(const wxString &name = wxEmptyString, 
						 const wxString &ret = wxEmptyString,
						 wxScriptTypeInfo *arr = NULL, int n = 0) { Set(name, ret, arr, n);	}
	virtual ~wxScriptFunctionPerl() {}

	virtual bool Exec(wxScriptVar &ret, wxScriptVar *arg) const;
	
	wxSCRIPTFNC_IMPLEMENT_CLONE(wxScriptFunctionPerl)
};


//! A Perl file script.
class WXDLLIMPEXP_WXSCRIPT wxScriptFilePerl : public wxScriptFile
{
public:
	wxScriptFilePerl(const wxString &toload = wxEmptyString) { 
		m_tScriptFile = wxPERL_SCRIPTFILE;
		if (!toload.IsEmpty())
			Load(toload);
	}
	virtual ~wxScriptFilePerl() {}

	virtual bool Load(const wxString &filename);
};


//! The Perl interpreter.
class WXDLLIMPEXP_WXSCRIPT wxPerl : public wxScriptInterpreter
{
protected:

	//! TRUE if the script interpreter is ready to work.
	bool m_bInit;

public:
	wxPerl() : m_bInit(FALSE) {}
	virtual ~wxPerl() { Cleanup(); }

	//! Returns the global instance of this class.
	static wxPerl *Get() { return m_pPerl; }

	//! Inits Perl.
	virtual bool Init();

    //! Cleanup Perl
    void Cleanup();


	//! Returns TRUE if Perl is ready.
	virtual bool isReady() const { return m_bInit; }

	//! Returns the list of the functions currently recognized by the interpreter.
	virtual void GetFunctionList(wxScriptFunctionArray &) const;

	//! Returns Perl Interpreter version
	virtual wxString GetVersionInfo() const;


};


#endif		// wxSCRIPT_USE_PERL


