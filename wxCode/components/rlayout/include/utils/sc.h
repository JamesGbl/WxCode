#ifndef SC_H
#define SC_H

// Simple defines to work with ansi/UTF8/unicode 
// with 'sc* prefixed functions

#undef scfprintf
#undef scfscanf
#undef scfgetc
#undef scfopen
#undef scvprintf
#undef scsprintf
#undef scvsnprintf
#undef scstrnicmp
#undef scstrncmp
#undef scstrcmp
#undef scstrncpy
#undef scstrcpy
#undef scstrstr
#undef scstrchr
#undef scstrrchr
#undef scstrlen
#undef scstrtol
#undef scisspace
#undef _SC


#if defined(SCUNICODE) && !defined(SC_FORCE_ANSI)
    //#ifndef _SC
        #define _SC(a) L##a
    //#endif
    #define scfprintf   fwprintf
    #define scfscanf    fwscanf
    #define scfgetc     fwgetc
    #define scfopen	    _wfopen
    #define scvprintf   vwprintf
    #define scsprintf   swprintf
    #define scvsnprintf vswnprintf
    #define	scstrnicmp	wcsnicmp
    #define	scstrncmp	wcsncmp
    #define	scstrcmp	wcscmp
    #define	scstrncpy	wcsncpy
    #define	scstrcpy	wcscpy
    #define	scstrstr	wcsstr
    #define	scstrchr	wcschr
    #define	scstrrchr	wcsrchr
    #define	scstrlen	(int)wcslen
    #define	scstrtol	wcstol
    #define scisspace(ch) (ch>0 && ch<128 ? isspace(char(ch)) : false)
#else
    //#ifndef _SC
        #define _SC(a)  a
    //#endif
    #define scfprintf   fprintf
    #define scfscanf    fscanf
    #define scfgetc     fgetc
    #define scfopen	    fopen
    #define scvprintf   vprintf
    #define scsprintf   sprintf
    #define scvsnprintf vsnprintf
    #define	scstrnicmp	strnicmp
    #define	scstrncmp	strncmp
    #define	scstrcmp	strcmp
    #define	scstrncpy	strncpy
    #define	scstrcpy	strcpy
    #define	scstrstr	strstr
    #define	scstrchr	strchr
    #define	scstrrchr	strrchr
    #define	scstrlen	(int)strlen
    #define	scstrtol	strtol
    #define scisspace   isspace
#endif

#endif // SC_H

