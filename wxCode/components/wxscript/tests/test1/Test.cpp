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


// To be correctly linked this test program needs many libraries:
//
// ucc12.lib           for the UnderC interpreter
// libcint.lib         for the CINT interpreter
// tolua.lib 
//  + lua.lib 
//  + lualib.lib       for the LUA interpreter & TOLUA utility
//
// and wxWidgets libraries
//
//
// If you want to disable one or more of these dependencies you can compile
// the wxscript.lib library defining one of the following symbols:
//
//   wxSCRIPT_NO_LUA
//   wxSCRIPT_NO_CINT
//   wxSCRIPT_NO_UNDERC
//   wxSCRIPT_NO_PYTHON
//
// If you disable one or more of the interpreters, then you will need
// to rebuilt this test program without one or more of the following
// defines:

#define TEST_LUA
//#define TEST_CINT		// currently disabled because it does not compile on my RH9
//#define TEST_UNDERC	// some implementation problems...
//#define TEST_PYTHON


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
#include <conio.h>			// miscellaneous includes
#include <wx/script.h>		// our interpreters...


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

	// define some useful macros
	#define new                 new(_NORMAL_BLOCK, THIS_FILE, __LINE__)
	#define mcDUMP_ON_EXIT		{ _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF); }

	
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;


	// this little class is used to access Stackwalker functions
	// without changing a line of code...
	class mcLeakDetector {

	public:
		mcLeakDetector() { mcDUMP_ON_EXIT; }
		~mcLeakDetector() {}
	};

	// ...infact, instancing a STATIC mcLeakDetector class, we
	// can start memory-leak detection at the very beginning of
	// the program (when the main() or winmain() has not been
	// called yet, that is, when the framework is creating the
	// static variables of the program) and end it at the very
	// end of the program (when, after the main() or winmain(),
	// the framework removes the static variables).
	static mcLeakDetector detector;

#endif


// ----------------------------------------------------------------------------
// entry point
// ----------------------------------------------------------------------------

void CallFnc1(const wxScriptFunctionArray &arr)
{
	// try to exec func1 with both CINT & UnderC
	// func1 should be defined as:
	//
	//          int xxxx_func1(char *str, int n)
	//
	wxScriptFunction *fcint = arr.Get(wxT("cint_func1"));
	wxScriptFunction *fuc = arr.Get(wxT("uc_func1"));
	wxScriptFunction *flua = arr.Get(wxT("lua_func1"));

	wxScriptVar result;
	wxScriptVar args[3];

	args[0] = wxScriptVar(wxT("char*"), wxT("try"));
	args[1] = wxScriptVar(wxT("int"), wxT("3"));
	args[2] = wxScriptVar();		// close the list with an empty variable

	if (fcint) {

		// check this is the function we think
		if (fcint->Match(wxScriptTypeINT, 2, wxScriptTypePCHAR, wxScriptTypeINT)) {
			
			// ok, it's our function...
			fcint->Exec(result, args);
			wxPrintf(wxT(">%s('try', 3) returned %s\n"), fcint->GetName().c_str(),
				result.GetContentString().c_str());
		}
	}

	if (fuc) {

		// check this is the function we think
		if (fuc->Match(wxScriptTypeINT, 2, wxScriptTypePCHAR, wxScriptTypeINT)) {
			
			// ok, it's our function...
			fuc->Exec(result, args);
			wxPrintf(wxT(">%s('try', 3) returned %s\n"), fuc->GetName().c_str(), 
				result.GetContentString().c_str());
		}
	}

	if (flua) {

		// we cannot check if this is our function...
		if (!flua->Exec(result, args)) {
			
			wxPrintf(wxT("Execution failed: %s"), wxScriptInterpreter::GetLastErr().c_str());
			
		} else {
			
			wxPrintf(wxT(">%s('try', 3) returned %s\n"), flua->GetName().c_str(), 
				result.GetContentString().c_str());
		}
	}
}

void CallFnc2(const wxScriptFunctionArray &arr)
{
	// try to exec func1 with both CINT & UnderC
	// func1 should be defined as:
	//
	//          bool xxxx_func2(bool input)
	//
	wxScriptFunction *fcint = arr.Get(wxT("cint_func2"));
	wxScriptFunction *fuc = arr.Get(wxT("uc_func2"));
	wxScriptFunction *flua = arr.Get(wxT("lua_func2"));

	wxScriptVar result;
	wxScriptVar args[2];

	args[0] = wxScriptVar(wxT("bool"), wxT("true"));
	args[1] = wxScriptVar();

	if (fcint) {

		// check this is the function we think
		if (fcint->Match(wxScriptTypeBOOL, 1, wxScriptTypeBOOL)) {
			
			// ok, it's our function...
			fcint->Exec(result, args);
			wxPrintf(wxT(">%s('true') returned %s\n"), fcint->GetName().c_str(),
				result.GetContentString().c_str());
		}
	}

	if (fuc) {

		// check this is the function we think
		if (fcint->Match(wxScriptTypeBOOL, 1, wxScriptTypeBOOL)) {
			
			// ok, it's our function...
			fuc->Exec(result, args);
			wxPrintf(wxT(">%s('true') returned %s\n"), fuc->GetName().c_str(),
				result.GetContentString().c_str());
		}
	}

	if (flua) {

		// we cannot check if this is our function...
		if (!flua->Exec(result, args)) {
			
			wxPrintf(wxT("Execution failed: %s"), wxScriptInterpreter::GetLastErr().c_str());
			
		} else {
			
			wxPrintf(wxT(">%s('true') returned %s\n"), flua->GetName().c_str(),
				result.GetContentString().c_str());
		}		
	}
}

void MainTestSet()
{
	wxString basepath = wxGetCwd();
	 
	if (basepath.Right(5).IsSameAs(wxT("test1"), FALSE))
		basepath = basepath.Left(basepath.Len()-6);
	if (basepath.Right(5).IsSameAs(wxT("build"), FALSE))
		basepath = basepath.Left(basepath.Len()-6) + wxFileName::GetPathSeparator() + wxT("tests");
		
	basepath +=	wxFileName::GetPathSeparator();
	basepath += wxT("testscripts");
	basepath += wxFileName::GetPathSeparator();
	
	wxPrintf(wxT(">Base path is: '%s'\n"), basepath.c_str());

	// init
	wxScriptInterpreter::Init(TRUE, TRUE, TRUE);
	wxPrintf(wxT(">I'm initializing the script interpreter...\n"));
	if (!wxScriptInterpreter::areAllReady()) {
		wxPrintf(wxT("Initialization failed."));
		wxScriptInterpreter::Cleanup();
		return;
	}

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

	// get function list
	wxScriptFunctionArray arr;
	wxScriptInterpreter::GetTotalFunctionList(arr);
	wxPrintf(wxT(">Loaded %d functions:\n"), arr.GetCount());
	for (int i=0; i < arr.GetCount(); i++)
		wxPrintf(wxT("%s\n"), arr.GetName(i).c_str());

	// do some function calls with a lot of tests to be sure everything is okay
	CallFnc1(arr);
	CallFnc2(arr);

	// leave some space
	wxPrintf(wxT(">Test completed.\n\n\n"));

	// do not leave memory leaks
	wxScriptInterpreter::Cleanup();
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
    
	// run some tests
	MainTestSet();

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



