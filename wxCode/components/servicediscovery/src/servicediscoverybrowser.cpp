// --
// --	servicediscoverybrowser.cpp
// --
// --	revision   	$Rev$
// --	changed by	$Author$
// --	modified	$Date$
// --
// --	Created by Hank Schultz on 10/16/06.
// --	Copyright © 2006 Cedrus Corporation. wxWidgets licence.
// --
// --



#include <wx/wxprec.h>
#include <dns_sd.h>


#include "wx/servicediscoverybrowser.h"
#include "wx/servicediscoveryresult.h"
#include "wx/servicediscoveryservice.h"




	
	
#pragma mark  -- Constructors and Destructors --

wxServiceDiscoveryBrowser::wxServiceDiscoveryBrowser( wxEvtHandler * pListener,
													  bool bUseThreads,
													  wxServiceDiscoveryService & rService )
	: wxServiceDiscoveryTaskBase( pListener, bUseThreads ),
	m_rService		( rService )
{
}



wxServiceDiscoveryBrowser::~wxServiceDiscoveryBrowser( void )
{
}






#pragma mark						-
#pragma mark  						-- Searching for Services --

bool wxServiceDiscoveryBrowser::DoStart( void )
{
	DNSServiceBrowse( &m_rServiceRef, 
					  0, // no flags defined 
					  0, // any interface
					  m_rService.GetServiceType().mb_str( wxConvUTF8 ), 
					  NULL, // default domain 
					  BrowserCallback, 
					  this ); 
	
	return m_rServiceRef != NULL;
}




#pragma mark						-
#pragma mark  						-- Browser Callbacks --


void wxServiceDiscoveryBrowser::BrowserCallback ( DNSServiceRef sdRef, 
											   DNSServiceFlags flags, 
											   uint32_t interfaceIndex, 
											   DNSServiceErrorType errorCode, 
											   const char *serviceName, 
											   const char *regtype, 
											   const char *replyDomain, 
											   void *context )
{
	static_cast<wxServiceDiscoveryBrowser *>( context )->DoHandleBrowserCallback( sdRef, flags, interfaceIndex, errorCode, serviceName, regtype, replyDomain );
}



void wxServiceDiscoveryBrowser::DoHandleBrowserCallback(	DNSServiceRef WXUNUSED_UNLESS_DEBUG( sdRef ), 
														DNSServiceFlags flags, 
														uint32_t interfaceIndex, 
														DNSServiceErrorType errorCode, 
														const char *serviceName, 
														const char *regtype, 
														const char *replyDomain )
{
	wxLogDebug( wxT("Got browser callback:  error code:  %d  flags:  %x   name:  %s   regtype:  %s   domain:  %s"),
				int(errorCode),
				int(flags),
				wxString( serviceName, wxConvUTF8 ).c_str(),
				wxString( regtype, wxConvUTF8 ).c_str(),
				wxString( replyDomain, wxConvUTF8 ).c_str() );
	
	wxASSERT( m_pListener != NULL );
	wxASSERT( m_rServiceRef == sdRef );

	if ( errorCode != kDNSServiceErr_NoError )
	{
		wxLogDebug( wxT("Received an error from the bonjour browser:  %d"),
					errorCode );
	}
	else
	if ( m_pListener != NULL )
	{
		const bool bMoreComing = ( flags & kDNSServiceFlagsMoreComing ) != 0;
		
		wxServiceDiscoveryResult event;
		
		if ( ( flags & kDNSServiceFlagsAdd ) != 0 )
		{
			event.SetEventType( wxEVT_BONJOUR_ADD_SERVICE );
		}
		else
		{
			event.SetEventType( wxEVT_BONJOUR_REMOVE_SERVICE );
		}
		
		event.SetMoreComing( bMoreComing );
		event.SetServiceName( wxString( serviceName, wxConvUTF8 ) );
		event.SetNetworkInterface( interfaceIndex );
		event.SetRegType( wxString( regtype, wxConvUTF8 ) );
		event.SetDomain( wxString( replyDomain, wxConvUTF8 ) );
	
		wxPostEvent( m_pListener, event );
	}
}



