/////////////////////////////////////////////////////////////////////////////
// Name:        archive.h
// Purpose:     Streams for archive formats
// Author:      Mike Wetherell
// RCS-ID:      $Id: archive.h,v 1.4 2004-07-14 18:24:21 chiclero Exp $
// Copyright:   (c) 2004 Mike Wetherell
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_ARCHIVE_H__
#define _WX_ARCHIVE_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "archive.h"
#endif

#include "wx/defs.h"

#if wxUSE_STREAMS

#include "wx/stream.h"
#include "wx/filename.h"


/////////////////////////////////////////////////////////////////////////////
// wxArchiveNotifier

class wxArchiveNotifier
{
public:
    virtual ~wxArchiveNotifier() { }

    virtual void OnEntryChanged(class wxArchiveEntry& entry) = 0;
};


/////////////////////////////////////////////////////////////////////////////
// wxArchiveEntry
//
// Holds an entry's meta data, such as filename and timestamp.

class wxArchiveEntry : public wxObject
{
public:
    wxArchiveEntry() : m_notifier(NULL) { }
    virtual ~wxArchiveEntry() { }

    virtual wxDateTime GetDateTime() const = 0;
    virtual off_t      GetSize() const = 0;
    virtual off_t      GetOffset() const = 0;
    virtual bool       IsDir() const = 0;
    virtual wxString   GetInternalName() const = 0;
    virtual wxString   GetName(wxPathFormat format = wxPATH_NATIVE) const = 0;

    virtual void SetDateTime(const wxDateTime& dt) = 0;
    virtual void SetSize(off_t size) = 0;
    virtual void SetIsDir(bool isDir = true) = 0;
    virtual void SetName(const wxString& name,
                         wxPathFormat format = wxPATH_NATIVE) = 0;
    
    wxArchiveEntry *Clone() const { return DoClone(); }

    inline void SetNotifier(wxArchiveNotifier& notifier);
    virtual void UnsetNotifier() { m_notifier = NULL; }

protected:
    virtual void SetOffset(off_t offset) = 0;
    virtual wxArchiveEntry* DoClone() const = 0;

    wxArchiveNotifier *GetNotifier() const { return m_notifier; }
    inline wxArchiveEntry& operator=(const wxArchiveEntry& entry);

private:
    wxArchiveNotifier *m_notifier;

    DECLARE_ABSTRACT_CLASS(wxArchiveEntry)
};

void wxArchiveEntry::SetNotifier(wxArchiveNotifier& notifier)
{
    UnsetNotifier();
    m_notifier = &notifier;
    m_notifier->OnEntryChanged(*this);
}

wxArchiveEntry& wxArchiveEntry::operator=(const wxArchiveEntry& entry)
{
    m_notifier = entry.m_notifier;
    return *this;
}


/////////////////////////////////////////////////////////////////////////////
// wxArchiveExtra
//
// This holds any additional meta-data associated with the archive as a
// whole. For example, for zips it can hold a comment for the entire zip.
//
// Since not all archive formats have such data, there isn't much that
// can be done with it without knowing what type of archive is being used.
// However when making a modified copy of an existing archive, it is useful
// to be able to choose whether or not to transfer this extra information.

class wxArchiveExtra : public wxObject
{
public:
    virtual ~wxArchiveExtra() { }

    wxArchiveExtra *Clone() const { return DoClone(); }

protected:
    virtual wxArchiveExtra* DoClone() const = 0;

    wxArchiveExtra& operator=(const wxArchiveExtra& WXUNUSED(extra))
        { return *this; }

    DECLARE_ABSTRACT_CLASS(wxArchiveExtra)
};


/////////////////////////////////////////////////////////////////////////////
// wxArchiveInputStream
//
// GetNextEntry() returns an wxArchiveEntry object (and gives away ownership)
// containing the meta-data for the next entry in the archive. Reading from
// the wxArchiveInputStream then returns the entry's data. Eof() becomes
// true after an attempt has been made to read past the end of the entry's
// data.
//
// When there are no more entries, GetNextEntry() returns NULL and sets Eof().
//
//    wxDEFINE_SCOPED_PTR_TYPE(wxArchiveEntry);
//    wxArchiveEntryPtr entry;
//
//    while (entry.reset(arc.GetNextEntry()), entry.get() != NULL)
//    {
//        wxString name = entry->GetName();     // access meta-data
//        arc->Read(buffer, sizeof(buffer))     // access data
//        ...
//    }
//
// To access several entries randomly, the entire catalog of entries can
// be transfered to a container, such as a std::map or a wxHashMap (see
// wxArchiveIterator), then entries looked up by name can be opened using
// the OpenEntry() method.
//
// To open more than one entry simultaneously you need more than one
// underlying stream on the same archive:
//
//    // load the zip catalog
//    wxFFileInputStream in(filename);
//    wxZipInputStream2 zip(in);
//    typedef std::map<wxString, wxZipEntry*> ZipCatalog;
//    ZipCatalog cat((wxZipPairIter)zip, wxZipPairIter());
//
//    // open an entry by name
//    ZipCatalog::iterator it;
//    if ((it = cat.find(name)) != cat.end())
//        zip.OpenEntry(*it->second);
//
//    // opening another entry without closing the first requires another
//    // input stream for the same file
//    wxFFileInputStream in2(filename);
//    wxZipInputStream2 zip2(in2);
//    if ((it = cat.find(name2)) != cat.end())
//        zip2.OpenEntry(*it->second);
//

class wxArchiveInputStream : public wxFilterInputStream
{
public:
    typedef wxArchiveEntry entry_type;

    virtual ~wxArchiveInputStream() { }
    
    virtual bool OpenEntry(wxArchiveEntry& entry) = 0;
    virtual bool CloseEntry() = 0;

    wxArchiveEntry *GetNextEntry()  { return DoGetNextEntry(); }
    wxArchiveExtra *GetExtra()      { return DoGetExtra(); }

    virtual char Peek()             { return wxInputStream::Peek(); }
    
protected:
    wxArchiveInputStream(wxInputStream& stream, wxMBConv& conv);

    virtual wxArchiveEntry *DoGetNextEntry() = 0;
    virtual wxArchiveExtra *DoGetExtra() = 0;

    wxMBConv& GetConv() const       { return m_conv; }

private:
    wxMBConv& m_conv;
};


/////////////////////////////////////////////////////////////////////////////
// wxArchiveOutputStream
//
// PutNextEntry is used to create a new entry in the output archive, then
// the entry's data is written to the wxArchiveOutputStream.
//
// Only one entry can be open for output at a time; another call to
// PutNextEntry closes the current entry and begins the next.
// 
// The overload 'bool PutNextEntry(wxArchiveEntry *entry)' takes ownership
// of the entry object.
//
// To modify an existing archive, write a new copy of the archive to a new
// file, making any necessary changes along the way and transferring any
// unchanged entries using CopyEntry. For archive types which compress entry
// data, CopyEntry is likely to be much more efficient than transferring the
// data using Read and Write since it will copy them without decompressing
// and recompressing them.
//
// In general modifications aren't possible without rewriting the archive,
// though it may be possible in some limited cases. Even then, rewriting
// the archive is usually a better choice, since a failure can be handled
// without losing the whole archive.

class wxArchiveOutputStream : public wxFilterOutputStream
{
public:
    virtual ~wxArchiveOutputStream() { }

    virtual bool PutNextEntry(wxArchiveEntry *entry) = 0;

    virtual bool PutNextEntry(const wxString& name,
                              const wxDateTime& dt = wxDateTime::Now(),
                              off_t size = wxInvalidOffset) = 0;

    virtual bool PutNextDirEntry(const wxString& name,
                                 const wxDateTime& dt = wxDateTime::Now()) = 0;

    virtual bool CopyEntry(wxArchiveEntry *entry,
                           wxArchiveInputStream& stream) = 0;

    virtual bool CloseEntry() = 0;
    virtual bool Close() = 0;

    virtual void SetExtra(wxArchiveExtra *extra) = 0;

protected:
    wxArchiveOutputStream(wxOutputStream& stream, wxMBConv& conv);

    wxMBConv& GetConv() const { return m_conv; }

private:
    wxMBConv& m_conv;
};


/////////////////////////////////////////////////////////////////////////////
// wxArchiveClassFactory
//
// A wxArchiveClassFactory instance for a particular archive type allows
// the creation of the other classes that may be needed.

class wxArchiveClassFactory : public wxObject
{
public:
    wxArchiveEntry *NewEntry()
        { return DoNewEntry(); }
    wxArchiveInputStream *NewStream(wxInputStream& stream)
        { return DoNewStream(stream); }
    wxArchiveOutputStream *NewStream(wxOutputStream& stream)
        { return DoNewStream(stream); }

    void SetConv(wxMBConv& conv) { m_pConv = &conv; }
    wxMBConv& GetConv() const { return *m_pConv; }

protected:
    virtual wxArchiveEntry        *DoNewEntry() = 0;
    virtual wxArchiveInputStream  *DoNewStream(wxInputStream& stream) = 0;
    virtual wxArchiveOutputStream *DoNewStream(wxOutputStream& stream) = 0;

    wxArchiveClassFactory() : m_pConv(&wxConvFile) { }
    wxArchiveClassFactory& operator=(const wxArchiveClassFactory& WXUNUSED(f))
        { return *this; }

private:
    wxMBConv *m_pConv;

    DECLARE_ABSTRACT_CLASS(wxArchiveClassFactory)
};


/////////////////////////////////////////////////////////////////////////////
// wxArchiveIterator
//
// This is an input iterator that can be used to transfer an archive's
// catalog to a container. For example, given a wxArchiveInputStream 'arc':
//
//    typedef std::vector<wxArchiveEntry*> ArchiveCatalog;
//    ArchiveCatalog cat((wxArchiveIter)arc, wxArchiveIter());
//
// When the iterator is dereferenced, it gives away ownership of an entry
// object. So in the above example, when you have finished with 'cat'
// you must delete the pointers it contains.
//
// If you have smart pointers with normal copy semantics (i.e. not auto_ptr
// or wxScopedPtr), then you can create an iterator which uses them instead.
// For example, with a smart pointer class for zip entries 'ZipEntryPtr':
//
//    typedef std::vector<ZipEntryPtr> ZipCatalog;
//    typedef wxArchiveIterator<wxZipInputStream2, ZipEntryPtr> ZipIter;
//    ZipCatalog cat((ZipIter)zip, ZipIter());
//
// Iterators can also be constructed to return std::pair objects, which can be
// used to populate a std::map, to allow entries to be looked up by name, e.g:
//
//    typedef std::map<wxString, wxZipEntry*> ZipCatalog;
//    ZipCatalog cat((wxZipPairIter)zip, wxZipPairIter());
// 
// Or a pair containing a smart pointer (again ZipEntryPtr):
//      
//    typedef std::map<wxString, ZipEntryPtr> ZipCatalog;
//    typedef wxArchiveIterator<wxZipInputStream2,
//                std::pair<wxString, ZipEntryPtr> > ZipPairIter;
//    ZipCatalog cat((ZipPairIter)zip, ZipPairIter());
//
// The examples given so far require the compiler to support member templates.
// If your compiler does not, you could do the following instead:
//
//    typedef std::vector<wxZipEntry*> ZipCatalog;
//    ZipCatalog cat;
//    for (wxZipIter i(zip); i != wxZipIter(); ++i)
//        cat.push_back(*i);
//
// And if your compiler doesn't support that either, then you can do without
// the iterators altogether:
//
//    WX_DEFINE_ARRAY_PTR(wxZipEntry*, ZipCatalog);
//    ZipCatalog cat;
//    wxZipEntry *entry;
//    while ((entry = zip.GetNextEntry()) != NULL)
//        cat.push_back(entry);
//

#if wxUSE_STL
#include <iterator>
#include <utility>

template <class X, class Y>
void _wxSetArchiveIteratorValue(X& val, Y entry, void *WXUNUSED(d)) {
    val = X(entry);
}
template <class X, class Y, class Z>
void _wxSetArchiveIteratorValue(std::pair<X, Y>& val, Z entry, Z WXUNUSED(d)) {
    val = std::make_pair(X(entry->GetInternalName()), Y(entry));
}

// Older versions of VC++ don't allow typename here but some compilers
// require it
#if defined _MSC_VER && _MSC_VER < 1300
template <class Arc, class T = Arc::entry_type*>
#else
template <class Arc, class T = typename Arc::entry_type*>
#endif
class wxArchiveIterator
{
public:
    typedef std::input_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;

    wxArchiveIterator() : m_rep(NULL) { }

    wxArchiveIterator(Arc& arc) {
        typename Arc::entry_type* entry = arc.GetNextEntry();
        m_rep = entry ? new Rep(arc, entry) : NULL;
    }

    wxArchiveIterator(const wxArchiveIterator& it) : m_rep(it.m_rep) {
        if (m_rep)
            m_rep->AddRef();
    }
    
    ~wxArchiveIterator() {
        if (m_rep)
            m_rep->UnRef();
    }

    const T& operator *() const {
        return m_rep->GetValue();
    }

    const T* operator ->() const {
        return &**this;
    }

    wxArchiveIterator& operator =(const wxArchiveIterator& it) {
        if (it.m_rep)
            it.m_rep.AddRef();
        if (m_rep)
            m_rep.UnRef();
        m_rep = it.m_rep;
        return *this;
    }

    wxArchiveIterator& operator ++() {
        m_rep = m_rep->Next();
        return *this;
    }

    wxArchiveIterator operator ++(int) {
        wxArchiveIterator it(*this);
        ++(*this);
        return it;
    }

    friend bool operator ==(const wxArchiveIterator& i,
                            const wxArchiveIterator& j) {
        return i.m_rep == j.m_rep;
    }

    friend bool operator !=(const wxArchiveIterator& i,
                            const wxArchiveIterator& j) {
        return !(i == j);
    }

private:
    class Rep {
        Arc& m_arc;
        typename Arc::entry_type* m_entry;
        T m_value;
        int m_ref;
        
    public:
        Rep(Arc& arc, typename Arc::entry_type* entry)
            : m_arc(arc), m_entry(entry), m_value(), m_ref(1) { }
        ~Rep()
            { delete m_entry; }
        
        void AddRef() {
            m_ref++;
        }

        void UnRef() {
            if (--m_ref == 0)
                delete this;
        }

        Rep *Next() {
            typename Arc::entry_type* entry = m_arc.GetNextEntry();
            if (!entry) {
                UnRef();
                return NULL;
            }
            if (m_ref > 1) {
                m_ref--; 
                return new Rep(m_arc, entry);
            }
            delete m_entry;
            m_entry = entry;
            m_value = T();
            return this;
        }

        const T& GetValue() {
            if (m_entry) {
                _wxSetArchiveIteratorValue(m_value, m_entry, m_entry);
                m_entry = NULL;
            }
            return m_value;
        }
    } *m_rep;
};

typedef wxArchiveIterator<wxArchiveInputStream> wxArchiveIter;
typedef wxArchiveIterator<wxArchiveInputStream,
        std::pair<wxString, wxArchiveEntry*> >  wxArchivePairIter;

#endif // wxUSE_STL

#endif // wxUSE_STREAMS

#endif // _WX_ARCHIVE_H__
