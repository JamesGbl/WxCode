/////////////////////////////////////////////////////////////////////////////
// Name:        freephone.cpp
// Purpose:     wxFreePhone class, for driving FreePhone
// Author:      Julian Smart
// Modified by:
// Created:     10/6/98
// Copyright:   Julian Smart
// Licence:     As in readme
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#if !defined(_AFXDLL) && defined(_WINDOWS)
#include <windows.h>
#endif

#include "tts/freephone.h"
#include "t2s.h"
#include "MFDict.h"

wxFreePhone::wxFreePhone(const CStringRef dictionaryPath)
{
    m_implementation = new tsFreePhoneImplementation;

    m_implementation->init(& m_implementation->config_struct, & m_implementation->buffer_struct, & m_implementation->ling_list_struct, & m_implementation->sent_struct,
        & m_implementation->spl_struct, & m_implementation->ps_struct, & m_implementation->as_struct);

    strcpy(m_implementation->config_struct.prog, "wxFreePhone");
    m_implementation->config_struct.input_file = "-";
    m_implementation->config_struct.output_file = "-";
    m_implementation->config_struct.hash_file = "-";
    m_implementation->config_struct.diphone_file = "-";
    m_implementation->config_struct.format = "sun8k";
    strcpy(m_implementation->config_struct.prog,"high_level");
    m_implementation->config_struct.type = _MBROLA_;

#if !defined(__FP_MFC__) && !defined(__FP_WXWIN__)
    m_dictionaryPath = NULL;
    m_dictionaryFile = NULL;
    m_keywordFile = NULL;
    m_indexFile = NULL;
#else
    m_dictionaryPath = "";
    m_dictionaryFile = "";
    m_keywordFile = "";
    m_indexFile = "";
#endif

    FP_STRING_ASSIGN(m_dictionaryPath, dictionaryPath);

#if !defined(__FP_MFC__) && !defined(__FP_WXWIN__)
    if (m_dictionaryPath && strlen(m_dictionaryPath) > 0)
#else
    if (m_dictionaryPath != "")
#endif
    {
        char buf[512];
        strcpy(buf, (const char*) m_dictionaryPath);
        strcat(buf, ".dct");
        FP_STRING_ASSIGN(m_dictionaryFile, buf);
        strcpy(buf, (const char*) m_dictionaryPath);
        strcat(buf, ".key");
        FP_STRING_ASSIGN(m_keywordFile, buf);
        strcpy(buf, (const char*) m_dictionaryPath);
        strcat(buf, ".idx");
        FP_STRING_ASSIGN(m_indexFile, buf);
    }
}

wxFreePhone::~wxFreePhone()
{
    CleanUp();

    delete m_implementation;

    FP_STRING_DELETE(m_dictionaryPath);
    FP_STRING_DELETE(m_dictionaryFile);
    FP_STRING_DELETE(m_keywordFile);
    FP_STRING_DELETE(m_indexFile);
}

// Initialises dictionary etc.
Bool wxFreePhone::Init()
{
    if (!FP_STRING_ISEMPTY(m_dictionaryFile))
    {
	    m_implementation->config_struct.phoneme_db = MFOpenDictionaryEx((char*) (const char*) m_indexFile,
            (char*) (const char*) m_keywordFile,
            (char*) (const char*) m_dictionaryFile);
    }
    return TRUE;
}

// Cleans up dictionary etc.
Bool wxFreePhone::CleanUp()
{
    m_implementation->terminate(& m_implementation->config_struct, & m_implementation->buffer_struct, & m_implementation->ling_list_struct, & m_implementation->sent_struct,
        & m_implementation->spl_struct, & m_implementation->ps_struct, & m_implementation->as_struct);
    return TRUE;
}


#if defined(__FP_MFC__) || defined(__FP_WXWIN__)
Bool wxFreePhone::TextToPhoneme(const CStringRef text, CStringRef phonemes)
#else
Bool wxFreePhone::TextToPhoneme(const CStringRef text, char** phonemes)
#endif
{
    CONFIG *config = & m_implementation->config_struct;
    BUFFER *buffer = & m_implementation->buffer_struct;
    LING_LIST *ling_list = & m_implementation->ling_list_struct;
    SENT *sent = & m_implementation->sent_struct;
    SPROSOD_LIST *spl = & m_implementation->spl_struct;
    SPN *ps = &m_implementation->ps_struct;
    ACOUSTIC *as = &m_implementation->as_struct;

#if defined(__FP_MFC__) || defined(__FP_WXWIN__)
    CString inputFile(GetTempFilename());
    CString outputFile(GetTempFilename());
#else
    char inputFile[512];
    char outputFile[512];
    strcpy(inputFile, GetTempFilename());
    strcpy(outputFile, GetTempFilename());
#endif

    if (!WriteToFile(inputFile, text))
        return FALSE;

    // Set input and output files
    config->input_file = (char *) (const char*) inputFile;
    config->output_file = (char *) (const char*) outputFile;

	if((config->ifd=fopen(config->input_file,"r")) == NULL)
	{
		return FALSE;
	}

	m_implementation->output_open(config);

    if(!strcmp(config->prog,"low_level")) {
        m_implementation->go2(config, ps, as);
    } else if(!strcmp(config->prog,"high_level")) {
        m_implementation->go3(config, buffer, ling_list, sent, spl, ps);
    } else if(!strcmp(config->prog,"test_diphones")) {
#ifdef DEBUG
//        m_implementation->test_diphones(config, sent, spl, ps, as);
#endif
        ;
    } else {
        m_implementation->go(config, buffer, ling_list, sent, spl, ps, as);
    }

	m_implementation->output_close(config);
    if (!ReadFromFile(outputFile, phonemes))
    {
        // Delete the temporary files
        unlink((char *)(const char*) inputFile);
        unlink((char *)(const char*) outputFile);
        return FALSE;
    }

    // Delete the temporary files
    unlink((char *)(const char*) inputFile);
    unlink((char *)(const char*) outputFile);

    return TRUE;
}

Bool wxFreePhone::TextToPhonemeFile(const CStringRef text, const CStringRef phonemeFile)
{
    CONFIG *config = & m_implementation->config_struct;
    BUFFER *buffer = & m_implementation->buffer_struct;
    LING_LIST *ling_list = & m_implementation->ling_list_struct;
    SENT *sent = & m_implementation->sent_struct;
    SPROSOD_LIST *spl = & m_implementation->spl_struct;
    SPN *ps = &m_implementation->ps_struct;
    ACOUSTIC *as = &m_implementation->as_struct;

#if defined(__FP_MFC__) || defined(__FP_WXWIN__)
    CString inputFile(GetTempFilename());
    CString outputFile(phonemeFile);
#else
    char inputFile[512];
    char outputFile[512];
    strcpy(inputFile, GetTempFilename());
    strcpy(outputFile, phonemeFile);
#endif

    if (!WriteToFile(inputFile, text))
        return FALSE;

    // Set input and output files
    config->input_file = (char *) (const char*) inputFile;
    config->output_file = (char *) (const char*) outputFile;

	if((config->ifd=fopen(config->input_file,"r")) == NULL)
	{
		return FALSE;
	}

	m_implementation->output_open(config);

    if(!strcmp(config->prog,"low_level")) {
        m_implementation->go2(config, ps, as);
    } else if(!strcmp(config->prog,"high_level")) {
        m_implementation->go3(config, buffer, ling_list, sent, spl, ps);
    } else if(!strcmp(config->prog,"test_diphones")) {
#ifdef DEBUG
//        m_implementation->test_diphones(config, sent, spl, ps, as);
#endif
        ;
    } else {
        m_implementation->go(config, buffer, ling_list, sent, spl, ps, as);
    }

    // Delete the temporary file
    unlink((char *)(const char*) inputFile);

	m_implementation->output_close(config);

    return TRUE;
}

Bool wxFreePhone::WriteToFile(const CStringRef filename, const CStringRef text)
{
    FILE *fd = fopen((char *) (const char*) filename, "w");
    if (!fd)
        return FALSE;

    int len = strlen((const char*) text);
    int i;
    for (i = 0; i < len; i++)
    {
        putc(text[i], fd);
    }
    fclose(fd);
    return TRUE;
}

#if defined(__FP_MFC__) || defined(__FP_WXWIN__)
Bool wxFreePhone::ReadFromFile(const CStringRef filename, CStringRef text)
#else
Bool wxFreePhone::ReadFromFile(const CStringRef filename, char** text)
#endif
{
    FILE *fd = fopen((char *) (const char*) filename, "r");
    if (!fd)
        return FALSE;

#if defined(__FP_MFC__) || defined(__FP_WXWIN__)
    int ch;
    while ((ch = getc(fd)) != EOF)
        text += ch;
    fclose(fd);
    return TRUE;
#else
    int ch;
    int len = 0;
    int i = 0;

    // Must first get the length of the buffer required
    // before reading in the text.
    while ((ch = getc(fd)) != EOF)
        len ++;
    char* buf = new char[len+1];

    // Set to the start of the file
    fseek(fd, 0, SEEK_SET);

    while ((ch = getc(fd)) != EOF)
    {
        buf[i] = ch;
        i ++;
    }
    buf[i] = 0;

    fclose(fd);
    *text = buf;
    return TRUE;
#endif
}

CString wxFreePhone::GetTempFilename() const
{
  char* prefix = "phone";

#ifdef _WINDOWS
#if defined(__FP_MFC__) || defined(__FP_WXWIN__)
  char buf[512];
#else
  static char buf[512];
#endif

#ifndef WIN32
  char tmp[256];
  ::GetTempFileName(0, prefix, 0, tmp);
#else
  char tmp[256];
  char tmpPath[256];
  ::GetTempPath(256, tmpPath);
  ::GetTempFileName(tmpPath, prefix, 0, tmp);
#endif
  strcpy(buf, tmp);

#if defined(__FP_MFC__) || defined(__FP_WXWIN__)
  return CString(buf);
#else
  return buf;
#endif

#else
  static short last_temp = 0;   // cache last to speed things a bit
  // At most 1000 temp files to a process! We use a ring count.
  static char buf[64];
  char dest[256];

  for (short suffix = last_temp + 1; suffix != last_temp; ++suffix %= 1000)
    {
      sprintf (buf, "/tmp/%s%d.%03x", prefix, (int) getpid (), (int) suffix);
      if (!FileExists (buf))
        {
          // Touch the file to create it (reserve name)
          FILE *fd = fopen (buf, "w");
          if (fd)
            fclose (fd);
          last_temp = suffix;
          strcpy(dest, buf);
          return buf;
        }
    }
  return "";

#endif
}

Bool wxFreePhone::FileExists (const char *filename) const
{
  struct stat stbuf;

  // (char *) cast necessary for VMS
  if (filename && stat ((char *)filename, &stbuf) == 0)
    return TRUE;
  return FALSE;
}

char *wxFreePhone::copystring (const char *s)
{
  if (s == NULL) s = "";
  size_t len = strlen (s) + 1;

  char *news = new char[len];
  memcpy (news, s, len);

  return news;
}

tsFreePhoneImplementation::tsFreePhoneImplementation()
{
}

tsFreePhoneImplementation::~tsFreePhoneImplementation()
{
}

