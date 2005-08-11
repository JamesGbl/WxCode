//---------------------------------------------------------------------------
// $RCSfile: TreeMultiXmlMapper.cpp,v $
// $Source: /home/ulrich/cvs-wxcode/wxCode/components/treemultictrl/contrib/src/treemultictrl/TreeMultiXmlMapper.cpp,v $
// $Revision: 1.1 $
// $Date: 2005-08-11 20:37:33 $
//---------------------------------------------------------------------------
// Author:      Jorgen Bodde
// Copyright:   (c) Jorgen Bodde
// License:     wxWidgets License
//---------------------------------------------------------------------------

#ifdef wxUSE_TMC_XMLMAPPER

#ifdef __GNUG__
    #pragma implementation "TreeMultiXmlMapper.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "TreeMultiXmlMapper.h"

//----------------------------------------------------------------------------
// wxTreeMultiXmlMapper
//----------------------------------------------------------------------------

wxTreeMultiXmlMapper::wxTreeMultiXmlMapper(wxTreeMultiCtrl *ctrl)
	: m_ctrl(ctrl)
{
	// TODO: Implement this
}

wxTreeMultiXmlMapper::~wxTreeMultiXmlMapper()
{
	// TODO: Implement this
}

bool wxTreeMultiXmlMapper::LoadWizard(const wxString &filename, const wxString &start_tag)
{
	// TODO: Implement this
	return false;
}

bool wxTreeMultiXmlMapper::Validate(wxArrayString *items)
{
	// clear log if we have one
	if(items)
		items->Clear();

	// TODO: Validate treemulti
	return false;
}
    
bool wxTreeMultiXmlMapper::Store()
{
	// we cannot continue when validate is false
	if(!Validate())
		return false;

	return true;
}

#endif
