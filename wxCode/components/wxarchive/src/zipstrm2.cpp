/////////////////////////////////////////////////////////////////////////////
// Name:        zipstrm.cpp
// Purpose:     Streams for Zip files
// Author:      Mike Wetherell
// RCS-ID:      $Id: zipstrm2.cpp,v 1.4 2004-07-17 21:47:59 chiclero Exp $
// Copyright:   (c) Mike Wetherell
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
  #pragma implementation "zipstrm2.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#ifndef WX_PRECOMP
  #include "wx/defs.h"
#endif

#if wxUSE_ZLIB && wxUSE_STREAMS // && wxUSE_ZIPSTREAM

#include "wx/zipstrm2.h"
#include "wx/log.h"
#include "wx/intl.h"
#include "wx/datstrm.h"
#include "wx/zstream.h"
#include "wx/mstream.h"
#include "wx/utils.h"
#include "wx/ptr_scpd.h"

// FIXME
#if defined(__WXMSW__) && !defined(__WX_SETUP_H__) && !defined(wxUSE_ZLIB_H_IN_PATH)
   #include "../src/zlib/zlib.h"
#else
   #include "zlib.h"
#endif

// value for the 'version needed to extract' field (20 means 2.0)
enum {
    VERSION_NEEDED_TO_EXTRACT = 20
};

// signatures for the various records (PKxx)
enum {
    CENTRAL_MAGIC = 0x02014b50,     // central directory record
    LOCAL_MAGIC   = 0x04034b50,     // local header
    END_MAGIC     = 0x06054b50,     // end of central directory record
    SUMS_MAGIC    = 0x08074b50      // data descriptor
};

// unix file attributes. zip stores them in the high 16 bits of the
// 'external attributes' field, hence the extra zeros.
enum {
    wxZIP_S_IFDIR = 0x40000000,
    wxZIP_S_IFREG = 0x80000000
};

// minimum sizes for the various records
enum {
    CENTRAL_SIZE  = 46,
    LOCAL_SIZE    = 30,
    SUMS_SIZE     = 12
};

// The number of bytes that must be written to an wxZipOutputStream2 before
// a zip entry is created. The purpose of this latency is so that
// CreateCompressor() can see a little data before deciding which compressor
// it should use.
enum {
    OUTPUT_LATENCY = 4096
};

// Some offsets into the local header
enum {
    FLAGS_OFFSET = 6,
    SUMS_OFFSET  = 14
};

// When writing raw will the data stream have sums (i.e. crc & size) appended?
enum {
    RAW_NORMAL = 1,
    RAW_SUMS_FOLLOW = 2
};

// wxZipOutputStream2 needs to know whether the parent stream is seekable
enum {
    SEEK_NO,      // parent is not seekable
    SEEK_YES,     // parent is seekable
    SEEK_MAYBE    // unknown, will try seeking and check for error
};


IMPLEMENT_DYNAMIC_CLASS(wxZipEntry, wxArchiveEntry)
IMPLEMENT_DYNAMIC_CLASS(wxZipExtra, wxArchiveExtra)
IMPLEMENT_DYNAMIC_CLASS(wxZipClassFactory, wxArchiveClassFactory)


/////////////////////////////////////////////////////////////////////////////
// Helpers

// read a string of a given length
//
static wxString ReadString(wxInputStream& stream, wxUint16 len, wxMBConv& conv)
{
    if (!len)
        return wxEmptyString;

#if wxUSE_UNICODE
    wxCharBuffer buf(len);
    if (!buf)
        return wxEmptyString;
    stream.Read(buf.data(), len);
    wxString str(buf, conv);
#else
    wxString str;
    (void)conv;
    {
        wxStringBuffer buf(str, len);
        stream.Read(buf, len);
    }
#endif

    return str;
}


// Decode a little endian wxUint32 number from a character array
//
inline wxUint32 CrackUint32(const char *m)
{
    const unsigned char *n = (const unsigned char*)m;
    return (n[3] << 24) | (n[2] << 16) | (n[1] << 8) | n[0];
}


// Temporarily lower the logging level in debug mode to avoid a warning
// from SeekI about seeking on a stream with data written back to it.
//
inline off_t QuietSeek(wxInputStream& stream, off_t pos)
{
#ifdef __WXDEBUG__
    wxLogLevel level = wxLog::GetLogLevel();
    wxLog::SetLogLevel(wxLOG_Debug - 1);
    off_t result = stream.SeekI(pos);
    wxLog::SetLogLevel(level);
    return result;
#else
    return stream.SeekI(pos);
#endif
}


/////////////////////////////////////////////////////////////////////////////
// Stored input stream
// Trival decompressor for files which are 'stored' in the zip file.

class wxStoredInputStream : public wxFilterInputStream
{
public:
    wxStoredInputStream(wxInputStream& stream, off_t len, bool readsums);

    virtual char Peek() { return wxInputStream::Peek(); }
    virtual size_t GetSize() const { return m_len; }

protected:
    virtual size_t OnSysRead(void *buffer, size_t size);
    virtual off_t OnSysTell() const { return m_pos; }

private:
    off_t m_pos;
    off_t m_len;
    bool  m_readsums;

    DECLARE_NO_COPY_CLASS(wxStoredInputStream)
};


wxStoredInputStream::wxStoredInputStream(wxInputStream& stream,
                                         off_t len,
                                         bool readsums)
  : wxFilterInputStream(stream),
    m_pos(0),
    m_len(len),
    m_readsums(readsums)
{
}


size_t wxStoredInputStream::OnSysRead(void *buffer, size_t size)
{
    size_t count = wxMin(size, m_len - m_pos + (size_t)0);
    count = m_parent_i_stream->Read(buffer, count).LastRead();
    m_pos += count;

    if (m_pos == m_len) {
        if (m_readsums) {
            char magic[4];
            m_parent_i_stream->Read(magic, sizeof(magic));
            if (CrackUint32(magic) != SUMS_MAGIC)
                m_parent_i_stream->Ungetch(magic, sizeof(magic));
            m_len += SUMS_SIZE;
            m_readsums = false;
            if (count < size)
                count += OnSysRead((char*)buffer + count, size - count);
        } else {
            m_lasterror = wxSTREAM_EOF;
        }
    } else  if (!*m_parent_i_stream) {
        m_lasterror = wxSTREAM_READ_ERROR;
    }

    return count;
}


/////////////////////////////////////////////////////////////////////////////
// Stored output stream
// Trival compressor for files which are 'stored' in the zip file.

class wxStoredOutputStream : public wxFilterOutputStream
{
public:
    wxStoredOutputStream(wxOutputStream& stream) :
        wxFilterOutputStream(stream), m_pos(0) { }

    bool Close() {
        m_pos = 0;
        m_lasterror = wxSTREAM_NO_ERROR;
        return true;
    }

protected:
    virtual size_t OnSysWrite(const void *buffer, size_t size);
    virtual off_t OnSysTell() const { return m_pos; }

private:
    off_t m_pos;
    DECLARE_NO_COPY_CLASS(wxStoredOutputStream)
};


size_t wxStoredOutputStream::OnSysWrite(const void *buffer, size_t size)
{
    if (!IsOk() || !size)
        return 0;
    size_t count = m_parent_o_stream->Write(buffer, size).LastWrite();
    if (count != size)
        m_lasterror = wxSTREAM_WRITE_ERROR;
    m_pos += count;
    return count;
}


/////////////////////////////////////////////////////////////////////////////
// wxTeeInputStream & wxRawInputStream
//
// Used to handle the unusal case of raw copying an entry of unknown
// length. This can only happen when the zip being copied from is being
// read from a non-seekable stream, and also was original written to a
// non-seekable stream.
//
// In this case there's no option but to decompress the stream to find
// it's length, but we can still write the raw compressed data to avoid the
// compression overhead (which is the greater one).
//
// The two classes are used together something lke this:
//
//  wxTeeInputStream *tee = new wxTeeInputStream(*m_parent_i_stream);
//  m_decomp = new wxRawInputStream(CreateDecompressor(*tee), tee, m_entry);
//
// The role of the tee filter is something like the unix 'tee' command, it
// is a transparent filter, but allows the data read to be read a second
// time via an extra method 'GetData()'.
//
// The wxRawInputStream then draws data through the tee using a decompressor
// then instead of returning the decompressed data, retuns the raw data
// from the wxTeeInputStream::GetData().

class wxTeeInputStream : public wxFilterInputStream
{
public:
    wxTeeInputStream(wxInputStream& stream);

    size_t GetCount() const { return m_end - m_start; }
    size_t GetData(char *buffer, size_t size);
    void   Final() { m_end = m_buf.GetDataLen(); }

    wxInputStream& Read(void *buffer, size_t size);

protected:
    virtual size_t OnSysRead(void *buffer, size_t size);
    virtual off_t OnSysTell() const { return m_pos; }

private:
    off_t m_pos;
    wxMemoryBuffer m_buf;
    size_t m_start;
    size_t m_end;

    DECLARE_NO_COPY_CLASS(wxTeeInputStream)
};


wxTeeInputStream::wxTeeInputStream(wxInputStream& stream)
  : wxFilterInputStream(stream),
    m_pos(0), m_buf(8192), m_start(0), m_end(0)
{
}


wxInputStream& wxTeeInputStream::Read(void *buffer, size_t size)
{
    size_t count = wxInputStream::Read(buffer, size).LastRead();
    m_end = m_buf.GetDataLen();
    m_buf.AppendData(buffer, count);
    return *this;
}


size_t wxTeeInputStream::OnSysRead(void *buffer, size_t size)
{
    size_t count = m_parent_i_stream->Read(buffer, size).LastRead();
    m_lasterror = m_parent_i_stream->GetLastError();
    return count;
}


size_t wxTeeInputStream::GetData(char *buffer, size_t size)
{
    if (m_wbacksize) {
        size_t len = m_buf.GetDataLen();
        len = len > m_wbacksize ? len - m_wbacksize : 0;
        m_buf.SetDataLen(len);
        if (m_end > len) {
            wxFAIL; // we've already returned data that's now being ungot
            m_end = len;
        }
        m_parent_i_stream->Ungetch(m_wback, m_wbacksize);
        free(m_wback);
        m_wback = NULL;
        m_wbacksize = 0;
        m_wbackcur = 0;
    }

    if (size > GetCount())
        size = GetCount();
    if (size) {
        memcpy(buffer, m_buf + m_start, size);
        m_start += size;
        wxASSERT(m_start <= m_end);
    }

    if (m_start == m_end && m_start > 0 && m_buf.GetDataLen() > 0) {
        size_t len = m_buf.GetDataLen();
        char *buf = (char*)m_buf.GetWriteBuf(len);
        len -= m_end;
        memmove(buf, buf + m_end, len);
        m_buf.UngetWriteBuf(len);
        m_start = m_end = 0;
    }

    return size;
}


class wxRawInputStream : public wxFilterInputStream
{
public:
    wxRawInputStream(wxInputStream *decomp,
                     wxTeeInputStream *tee,
                     wxZipEntry& entry);

    virtual ~wxRawInputStream() { delete m_parent_i_stream; delete m_tee; }

protected:
    virtual size_t OnSysRead(void *buffer, size_t size);
    virtual off_t OnSysTell() const { return m_pos; }

private:
    off_t m_pos;
    wxTeeInputStream *m_tee;
    wxZipEntry *m_entry;

    enum { BUFSIZE = 8192 };
    wxCharBuffer m_dummy;

    DECLARE_NO_COPY_CLASS(wxRawInputStream)
};


wxRawInputStream::wxRawInputStream(wxInputStream *decomp,
                                   wxTeeInputStream *tee,
                                   wxZipEntry& entry)
  : wxFilterInputStream(*decomp),
    m_pos(0),
    m_tee(tee),
    m_entry(&entry),
    m_dummy(BUFSIZE)
{
}


size_t wxRawInputStream::OnSysRead(void *buffer, size_t size)
{
    char *buf = (char*)buffer;
    size_t count = 0;

    while (count < size && IsOk())
    {
        while (m_parent_i_stream->IsOk() && m_tee->GetCount() == 0)
            m_parent_i_stream->Read(m_dummy.data(), BUFSIZE);

        size_t n = m_tee->GetData(buf + count, size - count);
        count += n;

        if (n == 0) {
            if (m_entry) {
                m_entry->ReadDescriptor(*m_tee);
                m_tee->Final();
                m_entry = NULL;
            } else {
                m_lasterror = m_parent_i_stream->GetLastError();
            }
        }
    }

    m_pos += count;
    return count;
}


/////////////////////////////////////////////////////////////////////////////
// ZipEntry

wxZipEntry::wxZipEntry(
    const wxString& name /*=wxEmptyString*/,
    const wxDateTime& dt /*=wxDateTime::Now()*/)
  : 
    m_SystemMadeBy(wxZIP_SYSTEM_MSDOS),
    m_VersionMadeBy(wxMAJOR_VERSION * 10 + wxMINOR_VERSION),
    m_VersionNeeded(VERSION_NEEDED_TO_EXTRACT),
    m_Flags(0),
    m_Method(wxZIP_METHOD_DEFAULT),
    m_DateTime(dt),
    m_Crc(0),
    m_CompressedSize(wxInvalidOffset),
    m_Size(wxInvalidOffset),
    m_Key(wxInvalidOffset),
    m_Offset(wxInvalidOffset),
    m_IsDir(false),
    m_DiskStart(0),
    m_InternalAttributes(0),
    m_ExternalAttributes(0),
    m_Extra(0),
    m_LocalExtra(0),
    m_IsLocalExtraSet(false),
    m_zipnotifier(NULL)
{
    SetName(name);
}


wxZipEntry::~wxZipEntry()
{
    if (m_backref)
        m_backref->ReleaseRef(this);
}


wxString wxZipEntry::GetName(wxPathFormat format /*=wxPATH_NATIVE*/) const
{
    if (wxFileName::GetFormat(format) != wxPATH_UNIX) {
        if (m_IsDir && !m_Name.empty())
            return wxFileName(m_Name, _T(""), wxPATH_UNIX).GetFullPath(format);
        else
            return wxFileName(m_Name, wxPATH_UNIX).GetFullPath(format);
    } else {
        return m_IsDir && !m_Name.empty() ? m_Name + _T("/") : m_Name;
    }
}


// Internally tars and zips use forward slashes for the path separator,
// absolute paths aren't allowed, and directory names have a trailing slash.
// This function converts a path into this internal format, but without a
// trailing slash for a directory.
//
void wxZipEntry::SetName(const wxString& name,
                         wxPathFormat format /*=wxPATH_NATIVE*/)
{
    if (wxFileName::GetFormat(format) != wxPATH_UNIX)
        m_Name = wxFileName(name, format).GetFullPath(wxPATH_UNIX);
    else
        m_Name = name;

    SetIsDir(!m_Name.empty() && m_Name.Last() == '/');
    if (IsDir())
        m_Name.erase(m_Name.length() - 1);

    while (!m_Name.empty() && *m_Name.begin() == '/')
        m_Name.erase(0, 1);
    while (!m_Name.empty() && m_Name.compare(0, 2, _T("./")) == 0)
        m_Name.erase(0, 2);
    if (m_Name == _T(".") || m_Name == _T(".."))
        m_Name = wxEmptyString;
}


void wxZipEntry::SetSystemMadeBy(int system)
{
    int mode = GetMode();
    bool wasUnix = IsMadeByUnix();

    m_SystemMadeBy = system;

    if (!wasUnix && IsMadeByUnix()) {
        SetIsDir(IsDir());
        SetMode(mode);
    } else if (wasUnix && !IsMadeByUnix()) {
        m_ExternalAttributes &= 0xffff;
    }
}


void wxZipEntry::SetIsDir(bool isDir /*=true*/)
{
    m_IsDir = isDir;

    if (isDir)
        m_ExternalAttributes |= wxZIP_A_SUBDIR;
    else
        m_ExternalAttributes &= ~wxZIP_A_SUBDIR;

    if (IsMadeByUnix()) {
        if (isDir) {
            m_ExternalAttributes &= ~wxZIP_S_IFREG;
            m_ExternalAttributes |= wxZIP_S_IFDIR;
        } else {
            m_ExternalAttributes |= wxZIP_S_IFREG;
            m_ExternalAttributes &= ~wxZIP_S_IFDIR;
        }
    }
}


// Return unix style permission bits
//
int wxZipEntry::GetMode() const
{
    // return unix permissions if present
    if (IsMadeByUnix())
        return (m_ExternalAttributes >> 16) & 0777;

    // otherwise synthesize from the dos attribs
    int mode = 0644;
    if (m_ExternalAttributes & wxZIP_A_RDONLY)
        mode &= ~0200;
    if (m_ExternalAttributes & wxZIP_A_SUBDIR)
        mode |= 0111;

    return mode;
}


// Set unix permissions
//
void wxZipEntry::SetMode(int mode)
{
    // Set dos attrib bits to be compatible
    if (mode & 0222)
        m_ExternalAttributes &= ~wxZIP_A_RDONLY;
    else
        m_ExternalAttributes |= wxZIP_A_RDONLY;

    // set the actual unix permission bits if the system type allows
    if (IsMadeByUnix()) {
        m_ExternalAttributes &= ~(0777L << 16);
        m_ExternalAttributes |= (mode & 0777L) << 16;
    }
}


void wxZipEntry::SetExtra(const char *extra, size_t len)
{
    wxMemoryBuffer ex(len);
    if (len) {
        memcpy(ex.GetWriteBuf(len), extra, len);
        ex.UngetWriteBuf(len);
    }
    SetExtra(ex);
}


void wxZipEntry::SetLocalExtra(const char *extra, size_t len)
{
    wxMemoryBuffer ex(len);
    if (len) {
        memcpy(ex.GetWriteBuf(len), extra, len);
        ex.UngetWriteBuf(len);
    }
    SetLocalExtra(ex);
}


size_t wxZipEntry::ReadLocal(wxInputStream& stream, wxMBConv& conv)
{
    wxUint16 nameLen, extraLen;
    wxUint32 compressedSize, size, crc;

    wxDataInputStream ds(stream);

    ds >> m_VersionNeeded >> m_Flags >> m_Method;
    SetDateTime(wxDateTime().SetFromDOS(ds.Read32()));
    ds >> crc >> compressedSize >> size >> nameLen >> extraLen;

    if ((m_Flags & wxZIP_SUMS_FOLLOW) == 0) {
        m_Crc = crc;
        m_CompressedSize = compressedSize;
        SetSize(size);
    }

    SetName(ReadString(stream, nameLen, conv), wxPATH_UNIX);

    // note: wxMemoryBuffer shares the buffer completely, whereas what we
    // need here is 'copy on write'. So remember to always assign a new one
    // when m_Extra or m_LocalExtra change.
    m_LocalExtra = wxMemoryBuffer(extraLen);
    if (extraLen) {
        stream.Read(m_LocalExtra.GetWriteBuf(extraLen), extraLen);
        m_LocalExtra.UngetWriteBuf(stream.LastRead());
    }

    return LOCAL_SIZE + nameLen + extraLen;
}


size_t wxZipEntry::WriteLocal(wxOutputStream& stream, wxMBConv& conv) const
{
    wxString unixName = GetName(wxPATH_UNIX);
    const wxWX2MBbuf name_buf = conv.cWX2MB(unixName);
    const char *name = name_buf;
    if (!name) name = "";
    wxUint16 nameLen = strlen(name);

    wxDataOutputStream ds(stream);

    ds << LOCAL_MAGIC << m_VersionNeeded << m_Flags << m_Method;
    ds.Write32(GetDateTime().GetAsDOS());
    
    if (m_Flags & wxZIP_SUMS_FOLLOW) {
        ds.Write32(0);
        ds.Write32(0);
        ds.Write32(0);
    } else {
        ds << m_Crc << (wxUint32)m_CompressedSize << (wxUint32)GetSize();
    }
    ds << nameLen;
    ds.Write16(GetLocalExtraLen());

    stream.Write(name, nameLen);
    if (GetLocalExtraLen())
        stream.Write(m_LocalExtra, GetLocalExtraLen());

    return LOCAL_SIZE + nameLen + GetLocalExtraLen();
}


size_t wxZipEntry::ReadCentral(wxInputStream& stream, wxMBConv& conv)
{
    wxUint16 nameLen, extraLen, commentLen;

    wxDataInputStream ds(stream);

    ds >> m_VersionMadeBy >> m_SystemMadeBy;

    SetVersionNeeded(ds.Read16());
    SetFlags(ds.Read16());
    SetMethod(ds.Read16());
    SetDateTime(wxDateTime().SetFromDOS(ds.Read32()));
    SetCrc(ds.Read32());
    SetCompressedSize(ds.Read32());
    SetSize(ds.Read32());

    ds >> nameLen >> extraLen >> commentLen
       >> m_DiskStart >> m_InternalAttributes >> m_ExternalAttributes;
    SetOffset(ds.Read32());
 
    SetName(ReadString(stream, nameLen, conv), wxPATH_UNIX);

    m_Extra = wxMemoryBuffer(extraLen);
    if (extraLen) {
        stream.Read(m_Extra.GetWriteBuf(extraLen), extraLen);
        m_Extra.UngetWriteBuf(stream.LastRead());
    }

    m_Comment = ReadString(stream, commentLen, conv);

    return CENTRAL_SIZE + nameLen + extraLen + commentLen;
}


size_t wxZipEntry::WriteCentral(wxOutputStream& stream, wxMBConv& conv) const
{
    wxString unixName = GetName(wxPATH_UNIX);
    const wxWX2MBbuf name_buf = conv.cWX2MB(unixName);
    const char *name = name_buf;
    if (!name) name = "";
    wxUint16 nameLen = strlen(name);

    const wxWX2MBbuf comment_buf = conv.cWX2MB(m_Comment);
    const char *comment = comment_buf;
    if (!comment) comment = "";
    wxUint16 commentLen = strlen(comment);

    wxDataOutputStream ds(stream);

    ds << CENTRAL_MAGIC << m_VersionMadeBy << m_SystemMadeBy;

    ds.Write16(GetVersionNeeded());
    ds.Write16(GetFlags());
    ds.Write16(GetMethod());
    ds.Write32(GetDateTime().GetAsDOS());
    ds.Write32(GetCrc());
    ds.Write32(GetCompressedSize());
    ds.Write32(GetSize());
    ds.Write16(nameLen);
    ds.Write16(GetExtraLen());

    ds << commentLen << m_DiskStart << m_InternalAttributes
       << m_ExternalAttributes << (wxUint32)GetOffset();
 
    stream.Write(name, nameLen);
    if (GetExtraLen())
        stream.Write(GetExtra(), GetExtraLen());
    stream.Write(comment, commentLen);

    return CENTRAL_SIZE + nameLen + GetExtraLen() + commentLen;
}


size_t wxZipEntry::ReadDescriptor(wxInputStream& stream)
{
    wxDataInputStream ds(stream);
    wxUint32 n = ds.Read32();
    SetCrc(n == SUMS_MAGIC ? ds.Read32() : n);
    SetCompressedSize(ds.Read32());
    SetSize(ds.Read32());
    return n == SUMS_MAGIC ? SUMS_SIZE + 4 : SUMS_SIZE;
}


/////////////////////////////////////////////////////////////////////////////
// wxZipExtra - holds additional meta data for the zip as a whole

wxZipExtra::wxZipExtra()
  : m_DiskNumber(0),
    m_StartDisk(0),
    m_EntriesHere(0),
    m_TotalEntries(0),
    m_Size(0),
    m_Offset(0),
    m_zipnotifier(NULL)
{
}


wxZipExtra::~wxZipExtra()
{
    if (m_backref)
        m_backref->ReleaseRef(this);
}


bool wxZipExtra::Write(wxOutputStream& stream, wxMBConv& conv) const
{
    const wxWX2MBbuf comment_buf = conv.cWX2MB(m_Comment);
    const char *comment = comment_buf;
    if (!comment) comment = "";
    wxUint16 commentLen = strlen(comment);

    wxDataOutputStream ds(stream);

    ds << END_MAGIC << m_DiskNumber << m_StartDisk << m_EntriesHere
       << m_TotalEntries << m_Size << m_Offset << commentLen;

    stream.Write(comment, commentLen);

    return stream.IsOk();
}


bool wxZipExtra::Read(wxInputStream& stream, wxMBConv& conv)
{
    wxDataInputStream ds(stream);

    ds >> m_DiskNumber >> m_StartDisk >> m_EntriesHere
       >> m_TotalEntries >> m_Size >> m_Offset;

    m_Comment = ReadString(stream, ds.Read16(), conv);

    if (stream.IsOk())
        if (m_DiskNumber == 0 && m_StartDisk == 0 &&
                m_EntriesHere == m_TotalEntries)
            return true;
        else
            wxLogError(_("unsupported zip archive"));

    return false;
}


/////////////////////////////////////////////////////////////////////////////
// Input stream

wxDECLARE_SCOPED_PTR(wxZipEntry, _wxZipEntryPtr)
wxDEFINE_SCOPED_PTR (wxZipEntry, _wxZipEntryPtr)

wxZipInputStream2::wxZipInputStream2(wxInputStream& stream,
                                     wxMBConv& conv /*=wxConvFile*/)
  : wxArchiveInputStream(stream, conv),
    m_extra(new wxZipExtra),
    m_ownsExtra(true),
    m_raw(false),
    m_headerSize(0),
    m_decomp(NULL),
    m_parentSeekable(false),
    m_offsetAdjustment(0),
    m_position(0),
    m_signature(0)
{
    m_extra->m_backref = this;

    // First find the end-of-central-directory record.
    if (!FindEndRecord()) {
        // failed, so either this is a non-seekable stream (ok), or not a zip
        if (m_parentSeekable) {
            m_lasterror = wxSTREAM_READ_ERROR;
            wxLogError(_("invalid zip file"));
            return;
        }
        else {
            wxLogNull nolog;
            off_t pos = m_parent_i_stream->TellI();
            // FIXME
            //if (pos != wxInvalidOffset)
            if (pos >= 0 && pos <= LONG_MAX)
                m_offsetAdjustment = m_position = pos;
            return;
        }
    }

    // Read in the end record
    off_t endPos = m_parent_i_stream->TellI() - 4;
    if (!m_extra->Read(*m_parent_i_stream, GetConv())) {
        if (!*m_parent_i_stream) {
            m_lasterror = wxSTREAM_READ_ERROR;
            return;
        }
        // TODO: try this out
        wxLogWarning(_("assuming this is a multi-part zip concatenated"));
    }

    // Now find the central-directory. we have the file offset of
    // the CD, so look there first. 
    if (m_parent_i_stream->SeekI(m_extra->GetOffset()) != wxInvalidOffset &&
            ReadSignature() == CENTRAL_MAGIC) {
        m_signature = CENTRAL_MAGIC;
        m_position = m_extra->GetOffset();
        m_offsetAdjustment = 0;
        return;
    }
 
    // If it's not there, then it could be that the zip has been appended
    // to a self extractor, so take the CD size (also in m_extra), subtract
    // it from the file offset of the end-central-directory and look there.
    if (m_parent_i_stream->SeekI(endPos - m_extra->GetSize())
            != wxInvalidOffset && ReadSignature() == CENTRAL_MAGIC) {
        m_signature = CENTRAL_MAGIC;
        m_position = endPos - m_extra->GetSize();
        m_offsetAdjustment = m_position - m_extra->GetOffset();
        return;
    }

    wxLogError(_("can't find central directory in zip"));
    m_lasterror = wxSTREAM_READ_ERROR;
}


// Find the end-of-central-directory record.
// If found the stream will be positioned just past the 4 signature bytes.
//
bool wxZipInputStream2::FindEndRecord()
{
    // usually it's 22 bytes in size and the last thing in the file
    const int RECSIZE = 22;
    { 
        wxLogNull nolog;
        if (m_parent_i_stream->SeekI(-RECSIZE, wxFromEnd) == wxInvalidOffset)
            return false;
    }

    m_parentSeekable = true;
    m_signature = 0;
    char magic[4];
    if (m_parent_i_stream->Read(magic, 4).LastRead() != 4)
        return false;
    if ((m_signature = CrackUint32(magic)) == END_MAGIC)
        return true;

    // unfortunately, the record has a comment field that can be up to 65535
    // bytes in length, so if the signature not found then search backwards.
    off_t pos = m_parent_i_stream->TellI();
    const int BUFSIZE = 1024;
    wxCharBuffer buf(BUFSIZE);

    memcpy(buf.data(), magic, 3);
    off_t minpos = wxMax(pos - 65535L, 0);

    while (pos > minpos) {
        size_t len = pos - wxMax(pos - (BUFSIZE - 3), minpos);
        memcpy(buf.data() + len, buf, 3);
        pos -= len;
    
        if (m_parent_i_stream->SeekI(pos, wxFromStart) == wxInvalidOffset ||
                m_parent_i_stream->Read(buf.data(), len).LastRead() != len)
            return false;

        char *p = buf.data() + len;

        while (p-- > buf.data()) {
            if ((m_signature = CrackUint32(p)) == END_MAGIC) {
                size_t remainder = buf.data() + len - p;
                if (remainder > 4)
                    m_parent_i_stream->Ungetch(p + 4, remainder - 4);
                return true;
            }
        }
    }

    return false;
}


wxZipInputStream2::~wxZipInputStream2()
{
    delete m_decomp;

    _wxOffsetZipEntryMap::iterator it;

    for (it = m_weakrefs.begin(); it != m_weakrefs.end(); ++it) 
        it->second->m_backref = NULL;
    
    if (m_extra)
        if (m_ownsExtra)
            delete m_extra;
        else
            m_extra->m_backref = NULL;
}


void wxZipInputStream2::AddRef(wxZipEntry *entry)
{
    wxZipEntry*& slot = m_weakrefs[entry->GetKey()];
    wxCHECK_RET(slot == NULL, _T("two entries at the same offset"));
    slot = entry;
    entry->m_backref = this;
}


void wxZipInputStream2::ReleaseRef(wxZipEntry *entry)
{
    wxCHECK_RET(m_weakrefs.erase(entry->GetKey()),
               _T("weak reference not found"));
}


void wxZipInputStream2::ReleaseRef(wxZipExtra *WXUNUSED(extra))
{
    wxASSERT_MSG(m_extra, _T("weak reference not found"));
    m_extra = NULL;
}


wxZipEntry *wxZipInputStream2::GetRef(off_t key)
{
    _wxOffsetZipEntryMap::iterator it = m_weakrefs.find(key);
    return it != m_weakrefs.end() ?  it->second : NULL;
}


wxZipExtra *wxZipInputStream2::GetExtra()
{
    if (m_ownsExtra) {
        m_ownsExtra = false;
        return m_extra;
    }
    return NULL;
}


wxZipEntry *wxZipInputStream2::GetNextEntry()
{
    m_lasterror = m_parentSeekable ? ReadCentral() : ReadLocal();

    if (!IsOk())
        return NULL;

    _wxZipEntryPtr entry(new wxZipEntry(m_entry));
    AddRef(entry.get());

    return entry.release();
}


wxStreamError wxZipInputStream2::ReadCentral()
{
    if (IsOpened())
        CloseEntry();

    if (m_signature == END_MAGIC)
        return wxSTREAM_EOF;

    if (m_signature != CENTRAL_MAGIC) {
        wxLogError(_("error reading zip central directory"));
        return wxSTREAM_READ_ERROR;
    }

    if (QuietSeek(*m_parent_i_stream, m_position + 4) == wxInvalidOffset)
        return wxSTREAM_READ_ERROR;

    m_position += m_entry.ReadCentral(*m_parent_i_stream, GetConv());
    if (m_parent_i_stream->GetLastError() == wxSTREAM_READ_ERROR) {
        m_signature = 0;
        return wxSTREAM_READ_ERROR;
    }

    m_signature = ReadSignature();

    if (m_offsetAdjustment)
        m_entry.SetOffset(m_entry.GetOffset() + m_offsetAdjustment);
    m_entry.SetKey(m_entry.GetOffset());

    return wxSTREAM_NO_ERROR;
}


wxStreamError wxZipInputStream2::ReadLocal()
{
    if (!AtHeader())
        CloseEntry();

    wxUint32 signature = ReadSignature();

    while (signature == CENTRAL_MAGIC) {
        if (m_weakrefs.empty() && m_extra == NULL)
            return wxSTREAM_EOF;
        
        m_position += m_entry.ReadCentral(*m_parent_i_stream, GetConv());
        if (m_parent_i_stream->GetLastError() == wxSTREAM_READ_ERROR)
            return wxSTREAM_READ_ERROR;

        wxZipEntry *entry = GetRef(m_entry.GetOffset());
        if (entry) {
            entry->SetSystemMadeBy(m_entry.GetSystemMadeBy());
            entry->SetVersionMadeBy(m_entry.GetVersionMadeBy());
            entry->SetComment(m_entry.GetComment());
            entry->SetDiskStart(m_entry.GetDiskStart());
            entry->SetInternalAttributes(m_entry.GetInternalAttributes());
            entry->SetExternalAttributes(m_entry.GetExternalAttributes());
            entry->SetExtra(m_entry.m_Extra);
            entry->Notify();
            entry->m_backref = NULL;
            ReleaseRef(entry);
        }

        signature = ReadSignature();
    }

    if (signature == END_MAGIC) {
        if (m_extra) {
            m_extra->Read(*m_parent_i_stream, GetConv());
            m_extra->Notify();
        }
        return wxSTREAM_EOF;
    }
            
    if (signature != LOCAL_MAGIC) {
        wxLogError(_("error reading zip local header"));
        return wxSTREAM_READ_ERROR;
    }

    m_headerSize = m_entry.ReadLocal(*m_parent_i_stream, GetConv());
    m_entry.SetOffset(m_position);
    m_entry.SetKey(m_position);

    return m_parent_i_stream->GetLastError() == wxSTREAM_READ_ERROR ?
        wxSTREAM_READ_ERROR : wxSTREAM_NO_ERROR;
}


wxUint32 wxZipInputStream2::ReadSignature()
{
    char magic[4];
    m_parent_i_stream->Read(magic, 4);
    return m_parent_i_stream->LastRead() == 4 ? CrackUint32(magic) : 0;
}


bool wxZipInputStream2::OpenEntry(wxArchiveEntry& entry)
{
    wxZipEntry *zipEntry = wxStaticCast(&entry, wxZipEntry);
    return zipEntry ? OpenEntry(*zipEntry) : false;
}


// Open an entry
//
bool wxZipInputStream2::DoOpen(wxZipEntry *entry, bool raw)
{
    if (m_lasterror == wxSTREAM_READ_ERROR)
        return false;
    wxCHECK(!IsOpened(), false);

    m_raw = raw;

    if (entry) {
        if (AfterHeader() && entry->GetKey() == m_entry.GetOffset())
            return true;
        // can only open the current entry on a non-seekable stream
        wxCHECK(m_parentSeekable, false);
    }
    
    m_lasterror = wxSTREAM_READ_ERROR;
    
    if (entry)
        m_entry = *entry;

    if (m_parentSeekable) {
        if (QuietSeek(*m_parent_i_stream, m_entry.GetOffset())
                == wxInvalidOffset)
            return false;
        if (ReadSignature() != LOCAL_MAGIC) {
            wxLogError(_("bad zipfile offset to entry"));
            return false;
        }
    }

    if (m_parentSeekable || AtHeader()) {
        m_headerSize = m_entry.ReadLocal(*m_parent_i_stream, GetConv());
        if (m_parentSeekable) {
            if (entry && !entry->m_IsLocalExtraSet) {
                entry->SetLocalExtra(m_entry.m_LocalExtra);
                entry->Notify();
            }
            wxZipEntry *ref = GetRef(m_entry.GetKey());
            if (ref && !ref->m_IsLocalExtraSet) {
                ref->SetLocalExtra(m_entry.m_LocalExtra);
                ref->Notify();
            }
        }
    }

    m_lasterror = m_parent_i_stream->GetLastError();
    return IsOk();
}


bool wxZipInputStream2::ImplOpen()
{
    wxASSERT(AfterHeader());

    if (m_raw) {
        if (m_entry.GetCompressedSize() != wxInvalidOffset) {
            m_decomp = new wxStoredInputStream(
                                *m_parent_i_stream,
                                m_entry.GetCompressedSize(),
                                (m_entry.GetFlags() & wxZIP_SUMS_FOLLOW) != 0);
        } else {
            wxTeeInputStream *tee = new wxTeeInputStream(*m_parent_i_stream);

            m_decomp = new wxRawInputStream(
                                CreateDecompressor(*tee),
                                tee,
                                m_entry);
        }
    } else {
        m_decomp = CreateDecompressor(*m_parent_i_stream);
    }

    m_crcAccumulator = crc32(0, Z_NULL, 0);
    m_lasterror = m_decomp ? m_decomp->GetLastError() : wxSTREAM_READ_ERROR;
    return IsOk();
}


// Can be overriden to add support for additional decompression methods
//
wxInputStream *wxZipInputStream2::CreateDecompressor(wxInputStream& stream)
{
    switch (m_entry.GetMethod()) {
        case wxZIP_METHOD_STORE:
            if (m_entry.GetSize() == wxInvalidOffset) {
                wxLogError(_("stored file length not in Zip header"));
                break;
            }
            return new wxStoredInputStream(stream, m_entry.GetSize(), false);

        case wxZIP_METHOD_DEFLATE:
            return new wxZlibInputStream(stream, wxZLIB_NO_HEADER);

        default:
            wxLogError(_("unsupported Zip compression method"));
    }

    return NULL;
}


// Closes the current entry and positions the underlying stream at the start
// of the next entry
//
bool wxZipInputStream2::CloseEntry()
{
    if (AtHeader())
        return true;
    if (m_lasterror == wxSTREAM_READ_ERROR)
        return false;

    if (!m_parentSeekable) {
        if (!IsOpened() && (m_raw = true, !ImplOpen()))
            return false;

        const int BUFSIZE = 8192;
        wxCharBuffer buf(BUFSIZE);
        while (IsOk())
            Read(buf.data(), BUFSIZE);

        m_position += m_headerSize + m_entry.GetCompressedSize();
    }

    if (m_lasterror == wxSTREAM_EOF)
        m_lasterror = wxSTREAM_NO_ERROR;

    delete m_decomp;
    m_decomp = NULL;
    m_entry = wxZipEntry();
    m_headerSize = 0;
    m_raw = false;

    return IsOk();
}


size_t wxZipInputStream2::OnSysRead(void *buffer, size_t size)
{
    if (!IsOpened())
        if ((!AfterHeader() && !DoOpen()) || !ImplOpen())
            m_lasterror = wxSTREAM_READ_ERROR;
    if (!IsOk() || !size)
        return 0;

    m_decomp->Read(buffer, size);
    m_crcAccumulator =
            crc32(m_crcAccumulator, (Byte*)buffer, m_decomp->LastRead());
    m_lasterror = m_decomp->GetLastError();

    if (Eof()) {
        bool sums = (m_entry.GetFlags() & wxZIP_SUMS_FOLLOW) != 0;

        if (!m_raw) {
            if (sums)
                m_headerSize += m_entry.ReadDescriptor(*m_parent_i_stream);

            m_lasterror = wxSTREAM_READ_ERROR;

            if (m_parent_i_stream->IsOk()) {
                if (m_entry.GetSize() != TellI())
                    wxLogError(_("reading zip stream (entry %s): bad length"),
                               m_entry.GetName().c_str());
                else if (m_crcAccumulator != m_entry.GetCrc())
                    wxLogError(_("reading zip stream (entry %s): bad crc"),
                               m_entry.GetName().c_str());
                else
                    m_lasterror = wxSTREAM_EOF;
            }
        } else if (sums) {
            off_t descriptor = m_decomp->TellI() - m_entry.GetCompressedSize();
            wxASSERT(descriptor == SUMS_SIZE || descriptor == SUMS_SIZE + 4);
            m_headerSize += descriptor;
        }

        if (sums) {
            wxZipEntry *entry = GetRef(m_entry.GetKey());
            if (entry) {
                entry->SetCrc(m_entry.GetCrc());
                entry->SetCompressedSize(m_entry.GetCompressedSize());
                entry->SetSize(m_entry.GetSize());
                entry->Notify();
            }
        }
    }

    return m_decomp->LastRead();
}


/////////////////////////////////////////////////////////////////////////////
// Output stream

wxZipOutputStream2::wxZipOutputStream2(wxOutputStream& stream,
                                       int level      /*=-1*/,
                                       wxMBConv& conv /*=wxConvFile*/)
  : wxArchiveOutputStream(stream, conv),
    m_store(new wxStoredOutputStream(stream)),
    m_initialData(new char[OUTPUT_LATENCY]),
    m_initialSize(0),
    m_pending(NULL),
    m_raw(0),
    m_rawsums(new char[SUMS_SIZE]),
    m_headerOffset(0),
    m_headerSize(0),
    m_entrySize(0),
    m_comp(NULL),
    m_level(level),
    m_parentSeekable(SEEK_MAYBE),
    m_extra(NULL)
{
}

                                
wxZipOutputStream2::~wxZipOutputStream2()
{
    Close();
    WX_CLEAR_ARRAY(m_entries);
    delete m_store;
    delete m_pending;
    delete m_extra;
    delete [] m_initialData;
    delete [] m_rawsums;
}


bool wxZipOutputStream2::PutNextEntry(
    const wxString& name,
    const wxDateTime& dt /*=wxDateTime::Now()*/,
    off_t WXUNUSED(size) /*=wxInvalidOffset*/)
{
    return PutNextEntry(new wxZipEntry(name, dt));
}


bool wxZipOutputStream2::PutNextDirEntry(
    const wxString& name,
    const wxDateTime& dt /*=wxDateTime::Now()*/)
{
    wxZipEntry *entry = new wxZipEntry(name, dt);
    entry->SetIsDir();
    return PutNextEntry(entry);
}


bool wxZipOutputStream2::CopyEntry(wxZipEntry *entry,
                                   wxZipInputStream2& inputStream)
{
    _wxZipEntryPtr e(entry);

    return inputStream.DoOpen(e.get(), true) && DoCreate(e.release(), true) &&
           Write(inputStream).IsOk() && inputStream.Eof();
}


bool wxZipOutputStream2::PutNextEntry(wxArchiveEntry *entry)
{
    wxZipEntry *zipEntry = wxStaticCast(entry, wxZipEntry);
    if (!zipEntry)
        delete entry;
    return PutNextEntry(zipEntry);
}


bool wxZipOutputStream2::CopyEntry(wxArchiveEntry *entry,
                                   wxArchiveInputStream& stream)
{
    wxZipEntry *zipEntry = wxStaticCast(entry, wxZipEntry);

    if (!zipEntry || !stream.OpenEntry(*zipEntry)) {
        delete entry;
        return false;
    }

    return CopyEntry(zipEntry, wx_static_cast(wxZipInputStream2&, stream));
}


void wxZipOutputStream2::SetExtra(wxArchiveExtra *extra)
{
    SetExtra(wxStaticCast(extra, wxZipExtra));
    if (!m_extra)
        delete extra;
}


bool wxZipOutputStream2::DoCreate(wxZipEntry *entry, bool raw /*=false*/)
{
    CloseEntry();

    m_pending = entry;
    if (!m_pending)
        return false;

    {
        wxLogNull nolog;
        off_t here = m_parent_o_stream->TellO();
        // FIXME
        //if (here != wxInvalidOffset)
        if (here >= 0 && here <= LONG_MAX)
            m_headerOffset = here;
    }

    m_pending->SetOffset(m_headerOffset);

    m_crcAccumulator = crc32(0, Z_NULL, 0);

    if (raw)
        m_raw = m_pending->GetFlags() & wxZIP_SUMS_FOLLOW ?
                    RAW_SUMS_FOLLOW : RAW_NORMAL;

    // For raw writing the zip entry is written right away, otherwise
    // the zip entry is held pending to be written by CreatePendingEntry
    if (m_raw) {
        m_headerSize = m_pending->WriteLocal(*m_parent_o_stream, GetConv());
        m_lasterror = m_parent_o_stream->GetLastError();
        if (IsOk()) {
            m_comp = m_store;
            m_entries.push_back(m_pending);
        } else {
            delete m_pending;
        }
        m_pending = NULL;
        return IsOk();
    }

    m_lasterror = wxSTREAM_NO_ERROR;
    return true;
}


// Can be overriden to add support for additional compression methods
//
wxOutputStream *wxZipOutputStream2::CreateCompressor(
    wxOutputStream& stream,
    wxZipEntry& entry,
    const Buffer bufs[])
{
    if (entry.GetMethod() == wxZIP_METHOD_DEFAULT) {
        if (GetLevel() == 0 && GetIsParentSeekable()) {
            entry.SetMethod(wxZIP_METHOD_STORE);
        } else {
            int size = 0;
            for (int i = 0; bufs[i].m_data; ++i)
                size += bufs[i].m_size;
            entry.SetMethod(size <= 6 ?
                            wxZIP_METHOD_STORE : wxZIP_METHOD_DEFLATE);
        }
    }

    switch (entry.GetMethod()) {
        case wxZIP_METHOD_STORE:
            return m_store;

        case wxZIP_METHOD_DEFLATE:
        {
            int defbits = wxZIP_DEFLATE_NORMAL;
            switch (GetLevel()) {
                case 0: case 1:
                    defbits = wxZIP_DEFLATE_SUPERFAST;
                    break;
                case 2: case 3: case 4:
                    defbits = wxZIP_DEFLATE_FAST;
                    break;
                case 8: case 9:
                    defbits = wxZIP_DEFLATE_EXTRA;
                    break;
            }
            entry.SetFlags((entry.GetFlags() & ~wxZIP_DEFLATE_MASK) |
                            defbits | wxZIP_SUMS_FOLLOW);

            return new wxZlibOutputStream(stream, GetLevel(), wxZLIB_NO_HEADER);
        }

        default:
            wxLogError(_("unsupported Zip compression method"));
    }

    return NULL;
}


// This is called when OUPUT_LATENCY bytes has been written to the
// wxZipOutputStream2 to actually create the zip entry.
//
void wxZipOutputStream2::CreatePendingEntry(const void *buffer, size_t size)
{
    wxASSERT(IsOk() && m_pending && !m_comp);
    _wxZipEntryPtr spPending(m_pending);
    m_pending = NULL;

    Buffer bufs[] = {
        { m_initialData, m_initialSize },
        { (const char*)buffer, size },
        { NULL, 0 }
    };

    m_comp = CreateCompressor(*m_store, *spPending,
                              m_initialSize ? bufs : bufs + 1);
    m_headerSize = spPending->WriteLocal(*m_parent_o_stream, GetConv());

    if (GetLevel() == 0 && m_parent_o_stream->IsOk()
            && m_comp != m_store && m_parentSeekable != SEEK_NO)
    {
        bool logging = wxLog::IsEnabled();
        wxLogNull nolog;
        off_t here = m_parent_o_stream->TellO();

        if (m_parent_o_stream->SeekO(m_headerOffset + FLAGS_OFFSET)
                != wxInvalidOffset)
        {
            wxLog::EnableLogging(logging);
            m_parentSeekable = SEEK_YES;

            int flags = spPending->GetFlags() & ~wxZIP_SUMS_FOLLOW;
            spPending->SetFlags(flags);
            spPending->SetMethod(wxZIP_METHOD_STORE);

            wxDataOutputStream ds(*m_parent_o_stream);
            ds.Write16(flags);
            ds.Write16(wxZIP_METHOD_STORE);
            
            m_parent_o_stream->SeekO(here);
            delete m_comp;
            m_store->Close();
            m_comp = m_store;
            m_parent_o_stream->SeekO(here);
        } else {
            m_parentSeekable = SEEK_NO;
        }
    }
            
    m_lasterror = m_parent_o_stream->GetLastError();
    if (IsOk())
        m_entries.push_back(spPending.release());
    OnSysWrite(m_initialData, m_initialSize);
    m_initialSize = 0;
}


// This is called to write out the zip entry when Close has been called
// before OUTPUT_LATENCY bytes has been written to the wxZipOutputStream2.
//
void wxZipOutputStream2::CreatePendingEntry()
{
    wxASSERT(IsOk() && m_pending && !m_comp);
    _wxZipEntryPtr spPending(m_pending);
    m_pending = NULL;
    m_lasterror = wxSTREAM_WRITE_ERROR;

    // Initially compresses the data to memory, then fall back to 'store' if
    // the default compressor makes the data larger rather than smaller.
    wxMemoryOutputStream mem;
    Buffer bufs[] = { { m_initialData, m_initialSize }, { NULL, 0 } };
    wxOutputStream *comp = CreateCompressor(mem, *spPending, bufs);

    if (!comp)
        return;
    if (comp != m_store) {
        bool ok = comp->Write(m_initialData, m_initialSize).IsOk();
        delete comp;
        if (!ok)
            return;
    }

    spPending->SetFlags(spPending->GetFlags() & ~wxZIP_SUMS_FOLLOW);
    spPending->SetSize(m_initialSize);
    spPending->SetCrc(crc32(0, (Byte*)m_initialData, m_initialSize));

    if (mem.GetSize() > 0 && mem.GetSize() < m_initialSize) {
        spPending->SetCompressedSize(mem.GetSize());
        m_initialSize = mem.GetSize();
        mem.CopyTo(m_initialData, m_initialSize);
    } else {
        spPending->SetMethod(wxZIP_METHOD_STORE);
        spPending->SetCompressedSize(m_initialSize);
    }

    m_headerSize = spPending->WriteLocal(*m_parent_o_stream, GetConv());

    if (m_parent_o_stream->IsOk()) {
        m_entries.push_back(spPending.release());
        m_comp = m_store;
        m_raw = RAW_NORMAL;
        m_store->Write(m_initialData, m_initialSize);
    }

    m_initialSize = 0;
    m_lasterror = m_parent_o_stream->GetLastError();
}


// Write the 'central directory' and the 'end-central-directory' records.
//
bool wxZipOutputStream2::Close()
{
    CloseEntry();

    if (m_lasterror == wxSTREAM_WRITE_ERROR || m_entries.size() == 0)
        return false;

    if (!m_extra)
        m_extra = new wxZipExtra;

    m_extra->SetEntriesHere(m_entries.size());
    m_extra->SetTotalEntries(m_entries.size());
    m_extra->SetOffset(m_headerOffset);

    off_t size = 0;

    for (size_t i = 0; i < m_entries.size(); ++i)
        size += m_entries[i]->WriteCentral(*m_parent_o_stream, GetConv());
    WX_CLEAR_ARRAY(m_entries);

    m_extra->SetSize(size);
    m_extra->Write(*m_parent_o_stream, GetConv());

    delete m_extra;
    m_extra = NULL;

    m_lasterror = m_parent_o_stream->GetLastError();
    if (!IsOk())
        return false;
    m_lasterror = wxSTREAM_EOF;
    return true;
}


// Finish writing the current entry
//
bool wxZipOutputStream2::CloseEntry()
{
    if (IsOk() && m_pending)
        CreatePendingEntry();
    if (!m_comp)
        m_lasterror = wxSTREAM_WRITE_ERROR;
    if (!IsOk())
        return false;

    if (m_comp != m_store)
        delete m_comp;
    m_comp = NULL;

    off_t compressedSize = m_store->TellO();
    int extra = 0;

    wxZipEntry& entry = *m_entries[m_entries.size() - 1];

    if (m_raw == RAW_NORMAL) {
        if (compressedSize != entry.GetCompressedSize())
            m_lasterror = wxSTREAM_WRITE_ERROR;
    }
    else if (m_raw == RAW_SUMS_FOLLOW) {
        wxUint32 crc = CrackUint32(m_rawsums);
        off_t compSize = CrackUint32(m_rawsums + 4);
        off_t size = CrackUint32(m_rawsums + 8);

        entry.SetCrc(crc);
        entry.SetCompressedSize(compSize);
        entry.SetSize(size);

        if (compSize != compressedSize - SUMS_SIZE ||
            (entry.GetCompressedSize() != wxInvalidOffset &&
             (entry.GetCompressedSize() != compSize ||
              entry.GetCrc() != crc ||
              entry.GetSize() != size)))
            m_lasterror = wxSTREAM_WRITE_ERROR;
    }
    else if ((entry.GetFlags() & wxZIP_SUMS_FOLLOW)
            || m_crcAccumulator != entry.GetCrc()
            || m_entrySize != entry.GetSize()
            || compressedSize != entry.GetCompressedSize())
    {
        // If the local header didn't have the correct crc and size
        // written to it then try to seek back and fix it, or if that's
        // not possible write the sums and crc in the trailing 'data
        // descriptor'
        wxDataOutputStream ds(*m_parent_o_stream);
        off_t here = wxInvalidOffset;
        m_lasterror = wxSTREAM_WRITE_ERROR;

        if (m_parentSeekable != SEEK_NO) {
            bool logging = wxLog::IsEnabled();
            wxLogNull nolog;
            here = m_parent_o_stream->TellO();

            if (here != wxInvalidOffset &&
                m_parent_o_stream->SeekO(m_headerOffset + FLAGS_OFFSET) != wxInvalidOffset)
            {
                m_parentSeekable = SEEK_YES;
                wxLog::EnableLogging(logging);
                entry.SetFlags(entry.GetFlags() & ~wxZIP_SUMS_FOLLOW);
                ds.Write16(entry.GetFlags());
                m_parent_o_stream->SeekO(m_headerOffset + SUMS_OFFSET);
                m_lasterror = m_parent_o_stream->GetLastError();
            }
        }

        if (m_parentSeekable != SEEK_YES) {
            m_parentSeekable = SEEK_NO;
            here = wxInvalidOffset;
            if (entry.GetFlags() & wxZIP_SUMS_FOLLOW) {
                m_lasterror = wxSTREAM_NO_ERROR;
                extra = SUMS_SIZE;
            }
        }

        if (IsOk()) {
            ds.Write32(m_crcAccumulator);
            ds.Write32(compressedSize);
            ds.Write32(m_entrySize);
        }

        if (here != wxInvalidOffset)
            m_parent_o_stream->SeekO(here);

        entry.SetCrc(m_crcAccumulator);
        entry.SetCompressedSize(compressedSize);
        entry.SetSize(m_entrySize);
    }

    m_headerOffset += m_headerSize + compressedSize + extra;
    m_headerSize = m_entrySize = 0;
    m_store->Close();
    m_raw = 0;

    if (IsOk())
        m_lasterror = m_parent_o_stream->GetLastError();
    else
        wxLogError(_("bad crc or length written for entry '%s'"),
                   entry.GetName().c_str());
    return IsOk();
}


void wxZipOutputStream2::Sync()
{
    if (IsOk() && m_pending)
        CreatePendingEntry(NULL, 0);
    if (!m_comp)
        m_lasterror = wxSTREAM_WRITE_ERROR;
    if (IsOk()) {
        m_comp->Sync();
        m_lasterror = m_comp->GetLastError();
    }
}


size_t wxZipOutputStream2::OnSysWrite(const void *buffer, size_t size)
{
    if (IsOk() && m_pending) {
        if (m_initialSize + size < OUTPUT_LATENCY) {
            memcpy(m_initialData + m_initialSize, buffer, size);
            m_initialSize += size;
            return size;
        } else {
            CreatePendingEntry(buffer, size);
        }
    }

    if (!m_comp)
        m_lasterror = wxSTREAM_WRITE_ERROR;
    if (!IsOk() || !size)
        return 0;

    if (m_raw == RAW_SUMS_FOLLOW) {
        if (size < SUMS_SIZE) {
            memmove(m_rawsums, m_rawsums + size, SUMS_SIZE - size);
            memcpy(m_rawsums + size, buffer, size);
        } else {
            memcpy(m_rawsums, (const char*)buffer + size - SUMS_SIZE, SUMS_SIZE);
        }
    }

    if (m_comp->Write(buffer, size).LastWrite() != size)
        m_lasterror = wxSTREAM_WRITE_ERROR;
    m_crcAccumulator = crc32(m_crcAccumulator, (Byte*)buffer, size);
    m_entrySize += m_comp->LastWrite();

    return m_comp->LastWrite();
}


#endif // wxUSE_ZLIB && wxUSE_STREAMS && wxUSE_ZIPSTREAM
