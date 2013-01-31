/***************************************************************
 * Name:      stringformatterapp.h
 * Purpose:   Defines Application Class
 * Author:    Nathan Shaffer
 * Created:   2013-01-27
 * Copyright: Nathan Shaffer
 * License:   wxWindows licence
 **************************************************************/

#ifndef STRINGFORMATTERAPP_H
#define STRINGFORMATTERAPP_H

#include <wx/app.h>
#include "stringformatterframe.h"

class stringformatterApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // STRINGFORMATTERAPP_H
