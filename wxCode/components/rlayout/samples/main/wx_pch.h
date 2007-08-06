#ifndef WX_PCH_H_INCLUDED
#define WX_PCH_H_INCLUDED

#if ( defined(USE_PCH) && !defined(WX_PRECOMP) )
    #define WX_PRECOMP
#endif // USE_PCH

// basic wxWidgets headers
#include <wx/wxprec.h>

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#ifdef USE_PCH
	// put here all your rarely-changing header files
	// #include "wx/aui/aui.h"
    #include "wx/notebook.h"
    #include "wx/listctrl.h"
    #include "wx/filename.h"
    #include "wx/xrc/xmlres.h"
    #include "wx/clipbrd.h"
    #include "wx/dir.h"    
    #include "wx/regex.h"

    /*    
    // wxStEditor
    #include "wx/stedit/stedefs.h"
    #include "wx/stedit/steprefs.h"
    #include "wx/stedit/stestyls.h"
    #include "wx/stedit/stelangs.h"
    #include "wx/stedit/steopts.h"
    #include "wx/stedit/stemenum.h"
    #include "wx/stedit/stedlgs.h"
    #include "wx/stedit/steframe.h"
    #include "wx/stedit/stefindr.h"


    // wxFlatNotebook
    #include "wx/wxFlatNotebook/wxFlatNotebook.h"

    // wx files used by wxStEditor
    #include "wx/fdrepdlg.h"
    #include "wx/treebase.h"
    #include "wx/treectrl.h"
    #include "wx/prntbase.h"
    #include "wx/artprov.h"
    #include "wx/splitter.h"
    #include "wx/generic/splitter.h"
    */
        
    #include "stdio.h"
#endif // USE_PCH

#endif // WX_PCH_H_INCLUDED
