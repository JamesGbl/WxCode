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
#include "wx/servicediscoverytaskbase.h"
#include "wx/servicediscoveryresult.h"



	
#pragma mark  -- class wxServiceDiscoveryResolver --

class WXDLLIMPEXP_SERVICEDISCOVERY wxServiceDiscoveryResolver : public wxServiceDiscoveryTaskBase
{
public:

	#pragma mark  						-- Constructors and Destructors --

										wxServiceDiscoveryResolver( wxEvtHandler * pListener,
																 bool bUseThreads,
																 wxServiceDiscoveryResult & rResult,
																 bool bDeleteWhenDone = false );

	virtual								~wxServiceDiscoveryResolver( void );


	#pragma mark  						-- Resolving Services --

	virtual	bool						DoStart( void );

	
	
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

		
	wxServiceDiscoveryResult				m_Result;
	bool								m_bDeleteWhenDone;
	
	DECLARE_NO_COPY_CLASS( wxServiceDiscoveryResolver )
};




#endif // _WX_SERVICE_DISCOVERY_RESOLVER_H_
