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

#if wxUSE_SERVICE_DISCOVERY

#include "wx/servicediscoverytaskbase.h"
#include "wx/servicediscoveryresult.h"
#include "wx/servicediscoveryresolver.h"


class wxServiceDiscoveryService;
	
#pragma mark  -- class wxServiceDiscoveryBrowser --

class WXDLLIMPEXP_SERVICEDISCOVERY wxServiceDiscoveryBrowser : public wxServiceDiscoveryTaskBase
{
public:

	#pragma mark  						-- Constructors and Destructors --

										wxServiceDiscoveryBrowser( wxEvtHandler * pListener,
																   bool bUseThreads,
																   wxServiceDiscoveryService & rService,
																   bool	bAutoResolve );
	virtual								~wxServiceDiscoveryBrowser( void );
	
	
	
	#pragma mark  						-- Searching for Services --

	virtual	bool						DoStart( void );
	
	virtual const wxServiceDiscoveryResultArray &
										GetArray( void );
	
	
protected:
		friend class wxServiceDiscoveryResolver;
	
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

	virtual	void						ResolutionCompleted( wxServiceDiscoveryResolver * pResolver,
															 wxServiceDiscoveryResult & rResult );
		
	#pragma mark 						-
	#pragma mark  						-- Member Variables --

protected:
		
			
	wxServiceDiscoveryService&			m_rService;
	wxServiceDiscoveryResultArray		m_ResultsArray;
	wxServiceDiscoveryResolveArray		m_ActiveResolvers;
	bool								m_bAutoResolve;

	DECLARE_NO_COPY_CLASS( wxServiceDiscoveryBrowser )
};




#endif // wxUSE_SERVICE_DISCOVERY

#endif // _WX_SERVICE_DISCOVERY_BROWSER_H_
