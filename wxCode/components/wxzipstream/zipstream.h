/////////////////////////////////////////////////////////////////////////////
// Name:        zipstream.h
// Purpose:     Memory stream classes
// Author:      Ryan Norton
// Modified by:
// Created:     09/05/03
// RCS-ID:      $Id: zipstream.h,v 1.1 2004-07-17 20:08:04 ryannpcs Exp $
// Copyright:   (c) Ryan Norton
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef _WX_WXZSTREAM_H__
#define _WX_WXZSTREAM_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "zipstream.h"
#endif

#include "wx/defs.h"

#if wxUSE_ZLIB && wxUSE_STREAMS

#include "wx/stream.h"

struct wxZipFileInfo
{
	wxString szName;
	wxString szComment;

	wxUint32 dwUncompressedSize;
	wxUint32 dwCompressedSize;
	wxUint32 dwTime;
};

class wxZipInputStream : public wxFilterInputStream
{
public:
	wxZipInputStream(wxInputStream& stream);
	virtual ~wxZipInputStream();

	bool OpenFile(const char* szFileName);
	bool OpenNextFile();
	bool GetFileInfo(wxZipFileInfo& Info);

	wxInputStream& ReadRaw(void* pBuffer, size_t size);
	off_t TellRawI();
	off_t SeekRawI(off_t pos, wxSeekMode sm = wxFromStart);

	void* GetHandleI() {return hZip;}
protected:
	virtual size_t OnSysRead(void *buffer, size_t size);

	void* hZip;
	void* pFileFuncs;
};
class wxZipOutputStream : public wxFilterOutputStream
{
public:
	wxZipOutputStream(wxOutputStream& stream);
	virtual ~wxZipOutputStream();

	//named it CreateFile, but then got link error in MSVC???
	bool MakeFile(wxZipFileInfo& Info, wxInt32 level = -1);

	wxOutputStream& WriteRaw(void* pBuffer, size_t size);
	off_t TellRawO();
	off_t SeekRawO(off_t pos, wxSeekMode sm = wxFromStart);

	void* GetHandleO() {return hZip;}
protected:
	size_t OnSysWrite(void *buffer, size_t bufsize);
	
	void* hZip;
	void* pFileFuncs;
};
class wxZipStream : public wxZipInputStream, wxZipOutputStream
{
public:
	wxZipStream(wxInputStream& istream, wxOutputStream& ostream);
	virtual ~wxZipStream();

};

#endif
#endif