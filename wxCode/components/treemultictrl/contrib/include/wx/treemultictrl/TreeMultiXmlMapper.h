//---------------------------------------------------------------------------
// $RCSfile: TreeMultiXmlMapper.h,v $
// $Source: /home/ulrich/cvs-wxcode/wxCode/components/treemultictrl/contrib/include/wx/treemultictrl/TreeMultiXmlMapper.h,v $
// $Revision: 1.2 $
// $Date: 2005-08-16 19:42:18 $
//---------------------------------------------------------------------------
// Author:      Jorgen Bodde
// Copyright:   (c) Jorgen Bodde
// License:     wxWidgets License
//---------------------------------------------------------------------------

#ifndef __TREEMULTI_XML_MAPPER_H__
#define __TREEMULTI_XML_MAPPER_H__

// just to make sure no unwanted errors occur
#ifdef wxUSE_TMC_XMLMAPPER

// -- atributes
// attribute declaration for name="something"
#define XMLMAP_ATTR_NAME    "name"
// the default declaration
#define XMLMAP_ATTR_DEFAULT "default"
// attribute declaration for caption="something"
#define XMLMAP_ATTR_CAPTION "caption"

// -- tags 
// the <category> tag
#define XMLMAP_TAG_CATEGORY "category"
// the <check> tag
#define XMLMAP_TAG_CHECKBOX "check"
// the <row> tag to encapsulate elements 
#define XMLMAP_TAG_ROW "row"
// the <button> element
#define XMLMAP_TAG_BUTTON "button"
// the base ID for all elements to be created
#define XMLMAP_BASE_ID      10000

#ifdef __GNUG__
    #pragma interface "TreeMultiXmlMapper.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/filename.h>

#include "wxTreeMultiCtrl.h"
#include "tinyxml.h"

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
    
	/** Initializes the wxTreeMultiCtrl. The XML contents is parsed from the string, and 
        when parsed succesfully, the tag pointed out by start_tag will be searched for.
        All data between the tag will be presented in the wxTreeMultiCtrl. All controls
        and behaviour will be created for you and owned by the wxTreeMultiCtrl */
	bool InitWizard(const wxString &xmlstring, const wxString &start_tag);

	/** Initializes the wxTreeMultiCtrl. The XML contents is read from the file. See, and 
        when parsed succesfully, the tag pointed out by start_tag will be searched for.
        All data between the tag will be presented in the wxTreeMultiCtrl. All controls
        and behaviour will be created for you and owned by the wxTreeMultiCtrl */
    bool InitWizard(const wxFileName &xmlfile, const wxString &start_tag);    

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
    // Initializes the XML document
    void InitXML();

    // The real functionality of displaying all data
    bool DoInitWizard(const wxString &start_tag);

    // Create controls inside the category group, or create more categories
    bool DoCreateControls(TiXmlElement *cat, wxTreeMultiItem &pitem, wxWindow *pwnd, wxSizer *szr);

    // Add a category element to the wxTreeMultiCtrl
    wxTreeMultiItem AddCategory(TiXmlElement *cat, wxTreeMultiItem &parent);

    // Add a checkbox element to the parent (could be other then wxTMC) 
    wxWindow *AddCheckBox(TiXmlElement *cb, wxWindow *parent);

    // Add a button element to the parent (could be other then wxTMC) 
    wxWindow *AddButton(TiXmlElement *cb, wxWindow *parent);

    // returns 0 when false, 1 when true, and -1 when error. Translates the 
    // most common strings for false: 0, off, no, false and the opposites for
    // true.
    int GetDefaultBoolAttribute(TiXmlElement *el);

    // helper function to get caption string. Gives error when allowEmpty is 
    // true when attribute does not exist.
    wxString GetCaptionAttribute(TiXmlElement *el, bool allowEmpty = false);

private:
	// The wxTreeMultiCtrl that will be filled with XML data
    wxTreeMultiCtrl *m_ctrl;
    // The tinyXML document
    TiXmlDocument *m_tiDoc;
    // The ID counter for the controls
    int m_currentId;
};

#endif 	// #ifdef wxUSE_TMC_XMLMAPPER

#endif
