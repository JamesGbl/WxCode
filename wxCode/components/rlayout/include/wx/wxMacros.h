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


#ifndef WX_MACROS_H
#define WX_MACROS_H

// Only header file stuff, requires no cpp file
#include <wx/filename.h>
#define wxPSEP (wxFileName::GetPathSeparator())

// Convenience macros for wx flags
#define wxBORDER_MASK (wxBORDER_DOUBLE|wxSUNKEN_BORDER|wxRAISED_BORDER|wxSIMPLE_BORDER|wxSTATIC_BORDER|wxTOP|wxLEFT|wxBOTTOM|wxRIGHT)
#define wxALL_DIR     (wxTOP|wxLEFT|wxBOTTOM|wxRIGHT)
#define wxALIGN_MASK  (wxCENTER|wxALIGN_CENTER_HORIZONTAL|wxALIGN_RIGHT|wxALIGN_BOTTOM|wxALIGN_CENTER_VERTICAL)

// Convenience macros for event handlers  and connect
#ifndef wxEvtFnCast
	typedef void (wxEvtHandler::*wxKeyEventFunction)(wxKeyEvent&);
	#define wxEvtFnCast(EvType) (wxObjectEventFunction)(wx##EvType##EventFunction)
#endif


#endif // WX_MACROS_H

