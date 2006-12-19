// --
// --	servicediscoverytaskbase.h
// --
// --	@class wxServiceDiscoveryTaskBase
// --
// --	revision   	$Rev$
// --	changed by	$Author$
// --	modified	$Date$
// --
// --	Created by Hank Schultz on 10/25/06.
// --	@copyright © 2006 Cedrus Corporation. wxWidgets licence.
// --
// --

#ifndef _WX_SERVICE_DISCOVERY_TASK_BASE_H_
#define _WX_SERVICE_DISCOVERY_TASK_BASE_H_


#include "wx/servicediscovery.h"
#include "wx/thread.h"

#if wxUSE_SERVICE_DISCOVERY

#include <dns_sd.h>

#ifdef _WIN32
	#include <process.h>
	typedef int pid_t;
	#define getpid _getpid
	#define strcasecmp _stricmp
	#define snprintf _snprintf

	#pragma comment(lib, "dnssd.lib")
	#pragma comment(lib, "WSock32.Lib")
#else
	#include <sys/select.h>
#endif

//  This is the base class for all service discovery tasks.  There are four
//  methods implimented in this file (and the accompanying cpp file):
//
//  1)  Thread-based				-- potentially supports all platforms.  YMMV.
//  2)  select() based				-- potentially supports all platforms.  Requires calling DoIdle() on 
//									   a regular basis.  Does not appear to work correctly.  YMMV.
//  3)  CFSocket/CFRunLoop based	-- supports OS X.  Requires an event loop.  Appears stable to author
//  4)  HWND Events					-- supports Windows.  Requires an event loop.  Appears stable to author
//
//  Regardless of the implementation being used, tasks must be started before they will do anything.
//  Tasks will continue to run, producing results, until stopped.  Generally, this is what you want.
//  If you are advertising a service, you need to create a wxServiceDiscoveryService object and Start()
//  it.  Your service will need a service name and a port.  Once you do this, it will appear in the 
//  bonjour domain.  Generally, you would use the same service name for all instances of your service
//  (e.g.  http://developer.apple.com/qa/qa2001/qa1312.html ).  If you are looking for other instances
//  of a type of service, create a service object (or use one you already have); you don't have to start
//  it if you aren't broadcasting.  You do have to start the browser, and to receive notification of
//  results, you have to pass an event handler to the constructor.  You can instruct the browser
//  to automatically resolve results for you, in which case you will not need to create a resolver
//  object for each of your results.
//
//  The browser is designed to keep track of results *for* you so that you don't necessarily have to
//  do this on your own.  However, this array of results includes the service that may be running on
//  your local machine.  If you do have the service running on your local machine, you may want to
//  ignore results that have the same ServiceName() as your local server's service name
//  ( wxServiceDiscoveryService::GetServiceName() ).
//
//  A list of events you may want your event handler to receive is located at the bottom of
//  servicediscoveryresult.h.  wxServiceDiscoveryResult is a subclass of wxEvent, but it represents
//  all of the information you will ever receive from bonjour.  It's important to not that
//  operator==() does not compare to items to see if they are identical--it compares two items
//  to see if they represent the same service over the same network interface.  Therefore, a
//  a resolved result should be considered equal to an unresolved result that references the
//  same service.  This should make it significantly easier to iterate through a list of results
//  and compare two results in order to update an old result with a new result.


#pragma mark  -- class wxServiceDiscoveryTaskBase --

class WXDLLIMPEXP_SERVICEDISCOVERY wxServiceDiscoveryTaskBase
{
public:

	#pragma mark  						-- Constructors and Destructors --

										wxServiceDiscoveryTaskBase( wxEvtHandler * pListener,
																	bool bUseThreads );

	virtual								~wxServiceDiscoveryTaskBase( void );

	
	#pragma mark						-
	#pragma mark  						-- mDNS interaction --

			bool						Start( void );
	virtual bool						Stop( void );
	
protected:
	virtual bool						DoStart( void ) = 0;

	
	
	#pragma mark						-
	#pragma mark  						-- Handling Socket Data --

protected:
#ifdef __DARWIN__
	static void							Mac_Socket_Callback( CFSocketRef s, 
															 CFSocketCallBackType callbackType,
															 CFDataRef address, 
															 const void* data, 
															 void* info );
	
	virtual void						Mac_Socket_Callback( CFSocketRef s, 
															 CFSocketCallBackType callbackType,
															 CFDataRef address, 
															 const void* data );
#elif defined( _WIN32 )
	static	LRESULT CALLBACK 			WndProc(HWND inWindow, 
												UINT inMsg, 
												WPARAM inWParam, 
												LPARAM inLParam);
#else
public:
		// Not used if you are using threads.
		virtual	bool						DoIdle( void );
#endif
	

protected:
		
	#pragma mark  						-- Helper Thread --
		
	class WXDLLIMPEXP_SERVICEDISCOVERY wxServiceDiscoveryServiceHelperThread : public wxThread
	{
	public:
		wxServiceDiscoveryServiceHelperThread( DNSServiceRef rService );
		
	protected:
		virtual void *	Entry();
			
		
		DNSServiceRef m_ServiceRef;
	};
	
		
	
	
	#pragma mark 						-
	#pragma mark  						-- Member Variables --

	wxEvtHandler *						m_pListener;	
	DNSServiceRef						m_rServiceRef;
	bool								m_bUseThreads;

	wxServiceDiscoveryServiceHelperThread*	m_pThread;

	
	int									dns_sd_fd;

#ifdef __DARWIN__
	
	CFSocketRef         				cfsocket;   // CFSocket for this operation 
    CFRunLoopSourceRef  				source;     // RunLoopSource for this CFSocket 
	
#elif defined( _WIN32 )
	
	static	WNDCLASSEX &				GetWndClass( void );
	
	static	HINSTANCE					instance; 
//    WNDCLASSEX							wcex; 
    HWND								wind; 
    MSG									msg; 
    int									err; 	
	
#else
	
	int									nfds;
	fd_set								readfds;
	struct								timeval tv;
	
#endif

		
	DECLARE_NO_COPY_CLASS( wxServiceDiscoveryTaskBase )
};


#endif // wxUSE_SERVICE_DISCOVERY

#endif // _WX_SERVICE_DISCOVERY_TASK_BASE_H_
