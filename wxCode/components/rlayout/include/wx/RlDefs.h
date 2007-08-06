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


#ifndef RL_DEFS_H
#define RL_DEFS_H

//
// Flag values for the m_capab member of wxRlItem - Controls several aspects of behavious
// This field is constant once the item has been created.
//
#define RL_CAPAB_MOUSE_REPORT			0x0001  // Item reports mouse notfications upwards
#define RL_CAPAB_MOUSE_PROPAGATE 		0x0002  // Item propagates mouse reports upwards
#define RL_CAPAB_MOUSE_WXEVENT			0x0004  // Generates mouse event to containing window
#define RL_CAPAB_REPAINT_ON_ENTER_LEAVE 0x0008  // Item wants to be repainted when mouse enters or leaves
#define RL_CAPAB_DISCARD_ON_SAVE		0x0100	// Discard this item on saving XML
#define RL_CAPAB_TEMP_BG				0x0200	// Item is temporary background
#define RL_CAPAB_ENTITY	            	0x1000	// This is an entity. Internals of it should not be treated as part of layout
#define RL_CAPAB_FIX					0x2000	// The item (a sizer) cannot be inserted/removed from 
												// It gives a way of 'fixing' a layout
#define RL_CAPAB_GRIPPER				0x4000  // Item is a gripper, (resizes its neighbours)
//#define RL_CAPAB_HOVER 			    		// Item is 'hoverable'
//#define RL_CAPAB_SNAP_TO_BORDER			    // Item should be aligned to the border of the sizer (read window)              


// These stay the same even if item changes type
#define RL_CAPAB_FIX_MASK	(RL_CAPAB_ENTITY|RL_CAPAB_FIX)
// # Enumerable and Edge could be a capab flag ?


//
// Flag values for the m_state member of wxRlItem 
// Dynamic, changes during lifetime.
//
//#define RL_STATE_SELECTED	0x0001		// If an item is 'user selected'
#define RL_STATE_HOVER		0x0002		// Mouse hover flag
#define RL_STATE_MOUSE_OVER	0x0004		// Mouse is over item
#define RL_STATE_ACTIVE		0x0008		// The RlItem is active (has focus)

#define RL_STATE_APP1		0x1000		// Flag used by derived class
#define RL_STATE_APP2		0x2000		// Flag used by derived class
#define RL_STATE_APP3		0x4000		// Flag used by derived class
#define RL_STATE_APP4		0x8000		// Flag used by derived class


//
// These are the different elements that make up the RL layout
// Each wxRlItem object has a member saying it's type (below enums)
//
enum RlUiArea {
    RlUnSpeced = -1,     // Unspecified, check dynamically
    RlNoPart = 0,       // Invalid part

    // Atomic ones
    RlPaneEdge = 1,
    RlMediumEdge,
    RlThinEdge,
    RlToolBarEdge, 		// For moving toolbars internally
    RlLayoutEdge,		// For dialog borders
    RlCustomEdge1,
    RlCustomEdge2,

    RlFrameBg=20,
    RlClientBg,
    RlClientBgHint,
    RlClientTempBg,
    RlPaneMoveHint,
    RlBgLast,	// Last background item

    RlButton=40,

    RlSpacer=50,
    RlSpacerTemp,		// Transparent temporary item
    RlSpacerEnum,		// Enumerable space (will have edges around it)
    RlSpacerUnknown,    // For auto created spacers

    RlCaptionText=70,
    RlToolGripper,
    RlLayoutGripper,
    RlTabItem,
    
    RlText = 80,		// A generic text item 
    
    RlLastSpacer=99, 

    // For adding windows
    RlWindow = 100,
    RlToolWindow = 101,	// For toolbars and similar side controls 

    // These are the sizer/dock types
    RlSizer = 110,           // For the wxCBoxSizer, edges are dragged correctly
    RlToolbarDock,          // Can hold several toolbars or tool windows
    RlUserArea,             // Inside an app pane, this is where the window or app subsizer goes
    RlStaticBoxSizer,       // for dialogs
    RlSizerUnk,         	// Generic, for any non specialized sizer

    // These are compound ones
    //RlCaptionButtons = 100,
    RlCaption = 130,
    RlAppPane,
    RlToolPane,             // A draggable tool window
    RlTabBar,
    RlNoteBook,
    
    RlPaneContent=150,		// The content of a pane - can be window, sizer or space 
    
    RlLastSizer = 199,

    // This is an invalid item (returned when insert/move/... operation fails
    RlNull=999
};

// Convenience defines for dealing with types of items
#define RL_IS_KNOWN_EDGE(rl_type) (rl_type>=RlPaneEdge && rl_type<=RlCustomEdge2)
//#define RL_IS_TEMP_ITEM(rl_type) (rl_type==RlClientTempBg || rl_type==RlSpacerTemp)
#define RL_IS_EMPTY_SPACER(rl_type) (rl_type>=RlSpacer && rl_type<=RlSpacerUnknown)
#define RL_IS_BG_AREA(rl_type) (rl_type>=RlFrameBg && rl_type<=RlBgLast)


//
// Default sizes, proportions, ... when creating items from XML
//
#define RL_DEFAULT_PROPORTION       10000
#define RL_DEF_PROP  			    RL_DEFAULT_PROPORTION      // Save some typing

#define RL_DEF_EMPTY_SPACE_PROP	    (RL_DEF_PROP/10)
#define RL_DEF_EMPTY_SPACE_SIZE     5

#define RL_DEF_WINDOW_BORDER        0		// 5
#define RL_DEF_WINDOW_FLAGS         (wxLEFT|wxRIGHT|wxTOP|wxBOTTOM)
#define wxGEOMETRY_MASK             (wxEXPAND|wxBOTH|wxFIXED_MINSIZE)

// Macros to set/clear a flag bit
#define RL_SET_FLAG(flag,mask,do_set) flag=((flag&~mask)|(do_set?mask:0))


// Used on floating windows for a small visual offset
#define RL_FLOAT_OFFSET_X	20		
#define RL_FLOAT_OFFSET_Y   40

#define RL_WINDOW_MIN_SIZE	60



//
// These flags are used in reporting mouse events
//
#define RL_MOUSE_DOWN	1
#define RL_MOUSE_UP		2
#define RL_MOUSE_MOVE	3
#define RL_MOUSE_HOVER  4		// When mouse has stayed long enough over item
#define RL_MOUSE_ENTER  5		// Mouse enters item
#define RL_MOUSE_LEAVE  6		// Mouse leaves item
#define RL_MOUSE_REASON_MASK 0x07
#define RL_MOUSE_LEFT	0x10
#define RL_MOUSE_MID	0x20
#define RL_MOUSE_RIGHT	0x30
#define RL_MOUSE_BUTTON_MASK  0x30

#define RL_REASON(mouse) (mouse&RL_MOUSE_REASON_MASK)
#define RL_BUTTON(mouse) (mouse&RL_MOUSE_BUTTON_MASK)

//
// Used when reporting 'service' upwards from an wxRlItem 
//
#define RL_REQUEST(reason)  (reason&RL_REQUEST_MASK)
#define RL_REQUEST_MASK     0x00FF0000
#define RL_REQUEST_REPAINT	0x00010000	// An items asks to be repainted
#define RL_REQUEST_CAPTURE	0x00020000	// An item wants mouse capture


//
// Button indeces
//
#define CAP_BUTTON_CLOSE    0
#define CAP_BUTTON_DOWN     1
#define CAP_BUTTON_MENU     CAP_BUTTON_DOWN
#define CAP_BUTTON_LEFT     2
#define CAP_BUTTON_RIGHT    3
#define CAP_BUTTON_UP       4
#define CAP_BUTTON_INC_SIZE 5
#define CAP_BUTTON_DEC_SIZE 6

// NOTE: We use bit value (below) when constructing panes and caption buttons
// In callbacks we use index value (above)
// Button bits 
#define CAP_BIT_BUTTON_CLOSE    (1<<CAP_BUTTON_CLOSE)
#define CAP_BIT_BUTTON_DOWN     (1<<CAP_BUTTON_DOWN)
#define CAP_BIT_BUTTON_MENU     (1<<CAP_BUTTON_MENU)
#define CAP_BIT_BUTTON_LEFT     (1<<CAP_BUTTON_LEFT)
#define CAP_BIT_BUTTON_RIGHT    (1<<CAP_BUTTON_RIGHT)
#define CAP_BIT_BUTTON_UP       (1<<CAP_BUTTON_UP)
#define CAP_BIT_BUTTON_INC_SIZE (1<<CAP_BUTTON_INC_SIZE)
#define CAP_BIT_BUTTON_DEC_SIZE (1<<CAP_BUTTON_DEC_SIZE)

// Macro to convert position to bit in that position
#define RLP2B(p) (1<<(p))


#endif // #define RL_DEFS_H

