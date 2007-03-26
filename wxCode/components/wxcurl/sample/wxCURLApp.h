/*
 *	wxCURLApp.h
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Mon May 24 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

#ifndef _WXCURLAPP_H__INCLUDED_
#define _WXCURLAPP_H__INCLUDED_

// wxCURLApp.h: interface for the wxCURLApp class.
//
//////////////////////////////////////////////////////////////////////

#include <wx/app.h>

class wxCURLApp : public wxApp
{
public:
	wxCURLApp();
	virtual ~wxCURLApp();

	// override base class virtuals
	// ----------------------------

	// this one is called on application startup and is a good place for the app
	// initialization (doing it here and not in the ctor allows to have an error
	// return: if OnInit() returns false, the application terminates)
	virtual bool OnInit();
};

DECLARE_APP(wxCURLApp)

#endif // _WXCURLAPP_H__INCLUDED_


