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

#ifndef SIGTHROW_H
#define SIGTHROW_H

// This is a class throwed when a signal is caught. 
// We make it globally visible even when not using 
// 'thrown signals' to simplify app coding. It is 
// only used when SIGTHROW_NOLONGJUMP is defined, 
// if not such an exception is never thrown.
class stSignalException {
public:
    stSignalException( int sig_no, void *ebp ) : 
        m_sig_no(sig_no), m_ebp(ebp) { }
        
    int     m_sig_no;   // Signal number
    void    *m_ebp;     // Stack base pointer received in signal handler
    //void    *m_eip;     // We should also record instruction that caused exception
};

#ifdef SIGTHROW_ENABLE 

    bool stInstallSyncSignalHandlers( );
            
    #ifndef SIGTHROW_NOLONGJUMP
        // This version uses setjmp / longjmp
        #include <setjmp.h>
        
        #define stFRAME \
            stFrame __sttp; \
            int __sig_no = sigsetjmp(__sttp.sjb,1)
        
        #define stTRY \
            stFRAME; \
            if( !__sig_no )
        
        #define stEXCEPT else
            
        
        #define stTRY_THROW \
            stFRAME; \
            if( __sig_no ) \
                throw __sig_no;

		#define sigtry \
			try{ \
				stTRY_THOW
		
		#define sigcatch \
			}catch
    
        struct stFrame {
            stFrame();
            ~stFrame();
            
            sigjmp_buf sjb;
        protected:
            stFrame *prev;
            void Ctor();
            void Dtor();
        };
        
	#else   // !defined(SIGTHROW_NOLONGJUMP)
    
        // The light-weight version, without setjmp

        // This version does not rely on setjmp / longjmp
        // Instead it just restores the stack to the "try" point
        // and jumps to a fix location. 
        // It is x86 specific
        struct stFrame {
            stFrame( ) : m_do_throw(false){ }
            stFrame( bool do_throw ); 	// Constructor used only when throwing in signal handler
            ~stFrame();
            
        protected:
            int *m_ebp;			// Stack frame for caller
            int *m_esp;
            int *m_eip;			// Return address
#ifdef SIGTHROW_SIGNAL_MASK_IN_FRAME            
            int  m_sig_mask;	// Deprecated.
#endif            
            bool m_do_throw;	// Determines if throw or return 
            
            stFrame *prev;
            //void Ctor();
            void Dtor();
            
            friend void stSignalHandler( int sig_nr, siginfo_t *psi, void *pv );
            friend stSignalException *stSetJmp( stFrame *pstf );
        };

        stSignalException *stSetJmp( stFrame *pstf );
        //void stSetJmpThrow( stFrame *pstf );

		#define stFRAME \
		    stFrame __stf; \
		    stSignalException *__pse = stSetJmp(&__stf);
		
		#define stTRY \
		    stFRAME; \
		    if( !__pse )
		
		#define stEXCEPT else
		    
		#define sigtry \
			try{ \
		    	stFrame __stf(true);

		#define sigcatch \
			}catch
		    	
    #endif  // SIGTHROW_NOLONGJUMP
    
    struct stFrame;
	
	// Return top frame for this thread
	stFrame *stGetTop();

#else
    // When this is not used
	#define stTRY
	#define stCATCH
	#define stFRAME_THROW
	#define sigtry     try
	#define sigcatch   catch
#endif // ENABLE_SIGTHROW 

#endif // SIGTHROW_H

