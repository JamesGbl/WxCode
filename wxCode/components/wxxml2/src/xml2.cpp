/////////////////////////////////////////////////////////////////////////////
// Name:        xml.cpp
// Purpose:     wxXml2Document - XML parser & data holder class
// Author:      Francesco Montorsi (based on Vaclav Slavik's design)
// Created:     2003/09/05
// Last modify: 2004/12/18
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vaclav Slavik and (c) 2003 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifdef __GNUG__
#pragma implementation "xml2.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
	#include "wx/datstrm.h"
	#include "wx/zstream.h"
	#include "wx/log.h"
	#include "wx/intl.h"
	#include "wx/strconv.h"
#endif

#include "wx/wfstream.h"		// not included by wxprec.h
#include "wx/filename.h"
#include "wx/xml2.h"



// implementation of dynamic classes
IMPLEMENT_ABSTRACT_CLASS(wxXml2Wrapper, wxObject)
IMPLEMENT_DYNAMIC_CLASS(wxXml2Node, wxXml2Wrapper)
IMPLEMENT_DYNAMIC_CLASS(wxXml2Document, wxXml2Wrapper)
IMPLEMENT_DYNAMIC_CLASS(wxXml2Property, wxXml2Wrapper)
IMPLEMENT_DYNAMIC_CLASS(wxXml2Namespace, wxXml2Wrapper)
IMPLEMENT_DYNAMIC_CLASS(wxXml2DTD, wxXml2Wrapper)


// a macro used in the overloaded operator==; this is the return table:
//
//         x            y          returns
//       NULL          NULL          TRUE		(they are equal)
//       NULL        non-NULL        FALSE		(they are different)
//     non-NULL        NULL          FALSE		(they are different)
//     non-NULL      non-NULL      nothing: no 'return' statement is executed:
//                                 x and y could be equal but they could also
//                                 be different. The code following the macro
//                                 must care about this possibility
//
#define CHECK_NULL_POINTERS(x, y)		\
	if (x == NULL && y == NULL)			\
		return TRUE;					\
	if ((x == NULL && y != NULL) ||		\
		(x != NULL && y == NULL))		\
		return FALSE;


// global instances (they wraps NULL pointers)
wxXml2Document wxXml2EmptyDoc(NULL);
wxXml2Node wxXml2EmptyNode(NULL);
wxXml2Property wxXml2EmptyProperty(NULL);
wxXml2Namespace wxXml2EmptyNamespace(NULL, wxXml2EmptyNode);
wxXml2DTD wxXml2EmptyDTD(NULL);





//-----------------------------------------------------------------------------
//  wxXml2Wrapper
//-----------------------------------------------------------------------------

void wxXml2Wrapper::DestroyIfUnlinked()
{
	DecRefCount();
	int refcount = GetRefCount();
	
	// we must check not only if this element is unlinked from a
	// wider XML tree (if it is, then the root of that tree will
	// destroy all its children recursively) but also if it is
	// wrapped by another wxXml2Wrapper-derived class:
	// in this case we must not destroy the libxml2 structure,
	// otherwise the other wrapper would almost certainly throw
	// an exception since it would try to access invalid memory...
	if (IsUnlinked() && refcount == 0) {
		
		// ... anyway, if there are no parents of this structure
		// and no wrappers are embedding it, then we can safely
		// destroy it.
		Destroy();
		wxLogDebug("%s::DestroyIfUnlinked - destroyed", 
			GetClassInfo()->GetClassName());

	} else {

		// the memory associated with this property will be freed
		// by the node which owns this object or by another wrapper	
		wxLogDebug("%s::DestroyIfUnlinked - NOT destroyed (because %s)", 
			GetClassInfo()->GetClassName(),
			(!IsUnlinked() ? "still linked" : 
					wxString::Format("refcount is %d", refcount).c_str()));
	}
}



//-----------------------------------------------------------------------------
//  wxXml2Property
//-----------------------------------------------------------------------------

bool wxXml2Property::operator==(const wxXml2Property &p) const
{
	CHECK_NULL_POINTERS(p.GetObj(), GetObj());

	// the same name and same value are required
	if (GetName().CmpNoCase(p.GetName()) == 0 &&
		GetValue() == p.GetValue())
		return TRUE;
	return FALSE;
}

void wxXml2Property::Create(const wxString &name, const wxString &value, wxXml2Node &owner)
{
	// if this property was already build, first destroy it
	UnwrappingOld();

	// create the property; if the owner is an empty node, the property
	// will be created unlinked from the main tree.
	m_attr = xmlNewProp(owner.GetObj(), WX2XML(name), WX2XML(value));
	JustWrappedNew();
}

bool wxXml2Property::IsUnlinked() const
{
	if (m_attr == NULL)
		return TRUE;

	// if all links are NULL, then we are unlinked...
	if (m_attr != NULL && m_attr->parent == NULL &&
		m_attr->next == NULL && m_attr->prev == NULL)
		return TRUE;
	
	// at least one is set: we're linked
	return FALSE;
}

void wxXml2Property::SetValue(const wxString &value)
{
	// reset the TEXT child node
	xmlFree((void *)m_attr->children->content);
	m_attr->children->content = xmlStrdup(WX2XML(value));
}

void wxXml2Property::SetName(const wxString &name)
{
	// reset the NAME field
	xmlFree((void *)m_attr->name);
	m_attr->name = xmlStrdup(WX2XML(name));
}

void wxXml2Property::SetNext(wxXml2Property &next)
{
	// reset parent relations
	if (GetNext() != wxXml2EmptyProperty)
		GetNext().SetPrevious(wxXml2EmptyProperty);
	m_attr->next = next.GetObj();

	// cannot use "next.SetPrevious(*this)" because if SetPrevious()
	// makes use of SetNext() on the given node, a stack overflow would happen
	next.GetObj()->prev = m_attr;
}

void wxXml2Property::SetPrevious(wxXml2Property &prev)
{
	// reset parent relations
	if (GetPrevious() != wxXml2EmptyProperty)
		GetPrevious().SetNext(wxXml2EmptyProperty);
	m_attr->prev = prev.GetObj();

	// cannot use "prev.SetNext(*this)" because if SetNext()
	// makes use of SetPrevious() on the given node, a stack overflow would happen
	prev.GetObj()->next = m_attr;
}

void wxXml2Property::SetOwner(wxXml2Node &owner)
{
	// we are forced to recreate this property because the owner has changed;
	// Create() will care about the destruction and the unlinking of the old
	// property...
	Create(GetName(), GetValue(), owner);
}

wxXml2Node wxXml2Property::GetOwner() const
{
	// this function cannot be declared in the header because it makes
	// use of the contructor of wxXml2Node...
	return wxXml2Node(m_attr->parent);
}




//-----------------------------------------------------------------------------
//  wxXml2Namespace
//-----------------------------------------------------------------------------

wxXml2Namespace::wxXml2Namespace(xmlNs *ns, wxXml2Node &owner)
: m_ns(ns), m_owner(owner.GetObj()) 
{ JustWrappedNew(); }

bool wxXml2Namespace::operator==(const wxXml2Namespace &ns) const
{
	CHECK_NULL_POINTERS(ns.GetObj(), GetObj());

	if (GetPrefix() == ns.GetPrefix() &&
		GetURI().CmpNoCase(ns.GetURI()) == 0)
		return TRUE;
	return FALSE;
}

void wxXml2Namespace::Create(const wxString &prefix,
						   const wxString &uri,
						   wxXml2Node &owner)
{
	// if this namespace was already built, first destroy it
	UnwrappingOld();

	// create the property; if the owner is an empty node, the property
	// will be created unlinked from the main tree.
	wxASSERT_MSG(owner != wxXml2EmptyNode, 
              "If I create an unlinked namespace, its "
			  "memory will never be freed...");
	m_ns = xmlNewNs(owner.GetObj(), WX2XML(uri), WX2XML(prefix));
	m_owner = owner.GetObj();		// update also the owner pointer
	JustWrappedNew();

	// just to avoid to be forced to copy-and-paste the checks done in
	// wxXml2Namespace::SetPrefix, make a dummy call to that function
	SetPrefix(prefix);
}

void wxXml2Namespace::SetPrefix(const wxString &p)
{
	xmlFree((void *)m_ns->prefix);
	m_ns->prefix = xmlStrdup(WX2XML(p));

	// if prefix changed, maybe we must update the main
	// node which uses this namespace
	/*if (m_owner != NULL) {
		if (!p.IsEmpty())
			m_owner->SetNamespace(*this);
		else
			m_owner->SetNamespace(wxXml2EmptyNamespace);
	}*/
}

void wxXml2Namespace::SetURI(const wxString &href)
{
	xmlFree((void *)m_ns->href);
	m_ns->href = xmlStrdup(WX2XML(href));
}





//-----------------------------------------------------------------------------
//  wxXml2DTD - creation functions
//-----------------------------------------------------------------------------

void wxXml2DTD::Create(const wxXml2Document &doc,
					   const wxString &name, const wxString &externalID,
						const wxString &systemid)
{
	UnwrappingOld();

	// xmlNewDtd API will link this DTD with the given document...
	m_dtd = xmlNewDtd(doc.GetObj(), WX2XML(name),
					WX2XML(externalID), WX2XML(systemid));
	JustWrappedNew();
}

bool wxXml2DTD::operator==(const wxXml2DTD &dtd) const
{
	CHECK_NULL_POINTERS(dtd.GetObj(), GetObj());

	if (GetName() == dtd.GetName() &&
		GetSystemID() == dtd.GetSystemID() &&
		GetExternalID() == dtd.GetExternalID())
		return TRUE;
	return FALSE;
}

bool wxXml2DTD::IsOk() const
{
	if (m_dtd == NULL)
		return FALSE;

	if (GetName() == wxEmptyString)
		return FALSE;

	// if we are an external reference, then we should
	// be a SYSTEM or a PUBLIC subset...
	if (IsExternalReference() &&
		!IsSystemSubset() &&
		!IsPublicSubset())
		return FALSE;

	return TRUE;
}

bool wxXml2DTD::Load(const wxString &filename, wxString *pErr)
{
	wxFileInputStream stream(filename);	
	if (!stream.IsOk() || !wxFileName::FileExists(filename)) return FALSE;		
	return Load(stream, pErr);
}
	
bool wxXml2DTD::Save(const wxString &filename) const
{
	wxFileOutputStream stream(filename);
	if (!stream.IsOk()) return FALSE;
	return (Save(stream) != -1);
}

// helper function for wxXml2DTD::Load
static void XMLDTDMsg(void *ctx, const char *pszFormat, ...)
{
	// get the variable argument list
	va_list argptr;
	va_start(argptr, pszFormat);
	wxString str = wxString::FormatV(pszFormat, argptr);
	va_end(argptr);

	// append the error string to the private member of the parser context
	xmlParserCtxt *p = (xmlParserCtxt *)ctx;
	wxString *err = (wxString *)p->_private;
	if (err != NULL) *err += str;
}

// helper function for wxXml2DTD::Load
static int XMLDTDRead(void *ctx, char *buffer, int len)
{
	wxInputStream *stream = (wxInputStream*)ctx;
	return stream->Read(buffer, len).LastRead();
}

bool wxXml2DTD::Load(wxInputStream &stream, wxString *pErr)
{
	// are we attached to a wxXml2Document ?
	xmlDoc *doc = (m_dtd ? m_dtd->parent : NULL);
	if (doc) xmlUnlinkNode((xmlNode *)m_dtd);

	// init the SAX handler
	xmlSAXHandler h;
	memset(&h, 0, sizeof(h));
	xmlSAX2InitDefaultSAXHandler(&h, 0);

	// set the error & warning handlers
	h.error = XMLDTDMsg;
	h.warning = XMLDTDMsg;
	h._private = pErr;

	// xmlSAXHandler::fatalError is unused (in LibXML2 2.5.x),
	// however, we set it anyway to support future versions...
	h.fatalError = XMLDTDMsg;

	// setup our input buffer
	xmlParserInputBuffer *myparserbuf = xmlAllocParserInputBuffer(XML_CHAR_ENCODING_NONE);
	myparserbuf->readcallback = XMLDTDRead;
	myparserbuf->context = &stream;

	// parse from buffer
	wxString error;
	UnwrappingOld();
	m_dtd = xmlIOParseDTD(&h, myparserbuf, XML_CHAR_ENCODING_NONE);
	JustWrappedNew();		// don't forget this
	
	if (!IsOk())
		return FALSE;		// don't proceed

	// reattach to the old wxXml2Document, if there was one...
	if (doc)
		wxXml2Document(doc).SetDTD(*this);

	// the xmlParserInputBuffer has been already freed by libxml2
	// and the SAX handler is on the stack... we can return	
	return TRUE;
}

// helper function for wxXml2DTD::Save
static int XMLDTDWrite(void *context, const char *buffer, int len)
{
	wxOutputStream *stream = (wxOutputStream *)context;
	stream->Write(buffer, len);
	return stream->LastWrite();
}
/*
xmlDtdDumpOutput(xmlSaveCtxtPtr ctxt, xmlDtdPtr dtd) {
    xmlOutputBufferPtr buf;
    int format, level;
    xmlDocPtr doc;

     * Dump the notations first they are not in the DTD children list
     * Do this only on a standalone DTD or on the internal subset though.
     
	   if ((dtd->notations != NULL) && ((dtd->doc == NULL) ||
        (dtd->doc->intSubset == dtd))) {
        xmlDumpNotationTable(buf->buffer, (xmlNotationTablePtr) dtd->notations);
    }
    format = ctxt->format;
    level = ctxt->level;
    doc = ctxt->doc;
    ctxt->format = 0;
    ctxt->level = -1;
    ctxt->doc = dtd->doc;
    xmlNodeListDumpOutput(ctxt, dtd->children);
}
*/

int wxXml2DTD::Save(wxOutputStream &stream) const
{
	// are we attached to a wxXml2Document ?
/*	xmlDoc *doc = (m_dtd ? m_dtd->parent : NULL);
	if (doc) xmlUnlinkNode((xmlNode *)m_dtd);

	// we must create a temporary document containing
	// only ourselves...
	wxXml2Document tmp;
	wxXml2DTD dtd(*this);
	tmp.SetDTD(dtd);
	int written = tmp.Save(stream);

	// reattach to the old wxXml2Document, if there was one...
	if (doc)
		wxXml2Document(doc).SetDTD((wxXml2DTD &)*this);*/
	xmlOutputBuffer *buf = xmlAllocOutputBuffer(NULL);
	buf->context = &stream;
	buf->writecallback = XMLDTDWrite;
	m_dtd->doc = NULL;
	xmlNodeDumpOutput(buf, NULL, (xmlNode *)m_dtd, 1, 0, NULL);
	int written = buf->written;
 /*   xmlSaveCtxt ctxt;
    memset(&ctxt, 0, sizeof(ctxt));
    ctxt.doc = NULL;
    ctxt.buf = buf;
    ctxt.level = 0;
    ctxt.format = 1;
    ctxt.encoding = (const xmlChar *)NULL;
    xmlSaveCtxtInit(&ctxt);

	ctxt->format = 0;
    ctxt->level = -1;
    ctxt->doc = dtd->doc;
    //xmlNodeListDumpOutput(ctxt, dtd->children);
	xmlNodeDump(*/

	return written;
}

bool wxXml2DTD::IsPublicSubset() const
{
	// if we are public, then we should have both a 
	// non-empty SystemID (which contains our external URI, in reality)
	// and a non-empty ExternalID
	return m_dtd->SystemID != NULL &&
		m_dtd->ExternalID != NULL;
}

bool wxXml2DTD::IsSystemSubset() const
{
	// SYSTEM subsets only need a SystemID so if we are really
	// a system subset we should have an empty ExternalID
	return m_dtd->SystemID != NULL &&
		m_dtd->ExternalID == NULL;
}

bool wxXml2DTD::IsExternalReference() const
{
	// just check if we have any children: if we don't
	// it is because we are just a sort of link to a
	// full DTD located somewhere else...
	if (GetRoot() == wxXml2EmptyNode)
		return TRUE;
	return FALSE;
}

wxXml2Node wxXml2DTD::GetRoot() const
{ 
	if (m_dtd == NULL || m_dtd->children == NULL)
		return wxXml2EmptyNode;

	wxXml2Node ret(m_dtd->children);

#ifdef __WXDEBUG__
	// be sure that our children are DTD declarations...
	wxXml2NodeType t = ret.GetType();
	wxASSERT(t == wxXML_ELEMENT_DECL ||
		t == wxXML_ATTRIBUTE_DECL ||
		t == wxXML_ENTITY_DECL ||
		t == wxXML_NAMESPACE_DECL ||
		t == wxXML_COMMENT_NODE);		// don't forget comments: they are allowed in DTDs
#endif
	return ret;
}

bool wxXml2DTD::LoadFullDTD(wxString *perr)
{
	if (IsSystemSubset()) {

		// we have our DTD somewhere on this system:
		// load it supposing that the SystemID which
		// should be an URI for XML specification
		// is just the name of our file...
		return Load(GetSystemID(), perr);

	}
	
	wxASSERT(IsPublicSubset());
	
	// public DTD loading is not implemented yet:
	// we would need a URI parser to do that and 
	// we should also create an HTTP connection, probably....
	// let me know if you need this feature.
	//wxASSERT(0);
	if (perr)
		*perr = "PUBLIC DTD loading not supported.";
	
	return FALSE;
}

void wxXml2DTD::SetName(const wxString &str)
{
	// reset the name string
	xmlFree((void *)m_dtd->name);
	m_dtd->name = xmlStrdup(WX2XML(str));
}

void wxXml2DTD::SetSystemID(const wxString &str)
{
	// reset the SystemID string
	xmlFree((void *)m_dtd->SystemID);
	m_dtd->SystemID = xmlStrdup(WX2XML(str));
}

void wxXml2DTD::SetExternalID(const wxString &str)
{
	// reset the ExternalID string
	xmlFree((void *)m_dtd->ExternalID);
	m_dtd->ExternalID = xmlStrdup(WX2XML(str));
}





//-----------------------------------------------------------------------------
//  wxXml2Node - creation functions
//-----------------------------------------------------------------------------

void wxXml2Node::CreateRoot(wxXml2Document &doc,
						   const wxString &name,
						   wxXml2Namespace &ns,
						   wxXml2Property &props)
{
	Build(wxXML_ELEMENT_NODE, wxXml2EmptyNode, doc, ns, name,
		wxEmptyString, props, wxXml2EmptyNode);

	// a last touch
	xmlDocSetRootElement(doc.GetObj(), GetObj());
}

void wxXml2Node::CreateChild(wxXml2NodeType type,
							wxXml2Node &parent,
							const wxString &name,
							const wxString &content,
							wxXml2Namespace &ns,
							wxXml2Property &props,
							wxXml2Node &next)
{
	Build(type, parent, wxXml2EmptyDoc, ns, name, content, props, next);
}

void wxXml2Node::CreateTemp(wxXml2NodeType type,
						   wxXml2Document &doc,
						   const wxString &name,
						   const wxString &content,
						   wxXml2Namespace &ns,
						   wxXml2Property &props)
{
	Build(type, wxXml2EmptyNode, doc, ns, name, content, props, wxXml2EmptyNode);
}


void wxXml2Node::Build(const wxXml2NodeType type, wxXml2Node &parent,
					  wxXml2Document &doc, wxXml2Namespace &ns,
					  const wxString &name, const wxString &content,
					  wxXml2Property &prop, wxXml2Node &next,
					  wxXml2Node &previous, wxXml2Node &children)
{
	// if this node was already build, first destroy it
	UnwrappingOld();

	// then, build the node; this passage should set:
	// - the type of the node
	// - the parent and/or the document which owns this node
	// - the name and/or content
	// - the namespace associated with this node
	switch (type) {
	case wxXML_TEXT_NODE:

		// is the content of this text child an entity ?
		if (content.StartsWith("&") && content.Last() == ';') {

			// yes, it is; just check if the caller
			// also wants to create a tag which wraps the entity
			if (!name.IsEmpty()) {

				// first create an element node....
				Build(wxXML_ELEMENT_NODE, parent, doc, ns, name, wxEmptyString, prop, next, previous);

				// and then adds the entity into a new text child...
				wxXml2Node entity;
				entity.Build(wxXML_TEXT_NODE, parent, doc, ns, wxEmptyString, content);
				AddChild(entity);

			} else {

				// yes, it is.... we just need to create a wxXML_ENTITY_REF_NODE nodetype
				m_node = xmlNewReference(doc.GetObj(), WX2XML(content));
				JustWrappedNew();
				SetNamespace(ns);
			}
			break;
		}

		// not, it wasn't an entity node
		if (!name.IsEmpty()) {

			if (wxXml2EmptyNode != parent) {
				m_node = xmlNewTextChild(parent.GetObj(),
							ns.GetObj(), WX2XML(name), WX2XML(content));
				JustWrappedNew();

			} else {
				// this function can accept NULL as first argument
				m_node = xmlNewDocRawNode(doc.GetObj(), ns.GetObj(),
					  WX2XML(name), WX2XML(content));
				JustWrappedNew();
			}

		} else {

			// do not use libxml2 xmlNewTextChild or xmlNewDocRawNode functions:
			// they would create first a wxXML_ELEMENT_NODE with a wxXML_TEXT_NODE
			// as child; instead we want to create directly a wxXML_TEXT_NODE...
			if (wxXml2EmptyNode != parent) {
				m_node = xmlNewText(WX2XML(content));
				parent.AddChild(*this);
				JustWrappedNew();

			} else {

				// this function can accept NULL as first argument
				m_node = xmlNewDocText(doc.GetObj(), WX2XML(content));
				JustWrappedNew();
			}
		}
		break;

	case wxXML_ELEMENT_NODE:
		if (wxXml2EmptyNode != parent) {

			wxASSERT_MSG(parent.GetType() == wxXML_ELEMENT_NODE,
				"Cannot add children to a non-container node. If you want to add "
				"tags after special nodes like wxXML_PI_NODE or wxXML_CDATA_NODE, "
				"use one of wxXml2Node::Add***Child() on the parent of the special "
				"node. Special nodes cannot have children.");
			m_node = xmlNewChild(parent.GetObj(), ns.GetObj(), WX2XML(name), NULL);
			JustWrappedNew();

		} else {

			// this function can accept NULL as first argument
			m_node = xmlNewDocRawNode(doc.GetObj(), ns.GetObj(), WX2XML(name), NULL);
			JustWrappedNew();
		}
		break;

	case wxXML_COMMENT_NODE:
		if (wxXml2EmptyNode != parent) {
			m_node = xmlNewComment(WX2XML(content));
			JustWrappedNew();

			// set this comment as child of the given parent
			parent.AddChild(*this);
		} else {

			// this function can accept NULL as first argument
			m_node = xmlNewDocComment(doc.GetObj(), WX2XML(content));
			JustWrappedNew();
		}
		break;

	case wxXML_CDATA_SECTION_NODE:
		m_node = xmlNewChild(parent.GetObj(), ns.GetObj(), WX2XML(name), NULL);
		JustWrappedNew();

		// change the type of this node & its contents
		m_node->type = (xmlElementType)wxXML_CDATA_SECTION_NODE;
		xmlNodeSetContent(m_node, WX2XML(content));
		break;

	case wxXML_PI_NODE:
		m_node = xmlNewPI(WX2XML(name), WX2XML(content));
		JustWrappedNew();

		wxASSERT_MSG(doc != wxXml2EmptyDoc, 
			"A PI node must have no parents: it must be attached directly to a "
			"document since it must be placed before any other type of node");
		
		// prepend the node to the children list of the document
		xmlAddPrevSibling(doc.GetObj()->children, m_node);
		break;

	case wxXML_ENTITY_REF_NODE:
		wxASSERT_MSG(0, "Such node is not supported directly: you should create "
			"a text node (wxXML_TEXT_NODE) whose content is an entity. In this "
			"way, a wxXML_ENTITY_REF_NODE node will be created automatically");

	case wxXML_DTD_NODE:
	case wxXML_DOCUMENT_NODE:
	case wxXML_HTML_DOCUMENT_NODE:
	case wxXML_NAMESPACE_DECL:
		wxASSERT_MSG(0, "Such type of node cannot be created !!\n"
						"You must use wxXml2Document, wxXml2DTD or wxXml2Namespace "
						"to create such types of 'nodes'...");

	case wxXML_ELEMENT_DECL:
	case wxXML_ATTRIBUTE_DECL:
	case wxXML_ENTITY_DECL:
		wxASSERT_MSG(0, "still not wrapped");
		break;
	}

	// now, set up:
	// - prop
	// - next, previous & children
	if (wxXml2EmptyProperty != prop) AddProperty(prop);
	if (wxXml2EmptyNode != previous) AddPrevious(previous);
	if (wxXml2EmptyNode != next) AddNext(previous);
	if (wxXml2EmptyNode != children) AddChild(children);
}

wxXml2Node wxXml2Node::AddContainerChild(const wxString &name)
{
	wxXml2Node child;
	child.CreateChild(wxXML_ELEMENT_NODE, *this, name);
	return child;
}

wxXml2Node wxXml2Node::AddTextChild(const wxString &name, const wxString &content)
{
	wxXml2Node child;
	child.CreateChild(wxXML_TEXT_NODE, *this, name, content);
	return child;
}

wxXml2Node wxXml2Node::AddCommentChild(const wxString &content)
{
	wxXml2Node child;
	child.CreateChild(wxXML_COMMENT_NODE, *this, wxT(""), content);
	return child;
}

wxXml2Node wxXml2Node::AddCDATAChild(const wxString &content)
{
	wxXml2Node child;
	child.CreateChild(wxXML_CDATA_SECTION_NODE, *this, wxT(""), content);
	return child;
}

wxXml2Node wxXml2Node::AddPIChild(const wxString &name, const wxString &content)
{
	// this must be a valid node.
	wxASSERT(m_node != NULL && m_node->doc != NULL);
	wxXml2Node child;
	wxXml2Document doc(GetDoc());

	// use CreateTemp even if the created node won't be really temporaneous...
	child.CreateTemp(wxXML_PI_NODE, doc, name, content);
	return child;
}

wxXml2Node wxXml2Node::AddBreakLineChild(int breaklines)
{
	// a "break line child" is just a text node with a new line.
	return AddTextChild(wxT(""), wxString('\n', breaklines));
}

wxXml2Property wxXml2Node::AddProperty(const wxString &name, const wxString &value)
{
	wxXml2Property prop;
	prop.Create(name, value, *this);
	return prop;
}

wxXml2Namespace wxXml2Node::AddNamespace(const wxString &prefix, const wxString &href)
{
	wxXml2Namespace ns;
	ns.Create(prefix, href, *this);
	return ns;
}

void wxXml2Node::AddChild(wxXml2Node &child)
{
	// child will be added at the end of the children list of this
	// object. Adding a child requires also to update "last" pointer
	// in the xmlNode structure and to set child->parent relation.
	// This function will do everything.
	xmlAddChild(GetObj(), child.GetObj());

	// do we have namespaces to update ?
	if (GetNamespace() == wxXml2EmptyNamespace &&
		GetNamespaceDecl() == wxXml2EmptyNamespace)
		return;

	wxXml2Namespace ns;
	if (GetNamespace() == wxXml2EmptyNamespace)
		ns = GetNamespaceDecl();
	else
		ns = GetNamespace();

	// doing something like:
	//    child.GetObj()->ns = GetObj()->ns
	//                    or
	//    xmlSetNs(child.GetObj(), GetObj()->ns);
	//
	// won't work because all the children's namespaces of 'child' must be
	// updated; use a recursive function !!!
	child.SetNamespace(ns);
}

void wxXml2Node::AddProperty(wxXml2Property &prop)
{
	// this function will change the eventually preexisting property with
	// the same name of the given one, or will add it if this node hasn't
	// got any property with that name...
	xmlSetProp(GetObj(), WX2XML(prop.GetName()), WX2XML(prop.GetValue()));
}

void wxXml2Node::AddPrevious(wxXml2Node &node)
{
	// unlinks given node and links it as previous sibling of this node
	xmlAddPrevSibling(GetObj(), node.GetObj());
}

void wxXml2Node::AddNext(wxXml2Node &node)
{
	xmlAddNextSibling(GetObj(), node.GetObj());
}




//-----------------------------------------------------------------------------
//  wxXml2Node - set functions
//-----------------------------------------------------------------------------

void wxXml2Node::SetNext(const wxXml2Node &next)
{
	//GetNext().Destroy();
	if (wxXml2EmptyNode != next)
		xmlAddNextSibling(m_node, next.GetObj());
	else
		m_node->next = NULL;
}

void wxXml2Node::SetChildren(const wxXml2Node &n)
{
	// FIXME
	m_node->children = n.GetObj();
}

void wxXml2Node::SetProperties(const wxXml2Property &prop)
{
	m_node->properties = prop.GetObj();
	prop.GetObj()->parent = m_node;
}

void wxXml2Node::SetNamespace(wxXml2Namespace &ns)
{
	//ns.SetOwner(*this);
	m_node->ns = ns.GetObj();

	// set namespace for all the children of this node
	wxXml2Node p = this->GetChildren();
	while (p != wxXml2EmptyNode) {
		p.SetNamespace(ns);
		p = p.GetNext();
	}
}




//-----------------------------------------------------------------------------
//  wxXml2Node - miscellaneous functions
//-----------------------------------------------------------------------------

bool wxXml2Node::Cmp(const wxXml2Node &node) const
{
	// check contained data
	if (CmpNoNs(node)) {
		CHECK_NULL_POINTERS(node.GetObj(), GetObj());

		// also check namespace
		if (GetNamespace() == node.GetNamespace()) {
		
			// everything was identic (but we didn't check on purpose
			//  parents & children nor properties)
			return TRUE;
		}
	}

	return FALSE;
}

bool wxXml2Node::CmpNoNs(const wxXml2Node &node) const
{
	CHECK_NULL_POINTERS(node.GetObj(), GetObj());

	// check contained data
	if (GetName().CmpNoCase(node.GetName()) == 0 &&
		GetContent() == node.GetContent() &&
		GetType() == node.GetType()) {

		// everything was identic (but we didn't check parents & children
		// nor properties)
		return TRUE;
	}

	return FALSE;
}

bool wxXml2Node::HasProp(const wxString &propName) const
{
    wxXml2Property prop = GetProperties();

    while (prop != wxXml2EmptyProperty) {
    	if (prop.GetName() == propName) 
    	     return TRUE;
    	prop = prop.GetNext();
    }

    return FALSE;
}

bool wxXml2Node::GetPropVal(const wxString &propName, wxString *value) const
{
	wxXml2Property prop = GetProperties();

	while (prop != wxXml2EmptyProperty) {
		if (prop.GetName() == propName) {
			*value = prop.GetValue();
			return TRUE;
		}
		prop = prop.GetNext();
	}

    return FALSE;
}

wxString wxXml2Node::GetPropVal(const wxString &propName, 
								const wxString &defaultVal) const
{
	wxString tmp;
	if (GetPropVal(propName, &tmp))
		return tmp;
    else
		return defaultVal;
}

bool wxXml2Node::IsUnlinked() const
{
	if (m_node == NULL)
		return TRUE;

	// if we have all null pointers, we are not linked anywhere
	if (m_node != NULL && m_node->parent == NULL && 
		m_node->prev == NULL &&	m_node->next == NULL)
		return TRUE;

	// at least one link is valid
	return FALSE;
}

void wxXml2Node::MakeUpper()
{
	SetName(GetName().MakeUpper());

	// set namespace for all the children of this node
	wxXml2Node p = this->GetChildren();
	while (p != wxXml2EmptyNode) {
		p.MakeUpper();
		p = p.GetNext();
	}
}

void wxXml2Node::MakeLower()
{
	SetName(GetName().MakeLower());

	// set namespace for all the children of this node
	wxXml2Node p = this->GetChildren();
	while (p != wxXml2EmptyNode) {
		p.MakeLower();
		p = p.GetNext();
	}
}

wxXml2Node wxXml2Node::Find(const wxString &name, const wxString &content, 
							int occ, bool bNS) const
{
	wxXml2Node tmp;    // this will be automatically removed
	wxXml2Document doc(GetDoc());

	// create a temporary node so that we can use the other overload of Find()
	tmp.CreateTemp(wxXML_ELEMENT_NODE, doc, name, content);
	return Find(tmp, occ, bNS);
}

wxXml2Node wxXml2Node::Find(const wxXml2Node &tofind, int occ, bool bNS) const
{
	// check pointer is okay
	wxASSERT_MSG(tofind != NULL, "Invalid pointer");

	// declare some variables
	wxXml2Node r = wxXml2EmptyNode, n = GetChildren();

	// search among the children of the given node
	while (n != wxXml2EmptyNode)
	{
		// check this node
		bool found = FALSE;
		if (bNS)
			found = n.Cmp(tofind);
		else
			found = n.CmpNoNs(tofind);

		if (found) {

			// found an occurrence of the tag
			occ--;
			if (occ == -1)
				return n;
		}

		// search among the children of this node...
		if ((r = n.Find(tofind, occ, bNS)) != NULL)
			return r;

		// search in next child
		n = n.GetNext();
	}

	// we didn't found it
	return wxXml2EmptyNode;
}

void wxXml2Node::Replace(const wxXml2Node &newnode)
{
	// we've got a perfect API for this...
	xmlReplaceNode(GetObj(), newnode.GetObj());
}

void wxXml2Node::Encapsulate(const wxString &nodename)
{
	wxXml2Node tmp, copy(*this);

	tmp.CreateTemp(wxXML_ELEMENT_NODE, wxXml2EmptyDoc, nodename);
	this->Replace(tmp);
	this->AddChild(copy);
}

bool wxXml2Node::operator==(const wxXml2Node &node) const
{
    // just use the case-sensitive comparison function
	return Cmp(node);
}





//-----------------------------------------------------------------------------
//  wxXml2Document
//-----------------------------------------------------------------------------

// helper function for wxXml2Document::Load
static void XMLDocumentMsg(void *ctx, const char *pszFormat, ...)
{
	// get the variable argument list
	va_list argptr;
	va_start(argptr, pszFormat);
	wxString str = wxString::FormatV(pszFormat, argptr);
	va_end(argptr);

	// append the error string to the private member of the parser context
	xmlParserCtxt *p = (xmlParserCtxt *)ctx;
	wxString *err = (wxString *)p->_private;
	if (err != NULL) *err += str;
}

// helper function for wxXml2Document::Save
static int XMLDocumentWrite(void *context, const char *buffer, int len)
{
	wxOutputStream *stream = (wxOutputStream *)context;
	stream->Write(buffer, len);
	return stream->LastWrite();
}

bool wxXml2Document::operator==(const wxXml2Document &doc) const
{
	// check for null pointers
	if ((int)doc.GetObj() ^ (int)GetObj())
		return FALSE;
	if (GetRoot() == doc.GetRoot())
		return TRUE;
	return FALSE;
}

bool wxXml2Document::Load(wxInputStream &stream, wxString *pErr)
{
	long l = stream.GetSize();
	wxString buf;

    // load everything in a buffer
	stream.Read(buf.GetWriteBuf(l), l);
	buf.UngetWriteBuf();

	// set the error & warning handlers
	xmlSAXHandler h;
	memset(&h, 0, sizeof(h));
	xmlSAX2InitDefaultSAXHandler(&h, 0);
	h.error = XMLDocumentMsg;
	h.warning = XMLDocumentMsg;

	// xmlSAXHandler::fatalError is unused (in LibXML2 2.5.x),
	// however, we set it anyway to support future versions...
	h.fatalError = XMLDocumentMsg;

	// parse from buffer
	wxString error;
	UnwrappingOld();
	m_doc = xmlSAXParseMemoryWithData(&h, buf, l, 1, &error);
	JustWrappedNew();

	// copy error string
	if (pErr) *pErr = error;

	return IsOk();
}

int wxXml2Document::Save(wxOutputStream &stream,
						 const wxString &encoding, int /*indentstep*/) const
{
	xmlCharEncodingHandler *encoder = NULL;

	// use one of the libxml2 encoders if required...
	if (!encoding.IsEmpty())
		encoder = xmlFindCharEncodingHandler(encoding);

	xmlOutputBuffer *ob = xmlOutputBufferCreateIO(XMLDocumentWrite, NULL,
								(void *)(&stream), encoder);

	// set up output options
	xmlKeepBlanksDefault(0);

	// save old indentation string
	//wxString old = xmlTreeIndentString;
	//xmlTreeIndentString = wxString(' ', indentstep);

	// save file & return (the output buffer will be deleted by libxml2)
	xmlSubstituteEntitiesDefault(0);
	int res = xmlSaveFormatFileTo(ob, m_doc, encoding, 1);

	// restore old indentation string
	//xmlTreeIndentString = old;

	return res;
}

void wxXml2Document::SetRoot(wxXml2Node &node)	// not const because its object is modified...
{
	// and the old root should be deleted or it is automatically
	// deleted by libxml2 ?
	xmlDocSetRootElement(m_doc, node.GetObj()); 
}

void wxXml2Document::SetDTD(wxXml2DTD &dtd)
{
	// avoid unhandled exceptions
	if (GetObj() == NULL) return;

	xmlDtd *cur = dtd.GetObj();
	xmlDoc *doc = GetObj();

	// link the document with the given DTD: this section of code
	// has been copied from the xmlCreateIntSubset() libxml2's function.
	// Libxml2 in fact, misses a function like xmlSetDocDTD and thus
	// the only other way to implement wxXml2Document::SetDTD would
	// be to create on the fly another DTD with xmlCreateIntSubset():
	// but in such way the DTD of this document wouldn't be the DTD
	// given to this function but a copy of it...
	doc->intSubset = cur;
	if (cur == NULL) return;		// this was a little add by me
	cur->parent = doc;
	cur->doc = doc;

	if (doc->children == NULL) {
		doc->children = (xmlNodePtr) cur;
		doc->last = (xmlNodePtr) cur;
	} else {
		if (doc->type == XML_HTML_DOCUMENT_NODE) {
			xmlNodePtr prev;
			
			prev = doc->children;
			prev->prev = (xmlNodePtr) cur;
			cur->next = prev;
			doc->children = (xmlNodePtr) cur;
		} else {
			xmlNodePtr next;
			
			next = doc->children;
			while ((next != NULL) && (next->type != XML_ELEMENT_NODE))
				next = next->next;
			if (next == NULL) {
				cur->prev = doc->last;
				cur->prev->next = (xmlNodePtr) cur;
				cur->next = NULL;
				doc->last = (xmlNodePtr) cur;
			} else {
				cur->next = next;
				cur->prev = next->prev;
				if (cur->prev == NULL)
					doc->children = (xmlNodePtr) cur;
				else
					cur->prev->next = (xmlNodePtr) cur;
				next->prev = (xmlNodePtr) cur;
			}
		}
	}
}

wxXml2Node wxXml2Document::GetRoot() const
{
	// wrap our root in a wxXml2Node object...
	return wxXml2Node(xmlDocGetRootElement(m_doc));
}

wxXml2DTD wxXml2Document::GetDTD() const
{
	// wrap our DTD into a wxXml2DTD
	return wxXml2DTD(m_doc->intSubset);
}

void wxXml2Document::SetMathMLDTD()
{
	// this is the standard DTD reference for MathML 2.0 documents
	wxXml2DTD mathml(*this, "math", "-//W3C//DTD MathML 2.0//EN",
          "http://www.w3.org/TR/MathML2/dtd/mathml2.dtd");

	SetDTD(mathml);
}

void wxXml2Document::SetXHTMLStrictDTD()
{
	// this is a standard DTD reference for XHTML 1.0 documents
	wxXml2DTD xhtml(*this, "html", "-//W3C//DTD XHTML 1.0 Strict//EN",
					"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd");

	SetDTD(xhtml);
}

void wxXml2Document::SetXHTMLTransitionalDTD()
{
	// this is a standard DTD reference for XHTML 1.0 documents
	wxXml2DTD xhtml(*this, "html", "-//W3C//DTD XHTML 1.0 Transitional//EN",
					"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd");

	SetDTD(xhtml);
}

void wxXml2Document::SetXHTMLFrameSetDTD()
{
	// this is a standard DTD reference for XHTML 1.0 documents
	wxXml2DTD xhtml(*this, "html", "-//W3C//DTD XHTML 1.0 Frameset//EN",
					"http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd");

	SetDTD(xhtml);
}

void wxXml2Document::SetStyleSheet(const wxString &xslfile)
{
	wxString content(wxT("type=\"text/xsl\" href=\"" + xslfile + "\""));
	wxXml2Node root(GetRoot());
	
	if (root == wxXml2EmptyNode) {

		// we must create a "root" which is then 
		// transformed into a PI node
		root.CreateRoot(*this, wxT("xml-stylesheet"));
		root.SetType(wxXML_PI_NODE);
		root.SetContent(content);
		
	} else {
	
		// we can directly use the pre-existing root
		root.AddPIChild(wxT("xml-stylesheet"), content);
	}
}

bool wxXml2Document::Load(const wxString &filename, wxString *pErr)
{
	wxFileInputStream stream(filename);	
	if (!stream.IsOk() || !wxFileName::FileExists(filename)) return FALSE;		
	return Load(stream, pErr);
}
	
bool wxXml2Document::Save(const wxString &filename, const wxString &encoding, int indentstep) const
{
	wxFileOutputStream stream(filename);
	if (!stream.IsOk()) return FALSE;
	return (Save(stream, encoding, indentstep) != -1);
}	

// helper function for wxXml2Document::IsDTDValid
static void XMLValidationMsg(void *out, const char *pszFormat, ...)
{
	// get the variable argument list
	va_list argptr;
	va_start(argptr, pszFormat);
	wxString str = wxString::FormatV(pszFormat, argptr);
	va_end(argptr);

	// append the error/warning string to the void pointer
	// which is the "userData" member of the xmlValidCtxt
	// used for validation, which was set to a pointer to
	// a wxString by wxXml2Document::IsDTDValid
	wxString *err = (wxString *)out;
	if (err != NULL) *err += str;
}

bool wxXml2Document::IsDTDValid(wxString *err, int bUseInternal) const
{
	// create a new validation context
	xmlValidCtxt *ctxt = xmlNewValidCtxt();

	// set some useful callbacks
	ctxt->userData = err;
	ctxt->error = XMLValidationMsg;
	ctxt->warning = XMLValidationMsg;

	// try to validate the document...
	bool ret = FALSE;
	
	if (bUseInternal == 1) {

		// use the internal subset; do nothing if it's not available.
		if (!m_doc->intSubset) return FALSE;
		ret = xmlValidateDtd(ctxt, m_doc, m_doc->intSubset) != 0;
	
	} else if (bUseInternal == 0) {

		// use the external subset; do nothing if it's not available.
		if (!m_doc->extSubset) return FALSE;
		ret = xmlValidateDtd(ctxt, m_doc, m_doc->extSubset) != 0;

	} else {

		// use subset which is not NULL
		if (!m_doc->intSubset && !m_doc->extSubset) return FALSE;
		ret = xmlValidateDtd(ctxt, m_doc, 
			(m_doc->intSubset ? m_doc->intSubset : m_doc->extSubset)) != 0;
	}

	xmlFreeValidCtxt(ctxt);
	return ret;
}


