/////////////////////////////////////////////////////////////////////////////
// Name:        archive.h
// Purpose:     Streams for archive formats
// Author:      Mike Wetherell
// RCS-ID:      $Id: archive.h,v 1.2 2004-06-28 11:25:16 chiclero Exp $
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
// Archive entry - holds an entry's meta data

class wxArchiveEntry : public wxObject
{
public:
    virtual ~wxArchiveEntry() { }

    virtual wxDateTime GetDateTime() const = 0;
    virtual off_t      GetSize() const = 0;
    virtual off_t      GetOffset() const = 0;
    virtual bool       IsDir() const = 0;
    virtual wxString   GetInternalName() const = 0;
    virtual wxString   GetName(wxPathFormat format = wxPATH_NATIVE) const = 0;

    virtual void SetDateTime(const wxDateTime& dt) = 0;
    virtual void SetSize(off_t size) = 0;
    virtual void SetOffset(off_t offset) = 0;
    virtual void SetIsDir(bool isDir = true) = 0;
    virtual void SetName(const wxString& name, wxPathFormat format = wxPATH_NATIVE) = 0;
    
    wxArchiveEntry *Clone() const { return DoClone(); }

protected:
    virtual wxArchiveEntry* DoClone() const = 0;

    DECLARE_ABSTRACT_CLASS(wxArchiveEntry)
    DECLARE_NO_ASSIGN_CLASS(wxArchiveEntry)
};


/////////////////////////////////////////////////////////////////////////////
// wxArchiveInputStream

class wxArchiveInputStream : public wxFilterInputStream
{
public:
    typedef wxArchiveEntry entry_type;

    virtual ~wxArchiveInputStream() { }
    
    virtual bool Open() = 0;

    // Open a particular entry from the archive's catalog
    virtual bool Open(wxArchiveEntry& entry) = 0;

    // If the archive contains compressed entries, then OpenRaw allows
    // them to be read still compressed. Allows for efficient copying
    // to an output archive.
    virtual bool OpenRaw() = 0;
    virtual bool OpenRaw(wxArchiveEntry& entry) = 0;

    virtual bool Close() = 0;
    virtual bool IsOpened() const = 0;

    wxArchiveEntry *GetEntry() { return DoGetEntry(); }

    virtual char Peek()        { return wxInputStream::Peek(); }

protected:
    wxArchiveInputStream(wxInputStream& stream, wxMBConv& conv);

    virtual wxArchiveEntry *DoGetEntry() = 0;
    wxMBConv& GetConv() const { return m_conv; }

private:
    wxMBConv& m_conv;

    DECLARE_NO_ASSIGN_CLASS(wxArchiveInputStream)
};


/////////////////////////////////////////////////////////////////////////////
// wxArchiveOutputStream

class wxArchiveOutputStream : public wxFilterOutputStream
{
public:
    virtual ~wxArchiveOutputStream() { }

    // Open a new entry for writing
    virtual bool Create(wxArchiveEntry *entry) = 0;
    virtual bool CreateRaw(wxArchiveEntry *entry) = 0;

    virtual bool Create(const wxString& name,
                        const wxDateTime& dt = wxDateTime::Now(),
                        off_t size = wxInvalidOffset) = 0;

    virtual bool CreateDir(const wxString& name,
                           const wxDateTime& dt = wxDateTime::Now()) = 0;

    virtual bool Close() = 0;
    virtual bool CloseArchive() = 0;
    virtual bool IsOpened() const = 0;

protected:
    wxArchiveOutputStream(wxOutputStream& stream, wxMBConv& conv);

    wxMBConv& GetConv() const { return m_conv; }

private:
    wxMBConv& m_conv;

    DECLARE_NO_ASSIGN_CLASS(wxArchiveOutputStream)
};


/////////////////////////////////////////////////////////////////////////////
// wxArchiveClassFactory

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

private:
    wxMBConv *m_pConv;

    DECLARE_ABSTRACT_CLASS(wxArchiveClassFactory)
    DECLARE_NO_ASSIGN_CLASS(wxArchiveOutputStream)
};


/////////////////////////////////////////////////////////////////////////////
// wxArchiveIterator

#if wxUSE_STL
#include <iterator>

template <class X, class Y>
void wxSetArchiveIteratorValue(X& val, Y entry) {
    val = X(entry);
}
template <class X, class Y, class Z>
void wxSetArchiveIteratorValue(std::pair<X, Y>& val, Z entry) {
    val = std::make_pair(X(entry->GetInternalName()), Y(entry));
}

template <class Arc, class T = typename Arc::entry_type*>
class wxArchiveIterator : public std::iterator<std::input_iterator_tag, T>
{
public:
    wxArchiveIterator() : m_rep(NULL) { }

    wxArchiveIterator(Arc& arc) {
        typename Arc::entry_type* entry = arc.GetEntry();
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
        if (m_rep)
            m_rep.UnRef();
        m_rep = it.m_rep;
        if (m_rep)
            m_rep.AddRef();
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
            typename Arc::entry_type* entry = m_arc.GetEntry();
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
                wxSetArchiveIteratorValue(m_value, m_entry);
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
