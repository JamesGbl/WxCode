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

#ifndef RLLAYOUT_H
#define RLLAYOUT_H

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/rlayoutdef.h"

#include "wx/defs.h"
#include "wx/window.h"
#include "wx/dynarray.h"
#include "wx/timer.h"
#include "wx/sizer.h"
#include "wx/string.h"

#include "wx/RlDefs.h"     // Most defines for wxRlManager are in here
#include "wx/wxUtils.h"
//#include "wx/SizerUtils.h"

#include "utils/ExpArr.hpp"

// Forward declarations
class SizerIter;
class wxRlManager;


class WXDLLIMPEXP_RLAYOUT wxRlItem : public wxSizerItem {
    friend class wxRlManager;
    public:
        // A user interface part, gets its details from rlPainter (using type)
        wxRlItem( RlUiArea type=RlNoPart, int flag=0 );

        // Window
        wxRlItem( wxWindow *window, int proportion, int flag, int type, wxObject* userData=NULL );
        // subsizer, specified type
        wxRlItem( wxSizer *sizer, int proportion=RL_DEF_PROP, int flag=wxEXPAND, int type=-1, wxObject* userData=NULL );
        // subsizer, type dynamically determined
        //wxRlItem( wxSizer *sizer, int proportion, int flag, wxObject* userData=NULL );
        // Spacer 
        wxRlItem( int w, int h, int proportion=RL_DEF_EMPTY_SPACE_PROP, int flag=wxEXPAND, int type=RlSpacer, wxObject* userData=NULL );
        // Copy from existing wxSizerItem
        wxRlItem( wxSizerItem *si );

        virtual ~wxRlItem( );

        RlUiArea GetType();
        const wxChar *Type2Str( );

        //
        // Basic functions to handle its area
        //
        
        // The item paints itself to given DC
        virtual void Render( wxDC *dc );
        
        // Mouse handler for item - called only for spacers (bottom most elements)
        virtual void MouseEvent( int mouse, const wxPoint &pt ); 

        // Mouse handler propagator - Use the message, drop it or propagate it upwards 
        virtual void RlNotify( wxRlItem *rli_hit, int mouse, const wxPoint &pt ); 


        // Used in call to wxRlItem::ToXml
        #define RL_XML_ENUMERABLE       1
        #define RL_XML_VERBOSE          2   // Meaning human readable
        #define RL_XML_ALL_DETAILS      4   // Get max details
        #define RL_XML_FULL_RECURSION   8   // Go into parts that are 'entities' 
        
        // Generate XML to describe this item and children recursively
        bool ToXml( wxString &xml, int options, int rec_lvl=0 );
        // The 'Config' bit below is interior XML
        // During creation, further configure from XML - used to setup properties not in base class
        virtual bool ConfigFromXml( const char *&xml ){ return true; }
        // During save, append custom XML
        virtual wxString ConfigToXml( ){ return wxEmptyString; }
        
        //
        // These are functions for managing subnodes, creating, deleting
        // and moving panes and objects
        //

        // Goes to sub item nr index:th. Negative indices traverses from the end.
        wxRlItem &Part( int index );
        
        // return index of this item in parent sizer
        int IndexOf(  );

        // Goes to the parent item
        wxRlItem &Up();

		// Give numeric address of this item: "1,3,2"
		wxString GetAddress(  ){ return m_parent ? m_parent->AddressOf(this) : _T(""); }

		// Return item referred to by address, start indexing in root
		// Call from the root if the address was created relative the root
        wxRlItem &FindAddress( const wxChar *addr );

        // Inserts an item into a sub sizer, or creates a new one if there
        // isn't one. The old item goes into the 1st position of the new sizer
        wxRlItem &Insert( int pos, wxRlItem *item, RlUiArea edge=RlUnSpeced );
        wxRlItem &Insert( int pos, wxWindow *window, int prop=0, int flag=wxEXPAND, RlUiArea edge=RlUnSpeced );
        wxRlItem &Insert( int pos, wxSizer *sizer, int prop=RL_DEF_PROP, int flag=wxEXPAND, RlUiArea=RlUnSpeced );

        // Creates a new sub sizer around the current object. 
        // Returns the new enclosing object 
        wxRlItem &Enclose( int flag=wxEXPAND, bool take_over_border=true );

        // Detaches the current item from the layout
        wxRlItem &Detach( ){ return DetachInternal(true); }

        // Deletes the current item (NOTE: wxWindows can be deleted also)
        bool Destroy( bool destroy_wnd=false );

        // Deletes the current item and replaces with new item
        wxRlItem &Replace( wxRlItem *new_item, bool destroy_wnd=false );

        // Replace the current item without deleting it
        wxRlItem &ReplaceDetach( wxRlItem *new_item );
        
		// Inserts an item at the given address
		bool InsertAt( wxRlItem *rli, const wxChar *addr );

		
        // To check that it really is of this type
        bool IsRlItem();

        // Returns true for RlItem and false for others, including the error object (g_rl_null)
        bool IsOk();
        bool IsRoot(){ return !m_parent; }

        // Only app level items are enumerable
        virtual bool IsEnumerable( );
        static bool IsEnumerable( int type );
        
        bool     IsChild( wxRlItem *rli );
        virtual bool IsUserSizer( );

        wxRlItem *GetParent(){ return m_parent; }
        wxWindow *GetParentWindow( );
        wxRlItem *GetParentWithType( RlUiArea type );
        wxRlItem *GetParentWithCapab( int capab );
        wxRlItem *GetChildWithType( RlUiArea type, bool also_self=true, bool l2r=true );

		// Find best place to insert new frame
		wxString FindInsertionAddress( wxPoint &pt, wxRlItem *rli ); 
        wxRlItem *FindItemAt( const wxPoint &pt );

        // Go through the layout of this item (and subitems), check relationships 
        // and possibly simplify.
        void UpdateItem( int has_bg=-1 );
                
        int  GetCapab(){ return m_capab; }
        int  GetState(){ return m_state; }
        void SetState( int state );
        
        // An entity item forms a border between outside and inside in a layout
        // One cannot drag & drop items into other 'entities' 
        void SetIsEntity();
        
        virtual const wxChar *GetError( ){ return _T("wxRlItem: No error"); }

        // This will also set/clear the wxFIXED_MINSIZE flag
		wxRlItem &SetProportion( int prop );
		
		// Copy size, proportion & size related flags from the item
		void CopyGeometryFrom( wxRlItem *rli );

        // These are used by compound items, (such as RlAppPane) to set interior
		// child parts, not necessarily on the first level.  
        // Only items with RL_CAPAB_ENTITY should implement these.
        virtual bool SetContents( wxRlItem *rli, int ix=0 );
        virtual wxRlItem *GetContents( int ix );
        virtual wxRlItem *DetachContents( int ix );
        
        // These methods will free & delete the previous contents (except for windows)
		void SetWindow( wxWindow *wnd );
		void SetSizer( wxSizer *sizer, int type );	
		void SetSpacer( const wxSize &size, int type );

        // These methods will NOT free the previous contents 
		void ReplaceWith( wxWindow *wnd );
		void ReplaceWith( wxSizer *sizer, int type=-1 );
		void ReplaceWith( const wxSize &size, int type );
		
		// Print layout to stdout
		// 1 - Recursive to RLCAPAB_ENTITY
		// 2 - Full recursive
		// 0 - This level only
		// -1 - Only item 
		void Dump( int rec );	 
		
		int First( int ); 
		int Next( int ix );
		wxRlItem *First( wxRlItem * );
		wxRlItem *Next( wxRlItem *rli );
		bool IndexValid( int ix );
		bool IndexValid( wxRlItem *rli );
		wxRlItem *Elem( int ix );
		wxRlItem *Elem( wxRlItem *rli ){ return rli; }
		
    protected:
        void SetParent( wxRlItem *parent ){ m_parent=parent; }
        
        void SetType( int type );		// This sets m_capab based on type
        void ModifyTypeFor( wxRlItem *rli, int type );	// This does not change m_capab
		bool SetCapabFor( wxRlItem *rli, int capab );
        void Init( int type );
        void AdjustItemOrientation( );

        wxRlItem &DetachInternal( bool detach_final );
        static wxRlItem *ConvertToRlItem( SizerIter &sit, wxSizerItem *si, wxSizerItem *node );
        wxString CheckLayout( );

        virtual bool GetXmlAttribs( wxString &xml, int options );

        // Recursively build address of item
        wxString AddressOf( wxRlItem *rli );
        
		// Find best place to insert new frame
		wxString FindInsertionAddress( wxPoint &pt, wxRlItem *rli, int &dbest ); 

		void RenderBorder( wxDC *dc );
		
		void ChildReleaseNoExpand( );
		void ChildReleaseNoProportion( );
				
		// Return true if this item is an empty node where content can be put
        bool IsEmptyContent();
        
        short m_type;
        short m_type_check;
        
        short	m_state;	// Extra flags, public - dynamic
        short	m_capab;	// Extra features - static for type
        
        wxRlItem *m_parent;
        
    	DECLARE_CLASS(wxRlItem);
    	/*
		// These methods are the same as in wxSizerItem, they return a reference to the object though
		wxRlItem &SetFlag( int flag ){ wxSizerItem::SetFlag(flag); return *this; }
		wxRlItem &SetBorder( int border ){ wxSizerItem::SetBorder(border); return *this; }
		*/
    	
    friend wxRlItem *wxRlXmlLoad( const char*& xml, wxWindow *prnt_wnd, wxRlItem *prnt_rli, int opts );    	
};

// Returns the type of the item
WXDLLIMPEXP_RLAYOUT RlUiArea GetTypeOf( wxRlItem *rli );


// A structure for storing the contents of a wxRlNode while it is "un-hooked" 
// from a layout.
struct WXDLLIMPEXP_RLAYOUT wxRlContents {
	wxRlContents( const wxChar *name ) : m_name(name), m_used(false) {  }
	~wxRlContents(  ){ m_rli_contents.DeleteAll(); } 
	
	bool				m_used;	// Indicates that some elements has been taken out
	wxString 			m_name;
	ExpArrP<wxRlItem*> 	m_rli_contents;
	// Operator used as predicate when searching for an item
	bool operator == (const wxChar *name){ return m_name==name; }
};

// A structure to store a collection of wxRlContents
class WXDLLIMPEXP_RLAYOUT wxRlContentStore {
public:
	wxRlContentStore(){ }
	~wxRlContentStore(){ m_contents.DeleteAll(); }
	
	ExpArrP<wxRlContents*> m_contents;
};



// Forward declarations
class RlPainter;
class wxRlAppPane;
class wxRlManager;
struct wxRlDragPaneInfo;
struct wxRlDockInfo;


// Different actions in the wxRlManager
enum RlAction {
        RlNoAction = 0,
        RlMouseDragEdge,
        RlMouseDragPane,
        RlMouseDragWindow,
        RlMouseDragToolBar,
};


// The frame/window layout manager
class WXDLLIMPEXP_RLAYOUT wxRlManager : public wxEvtHandler {

    public:
        wxRlManager(    wxWindow *wnd,
                        int init_sizer_flag=wxVERTICAL|wxEXPAND,
                        RlUiArea default_bg=RlClientTempBg );
        virtual ~wxRlManager();
        void Dtor();
        
		static wxRlManager *GetRlManagerOf( wxRlItem *rli );
		static wxRlManager *GetRlManagerOf( wxWindow *wnd );

		
		// Functions to recursively attach/detach contents of a layout
		bool DetachContents( wxRlContentStore &cs, wxRlItem *rli=NULL );
		bool AttachContents( wxRlContentStore &cs, wxRlItem *rli=NULL );
		bool VerifyNodeNames( wxRlItem *rli=NULL );
		
		// Functions from StateI - but we don't inherit interface
		bool GetState( wxString& state );
#ifndef RL_NO_STATE		
		bool SetState( const char *state, int opts=0, wxRlContentStore *pcs=NULL );	
	#if wxUSE_UNICODE==1
		bool SetState( const wxChar *state, int opts=0, wxRlContentStore *pcs=NULL );	
	#endif		
#endif
		
        virtual void SetWindow(wxWindow *wnd);
        wxWindow *GetWindow(){ return m_wnd; }
        wxRlItem &GetRoot(){ return *m_root; }
        void SetRoot(wxRlItem *sizer_item);
        void SetRoot(wxSizer *sizer, int flag);
        void Update( );
        void SetDirty( ){ m_did_init_layout=false; }	// After inserting manually with wxRlItem, must call here
        void AdjustWindowMinSize();

        void SetActivePane( wxRlAppPane *pane, bool app_is_active=true );
        bool SetActive( wxRlItem *item, bool app_is_active, bool gen_evt );
        wxRlItem *GetActive(  ){ return m_active; }
        
        // # These should probably all be static 
        wxSizer *FindSizer( const wxChar *name );
        wxRlItem *FindItem( const wxChar *name );
        wxRlAppPane *FindAppPane( const wxChar *name );
        wxWindow *FindToolBar( const wxChar *name );
        //wxRlItem *FindRlItemOf( wxSizer *sizer );
        wxRlItem *FindRlItemOf( wxWindow *wnd );
        wxRlItem *FindItemAt( const wxPoint &pt );
        
		wxRlManager *FloatPane( const wxChar *name, wxWindow *wnd=NULL, wxPoint offset = wxPoint(RL_FLOAT_OFFSET_X,RL_FLOAT_OFFSET_Y) );
		wxRlManager *Float( wxRlItem *rli, wxWindow *wnd=NULL, wxPoint offset = wxPoint(RL_FLOAT_OFFSET_X,RL_FLOAT_OFFSET_Y) );
		bool Dock( wxRlManager *prlm, wxString addr=wxEmptyString, wxPoint pt_dock=wxDefaultPosition );
        static void SetPainter( RlPainter *painter );
        static RlPainter* GetPainter();

		// Can refresh sizer from outside. If pdc==NULL, DC is created automatically
        void RenderSizer( wxDC *pdc, wxSizer *node );

        void ItemTypeHide( RlUiArea rl_type ){
        	if( !m_hidden.Has(rl_type) ) m_hidden.Push(rl_type);
        }

        void ItemTypeShow( RlUiArea rl_type ){
        	m_hidden.RemoveUnordered(rl_type);
        }
        bool ItemTypeIsShown( RlUiArea rl_type ){
        	return !m_hidden.Has(rl_type);
        }
        bool ItemHideAll( bool hide ){
        	m_no_paint = hide;
        	if( m_wnd ){
        		// Force refresh
        		m_wnd->Refresh();
        	}
        }
        bool GetHideAll( ){ return m_no_paint; }
        bool IsDialog( ){ return m_is_dialog; }
        
        void DetachNotify( wxRlItem *rli );
        void DeleteNotify( wxRlItem *rli );
        
        // For ExpArrP find operation
        bool operator == (const wxRlItem *rli){ return m_root==rli; }
        bool operator == (const wxWindow *wnd){ return m_wnd==wnd; }

    protected:
        virtual void RenderSizerItem( wxDC *pdc, wxSizerItem *node );
        
        void MouseEvtSizerItem( wxRlItem *item );
        bool MouseEvtSizer( wxSizerItem *item_node );

        void InitRootItem( int init_sizer_flag, RlUiArea default_bg=RlClientTempBg );

        // To get DC we can paint also in dialog, where we have to paint over wxStaticBox
        virtual wxDC *GetClientDc( wxRlItem *rli );	
        
        // Put down pane we're dragging
		void StopDragPane( );

		void DoSetActive( wxDC *pdc, wxRlItem *rli, bool active );
		
        wxRlItem  *m_root;      // The root item of the recursive description
        wxWindow  *m_wnd;       // The window we manage
        //int       m_flags;

        wxMouseEvent &m_mouse_evt;
        wxPoint      m_mouse_last;
        wxPoint		 m_mouse_set_cursor;
        //wxPoint      m_mouse_drag_offset;
        wxRlItem     *m_mouse_over;

        wxRlDragPaneInfo *m_pane_drag_info;
        
        RlAction     m_mouse_action;
        wxRlItem     *m_action_node;
        wxRlItem  	 *m_action_items[3];
        wxWindow 	 *m_wnd_cursor;
        //wxWindow     *m_wnd_drag;
        wxTimer		m_timer_cursor;
        wxTimer		m_timer_drag;
        wxTimer		m_timer_debug;
        wxTimer		m_timer_leave;

        bool 		m_did_init_layout;
        wxRlItem    *m_active;
        bool		m_app_is_active;	// If so, hilight caption for active pane
        bool		m_active_from_event;
        bool 		m_is_dialog;
        bool		m_no_paint;

		wxCursor	m_cursor_drag_edge_v;	// # Static!
		wxCursor	m_cursor_drag_edge_h;
		wxCursor	m_cursor_drag_pane;

		ExpArr<RlUiArea,short> m_hidden;
		
		wxRlDockInfo *m_pdock_info;
		
        static RlPainter *ms_painter;

        // protected events
        void OnPaint(wxPaintEvent& evt);
        void OnEraseBackground(wxEraseEvent& evt);
        void OnSize(wxSizeEvent& evt);
        // void OnSetCursor(wxSetCursorEvent& evt);
        void OnLeftDown(wxMouseEvent& evt);
        void OnLeftUp(wxMouseEvent& evt);
        void OnRightDown(wxMouseEvent& evt);
        void OnRightUp(wxMouseEvent& evt);
        void OnMotion(wxMouseEvent& evt);
        void OnMouseCaptureLost(wxMouseCaptureLostEvent& evt);
        void OnLeaveWindow(wxMouseEvent& evt);
        //void OnEnterWindow(wxMouseEvent& evt);
        void OnChildFocus(wxChildFocusEvent& evt);
        void OnScroll( wxScrollWinEvent &evt );
        void OnTimer( wxTimerEvent& event );
	    void OnActivate( wxActivateEvent &event );
	    void OnIdle( wxIdleEvent &event );
    DECLARE_EVENT_TABLE()
    
    DECLARE_CLASS(wxRlManager);
    
    friend class wxRlItem;	// Simplifies things
};


// The events exported by wxRlManager
BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_LOCAL_EVENT_TYPE(wxEVT_RL_PANE_ACTIVATE, -1)
	DECLARE_LOCAL_EVENT_TYPE(wxEVT_RL_MOUSE, -1)
END_DECLARE_EVENT_TYPES()

typedef void (wxEvtHandler::*wxRlPaneActivateEventFunction)(wxCommandEvent&);
#define wxRlPaneActivateEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxRlPaneActivateEventFunction, &func)

typedef void (wxEvtHandler::*wxRlMouseEventFunction)(wxCommandEvent&);
#define wxRlMouseEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxRlMouseEventFunction, &func)


#define EVT_RL_PANE_ACTIVATE(func) \
   wx__DECLARE_EVT0(wxEVT_RL_PANE_ACTIVATE, wxRlPaneActivateEventHandler(func))

#define EVT_RL_MOUSE(func) \
   wx__DECLARE_EVT0(wxEVT_RL_MOUSE, wxRlMouseEventHandler(func))




//
// Macros to create fixed UI parts (they need no class on their own)
//
#define wxRlPaneEdge 		wxRlItem(RlPaneEdge)
#define wxRlMediumEdge 		wxRlItem(RlMediumEdge)
#define wxRlThinEdge 		wxRlItem(RlThinEdge)
#define wxRlFrameBg 		wxRlItem(RlFrameBg)
#define wxRlClientBg 		wxRlItem(RlClientBg)
#define wxRlClientTempBg 	wxRlItem(RlClientTempBg)
#define wxRlGripper 		wxRlItem(RlGripper)
#define wxRlSpacer(size,prop) wxRlItem(size,size,prop,wxEXPAND,RlSpacer)

// A macro for adding windows
//#define wxRlWindow(window, prop, flags) wxRlItem(window, prop, flags, RlWindow)


//
// All classes below derive from wxRlItem. They all have a specific function
// in the user interface and are by default visible and paint themseleves.
// They all add some data to the base class that is specific to them.
//
// They come in two categories
// 1 - Items that are spacers, i.e. they don't have any children.
//     This includes buttons and caption text.
// 2 - Items that are sizers and have a child hierarchy. Among them
//     are the full caption component, the application pane, and 
//     a container for toolbars
//


// Text item of caption
class wxRlCaptionText : public wxRlItem {
    public:
        wxRlCaptionText( const wxChar *str ) : wxRlItem(RlCaptionText) { m_caption=str; m_dc_width=0; }
        void SetActive( bool active=true ){ RL_SET_FLAG(m_state,RL_STATE_ACTIVE,active); }

        int GetTextWidthInPixels( ){ return m_dc_width; }
        wxString m_caption;
        int      m_dc_width;
        
    DECLARE_CLASS(wxRlCaptionText);
};

class wxRlButton : public wxRlItem {
    public:
        wxRlButton( int button, int flag=0 ) : wxRlItem(RlButton,flag) { m_button=button; }
        int m_button;
    DECLARE_CLASS(wxRlButton);
};
#define RL_BUTTON_TYPE(rli) (wxDynamicCast(rli,wxRlButton)?wxDynamicCast(rli,wxRlButton)->m_button:-1)


class wxRlClientBgHint : public wxRlItem {
    public:
        wxRlClientBgHint( const wxChar *hint=_T("Bg Hint"), int point_size=-1 ) : wxRlItem(RlClientBgHint) { 
        	m_hint = hint;
        	m_point_size = point_size;
        }
        
        virtual bool ConfigFromXml( const char *&xml );
        virtual wxString ConfigToXml( );
        
        wxString m_hint;
        int      m_point_size;
        wxFont   m_font;
    DECLARE_CLASS(wxRlClientBgHint);
};


class wxRlToolPane;

// The caption class. Used internally for wxRlAppPane
class wxRlCaptionSi : public wxRlItem {
    public:
        wxRlCaptionSi( const wxChar *str, int buttons=CAP_BIT_BUTTON_DOWN, int orient=wxHORIZONTAL );

        void SetCaption( const wxChar *str );
        const wxChar *GetCaption( );
        void AddButtons( int button, int pos=0 );
        void AddButtonSeparator( int size=2, int pos=0 );

    protected:
        void SetActive( bool active=true );
        void Init( const wxChar *str, int buttons, int orient );
        
        wxRlCaptionText *m_pc_txt;
        
        friend class wxRlManager;
        friend class wxRlAppPane;
        
    DECLARE_CLASS(wxRlCaptionSi);
};


// The pane class, inteface to the application
class WXDLLIMPEXP_RLAYOUT wxRlAppPane : public wxRlItem {

    public:
        wxRlAppPane( const wxChar *pane_name, int buttons=CAP_BIT_BUTTON_DOWN,
        			 int prop=RL_DEF_PROP, int flag=wxEXPAND );
        wxRlAppPane( const wxChar *pane_name, wxWindow *wnd, int buttons=CAP_BIT_BUTTON_DOWN,
        			 int prop=RL_DEF_PROP, int flag=wxEXPAND );

        void     SetCaption( const wxChar *str );
        wxString GetCaption( );

        void     SetWindow( wxWindow *wnd );
        wxWindow *GetWindow(  );
        
		// One way to set toolbar for the pane
        void     SetToolBar( wxWindow *ptb, bool horizontal=true );
        wxWindow *GetToolBar(  );

        const wxChar *GetName( );
        
        virtual bool SetContents( wxRlItem *rli, int ix=0 );
        virtual wxRlItem *GetContents( int ix=0 );
        virtual wxRlItem *DetachContents( int ix=0 );

        virtual bool ConfigFromXml( const char *&xml );
        virtual wxString ConfigToXml( );
        
    protected:
        void Init( const wxChar *name, int buttons, int prop, wxWindow *wnd );
        void SetActive( bool active=true );

    protected:
    	wxRlCaptionSi   *m_caption;
    	wxRlItem 		*m_contents;
    	wxRlToolPane    *m_tool_pane;
    	
    	bool m_toolbar_vert;
        
    	void FlipToolBarPane( );
        friend class wxRlManager;
        //friend class wxRlItem;
        
    DECLARE_CLASS(wxRlAppPane);
};


// Can currently contain one toolbar or other tool window
class WXDLLIMPEXP_RLAYOUT wxRlToolPane : public wxRlItem {
public:
	wxRlToolPane( const wxChar *name, int flag=wxHORIZONTAL|wxEXPAND,
					wxWindow *wnd=NULL, bool with_gripper=false );
	
	// Replaces whatever tool windows are already present.
	bool SetToolWindow( wxWindow *wnd );
	//bool AddToolWindow( wxWindow *wnd );
	//bool RemoveToolWindow( wxWindow *wnd );

    virtual bool SetContents( wxRlItem *rli, int ix=0 );
    virtual wxRlItem *GetContents( int ix=0 );
    virtual wxRlItem *DetachContents( int ix=0 );
	
    virtual bool ConfigFromXml( const char *&xml );
    virtual wxString ConfigToXml( );
    
protected:
	bool m_has_gripper;
    DECLARE_CLASS(wxRlToolPane);
};



// # wxRlText should go in its own file
#define RL_STATE_TEXT_FIT 		RL_STATE_APP1	// Text has been fitted to sizeritem
#define RL_STATE_TEXT_NO_MARGIN RL_STATE_APP2	// Don't se any margin

#define RL_TEXT_MAX_WIDTH   1024	// These are used in static bitmap to measure text extent
#define RL_TEXT_MAX_HEIGHT  128
#define RL_TEXT_MARGIN      3		// Margin to the left and right of text (background color fills this in)

class WXDLLIMPEXP_RLAYOUT wxRlText : public wxRlItem {
    public:
        wxRlText( const wxChar *str, 
        		  wxFont font=*wxNORMAL_FONT,
        		  wxColour fg_col=wxNullColour,
        		  wxColour bg_col=wxNullColour,
        		  int flag=0 );
        virtual ~wxRlText();

        void SetText( const wxChar *str, bool do_fit=true );
        void SetFont( wxFont font, bool do_fit=true );
        void SetFgCol( wxColour fg_col );
        void SetBgCol( wxColour bg_col );
        void SetSel( short start=0, short end=0 ){ m_sel_start=start; m_sel_end=end; } 

        const wxChar *GetText(){ return m_str; }
        const wxFont *GetFont(){ return m_pfont; }
        wxColour *GetFgCol(){ return m_pfg_col; }
        wxColour *GetBgCol(){ return m_pbg_col; }
        void GetSel( int& start, int& end ){ start=m_sel_start; end=m_sel_end; }
        
        virtual void Render( wxDC *dc );
        
    protected:
    	// Fits (minsize) of wxSizerItem to extent of text and sets proportion to 0 
    	void FitToText();
    	
        wxString m_str;
        wxFont   *m_pfont;
        wxColour *m_pfg_col;
        wxColour *m_pbg_col;
        short m_sel_start;
        short m_sel_end;
        
    DECLARE_CLASS(wxRlText);
};

#endif // RLLAYOUT_H

