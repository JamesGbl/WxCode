////////////////////////////////////////////////////////////////////////////
// Name:        arc.cpp
// Purpose:     Examples for archive classes
// Author:      Mike Wetherell
// RCS-ID:      $Id: arc.cpp,v 1.1 2004-06-25 23:21:08 chiclero Exp $
// Copyright:   (c) Mike Wetherell
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

//
// There are example functions for the following: Create, Extract, List,
// Remove, Rename and Add. I've tried to make each one pretty much self
// contained and short so that they can be looked at individually.
//
// There are also 'Filter' version of the above (e.g. FilterExtract) which
// demonstrate the same things on non-seekable streams.
//
// Current limitations (temporary):
//
// Some of the meta-data for entries in zip files is stored at the end of the
// file. So for non-seekable streams some of the meta data has to be handled
// when the end of the zip has been reached. The wxZip classes do allow this,
// however the generic wxArchive classes don't currently give access to 
// these features. So currently this program removes some meta-data from
// zips when they are handled by the filter functions.
//
// I've used std::map here (not in the stream classes which don't use any
// features not available everywhere). Though even VC++ 5.0 can compile it
// so I think it's compilable by all but Open Watcom.
//

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
wxDEFINE_SCOPED_PTR_TYPE(wxFFileOutputStream);
wxDEFINE_SCOPED_PTR_TYPE(wxArchiveInputStream);
wxDEFINE_SCOPED_PTR_TYPE(wxArchiveOutputStream);
wxDEFINE_SCOPED_PTR_TYPE(wxArchiveEntry);
wxDEFINE_SCOPED_PTR_TYPE(wxArchiveCatalog);
wxDEFINE_SCOPED_PTR_TYPE(wxArchiveClassFactory);

// a map for the list of files to process
typedef std::map<wxString, wxString> NameMap;


/////////////////////////////////////////////////////////////////////////////
// The application class

class ArcApp : public wxAppConsole
{
public:
    bool OnInit() { setlocale(LC_ALL, ""); return true; }
    int OnRun();

    // the examples
    bool Create(bool filter);
    bool Extract();
    bool List();
    bool Remove();
    bool Rename();
    bool Add();

    // filter versions of the same examples
    bool FilterExtract();
    bool FilterList();
    bool FilterRemove();
    bool FilterRename();
    bool FilterAdd();

    // data members used by the examples
    wxString m_archive;                 // the archive file
    wxString m_output;                  // the output archive file
    wxArrayString m_args;               // any additional command line args
    wxArchiveClassFactoryPtr m_factory; // the class factory

    // make a class factory
    bool MakeFactory();

    // make the list of files to process
    void MakeFileList(NameMap& files,
                      const wxString& exclude1 = wxEmptyString,
                      const wxString& exclude2 = wxEmptyString);

    // commit or rollback changes
    void Commit(bool commit);

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
    bool m_filter;          // filter stdin to stdout?
};


IMPLEMENT_APP_CONSOLE(ArcApp)


/////////////////////////////////////////////////////////////////////////////
// Create

bool ArcApp::Create(bool filter)
{
    if (!filter)
        std::cout << "creating " << m_archive.mb_str() << std::endl;

    wxFFileOutputStreamPtr out(filter ?
        new wxFFileOutputStream(stdout) : new wxFFileOutputStream(m_archive));
    if (!out->Ok())
        return false;

    wxArchiveOutputStreamPtr arc(m_factory->NewStream(*out));

    NameMap files;
    MakeFileList(files, filter ? _T("") : m_archive.c_str());

    for (NameMap::iterator it = files.begin(); it != files.end(); ++it) {
        wxString name = it->second;

        if (!filter)
            std::cout << "adding " << name.mb_str() << std::endl;

        if (wxDirExists(name)) {
            wxDateTime dt = wxFileName(name, _T("")).GetModificationTime();
            if (!arc->CreateDir(name, dt))
                return false;
        }
        else {
            wxFFileInputStream in(name);

            if (in.Ok()) {
                wxDateTime dt = wxFileName(name).GetModificationTime();
                if (!arc->Create(name, dt, in.GetSize()) ||
                        !arc->Write(in) || !in.Eof())
                    return false;
            }
        }
    }

    return arc->CloseArchive();
}


/////////////////////////////////////////////////////////////////////////////
// Extract - extracts the contents of an archive
//
// m_args[0] may contain a pattern, and if so only files matching are
// extracted.

bool ArcApp::Extract()
{
    std::cout << "extracting from " << m_archive.mb_str() << std::endl;
    wxFFileInputStream in(m_archive);
    if (!in.Ok()) return false;

    wxArchiveCatalogPtr     cat(m_factory->NewCatalog(in));
    wxArchiveInputStreamPtr arc(m_factory->NewStream(in));
    wxArchiveEntryPtr       entry(cat->ReadEntry());

    for (; entry.get(); entry.reset(cat->ReadEntry())) {
        if (m_args.size() > 0 && !entry->GetName().Matches(m_args[0]))
            continue;
        std::cout << "extracting " << entry->GetName().mb_str() << std::endl;
        wxFileName fn(entry->GetName());

        if (entry->IsDir()) {
            fn.Mkdir(0777, wxPATH_MKDIR_FULL);
        } else {
            wxFileName::Mkdir(fn.GetPath(), 0777, wxPATH_MKDIR_FULL);

            wxFFileOutputStream out(entry->GetName());
            if (!out || !arc->Open(*entry) || !out.Write(*arc) || !arc->Eof())
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

    return cat->IsEof();
}


/////////////////////////////////////////////////////////////////////////////
// List - shows how to retreive all the meta data for the archive entries

bool ArcApp::List()
{
    std::cout << "listing " << m_archive.mb_str() << std::endl;
    wxFFileInputStream in(m_archive);
    if (!in.Ok()) return false;

    wxArchiveCatalogPtr cat(m_factory->NewCatalog(in));
    wxArchiveEntryPtr   entry(cat->ReadEntry());

    for (; entry.get(); entry.reset(cat->ReadEntry()))
        if (m_args.empty() || entry->GetName().Matches(m_args[0]))
            std::cout
                << std::setw(9) << entry->GetSize() << " "
                << entry->GetDateTime().Format(_T("%x %X")).mb_str() << " "
                << entry->GetName().mb_str() << std::endl;

    return cat->IsEof();
}


/////////////////////////////////////////////////////////////////////////////
// Remove - delete entries from an archive
//
// Copies all the entries from the old archive to a new one, leaving out
// any matching the deletion pattern.

bool ArcApp::Remove()
{
    std::cout << "removing entries from " << m_archive.mb_str() << std::endl;
    wxFFileInputStream in(m_archive);
    if (!in.Ok()) return false;

    wxString output = m_archive + _T(".tmp");
    wxFFileOutputStream out(output);
    if (!out.Ok()) return false;
    m_output = output;
    wxString pattern = m_args[0];

    wxArchiveCatalogPtr      cat(m_factory->NewCatalog(in));
    wxArchiveInputStreamPtr  arc(m_factory->NewStream(in));
    wxArchiveOutputStreamPtr outarc(m_factory->NewStream(out));
    wxArchiveEntryPtr        entry(cat->ReadEntry());

    for (; entry.get(); entry.reset(cat->ReadEntry()))
        if (entry->GetName().Matches(pattern))
            std::cout << "removing " << entry->GetName().mb_str() << std::endl;
        else
            if (!arc->OpenRaw(*entry) || !outarc->CreateRaw(*entry) ||
                    !outarc->Write(*arc) || !arc->Eof())
                return false;

    return cat->IsEof() && outarc->CloseArchive();
}


/////////////////////////////////////////////////////////////////////////////
// Rename an entry
//
// Copies all the entries from the old archive to a new one. Changes
// can be made to an entry's meta data during this process, in this case
// changing the name.

bool ArcApp::Rename()
{
    std::cout << "rename entry in " << m_archive.mb_str() << std::endl;
    wxFFileInputStream in(m_archive);
    if (!in.Ok()) return false;

    wxString output = m_archive + _T(".tmp");
    wxFFileOutputStream out(output);
    if (!out.Ok()) return false;
    m_output = output;

    wxString from = m_args[0];
    wxString to = m_args[1];

    wxArchiveCatalogPtr      cat(m_factory->NewCatalog(in));
    wxArchiveInputStreamPtr  arc(m_factory->NewStream(in));
    wxArchiveOutputStreamPtr outarc(m_factory->NewStream(out));
    wxArchiveEntryPtr        entry(cat->ReadEntry());

    for (; entry.get(); entry.reset(cat->ReadEntry())) {
        if (entry->GetName() == from) {
            std::cout << "renaming '" << from.mb_str()
                      << "' to '" << to.mb_str() << "'" << std::endl;
            entry->SetName(to);
        }

        if (!arc->OpenRaw(*entry) || !outarc->CreateRaw(*entry) ||
                !outarc->Write(*arc) || !arc->Eof())
            return false;
    }

    return cat->IsEof() && outarc->CloseArchive();
}


/////////////////////////////////////////////////////////////////////////////
// Add - add files to an archive
//
// This more complex example shows how to update an existing archive
// with a list of new or replacement files.
//
// A few approaches are possible, the one taken here is to sort both the
// list of new files and the archive's catalog into the same order, then
// walk down the two lists in parallel, taking the new file if there is one
// or keeping the existing archive entry if not.

WX_DEFINE_ARRAY_PTR(wxArchiveEntry*, ArchiveEntryArrayBase);

class ArchiveEntryArray : public ArchiveEntryArrayBase {
public:    
    ~ArchiveEntryArray() { WX_CLEAR_ARRAY(*this); }
};

int CMPFUNC_CONV NameCompare(wxArchiveEntry **a, wxArchiveEntry **b) {
    return wxStrcmp((*a)->GetInternalName(), (*b)->GetInternalName());
}

bool ArcApp::Add()
{
    // Open the input file (which may not exist)
    wxLog::EnableLogging(false);
    wxFFileInputStream in(m_archive);
    wxLog::EnableLogging(true);

    // Read the archive's catalog into an array
    ArchiveEntryArray entries;

    if (in.Ok()) {
        std::cout << "updating " << m_archive.mb_str() << std::endl;
        wxArchiveCatalogPtr cat(m_factory->NewCatalog(in));
        wxArchiveEntry *pEntry;

        while ((pEntry = cat->ReadEntry()) != NULL)
            entries.push_back(pEntry);

        if (!cat->IsEof())
            return false;
        entries.Sort(NameCompare);
    }
    else {
        std::cout << "creating " << m_archive.mb_str() << std::endl;
    }

    // Create a temporary file for the output
    wxString output = m_archive + _T(".tmp");
    wxFFileOutputStream out(output);
    if (!out.Ok()) return false;
    m_output = output;

    // turn m_args into a list of files, i.e. expand wildcards,
    // recurse directories and remove duplicates
    NameMap files;
    MakeFileList(files, m_archive, m_output);

    // Create the archive input and output streams
    wxArchiveInputStreamPtr arc(in.Ok() ? m_factory->NewStream(in) : NULL);
    wxArchiveOutputStreamPtr outarc(m_factory->NewStream(out));

    // Now walk down the two lists in parallel...
    NameMap::iterator i = files.begin();
    ArchiveEntryArray::iterator j = entries.begin();

    while (i != files.end() || j != entries.end()) {
        int cmp;            // +ve = keep, -ve == new, 0 == replace
        bool ok = false;

        // which is next in order, a file on the file list, or an entry
        // from the catalog?
        if (i == files.end())
            cmp = +1;
        else if (j == entries.end())
            cmp = -1;
        else
            cmp = wxStrcmp(i->first, (*j)->GetInternalName());

        // new file, or replace an existing one
        if (cmp <= 0) {
            wxString name = i->second;
            ++i;

            if (cmp < 0)
                std::cout << "adding " << name.mb_str() << std::endl;
            else
                std::cout << "replacing " << name.mb_str() << std::endl;

            if (wxDirExists(name)) {
                wxDateTime dt = wxFileName(name, _T("")).GetModificationTime();
                if (!outarc->CreateDir(name, dt))
                    return false;
                ok = true;
            } else {
                wxFFileInputStream file(name);

                if (file.Ok()) {
                    wxDateTime dt = wxFileName(name).GetModificationTime();

                    if (!outarc->Create(name, dt, file.GetSize()) ||
                            !outarc->Write(file) || !file.Eof())
                        return false;

                    ok = true;
                }
            }
        }

        // keep the entry in the catalog
        // (cmp == 0 && !ok) means replace failed, so keep instead
        if (cmp > 0 || (cmp == 0 && !ok))
            if (!arc->OpenRaw(**j) || !outarc->CreateRaw(**j) ||
                    !outarc->Write(*arc) || !arc->Eof())
                return false;

        if (cmp >= 0)
            ++j;
    }

    return outarc->CloseArchive();
}


/////////////////////////////////////////////////////////////////////////////
// FilterExtract - extracts the contents of an archive piped into stdin
//
// m_args[0] may contain a pattern, and if so only files matching are
// extracted.

bool ArcApp::FilterExtract()
{
    std::cout << "extracting from " << m_archive.mb_str() << std::endl;
    wxFFileInputStream in(stdin);
    wxArchiveInputStreamPtr arc(m_factory->NewStream(in));

    while (arc->Open()) {
        const wxArchiveEntry& entry = arc->GetEntry();

        if (m_args.size() > 0 && !entry.GetName().Matches(m_args[0]))
            continue;
        std::cout << "extracting " << entry.GetName().mb_str() << std::endl;
        wxFileName fn(entry.GetName());

        if (entry.IsDir()) {
            fn.Mkdir(0777, wxPATH_MKDIR_FULL);
        } else {
            wxFileName::Mkdir(fn.GetPath(), 0777, wxPATH_MKDIR_FULL);

            wxFFileOutputStream out(entry.GetName());
            if (!out || !out.Write(*arc) || !arc->Eof())
                return false;
        }

#ifdef __WXMSW__
        if (!entry.IsDir()) {
#endif
            wxDateTime dt = entry.GetDateTime();
            fn.SetTimes(NULL, &dt, NULL);
#ifdef __WXMSW__
        }
#endif
    }

    return arc->Eof();
}


/////////////////////////////////////////////////////////////////////////////
// FilterList - list the contents of an archive piped into stdin
//
// Of course, there isn't an efficient way to do this. In general it's
// necessary to open all the entries in the archive.

bool ArcApp::FilterList()
{
    std::cout << "listing " << m_archive.mb_str() << std::endl;
    wxFFileInputStream in(stdin);
    wxArchiveInputStreamPtr arc(m_factory->NewStream(in));

    while (arc->Open()) {
        const wxArchiveEntry& entry = arc->GetEntry();

        if (m_args.empty() || entry.GetName().Matches(m_args[0])) {
            off_t size = entry.GetSize();

            if (size == wxInvalidOffset) {
                char buf[1024];
                while (arc->IsOk())
                    arc->Read(buf, sizeof(buf));
                if (!arc->Eof())
                    return false;
                size = arc->TellI();
            }
            
            std::cout
                << std::setw(9) << size << " "
                << entry.GetDateTime().Format(_T("%x %X")).mb_str() << " "
                << entry.GetName().mb_str() << std::endl;
        }
    }

    return arc->Eof();
}


/////////////////////////////////////////////////////////////////////////////
// FilterRemove - delete entries from an archive piped into stdin and
// output the result to stdout
//
// Copies all the entries from the old archive to a new one, leaving out
// any matching the deletion pattern.

bool ArcApp::FilterRemove()
{
    wxFFileInputStream in(stdin);
    wxFFileOutputStream out(stdout);
    wxArchiveInputStreamPtr arc(m_factory->NewStream(in));
    wxArchiveOutputStreamPtr outarc(m_factory->NewStream(out));

    wxString pattern = m_args[0];

    while (arc->OpenRaw())
        if (!arc->GetEntry().GetName().Matches(pattern))
            if (!outarc->CreateRaw(arc->GetEntry()) ||
                    !outarc->Write(*arc) || !arc->Eof())
                return false;

    return arc->Eof() && outarc->CloseArchive();
}


/////////////////////////////////////////////////////////////////////////////
// FilterRename - an entry of an archive piped into stdin and output the
// result to stdout
//
// Copies all the entries from the old archive to a new one. Changes
// can be made to an entry's meta data during this process, in this case
// changing the name.

bool ArcApp::FilterRename()
{
    wxFFileInputStream in(stdin);
    wxFFileOutputStream out(stdout);
    wxArchiveInputStreamPtr arc(m_factory->NewStream(in));
    wxArchiveOutputStreamPtr outarc(m_factory->NewStream(out));
    wxArchiveEntryPtr newEntry;

    wxString from = m_args[0];
    wxString to = m_args[1];

    while (arc->OpenRaw()) {
        const wxArchiveEntry *entry = &arc->GetEntry();

        if (entry->GetName() == from) {
            newEntry.reset(entry->Clone());
            newEntry->SetName(to);
            entry = newEntry.get();
        }

        if (!outarc->CreateRaw(*entry) || !outarc->Write(*arc) || !arc->Eof())
            return false;
    }

    return arc->Eof() && outarc->CloseArchive();
}


/////////////////////////////////////////////////////////////////////////////
// FilterAdd - add files to an archive
//
// Update an existing archive with a list of new or replacement files.

bool ArcApp::FilterAdd()
{
    wxFFileInputStream in(stdin);
    wxFFileOutputStream out(stdout);
    wxArchiveInputStreamPtr arc(m_factory->NewStream(in));
    wxArchiveOutputStreamPtr outarc(m_factory->NewStream(out));

    // turn m_args into a list of files, i.e. expand wildcards, recurse
    // directories and remove duplicates
    NameMap files;
    MakeFileList(files);

    // more files in the input archive?
    bool arcEnd = false;

    for (;;) {
        bool keep;
        NameMap::iterator it;

        if (!arcEnd)
            arcEnd = !arc->OpenRaw();

        if (arcEnd) {
            if (files.empty())
                break;
            it = files.begin();
            keep = false;
        } else {
            it = files.find(arc->GetEntry().GetInternalName());
            keep = true;
        }

        if (it != files.end()) {
            wxString name = it->second;
            files.erase(it);

            if (wxDirExists(name)) {
                wxDateTime dt = wxFileName(name, _T("")).GetModificationTime();
                if (!outarc->CreateDir(name, dt))
                    return false;
                keep = false;
            } else {
                wxFFileInputStream file(name);

                if (file.Ok()) {
                    wxDateTime dt = wxFileName(name).GetModificationTime();
                    if (!outarc->Create(name, dt, file.GetSize()) ||
                            !outarc->Write(file) || !file.Eof())
                        return false;
                    keep = false;
                }
            }
        }

        if (keep)
            if (!outarc->CreateRaw(arc->GetEntry()) ||
                    !outarc->Write(*arc) || !arc->Eof())
                return false;
    }

    return arc->Eof() && outarc->CloseArchive();
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
    const wxString& exclude1 /*=wxEmptyString*/,
    const wxString& exclude2 /*=wxEmptyString*/)
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
// Commit or rollback changes

void ArcApp::Commit(bool commit)
{
    bool exists = wxFileExists(m_archive);

    if (commit) {
        if (exists) {
            std::cout << "commiting changes\n";
            wxRemove(m_archive);
        }
        wxRename(m_output, m_archive);
    } else {
        if (exists)
            std::cout << "rolling back changes\n";
        wxRemove(m_output);
    }
}


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
        ok = m_filter ? FilterAdd() : Add();
    if (IsCmd(_T("create"),  O, _T("FILES..."),  _T("create an archive")))
        ok = Create(m_filter);
    if (IsCmd(_T("extract"), I, _T("[PATTERN]"), _T("extract entries")))
        ok = m_filter ? FilterExtract() : Extract();
    if (IsCmd(_T("list"),    I, _T("[PATTERN]"), _T("list contents")))
        ok = m_filter ? FilterList() : List();
    if (IsCmd(_T("remove"), IO, _T("PATTERN"),   _T("delete entries")))
        ok = m_filter ? FilterRemove() : Remove();
    if (IsCmd(_T("rename"), IO, _T("FROM TO"),   _T("rename an entry")))
        ok = m_filter ? FilterRename() : Rename();

    if (!m_output.empty())
        Commit(ok);
        
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

    // set stdin/stdout to binary mode?
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
    if (m_filter) {
        if (inputOutput & I)
            freopen(NULL, "rb", stdin);
        if (inputOutput & O)
            freopen(NULL, "wb", stdout);
    }
#elif defined __WXMSW__
    if (m_filter) {
        if (inputOutput & I)
            setmode(fileno(stdin), O_BINARY);
        if (inputOutput & O)
            setmode(fileno(stdout), O_BINARY);
    }
#else
    (void)inputOutput;
#endif

    return true;
}
