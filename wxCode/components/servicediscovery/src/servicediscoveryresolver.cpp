// --
// --	servicediscoveryresolver.cpp
// --
// --	revision   	$Rev$
// --	changed by	$Author$
// --	modified	$Date$
// --
// --	Created by Hank Schultz on 10/26/06.
// --	Copyright © 2006 Cedrus Corporation. wxWidgets licence.
// --
// --

#include "wx/wxprec.h"
#include "wx/wx.h"
#include "wx/arrimpl.cpp"

#include "wx/servicediscoveryresolver.h"

#if wxUSE_SERVICE_DISCOVERY

#include "wx/servicediscoveryresult.h"
#include "wx/servicediscoverybrowser.h"


WX_DEFINE_USER_EXPORTED_OBJARRAY( wxServiceDiscoveryResolveArray );


#pragma mark  -- Constructors and Destructors --

wxServiceDiscoveryResolver::wxServiceDiscoveryResolver( wxEvtHandler * pListener,
														bool bUseThreads,
														wxServiceDiscoveryResult & rResult,
														wxServiceDiscoveryBrowser * pParentBrowser )
	: wxServiceDiscoveryTaskBase( pListener, bUseThreads ),
	m_Result		( rResult ),
	m_pParentBrowser ( pParentBrowser )
{
}



wxServiceDiscoveryResolver::~wxServiceDiscoveryResolver( void )
{
}



#pragma mark						-
#pragma mark  						-- Resolving Services --

bool wxServiceDiscoveryResolver::DoStart( void )
{
	DNSServiceResolve( &m_rServiceRef, 
					   0, // no flags defined 
					   m_Result.NetworkInterface(),
					   m_Result.ServiceName().mb_str( wxConvUTF8 ),
					   m_Result.RegType().mb_str( wxConvUTF8 ), 
					   m_Result.Domain().mb_str( wxConvUTF8 ), 
					   ResolveCallback, 
					   this ); 
	
	return m_rServiceRef != NULL;
}




#pragma mark						-
#pragma mark  						-- Resolution Callbacks --

void wxServiceDiscoveryResolver::ResolveCallback(	DNSServiceRef sdRef, 
													DNSServiceFlags flags, 
													uint32_t interfaceIndex, 
													DNSServiceErrorType errorCode, 
													const char *fullname, 
													const char *hosttarget, 
													uint16_t port, 
													uint16_t txtLen, 
													const char *txtRecord, 
													void *context )
{
	static_cast<wxServiceDiscoveryResolver *>( context )->DoHandleResolveCallback( sdRef, flags, interfaceIndex, errorCode, 
																				   fullname, hosttarget, port, txtLen, txtRecord );
}



void wxServiceDiscoveryResolver::DoHandleResolveCallback(	DNSServiceRef WXUNUSED_UNLESS_DEBUG( sdRef ), 
															DNSServiceFlags flags, 
															uint32_t interfaceIndex, 
															DNSServiceErrorType errorCode, 
															const char *fullname, 
															const char *hosttarget, 
															uint16_t port, 
															uint16_t WXUNUSED( txtLen ), 
															const char *txtRecord )
{
	wxASSERT( m_pListener != NULL );
	wxASSERT( m_rServiceRef == sdRef );
	
	if ( ( m_pListener != NULL ) ||
		 ( m_pParentBrowser != NULL ) )
	{
		m_Result.SetEventType( wxEVT_BONJOUR_RESOLVE_SERVICE );

		if ( errorCode != kDNSServiceErr_NoError )
		{
			wxLogDebug( wxT("Received an error from the bonjour resolver:  %d"),
						errorCode );
			m_Result.SetResolved( false );
		}
		else 
		{
			const bool bMoreComing = ( flags & kDNSServiceFlagsMoreComing ) != 0;
			
			m_Result.SetMoreComing( bMoreComing );
			m_Result.SetNetworkInterface( interfaceIndex );
			
			m_Result.SetResolved( true );
			
			m_Result.SetFullName( wxString( fullname, wxConvUTF8 ) );
			m_Result.SetPort( wxUINT16_SWAP_ON_BE( port ) );
			m_Result.SetTarget( wxString( hosttarget, wxConvUTF8 ) );
			m_Result.SetTextRecord( wxString( txtRecord, wxConvUTF8 ) );
		}
		
		if ( m_pListener != NULL )
			wxPostEvent( m_pListener, m_Result );
		
		if ( m_pParentBrowser != NULL )
		{
			// Note that the parent could delete us here, so we can't reference
			// this afterwards
			m_pParentBrowser->ResolutionCompleted( this, m_Result );
		}
	}
}


#endif // wxUSE_SERVICE_DISCOVERY
