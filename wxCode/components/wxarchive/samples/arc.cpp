////////////////////////////////////////////////////////////////////////////
// Name:        arc.cpp
// Purpose:     Examples for archive classes
// Author:      Mike Wetherell
// RCS-ID:      $Id: arc.cpp,v 1.5 2004-07-17 14:31:16 chiclero Exp $
// Copyright:   (c) Mike Wetherell
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/archive.h"
#include "wx/ffile.h"
#include "wx/filename.h"
#include "wx/wfstream.h"
#include "wx/ptr_scpd.h"
#include "wx/dir.h"

#include <locale.h>
#include <iostream>
#include <iomanip>
#include <map>

// needed for setmode and O_BINARY
#if defined __WXMSW__ && \
    (!defined __STDC_VERSION__ || __STDC_VERSION__ < 199901L)
#   include <fcntl.h>
#   include <io.h>
#endif

// some smart pointers
wxDEFINE_SCOPED_PTR_TYPE(wxFFileInputStream);
wxDEFINE_SCOPED_PTR_TYPE(wxFFileOutputStream);
wxDEFINE_SCOPED_PTR_TYPE(wxArchiveInputStream);
wxDEFINE_SCOPED_PTR_TYPE(wxArchiveOutputStream);
wxDEFINE_SCOPED_PTR_TYPE(wxArchiveEntry);
wxDEFINE_SCOPED_PTR_TYPE(wxArchiveClassFactory);

// a map for the list of files to process
typedef std::map<wxString, wxString> NameMap;


/////////////////////////////////////////////////////////////////////////////
// The application class

class ArcApp : public wxAppConsole
{
public:
    ArcApp() :
        m_info(&std::cout), m_filter(false),
        m_validCmd(false), m_help(false) { } 

    bool OnInit() { /*setlocale(LC_ALL, "");*/ return true; }
    int OnRun();

    // the examples
    bool Create();
    bool Extract();
    bool List();
    bool Remove();
    bool Rename();
    bool Add();

    // make a class factory
    bool MakeFactory();

    // make m_files the list of files to process (for 'create' or 'add')
    void MakeFileList(NameMap& files,
                      const wxString& exclude1,
                      const wxString& exclude2);

    // commit or rollback changes
    bool Commit(bool commit);

    // data members used by the examples
    wxString m_archive;                 // the archive file name
    wxString m_output;                  // the output archive file name
    wxArrayString m_args;               // any additional command line args
    wxArchiveClassFactoryPtr m_factory; // the class factory
    wxFFileInputStreamPtr m_in;         // the input stream
    wxFFileOutputStreamPtr m_out;       // the output stream
    std::ostream *m_info;               // where to send progress info
    bool m_filter;                      // filter stdin to stdout?

private:
    enum InputOutput { I = 1, O = 2, IO = I | O };

    // command line processing...
    bool IsCmd(const wxString& cmd,
               InputOutput inputOutput,
               const wxString& params,
               const wxChar *description);

    wxString m_cmd;         // the command
    wxString m_availCmds;   // usage of all the commands
    bool m_validCmd;        // true a valid command was given
    wxString m_errMsg;      // command line error message
    bool m_help;            // must display help and exit
};


IMPLEMENT_APP_CONSOLE(ArcApp)


/////////////////////////////////////////////////////////////////////////////
// Create

bool ArcApp::Create()
{
    wxArchiveOutputStreamPtr arc(m_factory->NewStream(*m_out));

    // turn m_args into a list of files, i.e. expand wildcards, recurse
    // directories and remove duplicates
    NameMap files;
    MakeFileList(files, m_filter ? wxString() : m_archive, m_output);

    for (NameMap::iterator it = files.begin(); it != files.end(); ++it) {
        wxString name = it->second;

        *m_info << "adding " << name.mb_str() << std::endl;

        if (wxDirExists(name)) {
            wxDateTime dt = wxFileName(name, _T("")).GetModificationTime();
            if (!arc->PutNextDirEntry(name, dt))
                return false;
        }
        else {
            wxFFileInputStream in(name);

            if (in.Ok()) {
                wxDateTime dt = wxFileName(name).GetModificationTime();
                if (!arc->PutNextEntry(name, dt, in.GetSize()) ||
                        !arc->Write(in) || !in.Eof())
                    return false;
            }
        }
    }

    return arc->Close();
}


/////////////////////////////////////////////////////////////////////////////
// Extract - extracts the contents of an archive
//
// m_args[0] may contain a pattern, and if so only files matching are
// extracted.

bool ArcApp::Extract()
{
    wxArchiveInputStreamPtr arc(m_factory->NewStream(*m_in));
    wxArchiveEntryPtr       entry(arc->GetNextEntry());

    for (; entry.get(); entry.reset(arc->GetNextEntry())) {
        if (!m_args.empty() && !entry->GetName().Matches(m_args[0]))
            continue;
        *m_info << "extracting " << entry->GetName().mb_str() << std::endl;

        wxFileName fn(entry->GetName());

        if (entry->IsDir()) {
            fn.Mkdir(0777, wxPATH_MKDIR_FULL);
        } else {
            wxFileName::Mkdir(fn.GetPath(), 0777, wxPATH_MKDIR_FULL);

            wxFFileOutputStream out(entry->GetName());
            if (!out || !out.Write(*arc) || !arc->Eof())
                return false;
        }

#ifdef __WXMSW__
        if (!entry->IsDir()) {
#endif
            wxDateTime dt = entry->GetDateTime();
            fn.SetTimes(NULL, &dt, NULL);
#ifdef __WXMSW__
        }
#endif
    }

    return arc->Eof();
}


/////////////////////////////////////////////////////////////////////////////
// List - shows how to retreive all the meta data for the archive entries

bool ArcApp::List()
{
    wxArchiveInputStreamPtr arc(m_factory->NewStream(*m_in));
    wxArchiveEntryPtr       entry(arc->GetNextEntry());

    for (; entry.get() && arc->CloseEntry(); entry.reset(arc->GetNextEntry()))
        if (m_args.empty() || entry->GetName().Matches(m_args[0]))
            *m_info
                << std::setw(9) << entry->GetSize() << " "
                << entry->GetDateTime().Format(_T("%x %X")).mb_str() << " "
                << entry->GetName().mb_str() << std::endl;

    return arc->Eof();
}


/////////////////////////////////////////////////////////////////////////////
// Remove - delete entries from an archive
//
// Copies all the entries from the old archive to a new one, leaving out
// any matching the deletion pattern.

bool ArcApp::Remove()
{
    wxString pattern = m_args[0];

    wxArchiveInputStreamPtr  arc(m_factory->NewStream(*m_in));
    wxArchiveOutputStreamPtr outarc(m_factory->NewStream(*m_out));
    wxArchiveEntryPtr        entry(arc->GetNextEntry());

    outarc->SetExtra(arc->GetExtra());

    for (; entry.get(); entry.reset(arc->GetNextEntry()))
        if (entry->GetName().Matches(pattern))
            *m_info << "removing " << entry->GetName().mb_str() << std::endl;
        else
            if (!outarc->CopyEntry(entry.release(), *arc))
                return false;

    return arc->Eof() && outarc->Close();
}


/////////////////////////////////////////////////////////////////////////////
// Rename an entry
//
// Copies all the entries from the old archive to a new one. Changes
// can be made to an entry's meta data during this process, in this case
// changing the name.

bool ArcApp::Rename()
{
    wxString from = m_args[0];
    wxString to = m_args[1];

    wxArchiveInputStreamPtr  arc(m_factory->NewStream(*m_in));
    wxArchiveOutputStreamPtr outarc(m_factory->NewStream(*m_out));
    wxArchiveEntryPtr        entry(arc->GetNextEntry());

    outarc->SetExtra(arc->GetExtra());

    for (; entry.get(); entry.reset(arc->GetNextEntry())) {
        if (entry->GetName() == from) {
            *m_info << "renaming '" << from.mb_str()
                    << "' to '" << to.mb_str() << "'" << std::endl;
            entry->SetName(to);
        }

        if (!outarc->CopyEntry(entry.release(), *arc))
            return false;
    }

    return arc->Eof() && outarc->Close();
}


/////////////////////////////////////////////////////////////////////////////
// Add - add files to an archive
//
// Update an existing archive with a list of new or replacement files.

bool ArcApp::Add()
{
    // turn m_args into a list of files, i.e. expand wildcards, recurse
    // directories and remove duplicates
    NameMap files;
    MakeFileList(files, m_filter ? wxString() : m_archive, m_output);

    wxArchiveInputStreamPtr  arc(m_factory->NewStream(*m_in));
    wxArchiveOutputStreamPtr outarc(m_factory->NewStream(*m_out));
    wxArchiveEntryPtr        entry(arc->GetNextEntry());

    outarc->SetExtra(arc->GetExtra());

    for (;;) {
        bool keep;
        NameMap::iterator it;

        if (!entry.get()) {
            if (files.empty())
                break;
            it = files.begin();
            keep = false;
        } else {
            it = files.find(entry->GetInternalName());
            keep = true;
        }

        if (it != files.end()) {
            wxString name = it->second;
            files.erase(it);

            if (wxDirExists(name)) {
                *m_info << "adding " << name.mb_str() << std::endl;
                wxDateTime dt = wxFileName(name, _T("")).GetModificationTime();
                if (!outarc->PutNextDirEntry(name, dt))
                    return false;
                keep = false;
            } else {
                wxFFileInputStream file(name);

                if (file.Ok()) {
                    *m_info << "adding " << name.mb_str() << std::endl;
                    wxDateTime dt = wxFileName(name).GetModificationTime();
                    if (!outarc->PutNextEntry(name, dt, file.GetSize()) ||
                            !outarc->Write(file) || !file.Eof())
                        return false;
                    keep = false;
                }
            }
        }

        if (keep)
            if (!outarc->CopyEntry(entry.release(), *arc))
                return false;

        if (entry.get() || keep)
            entry.reset(arc->GetNextEntry());
    }

    return arc->Eof() && outarc->Close();
}


/////////////////////////////////////////////////////////////////////////////
// wxDirTraverser class used by MakeFileList below to recurse directories

class DirTraverser : public wxDirTraverser
{
public:
    DirTraverser(NameMap& files,
                 wxArchiveEntry& entry,
                 const wxString& exclude1,
                 const wxString& exclude2)
    :   m_files(files),
        m_entry(entry),
        m_exclude1(exclude1),
        m_exclude2(exclude2)
    { }

    virtual ~DirTraverser() { }

    wxDirTraverseResult OnFile(const wxString& name)
    {
        m_entry.SetName(name);
        wxString key = m_entry.GetInternalName();

        // exclude the files given in m_exclude# plus any files who's name
        // would degenerate to "" in the archive e.g. "/" or "." for zip or tar
        if (!key.empty() && m_exclude1 != name && m_exclude2 != name) {
            NameMap::iterator it = m_files.lower_bound(key);

            if (it != m_files.end() && it->first == key)
                wxLogWarning(_("ignoring duplicate file '%s'"), name.c_str());
            else
                m_files.insert(it, std::make_pair(key, name));
        }

        return wxDIR_CONTINUE;
    }

    wxDirTraverseResult OnDir(const wxString& name)
    {
        return OnFile(name);
    }

    wxDirTraverseResult OnOpenError(const wxString& name)
    {
        wxLogSysError(_T("can't open directory '%s'"), name.c_str());
        return wxDIR_IGNORE;
    }

private:
    NameMap& m_files;
    wxArchiveEntry& m_entry;
    wxFileName m_exclude1;
    wxFileName m_exclude2;
};


/////////////////////////////////////////////////////////////////////////////
// Make the list of files to process
//
// Populates 'files' with the files to process given 'FILES...' in 'm_args'
//  - expands wildcards on Windows
//  - recurses directories
//  - removes duplicates

void ArcApp::MakeFileList(
    NameMap& files,
    const wxString& exclude1,
    const wxString& exclude2)
{
    wxArchiveEntryPtr entry(m_factory->NewEntry());
    DirTraverser traverser(files, *entry, exclude1, exclude2);

    for (size_t i = 0; i < m_args.size(); ++i) {
#ifndef __WXMSW__
        wxString name = m_args[i];
#else
        // on Windows expand wildcards
        wxString name = wxFindFirstFile(m_args[i], 0);
        if (name.empty())
            wxLogError(_T("file not found '%s'"), m_args[i].c_str());
        while (!name.empty()) {
#endif
            // if the name is a directory recursively add all it's contents
            if (wxDirExists(name)) {
                if (traverser.OnDir(name) == wxDIR_CONTINUE) {
                    wxDir dir(name);
                    if (dir.IsOpened())
                        dir.Traverse(traverser);
                }
            } else {
                traverser.OnFile(name);
            }
#ifdef __WXMSW__
            name = wxFindNextFile();
        }
#endif
    }

    if (files.empty())
        wxLogWarning(_T("nothing to do"));
}


/////////////////////////////////////////////////////////////////////////////
// Make a class factory using the archive file's extension.
// e.g. for 'test.zip' creates wxZipClassFactory.

bool ArcApp::MakeFactory()
{
    wxString type = m_archive.AfterLast(_T('.'));
    wxString name = _T("wx") + type.Left(1).Upper() +
                    type.Mid(1).Lower() + _T("ClassFactory");

    wxObject *pObj = wxCreateDynamicObject(name);
    wxArchiveClassFactory *pcf = wxDynamicCast(pObj, wxArchiveClassFactory);

    if (pcf) {
        m_factory.reset(pcf);
    } else {
        wxLogError(_T("can't handle '%s' archives"), type.c_str());
        delete pObj;
    }

    return pcf != NULL;
}


/////////////////////////////////////////////////////////////////////////////
// Commit or rollback changes

bool ArcApp::Commit(bool commit)
{
    if (!m_output.empty()) {
        bool exists = wxFileExists(m_archive);

        if (commit) {
            if (exists) {
                *m_info << "commiting changes\n";
                wxRemove(m_archive);
            }
            if (wxRename(m_output, m_archive) == -1) {
                wxLogSysError(_T("can't commit '%s'"), m_archive.c_str());
                return false;
            }
        } else {
            if (exists)
                *m_info << "rolling back changes\n";
            wxRemove(m_output);
        }
    }

    return commit;
}


/////////////////////////////////////////////////////////////////////////////
// Windows workaround, seeking returns success on pipes but doesn't work

#ifdef __WXMSW__
bool IsPipe(FILE *fp) {
    return fp != NULL &&
        GetFileType((HANDLE)_get_osfhandle(fileno(fp))) == FILE_TYPE_PIPE;
}

class FFileOutputStream : public wxFFileOutputStream
{
public:
    FFileOutputStream(const wxString& name) :
        wxFFileOutputStream(name) { m_pipe = IsPipe(m_file->fp()); }
    FFileOutputStream(FILE *fp) :
        wxFFileOutputStream(fp), m_pipe(IsPipe(fp)) { }

    off_t SeekO(off_t pos, wxSeekMode mode = wxFromStart) {
        return m_pipe ? wxInvalidOffset : wxFFileOutputStream::SeekO(pos, mode);
    }
    off_t TellO() const {
        return m_pipe ? wxInvalidOffset : wxFFileOutputStream::TellO();
    }

private:
    bool m_pipe;
};

class FFileInputStream : public wxFFileInputStream
{
public:
    FFileInputStream(const wxString& name) :
        wxFFileInputStream(name) { m_pipe = IsPipe(m_file->fp()); }
    FFileInputStream(FILE *fp) :
        wxFFileInputStream(fp), m_pipe(IsPipe(fp)) { }

    off_t SeekI(off_t pos, wxSeekMode mode = wxFromStart) {
        return m_pipe ? wxInvalidOffset : wxFFileInputStream::SeekI(pos, mode);
    }
    off_t TellI() const {
        return m_pipe ? wxInvalidOffset : wxFFileInputStream::TellI();
    }

private:
    bool m_pipe;
};
#else
typedef wxFFileOutputStream FFileOutputStream;
typedef wxFFileInputStream FFileInputStream;
#endif


/////////////////////////////////////////////////////////////////////////////
// The rest is just plumbing

int ArcApp::OnRun()
{
    wxString progname;
    m_validCmd = false;
    m_help = argc < 2;
    bool ok = false;
    bool dashdash = false;

    if (*argv && **argv)
        progname = wxFileName(*argv).GetName();
    else
        progname = _T("arc");

    // parse the command line
    for (int i = 1; i < argc; ++i) {
        wxString arg = argv[i];

        if (!dashdash) {
            if (arg == _T("--")) {
                dashdash = true;
                continue;
            } else {
                if (arg == _T("--help") || arg == _T("-h") || arg == _T("/?")) {
                    m_help = true;
                    continue;
                } else {
                    if (*arg == _T('-')) {
                        m_errMsg = _T("invalid option '") + arg + _T("'");
                        break;
                    }
                }
            }
        }
        if (m_cmd.empty())
            m_cmd = arg;
        else if (m_archive.empty())
            m_archive = arg;
        else
            m_args.push_back(arg);
    }

    if (m_errMsg.empty() && !m_archive.empty() && !MakeFactory())
        return EXIT_FAILURE;

    m_filter = m_archive.find(_T('.')) == wxString::npos;

    // dispatch the command, and build up help information
    if (IsCmd(_T("add"),    IO, _T("FILES..."),  _T("add files to an archive")))
        ok = Add();
    if (IsCmd(_T("create"),  O, _T("FILES..."),  _T("create an archive")))
        ok = Create();
    if (IsCmd(_T("extract"), I, _T("[PATTERN]"), _T("extract entries")))
        ok = Extract();
    if (IsCmd(_T("list"),    I, _T("[PATTERN]"), _T("list contents")))
        ok = List();
    if (IsCmd(_T("remove"), IO, _T("PATTERN"),   _T("delete entries")))
        ok = Remove();
    if (IsCmd(_T("rename"), IO, _T("FROM TO"),   _T("rename an entry")))
        ok = Rename();

    m_in.reset();
    m_out.reset();

    ok = Commit(ok);
        
    // IsCmd populates m_availCmds, now use it to make a usage string
    wxString usage = _T("Usage: ") + progname +
        _T(" [-h|--help|/?] [COMMAND ARCHIVE [ARGUMENTS...]]\n") +
        _T("Commands:\n") + m_availCmds;

    // display help or error if necessary
    if (m_errMsg.empty())
        if (m_help) {
            std::cout << usage.mb_str() <<
            "Examples:\n"  <<
            "    " << progname.mb_str() << " add test.zip *\n" <<
            "        The archive type is deduced from the file extension.\n" <<
            "    " << progname.mb_str() << " extract zip < test.zip\n" <<
            "        If ARCHIVE has no extension it is taken to be the name\n" << 
            "        of an archive format and " << progname.mb_str() << " acts as a filter.\n";
            ok = true;
        } else if (!m_validCmd) {
            m_errMsg = _T("invalid command '") + m_cmd + _T("'");
        }

    if (!m_errMsg.empty())
        std::cerr << progname.mb_str() << ": " << m_errMsg.mb_str() << "\n"
                  << usage.mb_str();

    return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}


/////////////////////////////////////////////////////////////////////////////
// Helper to put a stream in bin-mode

void Reopen(const char *mode, FILE *stream)
{
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
    freopen(NULL, mode, stream);
#elif defined __WXMSW__
    setmode(fileno(stream), O_BINARY);
    (void)mode;
#else
    (void)mode;
    (void)stream;
#endif
}


/////////////////////////////////////////////////////////////////////////////
// tests if m_cmd is the given command

bool ArcApp::IsCmd(const wxString& cmd,
                   InputOutput inputOutput,
                   const wxString& params,
                   const wxChar *description)
{
    // count the expected parameters (actually count the spaces and brackets)
    size_t expected = params.empty() ? 1 : 2;
    size_t brackets = 0;
    for (wxString::size_type i = 0; params[i]; ++i)
        if (params[i] == _T(' '))
            ++expected;
        else if (params[i] == _T('['))
            ++brackets;

    // add the description of the cmd to m_availCmds to be used in the usage
    m_availCmds << wxString::Format(_T("    %-8s ARCHIVE %-12s %s\n"),
                                    cmd.c_str(), params.c_str(), description);

    // if this is the command to execute...
    if (m_help || !m_errMsg.empty() || m_cmd != cmd)
        return false;
    
    m_validCmd = true;
    size_t num = m_args.size() + (m_archive.empty() ? 0 : 1);

    if (num < expected - brackets ||
            (params.Right(3) != _T("...") && num > expected)) {
        m_errMsg = _T("wrong number of arguments for '") + cmd + _T("'");
        return false;
    }

    // open the input file
    if (inputOutput & I) {
        if (m_filter) {
            Reopen("rb", stdin);
            m_in.reset(new FFileInputStream(stdin));
        }
        else {
            m_in.reset(new FFileInputStream(m_archive));
            if (!m_in->Ok())
                return false;
        }
    }

    // open the output file
    if (inputOutput & O) {
        if (m_filter) {
            m_info = &std::cerr;
            Reopen("wb", stdout);
            m_out.reset(new FFileOutputStream(stdout));
        }
        else {
            wxString output = m_archive + _T(".tmp");
            m_out.reset(new FFileOutputStream(output));
            if (!m_out->Ok())
                return false;
            m_output = output;
        }
    }

    *m_info << cmd.mb_str() << " " << m_archive.mb_str() << std::endl;

    return true;
}
