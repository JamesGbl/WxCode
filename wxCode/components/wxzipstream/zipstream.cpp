/////////////////////////////////////////////////////////////////////////////
// Name:        zipstream.cpp
// Purpose:     Zip stream classes
// Author:      Ryan Norton
// Modified by:
// Created:     09/05/03
// RCS-ID:      $Id: zipstream.cpp,v 1.1 2004-07-17 20:08:04 ryannpcs Exp $
// Copyright:   (c) Guilhem Lavaux
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

//===========================================================================
//                          HEADERS
//===========================================================================

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "zipstream.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#if wxUSE_ZLIB && wxUSE_STREAMS

#include "zipstream.h"
#include "wx/utils.h"
#include "wx/intl.h"
#include "wx/log.h"

#include "zip/zlib.h"
#include "zip/ioapi.h"
#include "zip/zip.h"
#include "zip/unzip.h"

//===========================================================================
//                          IMPLEMENTATION
//===========================================================================

/*
#include "wx/module.h"

class wxZipModule : public wxModule
{
	DECLARE_DYNAMIC_CLASS(wxZipModule);
public:
	wxZipModule(){}
	~wxZipModule(){}

	static zlib_filefunc_def funcs;
	bool OnInit()
	{
		fill_fopen_filefunc(&funcs);
		return true;
	}
	void OnExit()
	{
	}
};
IMPLEMENT_DYNAMIC_CLASS(wxZipModule, wxModule);

zlib_filefunc_def wxZipModule::funcs;
*/
//////////////////////////////////////////////////////////////////////////
//						IOAPI STUFF										//
//////////////////////////////////////////////////////////////////////////
//	This is where that magical i/o re-routing is done with the zip		//
//	ioapi to let us use streams instead of files or memory				//
//////////////////////////////////////////////////////////////////////////
struct wxIOAPI
{
	//ZLIB_FILEFUNC_MODE_READ || ZLIB_FILEFUNC_MODE_WRITE etc.
	int mode;
	wxStreamBase* pStream;
};

voidpf ZCALLBACK fopen_wx_func OF((
   voidpf opaque,
   const char* filename,
   int mode))
{	
	wxIOAPI* io = new wxIOAPI;
	io->mode = mode;
	io->pStream = ((wxStreamBase*) wxConstCast(filename, char));

    return io;
}

uLong ZCALLBACK fread_wx_func OF((
   voidpf opaque,
   voidpf stream,
   void* buf,
   uLong size))
{
	wxIOAPI* io = (wxIOAPI*) stream;
	wxZipInputStream* pStream = (wxZipInputStream*) io->pStream;
	off_t p = pStream->TellRawI();
	pStream->ReadRaw(buf, size);
//	wxInputStream* s = &pStream->ReadRaw(buf, size);
//	return s->LastRead();
	return pStream->TellRawI() - p;
}

uLong ZCALLBACK fwrite_wx_func OF((
   voidpf opaque,
   voidpf stream,
   const void* buf,
   uLong size))
{
	wxIOAPI* io = (wxIOAPI*) stream;
	wxZipOutputStream* pStream = (wxZipOutputStream*) io->pStream;
	return pStream->WriteRaw((void*)buf, size).LastWrite();
}

long ZCALLBACK ftell_wx_func OF((
   voidpf opaque,
   voidpf stream))
{
	wxIOAPI* io = (wxIOAPI*) stream;
	if((io->mode & ZLIB_FILEFUNC_MODE_READ) &&
	   !(io->mode & ZLIB_FILEFUNC_MODE_WRITE))
		return ((wxZipInputStream*) io->pStream)->TellRawI();
	else
		return ((wxZipOutputStream*) io->pStream)->TellRawO();
}


long ZCALLBACK fseek_wx_func OF((
   voidpf opaque,
   voidpf stream,
   uLong offset,
   int origin))
{
	wxIOAPI* io = (wxIOAPI*) stream;

	switch (origin)
    {
    case ZLIB_FILEFUNC_SEEK_CUR :
        origin = wxFromCurrent;
        break;
    case ZLIB_FILEFUNC_SEEK_END :
		origin = wxFromEnd;
        break;
    case ZLIB_FILEFUNC_SEEK_SET :
		origin = wxFromStart;
        break;
    default: return -1;
    }

	if((io->mode & ZLIB_FILEFUNC_MODE_READ) &&
	   !(io->mode & ZLIB_FILEFUNC_MODE_WRITE))
	{
		((wxZipInputStream*) io->pStream)->SeekRawI(offset, (wxSeekMode) origin);
//		return ((wxZipInputStream*) io->pStream)->TellI();
		return 0;
	}
	else
	{
		((wxZipOutputStream*) io->pStream)->SeekRawO(offset,(wxSeekMode) origin);
//		return ((wxZipOutputStream*) io->pStream)->TellO();
		return 0;  //should do error checking here...
	}
}

int ZCALLBACK fclose_wx_func OF((
   voidpf opaque,
   voidpf stream))
{
	delete (wxIOAPI*) stream;

	//that was easy!
	return 0;
}

int ZCALLBACK ferror_wx_func OF((
   voidpf opaque,
   voidpf stream))
{
	wxIOAPI* io = (wxIOAPI*) stream;

	char *szMode, *szOK;
	size_t nTell;
	if((io->mode & ZLIB_FILEFUNC_MODE_READ) &&
	   !(io->mode & ZLIB_FILEFUNC_MODE_WRITE))
	{
		szMode = "Input";
		nTell = ((wxZipInputStream*)io->pStream)->TellRawI();
	}
	else
	{
		szMode = "Output";
		nTell = ((wxZipOutputStream*)io->pStream)->TellRawO();
	}

	if(io->pStream->IsOk())
		szOK = "Yes";
	else
		szOK = "No";

	wxLogSysError("Misc error in zipstream...\nzipstream type:%s\naddress:%x\nisok?:%s\ntell:%i",
					szMode, io->pStream, szOK, nTell);

	return 0;
}

void fill_wx_filefunc (zlib_filefunc_def* pzlib_filefunc_def)
{
    pzlib_filefunc_def->zopen_file = fopen_wx_func;
    pzlib_filefunc_def->zread_file = fread_wx_func;
    pzlib_filefunc_def->zwrite_file = fwrite_wx_func;
    pzlib_filefunc_def->ztell_file = ftell_wx_func;
    pzlib_filefunc_def->zseek_file = fseek_wx_func;
    pzlib_filefunc_def->zclose_file = fclose_wx_func;
    pzlib_filefunc_def->zerror_file = ferror_wx_func;
    pzlib_filefunc_def->opaque = NULL;
}

// ----------------------------------------------------------------------------
// wxZipInputStream
// ----------------------------------------------------------------------------

wxZipInputStream::wxZipInputStream(wxInputStream& Stream) : wxFilterInputStream(Stream)
{
	pFileFuncs = new zlib_filefunc_def;
	fill_wx_filefunc((zlib_filefunc_def*)pFileFuncs);

	hZip = unzOpen2((const char*) this, (zlib_filefunc_def*) pFileFuncs/*&wxZipModule::funcs*/);

	if (hZip == NULL)
	{
		wxLogSysError("Could not open zip file");
		m_lasterror = wxSTREAM_EOF;
	}

	if (unzGoToFirstFile(hZip) != UNZ_OK)
	{
		wxLogSysError("Could not seek to initial position");
		m_lasterror = wxSTREAM_EOF;
	}

	if (unzOpenCurrentFile(hZip) != UNZ_OK)
	{
		wxLogSysError("Could not open initial file");
		m_lasterror = wxSTREAM_EOF;
	}
}

wxZipInputStream::~wxZipInputStream()
{
	unzClose(hZip);
	delete (zlib_filefunc_def*) pFileFuncs;
//	delete m_parent_i_stream;
}

wxInputStream& wxZipInputStream::ReadRaw(void* pBuffer, size_t size)
{
	return m_parent_i_stream->Read(pBuffer, size);
}

off_t wxZipInputStream::TellRawI()
{
	return m_parent_i_stream->TellI();
}

off_t wxZipInputStream::SeekRawI(off_t pos, wxSeekMode sm)
{
	return m_parent_i_stream->SeekI(pos, sm);
}

bool wxZipInputStream::OpenNextFile()
{
	//don't check for errors here...
	unzCloseCurrentFile(hZip);

	if (unzGoToNextFile(hZip) != UNZ_OK)
	{
		m_lasterror = wxSTREAM_EOF;
		return false;
	}

	if (unzOpenCurrentFile(hZip) != UNZ_OK)
	{
		m_lasterror = wxSTREAM_READ_ERROR;
		return false;
	}

	return true;
}

bool wxZipInputStream::OpenFile(const char* szFileName)
{
	//don't check for errors here...
	unzCloseCurrentFile(hZip);

	if (unzLocateFile(hZip, szFileName, 0) != UNZ_OK)
		return false;

	if (unzOpenCurrentFile(hZip) != UNZ_OK)
	{
		m_lasterror = wxSTREAM_READ_ERROR;
		return false;
	}

	return true;
}

bool wxZipInputStream::GetFileInfo(wxZipFileInfo& Info)
{
	unz_file_info info;

	//I can't imagine how this would fail...
	unzGetCurrentFileInfo(hZip, &info, NULL, 0, NULL, 0, NULL, 0);

	Info.dwCompressedSize = info.compressed_size;
	Info.dwUncompressedSize = info.uncompressed_size;
	Info.dwTime = info.dosDate;

	Info.szName.Alloc(info.size_filename+1);
	Info.szComment.Alloc(info.size_file_comment+1);

	unzGetCurrentFileInfo(hZip, NULL, (char*) Info.szName.c_str(), info.size_filename,
								NULL, 0, 
								(char*) Info.szName.c_str(), info.size_file_comment);

	//arg, doesn't add null character :)
 	Info.szName[(size_t)info.size_filename] = '\0';
	Info.szComment[(size_t)info.size_file_comment] = '\0';

	return true;
}

size_t wxZipInputStream::OnSysRead(void* buffer, size_t bufsize)
{
	size_t nRead;
	nRead = unzReadCurrentFile(hZip, buffer, bufsize);

	return nRead;	
}

// ----------------------------------------------------------------------------
// wxZipOutputStream
// ----------------------------------------------------------------------------

wxZipOutputStream::wxZipOutputStream(wxOutputStream& Stream) : wxFilterOutputStream(Stream)
{
	pFileFuncs = new zlib_filefunc_def;
	fill_wx_filefunc((zlib_filefunc_def*)pFileFuncs);

	hZip = zipOpen2((const char*) this, APPEND_STATUS_CREATEAFTER, NULL,
					(zlib_filefunc_def*) pFileFuncs/*&wxZipModule::funcs*/);

	if (hZip == NULL)
	{
		wxLogSysError("Could not write zip file");
		m_lasterror = wxSTREAM_EOF;
	}
}

wxZipOutputStream::~wxZipOutputStream()
{
	zipClose(hZip, NULL);
	delete (zlib_filefunc_def*) pFileFuncs;
//	delete m_parent_o_stream;
}

wxOutputStream& wxZipOutputStream::WriteRaw(void* pBuffer, size_t size)
{
	return m_parent_o_stream->Write(pBuffer, size);
}

off_t wxZipOutputStream::TellRawO()
{
	return m_parent_o_stream->TellO();
}

off_t wxZipOutputStream::SeekRawO(off_t pos, wxSeekMode sm)
{
	return m_parent_o_stream->SeekO(pos, sm);
}

bool wxZipOutputStream::MakeFile(wxZipFileInfo& Info, wxInt32 level)
{
	zipCloseFileInZip(hZip);

	if (level == -1)
		level = Z_DEFAULT_COMPRESSION;

	zip_fileinfo info;
	tm_zip t;
	t.tm_hour=t.tm_mday=t.tm_min=t.tm_mon=t.tm_sec=t.tm_year=0;
	info.external_fa=0;
	info.internal_fa=0;
	info.tmz_date = t;
	info.dosDate = Info.dwTime; //0 == current time
	if (zipOpenNewFileInZip(hZip, Info.szName, 
		&info,			
		NULL,0,NULL,0  //some etc. stuff
		, Info.szComment.c_str()		//comment
		, Z_DEFLATED		//compression method -> Zip
		, level) != ZIP_OK)
	{
		return false;
	}
	
	return true;
}

size_t wxZipOutputStream::OnSysWrite(void* buffer, size_t bufsize)
{
	size_t nWrote;
	nWrote = zipWriteInFileInZip(hZip, (void*) buffer, bufsize);
	return nWrote;	
}

// ----------------------------------------------------------------------------
// wxZipStream
// ----------------------------------------------------------------------------

wxZipStream::wxZipStream(wxInputStream& i, wxOutputStream& o) : 
	wxZipInputStream(i), wxZipOutputStream(o) {}

wxZipStream::~wxZipStream(){}

#endif  //wxUSE_ZLIB && wxUSE_STREAMS