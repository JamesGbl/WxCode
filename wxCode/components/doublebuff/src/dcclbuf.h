#ifndef _WX_BUFDCCLIENT_H_BASE_
#define _WX_BUFDCCLIENT_H_BASE_

#if defined(__WXMSW__)
#include "msw/dcclbuf.h"
#elif defined(__WXMOTIF__)
#include "motif/dcclbuf.h"
#elif defined(__WXGTK__)
#include "gtk/dcclbuf.h"
#elif defined(__WXQT__)
#include "qt/dcclbuf.h"
#elif defined(__WXMAC__)
#include "mac/dcclbuf.h"
#elif defined(__WXPM__)
#include "os2/dcclbuf.h"
#elif defined(__WXSTUBS__)
#include "stubs/dcclbuf.h"
#endif

#endif
    // _WX_BUFDCCLIENT_H_BASE_
