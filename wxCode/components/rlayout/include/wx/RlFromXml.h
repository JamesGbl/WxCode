//
// Copyright (C) 2007 Arne Steinarson <arst at users dot sourceforge dot net>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must
//    not claim that you wrote the original software. If you use this
//    software in a product, an acknowledgment in the product
//    documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must
//    not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source
//    distribution.
//


#ifndef WXRLFROMXML_H
#define WXRLFROMXML_H

#include "wx/rlayoutdef.h"

class wxRlItem;
class wxWindow;

// Callback to to handle custom creation of wxRlItems
//typedef wxRlItem* (*RlItemFromXmlFn)( const char *xml, wxWindow *prnt_wnd, wxRlItem *prnt_rli );


// Create a layout from XML nodes
#define RL_XMLLOAD_IS_DIALOG    1
#define RL_XMLLOAD_NO_USERDATA  2
#define RL_XMLLOAD_SKELETON     4  // Keep going even when encountering empty windows (without classes)

WXDLLIMPEXP_RLAYOUT wxRlItem *wxRlXmlLoad( const char*& xml, wxWindow *prnt_wnd, wxRlItem *prnt_rli, int opts );


// Return position in XML where last error / warning was.
WXDLLIMPEXP_RLAYOUT const char *wxRlXmlGetLast_ErrorXml();
WXDLLIMPEXP_RLAYOUT const char *wxRlXmlGetLast_WarningXml();
WXDLLIMPEXP_RLAYOUT const char *wxRlXmlGetLast_Msg();


#endif // WXRLFROMXML_H

