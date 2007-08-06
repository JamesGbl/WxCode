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


#ifdef SIGTHROW_ENABLE

/*
 * This does only work on Unix system. It does not work on MinGW.
 */

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <csignal>
#include <signal.h>

#include "pi/piThread.h"
#include "pi/sigthrow.h"

struct stThreadFrame;

// The first stThreadFrame
stThreadFrame *g_ptf;

// Record one global instance of enabled signals
sigset_t g_st_sst;

bool g_st_did_init;


// This is a structure that is one-per-thread. They are stored in a linked list 
// and automatically allocated for new threads.
struct stThreadFrame {
	stThreadFrame( ) : ebp(NULL), sig_no(0), top(NULL) {
		thid = piGetThreadId();
        // Link this instance
		nxt = g_ptf;
        g_ptf = this;
   	}
	int 		thid;	// Owbers thread ID
    void        *ebp;	// Base pointer of exception trigger function
    int         sig_no; // The signal index
    
	stFrame 	  *top;
	stThreadFrame *nxt;
};


stThreadFrame *stGetThreadFrame( ){
	// The first time here, initialize default global signal mask
	if( !g_st_did_init ){
		//int sz = sizeof( pstf->m_sst );	// 128 bytes! too much to keep one for each frame
		sigprocmask( 0, NULL, &g_st_sst );
		g_st_did_init = true;
	}
	
	int thid = piGetThreadId();
	stThreadFrame *ptf = g_ptf;
	for( ;ptf && ptf->thid!=thid; ptf=ptf->nxt )
        ;
    return ptf;
}


extern "C" {
	// Exists just to throw an exception
	void stSignalThrow( stSignalException *pse ){
		throw pse;
	}
}

void stSignalHandler( int sig_nr, siginfo_t *psi, void *pv ){
	// We only come here from the signals we have installed handlers for 
	// (which should be only synchronous ones), so no need to check if 
	// we should process them. Do all.
	
	printf( "Signal: Received signal no %d.\n", sig_nr );
	
	// Get the topmost registered app frame
    stThreadFrame *pstf= stGetThreadFrame( );
	if( !pstf ){
		printf( "Signal(%d): Error - No thread specific frame found. Aborting. \n", sig_nr );
		assert( 0 );
	}
    
    // Save current stack frame for debug
    // # I think that %%ebp+4 is return address (violating instruction)
    // We should save that.
    pstf->sig_no = sig_nr;
    int *_ebp;
    asm volatile (  "mov %%ebp, %0 ;"     // Save base frame pointer
                    : "=r"(_ebp) : );     // Output
    pstf->ebp = _ebp;
    
    stFrame *ptf = pstf->top;
	if( !ptf ){
		printf( "Signal(%d): Error - No stack frame found. Aborting. \n", sig_nr );
		assert( 0 );
	}
	else{
#ifndef SIGTHROW_NOLONGJUMP
        // Longjump case
		siglongjmp( ptf->sjb, sig_nr );
#else
        // Without using longjump
        stSignalException *pse = new stSignalException( sig_nr, _ebp );
        if( !pse ){
            printf( "Signal(%d): Error - Failed allocating exception structure. Aborting. \n", sig_nr );
            assert( 0 );
        }
        /*int **/_ebp = ptf->m_ebp;
        int *_esp = ptf->m_esp;
        int *_eip = ptf->m_eip;
        
        // With signal masks:
#ifdef SIGTHROW_SIGNAL_MASK_IN_FRAME
        sigsetmask( ptf->m_sig_mask );     // Restore signal mask
#else
        // Restore global signal set
        sigprocmask( SIG_SETMASK, &g_st_sst, NULL );
#endif
        
        if( ptf->m_do_throw ){
        	// After setting up stack frame as when declaring stFrame, jump to throw function
	        asm volatile (
	                      "mov %%ebx, %%esp ;"  // Change stack
	                      "mov %%eax, %%ebp ;"  // Restore base pointer
			  		  	  "mov %%edx, (%%esp) ;"  // First arg (pse)
	                      "push %%ecx  ;"       // Put return address on top of (changed) stack
	        			  "jmp stSignalThrow ;" // Jump to new function 
	            :: "a"(_ebp), "b"(_esp), "c"(_eip), "d"(pse) ); // Inputs
        }
        else{
        	// After setting up stack frame as when declaring stFrame, just return
	        asm volatile (
	                      "mov %%ebx, %%esp ;"  // Change stack
	                      "mov %%eax, %%ebp ;"  // Restore base pointer
	                      "mov %%edx, %%eax ;"  // return value = pse
	                      "mov %%ecx, (%%esp) ;"  // Put return address on top of (changed) stack
	                      "ret"                 // Return to eip
	            :: "a"(_ebp), "b"(_esp), "c"(_eip), "d"(pse) ); // Inputs
        }
#endif
    }
}


// Install a signal handler that enables later throwing a C++ exception,
// (or a similar construct not based on try/throw/catch)
bool stInstallSyncSignalHandlers( ){
	// The synchronous signals 
	int signals [] = { SIGSEGV, SIGBUS, SIGFPE, SIGTRAP, SIGILL, 0 };
	for( int ix=0; signals[ix]; ix++ ){
		struct sigaction sa;
		memset (&sa, 0, sizeof (sa));
		sa.sa_sigaction = stSignalHandler;
		sa.sa_flags     = SA_SIGINFO | SA_NOMASK;
		sigaction( signals[ix], &sa, 0 );    
	}
	return true;
}


#ifndef SIGTHROW_NOLONGJUMP

stFrame::stFrame(){ Ctor(); }

void stFrame::Ctor(){
	stThreadFrame *ptf = stGetThreadFrame();
	if( !ptf ){
		// Put new frame first in linked list - in Ctor
		ptf = new stThreadFrame;
	}
	
	// We have a thread specific frame list, put this on top
	prev = ptf->top;
	ptf->top = this;
}
#else
// Constructor for throwing from signal handler
stFrame::stFrame( bool do_throw ) 
: m_do_throw(true) {
	stThreadFrame *ptf = stGetThreadFrame();
	if( !ptf ){
		// Put new frame first in linked list - in Ctor
		ptf = new stThreadFrame;
	}
	
	// Setup 'catch' environment 
    int *_ebp;
    int *_esp;
    int *_eip; 
    int  _sig_no;
    
	// We have a thread specific frame list, put pstf on top
	prev = ptf->top;
	ptf->top = this;
	
    // Save stack frame
    asm volatile ("mov (%%ebp), %0 ;"
    			  "mov %%ebp, %1 ;" 
		  		  "mov 4(%%ebp), %2 ;" 
        : "=r"(_ebp), "=r"(_esp), "=r"(_eip) : );	    // Outputs
    m_ebp = _ebp;     // Base pointer in previous frame
    m_esp = _esp+1;   // Stack pointer in previous frame
    m_eip = _eip;     // Return address 
#ifdef SIGTHROW_SIGNAL_MASK_IN_FRAME
    m_sig_mask = siggetmask();
#endif    
}
#endif


stFrame::~stFrame(){ Dtor(); }

void stFrame::Dtor(){
	int thid = piGetThreadId();
	stThreadFrame *ptf = stGetThreadFrame();
	if( !ptf ){
		// This is an error
		printf( "stFrame::Dtor - Could not find stThreadFrame. Aborting\n" );
		assert(0);
        return;
	}
	
	// Unlink ourselves
	ptf->top = prev;
	if( !prev ){
		// If we're the last, remove thread frame
		stThreadFrame **pptf = &g_ptf;
		for( ;*pptf && *pptf!=ptf; pptf=&(*pptf)->nxt );
		if( !*pptf ){
			// An error
			printf( "stFrame::Dtor - Could not remove current stThreadFrame. Aborting\n" );
            assert(0);
			return;
		}
		// Remove from linked list
		*pptf = ptf->nxt;
		delete ptf;
	}
}

stFrame *stGetTop(){
	int thid = piGetThreadId();
	stThreadFrame *ptf = g_ptf;
	for( ;ptf && ptf->thid!=thid; ptf=ptf->nxt );
	return ptf ? ptf->top : NULL;
}


#ifdef SIGTHROW_NOLONGJUMP
// The case without using longjmp

stSignalException *stSetJmp( stFrame *pstf ){
	stThreadFrame *ptf = stGetThreadFrame();
	if( !ptf ){
		// Put new frame first in linked list - in Ctor
		ptf = new stThreadFrame;
	}
	
	// Setup 'catch' environment 
    int *_ebp;
    int *_esp;
    int *_eip;
    int  _sig_no;
    if( pstf ){
    	// We have a thread specific frame list, put pstf on top
    	pstf->prev = ptf->top;
    	ptf->top = pstf;
    	
        // Save stack frame
        asm volatile ("mov (%%ebp), %0 ;"
        			  "mov %%ebp, %1 ;" 
  			  		  "mov 4(%%ebp), %2 ;" 
            : "=r"(_ebp), "=r"(_esp), "=r"(_eip) : );	    // Outputs
        pstf->m_ebp = _ebp;     // Base pointer in previous frame
        pstf->m_esp = _esp+1;   // Stack pointer in previous frame
        pstf->m_eip = _eip;     // Return address
#ifdef SIGTHROW_SIGNAL_MASK_IN_FRAME
        pstf->m_sig_mask = siggetmask();
#endif        
        return NULL;
    }
    else{
        assert( 0 );
        return NULL;
    }
}
#endif // SIGTHROW_NOLONGJUMP

#endif // SIGTHROW_ENABLE

