///////////////////////////////////////////////////////////////////////////////
// Name:        gziptest.cpp
// Purpose:     Test program for wxGzipInputStream and wxGzipOutputStream
// Author:      Mike Wetherell
// RCS-ID:      $Id: gziptest.cpp,v 1.1 2004-04-30 23:18:47 chiclero Exp $
// Copyright:   (c) 2003-2004 Mike Wetherell
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma implementation
    #pragma interface
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/wfstream.h"
#include "wx/filename.h"
#include "wx/txtstrm.h"
#include <iostream>

#include "wx/gzstream.h"

using namespace std;

// The application class
//
class GzipTestApp : public wxAppConsole
{
public:
    GzipTestApp() : m_fails(0) { }
    int OnRun();
private:
    void Verify(wxInputStream &in1, wxInputStream &in2);
    void Fail(const wxString& msg) { m_fails++; Info(msg); }
    void Info(const wxString& msg) const { cout << msg.mb_str() << endl; }
    int m_fails;
};

IMPLEMENT_APP_CONSOLE(GzipTestApp)

// Run - write a gzip file and read it back.
//
int GzipTestApp::OnRun()
{
    Info(_T("*** Testing gzip streams ***\n"));

    const wxString testname = _T("testdata");
    const wxString gzipname = testname + _T(".gz");

    // write some random test data to a testfile
    Info(_T("Writing random test data to ") + testname + _T("..."));
    {
        wxFFileOutputStream outstream(testname);
        wxTextOutputStream textout(outstream);

        for (int i = 0; i < 1000 && outstream.Ok(); i++)
            textout << rand() << rand() << rand() << rand() << _T("\n");

        Info(_T("    Done"));
    }

    wxFileName fn(testname);
    wxDateTime dt = fn.GetModificationTime();
    wxFFileInputStream instream(testname);

    // try writing a gzip file
    Info(_T("Writing ") + gzipname + _T(" using wxGzipOutputStream..."));
    {
        wxFFileOutputStream outstream(gzipname);
        wxGzipOutputStream gzip(outstream, testname, dt);

        if (!gzip.Write(instream))
            Fail(_T("    Failure"));
        else
            Info(_T("    Success"));
    }

    // try reading the gzip file
    Info(_T("Reading ") + gzipname + _T(" using wxGzipInputStream..."));
    {
        instream.SeekI(0);
        wxFFileInputStream instream2(gzipname);
        wxGzipInputStream gzip(instream2);
        Verify(instream, gzip);

        if (gzip.GetName() != fn.GetFullName())
            Fail(gzipname + _T(" contains incorrect filename: ")
                    + gzip.GetName());
        if (dt.IsValid() && gzip.GetDateTime() != dt)
            Fail(gzipname + _T(" contains incorrect timestamp: ")
                    + gzip.GetDateTime().Format());
    }

#ifdef __UNIX__
    // then verify it using gzip program if it is in the path
    Info(_T("Reading ") + gzipname + _T(" using gzip program..."));
    wxFFile file(popen((_T("gzip -d -c ") + gzipname).mb_str(), "r"));
    if (file.fp()) {
        wxFFileInputStream instream2(file);
        instream.SeekI(0);
        Verify(instream, instream2);
        pclose(file.fp());
        file.Detach();
    }

    // try reading a gzip created by gzip program
    Info(_T("Reading output of gzip program using wxGzipInputStream..."));
    file.Attach(popen((_T("gzip -c ") + testname).mb_str(), "r"));
    if (file.fp()) {
        wxFFileInputStream instream2(file);
        wxGzipInputStream gzip(instream2);
        instream.SeekI(0);
        Verify(instream, gzip);
        pclose(file.fp());
        file.Detach();
    }
#endif

    if (m_fails) {
        Info(_T("\n--- FAIL ---"));
        return EXIT_FAILURE;
    } else {
        Info(_T("\n--- SUCCESS ---"));
        return EXIT_SUCCESS;
    }
}

// Reads two input streams and verifies that they are the same (and non-emtpy)
//
void GzipTestApp::Verify(wxInputStream &in1, wxInputStream &in2)
{
    if (!in1 || !in2) {
        Fail(_T("    Can't verify"));
        return;
    }

    const int BUFSIZE = 8192;
    wxCharBuffer buf1(BUFSIZE);
    wxCharBuffer buf2(BUFSIZE);
    bool none = true;

    for (;;)
    {
        int n1 = in1.Read(buf1.data(), BUFSIZE).LastRead();
        int n2 = in2.Read(buf2.data(), BUFSIZE).LastRead();

        if (n1 != n2 || (n1 && memcmp(buf1, buf2, n1) != 0) || (!n1 && none)) {
            Fail(_T("    Failure"));
            break;
        }

        if (!n1) {
            Info(_T("    Success"));
            break;
        }

        none = false;
    }

    while (in1.IsOk())
        in1.Read(buf1.data(), BUFSIZE);
    while (in2.IsOk())
        in2.Read(buf2.data(), BUFSIZE);
}
