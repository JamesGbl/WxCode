// --
// --	servicediscoverytaskbase.cpp
// --
// --	revision   	$Rev$
// --	changed by	$Author$
// --	modified	$Date$
// --
// --	Created by Hank Schultz on 10/25/06.
// --	Copyright © 2006 Cedrus Corporation. wxWidgets licence.
// --
// --

#include <wx/wxprec.h>
#include "wx/servicediscoverytaskbase.h"

#ifdef _WIN32
	#define DNSSD_EVENT        (WM_USER + 0x100) 
#endif

	
#pragma mark  						-- Helper Thread --
	
wxServiceDiscoveryTaskBase::wxServiceDiscoveryServiceHelperThread::wxServiceDiscoveryServiceHelperThread( DNSServiceRef rService )
	: wxThread( wxTHREAD_JOINABLE ),
	m_ServiceRef ( rService )
{
}
	


void *	wxServiceDiscoveryTaskBase::wxServiceDiscoveryServiceHelperThread::Entry()
{
		//	static wxCriticalSection section;
		//	wxCriticalSectionLocker section_locker( section );
		
		wxASSERT( ! wxThread::IsMain() );
		
		bool cancel_running = false;
		DNSServiceErrorType err = kDNSServiceErr_NoError; 
		
		do
		{
			err = DNSServiceProcessResult( m_ServiceRef );
			
			if ( err )
			{
				if ( err == kDNSServiceErr_Unknown )
				{
					wxLogDebug( wxT("Received an unknown bonjour error, but this was probably just a method of quitting our thread.") );
				}
				else
				{
					wxLogDebug( wxT("got error!  %d"),
								err );
				}
				cancel_running = true;
			}
			
		} while ( ! TestDestroy() && !cancel_running );
		
		return NULL;
}



#pragma mark  -
#pragma mark  -- Constructors and Destructors --

wxServiceDiscoveryTaskBase::wxServiceDiscoveryTaskBase( wxEvtHandler * pListener,
												  bool bUseThreads )
: m_pListener	( pListener ),
m_rServiceRef	( NULL ),
m_bUseThreads	( bUseThreads ),
m_pThread		( NULL )
{
#ifdef __DARWIN__
	cfsocket = NULL;
	source = NULL;
#endif
}



wxServiceDiscoveryTaskBase::~wxServiceDiscoveryTaskBase( void )
{
	Stop();
}



#pragma mark						-
#pragma mark  						-- mDNS interaction --

bool wxServiceDiscoveryTaskBase::Start( void )
{
	wxASSERT( m_rServiceRef EQ NULL );

	bool output = DoStart();
	
	if ( output )
	{
		wxASSERT( m_rServiceRef NE NULL );
		
		if ( m_rServiceRef NE NULL )
		{
			if ( m_bUseThreads )
			{
				wxASSERT( m_pThread EQ NULL );
				
				if ( m_pThread NE NULL )
				{
					m_pThread->Delete();
					delete m_pThread;
					m_pThread = NULL;
				}
				
				m_pThread = new wxServiceDiscoveryServiceHelperThread( m_rServiceRef );
				
				if ( m_pThread NE NULL )
				{
					m_pThread->Create();
					m_pThread->Run();
				}
			}
			else
			{
				dns_sd_fd = DNSServiceRefSockFD( m_rServiceRef );

				#ifdef __DARWIN__
				
					CFSocketContext context = { 0, this, NULL, NULL, NULL }; 
				
					cfsocket = CFSocketCreateWithNative(NULL, 
														dns_sd_fd, 
														kCFSocketReadCallBack, 
														wxServiceDiscoveryTaskBase::Mac_Socket_Callback, 
														&context);
					
					// Prevent CFSocketInvalidate from closing DNSServiceRef's socket. 
					CFOptionFlags f = CFSocketGetSocketFlags( cfsocket ); 
					CFSocketSetSocketFlags( cfsocket, f & ~kCFSocketCloseOnInvalidate); 
					
					source = CFSocketCreateRunLoopSource( NULL, cfsocket, 0 ); 
					CFRunLoopAddSource( CFRunLoopGetCurrent(), source, kCFRunLoopCommonModes );
					
				#elif defined( _WIN32 )
					
				    // Create the window. This window won't actually be shown, 
					// but it demonstrates how to use DNS-SD with Windows GUI 
					// applications by having DNS-SD events processed as messages 
					// to a Window. 
					instance = GetModuleHandle(NULL); 
					assert(instance); 
					wcex.cbSize = sizeof(wcex); 
					wcex.style     = 0; 
					wcex.lpfnWndProc = (WNDPROC) WndProc; 
					wcex.cbClsExtra     = 0; 
					wcex.cbWndExtra     = 0; 
					wcex.hInstance = instance; 
					wcex.hIcon     = NULL; 
					wcex.hCursor = NULL; 
					wcex.hbrBackground = NULL; 
					wcex.lpszMenuName = NULL; 
					wcex.lpszClassName = TEXT("ZeroconfExample"); 
					wcex.hIconSm = NULL; 
					RegisterClassEx(&wcex); 
					wind = CreateWindow(wcex.lpszClassName, 
										wcex.lpszClassName,
										0, 
										CW_USEDEFAULT,
										0,
										CW_USEDEFAULT,
										0,
										NULL, 
										NULL,
										instance,
										this ); 

					SetProp( wind, wxT("wxServiceDiscoveryTaskBase pointer"), this );
					
					
					// Associate the DNS-SD browser with our window 
					// using the WSAAsyncSelect mechanism. Whenever something 
					// related to the DNS-SD browser occurs, our private Windows message 
					// will be sent to our window so we can give DNS-SD a 
					// chance to process it. This allows DNS-SD to avoid using a 
					// secondary thread (and all the issues with synchronization that 
					// would introduce), but still process everything asynchronously. 
					
					// This also simplifies app code because DNS-SD will only run when we 
					// explicitly call it. 
					
					err = WSAAsyncSelect( dns_sd_fd, 
										  wind, 
										  DNSSD_EVENT, 
										  FD_READ | FD_CLOSE); 
					
					assert(err == kDNSServiceErr_NoError); 

				#else
					
					nfds = dns_sd_fd + 1;
					
					FD_ZERO(&readfds);
					FD_SET(dns_sd_fd,&readfds);
					
				#endif
			}
		}
	}
	
	return output;	
}


bool wxServiceDiscoveryTaskBase::Stop( void )
{
	bool output = false;
	
	if ( ! m_bUseThreads )
	{
		#ifdef __DARWIN__
		
			if ( ( CFRunLoopGetCurrent() NE NULL ) && 
				 ( source NE NULL ) &&
				 ( cfsocket NE NULL ) )
			{
				// Remove the CFRunLoopSource from the current run loop. 
				CFRunLoopRemoveSource(CFRunLoopGetCurrent(), source, kCFRunLoopCommonModes); 
				CFRelease(source); 
				// Invalidate the CFSocket. 
				CFSocketInvalidate(cfsocket); 
				CFRelease(cfsocket); 
				
				cfsocket = NULL;
				source = NULL;
				
				// Workaround to give time to CFSocket's select() thread 
				// so it can remove the socket from its FD set before we 
				// close the socket by calling DNSServiceRefDeallocate. 
				usleep(1000); 
			}
			
		#elif defined( _WIN32 )
			
			if ( wind NE NULL )
			{
				// Clean up. This is not strictly necessary since the normal 
				// process cleanup will close DNS-SD socket(s) and release memory, 
				// but it's here to demonstrate how to do it. 
				WSAAsyncSelect( dns_sd_fd,
								wind, 
								DNSSD_EVENT,
								0 ); 
				wind = NULL;
			}
			
		#endif
	}
	
	
	if ( m_rServiceRef NE NULL )
	{
		// Order is important.  We deallocate the DNSServiceRef *prior* to deleting
		// the thread.  The thread is blocking permanently for a response on this
		// service reference.  If we delete the thread first, we wind up in a
		// deadlock situation.  If we delete the service first, we get an error,
		// but at least the thread stops.  As long as we prepare for that error,
		// everything will be fine.
		
		DNSServiceRefDeallocate( m_rServiceRef );
		m_rServiceRef = NULL;
		output = true;
	}
	
	if ( m_bUseThreads )
	{
		if ( m_pThread NE NULL )
		{
			m_pThread->Delete();
			delete m_pThread;
			m_pThread = NULL;
		}
	}
	
	return output;
}



#pragma mark						-
#pragma mark  						-- Handling Socket Data --


#ifdef __DARWIN__


void wxServiceDiscoveryTaskBase::Mac_Socket_Callback( CFSocketRef s, 
												  CFSocketCallBackType callbackType,
												  CFDataRef address, 
												  const void* data, 
												  void* info )
{
	static_cast<wxServiceDiscoveryTaskBase *>( info )->Mac_Socket_Callback( s, callbackType, address, data );
}


void wxServiceDiscoveryTaskBase::Mac_Socket_Callback( CFSocketRef WXUNUSED( s ), 
												   CFSocketCallBackType WXUNUSED( callbackType ),
												   CFDataRef WXUNUSED( address ), 
												   const void* WXUNUSED( data ) )
{
	wxLogDebug( wxT("Got socket event callback.") );
	
	DNSServiceErrorType err = DNSServiceProcessResult( m_rServiceRef ); 
	
	if (err != kDNSServiceErr_NoError) 
	{ 
		Stop();
	} 	
}


#elif defined( _WIN32 )
LRESULT CALLBACK wxServiceDiscoveryTaskBase::WndProc(HWND inWindow,
												  UINT inMsg,
												  WPARAM inWParam,
												  LPARAM inLParam ) 
{ 
    LRESULT result; 
    switch(inMsg) 
	{ 
        case DNSSD_EVENT: 
			// Process the DNS-SD event. All DNS-SD callbacks occur from 
			// within this function. 
			{
				wxServiceDiscoveryTaskBase * self = reinterpret_cast<wxServiceDiscoveryTaskBase *>( GetProp( inWindow, wxT("wxServiceDiscoveryTaskBase pointer") ) );

				if (DNSServiceProcessResult( self->m_rServiceRef ) != kDNSServiceErr_NoError) 
					result = -1; 
				else 
					result = 0; 
			}
			break; 
        default: 
			result = DefWindowProc(inWindow, inMsg, inWParam, inLParam); 
			break; 
	} 
    return(result); 
} 

#else

bool wxServiceDiscoveryTaskBase::DoIdle( void )
{
	bool output = ! m_bUseThreads;
	
	if ( output )
	{
		tv.tv_sec = 0;
		tv.tv_usec = 1000;
		
		int result = select(nfds, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);
		
		if ( result > 0 )
		{
			DNSServiceErrorType err = kDNSServiceErr_NoError; 
			if ( FD_ISSET(dns_sd_fd, &readfds) )
			{
				wxLogDebug( wxT("got something!") );
				err = DNSServiceProcessResult(m_rServiceRef);
			}
			if ( err )
			{
				wxLogDebug( wxT("got error!") );
				output = false;
			}
		}
		else
		{
			if ( result < 0 )
			{
				wxLogDebug( wxT("got error!") );
				output = false;
			}
			//		else
			//			wxLogDebug( wxT("got nothing...") );
		}
	}
	
	return output;
}

#endif

