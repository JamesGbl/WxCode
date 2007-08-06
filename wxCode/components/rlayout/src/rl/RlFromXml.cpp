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


// Turn off support for loading layouts with this flag
#ifndef RL_NO_STATE 

#include "wx/RlFromXml.h"
#include "wx/RlManager.h"
#include "wx/RlManagerInt.h"
#include "wx/wxUtils.h"

#if wxUSE_XRC==1
	#include <wx/xrc/xmlres.h>
	#include <wx/xml/xml.h>
	#include <wx/mstream.h>
	#include <wx/fs_mem.h>
#endif 

#include "utils/CharBuf.h"
#include "utils/StrUtils.h"
//#include "utils/utf8String.h"
#include "utils/XmlParse.h"


// Global XML resource used by RlWindow
wxXmlResource g_rl_xml_res;
CharBuf g_cb_last_msg;
const char *g_rl_xml_warning;
const char *g_rl_xml_error;


// Convenience function to return error state of parsing
wxRlItem *FromXmlError( const char *xml, const wxChar *err ){
	g_cb_last_msg = WX2MB(err);
	g_rl_xml_error = xml;
	wxLogMessage( err );
	return NULL;
}

// Convenience function to return error state of parsing
wxRlItem *FromXmlError( const char *xml, const wxChar *err, const wxChar *arg ){
	g_cb_last_msg = WX2MB(err);
	g_rl_xml_error = xml;
	wxLogMessage( err, arg );
	return NULL;
}

const char *wxRlXmlGetLast_ErrorXml(){
	return g_rl_xml_error;
}

const char *wxRlXmlGetLast_WarningXml(){
	return g_rl_xml_warning;
}

const char *wxRlXmlGetLast_Msg(){
	return g_cb_last_msg;
}



// Create a layout recursively from an XML description
wxRlItem *wxRlXmlLoad( const char*& xml, wxWindow *prnt_wnd, wxRlItem *prnt_rli, int opts ){
	
#if wxUSE_XRC==1
	static bool did_init;
	if( !did_init ){
		if( !wxFileSystem::HasHandlerForPath(_T("memory:file.htm")) )
			wxFileSystem::AddHandler( new wxMemoryFSHandler );
		g_rl_xml_res.InitAllHandlers();
		did_init = true;
	}
#endif
	
	// Basic check, init
	const char *pc_xml = xml; 	// To see it in the debugger
	if( !xml ) 
		return FromXmlError( xml, _T("RlItemFromXml, No input string") );
	xml = SkipSpace(xml);

	// Find end of tag
	const char *pend = strchr_nostr(xml,'>'); 
	if( !pend ) 
		return FromXmlError( xml, _T("RlItemFromXml, Parsing error, > not found") );

	// Find item type
	bool is_sizer = false;
	bool is_spacer = false;
	bool is_window = false;
	
	bool is_indent = false;
	bool is_text = false;
	bool is_empty_space = false;
	bool is_dialog = opts&RL_XMLLOAD_IS_DIALOG;//wxDynamicCast(prnt_wnd,wxDialog);
	
	// We accept these tags
	const char *pclose_tag = NULL;
	if( keywordieq(xml,"<sizer") ) is_sizer = true, pclose_tag = "</sizer>";
	else if( keywordieq(xml,"<indent") ) is_indent = is_sizer = true, pclose_tag = "</indent>";
	else if( keywordieq(xml,"<window") ) is_window = true, pclose_tag = "</window>";
	else if( keywordieq(xml,"<spacer") ) is_spacer = true, pclose_tag = "</spacer>";
	else if( keywordieq(xml,"<space") ) is_spacer = true, pclose_tag = "</space>";
	else if( keywordieq(xml,"<text") ) is_text = is_spacer = true, pclose_tag = "</text>";
	else
		return FromXmlError( xml, _T("RlItemFromXml, unknown tag: %s"), MB2WX(TruncStr(xml,40)) );
	if( is_spacer && !is_text ) is_empty_space = true;
	
	xml = SkipSpace(SkipToSpace(xml));
	
	// Read shared attribs (flag, prop, name, border, align, ...)
	DWORD flags = is_window ? (DWORD)RL_DEF_WINDOW_FLAGS : (DWORD)wxEXPAND;	
	if( is_text ) flags = 0; // No expand for text items
	
	int prop = is_window ? 0 : RL_DEF_PROP;
	if( is_empty_space ) 
		prop = RL_DEF_EMPTY_SPACE_PROP;
	
	// For SXRC there is no difference between size and minsize, there is really only minsize
	// For XRC internals, this can have meaning though.
	wxSize size(-1,-1);
	wxSize minsize(-1,-1);
	if( is_empty_space ) 
		minsize = wxSize(RL_DEF_EMPTY_SPACE_SIZE,RL_DEF_EMPTY_SPACE_SIZE);	
	
	int border = is_window ? RL_DEF_WINDOW_BORDER : 0;
	int rl_type=-1;
	int capab = -1;
	
	// With defaults as below, padding and margin corresponds to 
	// <space> if not adjusted by 'margin' or 'padding' attributes
	bool    has_padding = false;
	int     sz_padding = RL_DEF_EMPTY_SPACE_SIZE;
	int     prop_padding = RL_DEF_EMPTY_SPACE_PROP;
	
	bool    has_margin = false;
	int     sz_margin = RL_DEF_EMPTY_SPACE_SIZE;
	int     prop_margin = RL_DEF_EMPTY_SPACE_PROP;
	
	CharBuf tclass;	// The user class (for wrapped XRC)
	CharBuf name;   // ID of control
	CharBuf value;	// Value
	CharBuf type;	// Convenience type - for dialog items 
	CharBuf label;	// Label of control
	CharBuf srl_type;	// RlType as string
	CharBuf state;	// State flags (misc flags/state for wxWindow)
	CharBuf style; 	// Style flags
	
	CharBuf cb;		// Temp buffer
	
	// Flags
	FindAttrib( xml, "flag", (int&)flags, pend );
	if( FindAttrib(xml,"orient",cb,pend) ){
		if( cb.eqi("vert") ) flags |= wxVERTICAL;
		else if( cb.eqi("horz") ) flags |= wxHORIZONTAL;
		else{
			wxLogMessage(_T("RlItemFromXml: Unknown value for 'orient' atribute"));
			g_rl_xml_warning = xml;
		}
	}
	
	if( FindAttrib(xml,"expand",cb,pend) ){
		if( IsSet(cb) )
			flags |= wxEXPAND;
		else 
			flags &= ~wxEXPAND;
	}
	
	// Proportion
	if( FindAttrib(xml,"prop",cb,pend) ){
		if( cb.eqi("min") ) prop = 1;
		else{
			sscanf(cb,"%d",&prop);
			prop *= 100;
		}
	}
	
	// Border: border="5,LT,raised"
	const char *cp = keywordifind(xml,"border",pend);
	if( cp && (cp=StrAccept(cp,"=")) ){
		flags &= ~wxBORDER_MASK;
		if( isdigit(cp[1]) ) sscanf(cp+1,"%d",&border);
		const char *pq = strchr(cp+1,'\"');
		const char *pc = strchr(cp+1,',');
		const char *pc1 = pc ? strchr(pc+1,',') : NULL;
		if( !pc1 || pc1>pq ) pc1=pq;
		if( pc && pq && pc<pq ){
			// Left/west?
			if( strpbrk(cp,"wWlL")<pc1 ) flags|=wxLEFT;
			// Right/east?
			if( strpbrk(cp,"eErR")<pc1 ) flags|=wxRIGHT;
			// North/Top?
			if( strpbrk(cp,"nNtT")<pc1 ) flags|=wxTOP;
			// South/Bottom?
			if( strpbrk(cp,"sSbB")<pc1 ) flags|=wxBOTTOM;
		}
		// If nothing set, set all sides border
		if( !(flags&(wxBOTTOM|wxLEFT|wxTOP|wxRIGHT)) )
			flags |= wxBOTTOM|wxLEFT|wxTOP|wxRIGHT;
		// Do raised, sunken, simple, double, static
		if( keywordifind(cp,"raised",pq) ) flags |= wxRAISED_BORDER;
		if( keywordifind(cp,"simple",pq) ) flags |= wxSIMPLE_BORDER;
		if( keywordifind(cp,"static",pq) ) flags |= wxSTATIC_BORDER;
		if( keywordifind(cp,"sunken",pq) ) flags |= wxSUNKEN_BORDER;
		if( keywordifind(cp,"double",pq) ) flags |= wxBORDER_DOUBLE;
	}
	
	// Align
	cp = keywordifind(xml,"align",pend);
	if( cp && (cp=StrAccept(cp,"=")) ){
		flags &= ~wxALIGN_MASK;
		const char *pq = strchr(cp+1,'\"');
		if( pq ){
			// Left/west?
			if( keywordieq(cp+1,"left") ) int z=0;
			// Right/east?
			if( keywordieq(cp+1,"right") ) 
                flags|=wxALIGN_RIGHT;
			// North/Top?
			if( keywordieq(cp+1,"top") ) int z=0;
			// South/Bottom?
			if( keywordieq(cp+1,"bottom") ) 
                flags|=wxALIGN_BOTTOM;
			// Center? It is problematic to know if this is vert/horz centre
			// # Use simple wxCENTER flag
			if( keywordieq(cp+1,"center") || keywordieq(cp+1,"centre") ) 
                flags |= wxCENTER;
		}
	}
	
	// Size: size="40,30" 
	int w=-1, h=-1;
	if( FindAttrib( xml,"size",cb,pend) ){
		g_rl_xml_warning = xml;			
		sscanf(cb,"%d",&w);
		h = w;
		if( cp=strchr(cb,',') )
			sscanf(cp+1,"%d",&h);
		size.SetWidth(w); size.SetHeight(h);
	}
	
	// Size: minsize="20,10" 
	if( FindAttrib( xml,"minsize",cb,pend) ){
		sscanf(cb,"%d",&w);
		h = w;
		if( cp=strchr(cb,',') )
			sscanf(cp+1,"%d",&h);
		minsize.SetWidth(w); minsize.SetHeight(h);
		// Since this is min size, adjust size
		if( size.GetWidth()<w ) size.SetWidth(w);
		if( size.GetHeight()<h ) size.SetHeight(h);
	}
	
	// Name (...ID?)
	FindAttrib(xml,"name",name,pend);
	
	// Look for class (this is C++ class for XRC wxWindow items)
	FindAttrib(xml,"class",tclass,pend);

	// Look for RlItem type (type in wxRlItem)
	if( FindAttrib(xml,"rltype",srl_type,pend) ){
		if( isdigit(srl_type[0]) )
			sscanf(srl_type,"%d",&rl_type);
	}
	
	// Look for RL capability 
	if( FindAttrib( xml, "capab", (int&)capab, pend ) ){
		int z = capab; 
	}

	// Look for type (this is a convenience tag)
	FindAttrib( xml,"type",type,pend );
	
	// Look for label (used for static text and others)
	FindAttrib( xml,"label",label,pend );
	
	// Look for value (used for edit text and others)
	FindAttrib( xml,"value",value,pend );
	
	// Look for state (enabled/focused/hidden/default/...)
	FindAttrib( xml,"state",state,pend );

	// Look for style 
	FindAttrib( xml,"style",style,pend );

	// Has padding ?  format: padding="5,10" for 5 pixels and 10 in proportion
	if( FindAttrib(xml,"padding",cb,pend) ){
		has_padding = true;
		if( isdigit(cb[0]) )
			sscanf(cb,"%d",&sz_padding);
		cp = strchr(cb,',');
		if( cp ){
			if( !strnicmp(cp+1,"min",3) )
				prop_padding = 1;
			else{
				sscanf(cp+1,"%d",&prop_padding);
				prop_padding *= 100;	// Scale it up for internal use
			}
		}
	}

	// Has margin (only for indent)?  format: margin="5,10" for 5 pixels and 10 in proportion
	if( FindAttrib(xml,"margin",cb,pend) ){
		if( !is_indent )
			wxLogMessage(_T("RlItemFromXml, Warning: The margin attribute is only for the <indent ...> tag"));
		has_margin = true;
		if( isdigit(cb[0]) )
			sscanf(cb,"%d",&sz_margin);
		cp = strchr(cb,',');
		if( cp ){
			if( !strnicmp(cp+1,"min",3) )
				prop_margin = 1;
			else{
				sscanf(cp+1,"%d",&prop_margin);
				prop_margin *= 100;	// Scale it up for internal use
			}
		}
	}
	
	// Have collected generic tags, now create item
	wxRlItem *rli = NULL, *rli_base=NULL;	// base is the first level we create (if several levels)
	
	// Try sizer first
	if( is_sizer ){
		if( is_indent ||
			rl_type==RlSizer || rl_type==RlSizerUnk || (rl_type==-1 && !srl_type.Length()) ||
			srl_type=="RlSizer" || srl_type=="RlSizerUnk" || keywordifind(type,"staticbox") ) {
			if( !(flags&wxBOTH) ){
				if( !prnt_rli || !prnt_rli->GetSizer() )
					return FromXmlError( xml, _T("RlItemFromXml- Root sizer with neither vert/horz orientation.") );
				flags &= ~wxBOTH;
				// Defaulting to opposite of parent
				flags |= (GetOrientation(prnt_rli->GetSizer())^wxBOTH);
			}
			if( !keywordifind(type,"staticbox") ){
				wxBoxSizer *psz = new wxBoxSizer( flags&wxBOTH );
			    rli = new wxRlItem(psz,prop,flags, RlSizer);
			    
				if( is_indent ){
					// We now have a sizer that goes on the other direction,
					// Put a spacer and a sizer in the other direction 
					// and finally a spacer at the end
					rli_base = rli;	// The base item in our structure
					// A spacer
					wxRlItem *rli_space = new wxRlItem( sz_margin, sz_margin, prop_margin, flags, RlSpacerEnum );
					rli_space->m_capab |= RL_CAPAB_DISCARD_ON_SAVE;
					rli_base->Insert(0,rli_space,is_dialog?RlLayoutEdge:RlUnSpeced);
					// The sizer - this is what we build further on
					wxBoxSizer *psz = new wxBoxSizer( (flags&wxBOTH)^wxBOTH );
					rli = new wxRlItem(psz,RL_DEF_PROP,wxEXPAND,RlSizer);
					rli->m_capab |= RL_CAPAB_DISCARD_ON_SAVE;
					rli_base->Insert( 1, rli, is_dialog?RlLayoutEdge:RlUnSpeced );
					// A spacer again
					rli_space = new wxRlItem( sz_margin, sz_margin, prop_margin, flags, RlSpacerEnum );
					rli_space->m_capab |= RL_CAPAB_DISCARD_ON_SAVE;
					rli_base->Insert(2,rli_space,is_dialog?RlLayoutEdge:RlUnSpeced);
				}
			}
			else{
				wxBoxSizer *psz = new wxStaticBoxSizer( flags&wxBOTH, prnt_wnd, MB2WX(label) );
			    rli = new wxRlItem(psz,prop,flags, RlStaticBoxSizer);
			}
		}
		else if( rl_type==RlAppPane || srl_type=="RlAppPane" ){
			//wxRlAppPane_ *ap = new wxRlAppPane_(MB2WX(name),(wxWindow*)NULL);
		    //rli = new wxRlItem( ap, prop, flags, RlAppPane );
			rli = new wxRlAppPane(MB2WX(name),(wxWindow*)NULL);
		}
		else if( rl_type==RlToolPane || srl_type=="RlToolPane" ){
			rli = new wxRlToolPane(MB2WX(name),flags,(wxWindow*)NULL);
		}
		else
			return FromXmlError( xml, _T("RlItemFromXml - Unknown sizer type") );
		// ...Do more types later

		if( rli && rli->GetSizer() && name.Length() )
			// RlToolbarDock
			wxSizer2RlItem::SetName( rli, MB2WX(name) );
	}
	
	// All spacers
	else if( is_spacer ){
		if( rl_type==RlClientBg || srl_type=="RlClientBg" ){
			rli = new wxRlItem(RlClientBg,flags);
		}
		else if( rl_type==RlFrameBg || srl_type=="RlFrameBg" ){
			rli = new wxRlItem(RlFrameBg,flags);
		}
		else if( rl_type==RlClientTempBg || srl_type=="RlClientTempBg" ){
		    rli = new wxRlItem(RlClientTempBg,flags);
		}
		else if( rl_type==RlClientBgHint || srl_type=="RlClientBgHint" ){
		    rli = new wxRlClientBgHint();
		}
		else if( is_text ){
			// OK with both label and value
			const char *pc = value.Length() ? value : label;
			wxFont font = wxNullFont;
			wxColour fg = wxNullColour;
			wxColour bg = wxNullColour;
			// fg and bg attributes
			if( FindAttrib(xml,"fg",cb,pend) )
				fg.Set(MB2WX(cb));
			if( FindAttrib(xml,"bg",cb,pend) )
				bg.Set(MB2WX(cb));
			rli = new wxRlText( MB2WX(pc), font, fg, bg, flags );
		}
		else{
			// Generic transparent spacer
			// We make it enumerable for now
			// Use the size that has been defined, if both, minsize
			wxSize sz = minsize.GetHeight()>=0 ? minsize : size;
			rli = new wxRlItem( sz.GetWidth(), sz.GetHeight(), prop, flags, RlSpacerEnum );
		}
	}
	
	// All wxWindows
	else{
		wxASSERT(is_window);

// We rely on XRC here
#if wxUSE_XRC==1
		
		// Tags we're collecting for later (as user data)
		wxRlXmlTags *prxt = NULL;
		
		// We basically want the XRC system to handle things now
		// So we must create a local XML document with the data
		
		// Either we leave it all to XRC (using tclass) or we have a 
		// 'convenience tag' and we generate the internal XML here
		
		CharBuf s_ixml;	// Inner xml for XRC we're building

		// Must either use a convenience type or have a wxWidgets class now
		if( type.Str() ){
			if( keywordifind(type,"text") ){
				tclass = "wxStaticText";
				prxt = new wxRlXmlTags("text");;
				if( !label.Str() ){
					wxLogMessage( _T("RlItemFromXml, Warning: Parsing type=text section, missing label.") );
					g_rl_xml_warning = xml;			
				}
			}
			else if( keywordifind(type,"edit") ){
				tclass = "wxTextCtrl";
				prxt = new wxRlXmlTags("edit");
				// # We need better handling of style
				if( keywordifind(style,"readonly") )
					s_ixml += "<style>wxTE_READONLY</style>\n";
			}
			else if( keywordifind(type,"button") ){
				tclass = "wxButton";
				prxt = new wxRlXmlTags("button");
				// Look for default value in state attribute 
				if( keywordifind(state,"default") )
					s_ixml += "<default>1</default>\n";
				// Indicates that a button closes a dialog
				if( keywordifind(state,"quits") )
					prxt->m_quits = true;
			}
			else if( keywordifind(type,"checkbox") ){
				tclass = "wxCheckBox";
				prxt = new wxRlXmlTags("checkbox");
			}
			else if( keywordifind(type,"radio") ){
				tclass = "wxRadioButton";
				prxt = new wxRlXmlTags("radio");
				if( keywordifind(state,"group") )
					s_ixml += "<style>wxRB_GROUP</style>\n";
			}
			else
				return FromXmlError( xml, _T("RlItemFromXml - Unrecognized type value"), MB2WX(type) );
		}

		// It seems we always need this one
		if( !prxt ) prxt = new wxRlXmlTags;

		// Take care of attribs for inner xml and for later saving
		if( label.Str() ){
			s_ixml += ("<label>"+label)+"</label>\n";
			prxt->AddAttrib( "label", label );
		}
		if( value.Str() ){
			s_ixml += "<value>"+value+"</value>\n";
			prxt->AddAttrib( "value", value );
		}
		if( size.x>-1 || size.y>-1 ){
			char buf[32];
			char sbuf[16];
			sprintf( sbuf, "%d,%d", size.x, size.y );
			sprintf( buf, "<size>%s</size>\n", sbuf );
			s_ixml += buf;
			prxt->AddAttrib( "size", sbuf );
		}
		if( minsize.x>-1 || minsize.y>-1 ){
			char buf[32];
			char sbuf[16];
			sprintf( sbuf, "%d,%d", minsize.x, minsize.y );
			sprintf( buf, "<minsize>%s</minsize>\n", sbuf );
			s_ixml += buf;
			prxt->AddAttrib( "minsize", sbuf );
		}
		if( FindAttrib( xml, "enables", cb, pend) ){
			prxt->m_enables = MB2WX(cb);
			prxt->AddAttrib( "enables", cb );
		}

		// fg and bg attributes
		if( FindAttrib(xml,"fg",cb,pend) ){
			prxt->AddAttrib( "fg", cb );
			s_ixml += "<fg>"+cb+"</fg>\n";
		}
		// bg does not work very well
		if( FindAttrib(xml,"bg",cb,pend) ){
			prxt->AddAttrib( "bg", cb );
			s_ixml += "<bg>"+cb+"</bg>\n";
		}
		if( name.Str() )
			prxt->AddAttrib( "name", name );

			
		if( state.Str() ){
			if( keywordifind(state,"enabled") || keywordifind(state,"enable") )
				s_ixml += "<enabled>1</enabled>\n";
			if( keywordifind(state,"disabled") || keywordifind(state,"disable") )
				s_ixml += "<enabled>0</enabled>\n";
			if( keywordifind(state,"hidden") )
				s_ixml += "<hidden>1</hidden>\n";
			if( keywordifind(state,"focused") || keywordifind(state,"focus") )
				s_ixml += "<focused>1</focused>\n";
			prxt->AddAttrib( "state", state );
		}
		
		// We must have tclass defined now - r the skeleton option
		if( !tclass.Length() && !(opts&RL_XMLLOAD_SKELETON) )
			return FromXmlError( xml, _T("RlItemFromXml - Missing class attribute in window tag - Use option RL_MXLLOAD_SKELETON to accept unspeced (empty) windows") );

		if( tclass.Length() ){
			// If we did not create XRC from type, then we must save the class attribute
			if( !type.Str() )
				prxt->AddAttrib( "class", tclass );
			
			// Note: Assume input data is UTF8
			CharBuf s;
			s = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
			s += "<resource>\n<object class=\"";
			s += tclass + "\" "; 
			if( !name.Length() ) name = "RlWindow_Name";
			s += "name=\"" + name + "\" " + ">\n";
			if( pend[-1]!='/' ){
				const char *pclose = keywordifind(xml,pclose_tag,NULL,false);
				if( !pclose ){
					delete prxt;
					return FromXmlError( xml, _T("RlItemFromXml - Parsing error, no closing tag found: %s"), MB2WX(pclose_tag) );
				}
				// Append everything until current closing tag
				s.Append(pend+1,pclose-(pend+1));
				// Save inner XRC XML for later (when saving layout)
				if( pclose>pend+1 ){
					if( !prxt ) prxt = new wxRlXmlTags;
					const char *pixml = pend+1;
					while( *pixml=='\n' || *pixml=='\r' ) pixml++;
					prxt->m_sub_nodes += MB2WX(CharBuf(pixml,pclose));
				}
				xml = pclose; // Advance to closing tag
			}
			s += s_ixml;	// Inner XML from type="..."
			s += "</object>\n</resource>\n";
			s.SaveToFile( "tmp.xrc" ); 
			
			//wxMemoryFSHandler::AddFile( _T("RlXml.xrc"), s.Base(), s.scString::Size() );
			wxMemoryFSHandler::AddFile( _T("RlXml.xrc"), s.Str(), s.Length() );
			if( !g_rl_xml_res.Load(_T("memory:RlXml.xrc")) ){
				delete prxt;
				return FromXmlError( xml, _T("RlItemFromXml - Failed loading window XML/XRC data (Memory VFS)"), MB2WX(pclose_tag) );
			}
			
			wxObject *pobj = g_rl_xml_res.LoadObject( prnt_wnd, MB2WX(name), MB2WX(tclass) );
			g_rl_xml_res.Unload(_T("memory:RlXml.xrc"));
			wxMemoryFSHandler::RemoveFile( _T("RlXml.xrc") );
			wxWindow *wnd = wxDynamicCast(pobj,wxWindow); 
			if( !wnd ){
				delete prxt;
				delete pobj;
				return FromXmlError( xml, _T("RlItemFromXml - Failed creating window from XML/XRC data"), MB2WX(pclose_tag) );
			}
			rli = new wxRlItem( wnd, prop, flags, RlWindow );
		}
		else
			// No window class specifified, let the app fill in later
			rli = new wxRlItem( 1,1, prop, flags, RlWindow );
		
		if( !(opts&RL_XMLLOAD_NO_USERDATA ))
			rli->SetUserData(prxt);
		else
			delete prxt;
		
#else // wxUSE_XRC==1
		// No XRC available, just return
		return FromXmlError( xml, _T("RlItemFromXml - Request to create window, XRC library not enabled"), MB2WX(pclose_tag) );
#endif
	}
	
	if( !rli )
		return FromXmlError( xml, _T("RlItemFromXml - Failed creating wxRlItem (error unknown)"), MB2WX(pclose_tag) );
	
	// In most cases rli and rli_base are the same
	if( !rli_base ) rli_base=rli;
	
	// Set border (wxRlItems does this from flags by themselves)
	if( border>0 ) 
		rli_base->SetBorder(border);
	
	if( capab!=-1 )
		rli_base->m_capab |= (short)capab;
	
	// Set min size - for spacers, apply size if given and no minsize
	if( minsize.x>-1 || minsize.y>-1 ) 
		rli_base->SetMinSize(minsize);
	else if( is_spacer && (size.x>-1 || size.y>-1) ) 
		rli_base->SetMinSize(size);
		
	// Unless we haven't parsed past this yet.
	if( xml<=pend ) xml=pend+1;

	// Further config of the item
	if( !rli->ConfigFromXml(xml) )
		return FromXmlError( xml, _T("RlItemFromXml - Failed ConfigFromXml"), MB2WX(pclose_tag) );

	// Item directly closed?
	if( pend[-1]=='/' ){ 
		// We don't want an empty RlSizer - unless 'skeleton mode'
		if( rli->GetType()==RlSizer && !(opts&RL_XMLLOAD_SKELETON) && !is_dialog )
			rli->Insert( 0, new wxRlClientTempBg );
		return rli_base;
	}

	if( is_sizer ){
		// This does internals for a sizer
		wxASSERT(rli->GetSizer());
		
		// There is different mode of operation for a RlSizer compared 
		// to an ordinary sizer. For the RlSizer, we only create the high
		// level objects and let the low level ones be created automatically.
		// Use different Insert functions for that.
		wxRlItem *rli_child = NULL;
		while( true ){
			xml = SkipSpace(xml);
			// Starting element?
			if( *xml!='<' ){
				// Error
				delete rli_base;
				return FromXmlError( xml, _T("RlItemFromXml - Parsing child items, no opening tag found"), MB2WX(pclose_tag) );
			}
			// If closing, we're done
			if( !strncmp(xml,"</",2) ) break;	// This is right
			rli_child = wxRlXmlLoad( xml, prnt_wnd, rli, opts );
			if( !rli_child ){ delete rli_base; return NULL; }	// We had error message already
			if( !rli_child->IsEnumerable() ) 
				rli->GetSizer()->Add( rli_child );
			else if( !rli->Insert( -1, rli_child, is_dialog?RlLayoutEdge:RlUnSpeced ).IsOk() ){
				// Error
				delete rli_base;
				return FromXmlError( xml, _T("RlItemFromXml - Parsing child items, failed wxRlItem::Insert"), MB2WX(pclose_tag) );
			}
		}
		if( rli->GetType()==RlSizer && !rli_child && !is_dialog ){
			// We don't want an empty RlSizer
			rli->Insert( 0, new wxRlClientTempBg );
		}
		
		// Insert padding at start & end of sizer?
		if( wxDynamicCast(rli->GetSizer(),wxBoxSizer) && has_padding ){
			wxRlItem &rli_fst = rli->Part(0);
			if( rli_fst.GetType()!=RlSpacerEnum )
				rli->Insert( 0, new wxRlItem( sz_padding, sz_padding, prop_padding, wxEXPAND, RlSpacerEnum ), is_dialog?RlLayoutEdge:RlUnSpeced );
			wxRlItem &rli_lst = rli->Part(-1);
			if( rli_lst.GetType()!=RlSpacerEnum )
				rli->Insert( -1, new wxRlItem( sz_padding, sz_padding, prop_padding, wxEXPAND, RlSpacerEnum ), is_dialog?RlLayoutEdge:RlUnSpeced );
		}
	}
	
	// We must consume closing tag here
	if( !keywordieq(xml,pclose_tag) ){
		delete rli_base;
		return FromXmlError( xml, _T("RlItemFromXml - non-matching closing tag"), MB2WX(pclose_tag) );
	}
	
	xml = SkipSpace(strchr(xml,'>')+1);	
	return rli_base;
}

#endif // RL_NO_STATE 


