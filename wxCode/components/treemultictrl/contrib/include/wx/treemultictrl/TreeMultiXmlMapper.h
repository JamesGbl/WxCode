//---------------------------------------------------------------------------
// $RCSfile: TreeMultiXmlMapper.h,v $
// $Source: /home/ulrich/cvs-wxcode/wxCode/components/treemultictrl/contrib/include/wx/treemultictrl/TreeMultiXmlMapper.h,v $
// $Revision: 1.1 $
// $Date: 2005-08-11 20:37:34 $
//---------------------------------------------------------------------------
// Author:      Jorgen Bodde
// Copyright:   (c) Jorgen Bodde
// License:     wxWidgets License
//---------------------------------------------------------------------------

#ifndef __TREEMULTI_XML_MAPPER_H__
#define __TREEMULTI_XML_MAPPER_H__

#ifdef wxUSE_TMC_XMLMAPPER

#ifdef __GNUG__
    #pragma interface "TreeMultiXmlMapper.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wxTreeMultiCtrl.h"

/** \class wxTreeMultiXmlMapper
    \ingroup classes
    \brief This class can map an XML file containing a wizard section to the wxTreeMultiCtrl..

	The mapper class makes it possible to load an XML file and map the wizard section to a wxTreeMultiCtrl. The mapper class
	will take care of creating the corresponding elements and initialize them with default values where applicable. In the 
	future the XML mapper will also take care of visual effects where e.g. controls are shown or hidden when a checkbox 
	or radio button is selected that corresponds for example with advanced values. 
	
	The mapper class will (also in the future) take care of dependencies and restrictions that are set in the wizard for 
	example a verify function will check if an edit ctrl is empty or not, or dynamically set more options, or clear others
	when there are conflicts in the dependencies. 
*/

class wxTreeMultiXmlMapper
{
public:
    /** The constructor of the class. Pass the pointer to the wxTreeMultiCtrl control that will be filled with
        the XML data.  */
    wxTreeMultiXmlMapper(wxTreeMultiCtrl *ctrl);

    /** Destructor */
    ~wxTreeMultiXmlMapper();
    
	/** Initializes the wxTreeMultiCtrl with the wizard section pointed out with start_tag. All contents 
	    between these tags is presented in the wxTreeMultiCtrl. If a problem occured, false is returned
	    and the wxLogError mechanism is used to present all the problems to the user. */
	bool LoadWizard(const wxString &filename, const wxString &start_tag);    
    
    /** Function that will validate if the data entered is valid. This is done by examining the rules in the 
        XML document object model, and acting upon it. Returned is a wxArrayString with observations of
        issues that are wrong, and can be reported to the user. If false is returned the validation is failed,
        and the function Store() will fail. 
        The wxArrayString is optional, if not present (pointer is 0) then no log data is returned, only false 
    */
    bool Validate(wxArrayString *items = 0);
    
    /** This function stores the contents of the wxTreeMultiCtrl back to the XML document object model, for 
        further processing, e.g. template generation or other actions that need to be performed. When false
        is returned the Store failed, this happens when Validate() fails */
    bool Store();
    
private:
	wxTreeMultiCtrl *m_ctrl;
};

#endif 	// #ifdef wxUSE_TMC_XMLMAPPER

#endif
