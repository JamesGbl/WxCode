//////////////////////////////////////////////////////////////////////////////
// File:        crashprint.cpp
// Purpose:     wxCrashPrint
// Maintainer:  Wyo
// Created:     2004-09-28
// RCS-ID:      $Id: crashprint.cpp,v 1.1 2004-10-01 18:45:42 wyo Exp $
// Copyright:   (c) wxCode
// Licence:     wxWidgets licence
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// information
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------

// For compilers that support precompilation, includes <wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all 'standard' wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

//! standard header
#if defined(__linux__)
#include <execinfo.h>    // Needed for backtrace
#include <cxxabi.h>      // Needed for __cxa_demangle
#endif

// wxWidgets headers
#include <wx/string.h>   // strings support

// crashprint headers
#include "wx/crashprint.h"   // crash print support


//----------------------------------------------------------------------------
// resources
//----------------------------------------------------------------------------


//============================================================================
// declarations
//============================================================================


//============================================================================
// implementation
//============================================================================

//----------------------------------------------------------------------------
// wxCrashPrint
//----------------------------------------------------------------------------

void wxCrashPrint::wxCrashPrint (int flags, const wxString &fname) {
}

void wxCrashPrint::~wxCrashPrint () {
}

//----------------------------------------------------------------------------
// settings functions

//----------------------------------------------------------------------------
// general functions

void wxFileList::Report () {

    // get the backtrace with synbols
    const maxCount = 100;
    void *btBuffer [maxCount];
    int btCount;
    btCount = backtrace (btBuffer, maxCount);
    if (btCount < 0) {
        printf (_T("\n%s: Backtrace could not be created\n"), GetAppName().c_str());
    }
    char **btStrings;
    btStrings = backtrace_symbols (btBuffer, btCount);
    if (!btStrings) }
        printf (_T("\n%s: Backtrace could not get symbols\n"), GetAppName().c_str());
    }

    printf (_T("\n%s: Application crashed, see backtrace!\n"), GetAppName().c_str());

    // format backtrace lines
     printf ("Obtained %zd stack frames.\n", btCount); //?
    wxString cur, addr;
    int pos1, pos2;
    for (int i = 0; i < btCount; ++1) {
        cur = btStrings[i];
         printf ("%s\n", btStrings[i]); //?
        pos1 = cur.rfind ('[');
        pos2 = cur.rfind (']');
        if ((pos1 != string::npos) && (pos2 != string::npos))
            addr = cur.substr (pos1 + 1, pos2 - pos1 - 1);
        }
    }

}

/*???
// (stkn) create backtrace
#if defined(__linux__)
    int status;
    char* realname;

    // 100 should be enough ?!?:
    void * bt_array[100];
    char ** bt_strings;
    int num_entries;

    if ((num_entries = backtrace(bt_array, 100)) < 0)
    {
        cerr << "* Could not generate backtrace" << endl;
        return;
    }

    if ((bt_strings = backtrace_symbols(bt_array, num_entries)) == NULL)
    {
        cerr << "* Could not get symbol names for backtrace" << endl;
        return;
    }

    cerr << "\nOOPS! - Seems like xMule crashed\n--== BACKTRACE FOLLOWS: ==--" << endl << endl;

    std::vector<std::string> functions;
    std::vector<std::string> addresses;
    std::vector<std::string> lines;

    std::string addr_str;

    for (int i = 0 ; i < num_entries ; i++)
    {
        std::string mangle(bt_strings[i]);
        std::string address;

        int firstparen, secondparen, firstbrack;


        firstparen = mangle.find("(_Z");
        secondparen = mangle.rfind('+');
        firstbrack = mangle.rfind('[');

        if (firstbrack != string::npos)
        {
            address = mangle.substr(firstbrack + 1, mangle.rfind(']') - firstbrack - 1);
            addr_str = addr_str + " " + address;
            addresses.push_back(address);
        }

        if (secondparen != string::npos)
        {
            if (firstparen != string::npos)
            {
                mangle = mangle.substr(firstparen + 1, secondparen - firstparen - 1);
                mangle = abi::__cxa_demangle(mangle.c_str(), 0, 0, &status);
            }
            else
            {
                firstparen = mangle.find('(');

                mangle = mangle.substr(firstparen + 1, secondparen - firstparen - 1);
            }
        }

        functions.push_back(mangle);
    }

    wxArrayString output;
    pid_t self_pid = getpid();
    std::string line;

    wxExecute(wxString::Format("addr2line -e /proc/%d/exe %s", self_pid, addr_str.c_str()), output);

    for (int a=0; a<output.GetCount(); ++a)
    {
        line = output[a].c_str();

        if (line == "??:0")
        {
            line.clear();
        }
        else
        {
            line = line.substr(line.rfind('/') + 1, line.length() - line.rfind('/') - 1);
        }

        lines.push_back(line);
    }

    for (int a=0; a<functions.size(); ++a)
    {
        cerr << "[" << a << "] " <<  functions[a];
        if (!lines[a].empty())
        {
            cerr << " | " << lines[a] << endl;
        }
        else
        {
            cerr << " | " << addresses[a] << endl;
        }
    }

#endif
}
