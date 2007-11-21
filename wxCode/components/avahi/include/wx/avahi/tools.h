/////////////////////////////////////////////////////////////////////////////
// Name:        wx/avahi/tools.h
// Purpose:     Avahi tool classes and functions.
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id $
// Copyright:   (c) 2006-2007 Kia Emilien
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef TOOLS_H_
#define TOOLS_H_

#include <wx/wx.h>
#include <wx/event.h>
#include <wx/regex.h>
#include <wx/tokenzr.h>


struct wxAvahiStringFilter
{
	/** Type of filter. */ 
	enum FilterType
	{
		filter_none = 0,
		filter_simple,
		filter_match,
		filter_regex,
		filter_advregex
	};

	wxAvahiStringFilter(){}
	virtual ~wxAvahiStringFilter(){}
	wxAvahiStringFilter(const wxString& filter, int type)
	{
		SetFilter(filter, type);
	}
	
	
	/** Set a filter. */ 
	virtual void SetFilter(const wxString& filter, int type){
		m_filtertype = type;
		switch(type)
		{
		case filter_simple:
		case filter_match:
			m_filters = wxStringTokenize(filter, wxT("|"));
			break;
		case filter_regex:
			if(!m_regexp.Compile(filter, wxRE_DEFAULT))
				m_filtertype = filter_none;
			break;
		case filter_advregex:
			if(!m_regexp.Compile(filter, wxRE_ADVANCED))
				m_filtertype = filter_none;
			break;
		}
	}
	
	/* Test if the param "pass" the filter.*/
	bool Test(const wxString& test)
	{
		int a, b;
		switch(m_filtertype)
		{
		case filter_regex:
		case filter_advregex:
			return m_regexp.Matches(test);
		case filter_match:
			for(a=0; a<(int)m_filters.GetCount(); a++)
			{
			    if(test.Matches(m_filters[a]))
			    	return true;
			}
			return false;
		case filter_simple:
			for(a=0; a<(int)m_filters.GetCount(); a++)
			{
			    if(test == m_filters[a])
			    	return true;
			}
			return false;;
		case filter_none:
			return true;
		default:
			return false;
		}
	}
	
protected:
	wxArrayString m_filters;
	wxRegEx       m_regexp;
	int m_filtertype;	
};


#endif /*TOOLS_H_*/
