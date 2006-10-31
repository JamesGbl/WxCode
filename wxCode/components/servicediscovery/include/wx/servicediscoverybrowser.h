// --
// --	servicediscoverybrowser.h
// --
// --	@class wxServiceDiscoveryBrowser
// --
// --	revision   	$Rev$
// --	changed by	$Author$
// --	modified	$Date$
// --
// --	Created by Hank Schultz on 10/16/06.
// --	@copyright © 2006 Cedrus Corporation. wxWidgets licence.
// --
// --



#ifndef _WX_SERVICE_DISCOVERY_BROWSER_H_
#define _WX_SERVICE_DISCOVERY_BROWSER_H_


#include "wx/servicediscovery.h"
#include "wx/servicediscoverytaskbase.h"


class wxServiceDiscoveryResult;
class wxServiceDiscoveryService;
	
#pragma mark  -- class wxServiceDiscoveryBrowser --

class WXDLLIMPEXP_SERVICEDISCOVERY wxServiceDiscoveryBrowser : public wxServiceDiscoveryTaskBase
{
public:

	#pragma mark  						-- Constructors and Destructors --

										wxServiceDiscoveryBrowser( wxEvtHandler * pListener,
																   bool bUseThreads,
																   wxServiceDiscoveryService & rService );
	virtual								~wxServiceDiscoveryBrowser( void );
	
	
	
	#pragma mark  						-- Searching for Services --

	virtual	bool						DoStart( void );
	
	
	
protected:
		
	#pragma mark  						-- Browser Callbacks --
		
	static	void DNSSD_API				BrowserCallback ( DNSServiceRef sdRef, 
														  DNSServiceFlags flags, 
														  uint32_t interfaceIndex, 
														  DNSServiceErrorType errorCode, 
														  const char *serviceName, 
														  const char *regtype, 
														  const char *replyDomain, 
														  void *context ); 

	
	virtual	void						DoHandleBrowserCallback(	DNSServiceRef sdRef, 
																	DNSServiceFlags flags, 
																	uint32_t interfaceIndex, 
																	DNSServiceErrorType errorCode, 
																	const char *serviceName, 
																	const char *regtype, 
																	const char *replyDomain );

	
	
		
	#pragma mark 						-
	#pragma mark  						-- Member Variables --

protected:
			
	wxServiceDiscoveryService&			m_rService;
	
		
	DECLARE_NO_COPY_CLASS( wxServiceDiscoveryBrowser )
};






#endif // _WX_SERVICE_DISCOVERY_BROWSER_H_
