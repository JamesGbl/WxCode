/////////////////////////////////////////////////////////////////////////////
// Name:        zipstrm.h
// Purpose:     Streams for Zip files
// Author:      Mike Wetherell
// RCS-ID:      $Id: zipstrm2.h,v 1.2 2004-07-14 18:24:21 chiclero Exp $
// Copyright:   (c) Mike Wetherell
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WXZIPSTREAM2_H__
#define _WX_WXZIPSTREAM2_H__

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "zipstrm2.h"
#endif

#include "wx/defs.h"

#if wxUSE_ZLIB && wxUSE_STREAMS // && wxUSE_ZIPSTREAM

#include "wx/archive.h"
#include "wx/buffer.h"
#include "wx/hashmap.h"
#include "wx/filename.h"


/////////////////////////////////////////////////////////////////////////////
// constants

// Compression Method, only 0 (store) and 8 (deflate) are supported here
//
enum wxZipMethod
{
    wxZIP_METHOD_STORE,
    wxZIP_METHOD_SHRINK,
    wxZIP_METHOD_REDUCE1,
    wxZIP_METHOD_REDUCE2,
    wxZIP_METHOD_REDUCE3,
    wxZIP_METHOD_REDUCE4,
    wxZIP_METHOD_IMPLODE,
    wxZIP_METHOD_TOKENIZE,
    wxZIP_METHOD_DEFLATE,
    wxZIP_METHOD_DEFLATE64,
    wxZIP_METHOD_BZIP2 = 12,
    wxZIP_METHOD_DEFAULT = 0xffff
};

// Originating File-System.
//
enum wxZipSystem
{
    wxZIP_SYSTEM_MSDOS,
    wxZIP_SYSTEM_AMIGA,
    wxZIP_SYSTEM_OPENVMS,
    wxZIP_SYSTEM_UNIX,
    wxZIP_SYSTEM_VM_CMS,
    wxZIP_SYSTEM_ATARI_ST,
    wxZIP_SYSTEM_OS2_HPFS,
    wxZIP_SYSTEM_MACINTOSH,
    wxZIP_SYSTEM_Z_SYSTEM,
    wxZIP_SYSTEM_CPM,
    wxZIP_SYSTEM_WINDOWS_NTFS,
    wxZIP_SYSTEM_MVS,
    wxZIP_SYSTEM_VSE,
    wxZIP_SYSTEM_ACORN_RISC,
    wxZIP_SYSTEM_VFAT,
    wxZIP_SYSTEM_ALTERNATE_MVS,
    wxZIP_SYSTEM_BEOS,
    wxZIP_SYSTEM_TANDEM,
    wxZIP_SYSTEM_OS_400
};

// Dos/Win file attributes
//
enum wxZipAttributes
{
    wxZIP_A_RDONLY = 0x01,
    wxZIP_A_HIDDEN = 0x02,
    wxZIP_A_SYSTEM = 0x04,
    wxZIP_A_SUBDIR = 0x10,
    wxZIP_A_ARCH   = 0x20,

    wxZIP_A_MASK   = 0x37
};

// Values for the flags field in the zip headers
//
enum wxZipFlags
{
    wxZIP_ENCRYPTED         = 0x0001,
    wxZIP_DEFLATE_NORMAL    = 0x0000,   // normal compression
    wxZIP_DEFLATE_EXTRA     = 0x0002,   // extra compression
    wxZIP_DEFLATE_FAST      = 0x0004,   // fast compression
    wxZIP_DEFLATE_SUPERFAST = 0x0006,   // superfast compression
    wxZIP_DEFLATE_MASK      = 0x0006,
    wxZIP_SUMS_FOLLOW       = 0x0008,   // crc and sizes come after the data
    wxZIP_ENHANCED          = 0x0010,
    wxZIP_PATCH             = 0x0020,
    wxZIP_STRONG_ENC        = 0x0040,
    wxZIP_UNUSED            = 0x0F80,
    wxZIP_RESERVED          = 0xF000
};

// Forward decls
//
class wxZipEntry;
class wxZipExtra;
class wxZipInputStream2;
class wxZipOutputStream2;


/////////////////////////////////////////////////////////////////////////////
// wxZipNotifier

class wxZipNotifier
{
public:
    virtual ~wxZipNotifier() { }

    virtual void OnEntryChanged(wxZipEntry& entry) = 0;
    virtual void OnExtraChanged(wxZipExtra& extra) = 0;
};


/////////////////////////////////////////////////////////////////////////////
// Internal use - pointer that can't be copied (copies are null)

class _wxZipInputStreamPtr
{
public:
    _wxZipInputStreamPtr()
        : m_p(NULL) { }
    _wxZipInputStreamPtr(const _wxZipInputStreamPtr& WXUNUSED(p))
        : m_p(NULL) { }

    _wxZipInputStreamPtr& operator =(const _wxZipInputStreamPtr& WXUNUSED(p))
        { m_p = NULL; return *this; }
    _wxZipInputStreamPtr& operator =(wxZipInputStream2 *p)
        { m_p = p; return *this; }
    wxZipInputStream2 *operator ->()
        { return m_p; }
    operator bool()
        { return m_p != NULL; }

private:
    wxZipInputStream2 *m_p;
};


/////////////////////////////////////////////////////////////////////////////
// Zip Entry - holds the meta data for a file in the zip

class wxZipEntry : public wxArchiveEntry
{
public:
    wxZipEntry(const wxString& name = wxEmptyString,
               const wxDateTime& dt = wxDateTime::Now());
    virtual ~wxZipEntry();

    // Get accessors
    wxDateTime  GetDateTime() const           { return m_DateTime; }
    off_t       GetSize() const               { return m_Size; }
    off_t       GetOffset() const             { return m_Offset; }
    bool        IsDir() const                 { return m_IsDir; }
    wxString    GetInternalName() const       { return m_Name; }
    int         GetMethod() const             { return m_Method; }
    int         GetFlags() const              { return m_Flags; }
    wxUint32    GetCrc() const                { return m_Crc; }
    off_t       GetCompressedSize() const     { return m_CompressedSize; }
    const char *GetLocalExtra() const         { return m_LocalExtra; }
    size_t      GetLocalExtraLen() const      { return m_LocalExtra.GetDataLen(); }
    const char *GetExtra() const              { return m_Extra; }
    size_t      GetExtraLen() const           { return m_Extra.GetDataLen(); }
    int         GetSystemMadeBy() const       { return m_SystemMadeBy; }
    wxString    GetComment() const            { return m_Comment; }
    wxUint32    GetExternalAttributes() const { return m_ExternalAttributes; }
    int         GetMode() const;
    wxString    GetName(wxPathFormat format = wxPATH_NATIVE) const;

    // is accessors
    inline bool IsText() const;
    inline bool IsReadOnly() const;
    inline bool IsMadeByUnix() const;

    // set accessors
    void SetDateTime(const wxDateTime& dt)    { m_DateTime = dt; }
    void SetSize(off_t size)                  { m_Size = size; }
    void SetMethod(int method)                { m_Method = method; }
    void SetComment(const wxString& comment)  { m_Comment = comment; }
    void SetExternalAttributes(wxUint32 attr) { m_ExternalAttributes = attr; }
    void SetSystemMadeBy(int system);
    void SetMode(int mode);
    void SetExtra(const char *extra, size_t len);
    void SetLocalExtra(const char *extra, size_t len);
    void SetName(const wxString& name, wxPathFormat format = wxPATH_NATIVE);

    // set is accessors
    void SetIsDir(bool isDir = true);
    inline void SetIsReadOnly(bool isReadOnly = true);
    inline void SetIsText(bool isText = true);

    wxZipEntry *Clone() const                 { return ZipClone(); }

    inline void SetNotifier(wxZipNotifier& notifier);
    inline void UnsetNotifier();

protected:
    // Internal attributes
    enum { TEXT_ATTR = 1 };

    // protected Get accessors
    int GetVersionNeeded() const              { return m_VersionNeeded; }
    off_t GetKey() const                      { return m_Key; }
    int GetVersionMadeBy() const              { return m_VersionMadeBy; }
    int GetDiskStart() const                  { return m_DiskStart; }
    int GetInternalAttributes() const         { return m_InternalAttributes; }

    void SetVersionNeeded(int version)        { m_VersionNeeded = version; }
    void SetOffset(off_t offset)              { m_Offset = offset; }
    void SetFlags(int flags)                  { m_Flags = flags; }
    void SetVersionMadeBy(int version)        { m_VersionMadeBy = version; }
    void SetCrc(wxUint32 crc)                 { m_Crc = crc; }
    void SetCompressedSize(off_t size)        { m_CompressedSize = size; }
    void SetKey(off_t offset)                 { m_Key = offset; }
    void SetDiskStart(int start)              { m_DiskStart = start; }
    void SetInternalAttributes(int attr)      { m_InternalAttributes = attr; }
    void SetExtra(const wxMemoryBuffer& e)    { m_Extra = e; };
    inline void SetLocalExtra(const wxMemoryBuffer& extra);

    virtual wxZipEntry *ZipClone() const      { return new wxZipEntry(*this); }

    inline void Notify();

private:
    wxArchiveEntry* DoClone() const           { return ZipClone(); }

    size_t ReadLocal(wxInputStream& stream, wxMBConv& conv);
    size_t WriteLocal(wxOutputStream& stream, wxMBConv& conv) const;

    size_t ReadCentral(wxInputStream& stream, wxMBConv& conv);
    size_t WriteCentral(wxOutputStream& stream, wxMBConv& conv) const;

    void ReadDescriptor(wxInputStream& stream);

    wxUint8    m_SystemMadeBy;       // one of enum wxZipSystem
    wxUint8    m_VersionMadeBy;      // major * 10 + minor

    wxUint16   m_VersionNeeded;      // ver needed to extract (20 i.e. v2.0)
    wxUint16   m_Flags;
    wxUint16   m_Method;             // compression method (one of wxZipMethod)
    wxDateTime m_DateTime;
    wxUint32   m_Crc;
    off_t      m_CompressedSize;
    off_t      m_Size;
    wxString   m_Name;               // in internal format
    off_t      m_Key;                // the original offset for copied entries
    off_t      m_Offset;             // file offset of the entry
    bool       m_IsDir;

    wxString   m_Comment;
    wxUint16   m_DiskStart;          // for multidisk archives, not unsupported
    wxUint16   m_InternalAttributes; // bit 0 set for text files
    wxUint32   m_ExternalAttributes; // system specific depends on SystemMadeBy

    wxMemoryBuffer m_Extra;
    wxMemoryBuffer m_LocalExtra;
    bool           m_IsLocalExtraSet;

    wxZipNotifier *m_zipnotifier;

    _wxZipInputStreamPtr m_backref;

    friend class wxZipInputStream2;
    friend class wxZipOutputStream2;
    friend class wxRawInputStream;

    DECLARE_DYNAMIC_CLASS(wxZipEntry)
};


/////////////////////////////////////////////////////////////////////////////
// wxZipExtra - holds additional meta data for the zip as a whole

class wxZipExtra : public wxArchiveExtra
{
public:
    wxZipExtra();
    virtual ~wxZipExtra();

    wxString GetComment() const               { return m_Comment; }
    void SetComment(const wxString& comment)  { m_Comment = comment; }
    int GetTotalEntries() const               { return m_TotalEntries; }

    inline void SetNotifier(wxZipNotifier& notifier);
    inline void UnsetNotifier();

    wxZipExtra *Clone() const                 { return ZipClone(); }

protected:
    int GetDiskNumber() const                 { return m_DiskNumber; }
    int GetStartDisk() const                  { return m_StartDisk; }
    int GetEntriesHere() const                { return m_EntriesHere; }
    off_t GetSize() const                     { return m_Size; }
    off_t GetOffset() const                   { return m_Offset; }

    void SetDiskNumber(int num)               { m_DiskNumber = num; }
    void SetStartDisk(int num)                { m_StartDisk = num; }
    void SetEntriesHere(int num)              { m_EntriesHere = num; }
    void SetTotalEntries(int num)             { m_TotalEntries = num; }
    void SetSize(off_t size)                  { m_Size = (wxUint32)size; }
    void SetOffset(off_t offset)              { m_Offset = (wxUint32)offset; }

    inline void Notify();

    virtual wxZipExtra *ZipClone() const      { return new wxZipExtra(*this); }

private:
    wxArchiveExtra* DoClone() const           { return ZipClone(); }

    bool Read(wxInputStream& stream, wxMBConv& conv);
    bool Write(wxOutputStream& stream, wxMBConv& conv) const;

    wxUint16 m_DiskNumber;
    wxUint16 m_StartDisk;
    wxUint16 m_EntriesHere;
    wxUint16 m_TotalEntries;
    wxUint32 m_Size;
    wxUint32 m_Offset;
    wxString m_Comment;

    wxZipNotifier *m_zipnotifier;

    _wxZipInputStreamPtr m_backref;

    friend class wxZipInputStream2;
    friend class wxZipOutputStream2;

    DECLARE_DYNAMIC_CLASS(wxZipExtra)
};


/////////////////////////////////////////////////////////////////////////////
// wxZipInputStream2 

WX_DECLARE_HASH_MAP(long, wxZipEntry*, wxIntegerHash,
                    wxIntegerEqual, _wxOffsetZipEntryMap);

class wxZipInputStream2 : public wxArchiveInputStream
{
public:
    typedef wxZipEntry entry_type;

    wxZipInputStream2(wxInputStream& stream, wxMBConv& conv = wxConvFile);
    virtual ~wxZipInputStream2();

    bool OpenEntry(wxZipEntry& entry)       { return DoOpen(&entry); }
    bool CloseEntry();

    wxZipEntry *GetNextEntry();
    wxZipExtra *GetExtra();

protected:
    size_t OnSysRead(void *buffer, size_t size);
    off_t OnSysTell() const { return m_decomp ? m_decomp->TellI() : 0; }

    virtual wxInputStream *CreateDecompressor(wxInputStream& stream);

private:
    wxArchiveEntry *DoGetNextEntry()        { return GetNextEntry(); }
    wxArchiveExtra *DoGetExtra()            { return GetExtra(); }

    bool OpenEntry(wxArchiveEntry& entry);

    wxStreamError ReadLocal();
    wxStreamError ReadCentral();

    wxUint32 ReadSignature();
    bool FindEndRecord();

    void AddRef(wxZipEntry *entry);
    wxZipEntry *GetRef(off_t key);
    void ReleaseRef(wxZipEntry *entry);
    void ReleaseRef(wxZipExtra *extra);

    bool AtHeader() const       { return m_headerSize == 0; }
    bool AfterHeader() const    { return m_headerSize > 0 && !m_decomp; }
    bool IsOpened() const       { return m_decomp != NULL; }

    bool DoOpen(wxZipEntry *entry = NULL, bool raw = false);
    bool ImplOpen();

    wxZipEntry m_entry;
    wxZipExtra *m_extra;
    bool m_ownsExtra;
    bool m_raw;
    size_t m_headerSize;
    wxUint32 m_crcAccumulator;
    wxInputStream *m_decomp;
    bool m_parentSeekable;
    _wxOffsetZipEntryMap m_weakrefs;
    off_t m_offsetAdjustment;
    off_t m_position;
    wxUint32 m_signature;

    friend wxZipEntry::~wxZipEntry();
    friend wxZipExtra::~wxZipExtra();
    friend class wxZipOutputStream2;

    DECLARE_NO_COPY_CLASS(wxZipInputStream2)
};


/////////////////////////////////////////////////////////////////////////////
// wxZipOutputStream2 

WX_DEFINE_ARRAY_PTR(wxZipEntry*, _wxZipEntryArray);

class wxZipOutputStream2 : public wxArchiveOutputStream
{
public:
    wxZipOutputStream2(wxOutputStream& stream,
                       int level = -1,
                       wxMBConv& conv = wxConvFile);
    virtual ~wxZipOutputStream2();

    bool PutNextEntry(wxZipEntry *entry) { return DoCreate(entry); }

    bool PutNextEntry(const wxString& name,
                      const wxDateTime& dt = wxDateTime::Now(),
                      off_t size = wxInvalidOffset);

    bool PutNextDirEntry(const wxString& name,
                         const wxDateTime& dt = wxDateTime::Now());

    bool CopyEntry(wxZipEntry *entry, wxZipInputStream2& inputStream);

    void Sync();
    bool CloseEntry();
    bool Close();

    void SetExtra(wxZipExtra *extra)    { delete m_extra; m_extra = extra; }

    int  GetLevel() const               { return m_level; }
    void SetLevel(int level)            { m_level = level; }
    
protected:
    virtual size_t OnSysWrite(const void *buffer, size_t size);
    virtual off_t OnSysTell() const { return m_entrySize; }

    struct Buffer { const char *m_data; size_t m_size; };
    virtual wxOutputStream *CreateCompressor(wxOutputStream& stream,
                                             wxZipEntry& entry,
                                             const Buffer bufs[]);

    bool GetIsParentSeekable() const { return m_parentSeekable == 1; }

private:
    bool PutNextEntry(wxArchiveEntry *entry);
    bool CopyEntry(wxArchiveEntry *entry, wxArchiveInputStream& stream);
    void SetExtra(wxArchiveExtra *extra);

    bool IsOpened() const { return m_comp || m_pending; }

    bool DoCreate(wxZipEntry *entry, bool raw = false);
    void CreatePendingEntry(const void *buffer, size_t size);
    void CreatePendingEntry();

    class wxStoredOutputStream *m_store;
    _wxZipEntryArray m_entries;
    char *m_initialData;
    size_t m_initialSize;
    wxZipEntry *m_pending;
    int m_raw;
    char *m_rawsums;
    off_t m_headerOffset;
    size_t m_headerSize;
    off_t m_entrySize;
    wxUint32 m_crcAccumulator;
    wxOutputStream *m_comp;
    int m_level;
    int m_parentSeekable;
    wxZipExtra *m_extra;

    DECLARE_NO_COPY_CLASS(wxZipOutputStream2)
};


/////////////////////////////////////////////////////////////////////////////
// wxZipClassFactory

class wxZipClassFactory : public wxArchiveClassFactory
{
public:
    wxZipEntry *NewEntry()
        { return new wxZipEntry; }
    wxZipInputStream2 *NewStream(wxInputStream& stream)
        { return new wxZipInputStream2(stream, GetConv()); }
    wxZipOutputStream2 *NewStream(wxOutputStream& stream)
        { return new wxZipOutputStream2(stream, -1, GetConv()); }

protected:
    wxArchiveEntry *DoNewEntry()
        { return NewEntry(); }
    wxArchiveInputStream *DoNewStream(wxInputStream& stream)
        { return NewStream(stream); }
    wxArchiveOutputStream *DoNewStream(wxOutputStream& stream)
        { return NewStream(stream); }

private:
    DECLARE_DYNAMIC_CLASS(wxZipClassFactory)
};

#if wxUSE_STL
typedef wxArchiveIterator<wxZipInputStream2> wxZipIter;
typedef wxArchiveIterator<wxZipInputStream2,
         std::pair<wxString, wxZipEntry*> >  wxZipPairIter;
#endif


/////////////////////////////////////////////////////////////////////////////
// wxZipEntry inlines

bool wxZipEntry::IsText() const
{
    return (m_InternalAttributes & TEXT_ATTR) != 0;
}

bool wxZipEntry::IsReadOnly() const
{
    return (m_ExternalAttributes & wxZIP_A_RDONLY) != 0;
}

void wxZipEntry::SetIsText(bool isText)
{
    if (isText)
        m_InternalAttributes |= TEXT_ATTR;
    else
        m_InternalAttributes &= ~TEXT_ATTR;
}

void wxZipEntry::SetIsReadOnly(bool isReadOnly)
{
    if (isReadOnly)
        SetMode(GetMode() & ~0222);
    else
        SetMode(GetMode() | 0200);
}

bool wxZipEntry::IsMadeByUnix() const
{
    switch (m_SystemMadeBy) {
        case wxZIP_SYSTEM_OPENVMS:
        case wxZIP_SYSTEM_UNIX:
        case wxZIP_SYSTEM_ATARI_ST:
        case wxZIP_SYSTEM_ACORN_RISC:
        case wxZIP_SYSTEM_BEOS:
        case wxZIP_SYSTEM_TANDEM:
            return true;
    }
    // note: some unix zippers put madeby = dos
    return m_SystemMadeBy == wxZIP_SYSTEM_MSDOS
           && (m_ExternalAttributes >> 16);
}

void wxZipEntry::SetLocalExtra(const wxMemoryBuffer& extra)
{
    m_LocalExtra = extra;
    m_IsLocalExtraSet = true;
}

void wxZipEntry::SetNotifier(wxZipNotifier& notifier)
{
    wxArchiveEntry::UnsetNotifier();
    m_zipnotifier = &notifier;
    m_zipnotifier->OnEntryChanged(*this);
}

void wxZipEntry::Notify()
{
    if (m_zipnotifier)
        m_zipnotifier->OnEntryChanged(*this);
    else if (GetNotifier())
        GetNotifier()->OnEntryChanged(*this);
}

void wxZipEntry::UnsetNotifier()
{
    wxArchiveEntry::UnsetNotifier();
    m_zipnotifier = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// wxZipExtra inlines

void wxZipExtra::SetNotifier(wxZipNotifier& notifier)
{
    m_zipnotifier = &notifier;
    m_zipnotifier->OnExtraChanged(*this);
}

void wxZipExtra::Notify()
{
    if (m_zipnotifier)
        m_zipnotifier->OnExtraChanged(*this);
}

void wxZipExtra::UnsetNotifier()
{
    m_zipnotifier = NULL;
}

#endif // wxUSE_ZLIB && wxUSE_STREAMS && wxUSE_ZIPSTREAM

#endif // _WX_WXZIPSTREAM2_H__
