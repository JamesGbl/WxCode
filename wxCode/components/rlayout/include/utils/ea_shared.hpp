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

#ifndef EA_SHARED_HPP
#define EA_SHARED_HPP

#include <string.h>	// For memset
#include <stddef.h>	// for size_t

#ifndef EA_LOG_ERROR
	#include <stdio.h>
	#define EA_LOG_ERROR(msg) puts(msg)
#endif


// First alloc size
#ifndef EA_START_SIZE
   #define EA_START_SIZE	4 
#endif

// Growth rate
#ifndef EA_GROW            
   #define EA_GROW(size) (size+size/2) 
#endif

// EA_REALLOC will use malloc/free/realloc rather then new/delete
#ifdef EA_REALLOC
	#include <stdlib.h>
#endif

// This is a constant saying 'insert/put at the end'
#define EA_AT_END ((int)0x80000000)

typedef void *(*AllocFunc)(size_t);
typedef void (*FreeFunc)(void *pv);
typedef void (*AssertFunc)( int expr_val );

// This is for setting breakpoints in case of error
#ifdef EA_DEBUG_HOOK
	// Function declaration for below
	void EA_DEBUG_HOOK(); 
#else
	#define EA_DEBUG_HOOK()
#endif

// This allows for either throwing an error or returning in ordinary fashion
// Or doing an assert (default in debug mode)
#ifndef EA_THROW
	#ifndef EA_NO_THROW
		#if defined(DO_DEBUG) || defined(__WXDEBUG__) || defined(DEBUG) || defined(__DEBUG__)
			#include <assert.h>
			#define EA_THROW(str) {EA_LOG_ERROR(str); EA_DEBUG_HOOK(); assert(0);}
			#define EA_THROW_RV(str,rv) {EA_LOG_ERROR(str); EA_DEBUG_HOOK(); assert(0); return rv;}
		#else
			#define EA_THROW(str) {EA_DEBUG_HOOK(); throw str;}
			#define EA_THROW_RV(str,rv) {EA_DEBUG_HOOK(); throw str;}
		#endif
	#else
		#define EA_THROW(str) {EA_LOG_ERROR(str); EA_DEBUG_HOOK(); return;}
		//#define EA_THROW(str) return
		#define EA_THROW_RV(str,rv) {EA_LOG_ERROR(str); EA_DEBUG_HOOK(); return rv;}
	#endif
#endif
	

template<class T> 
void *NewArrAlloc( size_t size ){
	return new T[size/sizeof(T)];
}

template<class T> 
void DelArrFree( void *pv ){
	delete [] (T*)pv;
}

#endif // EA_SHARED_HPP 


