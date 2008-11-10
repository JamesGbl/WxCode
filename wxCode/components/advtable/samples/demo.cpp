/////////////////////////////////////////////////////////////////////////////
// Name:        Demo.cpp
// Purpose:     
// Author:      Moskvichev Andrey V.
// Created:     2008/10/09
// RCS-ID:      $Id: wxAdvTable.h,v 1.3 2008/10/09 16:42:58 frm Exp $
// Copyright:   (c) 2008 Moskvichev A.V.
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "MainFrame.h"

/**
 * Demo application.
 */
class DemoApp : public wxApp
{
public:
	bool OnInit()
	{
		MainFrame *mainFrame = new MainFrame();

		mainFrame->Show(true);
		SetTopWindow(mainFrame);
		return true;
	}
};

IMPLEMENT_APP(DemoApp);
