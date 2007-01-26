//---------------------------------------------------------------------------
//
// Name:        wxCommanderApp.h
// Author:      Armando Urdiales González
// Created:     06/12/2006 12:53:20
// Description:
//
//---------------------------------------------------------------------------

#ifndef __WXCOMMANDERFRMApp_h__
#define __WXCOMMANDERFRMApp_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

class wxCommanderFrmApp : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
};

#endif
