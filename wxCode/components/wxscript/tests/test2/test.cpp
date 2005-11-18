///////////////////////////////////////////////////////////////////////
// Test - a little wxScript classes usage sample                     //
// Copyright (C) 2003 by Francesco Montorsi                          //
//                                                                   //
// This program is free software; you can redistribute it and/or     //
// modify it under the terms of the GNU General Public License       //
// as published by the Free Software Foundation; either version 2    //
// of the License, or (at your option) any later version.            //
//                                                                   //
// This program is distributed in the hope that it will be useful,   //
// but WITHOUT ANY WARRANTY; without even the implied warranty of    //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the      //
// GNU General Public License for more details.                      //
//                                                                   //
// You should have received a copy of the GNU General Public         //
// License along with this program; if not, write to the Free        //
// Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,   //
// MA 02111-1307, USA.                                               //
//                                                                   //
// For any comment, suggestion or feature request, please contact    //
// frm@users.sourceforge.net                                         //
//                                                                   //
///////////////////////////////////////////////////////////////////////


// To be correctly compiled this test program needs many libraries:
//
// ucc12.lib           for the UnderC interpreter
// libcint.lib         for the CINT interpreter
// tolua.lib 
//  + lua.lib 
//  + lualib.lib       for the LUA interpreter & TOLUA utility
// pythonXX.lib        for the Python interpreter
// perlXX.lib          for the Perl interpreter
//
// and wxWidgets libraries
//
// If you want to disable one or more of these dependencies you can use
// the USE_XXXXX options when compiling the sample using the makefiles
// of the BUILD folder.



// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include <wx/defs.h>		// wxWidgets includes
#include <wx/string.h>
#include <wx/file.h>
#include <wx/app.h>
#include <wx/log.h>
#include <wx/cmdline.h>
#include <wx/filename.h>

#include <stdio.h>			// miscellaneous includes
#ifdef __VISUALC__
#include <conio.h>			// miscellaneous includes
#endif

#ifdef wxSCRIPT_USE_LUA
#include <tolua.h>
#endif

#include <wx/script.h>		// our interpreter...
#include <wx/sclua.h>		// our interpreter...
#include <wx/sccint.h>		// our interpreter...
#include <wx/scunderc.h>		// our interpreter...
#include <wx/scpython.h>		// our interpreter...
#include <wx/scperl.h>		// our interpreter...
#include "test.h"



// ----------------------------------------------------------------------------
// defines
// ----------------------------------------------------------------------------

// these will be used to perform various types of tests:
#ifdef wxSCRIPT_USE_LUA
	#define TEST_LUA
#endif
#ifdef wxSCRIPT_USE_PYTHON
	#define TEST_PYTHON
#endif
#ifdef wxSCRIPT_USE_CINT
	#define TEST_CINT
#endif
#ifdef wxSCRIPT_USE_UNDERC
	#define TEST_UNDERC
#endif
#ifdef wxSCRIPT_USE_PERL
	#define TEST_PERL
#endif



#ifdef TEST_LUA

	// for advanced testing
	//TOLUA_API int tolua_toexport_open(lua_State *);
#endif


// without this pragma, the stupid compiler precompiles #defines below so that
// changing them doesn't "take place" later!
#ifdef __VISUALC__
    #pragma hdrstop
#endif


// first of all, decide if we can use the system...
#if defined(__VISUALC__)
	#define mcDETECT_MEMORY_LEAKS
#endif

#ifdef mcDETECT_MEMORY_LEAKS

	// "crtdbg.h" is included only with MSVC++ and Borland, I think...
	// "stackwalker.h" instead, contains a set of stack walker functions
	// created by Jochen Kalmbach (thanks !!!) which allow to read the
	// intercept unhandled exceptions and memory-leaks. 
	// To be used, the file must be part of the project; this is why
	// it's contained (with its CPP counterpart) in the folder of this
	// test program. Anyway,  you can find it also online at:
	//     http://www.codeproject.com/tools/leakfinder.asp
	#include <crtdbg.h>
	//#include <windows.h>
	#include "stackwalker.h"

	// define some useful macros
	#define new			new(_NORMAL_BLOCK, THIS_FILE, __LINE__)

	#define mcDUMP_ON_EXIT				{ _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF); }
	#define mcSTART_DETECTION			{ InitAllocCheck(ACOutput_Advanced, FALSE, FALSE); }
	#define mcEND_DETECTION				{ DeInitAllocCheck(); }
	#define mcEND_DETECTION_AND_DUMP	{ DeInitAllocCheck(); _CrtDumpMemoryLeaks(); }
	
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;


	// this little class is used to access Stackwalker functions
	// without changing a line of code...
	class mcLeakDetector {

	public:
		mcLeakDetector() { mcSTART_DETECTION; mcDUMP_ON_EXIT; }
		~mcLeakDetector() { mcEND_DETECTION; }
	};

	// ...infact, instancing a STATIC mcLeakDetector class, we
	// can start memory-leak detection at the very beginning of
	// the program (when the main() or winmain() has not been
	// called yet, that is, when the framework is creating the
	// static variables of the program) and end it at the very
	// end of the program (when, after the main() or winmain(),
	// the framework removes the static variables).
	//static mcLeakDetector detector;


	// this little class instead is used to generate a 'memdiff'
	// of the heap state from its creation to its destruction.
	class mcFindMemoryLeaks
	{
		  _CrtMemState m_checkpoint;

	public:

		  mcFindMemoryLeaks()
		  {
				_CrtMemCheckpoint(&m_checkpoint);
		  };

		  ~mcFindMemoryLeaks()
		  {
				_CrtMemState checkpoint;
				_CrtMemCheckpoint(&checkpoint);

				_CrtMemState diff;
				_CrtMemDifference(&diff, &m_checkpoint, &checkpoint);

				_CrtMemDumpStatistics(&diff);
				_CrtMemDumpAllObjectsSince(&diff);
		  };
	};

#endif


// ----------------------------------------------------------------------------
// entry point
// ----------------------------------------------------------------------------

void CallFnc1(const wxScriptFunctionArray &arr)
{
	// try to exec func1 with all the intepreters we are testing;
	// func1 should be defined as:
	//
	//          int xxxx_func1(char *str, int n)
	//
#ifdef TEST_CINT
	wxScriptFunction *fcint = arr.Get(wxT("cint_func1"));
#endif
#ifdef TEST_UNDERC
	wxScriptFunction *fuc = arr.Get(wxT("uc_func1"));
#endif
#ifdef TEST_LUA
	wxScriptFunction *flua = arr.Get(wxT("lua_func1"));
#endif
#ifdef TEST_PYTHON
	wxScriptFunction *fpy = arr.Get(wxT("py_func1"));
#endif
#ifdef TEST_PERL
	wxScriptFunction *fpl = arr.Get(wxT("pl_func1"));
#endif

	wxScriptVar result;
	wxScriptVar args[3];

	args[0] = wxScriptVar(wxT("char*"), wxT("try"));
	args[1] = wxScriptVar(wxT("int"), wxT("3"));
	args[2] = wxScriptVar();		// close the list with an empty variable

#ifdef TEST_CINT
	if (fcint) {

		// check this is the function we think
		if (fcint->Match(wxScriptTypeINT, 2, wxScriptTypePCHAR, wxScriptTypeINT)) {
			
			// ok, it's our function...
			fcint->Exec(result, args);
			wxPrintf(wxT(">%s('try', 3) returned %s\n"), fcint->GetName().c_str(),
				result.GetContentString().c_str());
		}
	}
#endif

#ifdef TEST_UNDERC
	if (fuc) {

		// check this is the function we think
		if (fuc->Match(wxScriptTypeINT, 2, wxScriptTypePCHAR, wxScriptTypeINT)) {
			
			// ok, it's our function...
			fuc->Exec(result, args);
			wxPrintf(wxT(">%s('try', 3) returned %s\n"), fuc->GetName().c_str(), 
				result.GetContentString().c_str());
		}
	}
#endif

#ifdef TEST_LUA
	if (flua) {

		// we cannot check if this is our function...
		if (!flua->Exec(result, args)) {
			
			wxPrintf(wxT("Execution failed: %s"), wxScriptInterpreter::GetLastErr().c_str());
			
		} else {
			
			wxPrintf(wxT(">%s('try', 3) returned %s\n"), flua->GetName().c_str(), 
				result.GetContentString().c_str());
		}
	}
#endif

#ifdef TEST_PYTHON
	if (fpy) {

		// we cannot check if this is our function...
		if (!fpy->Exec(result, args)) {
			
			wxPrintf(wxT("Execution failed: %s"), wxScriptInterpreter::GetLastErr().c_str());
			
		} else {
			
			wxPrintf(wxT(">%s('try', 3) returned %s\n"), fpy->GetName().c_str(), 
				result.GetContentString().c_str());
		}
	}
#endif

#ifdef TEST_PERL
	if (fpl) {

		// we cannot check if this is our function...
		if (!fpl->Exec(result, args)) {
			
			wxPrintf(wxT("Execution failed: %s"), wxScriptInterpreter::GetLastErr().c_str());
			
		} else {
			
			wxPrintf(wxT(">%s('try', 3) returned %s\n"), fpl->GetName().c_str(), 
				result.GetContentString().c_str());
		}
	}
#endif
}

void CallFnc2(const wxScriptFunctionArray &arr)
{
	// func2 should be defined as:
	//
	//          bool xxxx_func2(bool input)
	//
#ifdef TEST_CINT
	wxScriptFunction *fcint = arr.Get(wxT("cint_func2"));
#endif
#ifdef TEST_UNDERC
	wxScriptFunction *fuc = arr.Get(wxT("uc_func2"));
#endif
#ifdef TEST_LUA
	wxScriptFunction *flua = arr.Get(wxT("lua_func2"));
#endif
#ifdef TEST_PYTHON
	wxScriptFunction *fpy = arr.Get(wxT("py_func2"));
#endif
#ifdef TEST_PERL
	wxScriptFunction *fpl = arr.Get(wxT("pl_func2"));
#endif

	wxScriptVar result;
	wxScriptVar args[2];

	args[0] = wxScriptVar(wxT("bool"), wxT("true"));
	args[1] = wxScriptVar();

#ifdef TEST_CINT
	if (fcint) {

		// check this is the function we think
		if (fcint->Match(wxScriptTypeBOOL, 1, wxScriptTypeBOOL)) {
			
			// ok, it's our function...
			fcint->Exec(result, args);
			wxPrintf(wxT(">%s('true') returned %s\n"), fcint->GetName().c_str(),
				result.GetContentString().c_str());
		}
	}
#endif

#ifdef TEST_UNDERC
	if (fuc) {

		// check this is the function we think
		if (fcint->Match(wxScriptTypeBOOL, 1, wxScriptTypeBOOL)) {
			
			// ok, it's our function...
			fuc->Exec(result, args);
			wxPrintf(wxT(">%s('true') returned %s\n"), fuc->GetName().c_str(),
				result.GetContentString().c_str());
		}
	}
#endif

#ifdef TEST_LUA
	if (flua) {

		// we cannot check if this is our function...
		if (!flua->Exec(result, args)) {
			
			wxPrintf(wxT("Execution failed: %s"), wxScriptInterpreter::GetLastErr().c_str());
			
		} else {
			
			wxPrintf(wxT(">%s('true') returned %s\n"), flua->GetName().c_str(),
				result.GetContentString().c_str());
		}		
	}
#endif

#ifdef TEST_PYTHON
	if (fpy) {

		// we cannot check if this is our function...
		if (!fpy->Exec(result, args)) {
			
			wxPrintf(wxT("Execution failed: %s"), wxScriptInterpreter::GetLastErr().c_str());
			
		} else {
			
			wxPrintf(wxT(">%s('true') returned %s\n"), fpy->GetName().c_str(),
				result.GetContentString().c_str());
		}		
	}
#endif

#ifdef TEST_PERL
	if (fpl) {

		// we cannot check if this is our function...
		if (!fpl->Exec(result, args)) {
			
			wxPrintf(wxT("Execution failed: %s"), wxScriptInterpreter::GetLastErr().c_str());
			
		} else {
			
			wxPrintf(wxT(">%s('true') returned %s\n"), fpl->GetName().c_str(),
				result.GetContentString().c_str());
		}		
	}
#endif
}

void CallAdvanced(const wxScriptFunctionArray &arr)
{
	wxScriptFunction *flua = arr.Get(wxT("lua_advanced"));

	// this is the test object we will pass to our LUA function:
	// it should be able to modify it and return it...
	myClass test;
	test.set(FALSE);

	wxScriptVar result;
	wxScriptVar args[2];

	args[0] = wxScriptVar(wxT("myClass*"), (int *)(&test));
	args[1] = wxScriptVar();

	if (flua) {

		// we cannot check if this is our function...
		if (!flua->Exec(result, args)) {
			
			wxPrintf(wxT("Execution failed: %s"), wxScriptInterpreter::GetLastErr().c_str());
			
		} else {

			// if everything was right, result is a pointer to myClass * !!!
			if (!result.GetType().isPointer()) {
				wxPrintf(wxT("Invalid return value..."));
				return;
			}

			myClass *returned = (myClass *)result.GetPointer();
			//bool check = (returned == &test);
			
			wxPrintf(wxT(">%s(myClass(false)) returned myClass(%d)\n"), 
				flua->GetName().c_str(),
				returned->get());
		}		
	}

#ifdef wxSCRIPT_USE_UNDERC
#if 0
	// normally, to call a script function (whose you know name, return type
	// and argument list types) you should just write something like:
	const char *fncname = wxT("uc_func3");
	wxScriptFunction *myfnc = arr.Get(fncname);
	wxScriptVar ret;

	// exec that function
	bool bexec = (myfnc ? myfnc->Exec(ret, NULL) : FALSE);
	
	// get our real object pointer which can then be normally used in
	// the C++ code of this program...
	mcMathSystem *ms = (mcMathSystem *)ret.GetPointer();
	
	if (bexec && ms) {

		wxPrintf(wxT(">Called %s; the returned address is %s and the number of lines\n"),
			wxT(" contained in the returned mcMathSystem is %d.\n"),
			myfnc->GetName().c_str(), ret.GetContentString(), ms->GetLineCount());

	} else {

		wxPrintf(wxT(">Error calling the %s function !!!"), fncname);
	}
#endif
#endif
}

void MainTestSet()
{
	wxString basepath = wxGetCwd();
	 
	if (basepath.Right(5).IsSameAs(wxT("test2"), FALSE))
		basepath = basepath.Left(basepath.Len()-6);
	if (basepath.Right(5).IsSameAs(wxT("build"), FALSE))
		basepath = basepath.Left(basepath.Len()-6) + wxFileName::GetPathSeparator() + wxT("tests");
	
	// this basepath is used to load the script files
	basepath +=	wxFileName::GetPathSeparator();
	basepath += wxT("testscripts");
	basepath += wxFileName::GetPathSeparator();
	wxPrintf(wxT(">Base path is: '%s'\n"), basepath.c_str());

#ifdef TEST_CINT
	// load a CINT script file
	wxPrintf(wxT(">Loading the 'script1.cxx'...\n"));
	wxScriptFile *file1 = wxScriptInterpreter::Load(basepath + wxT("script1.cxx"));
	if (!file1) {
		wxPrintf(wxT("Load failed."));
		wxScriptInterpreter::Cleanup();
		return;
	}

	delete file1;
#endif

#ifdef TEST_UNDERC
	// load an UnderC script file
	wxPrintf(wxT(">Loading the 'script2.uc'...\n"));
	wxScriptFile *file2 = wxScriptInterpreter::Load(basepath + wxT("script2.uc"));
	if (!file2) {
		wxPrintf(wxT("Load failed."));
		wxScriptInterpreter::Cleanup();
		return;
	}

	delete file2;
#endif


#ifdef TEST_LUA
#if 0
	// before loading a Lua script file, open the ToExport package
	// created through the TOLUA program
	tolua_toexport_open(wxLua::Get()->m_state);
#endif
	// load a Lua script file
	wxPrintf(wxT(">Loading the 'script3.lua'...\n"));
	wxScriptFile *file3 = wxScriptInterpreter::Load(basepath + wxT("script3.lua"));
	if (!file3) {
		wxPrintf(wxT("\nLoad failed: %s"), wxScriptInterpreter::GetLastErr().c_str());		
		wxScriptInterpreter::Cleanup();
		return;
	}

	delete file3;
#endif

#ifdef TEST_PYTHON
	// load a python script file
	wxPrintf(wxT(">Loading the 'script4.py'...\n"));
	wxScriptFile *file4 = wxScriptInterpreter::Load(basepath + wxT("script4.py"));
	if (!file4) {
		wxPrintf(wxT("\nLoad failed: %s"), wxScriptInterpreter::GetLastErr().c_str());		
		wxScriptInterpreter::Cleanup();
		return;
	}

	delete file4;
#endif

#ifdef TEST_PERL
	// load a perl script file
	wxPrintf(wxT(">Loading the 'script5.pl'...\n"));
	wxScriptFile *file5 = wxScriptInterpreter::Load(basepath + wxT("script5.pl"));
	if (!file5) {
		wxPrintf(wxT("\nLoad failed: %s"), wxScriptInterpreter::GetLastErr().c_str());		
		wxScriptInterpreter::Cleanup();
		return;
	}

	delete file5;
#endif

	// get function list
	wxScriptFunctionArray arr;
	wxScriptInterpreter::GetTotalFunctionList(arr);
	wxPrintf(wxT(">Loaded %d functions:\n"), arr.GetCount());
	for (int i=0; i < arr.GetCount(); i++)
		wxPrintf(wxT("%s\n"), arr.GetName(i).c_str());

	// do some function calls with a lot of tests to be sure everything is okay
	CallFnc1(arr);
	CallFnc2(arr);
	//CallAdvanced(arr);

	// leave some space
	wxPrintf(wxT(">All tests completed.\n\n\n"));
}

int main(int, char **)
{
#ifdef __WXMSW__
	// first of all, do some checks
    wxApp::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "Test");
    wxInitializer initializer;
    if (!initializer) {
        fprintf(stderr, "Failed to initialize the wxWindows library, aborting.");
        return -1;
    }
#endif
	// now, type some help info...
	wxPrintf(wxT("\n\n"));
	wxPrintf(wxT(" wxScript test program\n"));
	wxPrintf(wxT(" -------------------------\n\n"));
	wxPrintf(wxT(" This is a little test program which runs some didactive tests \n"));
	wxPrintf(wxT(" about the wxScript and wxScript-related classes.\n\n"));

	// which interpreters do we have to test ?
	bool cint=FALSE, underc=FALSE, lua=FALSE, python=FALSE, perl=FALSE;
#ifdef TEST_CINT
	cint=TRUE;
#endif
#ifdef TEST_UNDERC
	underc=TRUE;
#endif
#ifdef TEST_LUA
	lua=TRUE;
#endif
#ifdef TEST_PYTHON
	python=TRUE;
#endif
#ifdef TEST_PERL
	perl=TRUE;
#endif

	// init
	wxScriptInterpreter::Init(cint, underc, lua, python, perl);
	wxPrintf(wxT(">I'm initializing the script interpreters...\n"));
	if (!wxScriptInterpreter::areAllReady()) {
		wxPrintf(wxT("Initialization failed."));
		wxScriptInterpreter::Cleanup();
		return 0;
	}

#if !defined(TEST_LUA) && !defined(TEST_PYTHON) && !defined(TEST_CINT) && !defined(TEST_UNDERC) && !defined(TEST_PERL)
	wxPrintf(wxT("No script interpreter to test ?\n"));
	return 0;
#endif

	wxPrintf(wxT(">For our tests, we are using:\n"));
#ifdef TEST_LUA
	wxPrintf(wxLua::Get()->GetVersionInfo() + wxT("\n"));
#endif
#ifdef TEST_PYTHON
	wxPrintf(wxPython::Get()->GetVersionInfo() + wxT("\n"));
#endif
#ifdef TEST_CINT
	wxPrintf(wxCINT::Get()->GetVersionInfo() + wxT("\n"));
#endif
#ifdef TEST_UNDERC
	wxPrintf(wxUnderC::Get()->GetVersionInfo() + wxT("\n"));
#endif
#ifdef TEST_PERL
	wxPrintf(wxPerl::Get()->GetVersionInfo() + wxT("\n"));
#endif
    
	// run some tests
	MainTestSet();

	// don't leak memory
	wxScriptInterpreter::Cleanup();

	// wait for user keypress
	wxPrintf(wxT("Press a key to continue...\n"));
#ifdef __VISUALC__
	while (!_kbhit());
#else
	getchar();
#endif

    // return a success value
    return 0;
}



