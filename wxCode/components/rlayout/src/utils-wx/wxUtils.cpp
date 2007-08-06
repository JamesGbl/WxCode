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


#include <wx/file.h>

#include "wx/wxUtils.h"
#include "wx/strconv.h"

#include "utils/types.h"


bool _is7bit( const char *ps ){
	if( !ps ) return true;
	while( *ps && !((*ps)&0x80) )
		ps++;
	return *ps ? false : true;
}

bool _is7bit( const wchar_t *pws ){
	if( !pws ) return true;
	while( *pws && !((*pws)&(~0x7F)) )
		pws++;
	return *pws ? false : true;
}


// Convert to local charset
wxCharBuffer _WC2AC( const wchar_t *pws ){
	//printf("%s\n",__func__);
	return wxConvLocal.cWC2MB(pws);
}

wxCharBuffer _U82AC( const char *pu8s ){
	//printf("%s\n",__func__);
	// Have to use intermediate step
	wxWCharBuffer tmp = wxConvUTF8.cMB2WC(pu8s);
	return wxConvLocal.cWC2MB(tmp.data());
}


// Convert to local UTF8
wxCharBuffer _WC2U8( const wchar_t *pws ){
	//printf("%s\n",__func__);
	return wxConvUTF8.cWC2MB(pws);
}

wxCharBuffer _AC2U8( const char *ps ){
	//printf("%s\n",__func__);
	// Have to use intermediate step
	wxWCharBuffer tmp = wxConvLocal.cMB2WC(ps);
	return wxConvUTF8.cWC2MB(tmp.data());
}



wxWCharBuffer _U82WC( const char *pu8s ){
	//printf("%s\n",__func__);
	return wxConvUTF8.cMB2WC(pu8s);
}

wxWCharBuffer _AC2WC( const char *ps ){
	//printf("%s\n",__func__);
	return wxConvLocal.cMB2WC(ps);
}



/*
// Conversion from Multibyte char array to wxString.
// On stack, thread-safe.
wxString MB2WX( const char *pmbs ){
#if wxUSE_UNICODE==1
	return wxString( pmbs, wxConvUTF8 );
#else
	// First convert to wide char array
    wxWCharBuffer wcb;
    int mbsl = strlen(pmbs) + 1;
    // wcb.SetBufSize( mbsl ); 	// 2.8.4 
    wcb.extend( mbsl );			// 2.9.0 
	wxConvUTF8.MB2WC( wcb.data(), pmbs, mbsl );

	// Now truncate down to Ansi range, and subst to '?' when outside range
	wxCharBuffer cb;
	int sl = wcslen(wcb);
	cb.extend(sl+1); // SetBufSize(sl+1);
	wchar_t wch;
	for( int ix=0; ix<sl; ix++ ){
		wch = wcb.data()[ix];
		cb.data()[ix] = char( wch>255 ? '?' : wch );
	}
	cb.data()[sl] = 0;
	
	return wxString(cb);
#endif
}
*/


/*
#define DEFINE_WCHAR_T_AS_INT
#include "utils/unicode.h"
#include "utils/unicode-fd32.h"

wxString MB2WX( const char *pmbs ){
	int ul, wc, wc1, ul1;
	wxString s;
	wxChar wxch;
	bool did_notify = false;
	while( *pmbs ){
  		unicode_utf8_to_wchar(&wc,pmbs,4);
  		ul = unicode_utf8_len(*pmbs);
  		unicode_utf8_to_wchar_(&wc1,pmbs);
  		ul1 = unicode_utf8_len_(*pmbs);
  		wxASSERT( ul==ul1 && wc==wc1 );
  		if( ul<1 || ul>6 ){
  			wxLogMessage( _T("Invalid character in wxUtils:MB2WX") );
  			return wxString();
  		}
  		pmbs += ul;
  		wxch = (wxChar)wc;	// Cast to wxChar range
  		s += wxch;
  		if( (int)wxch!=wc && !did_notify ){	// Cast back to int and see if same
  			wxLogMessage( _T("Character out of wxChar range in wxUtils:MB2WX") );
  			did_notify = true;
  		}
	}
	return s;
}

// Conversion from wxChar array to wxString.
#if wxUSE_UNICODE==1
wxCharBuffer WX2MB( const wxChar *wxs ){
	return wxString(wxs).mb_str(wxConvUTF8);
}
#endif
*/


void ConvTest(){
	// With 8 bit we must do all conversions
	char *ps_8bit = "123���-��-^�~";
	// With 7 bit, we should be able to skip many conversions
	char *ps_7bit = "123!#$,.-";
	char *pps[] = { ps_7bit, ps_8bit };
	for( int ix=0; ix<2; ix++ ){
		printf( "\n%s iteration:\n================\n", ix?"8bit":"7bit" );
		char *ps = pps[ix];
		
		printf( "Raw-%s  is7bit-%d \n", ps, _is7bit(ps) );
		
		printf( "AC->UTF8->AC: %s %d(0)\n", U82AC(AC2U8(ps)), strcmp(ps,U82AC(AC2U8(ps))) );
		printf( "AC->WC->AC: %s %d(0)\n", WC2AC(AC2WC(ps)), strcmp(ps,WC2AC(AC2WC(ps))) );
		
		printf( "AC->UTF8->WC->AC: %s %d(0)\n", WC2AC(U82WC(AC2U8(ps))), strcmp(ps,WC2AC(U82WC(AC2U8(ps)))) );
		printf( "AC->WC->UTF8->AC: %s %d(0)\n", U82AC(WC2U8(AC2WC(ps))), strcmp(ps,U82AC(WC2U8(AC2WC(ps)))) );

		
		wxString s(ps,wxConvLocal);
		const wchar_t *pws = wxConvCurrent->cWX2WC(s); 
		
		printf( "WC->WX->AC->WX->AC: %s %d(1)\n", WX2AC(AC2WX(WX2AC(WC2WX(pws)))), s==AC2WX(WX2AC(WC2WX(pws))) );
		printf( "WC->UTF8->WX->AC: %s %d(1)\n", WX2AC(U82WX(WC2U8(pws))), s==U82WX(WC2U8(pws)) );
	}
}






bool wxSaveString( const wxString& str, const wxChar *path ){
	wxFile f( path, wxFile::write );
	if( !f.IsOpened() ) return false;
	if( !f.Write( str ) ) return false;
	return true;
}

bool wxLoadString( wxString& str, const wxChar *path ){
	str.Empty();
	wxFile f( path, wxFile::read );
	if( !f.IsOpened() ) return false;
	char buf[257];
	size_t st;
	while( (st=f.Read(buf,255))>0 ){
		buf[st] = 0;
		str += (const wxChar*)wxConvUTF8.cMB2WX(buf);
	}
	
	return true;
}

const wxChar *g_wx_flag2str[] = {
	_T("wxCENTRE"), _T("wxCNTR"),
	_T("wxCENTRE_ON_SCREEN"), _T("wxCOSCR"),
	_T("wxHORIZONTAL"), _T("wxHORZ"),
	_T("wxVERTICAL"), _T("wxVERT"),

	_T("wxLEFT"),	_T("wxLFT"),
	_T("wxRIGHT"), _T("wxRGT"),
	_T("wxUP"), _T("wxUP"),
	_T("wxDOWN"), _T("wxDWN"),

	_T("wxALIGN_CENTER_HORIZONTAL"), _T("wxAL_CH"),
	_T("wxALIGN_RIGHT"), _T("wxAL_R"),
	_T("wxALIGN_BOTTOM"), _T("wxAL_B"),
	_T("wxALIGN_CENTER_VERTICAL"), _T("wxAL_CV"),

	_T("wxSHRINK"), _T("wxSHR"),
	_T("wxEXPAND"), _T("wxEXP"),
	_T("wxSHAPED"), _T("wxSHP"),
	_T("wxFIXED_MINSIZE"), _T("wxFMS"),

	_T("wxFULL_REPAINT_ON_RESIZE"), _T("wxREP_ON_RSZ"),
	_T("wxPOPUP_WINDOW"), _T("wxPOP_WND"),
	_T("wxWANTS_CHARS"), _T("wxWNT_CHR"),
	_T("wxTAB_TRAVERSAL"), _T("wxTAB_TRAV"),

	_T("wxTRANSPARENT_WINDOW"), _T("wxTRANSP"),
	_T("wxBORDER_NONE"),	_T("wxBO_NO"),
	_T("wxCLIP_CHILDREN "), _T("wxCLI_CHI"),
	_T("wxALWAYS_SHOW_SB"), _T("wxALW_SB"),

	_T("wxBORDER_STATIC"), _T("wxBO_STAT"),
	_T("wxBORDER_SIMPLE"), _T("wxBO_SIMP"),
	_T("wxBORDER_RAISED"), _T("wxBO_RAIS"),
	_T("wxBORDER_SUNKEN"), _T("wxBO_SUNK"),

	_T("wxBORDER_DOUBLE"), _T("wxBO_DBL"),
	_T("wxCAPTION"), _T("wxCAP"),
	_T("wxHSCROLL"), _T("wxHSCR"),
	_T("wxVSCROLL"), _T("wxVSCR")
};


const wxChar *wxFlag2Str( int flag, bool brief ){
    static wxString s;
    int n_found = 0;
    s = _T("");
    for( int i=0; i<32; i++ ){
        if( flag&(1<<i) ){
            if( n_found++ ) s += _T("|");
            s += g_wx_flag2str[i*2+(int)brief];
        }
    }
    return s;
}

wxString KeyCode2Str( int qual, int kc ){
	wxString s;
	
	if( qual&(wxMOD_META|wxMOD_CONTROL) ) s += _T("Ctrl+");
	if( qual&(wxMOD_SHIFT) ) s += _T("Shift+");
	if( qual&(wxMOD_ALT) ) s += _T("Alt+");
	if( !kc ) return s;

	switch( kc ){
	case WXK_BACK: s += _T("Back"); break;
	case WXK_TAB: s += _T("Tab"); break;
	case WXK_RETURN: s += _T("Enter"); break;
	case WXK_ESCAPE: s += _T("Escape"); break;
	case WXK_SPACE: s += _T("Space"); break;
	case WXK_DELETE: s += _T("Delete"); break;
	case WXK_START: s += _T("Start"); break;
	case WXK_CANCEL: s += _T("Cancel"); break;
	case WXK_PAUSE: s += _T("Pause"); break;
	case WXK_END: s += _T("End"); break;
	case WXK_HOME: s += _T("Home"); break;
	case WXK_LEFT: s += _T("Left"); break;
	case WXK_RIGHT: s += _T("Right"); break;
	case WXK_UP: s += _T("Up"); break;
	case WXK_DOWN: s += _T("Down"); break;
	case WXK_SELECT: s += _T("Select"); break;
	case WXK_PRINT: s += _T("Print"); break;
	case WXK_SNAPSHOT: s += _T("Snapshot"); break;
	case WXK_INSERT: s += _T("Insert"); break;
	case WXK_HELP: s += _T("Help"); break;
	//case WXK_MULTPLY: s += _T("Multiply"); break;
	case WXK_ADD: s += _T("Add"); break;
	case WXK_SUBTRACT: s += _T("Subtract"); break;
	case WXK_DECIMAL: s += _T("Decimal"); break;
	case WXK_DIVIDE: s += _T("Divide"); break;
	
	case WXK_NUMLOCK: s += _T("Numlock"); break;
	case WXK_SCROLL: s += _T("Scroll"); break;
	case WXK_PAGEUP: s += _T("PageUp"); break;
	case WXK_PAGEDOWN: s += _T("PageDown"); break;
	
	case WXK_NUMPAD_SPACE: s += _T("Num-Space"); break;
	case WXK_NUMPAD_TAB: s += _T("Num-Tab"); break;
	case WXK_NUMPAD_ENTER: s += _T("Num-Enter"); break;
	case WXK_NUMPAD_F1: s += _T("Num-F1"); break;
	case WXK_NUMPAD_F2: s += _T("Num-F2"); break;
	case WXK_NUMPAD_F3: s += _T("Num-F3"); break;
	case WXK_NUMPAD_F4: s += _T("Num-F4"); break;
	case WXK_NUMPAD_HOME: s += _T("Num-Home"); break;
	case WXK_NUMPAD_LEFT: s += _T("Num-Left"); break;
	case WXK_NUMPAD_UP: s += _T("Num-Up"); break;
	case WXK_NUMPAD_RIGHT: s += _T("Num-Right"); break;
	case WXK_NUMPAD_DOWN: s += _T("Num-Down"); break;
	case WXK_NUMPAD_PAGEUP: s += _T("Num-PageUp"); break;
	case WXK_NUMPAD_PAGEDOWN: s += _T("Num-PageDown"); break;
	case WXK_NUMPAD_END: s += _T("Num-End"); break;
	case WXK_NUMPAD_BEGIN: s += _T("Num-Begin"); break;
	case WXK_NUMPAD_INSERT: s += _T("Num-Ins"); break;
	case WXK_NUMPAD_DELETE: s += _T("Num-Del"); break;
	case WXK_NUMPAD_EQUAL: s += _T("Num-Equal"); break;
	case WXK_NUMPAD_MULTIPLY: s += _T("Num-Mul"); break;
	case WXK_NUMPAD_ADD: s += _T("Num-Add"); break;
	case WXK_NUMPAD_SEPARATOR: s += _T("Num-Sep"); break;
	case WXK_NUMPAD_SUBTRACT: s += _T("Num-Sub"); break;
	case WXK_NUMPAD_DECIMAL: s += _T("Num-Dec"); break;
	case WXK_NUMPAD_DIVIDE: s += _T("Num-Div"); break;
	
	case WXK_WINDOWS_LEFT: s += _T("Win-Left"); break;
	case WXK_WINDOWS_RIGHT: s += _T("Win-Right"); break;
	case WXK_WINDOWS_MENU: s += _T("Win-Menu"); break;
	case WXK_COMMAND: s += _T("Command"); break;
	
	default:
		if( kc>=WXK_F1 && kc<=WXK_F24 )
			s += wxString::Format(wxT("F%d"),kc-WXK_F1+1);
		else if( kc>' ' && kc<WXK_DELETE )
			s += (wxChar)kc;
		else 
			s += wxString::Format(wxT("Code:%d ('%c')"),kc,kc);
	}
	
	return s;
}

bool wxLLGetBit( const wxLongLong &ll, int bit ){
	if( bit>31 )
		return (ll.GetHi()&(1<<(bit-32)))!=0;
	else
		return (ll.GetLo()&(1<<bit))!=0;
}

void wxLLSetBit( wxLongLong &ll, int bit ){
	DWORD lo = ll.GetLo();
	DWORD hi = ll.GetHi();
	if( bit<32 ) lo |= 1<<bit;
	else hi |= 1<<(bit-32);
	ll = wxLongLong(hi,lo);
}

void wxLLClearBit( wxLongLong &ll, int bit ){
	DWORD lo = ll.GetLo();
	DWORD hi = ll.GetHi();
	if( bit<32 ) lo &= ~(1<<bit);
	else hi &= ~(1<<(bit-32));
	ll = wxLongLong(hi,lo);
}

/*
int wxLLNextBit( const wxLongLong &ll, bool set, int from_pos ){
	DWORD lo = o1.GetLo();
	while( from_pos<32 ){
		if( (set && lo&(1<<from_pos)) || (!set && !(lo&(1<<from_pos))) )
			return from_pos;
		from_pos++;
	}
	
	DWORD hi = o1.GetHi();
	from_pos -= 32;
	while( from_pos<32 ){
		if( (set && hi&(1<<from_pos)) || (!set && !(hi&(1<<from_pos))) )
			return from_pos+32;
		from_pos++;
	}
	return -1;
}
*/

#if wxUSE_LONGLONG_NATIVE==0
wxLongLong operator | ( const wxLongLong &o1, const wxLongLong &o2 ){
	DWORD lo = o1.GetLo() | o2.GetLo();
	DWORD hi = o1.GetHi() | o2.GetHi();
	return wxLongLong(hi,lo);
}

wxLongLong operator & ( const wxLongLong &o1, const wxLongLong &o2 ){
	DWORD lo = o1.GetLo() & o2.GetLo();
	DWORD hi = o1.GetHi() & o2.GetHi();
	return wxLongLong(hi,lo);
}

wxLongLong operator ^ ( const wxLongLong &o1, const wxLongLong &o2 ){
	DWORD lo = o1.GetLo() ^ o2.GetLo();
	DWORD hi = o1.GetHi() ^ o2.GetHi();
	return wxLongLong(hi,lo);
}
#endif //wxUSE_LONGLONG_NATIVE==0

wxLongLong operator ~ ( const wxLongLong &o1 ){
	DWORD lo = ~o1.GetLo();
	DWORD hi = ~o1.GetHi();
	return wxLongLong(hi,lo);
}

int GetCoord(const wxPoint &pt, int ix){
	wxASSERT( ix>=0 && ix<2 );
	return !ix ? pt.x : pt.y;
}

int GetCoord(const wxSize &sz, int ix){
	wxASSERT( ix>=0 && ix<2 );
	return !ix ? sz.GetWidth() : sz.GetHeight();
}

// In wxw<2.9.0 there is no way to check if there are unprocessed
// events (dangerous). 
bool wxProcessEventsUntilEmpty( ){
	bool hp = false;
#if wxCHECK_VERSION(2, 9, 0)
	hp = wxTheApp->HasPendingEvents();
#endif
	bool p = wxTheApp->Pending();
	if( !hp && !p ) 
		return true;
	
	wxTheApp->ProcessPendingEvents();
    while( wxTheApp->Pending() )
        wxTheApp->Dispatch();
	wxTheApp->ProcessPendingEvents();
	
#if wxCHECK_VERSION(2, 9, 0)
	hp = wxTheApp->HasPendingEvents();
#endif
	p = wxTheApp->Pending();
	
	return !hp && !p;
}


void wxInsertEventHandlerBefore(wxEvtHandler *_this, wxEvtHandler *handler)
{
    _this->SetNextHandler( handler );
    _this->SetPreviousHandler( handler->GetPreviousHandler() );
    if( handler->GetPreviousHandler() )
        handler->GetPreviousHandler()->SetNextHandler(_this);
    handler->SetPreviousHandler(_this);
}

void wxRemoveEventHandlerBefore(wxEvtHandler *_this, wxEvtHandler *handler )
{
    wxASSERT( handler == _this->GetNextHandler() );
    handler->SetPreviousHandler( _this->GetPreviousHandler() );
    if( _this->GetPreviousHandler() )
        _this->GetPreviousHandler()->SetNextHandler(handler);
    _this->SetPreviousHandler(NULL);
    _this->SetNextHandler(NULL);
}

void wxInsertEventHandlerAfter(wxEvtHandler *_this, wxEvtHandler *handler)
{
    _this->SetPreviousHandler( handler );
    _this->SetNextHandler( handler->GetNextHandler() );
    if( handler->GetNextHandler() )
        handler->GetNextHandler()->SetPreviousHandler(_this);
    handler->SetNextHandler(_this);
}

void wxRemoveEventHandlerAfter(wxEvtHandler *_this, wxEvtHandler *handler )
{
    wxASSERT( handler == _this->GetPreviousHandler() );
    handler->SetNextHandler( _this->GetNextHandler() );
    if( _this->GetNextHandler() )
        _this->GetNextHandler()->SetPreviousHandler(handler);
    _this->SetNextHandler(NULL);
    _this->SetPreviousHandler(NULL);
}


wxString DcChopText( wxDC *dc, const wxChar *str, int w, int *ph ){
    int mw, mh;
    wxString s(str);
    while( 1 ){
        dc->GetTextExtent(s, &mw, ph?ph:&mh);
        if( mw<w || !s.Length() ) break;
        s.RemoveLast();
    }
    return s;
}

// wxAuiBitmapFromBits() is a utility function that creates a
// masked bitmap from raw bits (XBM format)
wxBitmap wxAuiBitmapFromBits(const unsigned char bits[], int w, int h,
                             const wxColour& color_bg, const wxColour &color_fg,
                             bool use_mask )
{
    wxBitmap bm( (const char*)bits, w, h);
    wxImage img = bm.ConvertToImage();
    img.Replace( 0,0,0, color_fg.Red()^0xc7, 0, 0 );
    img.Replace( 255,255,255, color_fg.Red(),color_fg.Green(),color_fg.Blue() );
    img.Replace( color_fg.Red()^0xc7,0,0, color_bg.Red(),color_bg.Green(),color_bg.Blue() );
    if( use_mask )
        img.SetMaskColour(color_bg.Red(),color_bg.Green(),color_bg.Blue());
    bm = wxBitmap(img);
    return bm;
}

int BitmapTypeFromExt( const wxString name ){
	int pdot = name.Find( _T('.'), true );
	if( pdot==wxNOT_FOUND )
		return wxBITMAP_TYPE_PNG;
	
	wxString ext = name.Mid( pdot+1 ).Upper();
	if( ext==_T("BMP") )
		return wxBITMAP_TYPE_BMP;
	if( ext==_T("XPM") )
		return wxBITMAP_TYPE_XPM;
	if( ext==_T("GIF") )
		return wxBITMAP_TYPE_GIF;
	return wxBITMAP_TYPE_PNG;
}

wxWindow *FindChildWindowAt( wxWindow *wnd, const wxPoint &pt, bool full_rec ){
	if( !wnd ) return NULL;
	wxWindowList::compatibility_iterator pnode = wnd->GetChildren().GetFirst(); 
	while( pnode ){
		wxWindow *pw = pnode->GetData();
		if( pw ){
			wxRect r = pw->GetRect();
			if( r.Contains(pt) ){
				if( !full_rec ) return pw;
				//wxPoint ptc = pw->ScreenToClient( wnd->ClientToScreen(pt) );
				wxPoint ptc(pt.x-r.x, pt.y-r.y);
				return FindChildWindowAt(pw,ptc,true);
			}
		}
		pnode = pnode->GetNext();
	}
	return wnd;
}


wxColour g_wxu_fc_col(9, 199, 99);
wxColour g_wxu_caption_col(9, 199, 99);

bool GetFrameColour( wxColour &bgc ){
	if( g_wxu_fc_col==wxColour(9,199,99) ) return false;
	bgc = g_wxu_fc_col;
	return true;
}

// get active caption colour
bool GetCaptionColour( wxColour &cc ){
	if( g_wxu_caption_col==wxColour(9,199,99) ){
		if( !SetCaptionColourFrom( NULL, NULL ) ) return false;
	}
		
	cc = g_wxu_caption_col;
	return true;
}


bool SetFrameColourFrom( wxFrame *wnd ){
	if( !wnd ) return false;
	wxRect rs = wnd->GetScreenRect();
	wxRect rc = wnd->GetClientRect();
	int nch = rs.height-rc.height;	// Non client height inside window
	if( nch<10 ) return false;
	
	// This is shaky on wxGTK. GetMetrics seem to return 0 values for whatever queried
	// Frame border size
	int fsy = wxSystemSettings::GetMetric(wxSYS_FRAMESIZE_Y,wnd);
	
	int x,y;
	if( wnd->GetStatusBar() && fsy>0 && fsy<10 ){
		// look at statusbar pixel
		x = rs.x+rs.width/4 + 5;
		y = rs.y+rs.height-fsy-2;
	}
	else{
		// Look where menu bar usually is
		x = rs.x + rs.width/2 + 7;	
		y = rs.y + nch*3/4;
	}
	
	wxScreenDC dc;
	if( !dc.GetPixel(x,y, &g_wxu_fc_col) ) return false;
	int cr = g_wxu_fc_col.Red();
	int cg = g_wxu_fc_col.Green();
	int cb = g_wxu_fc_col.Blue();
	return true;
}

bool SetFrameColourFrom( wxToolBar *ptb ){
	if( !ptb ) return false;
	wxScreenDC dc;
	// Since we have the DC, try do the caption colour as well
	SetCaptionColourFrom( NULL, &dc );
	
	wxRect r = ptb->GetScreenRect();
	wxColour cl[3];	// Samples to the left 
	wxColour cr[3]; // and to the right
	int x = r.width/32;
	int y = (r.height*5)/6;
	//wxPen pen;
	//pen.SetColour( wxColour(255,255,0) );
	//dc.SetPen(pen);
	for( int ix=0; ix<3; ix++ ){
		if( !dc.GetPixel( r.x+(r.width*(32+ix))/35, r.y+y, &cr[ix]) ) return false;
		//dc.DrawPoint( r.x+(r.width*(32+ix))/35, r.y+y );
	}
	y = r.height/6;
	for( int ix=0; ix<3; ix++ ){
		if( !dc.GetPixel( r.x+(r.width*(1+ix))/35, r.y+y, &cl[ix]) ) return false;
		//dc.DrawPoint( r.x+(r.width*(1+ix))/35, r.y+y );
	}
	// # Could do better check that these colours are grey like
	if( cr[0]==cr[1] && cr[1]==cr[2] ){
		g_wxu_fc_col = cr[0];
	}
	else if( cl[0]==cl[1] && cl[1]==cl[2] ){
		g_wxu_fc_col = cl[0];
	}
	else return false;
	
	int r1 = cl[0].Red();
	int g1 = cl[0].Green();
	int b1 = cl[0].Blue();
	int r2 = cr[0].Red();
	int g2 = cr[0].Green();
	int b2 = cr[0].Blue();
	
	return true;
}

// get active caption colour
bool SetCaptionColourFrom( wxWindow *pfrm, wxScreenDC *pdc ){
	if( !pfrm ) pfrm = wxTheApp->GetTopWindow();
	if( !pfrm ) return false;
	wxFrame *frm = wxDynamicCast(pfrm,wxFrame);
	wxWindow *pmb = frm ? frm->GetMenuBar() : NULL;
	
	// A descendant of pfrm must have focus for it to be considered active
	wxWindow *pwf = wxWindow::FindFocus();
	while( pwf && pwf!=pfrm )
		pwf = pwf->GetParent();
	if( !pwf ) return false;
	
	wxRect r = pfrm->GetScreenRect();
	wxRect rc = pfrm->GetClientRect();
	pfrm->ClientToScreen( &rc.x, &rc.y );
	int bw = rc.x-r.x;	// Border width
	//int ch = wxSystemSettings::GetMetric(wxSYS_CAPTION_Y,pfrm);	// Does not work on GTK
	int ch = rc.y-r.y;
	if( pmb ) ch -= pmb->GetScreenRect().height;
	
	if( ch<5 || ch>100)return false;

	// So we have active window
	bool dc_create = false;
	if( !pdc ){
		pdc = new wxScreenDC;
		if( !pdc ) return false;
		dc_create = true;
	}

	// 3 samples to the left and to the right
	wxColour cl[3], cr[3];	
	// Left first
	int x = r.x + r.width/20;
	int y = r.y + (ch*5)/6;
	for( int ix=-1; ix<2; ix++ ){
		if( !pdc->GetPixel( x+((ix*r.width)/32), y, &cl[ix+1]) ){
			if(dc_create) delete pdc;
			return false;
		}
		//dc.DrawPoint( ...
	}
	// And now right
	x = r.x + (19*r.width)/20;
	for( int ix=-1; ix<2; ix++ ){
		if( !pdc->GetPixel( x+((ix*r.width)/32), y, &cr[ix+1]) ){
			if(dc_create) delete pdc;
			return false;
		}
		//dc.DrawPoint( ...
	}
	bool res = false;
	if( cl[0]==cl[1] && cl[1]==cl[2] ){
		g_wxu_caption_col = cl[0];
		res = true;
	}
	else if( cr[0]==cr[1] && cr[1]==cr[2] ){
		g_wxu_caption_col = cr[0];
		res = true;
	}
	
	int r1 = cl[0].Red();
	int g1 = cl[0].Green();
	int b1 = cl[0].Blue();
	/*
	r1 = cl[1].Red();
	g1 = cl[1].Green();
	b1 = cl[1].Blue();
	r1 = cl[2].Red();
	g1 = cl[2].Green();
	b1 = cl[2].Blue();
	*/
	
	if( dc_create ) delete pdc;
	return res;
}



// For redirecting to global new/delete operators
void *wxMalloc( size_t size ){
	return new char[size];
}

void wxFree( void *pv ){
	delete (char*)pv;
}

/*
wxDummyParent::wxDummyParent( wxWindow *parent, bool do_show ) 
  : wxDialog(NULL,wxID_ANY,_T("DummyParent"), 
	wxPoint(parent->GetScreenRect().x+(parent->GetScreenRect().width-100)/2, 
			parent->GetScreenRect().y+(parent->GetScreenRect().height-50)/2 ), 
	wxSize(100,50), wxTOPLEVEL_EX_DIALOG ) {
	if( do_show ) Show(true);
}

//	wxRect rp = parent->GetScreenRect( );
//	wxPoint pt( rp.x+(rp.width-100)/2, rp.y+(rp.height-50)/2 );
//    wxDialog dummy_parent(NULL, wxID_ANY, _T("Top-Level"), pt, wxSize(100,50), wxTOPLEVEL_EX_DIALOG);
//    dummy_parent.Show(true);


int wxLocalMsgBox(const wxString& message, const wxString& caption, long style,
                 wxWindow *parent, int WXUNUSED(x), int WXUNUSED(y) )
{
    long decorated_style = style;

    if ( ( style & ( wxICON_EXCLAMATION | wxICON_HAND | wxICON_INFORMATION | wxICON_QUESTION ) ) == 0 )
    {
        decorated_style |= ( style & wxYES ) ? wxICON_QUESTION : wxICON_INFORMATION ;
    }

    // It is very difficult to focus a wxMessageDialog on a child window (GTK), so 
    // we have to create a dummy parent window and hide it under the dialog...
    
    wxDummyParent dummy_parent(parent);
    wxMessageDialog dlg(&dummy_parent, message, caption, decorated_style|wxCENTER);

    int ans = dlg.ShowModal();
    switch ( ans )
    {
        case wxID_OK:
            return wxOK;
        case wxID_YES:
            return wxYES;
        case wxID_NO:
            return wxNO;
        case wxID_CANCEL:
            return wxCANCEL;
    }

    wxFAIL_MSG( _T("unexpected return code from wxMessageDialog") );

    return wxCANCEL;
}
*/

/*
wxLocalMsgDlg::wxLocalMsgDlg(    wxWindow *parent,
                                 const wxString& message,
                                 const wxString& caption,
                                 long style,
                                 const wxPoint& pos) 
 : wxMessageDialog(parent,message,caption,style,pos) {
	Ctor( parent );
}

// ...to see variables in debugger
void wxLocalMsgDlg::Ctor( wxWindow *parent ){
	if( parent && parent!=GetParent() ){
		wxRect rs = GetScreenRect( );
		wxRect rp = parent->GetScreenRect( );
		rs.x = rp.x + (rp.width-rs.width)/2;
		rs.y = rp.y + (rp.height-rs.height)/2;
		Move( rs.x, rs.y );
	}
}
*/


/*
#ifdef UTIL_OVERLOAD_NEW 
UTIL_INLINE void * operator new (size_t size, wxChar * fileName, int lineNum){
	return wxDebugAlloc( size, fileName, lineNum, false, false );
}

UTIL_INLINE void * operator new (size_t size){
	return wxDebugAlloc( size, NULL, -1, false, false );
}

UTIL_INLINE void * operator new[] (size_t size){
	return wxDebugAlloc( size, NULL, -1, false, true );
}

UTIL_INLINE void * operator new[] (size_t size, wxChar * fileName, int lineNum){
	return wxDebugAlloc( size, fileName, lineNum, false, true );
}

UTIL_INLINE void operator delete (void * buf){
	wxDebugFree( buf );
}

UTIL_INLINE void operator delete[] (void * buf){
	wxDebugFree( buf );
}
#endif // UTIL_OVERLOAD_NEW
*/

