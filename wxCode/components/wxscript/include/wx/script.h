/////////////////////////////////////////////////////////////////////////////
// Name:        script.h
// Purpose:     wxScriptInterpreter, wxScriptTypeInfo, wxScriptVar, 
//              wxScriptFunction, wxScriptFunctionArray, wxScriptFile
// Author:      Francesco Montorsi
// Created:     2004/29/4
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#ifndef SCRIPT_H
#define SCRIPT_H


// defines for conditional compilation: in this way you can choose
// at compile-time which interpreter you want to support: this is 
// very useful when you have not time to get all the libs for all
// the interpreters supported by wxScript.
//
// To define this, you should use the universal -D (or /D for MSVC)
// compile option in your makefiles for all the wxScript source files:
//
//    gcc -c -DwxSCRIPT_NO_CINT src/script.cpp         [ example how the
//    gcc -c -DwxSCRIPT_NO_CINT src/sclua.cpp       <--[ makefile output
//    gcc -c -DwxSCRIPT_NO_CINT src/sccint.cpp         [ should appear
//    [...]
// 
#ifndef wxSCRIPT_NO_CINT
#define wxSCRIPT_USE_CINT
#endif

#ifndef wxSCRIPT_NO_UNDERC
#define wxSCRIPT_USE_UNDERC
#endif

#ifndef wxSCRIPT_NO_LUA
#define wxSCRIPT_USE_LUA
#endif

#ifndef wxSCRIPT_NO_PYTHON
#define wxSCRIPT_USE_PYTHON
#endif



// Includes
// ------------------

#include <wx/wx.h>



// Defines & macros
// ------------------

//! The maximum number of arguments for a single function.
#define wxSCRIPTFNC_MAX_ARGS			32

#ifndef wxSAFE_DELETE
#define wxSAFE_DELETE(x)				{ if (x) delete x; x = NULL; }
#endif

#ifndef wxSAFE_DELETE_ARRAY
#define wxSAFE_DELETE_ARRAY(x)			{ if (x) delete [] x; x = NULL; }
#endif

#ifndef wxSCRIPT_EXPORT
#ifdef __VISUALC__
#define wxSCRIPT_EXPORT		_declspec(dllexport)
#else
#define wxSCRIPT_EXPORT     // GCC always exports everything
#endif
#endif





// Global objects
// ------------------

class wxScriptTypeInfo;		 // defined later

// basic types
extern wxScriptTypeInfo *wxScriptTypeVOID;
extern wxScriptTypeInfo *wxScriptTypeINT;
extern wxScriptTypeInfo *wxScriptTypeCHAR;
extern wxScriptTypeInfo *wxScriptTypeLONG;
extern wxScriptTypeInfo *wxScriptTypeFLOAT;
extern wxScriptTypeInfo *wxScriptTypeDOUBLE;
extern wxScriptTypeInfo *wxScriptTypeBOOL;

// pointers to basic types
extern wxScriptTypeInfo *wxScriptTypePVOID;
extern wxScriptTypeInfo *wxScriptTypePINT;
extern wxScriptTypeInfo *wxScriptTypePCHAR;
extern wxScriptTypeInfo *wxScriptTypePLONG;
extern wxScriptTypeInfo *wxScriptTypePFLOAT;
extern wxScriptTypeInfo *wxScriptTypePDOUBLE;
extern wxScriptTypeInfo *wxScriptTypePBOOL;


//! A classification of the types of a wxScriptVar.
enum wxScriptTypeGeneric {

	wxSTG_UNDEFINED = -1,		//! Something wrong.

	wxSTG_VOID,			//! Basic C++ types.
	wxSTG_INT,
	wxSTG_LONG,
	wxSTG_CHAR,
	wxSTG_FLOAT,
	wxSTG_DOUBLE,
	wxSTG_BOOL,

	wxSTG_USERDEFINED,	//! A C++ user-defined type (a class, structure, union or enum).

	wxSTG_POINTER,		//! A pointer to something.
	wxSTG_REFERENCE		//! A reference to something.
};



//! A sort of extended ENUM containing information and utilities
//! about a interpreted type which can be chosen at runtime.
class wxScriptTypeInfo
{
protected:

	//! The way this class stores a C++ type cannot be an enum or
	//! something like that because in C++ there can be infinite
	//! data types with different names, because an undefined
	//! number of classes, structures, unions... can be declared.
	wxString m_strName;

public:
	wxScriptTypeInfo(const wxString &str = wxEmptyString) { Set(str); }
	wxScriptTypeInfo(wxScriptTypeGeneric type) { SetGenericType(type); }
	virtual ~wxScriptTypeInfo() {}

	// miscellaneous
	virtual void DeepCopy(const wxScriptTypeInfo *p)		{ m_strName = p->m_strName; }
	virtual bool Match(const wxScriptTypeInfo &p) const		{ return (GetName().IsSameAs(p.GetName(), FALSE)); }
	virtual bool Match(const wxScriptTypeInfo *p) const		{ return p != NULL && Match(*p); }
	friend bool operator==(const wxScriptTypeInfo &first, const wxScriptTypeInfo &second) 
		{ return first.Match(second); }

	// setters
	virtual void Set(const wxString &str);
	virtual void SetGenericType(wxScriptTypeGeneric t);
	virtual void SetAsPointer()			{ Set(GetName() + wxT("*")); }
	virtual void SetAsReference()		{ Set(GetName() + wxT("&")); }
	
	// getters
	wxString GetPointerTypeName() const;
	wxString GetName() const						{ return m_strName; }
	wxScriptTypeInfo GetPointerType() const			{ return wxScriptTypeInfo(GetPointerTypeName()); }
	wxScriptTypeGeneric GetGenericType() const;

	// checkers
	bool isPointer() const;
	bool isReference() const;
	bool isValid() const			{ return !m_strName.IsEmpty(); }
};



//! A variable object containing both info about its type
//! (using wxScriptTypeInfo) and the contents of the variable.
class wxScriptVar
{
protected:

	//! The type of this variable.
	wxScriptTypeInfo m_tType;

	//! The contents of this variable.
	union {
		long m_content;
		double m_floatcontent;
	};

public:

	//! Copy constructor.
	wxScriptVar(const wxScriptVar &var) {
		Copy(var);
	}

	wxScriptVar(const wxString &type = wxEmptyString, 
				const wxString &content = wxEmptyString) : m_content(0) {
		SetType(type);
		SetContent(content);
	}

	//! This constructor is used to init this variable as a pointer.
	//! The given type string must end with "*" and the given memory
	//! address must be cast to "int *" to avoid ambiguity with the
	//! previous constructor.
	//!
	//! Example:
	//!    myClass test;
	//!    wxScriptVar arg1("myClass *", (int *)(&test));
	//!
	wxScriptVar(const wxString &type,
				int *pointer) : m_content((long)pointer) {

		SetType(type);
		wxASSERT(GetType().isPointer());
	}

	virtual ~wxScriptVar() { ResetContent(); }


	wxScriptVar &operator=(const wxScriptVar &tocopy)
		{ Copy(tocopy); return *this; }



	/////////////////////////////////////////////////////////////////////////////
	//! \name SET functions. @{

	//! Sets the type of this variable; wxScriptTypeInfo::Set() is used.
	virtual void SetType(const wxString &str)		{ m_tType.Set(str); }

	//! Sets the contents of this variable using the given string.
	//! The content decoding is done using the current type set.
	//! This function uses the same decode rules used to encode
	//! the content by the #GetContentString() function; so, no
	//! loss of data should happen if you do:
	//! \code
	//!     wxScriptVar myvar("bool", "false");
	//!     myvar.SetContent(myvar.GetContentString());
	//! \endcode
	virtual void SetContent(const wxString &);

	virtual void SetContent(const wxChar *str)		{ SetContent(wxString(str)); }
	virtual void SetContent(wxChar *str)			{ SetContent(wxString(str)); }
	virtual void SetContent(long l)					{ m_content = l; }
	virtual void SetContent(double l)				{ m_floatcontent = l; }
	virtual void SetContent(bool l)					{ m_content = (l != 0); }
	virtual void SetContent(void *l)				{ m_content = (long)l; }
	virtual void SetContent(const void *l)			{ m_content = (long)l; }

	//! Changes both type & content of this variable.
	void Set(const wxString &type, const wxString &content)		{ SetType(type); SetContent(content); }
	void Set(wxScriptTypeGeneric type, char *content)			{ m_tType.SetGenericType(type); SetContent(content); }
	void Set(wxScriptTypeGeneric type, long content)			{ m_tType.SetGenericType(type); SetContent(content); }
	void Set(wxScriptTypeGeneric type, double content)			{ m_tType.SetGenericType(type); SetContent(content); }
	void Set(wxScriptTypeGeneric type, bool content)			{ m_tType.SetGenericType(type); SetContent(content); }
	void Set(wxScriptTypeGeneric type, void *content)			{ m_tType.SetGenericType(type); SetContent(content); }
	void Set(wxScriptTypeGeneric type, const wxString &content) { m_tType.SetGenericType(type); SetContent(content); }

	//! Sets as empty the current contents (deleting eventually allocated memory).
	virtual void ResetContent();

	//! Copies the given wxScriptVar object.
	virtual void Copy(const wxScriptVar &var);

	//@}



	/////////////////////////////////////////////////////////////////////////////
	//! \name GET functions. @{

	//! Returns the type of this variable.
	wxScriptTypeInfo GetType() const	{ return m_tType; }

	//! Returns the contents of this variable as LONG.
	long GetContentLong() const			{ return m_content; }

	//! Returns the contents of this variable as a DOUBLE.
	double GetContentDouble() const		{ return (m_tType == wxSTG_INT || m_tType == wxSTG_LONG) ? m_content : m_floatcontent; }

	//! Returns the contents encoded in a string.
	//! The content encoding is done using the current type set.
	//! This function uses the same encode rules used to decode
	//! the content by the #SetContent(const wxString &) function.
	virtual wxString GetContentString() const;	

	//! Returns the memory address hold by this variable if it is set
	//! as a pointer.
	virtual void *GetPointer() const	{ return (m_tType.isPointer() ? (void*)m_content : NULL); }

	//@}
};



//! An interpreted function.
class wxScriptFunction
{
protected:

	//! The name of the function.
	wxString m_strName;

	//! The return type of the function.
	wxScriptTypeInfo m_tReturn;

	//! The argument type array.
	wxScriptTypeInfo m_pArgList[wxSCRIPTFNC_MAX_ARGS];

	//! The number of entries of #m_pArgList array.
	int m_nArgCount;

protected:

	//! Returns the "call string"; that is, the string containing
	//! function name + wxT("(") + arg[n]->GetContentString() + ")".
	wxString GetCallString(wxScriptVar *arr) const;

public:

	//! Creates & set the properties of the object.
	wxScriptFunction(const wxString &name = wxEmptyString, 
					const wxString &ret = wxEmptyString,
					wxScriptTypeInfo *arr = NULL, int n = 0) { 
		m_nArgCount = 0;
		Set(name, ret, arr, n);
	}

	virtual ~wxScriptFunction() {}


	//! Deep copies the function.
	virtual void DeepCopy(const wxScriptFunction *tocopy) {
		m_strName = tocopy->m_strName;
		m_nArgCount = tocopy->m_nArgCount;
		m_tReturn.DeepCopy(&tocopy->m_tReturn);

		for (int i=0; i < m_nArgCount; i++)
			m_pArgList[i].DeepCopy(&tocopy->m_pArgList[i]);
	}

	//! Returns a clone of this object.
	//! The caller must delete the returned object when it's no longer useful.
	virtual wxScriptFunction *Clone() const = 0;

	//! Checks if this function has the given return type, argument count
	//! and argument types.
	virtual bool Match(const wxScriptTypeInfo *ret, int argc, ...);

	//! Executes the function script.
	virtual bool Exec(wxScriptVar &ret, wxScriptVar *arg) const = 0;

	//! Sets the name, return type and arguments of this scripted function.
	virtual void Set(const wxString &name, const wxString &ret, 
					 wxScriptTypeInfo *arr, int n);


	//////////////////////////////////////////////////////////////////////
	//! \name Getters. @{

	//! Returns the name of this function
	wxString GetName() const	{ return m_strName; }

	//! Returns the number of arguments accepted by this function.
	int GetArgCount() const		{ return m_nArgCount; }

	//! Returns the type of the return value of this function.
	wxScriptTypeInfo GetRetType() const			{ return m_tReturn; }

	//! Returns the type of the n-th argument required by this function.
	wxScriptTypeInfo GetArgType(int n) const	{ return m_pArgList[n]; }

	//@}
};



//! An array of wxScriptFunction objects.
class wxScriptFunctionArray
{
protected:

	//! A simple auto-expanding array containing pointers to the 
	//! wxScriptFunction contained... the object handled by this array
	//! are not automatically deleted: wxScriptFunctionArray::Clear
	//! have to cleanup everything.
	//!
	//! VERY IMPORTANT: we cannot use the wxObjArray macros because
	//! we need to store pointers to abstract classes which cannot
	//! be copied (wxObjArray requires non-abstract classes to store).
	wxArrayPtrVoid m_arr;

	//! An optional string which is used by #GetName() to strip
	//! off unwanted function prefixes.
	wxString m_strToStrip;

public:
	wxScriptFunctionArray(const wxString &tostrip = wxEmptyString) : m_strToStrip(tostrip) {}
	virtual ~wxScriptFunctionArray() { Clear(); }


	// --------
	// Getters
	// --------

	wxScriptFunction *Get(int idx) const		{ return (wxScriptFunction *)m_arr.Item(idx); }
	wxScriptFunction *Get(const wxString &fncname, int n = 0) const;
	int GetIdx(const wxString &fncname, int n = 0) const;

	int GetCountOf(const wxString &fncname) const;
	wxString GetName(int n) const;
	wxString GetPrefixToStrip() const			{ return m_strToStrip; }
	int GetCount() const						{ return m_arr.GetCount(); }
	wxArrayPtrVoid &GetArray()					{ return m_arr; }


	// --------------
	// Miscellaneous
	// --------------

	void Insert(wxScriptFunction *toadd, int idx);
	void Append(wxScriptFunction *toadd)				{ m_arr.Insert(toadd, GetCount()); }
	void Append(const wxScriptFunctionArray &arr);

	void Remove(int i);
	void Remove(const wxString &fncname, int n = 0)		{ Remove(GetIdx(fncname, n)); }

	void SetPrefixToStrip(const wxString &str)			{ m_strToStrip = str; }
	void DeepCopy(const wxScriptFunctionArray *arr);
	void Clear();
};



//! The types of the script files recognized by wxScriptFile
enum wxScriptFileType {

	//! The extension of the script file will be used to recognize it.
	wxRECOGNIZE_FROM_EXTENSION = -2,

	//! The first non-whitespace characters found will be compared
	//! to the various types of comments in the different scripting
	//! languages supported to recognize the script file type.
	wxRECOGNIZE_FROM_COMMENT = -1,

	wxCINT_SCRIPTFILE = 0,		// extension = "cxx"
	wxUNDERC_SCRIPTFILE = 1,	// extension = "uc"
	wxLUA_SCRIPTFILE = 2,		// extension = "lua"
	wxPYTHON_SCRIPTFILE = 3,	// extension = "py"

	wxSCRIPT_SUPPORTED_FORMATS = 4		// this must be the last
};



//! A script file.
class wxScriptFile
{
protected:

	//! The name of the file containing the scripted functions.
	wxString m_strFileName;

	//! The default script file type.
	wxScriptFileType m_tScriptFile;

public:

	//! The array of the extensions recognized by wxScriptFile.
	//! You can access this array using the wxScriptFileType enum:
	//!
	//! wxString old = wxScriptFile::m_strFileExt[wxLUA_SCRIPTFILE];
	//! wxScriptFile::m_strFileExt[wxLUA_SCRIPTFILE] = "myext";
	//!
	static wxString m_strFileExt[wxSCRIPT_SUPPORTED_FORMATS];

	//! Returns a string in the form
	//!               EXT1;EXT2;EXT3;EXT4 .....
	//! where EXT1, EXT2 ... are the extensions supported by the auto-recognition
	//! algorithm of the wxScriptFile class (which uses wxScriptFile::m_strFileExt
	//! to store the extensions associated with each allowed script format).
	static wxString GetAllowedExtString();
	static wxArrayString GetAllowedExt();

public:
	wxScriptFile() {}
	virtual ~wxScriptFile() {}
	
	virtual bool Load(const wxString &file) = 0;
	wxScriptFileType GetScriptFileType() const			{ return m_tScriptFile; }	
};




// these will be, eventually, defined later
class wxCINT;
class wxUnderC;
class wxLua;
class wxPython;

//! A singleton class that wraps all the script interpreters supported.
class wxScriptInterpreter
{
protected:		// use wxCINT::Get(), wxUnderC::Get() or wxLua::Get() to
				// access them...

	//! The shared instance of the wxScriptInterpreter.
	static wxCINT *m_pCINT;
	static wxUnderC *m_pUnderC;
	static wxLua *m_pLua;
	static wxPython *m_pPython;

public:		// ctor & dtor

	wxScriptInterpreter();
	virtual ~wxScriptInterpreter();

	//! The description of the last error.
	static wxString m_strLastErr;

public:		// static functions

	// A STATIC function which returns the global instance of this class
	// cannot exist because wxScriptInterpreter is an abstract class
	// and thus, only derived classes (wxCINT, wxUnderC and wxLua) can
	// be instantiated; you can access them using their static Get()
	// functions: wxCINT::Get(), wxUnderC::Get() ....	

	//! Initializes the script interpreter.
	static bool Init(bool bCINT = TRUE, bool bUnderC = TRUE, 
						bool bLua = TRUE, bool bPython = TRUE);

	//! Deallocates the script interpreter.
	static void Cleanup();

	//! Returns TRUE if the script interpreter is ready to work.
	static bool areAllReady();

	//! Returns the description of the last occured error (an empty
	//! string if there were no errors).
	static wxString GetLastErr()	{ return m_strLastErr; }

	//! Returns the list of the functions currently recognized by the interpreter.
	static void GetTotalFunctionList(wxScriptFunctionArray &);

	//! Load the given scriptfile and then returns an instance to the
	//! scriptfile wrapper, or NULL if the file couldn't be loaded.
	static wxScriptFile *Load(const wxString &filename, 
			wxScriptFileType type = wxRECOGNIZE_FROM_EXTENSION);

public:		// virtual functions

	//! Returns TRUE if this script interpreter is ready to work.
	virtual bool isReady() const = 0;

	//! Returns the list of the functions currently recognized by the interpreter.
	virtual void GetFunctionList(wxScriptFunctionArray &) const = 0;

	//! Returns version info about this interpreter or a wxEmptyString if
	//! this interpreter does not supply any version info.
	//! The exact form of the string returned is interpreter-dependent,
	//! but it is usually given as "INTERPRETER_NAME VERSION_STRING".
	virtual wxString GetVersionInfo() const = 0;
};


// to avoid a lot of repetitions
#define wxSCRIPTFNC_IMPLEMENT_CLONE(x)            \
	virtual wxScriptFunction *Clone() const {     \
		wxScriptFunction *newf = new x();         \
		newf->DeepCopy(this);                     \
		return newf; }    

#endif		// SCRIPT_H

