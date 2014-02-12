/***************************************************************
 * Name:      stringformatterapp.cpp
 * Purpose:   Code for Application Class
 * Author:    Nathan Shaffer
 * Created:   2013-01-27
 * Copyright: Nathan Shaffer
 * License:   wxWindows licence
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "stringformatterapp.h"
#include <wx/log.h>

IMPLEMENT_APP(stringformatterApp);

bool stringformatterApp::OnInit()
{

    StringFormatterFrame* frm = new StringFormatterFrame(NULL);

    frm->Show();
    return true;
}
