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
	
	HINSTANCE							instance; 
    WNDCLASSEX							wcex; 
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




#endif // _WX_SERVICE_DISCOVERY_TASK_BASE_H_
