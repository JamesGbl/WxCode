/////////////////////////////////////////////////////////////////////////////
// Name:        zipstrm.cpp
// Purpose:     Streams for Zip files
// Author:      Mike Wetherell
// RCS-ID:      $Id: zipstrm2.cpp,v 1.5 2004-09-09 15:53:52 chiclero Exp $
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
    SUMS_MAGIC    = 0x08074b50      // data descriptor (info-zip)
};

// unix file attributes. zip stores them in the high 16 bits of the
// 'external attributes' field, hence the extra zeros.
enum {
    wxZIP_S_IFMT  = 0xF0000000,
    wxZIP_S_IFDIR = 0x40000000,
    wxZIP_S_IFREG = 0x80000000
};

// minimum sizes for the various records
enum {
    CENTRAL_SIZE  = 46,
    LOCAL_SIZE    = 30,
    END_SIZE      = 22,
    SUMS_SIZE     = 12
};

// The number of bytes that must be written to an wxZipOutputStream2 before
// a zip entry is created. The purpose of this latency is so that
// OpenCompressor() can see a little data before deciding which compressor
// it should use.
enum {
    OUTPUT_LATENCY = 4096
};

// Some offsets into the local header
enum {
    SUMS_OFFSET  = 14
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
#if wxUSE_UNICODE
    wxCharBuffer buf(len);
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
static inline wxUint32 CrackUint32(const char *m)
{
    const unsigned char *n = (const unsigned char*)m;
    return (n[3] << 24) | (n[2] << 16) | (n[1] << 8) | n[0];
}

// Temporarily lower the logging level in debug mode to avoid a warning
// from SeekI about seeking on a stream with data written back to it.
//
static off_t QuietSeek(wxInputStream& stream, off_t pos)
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
    wxStoredInputStream(wxInputStream& stream);

    void Open(off_t len) { Close(); m_len = len; }
    void Close() { m_pos = 0; m_lasterror = wxSTREAM_NO_ERROR; }

    virtual char Peek() { return wxInputStream::Peek(); }
    virtual size_t GetSize() const { return m_len; }

protected:
    virtual size_t OnSysRead(void *buffer, size_t size);
    virtual off_t OnSysTell() const { return m_pos; }

private:
    off_t m_pos;
    off_t m_len;

    DECLARE_NO_COPY_CLASS(wxStoredInputStream)
};

wxStoredInputStream::wxStoredInputStream(wxInputStream& stream)
  : wxFilterInputStream(stream),
    m_pos(0),
    m_len(0)
{
}

size_t wxStoredInputStream::OnSysRead(void *buffer, size_t size)
{
    size_t count = wxMin(size, m_len - m_pos + (size_t)0);
    count = m_parent_i_stream->Read(buffer, count).LastRead();
    m_pos += count;

    if (m_pos == m_len)
        m_lasterror = wxSTREAM_EOF;
    else if (!*m_parent_i_stream)
        m_lasterror = wxSTREAM_READ_ERROR;

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
// wxRawInputStream
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
// Usage is like this:
//  m_rawin = new wxRawInputStream(*m_parent_i_stream);
//  m_decomp = m_rawin->Open(OpenDecompressor(m_rawin->GetTee()));
//
// The wxRawInputStream owns a wxTeeInputStream object, the role of which
// is something like the unix 'tee' command; it is a transparent filter, but
// allows the data read to be read a second time via an extra method 'GetData'.
//
// The wxRawInputStream then draws data through the tee using a decompressor
// then instead of returning the decompressed data, retuns the raw data
// from wxTeeInputStream::GetData().

class wxTeeInputStream : public wxFilterInputStream
{
public:
    wxTeeInputStream(wxInputStream& stream);

    size_t GetCount() const { return m_end - m_start; }
    size_t GetData(char *buffer, size_t size);

    void Open();
    bool Final();

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

void wxTeeInputStream::Open()
{
    m_pos = m_start = m_end = 0;
    m_lasterror = wxSTREAM_NO_ERROR;
}

bool wxTeeInputStream::Final()
{
    bool final = m_end == m_buf.GetDataLen();
    m_end = m_buf.GetDataLen();
    return final;
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
    wxRawInputStream(wxInputStream& stream);
    virtual ~wxRawInputStream() { delete m_tee; }

    wxInputStream* Open(wxInputStream *decomp);
    wxInputStream& GetTee() const { return *m_tee; }

protected:
    virtual size_t OnSysRead(void *buffer, size_t size);
    virtual off_t OnSysTell() const { return m_pos; }

private:
    off_t m_pos;
    wxTeeInputStream *m_tee;

    enum { BUFSIZE = 8192 };
    wxCharBuffer m_dummy;

    DECLARE_NO_COPY_CLASS(wxRawInputStream)
};

wxRawInputStream::wxRawInputStream(wxInputStream& stream)
  : wxFilterInputStream(stream),
    m_pos(0),
    m_tee(new wxTeeInputStream(stream)),
    m_dummy(BUFSIZE)
{
}

wxInputStream *wxRawInputStream::Open(wxInputStream *decomp)
{
    if (decomp) {
        m_parent_i_stream = decomp;
        m_pos = 0;
        m_lasterror = wxSTREAM_NO_ERROR;
        m_tee->Open();
        return this;
    } else {
        return NULL;
    }
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

        if (n == 0 && m_tee->Final())
            m_lasterror = m_parent_i_stream->GetLastError();
    }

    m_pos += count;
    return count;
}


/////////////////////////////////////////////////////////////////////////////
// Zlib streams than can be reused without recreating. Perhaps these
// features could be put into the wxZlib streams themselves.

class wxZlibOutputStream2 : public wxZlibOutputStream
{
public:
    wxZlibOutputStream2(wxOutputStream& stream, int level) :
        wxZlibOutputStream(stream, level, wxZLIB_NO_HEADER) { }

    bool Open(wxOutputStream& stream);
    bool Close() { DoFlush(true); m_pos = wxInvalidOffset; return IsOk(); }
};

bool wxZlibOutputStream2::Open(wxOutputStream& stream)
{
    wxCHECK(m_pos == wxInvalidOffset, false);

    m_deflate->next_out = m_z_buffer;
    m_deflate->avail_out = m_z_size;
    m_pos = 0;
    m_lasterror = wxSTREAM_NO_ERROR;
    m_parent_o_stream = &stream;

    if (deflateReset(m_deflate) != Z_OK) {
        wxLogError(_("can't re-initialize zlib deflate stream"));
        m_lasterror = wxSTREAM_WRITE_ERROR;
        return false;
    }

    return true;
}

class wxZlibInputStream2 : public wxZlibInputStream
{
public:
    wxZlibInputStream2(wxInputStream& stream) :
        wxZlibInputStream(stream, wxZLIB_NO_HEADER) { }

    bool Open(wxInputStream& stream);
};

bool wxZlibInputStream2::Open(wxInputStream& stream)
{
    m_pos = 0;
    m_lasterror = wxSTREAM_NO_ERROR;
    m_parent_i_stream = &stream;

    if (inflateReset(m_inflate) != Z_OK) {
        wxLogError(_("can't re-initialize zlib inflate stream"));
        m_lasterror = wxSTREAM_READ_ERROR;
        return false;
    }

    return true;
}


/////////////////////////////////////////////////////////////////////////////
// ZipEntry

wxZipEntry::wxZipEntry(
    const wxString& name /*=wxEmptyString*/,
    const wxDateTime& dt /*=wxDateTime::Now()*/,
    off_t size           /*=wxInvalidOffset*/)
  : 
    m_SystemMadeBy(wxZIP_SYSTEM_MSDOS),
    m_VersionMadeBy(wxMAJOR_VERSION * 10 + wxMINOR_VERSION),
    m_VersionNeeded(VERSION_NEEDED_TO_EXTRACT),
    m_Flags(0),
    m_Method(wxZIP_METHOD_DEFAULT),
    m_DateTime(dt),
    m_Crc(0),
    m_CompressedSize(wxInvalidOffset),
    m_Size(size),
    m_Key(wxInvalidOffset),
    m_Offset(wxInvalidOffset),
    m_DiskStart(0),
    m_InternalAttributes(0),
    m_ExternalAttributes(0),
    m_Extra(0),
    m_LocalExtra(0),
    m_IsLocalExtraSet(false),
    m_zipnotifier(NULL)
{
    if (!name.empty())
        SetName(name);
}

wxZipEntry::~wxZipEntry()
{
    if (m_backref)
        m_backref->ReleaseRef(this);
}

wxString wxZipEntry::GetName(wxPathFormat format /*=wxPATH_NATIVE*/) const
{
    bool isDir = IsDir() && !m_Name.empty();

    switch (wxFileName::GetFormat(format)) {
        case wxPATH_DOS:
        {
            wxString name(isDir ? m_Name + _T("\\") : m_Name);
            for (size_t i = name.length() - 1; i > 0; --i)
                if (name[i] == _T('/'))
                    name[i] = _T('\\');
            return name;
        }

        case wxPATH_UNIX:
            return isDir ? m_Name + _T("/") : m_Name;

        default:
            ;
    }

    if (isDir)
        return wxFileName(m_Name, _T(""), wxPATH_UNIX).GetFullPath(format);
    else
        return wxFileName(m_Name, wxPATH_UNIX).GetFullPath(format);
}

// Static - Internally tars and zips use forward slashes for the path
// separator, absolute paths aren't allowed, and directory names have a
// trailing slash.  This function converts a path into this internal format,
// but without a trailing slash for a directory.
//
wxString wxZipEntry::GetInternalName(const wxString& name,
                                     wxPathFormat format /*=wxPATH_NATIVE*/,
                                     bool *pIsDir        /*=NULL*/)
{
    wxString internal;

    if (wxFileName::GetFormat(format) != wxPATH_UNIX)
        internal = wxFileName(name, format).GetFullPath(wxPATH_UNIX);
    else
        internal = name;

    bool isDir = !internal.empty() && internal.Last() == '/';
    if (pIsDir)
        *pIsDir = isDir;
    if (isDir)
        internal.erase(internal.length() - 1);

    while (!internal.empty() && *internal.begin() == '/')
        internal.erase(0, 1);
    while (!internal.empty() && internal.compare(0, 2, _T("./")) == 0)
        internal.erase(0, 2);
    if (internal == _T(".") || internal == _T(".."))
        internal = wxEmptyString;

    return internal;
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
    if (isDir)
        m_ExternalAttributes |= wxZIP_A_SUBDIR;
    else
        m_ExternalAttributes &= ~wxZIP_A_SUBDIR;

    if (IsMadeByUnix()) {
        m_ExternalAttributes &= ~wxZIP_S_IFMT;
        if (isDir)
            m_ExternalAttributes |= wxZIP_S_IFDIR;
        else
            m_ExternalAttributes |= wxZIP_S_IFREG;
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

size_t wxZipEntry::ReadLocal(wxInputStream& stream, wxMBConv& conv)
{
    wxUint16 nameLen, extraLen;
    wxUint32 compressedSize, size, crc;

    wxDataInputStream ds(stream);

    ds >> m_VersionNeeded >> m_Flags >> m_Method;
    SetDateTime(wxDateTime().SetFromDOS(ds.Read32()));
    ds >> crc >> compressedSize >> size >> nameLen >> extraLen;

    bool sumsValid = (m_Flags & wxZIP_SUMS_FOLLOW) == 0;

    if (sumsValid || crc)
        m_Crc = crc;
    if ((sumsValid || compressedSize) || m_Method == wxZIP_METHOD_STORE)
        m_CompressedSize = compressedSize;
    if ((sumsValid || size) || m_Method == wxZIP_METHOD_STORE)
        m_Size = size;

    SetName(ReadString(stream, nameLen, conv), wxPATH_UNIX);

    // note: wxMemoryBuffer shares the buffer completely, whereas what we
    // need here is 'copy on write'. So remember to always assign a new one
    // when m_Extra or m_LocalExtra change.
    if (extraLen || GetLocalExtraLen()) {
        m_LocalExtra = wxMemoryBuffer(extraLen);
        if (extraLen) {
            stream.Read(m_LocalExtra.GetWriteBuf(extraLen), extraLen);
            m_LocalExtra.UngetWriteBuf(stream.LastRead());
        }
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

    ds << m_VersionNeeded << m_Flags << m_Method;
    ds.Write32(GetDateTime().GetAsDOS());
    
    ds.Write32(m_Crc);
    ds.Write32(m_CompressedSize != wxInvalidOffset ? m_CompressedSize : 0);
    ds.Write32(m_Size != wxInvalidOffset ? m_Size : 0);

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

    if (extraLen || GetExtraLen()) {
        m_Extra = wxMemoryBuffer(extraLen);
        if (extraLen) {
            stream.Read(m_Extra.GetWriteBuf(extraLen), extraLen);
            m_Extra.UngetWriteBuf(stream.LastRead());
        }
    }

    if (commentLen)
        m_Comment = ReadString(stream, commentLen, conv);
    else
        m_Comment.clear();

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

// Info-zip prefixes this record with a signature, but pkzip doesn't. So if
// the 1st value is the signature then it is probably an info-zip record,
// though there is a small chance that it is in fact a pkzip record which
// happens to have the signature as it's CRC.
//
size_t wxZipEntry::ReadDescriptor(wxInputStream& stream)
{
    wxDataInputStream ds(stream);
    
    m_Crc = ds.Read32();
    m_CompressedSize = ds.Read32();
    m_Size = ds.Read32();

    // if 1st value is the signature then this is probably an info-zip record
    if (m_Crc == SUMS_MAGIC)
    {
        char buf[8];
        stream.Read(buf, sizeof(buf));
        wxUint32 u1 = CrackUint32(buf);
        wxUint32 u2 = CrackUint32(buf + 4);
        
        // look for the signature of the following record to decide which
        if ((u1 == LOCAL_MAGIC || u1 == CENTRAL_MAGIC) &&
            (u2 != LOCAL_MAGIC && u2 != CENTRAL_MAGIC))
        {
            // it's a pkzip style record after all!
            stream.Ungetch(buf, sizeof(buf));
        }
        else
        {
            // it's an info-zip record as expected
            stream.Ungetch(buf + 4, sizeof(buf) - 4);
            m_Crc = m_CompressedSize;
            m_CompressedSize = m_Size;
            m_Size = u1;
            return SUMS_SIZE + 4;
        }
    }

    return SUMS_SIZE;
}

size_t wxZipEntry::WriteDescriptor(wxOutputStream& stream, wxUint32 crc,
                                   off_t compressedSize, off_t size)
{
    m_Crc = crc;
    m_CompressedSize = compressedSize;
    m_Size = size;

    wxDataOutputStream ds(stream);

    ds.Write32(crc);
    ds.Write32(compressedSize);
    ds.Write32(size);

    return SUMS_SIZE;
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
    wxUint16 commentLen;

    ds >> m_DiskNumber >> m_StartDisk >> m_EntriesHere
       >> m_TotalEntries >> m_Size >> m_Offset >> commentLen;

    if (commentLen)
        m_Comment = ReadString(stream, commentLen, conv);

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
wxDECLARE_SCOPED_PTR(wxZipExtra, _wxZipExtraPtr)
wxDEFINE_SCOPED_PTR (wxZipExtra, _wxZipExtraPtr)

wxZipInputStream2::wxZipInputStream2(wxInputStream& stream,
                                     wxMBConv& conv /*=wxConvFile*/)
  : wxArchiveInputStream(stream, conv),
    m_store(new wxStoredInputStream(stream)),
    m_inflate(NULL),
    m_rawin(NULL),
    m_extra(new wxZipExtra),
    m_raw(false),
    m_headerSize(0),
    m_decomp(NULL),
    m_parentSeekable(false),
    m_offsetAdjustment(0),
    m_position(wxInvalidOffset),
    m_signature(0)
{
    m_extraWeakref = m_extra;
    m_extraWeakref->m_backref = this;
}

wxZipInputStream2::~wxZipInputStream2()
{
    CloseDecompressor(m_decomp);

    delete m_store;
    delete m_inflate;
    delete m_rawin;
    delete m_extra;

    _wxOffsetZipEntryMap::iterator it;
    for (it = m_weakrefs.begin(); it != m_weakrefs.end(); ++it) 
        it->second->m_backref = NULL;

    if (m_extraWeakref)
        m_extraWeakref->m_backref = NULL;
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
    entry->m_backref = NULL;
}

void wxZipInputStream2::ReleaseRef(wxZipExtra *extra)
{
    wxCHECK_RET(extra && extra == m_extraWeakref,
                _T("weak reference not found"));
    m_extraWeakref = NULL;
    extra->m_backref = NULL;
}

wxZipEntry *wxZipInputStream2::GetRef(off_t key)
{
    _wxOffsetZipEntryMap::iterator it = m_weakrefs.find(key);
    return it != m_weakrefs.end() ?  it->second : NULL;
}

wxZipExtra *wxZipInputStream2::GetExtra()
{
    if (m_position == wxInvalidOffset)
        if (!LoadEndRecord())
            return NULL;
    wxZipExtra *extra = m_extra;
    m_extra = NULL;
    return extra;
}

bool wxZipInputStream2::LoadEndRecord()
{
    wxCHECK(m_position == wxInvalidOffset, false);
    m_position = 0;

    // First find the end-of-central-directory record.
    if (!FindEndRecord()) {
        // failed, so either this is a non-seekable stream (ok), or not a zip
        if (m_parentSeekable) {
            m_lasterror = wxSTREAM_READ_ERROR;
            wxLogError(_("invalid zip file"));
            return false;
        }
        else {
            wxLogNull nolog;
            off_t pos = m_parent_i_stream->TellI();
            // FIXME
            //if (pos != wxInvalidOffset)
            if (pos >= 0 && pos <= LONG_MAX)
                m_offsetAdjustment = m_position = pos;
            return true;
        }
    }

    _wxZipExtraPtr spExtra;
    wxZipExtra *extra = m_extraWeakref;
    if (!extra)
        spExtra.reset(extra = new wxZipExtra);

    // Read in the end record
    off_t endPos = m_parent_i_stream->TellI() - 4;
    if (!extra->Read(*m_parent_i_stream, GetConv())) {
        if (!*m_parent_i_stream) {
            m_lasterror = wxSTREAM_READ_ERROR;
            return false;
        }
        // TODO: try this out
        wxLogWarning(_("assuming this is a multi-part zip concatenated"));
    }

    // Now find the central-directory. we have the file offset of
    // the CD, so look there first. 
    if (m_parent_i_stream->SeekI(extra->GetOffset()) != wxInvalidOffset &&
            ReadSignature() == CENTRAL_MAGIC) {
        m_signature = CENTRAL_MAGIC;
        m_position = extra->GetOffset();
        m_offsetAdjustment = 0;
        return true;
    }
 
    // If it's not there, then it could be that the zip has been appended
    // to a self extractor, so take the CD size (also in extra), subtract
    // it from the file offset of the end-central-directory and look there.
    if (m_parent_i_stream->SeekI(endPos - extra->GetSize())
            != wxInvalidOffset && ReadSignature() == CENTRAL_MAGIC) {
        m_signature = CENTRAL_MAGIC;
        m_position = endPos - extra->GetSize();
        m_offsetAdjustment = m_position - extra->GetOffset();
        return true;
    }

    wxLogError(_("can't find central directory in zip"));
    m_lasterror = wxSTREAM_READ_ERROR;
    return false;
}

// Find the end-of-central-directory record.
// If found the stream will be positioned just past the 4 signature bytes.
//
bool wxZipInputStream2::FindEndRecord()
{
    // usually it's 22 bytes in size and the last thing in the file
    { 
        wxLogNull nolog;
        if (m_parent_i_stream->SeekI(-END_SIZE, wxFromEnd) == wxInvalidOffset)
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

wxZipEntry *wxZipInputStream2::GetNextEntry()
{
    if (m_position == wxInvalidOffset)
        if (!LoadEndRecord())
            return NULL;

    m_lasterror = m_parentSeekable ? ReadCentral() : ReadLocal();

    if (!IsOk())
        return NULL;

    _wxZipEntryPtr entry(new wxZipEntry(m_entry));
    AddRef(entry.get());

    return entry.release();
}

wxStreamError wxZipInputStream2::ReadCentral()
{
    if (!AtHeader())
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
        if (m_weakrefs.empty() && m_extraWeakref == NULL)
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
            ReleaseRef(entry);
        }

        signature = ReadSignature();
    }

    if (signature == END_MAGIC) {
        if (m_extraWeakref) {
            m_extraWeakref->Read(*m_parent_i_stream, GetConv());
            m_extraWeakref->Notify();
            ReleaseRef(m_extraWeakref);
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
    if (m_position == wxInvalidOffset)
        if (!LoadEndRecord())
            return false;
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
            wxZipEntry *ref = GetRef(m_entry.GetKey());
            if (ref && !ref->m_IsLocalExtraSet) {
                ref->SetLocalExtra(m_entry.m_LocalExtra);
                ref->Notify();
                ReleaseRef(ref);
            }
            if (entry && entry != ref && !entry->m_IsLocalExtraSet) {
                entry->SetLocalExtra(m_entry.m_LocalExtra);
                entry->Notify();
            }
        }
    }

    m_lasterror = m_parent_i_stream->GetLastError();
    return IsOk();
}

bool wxZipInputStream2::OpenDecompressor(bool raw /*=false*/)
{
    wxASSERT(AfterHeader());

    off_t compressedSize = m_entry.GetCompressedSize();

    if (raw)
        m_raw = true;

    if (m_raw) {
        if (compressedSize != wxInvalidOffset) {
            m_store->Open(compressedSize);
            m_decomp = m_store;
        } else {
            if (!m_rawin)
                m_rawin = new wxRawInputStream(*m_parent_i_stream);
            m_decomp = m_rawin->Open(OpenDecompressor(m_rawin->GetTee()));
        }
    } else {
        if (compressedSize != wxInvalidOffset) {
            m_store->Open(compressedSize);
            m_decomp = OpenDecompressor(*m_store);
        } else {
            m_decomp = OpenDecompressor(*m_parent_i_stream);
        }
    }

    m_crcAccumulator = crc32(0, Z_NULL, 0);
    m_lasterror = m_decomp ? m_decomp->GetLastError() : wxSTREAM_READ_ERROR;
    return IsOk();
}

// Can be overriden to add support for additional decompression methods
//
wxInputStream *wxZipInputStream2::OpenDecompressor(wxInputStream& stream)
{
    switch (m_entry.GetMethod()) {
        case wxZIP_METHOD_STORE:
            if (m_entry.GetSize() == wxInvalidOffset) {
                wxLogError(_("stored file length not in Zip header"));
                break;
            }
            m_store->Open(m_entry.GetSize());
            return m_store;

        case wxZIP_METHOD_DEFLATE:
            if (!m_inflate)
                m_inflate = new wxZlibInputStream2(stream);
            else
                m_inflate->Open(stream);
            return m_inflate;

        default:
            wxLogError(_("unsupported Zip compression method"));
    }

    return NULL;
}

bool wxZipInputStream2::CloseDecompressor(wxInputStream *decomp)
{
    if (decomp && decomp == m_rawin)
        return CloseDecompressor(m_rawin->GetFilterInputStream());
    if (decomp != m_store && decomp != m_inflate)
        delete decomp;
    return true;
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
        if (!IsOpened() && !OpenDecompressor(true))
            return false;

        const int BUFSIZE = 8192;
        wxCharBuffer buf(BUFSIZE);
        while (IsOk())
            Read(buf.data(), BUFSIZE);

        m_position += m_headerSize + m_entry.GetCompressedSize();
    }

    if (m_lasterror == wxSTREAM_EOF)
        m_lasterror = wxSTREAM_NO_ERROR;

    CloseDecompressor(m_decomp);
    m_decomp = NULL;
    m_entry = wxZipEntry();
    m_headerSize = 0;
    m_raw = false;

    return IsOk();
}

size_t wxZipInputStream2::OnSysRead(void *buffer, size_t size)
{
    if (!IsOpened())
        if ((AtHeader() && !DoOpen()) || !OpenDecompressor())
            m_lasterror = wxSTREAM_READ_ERROR;
    if (!IsOk() || !size)
        return 0;

    size_t count = m_decomp->Read(buffer, size).LastRead();
    if (!m_raw)
        m_crcAccumulator = crc32(m_crcAccumulator, (Byte*)buffer, count);
    m_lasterror = m_decomp->GetLastError();

    if (Eof()) {
        if ((m_entry.GetFlags() & wxZIP_SUMS_FOLLOW) != 0) {
            m_headerSize += m_entry.ReadDescriptor(*m_parent_i_stream);
            wxZipEntry *entry = GetRef(m_entry.GetKey());

            if (entry) {
                entry->SetCrc(m_entry.GetCrc());
                entry->SetCompressedSize(m_entry.GetCompressedSize());
                entry->SetSize(m_entry.GetSize());
                entry->Notify();
            }
        }

        if (!m_raw) {
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
        }
    }

    return count;
}


/////////////////////////////////////////////////////////////////////////////
// Output stream

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(_wxZipEntryList);

wxZipOutputStream2::wxZipOutputStream2(wxOutputStream& stream,
                                       int level      /*=-1*/,
                                       wxMBConv& conv /*=wxConvFile*/)
  : wxArchiveOutputStream(stream, conv),
    m_store(new wxStoredOutputStream(stream)),
    m_deflate(NULL),
    m_initialData(new char[OUTPUT_LATENCY]),
    m_initialSize(0),
    m_pending(NULL),
    m_raw(false),
    m_headerOffset(0),
    m_headerSize(0),
    m_entrySize(0),
    m_comp(NULL),
    m_level(level),
    m_offsetAdjustment(wxInvalidOffset),
    m_extra(NULL)
{
}

                                
wxZipOutputStream2::~wxZipOutputStream2()
{
    Close();
    WX_CLEAR_LIST(_wxZipEntryList, m_entries);
    delete m_store;
    delete m_deflate;
    delete m_pending;
    delete m_extra;
    delete [] m_initialData;
}

bool wxZipOutputStream2::PutNextEntry(
    const wxString& name,
    const wxDateTime& dt /*=wxDateTime::Now()*/,
    off_t size           /*=wxInvalidOffset*/)
{
    return PutNextEntry(new wxZipEntry(name, dt, size));
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

    return
        inputStream.DoOpen(e.get(), true) &&
        DoCreate(e.release(), true) &&
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

void wxZipOutputStream2::SetLevel(int level)
{
    if (level != m_level) {
        if (m_comp != m_deflate)
            delete m_deflate;
        m_deflate = NULL;
        m_level = level;
    }
}

bool wxZipOutputStream2::DoCreate(wxZipEntry *entry, bool raw /*=false*/)
{
    CloseEntry();

    m_pending = entry;
    if (!m_pending)
        return false;

    // write the signature bytes right away
    wxDataOutputStream ds(*m_parent_o_stream);
    ds << LOCAL_MAGIC;

    // and if this is the first entry test for seekability
    if (m_headerOffset == 0) {
        bool logging = wxLog::IsEnabled();
        wxLogNull nolog;
        off_t here = m_parent_o_stream->TellO();

        if (here != wxInvalidOffset && here >= 4) {
            if (m_parent_o_stream->SeekO(here - 4) == here - 4) {
                m_offsetAdjustment = here - 4;
                wxLog::EnableLogging(logging);
                m_parent_o_stream->SeekO(here);
            }
        }
    }

    m_pending->SetOffset(m_headerOffset);

    m_crcAccumulator = crc32(0, Z_NULL, 0);

    if (raw)
        m_raw = true;

    m_lasterror = wxSTREAM_NO_ERROR;
    return true;
}

// Can be overriden to add support for additional compression methods
//
wxOutputStream *wxZipOutputStream2::OpenCompressor(
    wxOutputStream& stream,
    wxZipEntry& entry,
    const Buffer bufs[])
{
    if (entry.GetMethod() == wxZIP_METHOD_DEFAULT) {
        if (GetLevel() == 0
                && (IsParentSeekable()
                    || entry.GetCompressedSize() != wxInvalidOffset
                    || entry.GetSize() != wxInvalidOffset)) {
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
            if (entry.GetCompressedSize() == wxInvalidOffset)
                entry.SetCompressedSize(entry.GetSize());
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

            if (!m_deflate)
                m_deflate = new wxZlibOutputStream2(stream, GetLevel());
            else
                m_deflate->Open(stream);

            return m_deflate;
        }

        default:
            wxLogError(_("unsupported Zip compression method"));
    }

    return NULL;
}

bool wxZipOutputStream2::CloseCompressor(wxOutputStream *comp)
{
    if (comp == m_deflate)
        m_deflate->Close();
    else if (comp != m_store)
        delete comp;
    return true;
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

    if (m_raw)
        m_comp = m_store;
    else
        m_comp = OpenCompressor(*m_store, *spPending,
                                m_initialSize ? bufs : bufs + 1);

    if (IsParentSeekable()
        || (spPending->m_Crc
            && spPending->m_CompressedSize != wxInvalidOffset
            && spPending->m_Size != wxInvalidOffset))
        spPending->m_Flags &= ~wxZIP_SUMS_FOLLOW;
    else
        if (spPending->m_CompressedSize != wxInvalidOffset)
            spPending->m_Flags |= wxZIP_SUMS_FOLLOW;

    m_headerSize = spPending->WriteLocal(*m_parent_o_stream, GetConv());
    m_lasterror = m_parent_o_stream->GetLastError();

    if (IsOk()) {
        m_entries.push_back(spPending.release());
        OnSysWrite(m_initialData, m_initialSize);
    }

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

    if (!m_raw) {
        // Initially compresses the data to memory, then fall back to 'store'
        // if the compressor makes the data larger rather than smaller.
        wxMemoryOutputStream mem;
        Buffer bufs[] = { { m_initialData, m_initialSize }, { NULL, 0 } };
        wxOutputStream *comp = OpenCompressor(mem, *spPending, bufs);

        if (!comp)
            return;
        if (comp != m_store) {
            bool ok = comp->Write(m_initialData, m_initialSize).IsOk();
            CloseCompressor(comp);
            if (!ok)
                return;
        }

        m_entrySize = m_initialSize;
        m_crcAccumulator = crc32(0, (Byte*)m_initialData, m_initialSize);

        if (mem.GetSize() > 0 && mem.GetSize() < m_initialSize) {
            m_initialSize = mem.GetSize();
            mem.CopyTo(m_initialData, m_initialSize);
        } else {
            spPending->SetMethod(wxZIP_METHOD_STORE);
        }

        spPending->SetSize(m_entrySize);
        spPending->SetCrc(m_crcAccumulator);
        spPending->SetCompressedSize(m_initialSize);
    }

    spPending->m_Flags &= ~wxZIP_SUMS_FOLLOW;
    m_headerSize = spPending->WriteLocal(*m_parent_o_stream, GetConv());

    if (m_parent_o_stream->IsOk()) {
        m_entries.push_back(spPending.release());
        m_comp = m_store;
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

    _wxZipEntryList::iterator it;
    off_t size = 0;

    for (it = m_entries.begin(); it != m_entries.end(); ++it) {
        size += (*it)->WriteCentral(*m_parent_o_stream, GetConv());
        delete *it;
    }
    m_entries.clear();

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
    if (!IsOk())
        return false;
    if (!m_comp)
        return true;

    CloseCompressor(m_comp);
    m_comp = NULL;

    off_t compressedSize = m_store->TellO();

    wxZipEntry& entry = *m_entries.back();

    // When writing raw the crc and size can't be checked
    if (m_raw) {
        m_crcAccumulator = entry.GetCrc();
        m_entrySize = entry.GetSize();
    }

    // Write the sums in the trailing 'data descriptor' if necessary
    if (entry.m_Flags & wxZIP_SUMS_FOLLOW) {
        wxASSERT(!IsParentSeekable());
        m_headerOffset +=
            entry.WriteDescriptor(*m_parent_o_stream, m_crcAccumulator,
                                  compressedSize, m_entrySize);
        m_lasterror = m_parent_o_stream->GetLastError();
    }

    // If the local header didn't have the correct crc and size written to
    // it then seek back and fix it
    else if (m_crcAccumulator != entry.GetCrc()
            || m_entrySize != entry.GetSize()
            || compressedSize != entry.GetCompressedSize())
    {
        if (IsParentSeekable()) {
            off_t here = m_parent_o_stream->TellO();
            off_t headerOffset = m_headerOffset + m_offsetAdjustment;
            m_parent_o_stream->SeekO(headerOffset + SUMS_OFFSET);
            entry.WriteDescriptor(*m_parent_o_stream, m_crcAccumulator,
                                  compressedSize, m_entrySize);
            m_parent_o_stream->SeekO(here);
            m_lasterror = m_parent_o_stream->GetLastError();
        } else {
            m_lasterror = wxSTREAM_WRITE_ERROR;
        }
    }

    m_headerOffset += m_headerSize + compressedSize;
    m_headerSize = m_entrySize = 0;
    m_store->Close();
    m_raw = false;

    if (IsOk())
        m_lasterror = m_parent_o_stream->GetLastError();
    else
        wxLogError(_("error writing zip entry '%s': bad crc or length"),
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

    if (m_comp->Write(buffer, size).LastWrite() != size)
        m_lasterror = wxSTREAM_WRITE_ERROR;
    m_crcAccumulator = crc32(m_crcAccumulator, (Byte*)buffer, size);
    m_entrySize += m_comp->LastWrite();

    return m_comp->LastWrite();
}

#endif // wxUSE_ZLIB && wxUSE_STREAMS && wxUSE_ZIPSTREAM
