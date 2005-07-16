/////////////////////////////////////////////////////////////////////////////
// Name:        sclua.h
// Purpose:     wxScriptFunctionLua, wxScriptFileLua, wxLua
// Author:      Francesco Montorsi
// Created:     2004/6/5
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#if defined(wxSCRIPT_USE_LUA) && !defined(__SCLUA_H__)
#define __SCLUA_H__


// includes
extern "C" {			// this wrapping is very important: Lua is a C library !!!
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
};

#include "wx/script.h"		// base classes



// String conversions (all LUA functions uses always chars)
// ---------------------------------------------------------

#if wxUSE_UNICODE
	#define WX2LUA(x)		(wxString(x).mb_str(wxConvUTF8).data())
#else
	#define WX2LUA(x)		(wxString(x).mb_str(wxConvUTF8))
#endif

#define LUA2WX(x)			(wxString(x, wxConvUTF8))




//! A Lua interpreted function.
class WXDLLIMPEXP_WXSCRIPT wxScriptFunctionLua : public wxScriptFunction
{
public:

	//! Creates a wxScriptVar reading the top value in the lua stack.
	static wxScriptVar CreateScriptVarFromStack();

public:

	//! Creates the object; no info about return value and
	//! arguments are required since Lua does not store them.
	wxScriptFunctionLua(const wxString &name = wxEmptyString) { Set(name, wxT(""), NULL, 0); }
	virtual ~wxScriptFunctionLua() {}

	virtual bool Exec(wxScriptVar &ret, wxScriptVar *arg) const;
	
	wxSCRIPTFNC_IMPLEMENT_CLONE(wxScriptFunctionLua)
};




//! A Lua file script.
class WXDLLIMPEXP_WXSCRIPT wxScriptFileLua : public wxScriptFile
{
public:
	wxScriptFileLua(const wxString &toload = wxEmptyString) { 
		m_tScriptFile = wxLUA_SCRIPTFILE;
		if (!toload.IsEmpty())
			Load(toload);
	}
	virtual ~wxScriptFileLua() {}

	virtual bool Load(const wxString &filename);
};




//! The Lua interpreter.
class WXDLLIMPEXP_WXSCRIPT wxLua : public wxScriptInterpreter
{
public:

	//! The LUA state of the interpreter. This variable is public because
	//! is must be accessible also by extern functions/classes.
	lua_State *m_state;

protected:
	
	//! TRUE if the script interpreter is ready to work.
	bool m_bInit;

	//! The Lua standard libraries loaded by wxLua.
	static luaL_reg luaLibs[16];

	//! The array of the standard functions of lua.
	wxScriptFunctionArray m_arrStd;


protected:

	//! Returns the array containing ALL currently interpreted functions.
	//! This functions is different from #GetFunctionList() because
	//! it doesn't exclude functions loaded by default by UnderC.
	void GetFunctionListComplete(wxScriptFunctionArray &) const;


public:
	wxLua() : m_bInit(FALSE) {}
	virtual ~wxLua() { Cleanup(); }

	//! Returns the global instance of this class.
	static wxLua *Get() { return m_pLua; }

	//! Gets the description of the last error and puts it in the
	//! wxScriptInterpreter::m_strLastErr variable; then returns FALSE.
	static bool SetErrAndReturn(lua_State *L);

	//! Inits Lua.
	virtual bool Init();

	//! Undoes what #Init() does.
	virtual void Cleanup();

	//! Returns TRUE if Lua is ready.
	virtual bool isReady() const { return m_bInit; }

	//! Returns the list of the functions currently recognized by the interpreter.
	virtual void GetFunctionList(wxScriptFunctionArray &) const;

	//! Uses the _VERSION global variable to get info about the LUA intepreter.
	virtual wxString GetVersionInfo() const;
};



#endif		// wxSCRIPT_USE_LUA


