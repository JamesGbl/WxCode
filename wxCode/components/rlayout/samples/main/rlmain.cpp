
// Include wx things
#include "wx/log.h"
#include "wx/minifram.h"
#include "wx/textctrl.h"
#include "wx/clipbrd.h"
#include "wx/stdpaths.h"
#include "wx/filename.h"

// Include RL things
#include "wx/RlManager.h"
#include "wx/RlPainter.h"
#include "wx/RlDialog.h"
#include "wx/RlFromXml.h"

#include "wx/LocalMenu.h"
#include "wx/NamedDir.h"
#include "wx/wxUtils.h"

#include "utils/ExpArr.hpp"

// The app class
#include "main.h"


// If ENABLE_SIGTHROW is not defined, this boils down to nothing
#include "pi/sigthrow.h"


// These are used to create window and background areas by type
wxArrayString GetAreaTypes( bool only_fg, bool only_wnd=false );
wxRlItem *CreateArea( wxString type, wxWindow *parent=NULL );
wxWindow *CreateWindow( wxString type, wxWindow *parent=NULL );


// Prototype, shared event handler (between main window and flaoting windows) 
// for wxRlItem mouse events
void OnRlMouseShared( wxCommandEvent &evt, wxWindow *wnd, wxRlManager *rl_mgr, bool is_float );


// Keep track of the main window and the log
wxWindow *g_main_wnd;
wxRlAppPane *g_rl_log;

// For storing parts of the layout that have been detached
// while saving / loading layouts
wxRlContentStore g_rl_cs;


// IDs for menus
#define ID_MBAR_CREATE 1000
#define ID_MBAR_CLOSE  1050
#define ID_MBAR_SAVE_LAYOUT 	1101
#define ID_MBAR_LOAD_LAYOUT 	1102
#define ID_MBAR_NEW_LAYOUT      1103
#define ID_MBAR_PRINT_LAYOUT1   1104
#define ID_MBAR_PRINT_LAYOUT2   1105

#define ID_MBAR_SAVE_LOG	1200
#define ID_MBAR_COPY_LOG	1201
#define ID_MBAR_NEW_LOG		1202

#define ID_MENU_QUIT      1300
#define ID_MENU_ABOUT     1301
#define ID_MENU_WINDOW    1302



// Helper func
int GetMaxInt( const wxColour &c ){
	if( c.Red()>c.Green() )
		return c.Blue()>c.Red() ? c.Blue() : c.Red();
	else
		return c.Blue()>c.Green() ? c.Blue() : c.Green();
}

//
// A window that paints a pixel ruler along its sides
//
class wxFixSizeWnd : public wxWindow {
public:
	wxFixSizeWnd(  wxWindow* parent, 
				   const wxColour cfg=wxColour(0x80,0x80,0x80), const wxColour cbg=*wxWHITE, 
				   const wxChar *text=NULL )
                 : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER),
                 m_cfg(cfg), m_cbg(cbg), m_text(text), m_report_details(text?false:true), m_rli(NULL)
    {	
		// Scale up colors to max brightness
		int mi = GetMaxInt( cfg );
		if( mi==0 ) m_ocfg.Set(255,255,255);
		else m_ocfg.Set( cfg.Red()*255/mi, cfg.Green()*255/mi, cfg.Blue()*255/mi );

		mi = GetMaxInt( cbg );
		if( mi==0 ) m_ocbg.Set(255,255,255);
		else m_ocbg.Set( cbg.Red()*255/mi, cbg.Green()*255/mi, cbg.Blue()*255/mi );
	}

	void OnPaint(wxPaintEvent& WXUNUSED(evt))
	{
	    wxPaintDC dc(this);
	    wxSize size = GetClientSize();
	
        wxBrush br(m_cbg);
        wxPen pen(m_cfg);
        wxPen pen_bg(m_cbg);
	    dc.SetBrush(br);
	    dc.SetPen(pen_bg);
	    dc.DrawRectangle(0, 0, size.x, size.y);
	
        // Draw ruler along sides
	    dc.SetPen(pen);
        
        for( int x=0; x<size.x; x+=10 ){
            dc.DrawLine( x, 0, x, !(x%50)?16:8 );
            dc.DrawLine( x, size.y, x, !(x%50)?size.y-16:size.y-8 );
        }
        for( int y=0; y<size.y; y+=10 ){
            dc.DrawLine( 0, y, !(y%50)?10:5, y );
            dc.DrawLine( size.x, y, !(y%50)?size.x-16:size.x-8, y );
        }

        
	    int w, height;
	    dc.SetFont(*wxNORMAL_FONT);
        
        if( !m_report_details ){
    	    wxString s;
    	    s.Printf(wxT("%s: %d x %d"), (const wxChar*)m_text, size.x, size.y);
    	    while( true ){
    	    	dc.GetTextExtent(s, &w, &height);
    	    	if( w<size.x ) break;
    	    	s = s.Left( s.Length()-1 );
    	    }
            dc.DrawText(s,(size.x-w)/2, (size.y-height)/2);
        }
        else{
		    // Find manager of this window - a bit expensive, could cache value, but then 
	        // we float / dock so it changes
		    wxRlManager *prlm = wxRlManager::GetRlManagerOf(this);
		    // If the sizeritem holding us is not known, find it
		    if( prlm && !m_rli ){
		    	m_rli = ToRlItem( prlm->GetRoot().GetSizer()->GetItem(this,true) );
		    	// We really want to report for the pane item, not the window item
		    	if( m_rli && GetTypeOf(m_rli->GetParent())==RlAppPane ) m_rli = m_rli->GetParent();
		    }
	
		    // The strings we want to show
		    wxString s_sz, s_a, s_p;
		    s_sz.Printf(wxT("Size: %d x %d"), size.x, size.y);
		    s_a = _T("Address: ");
		    if( m_rli )
		    	s_a += m_rli->GetParent() ? m_rli->GetAddress() : _T("[Root]");
		    else  
		    	s_a += _T("No address found");
		    s_p = _T("Proportion: ");
		    s_p += m_rli ? wxString::Format(_T("%d"),m_rli->GetProportion()) : _T("?");
		    
		    dc.SetFont(*wxNORMAL_FONT);
		    dc.GetTextExtent(s_a, &w, &height);
	        dc.DrawText(s_a, (size.x-w)/2, (size.y-height)/2- height*2 );
		    dc.GetTextExtent(s_sz, &w, &height);
	        dc.DrawText(s_sz, (size.x-w)/2, (size.y-height)/2 );
		    dc.GetTextExtent(s_p, &w, &height);
	        dc.DrawText(s_p, (size.x-w)/2, (size.y-height)/2 + height*2 );
        }
	}

	void OnSize(wxSizeEvent& WXUNUSED(evt))
	{
	    Refresh();
	}
	
    wxColour m_cfg, m_cbg;
    wxColour m_ocfg, m_ocbg;
    wxString m_text;
    bool     m_report_details;
    wxRlItem *m_rli;
    
	DECLARE_EVENT_TABLE();
};


BEGIN_EVENT_TABLE(wxFixSizeWnd, wxWindow)
    EVT_PAINT(wxFixSizeWnd::OnPaint)
    EVT_SIZE(wxFixSizeWnd::OnSize)
    //EVT_ERASE_BACKGROUND(wxFixSizeWnd::OnEraseBackground)
END_EVENT_TABLE()



//
// This is a class that represents floating windows. It is based on wxMiniFrame,
// is managed by another wxRlManager and knows its origin (so it can be docked back). 
// We basically want to handle its mouse events.
//
class FloatingFrame : public wxMiniFrame {
	public:
		FloatingFrame( wxWindow *parent, wxRlManager *parent_rl_mgr ) :  
            wxMiniFrame( parent, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize,
            			wxRESIZE_BORDER ),
            m_parent_rl_mgr(parent_rl_mgr),
            m_rl_mgr(NULL) { }
        
		virtual ~FloatingFrame(){
			// Destroying windows is not always straight forward. 
			// If deleting the layout manager from inside OnRlMouse, 
			// we end up with unprocessed events that get sent to us.
			// Let all of that pass and delete the manager here.
			delete m_rl_mgr;
		}

        wxRlManager *m_rl_mgr;	// Has to be set after creation
        
	protected:
        wxRlManager *m_parent_rl_mgr;

		// Come here for reported mouse events in layout items. 
        // We route to an event handler than takes care of both cases:
        // 1 - The main frame window with many panes
        // 2 - Floating frames with just one pane
 		void OnRlMouse( wxCommandEvent &evt ){
			OnRlMouseShared( evt, this, m_rl_mgr, true );			
		}

    DECLARE_EVENT_TABLE();
};


BEGIN_EVENT_TABLE(FloatingFrame, wxMiniFrame)
    //EVT_ERASE_BACKGROUND(FloatingFrame::OnEraseBackground)
    //EVT_PAINT(FloatingFrame::OnPaint)
    EVT_RL_MOUSE(FloatingFrame::OnRlMouse)
    //EVT_RL_PANE_ACTIVATE(FloatingFrame::OnRlPaneActivate)
END_EVENT_TABLE()




// 
// wxFrame derived class, main window
//

// Style: 2 pixel sunken border, on all sides
#define RL_FRAME_MGR_STYLE (wxSUNKEN_BORDER|wxBORDER_DOUBLE|wxALL_DIR)

class MyFrame: public wxFrame
{
	public:
		MyFrame(const wxString& title) : 
            wxFrame( NULL, -1, title, wxDefaultPosition, wxDefaultSize,
                     (wxDEFAULT_FRAME_STYLE&~(wxFULL_REPAINT_ON_RESIZE))|wxNO_FULL_REPAINT_ON_RESIZE ),
            // Create a wxRlManager with a temporary background
            m_rl_mgr(this,RL_FRAME_MGR_STYLE,RlClientTempBg), m_wnd_fix(NULL), m_panel_fix(NULL) { 
			g_main_wnd = this;
			m_wait_close = false;
			Ctor(); 
        }
		void Ctor( );
        
		virtual ~MyFrame(){ Dtor(); }
		void Dtor( ){ }

	private:
        wxRlManager m_rl_mgr;
        RlWinStdPainter m_rl_painter;
        
        wxFixSizeWnd *m_wnd_fix;
        wxRlPanel *m_panel_fix;

        bool m_wait_close;
        
		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
        
		// Come here for reported mouse events in layout items
		void OnRlMouse( wxCommandEvent &evt ){
			// We used same code for main window and popup windows
			OnRlMouseShared( evt, this, &m_rl_mgr, false );			
		}
		
		void OnRlPaneActivate( wxCommandEvent &evt ){
			// Come here when a wxRlPane is activated
			//wxRlAppPane *rlpane = wxDynamicCast( evt.GetClientData( ), wxRlAppPane );
		}
		
		// Event handler when slider in child window is moved
        void OnScroll(wxScrollEvent &evt){
            int p = evt.GetPosition();
            if( m_wnd_fix && m_panel_fix ){
                wxRadioButton *pcb = wxDynamicCast( m_panel_fix->FindWindowById(XRCID("ID_FG")), wxRadioButton ); 
                if( pcb ){
                    wxColour *pcs = pcb->GetValue() ? &m_wnd_fix->m_ocfg : &m_wnd_fix->m_ocbg;
                    wxColour *pcd = pcb->GetValue() ? &m_wnd_fix->m_cfg : &m_wnd_fix->m_cbg;
                    pcd->Set( pcs->Red()*p/100, pcs->Green()*p/100, pcs->Blue()*p/100 ); 
                    m_wnd_fix->Update( );
					m_wnd_fix->Refresh( );
                } 
            }
        }
        
        void OnEraseBackground(wxEraseEvent &evt){
        }
        
        void OnPaint(wxPaintEvent &evt){
    	    wxPaintDC dc(this);
        }
        
        void OnMBarCreate( wxCommandEvent &evt ){
        	// Come here when creating new item from main menu bar
        	int ix = evt.GetId()-ID_MBAR_CREATE;
        	if( ix>=0 ){
        		wxArrayString as = GetAreaTypes(false);
        		if( ix<(int)(int)as.GetCount() ){
					wxRlItem *rli_new = CreateArea(as[ix],this);
					if( rli_new ){
						// Insert close to the middle of pane 
						wxPoint pt = wxPoint( GetSize().GetX()/2, GetSize().GetY()/2 );
						wxString addr = m_rl_mgr.GetRoot().FindInsertionAddress( pt, rli_new );
						if( !m_rl_mgr.GetRoot().InsertAt(rli_new,addr) ){
							wxMessageBox( _T("Failed inserting item"), _T("Insert failure") );
							delete rli_new;
						}
						else m_rl_mgr.Update();
					}
        		}
        	}
        }

        void OnMBarCloseArea( wxCommandEvent &evt ){
    		if( wxMessageBox(_T("After clicking OK in this dialog, \nclick where you want to close an area"), _T("Close Area"), wxOK|wxCANCEL)==wxOK ){
    			m_wait_close = true;
    			CaptureMouse();
    		}
        }
        
        void OnMBarLoadLayout( wxCommandEvent &evt ){
        	// Query for a name
        	wxString layout_name = wxGetTextFromUser( _T("Name of layout:"), _T("Load Layout") );
        	if( !layout_name.Length() ) return;

        	// Read the layout file
        	wxString layout;
        	if( !wxLoadString( layout, layout_name + _T(".xml") ) ){
        		wxMessageBox(_T("Failed loading layout XML"));
        		return;
        	}

        	
        	// Now the tricky bit comes... we must detach all content bits (essentially what is 
        	// inside the application panes and 'free windows') and save them for later (when possibly restoring 
        	// this layout). The name bit of a wxRlItem allows us to associate the contents 
        	// later on with the layout component.
        	m_rl_mgr.DetachContents( g_rl_cs );
        	
        	
        	// Now set the new layout
        	// We give the store of detached contents from the previous line
        	// Then, after setting up the 'skeleton' layout, it will fill in
        	// detached nodes wherever there is a name match.
        	if( !m_rl_mgr.SetState( layout, RL_XMLLOAD_SKELETON, &g_rl_cs ) ){
        		wxMessageBox(_T("Failed setting layout XML"));
        		// Try restoring the log
        		wxRlAppPane *ap = m_rl_mgr.FindAppPane(_T("Log Pane")); 
        		if( ap ){
        			ap->SetWindow( g_rl_log->GetWindow() );
        			g_rl_log = ap;
        		}
        		return;
        	}
        
        	// It went fine. It's good to have global access to the log, so see if we
        	// can find it:
        	g_rl_log = m_rl_mgr.FindAppPane(_T("Log Pane"));
        	
        	// Done!
        	m_rl_mgr.Update();
        }
        
        void OnMBarSaveLayout( wxCommandEvent &evt ){
        	// Query for a name
        	wxString layout_name = wxGetTextFromUser( _T("Name of layout:"), _T("Save Layout") );
        	if( !layout_name.Length() ) return;

        	// Before saving the layout, we must make sure that each part that has contents 
        	// to put aside, also has been assigned a name. Then we can later associate a
        	// container part in the layout the right content. 
        	m_rl_mgr.VerifyNodeNames( );
        	
        	// Get the layout as an XML string
        	wxString layout;
        	if( !m_rl_mgr.GetState( layout ) ){
        		wxMessageBox(_T("Failed getting layout XML"));
        		return;
        	}
        	if( !wxSaveString( layout, layout_name + _T(".xml") ) ){
        		wxMessageBox(_T("Failed saving layout XML"));
        		return;
        	}
        }
        
        void OnMBarNewLayout( wxCommandEvent &evt ){
        	if( wxMessageBox(_T("Clear current layout?"),_T("Clear Layout"), wxOK|wxCANCEL)==wxOK ){
        		// Before clearing the layout, detach contents
        		// See comment in OnMBarLoadLayout
            	m_rl_mgr.DetachContents( g_rl_cs );

            	// We set the root item to a BG hint
        		m_rl_mgr.SetRoot( new wxRlClientBgHint(_T("An empty layout")) );
        	}
        }
        
        void OnMBarPrintLayout( wxCommandEvent &evt ){
        	wxMessageBox(_T("Printing layout to stdout & log"), _T("Print Layout"), wxOK);
        	m_rl_mgr.GetRoot().Dump( 1 );
        }
        
        void OnMBarPrintLayoutContent( wxCommandEvent &evt ){
        	wxMessageBox(_T("Printing layout + content to stdout & log"), _T("Print Layout"), wxOK);
        	m_rl_mgr.GetRoot().Dump( 2 );
        }
        
        void OnMBarSaveLog( wxCommandEvent &evt ){
        	if( !g_rl_log ) return;
        	wxTextCtrl *ptctl = wxDynamicCast(g_rl_log->GetWindow(),wxTextCtrl);
        	if( !ptctl ) return;
        	
        	wxString log_file = wxGetTextFromUser( _T("Name of log file:"), _T("Save Log") );
        	if( !log_file.Length() ) return;
        	wxSaveString( ptctl->GetValue(), log_file ); 
        }
        
        void OnMBarCopyLog( wxCommandEvent &evt ){
        	if( !g_rl_log ) return;
        	wxTextCtrl *ptctl = wxDynamicCast(g_rl_log->GetWindow(),wxTextCtrl);
        	if( !ptctl ) return;
        	
        	if( wxTheClipboard->Open() ){
        	    wxTheClipboard->SetData( new wxTextDataObject(ptctl->GetValue()) );
        	    wxTheClipboard->Close();
        	    wxLogMessage(_T("Log contents copied to clipboard"));
        	}
        }
        
        void OnMBarClearLog( wxCommandEvent &evt ){
        	if( !g_rl_log ) return;
        	wxTextCtrl *ptctl = wxDynamicCast(g_rl_log->GetWindow(),wxTextCtrl);
        	if( !ptctl ) return;
        	if( wxMessageBox(_T("Clear the log?"),_T("Clear Log"), wxOK|wxCANCEL)==wxOK )
        		ptctl->SetValue(_T("")); 
        }
        
        void OnLeftDown( wxMouseEvent &evt ){
        	if( m_wait_close ){
        		m_wait_close = false;
        		ReleaseMouse();
        		
        		// Find the item where we clicked
        		wxRlItem *rli = m_rl_mgr.FindItemAt( evt.GetPosition() );
        		if( !rli ) return;
        		
        		// We don't want to close edges and other 'non user areas' in this way
        		if( !rli->IsEnumerable() ) return;
        		
        		// Go to top most wxRlItem that is entity
        		for( wxRlItem *rli_ent=rli; rli_ent; ){
        			rli = rli_ent;
        			rli_ent = rli->GetParentWithCapab( RL_CAPAB_ENTITY );
        		}
        		
        		// Don't close the log
        		if( rli==g_rl_log ) return;
        		
        		// Close it
        		rli->Destroy(true);
        		m_rl_mgr.Update();
        	}
        }
    DECLARE_EVENT_TABLE();
};


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_MENU_QUIT, MyFrame::OnQuit)
    EVT_MENU(ID_MENU_ABOUT, MyFrame::OnAbout)
    EVT_MENU_RANGE(ID_MBAR_CREATE,ID_MBAR_CREATE+20, MyFrame::OnMBarCreate)
    EVT_MENU(ID_MBAR_CLOSE, MyFrame::OnMBarCloseArea)
    EVT_MENU(ID_MBAR_SAVE_LAYOUT, MyFrame::OnMBarSaveLayout)
    EVT_MENU(ID_MBAR_LOAD_LAYOUT, MyFrame::OnMBarLoadLayout)
    EVT_MENU(ID_MBAR_NEW_LAYOUT, MyFrame::OnMBarNewLayout)
    EVT_MENU(ID_MBAR_PRINT_LAYOUT1, MyFrame::OnMBarPrintLayout)
    EVT_MENU(ID_MBAR_PRINT_LAYOUT2, MyFrame::OnMBarPrintLayoutContent)
	EVT_MENU(ID_MBAR_SAVE_LOG, MyFrame::OnMBarSaveLog)
	EVT_MENU(ID_MBAR_COPY_LOG, MyFrame::OnMBarCopyLog)
	EVT_MENU(ID_MBAR_NEW_LOG, MyFrame::OnMBarClearLog)
    
    EVT_ERASE_BACKGROUND(MyFrame::OnEraseBackground)
    EVT_PAINT(MyFrame::OnPaint)
    EVT_LEFT_DOWN(MyFrame::OnLeftDown)
    EVT_RL_MOUSE(MyFrame::OnRlMouse)
    EVT_RL_PANE_ACTIVATE(MyFrame::OnRlPaneActivate)
END_EVENT_TABLE()


// This is a small dialog script to fill in a wxPanel
const char *g_pc_bright_panel = " \
    <sizer orient='horz' padding='10,0' > \
      <sizer prop='10' orient='vert' padding='0,0' > \
        <window type='radio' expand='true' prop='100' label='Foreground' name='ID_FG' state='group,' /> \
        <window type='radio' expand='true' prop='100' label='Background' name='ID_BG' /> \
      </sizer> \
      <sizer orient='vert' padding='5,10' > \
        <window type='text' align='center' expand='true' prop='0' label='Adjust brightness' /> \
        <window class='wxSlider' name='ID_SLIDER' expand='true' prop='10' minsize='200,20' > \
          <value>50</value> \
          <min>0</min> \
          <max>100</max> \
        </window> \
      </sizer> \
    </sizer> ";


void MyFrame::Ctor( ){

	// Since we paint the whole area of the window, we don't need system background
	SetBackgroundStyle( wxBG_STYLE_CUSTOM );

    // We need to set the painter object 
    m_rl_mgr.SetPainter( &m_rl_painter );

    // This causes the background item to be enclosed into a vertical sizer. 
    // This sizer becomes the new root item
    m_rl_mgr.GetRoot().Enclose( wxVERTICAL|wxEXPAND );

    // Cache root item for convenient access
    wxRlItem &root = m_rl_mgr.GetRoot();
    wxRlItem *rli;
    
    // Make new horizontal sizer inside it
    root.Part(0).Enclose( wxHORIZONTAL|wxEXPAND );

    
    // The top horisontal layer - Three panes with windows and a background item
    rli = new wxRlAppPane( _T("Test Pane1"), new wxFixSizeWnd(this), CAP_BIT_BUTTON_DOWN|CAP_BIT_BUTTON_CLOSE );
    root.Part(0).Insert( 0, rli );

    rli = new wxRlAppPane( _T("Test Pane2"), new wxFixSizeWnd(this,*wxGREEN,*wxWHITE,_T("Resizable")), CAP_BIT_BUTTON_DOWN );
    root.Part(0).Insert( 1, rli );
    
    // Add a background item with a hint 
    root.Part(0).Insert( 2, new wxRlClientBgHint(_T("Click the right mouse button for a menu")) );

    rli = new wxRlAppPane( _T("Another pane..."), new wxFixSizeWnd(this), CAP_BIT_BUTTON_DOWN|CAP_BIT_BUTTON_CLOSE );
    root.Part(0).Insert( 3, rli );

    
    // Next horizontal layer
    // Create a log window - a pane without a close button
    wxTextCtrl *log = new wxTextCtrl(this,wxID_ANY,wxEmptyString, wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY|wxBORDER_NONE);
    // This is to make certain its min size is as small as the fix size 
    // window we're can show and move. If the two are in the same sizer
    // the log window cannot have a larger min size.
    log->SetMinSize( wxSize(80,80) );
    wxRlAppPane *rlap = new wxRlAppPane( _T("Log Pane"), log, 0 );
    // We want to keep track of the log so we don't destroy it later
    g_rl_log = rlap;
    wxLog::SetActiveTarget( new wxLogTextCtrl(log) );
    // Insert it in a layer below the previous ones
    root.Insert( -1, rlap ); // Position -1: last in sizer 
    wxLogMessage(_T("Log in wxTextCtrl"));

    
    // Bitmaps for toolbar are most likely located relative the executable
    wxString bm_path=_T("bitmaps");
	if( wxFileExists(bm_path + wxPSEP + _T("save.png")) )
		bm_path += wxPSEP;
	else{
		// "exec" is a name for the directory containing this executable
		// We look for "bitmaps" in that dir and parent directories
		bm_path = wxFindDir( wxGetNamedDir(_T("exec")), _T("bitmaps") );
		if( bm_path.Length() )
			bm_path = bm_path + _T("bitmaps") + wxPSEP; 
	}

    // # Will wxImage clear this one? 
    wxImage::AddHandler(new wxPNGHandler);

    // We want a small vertical toolbar to go inside the log pane 
    bool have_bms = true;
    wxBitmap tool_bms_log[] = { 
    		wxBitmap(bm_path+_T("save.png"),wxBITMAP_TYPE_PNG),
    		wxBitmap(bm_path+_T("copy.png"),wxBITMAP_TYPE_PNG),
    		wxBitmap(bm_path+_T("new.png"),wxBITMAP_TYPE_PNG),
    };
    if( !tool_bms_log[0].IsOk() ){
    	wxMessageBox(_T("Toolbar bitmaps under samples/bitmaps not found"));
    	have_bms = false;
    }
    wxToolBar *ptb = new wxToolBar( this, -1, wxDefaultPosition, wxDefaultSize, wxTB_VERTICAL|wxTB_FLAT|wxTB_NODIVIDER | (have_bms?0:wxTB_TEXT|wxTB_NOICONS) );
    ptb->AddTool( ID_MBAR_SAVE_LOG, _T("Save"), have_bms?tool_bms_log[0]:wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, _T("Save log") );
    ptb->AddSeparator( );
    ptb->AddTool( ID_MBAR_COPY_LOG, _T("Copy"), have_bms?tool_bms_log[1]:wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, _T("Copy log to clipboard") );
    ptb->AddSeparator( );
    ptb->AddTool( ID_MBAR_NEW_LOG, _T("New"), have_bms?tool_bms_log[2]:wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, _T("Clear log") );
    ptb->Realize( );

    // This will set the toolbar inside the log pane
    rlap->SetToolBar( ptb, false );
    
    
    
    // We create another horizontal layer that has fix vertical size
    // We don't want a caption, so we don't need the wxRlAppPane class
    // In creating the wxRlItem, it is the 0 proportion that makes it have fix Y size
    rli = new wxRlItem( new wxBoxSizer(wxHORIZONTAL), 0, wxEXPAND, RlSizer );
    // This will mark the item as an independent group, it cannot be target for pane moves
    rli->SetIsEntity( );
    
    // Create a wxPanel to go to the left in it
    m_panel_fix = new wxRlPanel(this,wxID_ANY);
    m_panel_fix->SetLayout( g_pc_bright_panel );
    m_panel_fix->SetBackgroundColour( wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE) );
    rli->Insert( 0, m_panel_fix, 0, wxEXPAND );
    
    // Insert a completely fix size window (again, it is 0 in proportion that does this)
    m_wnd_fix = new wxFixSizeWnd(this,*wxRED,*wxBLACK,_T("FIX!"));
    rli->Insert( 1, m_wnd_fix, 0, wxEXPAND ).SetMinSize(160,80);
    
    // Insert some panel colored background to fill up the layer (-1 means last)
    wxRlItem *rli_spc = new wxRlItem(RlFrameBg,wxEXPAND); 
    rli->Insert( -1, rli_spc );
    
    // And put this last (-1) in the root sizer
    root.Insert( -1, rli );
    
    
    // We want to connect events from the slider of the tool panel
    wxWindow *wnd = m_panel_fix->FindWindowById(XRCID("ID_SLIDER"));
    if( wnd ){
		const wxChar *pc = wnd->GetClassInfo()->GetClassName();
        wnd->Connect( wxEVT_SCROLLWIN_THUMBTRACK, wxEvtFnCast(Scroll) &MyFrame::OnScroll, NULL, this );
        wnd->Connect( wxEVT_COMMAND_SLIDER_UPDATED, wxEvtFnCast(Scroll) &MyFrame::OnScroll, NULL, this );
	}

    // Create a new global toolbar
    wxBitmap tool_bms[] = { 
    		wxBitmap(bm_path+_T("save.png"),wxBITMAP_TYPE_PNG),
    		wxBitmap(bm_path+_T("open.png"),wxBITMAP_TYPE_PNG),
    		wxBitmap(bm_path+_T("new.png"),wxBITMAP_TYPE_PNG),
    		wxBitmap(bm_path+_T("help.png"),wxBITMAP_TYPE_PNG),
    		wxBitmap(bm_path+_T("print.png"),wxBITMAP_TYPE_PNG),
    };
    
    /*wxToolBar **/ptb = new wxToolBar( this, -1, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxTB_FLAT|wxTB_NODIVIDER | (have_bms?0:wxTB_TEXT|wxTB_NOICONS) );
    ptb->AddTool( ID_MBAR_SAVE_LAYOUT, _T("Save"), have_bms?tool_bms[0]:wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, _T("Save layout") );
    ptb->AddTool( ID_MBAR_LOAD_LAYOUT, _T("Open"), have_bms?tool_bms[1]:wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, _T("Load layout") );
    ptb->AddSeparator( );
    ptb->AddTool( ID_MBAR_NEW_LAYOUT, _T("New"), have_bms?tool_bms[2]:wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, _T("New (empty) layout") );
    ptb->AddSeparator( );
    ptb->AddTool( ID_MENU_ABOUT, _T("Help"), have_bms?tool_bms[3]:wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, _T("About box") );
    ptb->AddSeparator( );
    ptb->AddTool( ID_MBAR_PRINT_LAYOUT1, _T("Print layout"), have_bms?tool_bms[4]:wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, _T("Print layout to stdout & log") );
    ptb->AddTool( ID_MBAR_PRINT_LAYOUT2, _T("Print layout + contents"), have_bms?tool_bms[4]:wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, _T("Print layout + contents to stdout & log") );
    ptb->Realize( );
    
    // Insert this at the top of the window - we will get a gripper to move it with
    root.Insert( 0, new wxRlToolPane(_T("Global"), wxHORIZONTAL|wxEXPAND, ptb, true) );

#if wxUSE_MENUS
    
    // Create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append( ID_MBAR_SAVE_LAYOUT, _T("&Save layout..."), _T("Save the layout to an XML file") );
    fileMenu->Append( ID_MBAR_LOAD_LAYOUT, _T("&Load layout..."), _T("Load the layout from an XML file") );
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_MENU_QUIT, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(ID_MENU_ABOUT, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));
    
    // Append menu items for creating contained windows
    wxMenu* windowMenu = new wxMenu(_T(""));
    windowMenu->Append( ID_MBAR_CLOSE, _T("&Close Area"), _T("Close an area in the main window") );
    windowMenu->AppendSeparator();
    wxArrayString as = GetAreaTypes(false);
    for( int ix=0; ix<(int)(int)as.GetCount(); ix++ ){
    	if( as[ix]==_T("---") )
    		windowMenu->AppendSeparator();
    	else{
	    	wxString s = _T("Create ");
	    	s += as[ix];
	    	windowMenu->Append( ID_MBAR_CREATE+ix, s, _("Create new window or background area") );
    	}
    }
    mbar->Append(windowMenu, _("&Window"));
        
    Show( true );	// Without show here, wxGTK hangs in the call to SetMenubar

    SetMenuBar(mbar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Hello world..."),0);
    //SetStatusText(wxbuildinfo(short_f),1);
#endif // wxUSE_STATUSBAR

    // Have to inform layout manager that size has changed (after adding menu & statubar)
    m_rl_mgr.Update( );
}


void MyFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = _T("wxRecursiveLayout sample");  // wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}



// This is a shared event handler between MyFrame (main app window) 
// which manages many panes and FloatingFrame which just manages one.
// 
// This is to remove duplicate code in mouse event handler which is mostly
// similar in the two cases. An alternative approach wopuld be to use a 
// shared template base class, but then, that syntax is sometimes awkward.
void OnRlMouseShared( wxCommandEvent &evt, wxWindow *wnd, wxRlManager *rl_mgr, bool is_float ){
	int reason = RL_REASON(evt.GetInt());
	int button = RL_BUTTON(evt.GetInt());
	wxRlItem *rli = wxDynamicCast(evt.GetEventObject(), wxRlItem);
	wxASSERT( rli );

	switch( rli->GetType() ){
	
	case RlClientBgHint:	// Mouse event in background hint area
	case RlClientBg:
		if( reason==RL_MOUSE_UP && button==RL_MOUSE_RIGHT ){
			// Show a popup menu
			wxMenuLocal menu;
			if( !rli->IsRoot() ){
				menu.Append( 100, _T("Close this area"), _T("Close the background area") );
				menu.AppendSeparator();
			}
			// Append items we can create here
			wxArrayString as = GetAreaTypes( true );
			for( int ix=0; ix<(int)as.GetCount(); ix++ ){
		    	if( as[ix]==_T("---") )
		    		menu.AppendSeparator();
		    	else
		    		menu.Append( ix, as[ix], _T("Create an item in the background area") );
			}
			
			int sel_id = menu.PopupMenu( wnd );
			
			if( sel_id>=0 && sel_id<(int)as.GetCount() ){
				wxRlItem *rli_new = CreateArea(as[sel_id],wnd);
				if( rli_new )
					rli->Replace( rli_new );
			}
			else if( sel_id==100 ){
				if( is_float )
					// Destroy this window
					wnd->Destroy( );
				else
					rli->Destroy( true );
			}
			
			if( sel_id!=-1 )
				rl_mgr->Update();
		}
		break;
	
		case RlButton:			// Mouse event on button
			if( reason==RL_MOUSE_UP && button==RL_MOUSE_LEFT ){
				
				// Destroy of this window triggered from several places
				bool do_destroy = false;
				
				// The type of the button
				int type = RL_BUTTON_TYPE(rli);
				
				if( type==CAP_BUTTON_DOWN ){
					// Retrieve the pane
					wxRlItem *rli_ap = rli->GetParentWithType(RlAppPane);
					wxRlAppPane *ap = wxDynamicCast( rli_ap, wxRlAppPane );
					
					if( ap ){
						// Prepare a menu under the 'down' button
						wxMenuLocal menu;
					    wxArrayString as_types = GetAreaTypes(true);
					    
					    if( !is_float )
					    	menu.Append( 1, _T("Float pane"), _T("Float this pane") );
					    else
					    	menu.Append( 1, _T("Dock pane"), _T("Dock this pane") );
					    
						if( !ap->GetContents()->IsSpacer() )
							menu.Append( 2, _T("Close contents"), _T("Close contents, keep pane") );
						else{
							menu.AppendSeparator();
							// Append all the types we can create in here
						    for( int ix=0; ix<(int)as_types.GetCount(); ix++ ){
						    	if( as_types[ix]==_T("---") ){
						    		menu.AppendSeparator();
						    	}
						    	else{
							    	wxString s = _T("New content: ");
							    	s += as_types[ix];
							    	if( s.Find(_T("pane"))==wxNOT_FOUND )
							    		menu.Append( ID_MBAR_CREATE+ix, s, _("Create new content") );
							    	else
							    		// To skip the following separator
							    		ix++;
						    	}
						    }
						}
						menu.AppendSeparator();
						menu.Append( 3, _T("Close pane"), _T("Close the pane with contents") );
						
						// Find where to show menu
						wxRect r = rli->GetRect();
						int sel_id = menu.PopupMenu(wnd,wxPoint(r.x+r.GetWidth()/2,r.GetBottom()));
						
						if( sel_id==1 ){
							if( !is_float ){
								// Create a floating frame (based on wxMiniFrame)
								// We give it a pointer to our wxRlManager so that it knows where to dock
								FloatingFrame *fwnd = new FloatingFrame(wnd,rl_mgr);
								// This gives us a new wxRlManager managing just the pane we want to float
								wxRlManager *prlm = rl_mgr->Float( rli_ap, fwnd );
								fwnd->m_rl_mgr = prlm;
								
								// Now it's floating!
							}
							else{
								// Dock back on main window 
								// A bit of a hack, we use the global to trace back to our parent manager
								wxRlManager *parent_rl_mgr = wxRlManager::GetRlManagerOf( g_main_wnd );
								wxASSERT( parent_rl_mgr );
								parent_rl_mgr->Dock( rl_mgr );
								// And destroy our window
								wnd->Destroy();
							}
						}
						
						else if( sel_id==2 ){
							// Destroy content window 
							ap->DeleteWindows();
							ap->SetWindow( NULL );
						}
						
						else if( sel_id==3 )
							do_destroy = true;
						
						else if( sel_id>=ID_MBAR_CREATE && sel_id<ID_MBAR_CREATE+(int)as_types.GetCount() ){
							wxRlItem *rli = CreateArea( as_types[sel_id-ID_MBAR_CREATE], wnd );
							ap->SetContents( rli );
						}
					}
				}
			
				else if( type==CAP_BUTTON_CLOSE )
					do_destroy = true;
				
				if( do_destroy ){
					if( !is_float ){
						wxRlItem *rli_ap = rli->GetParentWithType(RlAppPane);
						if( rli_ap )
							// Destroy the whole pane
							rli_ap->Destroy( true );
					}
					else{
						// Destroy window
						// We call wxWindow::Destroy here, it will later on delete 
						// this object. In the destructor, we delete the wxRlmanager
						wnd->Destroy();
					}
				}
			
				// The layout has changed so update it from the top
				rl_mgr->Update();
			}
	}
}



//
// wxApp class
// 
class MyApp : public wxApp {
public:
	MyApp(){ }
	virtual ~MyApp(){ }

	virtual bool OnInit() {
		sigtry{
	        MyFrame *pmf = new MyFrame( _T("wxRecursiveLayout Sample") );
	        //pmf->Show( );
		}sigcatch(...){
			int z=3;
		}
        return true;
	}
	
    void HandleEvent(wxEvtHandler *handler, wxEventFunction func, wxEvent& event) const {
        static int st_in_dialog;
        sigtry {
            (handler->*func)(event);
        } sigcatch ( stSignalException *pse ) {
            if( !st_in_dialog++ ){
                wxMessageBox( wxString::Format(_T("Caught signal exception: %d"),pse->m_sig_no), _T("Main.cpp - Exception") );
                st_in_dialog--;
            }
        } catch(...) {
            wxASSERT( 0 ); // Not using exceptions
        }
    }
    
	DECLARE_EVENT_TABLE();
};


IMPLEMENT_APP(MyApp);

BEGIN_EVENT_TABLE(MyApp, wxApp)
END_EVENT_TABLE()



//
// A customized wxRlItem: A window with four subwindows. When one window has not 
// yet been set, it is background painted and offers a popup menu,
//
// We want to build a layout like this:
//
// wxRlQuadItem (fixed, don't modify after ctor)
// |
// -- Sizer1 (fixed, don't modify after ctor)
// |  |
// |  -- Window1
// |  |
// |  -- Window2
// |  
// -- Sizer2 (fixed, don't modify after ctor)
//    |
//    -- Window3
//    |
//    -- Window4
//
class wxRlQuadItem : public wxRlItem {
public:
	wxRlQuadItem( int flag=wxVERTICAL) : wxRlItem(new wxBoxSizer(flag&wxBOTH),RL_DEF_PROP,wxEXPAND,RlSizer) {
		// We (this) now holds a sizer. Create two child sizers in opposite direction in it.
		for( int r=0; r<2; r++ ){
			wxRlItem *rli_mid = &Insert( r, new wxBoxSizer((flag^wxBOTH)&wxBOTH) );
			
			// In each sizer, put a temporary item with background color
			for( int c=0; c<2; c++ ){
				wxRlItem *rli = new wxRlClientBgHint(_T("Right click menu"),10); 
				rli_mid->Insert( c, rli, RlPaneEdge );
				// This item reports mouse events and is fix
				SetCapabFor( rli, RL_CAPAB_FIX | RL_CAPAB_MOUSE_REPORT );
				m_contents[r*2+c] = rli;
			}
			// The middle level item is fix
			SetCapabFor( rli_mid, rli_mid->GetCapab() | RL_CAPAB_FIX );
		}
		
		// Change capability for this item (setting the fix bit is like locking it
		// from future modifications),
		m_capab |= RL_CAPAB_FIX | RL_CAPAB_ENTITY;
	}
	
	// Can be used from outside to set a window
	bool SetWindow( int ix, wxWindow *wnd ){
		if( ix<0 || ix>3 ) return false;
		// This will restore empty background area if NULL
		m_contents[ix]->SetWindow( wnd );
		if( !wnd ){
			// When setting the window our type changes to RlWindow 
			// (but the c++ object stays the same). Change it back
			// to the background item now. Also verify that C++ type is OK.
			if( !wxDynamicCast(m_contents[ix],wxRlClientBgHint) ){
				wxRlItem *rli = new wxRlClientBgHint(_T("Right click menu"));
				if( !m_contents[ix]->Replace(rli).IsOk() ){
					delete rli;
					return false;
				}
				m_contents[ix] = rli;
			}
			ModifyTypeFor( m_contents[ix], RlClientBgHint );
		}
		
		return true;
	}
		
	// Come here on mouse events
    virtual void RlNotify( wxRlItem *rli_hit, int mouse, const wxPoint &pt ){
    	// Only check for right mouse button up
    	// Do not pass notification upwards, we are an entity!
    	if( RL_REASON(mouse)==RL_MOUSE_UP && RL_BUTTON(mouse)==RL_MOUSE_RIGHT ){
    		// Which item is it?
    		int ix_hit;
    		for( ix_hit=0; ix_hit<4; ix_hit++ )
    			if( m_contents[ix_hit]==rli_hit )
    				break;
    		wxASSERT( ix_hit<4 );
    		
    		// Show a mneu of windows to create
    		wxMenuLocal menu;
			wxArrayString as = GetAreaTypes( false, true );
			for( int ix=0; ix<(int)as.GetCount(); ix++ )
		    	if( as[ix]==_T("---") )
		    		menu.AppendSeparator();
		    	else
		    		menu.Append( ix, as[ix], _T("Create an item in the sub window") );
			int sel_id = menu.PopupMenu( GetParentWindow() );
			
			// Create a window and show it in the right content pane
			if( sel_id>=0 ){
				// Create the area and copy flags & proportion from it
				wxRlItem *rli = CreateArea(as[sel_id]);
				SetWindow( ix_hit, rli->GetWindow() );
				m_contents[ix_hit]->CopyGeometryFrom( rli );
				// The item just used to hold the window momentarily
				// Note: It has not been attached to the layout so can be safely deleted 
				delete rli;
				
				wxRlManager *rl_mgr = wxRlManager::GetRlManagerOf( this );
				wxASSERT( rl_mgr );
				// We insert a window at a low level in the layout. 
				// It can have effects at higher levels as well. Update fully.
				rl_mgr->Update( );
			}
    	}
    }
	
protected:	
	wxRlItem *m_contents[4];
    
    // The following three functions implement 'container' semantics that is 
    // used when differetiating between layout and content parts. Without
    // these functions the class can be content, but it cannot itself be 
    // part of the layout and give access to its contents. With the flag 
	// RL_CAPAB_ENTITY set, the wxRlManager will know that this is an 
	// independent group and keep it intact during layout changes.
    /*
public:	
	// A user can use this to set the contents to something different than a window
	virtual bool SetContents( wxRlItem *rli, int ix ){
		if( !rli->IsOk() ) return false;
		// -1 means look for first empty slot
		if( ix==-1 ){
			// Find an unused item
			for( ix=0; ix<4; ix++ )
				if( RL_IS_BG_AREA(m_contents[ix]->GetType()) )
					break;
		}
		if( ix<0 || ix>=4 ) return false;
		return m_contents[ix]->Replace(rli).IsOk();
	}

	
	// A user can use this to set the contents to something different than a window
	virtual wxRlItem *GetContents( int ix ){
		return ix>=0 && ix<4 ? m_contents[ix] : NULL;
	}

	// This will detach a content item (used during loading/saving layouts)
	virtual wxRlItem *DetachContents( int ix ){
		if( ix<0 || ix>3 ) return NULL;
		wxRlItem *rli = new wxRlClientBgHint(_T("Right click menu"),10);
		wxRlItem *rli_detach = m_contents[ix]; 
		if( m_contents[ix]->ReplaceDetach(rli).IsOk() ){
			m_contents[ix] = rli;
			return rli_detach;
		}
		else{
			delete rli;
			return NULL;
		}
	}
	*/
};





// A list of areas/windows we can create (for menus)
wxArrayString GetAreaTypes( bool only_fg, bool only_window ){
	wxArrayString as;
	if( !only_window ){
		as.Add( _T("New pane") );
		as.Add(_T("---") );
	}
	as.Add( _T("Demo window - details") );
	as.Add( _T("Demo window - simple") );
	as.Add( _T("Demo window - fix prop.") );
	as.Add( _T("Demo window - no expand") );
	if( !only_window ){
		as.Add(_T("---") );
		as.Add( _T("Quad area") );
	}
	//as.Add( _T("HTML window") );
	if( !only_fg && !only_window ){
		as.Add(_T("---") );
		as.Add( _T("Plain background") );
		as.Add( _T("Background with hint") );
	}
	return as;
}


// Create named item
wxRlItem *CreateArea( wxString type, wxWindow *parent ){
	if( type==_T("New pane") ){
		static int pane_ix;
		return new wxRlAppPane(wxString::Format(_T("Pane - %d"),pane_ix++), CAP_BIT_BUTTON_DOWN|CAP_BIT_BUTTON_CLOSE);
	}
	
	// We can create ordinary wxWindows and put inside wxRlItems
	wxWindow *wnd = CreateWindow( type, parent );
	if( wnd ){
		wxRlItem *rli = new wxRlItem(wnd, RL_DEF_PROP, wxEXPAND, RlWindow); 
		if( type.Find(_T("fix prop"))!=wxNOT_FOUND )
			rli->SetProportion( 0 );
		if( type.Find(_T("no expand"))!=wxNOT_FOUND )
			rli->SetFlag( rli->GetFlag()&~wxEXPAND );
		return rli;
	}

	if( type==_T("Quad area") )
		return new wxRlQuadItem();
	
	if( type==_T("Plain background") )
		return new wxRlItem(RlClientBg);
	if( type==_T("Background with hint") )
		return new wxRlClientBgHint(_T("Click here with the right mouse button for a menu"));
	
	return NULL;
}

wxWindow *CreateWindow( wxString type, wxWindow *parent ){
	wxWindow *wnd = NULL;
	if( !parent ) parent = g_main_wnd;
	if( type==_T("Demo window - details") )
		wnd = new wxFixSizeWnd(parent);
	if( type==_T("Demo window - simple") )
		wnd = new wxFixSizeWnd(parent,*wxGREEN,*wxWHITE,_T("Resizable"));
	if( type==_T("Demo window - fix prop.") )
		wnd = new wxFixSizeWnd(parent,*wxBLUE,*wxWHITE,_T("Fixed proportion"));
	if( type==_T("Demo window - no expand") )
		wnd = new wxFixSizeWnd(parent,*wxBLUE,*wxLIGHT_GREY,_T("No expand"));

	if( wnd ) wnd->SetMinSize(wxSize(60,60));
	
	return wnd;
}



