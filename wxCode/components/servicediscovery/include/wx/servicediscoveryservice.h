// --
// --	servicediscoveryservice.h
// --
// --	@class wxServiceDiscoveryService
// --
// --	revision   	$Rev$
// --	changed by	$Author$
// --	modified	$Date$
// --
// --	Created by Hank Schultz on 10/16/06.
// --	@copyright © 2006 Cedrus Corporation. wxWidgets licence.
// --
// --

#ifndef _WX_SERVICE_DISCOVERY_SERVICE_H_
#define _WX_SERVICE_DISCOVERY_SERVICE_H_

#include "wx/servicediscovery.h"

#if wxUSE_SERVICE_DISCOVERY

#include <dns_sd.h>

#include "wx/servicediscoverytaskbase.h"



	
#pragma mark  -- class wxServiceDiscoveryService --

class WXDLLIMPEXP_SERVICEDISCOVERY wxServiceDiscoveryService : public wxServiceDiscoveryTaskBase
{
public:

	#pragma mark  						-- Constructors and Destructors --

										wxServiceDiscoveryService( wxEvtHandler *	pListener,
																   bool				bUseThreads,
																   wxString			type,
																   wxUint16			port );
	
	virtual								~wxServiceDiscoveryService( void );	
	
										
	
	#pragma mark  						-- Publishing a Service --
										
	virtual	bool						DoStart( void );
	virtual bool						Stop( void );

	
										
	#pragma mark 						-
	#pragma mark  						-- Server Info --
	
	virtual	wxString					GetServiceType( void ) const { return m_Type; }
	virtual	wxUint16					GetServicePort( void ) const { return m_Port; }
	
	virtual	wxString					GetServiceName( void ) const { return m_Name; }

	
protected:
		
	#pragma mark  						-- Browser/Registration Callbacks --
		

	static void	DNSSD_API				RegistrationCallback( DNSServiceRef sdRef, 
															  DNSServiceFlags flags, 
															  DNSServiceErrorType errorCode, 
															  const char *name, 
															  const char *regtype, 
															  const char *domain, 
															  void *context );

	virtual void						DoHandleRegistrationCallback( DNSServiceRef sdRef, 
																	  DNSServiceFlags flags, 
																	  DNSServiceErrorType errorCode, 
																	  const char *name, 
																	  const char *regtype, 
																	  const char *domain );

		
		
	#pragma mark 						-
	#pragma mark  						-- Member Variables --


	const	wxString					m_Type;
			wxString					m_Name;
	const	wxUint16					m_Port;
	
		
	DECLARE_NO_COPY_CLASS( wxServiceDiscoveryService )
};


#endif // wxUSE_SERVICE_DISCOVERY

#endif // _WX_SERVICE_DISCOVERY_SERVICE_H_
