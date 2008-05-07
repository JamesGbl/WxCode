// --
// --	servicediscoverycontrolwithitemst.cpp
// --	Licenser
// --
// --
// --	revision   	$Rev$
// --	changed by	$Author$
// --	modified	$Date$
// --
// --	Created by William H. Schultz on 5/6/08.
// --	Copyright © 2008 Cedrus Corporation. All rights reserved.
// --
// --


#include <wx/wxprec.h>
#include <wx/listbox.h>

#include "wx/servicediscoverycontrolwithitemst.hpp"
#include "wx/servicediscoveryservice.h"

#if wxUSE_SERVICE_DISCOVERY


template<>
bool wxServiceDiscoveryControlWithItems<wxListBox>::TransferDataFromWindow()
{
	bool status = wxListBox::TransferDataFromWindow();
	
	
	if ( status )
	{
		m_ResultsArray.Empty();

		wxArrayInt selections;
		int num_selections = GetSelections( selections );
		
		status = false;
		
		for( int i = 0; i < num_selections; i++ )
		{
			int selection = selections[i];
			
			m_ResultsArray.Add( *( static_cast<wxServiceDiscoveryControlClientData*>( GetClientObject( selection ) )->GetBonjourEvent() ) );
			
			status = true;
		}		
	}


	return status;
}


#endif // wxUSE_SERVICE_DISCOVERY

