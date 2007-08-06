//
// Copyright (C) 2007 Arne Steinarson <arst at users dot sourceforge dot net>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must
//    not claim that you wrote the original software. If you use this
//    software in a product, an acknowledgment in the product
//    documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must
//    not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source
//    distribution.
//


#ifndef PLATFORM_H
#define PLATFORM_H

// This code has parts from Squirrel Shell -
// (c) 2006, Constantin Makshin (Dinosaur)

// Utility macro: PreProcessor to string
#define pp2str(s) #s
// Utility macro: PreProcessor concat
#define ppconc(s1,s2) s1##s2


// Detect platform
// Defines:
//   __WINDOWS__, __UNIX__,__UNIXLIKE__
//   PLATFORM_NAME
//   PTR_SIZE (64/32/16)
//   PTR_ALIGN_BITS (3/2/1)   - Number of trailing zeros on all pointers

// At the end we define a PLATFORM_FLAGS, using these
// It's a 16-bit integer and we only want to know platform here, 
// not int size or pointer size.
#define PFM_WINDOWS   0x01
#define PFM_UNIX      0x02
#define PFM_UNIX_LIKE 0x04
#define PFM_MAC       0x08


// Windows platforms
#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#  ifndef __WINDOWS__
#     define __WINDOWS__
#  endif
#  define PTR_SIZE 64
#  define PLATFORM_NAME "win64"
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#  ifndef __WINDOWS__
#     define __WINDOWS__
#  endif
#  define PTR_SIZE 32
#  define PLATFORM_NAME "win32"
#elif defined(WIN16) || defined(_WIN16) || defined(__WIN16__)
#  ifndef __WINDOWS__
#     define __WINDOWS__
#  endif
#  define PTR_SIZE 16
#  define PLATFORM_NAME "win16"
#endif
   
// Windows/Unix platforms
#if defined(__CYGWIN32__)
#  ifndef __WINDOWS__
#     define __WINDOWS__
#  endif
#  ifndef __UNIXLIKE__
#     define __UNIXLIKE__
#  endif
#  define PTR_SIZE 32
#  undef PLATFORM_NAME
#  define PLATFORM_NAME "cygwin"
#elif defined(__MINGW32__)
#  ifndef __WINDOWS__
#     define __WINDOWS__
#  endif
#  ifndef __UNIXLIKE__
#     define __UNIXLIKE__
#  endif
#  define PTR_SIZE 32 
#  undef PLATFORM_NAME
#  define PLATFORM_NAME "mingw32"
#endif
   
// Unix platforms
#if defined(linux) || defined(__linux__)
#  ifndef __UNIX__
#     define __UNIX__
#  endif
#  ifndef __LINUX__
#    define __LINUX__
#  endif   
#	if (defined(__LP64__) || defined(_LP64)) 
#     define PLATFORM_NAME "linux64"
#     define PTR_SIZE 64
#	else
#	  define PLATFORM_NAME	"linux32"
#     define PTR_SIZE 32
#	endif
#elif defined(__FREEBSD__)
#   define BSD
#   ifndef __UNIX__
#      define __UNIX__
#   endif
#	define PLATFORM_NAME	"freebsd"
#   define PTR_SIZE 32
#elif defined(__BSD__)
#   define BSD
#   ifndef __UNIX__
#      define __UNIX__
#   endif
#	define PLATFORM_NAME	"bsd"
#   define PTR_SIZE 32
#endif
    
#if defined(__MSDOS__) || defined(_MSDOS)
#   define  DOS
#	define	PLATFORM_NAME	"msdos"	// I'm not sure Squirrel Shell will even compile under DOS... :)
#   define PTR_SIZE 16
#elif defined(__OS2__) || defined(__EMX__)
#   define PLATFORM_NAME	"OS/2"
#   define PTR_SIZE 32
#elif defined(NETWARE)
#	define	PLATFORM_NAME	"netware"
#   define PTR_SIZE 32	// ??
#elif defined(__MWERKS__) && defined(macintosh)
#   define MAC 
#   define PTR_SIZE 32 
#	define PLATFORM_NAME	"macintosh"
#elif defined(VMS) || defined(__VMS)
#	define	PLATFORM_NAME	"vms"
#   define PTR_SIZE 32	// ??
#elif defined(__QNX__)
#	define	PLATFORM_NAME	"qnx"
#   define PTR_SIZE 32	// ??
#elif defined(NeXT) || defined(_NEXT_SOURCE)
#	define	PLATFORM_NAME	"next"
#   define PTR_SIZE 32	// ??
#endif

#ifdef __UNIX__
#  define __UNIXLIKE__
#endif

#ifndef PTR_SIZE
#   error Unknown platform     
#	define	PLATFORM_NAME	"unknown"
#endif

// Define PLATFORM_FLAGS based on above
#if defined(__WINDOWS__)
  #define PLATFORM_FLAGS PFM_WINDOWS
#elif defined(__UNIX__)
  #define PLATFORM_FLAGS PFM_UNIX
#elif defined(__MAC__)
  #define PLATFORM_FLAGS PFM_MAC
#elif defined(__UNIXLIKE__)
  #define PLATFORM_FLAGS PFM_UNIX_LIKE
#else
  #error Cannot define PLATFORM_FLAGS
#endif

#if PTR_SIZE==64
	#define PTR_ALIGN_BITS 3
#elif PTR_SIZE==32
	#define PTR_ALIGN_BITS 2
#elif PTR_SIZE==16
	#define PTR_ALIGN_BITS 1
#endif

#endif //PLATFORM_H


