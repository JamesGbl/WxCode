// --
// --	servicediscoveryresolver.h
// --
// --	@class wxServiceDiscoveryResolver
// --
// --	revision   	$Rev$
// --	changed by	$Author$
// --	modified	$Date$
// --
// --	Created by Hank Schultz on 10/26/06.
// --	@copyright © 2006 Cedrus Corporation. wxWidgets licence.
// --
// --

#ifndef _WX_SERVICE_DISCOVERY_RESOLVER_H_
#define _WX_SERVICE_DISCOVERY_RESOLVER_H_

#include "wx/servicediscovery.h"

#if wxUSE_SERVICE_DISCOVERY

#include "wx/servicediscoverytaskbase.h"
#include "wx/servicediscoveryresult.h"


class wxServiceDiscoveryBrowser;

	
#pragma mark  -- class wxServiceDiscoveryResolver --

class WXDLLIMPEXP_SERVICEDISCOVERY wxServiceDiscoveryResolver : public wxServiceDiscoveryTaskBase
{
public:

	#pragma mark  						-- Constructors and Destructors --

										wxServiceDiscoveryResolver( wxEvtHandler * pListener,
																	bool bUseThreads,
																	wxServiceDiscoveryResult & rResult,
																	wxServiceDiscoveryBrowser * pParentBrowser = NULL );

	virtual								~wxServiceDiscoveryResolver( void );


	#pragma mark  						-- Resolving Services --

	virtual	bool						DoStart( void );
	
	virtual bool						IsResolvingService( const wxServiceDiscoveryResult & rResult ) const { return m_Result == rResult; }

	
	
protected:
		
	#pragma mark  						-- Resolution Callbacks --
		
		
	static void	DNSSD_API				ResolveCallback(	DNSServiceRef sdRef, 
															DNSServiceFlags flags, 
															uint32_t interfaceIndex, 
															DNSServiceErrorType errorCode, 
															const char *fullname, 
															const char *hosttarget, 
															uint16_t port, 
															uint16_t txtLen, 
															const char *txtRecord, 
															void *context ); 
	
	virtual void						DoHandleResolveCallback(	DNSServiceRef sdRef, 
																	DNSServiceFlags flags, 
																	uint32_t interfaceIndex, 
																	DNSServiceErrorType errorCode, 
																	const char *fullname, 
																	const char *hosttarget, 
																	uint16_t port, 
																	uint16_t txtLen, 
																	const char *txtRecord ); 
	
	
protected:
	#pragma mark 						-
	#pragma mark  						-- Member Variables --

		
	wxServiceDiscoveryResult			m_Result;
	wxServiceDiscoveryBrowser *			m_pParentBrowser;
	
	DECLARE_NO_COPY_CLASS( wxServiceDiscoveryResolver )
};


WX_DECLARE_USER_EXPORTED_OBJARRAY( wxServiceDiscoveryResolver*, wxServiceDiscoveryResolveArray, WXDLLIMPEXP_SERVICEDISCOVERY );


#endif // wxUSE_SERVICE_DISCOVERY

#endif // _WX_SERVICE_DISCOVERY_RESOLVER_H_
