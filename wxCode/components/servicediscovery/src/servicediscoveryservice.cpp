// --
// --	servicediscoveryservice.cpp
// --
// --	revision   	$Rev$
// --	changed by	$Author$
// --	modified	$Date$
// --
// --	Created by Hank Schultz on 10/16/06.
// --	Copyright © 2006 Cedrus Corporation. wxWidgets licence.
// --
// --

#include "wx/wxprec.h"

#include "wx/servicediscoveryservice.h"
#include "wx/servicediscoveryresult.h"






#pragma mark  -- Constructors and Destructors --

wxServiceDiscoveryService::wxServiceDiscoveryService( wxEvtHandler *	pListener,
													  bool				bUseThreads,
													  wxString			type,
													  wxUint32			port )
: wxServiceDiscoveryTaskBase( pListener, bUseThreads ),
m_Type ( type ),
m_Port ( port )
{
}



wxServiceDiscoveryService::~wxServiceDiscoveryService( void )
{
}


#pragma mark						-
#pragma mark  						-- Publishing a Service --

bool wxServiceDiscoveryService::DoStart( void )
{
	bool output = false;
	
	DNSServiceErrorType error = DNSServiceRegister( &m_rServiceRef,
													0, // flags, unused
													0, // interface index, any
													NULL, // The OS will use the machine name and automatically handle collisions, which is what we want
													m_Type.mb_str( wxConvUTF8 ),
													NULL, // The OS will use the .local domain, which is what we want
													NULL, // current host, i.e. our self, not some other machine
													m_Port,
													0, // text size
													NULL, // text ptr
													RegistrationCallback,
													this );
	
	if ( error == kDNSServiceErr_NoError )
	{			
		output = true;
	}
	
	return output;
}


bool wxServiceDiscoveryService::Stop( void )
{
	bool output = wxServiceDiscoveryTaskBase::Stop();
	m_Name = wxEmptyString;
	return output;
}




#pragma mark						-
#pragma mark  						-- Registration Callbacks --
//
//
//
//
void wxServiceDiscoveryService::RegistrationCallback( DNSServiceRef sdRef, 
													  DNSServiceFlags flags, 
													  DNSServiceErrorType errorCode, 
													  const char *name, 
													  const char *regtype, 
													  const char *domain, 
													  void *context )
{
	static_cast<wxServiceDiscoveryService *>( context )->DoHandleRegistrationCallback( sdRef, flags, errorCode, name, regtype, domain );
}


void wxServiceDiscoveryService::DoHandleRegistrationCallback( DNSServiceRef sdRef, 
															  DNSServiceFlags flags, 
															  DNSServiceErrorType errorCode, 
															  const char *name, 
															  const char *regtype, 
															  const char *domain )
{
	wxLogDebug( wxT("Got registration callback:  error code:  %d   name:  %s   regtype:  %s   domain:  %s"),
				int(errorCode),
				wxString( name, wxConvUTF8 ).c_str(),
				wxString( regtype, wxConvUTF8 ).c_str(),
				wxString( domain, wxConvUTF8 ).c_str() );
	
	if ( errorCode != kDNSServiceErr_NoError )
	{
		wxLogDebug( wxT("Received an error from the bonjour browser:  %d"),
					errorCode );
	}
	else
	{
		m_Name = wxString( name, wxConvUTF8 );
		
		if ( m_pListener != NULL )
		{
			const bool bMoreComing = ( flags & kDNSServiceFlagsMoreComing ) != 0;
			
			wxServiceDiscoveryResult event;
			
			event.SetEventType( wxEVT_BONJOUR_REGISTER_SERVICE );
			
			event.SetMoreComing( bMoreComing );
			event.SetServiceName( wxString( name, wxConvUTF8 ) );
			event.SetNetworkInterface( 0 );
			event.SetRegType( wxString( regtype, wxConvUTF8 ) );
			event.SetDomain( wxString( domain, wxConvUTF8 ) );
			
			wxPostEvent( m_pListener, event );
		}	
	}
}




