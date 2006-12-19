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
#include "wx/wx.h"

#include "wx/servicediscoverybrowser.h"

#if wxUSE_SERVICE_DISCOVERY

#include "wx/servicediscoveryservice.h"

#include <dns_sd.h>


	
#pragma mark  -- Constructors and Destructors --

wxServiceDiscoveryBrowser::wxServiceDiscoveryBrowser( wxEvtHandler * pListener,
													  bool bUseThreads,
													  wxServiceDiscoveryService & rService,
													  bool	bAutoResolve )
: wxServiceDiscoveryTaskBase( pListener, bUseThreads ),
m_rService		( rService ),
m_bAutoResolve ( bAutoResolve )
{
}



wxServiceDiscoveryBrowser::~wxServiceDiscoveryBrowser( void )
{
	for( size_t i = 0; i < m_ActiveResolvers.GetCount(); i++ )
	{
		delete m_ActiveResolvers[0];
		m_ActiveResolvers.RemoveAt( 0 );
	}
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



const wxServiceDiscoveryResultArray & wxServiceDiscoveryBrowser::GetArray( void )
{
	return m_ResultsArray;
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
		
		event.SetMoreComing( bMoreComing );
		event.SetServiceName( wxString( serviceName, wxConvUTF8 ) );
		event.SetNetworkInterface( interfaceIndex );
		event.SetRegType( wxString( regtype, wxConvUTF8 ) );
		event.SetDomain( wxString( replyDomain, wxConvUTF8 ) );
		
		if ( ( flags & kDNSServiceFlagsAdd ) != 0 )
		{
			event.SetEventType( wxEVT_BONJOUR_ADD_SERVICE );
			
			m_ResultsArray.Add( event );
			
			if ( m_bAutoResolve )
			{
				wxServiceDiscoveryResolver * pResolve = new wxServiceDiscoveryResolver( m_pListener, 
																						m_bUseThreads, // use threads if the browser already is
																						event,
																						this );
				
				pResolve->Start();
				m_ActiveResolvers.Add( pResolve );
			}
		}
		else
		{
			event.SetEventType( wxEVT_BONJOUR_REMOVE_SERVICE );
			
			for( unsigned int i = 0; i < m_ResultsArray.Count(); i++ )
			{
				if ( m_ResultsArray[i] == event )
				{
					// we decrement i here so as to not skip anything when incrementing.
					m_ResultsArray.RemoveAt( i-- );
				}
			}
			
			if ( m_bAutoResolve )
			{
				for( unsigned int j = 0; j < m_ActiveResolvers.Count(); j++ )
				{
					if ( m_ActiveResolvers[j]->IsResolvingService( event ) )
					{
						// This service was still in the process of being resolved.
						// Stop that, as it isn't available anymore.
						delete m_ActiveResolvers[j];
						m_ActiveResolvers.RemoveAt( j );
						break;
					}
				}
			}
		}
		
		wxPostEvent( m_pListener, event );
	}
}


void wxServiceDiscoveryBrowser::ResolutionCompleted( wxServiceDiscoveryResolver * pResolver,
													 wxServiceDiscoveryResult & rResult )
{
	for( unsigned int i = 0; i < m_ResultsArray.Count(); i++ )
	{
		if ( m_ResultsArray[i] == rResult )
		{
			// we decrement i here so as to not skip anything when incrementing.
			m_ResultsArray.RemoveAt( i-- );
		}
	}
	
	m_ResultsArray.Add( rResult );

	
	for( unsigned int j = 0; j < m_ActiveResolvers.Count(); j++ )
	{
		if ( m_ActiveResolvers[j] == pResolver )
		{
			m_ActiveResolvers.RemoveAt( j );
			delete pResolver;
			break;
		}
	}
}


#endif // wxUSE_SERVICE_DISCOVERY

