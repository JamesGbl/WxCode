/////////////////////////////////////////////////////////////////////////////
// Name:        xml.cpp
// Purpose:     wxXml2Document - XML parser & data holder class
// Author:      Francesco Montorsi (based on Vaclav Slavik's design)
// Created:     2003/09/05
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vaclav Slavik and (c) 2003 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "xml.cpp"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif


// includes
#include "wx/wfstream.h"
#include "wx/datstrm.h"
#include "wx/zstream.h"
#include "wx/log.h"
#include "wx/intl.h"
#include "wx/strconv.h"

#include "wx/xml2.h"



// implementation of dynamic classes
IMPLEMENT_DYNAMIC_CLASS(wxXml2Node, wxObject)
IMPLEMENT_DYNAMIC_CLASS(wxXml2Document, wxObject)
IMPLEMENT_DYNAMIC_CLASS(wxXml2Property, wxObject)
IMPLEMENT_DYNAMIC_CLASS(wxXml2Namespace, wxObject)


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





//-----------------------------------------------------------------------------
//  wxXml2Property
//-----------------------------------------------------------------------------

bool wxXml2Property::operator==(const wxXml2Property &p)
{
	CHECK_NULL_POINTERS(p.GetObj(), GetObj());

	if (GetName().CmpNoCase(p.GetName()) == 0 &&
		GetValue() == p.GetValue())
		return TRUE;
	return FALSE;
}

void wxXml2Property::Create(const wxString &name, const wxString &value, wxXml2Node &owner)
{
	// if this property was already build, first destroy it
	Destroy();

	// create the property; if the owner is an empty node, the property
	// will be created unlinked from the main tree.
	m_attr = xmlNewProp(owner.GetObj(), WX2XML(name), WX2XML(value));
}

bool wxXml2Property::IsUnlinked()
{
	// check if at least one of the pointer is set
	if (m_attr != NULL && m_attr->parent == NULL &&
		m_attr->next == NULL && m_attr->prev == NULL)
		return TRUE;
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
/*
wxXml2Namespace::~wxXml2Namespace()
{
	if (m_owner.GetObj() == NULL)
		Destroy();
}*/

bool wxXml2Namespace::operator==(const wxXml2Namespace &ns)
{
	CHECK_NULL_POINTERS(ns.GetObj(), GetObj());

	if (GetPrefix() == ns.GetPrefix() &&
		GetURI().CmpNoCase(ns.GetURI()) == 0)
		return TRUE;
	return FALSE;
}

void wxXml2Namespace::Create(const wxString &prefix,
						   const wxString &uri,
						   wxXml2Node &owner) {
	// if this namespace was already build, first destroy it
	Destroy();

	// create the property; if the owner is an empty node, the property
	// will be created unlinked from the main tree.
	wxASSERT_MSG(owner != wxXml2EmptyNode, "If I create an unlinked namespace, its "
							"memory will never be freed...");
	m_ns = xmlNewNs(owner.GetObj(), WX2XML(uri), WX2XML(prefix));
	m_owner = &owner;


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
	if (m_owner != NULL) {
		if (!p.IsEmpty())
			m_owner->SetNamespace(*this);
		else
			m_owner->SetNamespace(wxXml2EmptyNamespace);
	}
}

void wxXml2Namespace::SetURI(const wxString &href)
{
	xmlFree((void *)m_ns->href);
	m_ns->href = xmlStrdup(WX2XML(href));
}
/*
wxXml2Node wxXml2Namespace::GetOwner() const
{
	// the libxml2's namespace structure, xmlNs, does not contain a pointer
	// to the owner of the namespace...
	return m_owner;
}

void wxXml2Namespace::SetOwner(wxXml2Node &owner)
{
	// unlink namespace from old owner
	if (m_owner != wxXml2EmptyNode)
		m_owner.SetNamespace(wxXml2EmptyNamespace);

	// link this namespace with the new owner
	m_owner = owner;
	xmlSetNs(m_owner.GetObj(), m_ns);
}
*/



//-----------------------------------------------------------------------------
//  wxXml2Node - creation functions
//-----------------------------------------------------------------------------

void wxXml2Node::CreateRoot(wxXml2Document doc,
						   const wxString &name,
						   wxXml2Namespace ns,
						   const wxXml2Property props)
{
	Build(wxXML_ELEMENT_NODE, wxXml2EmptyNode, doc, ns, name,
		wxEmptyString, props, wxXml2EmptyNode);

	// a last touch
	xmlDocSetRootElement(doc.GetObj(), GetObj());
}

void wxXml2Node::CreateChild(wxXml2NodeType type,
							wxXml2Node parent,
							const wxString &name,
							const wxString &content,
							wxXml2Namespace ns,
							const wxXml2Property props,
							const wxXml2Node next)
{
	Build(type, parent, wxXml2EmptyDoc, ns, name, content, props, next);
}

void wxXml2Node::CreateTemp(wxXml2NodeType type,
						   wxXml2Document doc,
						   const wxString &name,
						   const wxString &content,
						   wxXml2Namespace ns,
						   const wxXml2Property props)
{
	Build(type, wxXml2EmptyNode, doc, ns, name, content, props, wxXml2EmptyNode);
}


void wxXml2Node::Build(const wxXml2NodeType type, wxXml2Node &parent,
					  wxXml2Document &doc, wxXml2Namespace &ns,
					  const wxString &name, const wxString &content,
					  const wxXml2Property &prop, const wxXml2Node &next,
					  const wxXml2Node &previous, const wxXml2Node &children)
{
	// if this node was already build, first destroy it
	//Destroy();

	// check arguments
	/*wxASSERT_MSG(wxXml2EmptyNode != parent || wxXml2EmptyDoc != doc,
		"Cannot create a node without a parent nor a document");*/

	// then, build the node; this passage should set:
	// - the type of the node
	// - the parent and/or the document which owns this node
	// - the name and/or content
	// - the namespace associated with this node
	switch (type) {
	case wxXML_TEXT_NODE:

		// is the content of this text child an entity ?
		if (content.StartsWith("&") && content.Last() == ';') {

			if (!name.IsEmpty()) {
				Build(wxXML_ELEMENT_NODE, parent, doc, ns, name, wxEmptyString, prop, next, previous);
				wxXml2Node entity;
				entity.Build(wxXML_TEXT_NODE, parent, doc, ns, wxEmptyString, content);
				AddChild(entity);

			} else {

				// yes, it is.... we just need to create a wxXML_ENTITY_REF_NODE nodetype
				m_refData = new wxXml2NodeRefData(xmlNewReference(doc.GetObj(),
																WX2XML(content)));
				SetNamespace(ns);
			}
			break;
		}

		// not, it wasn't an entity node
		if (!name.IsEmpty()) {

			if (wxXml2EmptyNode != parent) {
				m_refData = new wxXml2NodeRefData(xmlNewTextChild(parent.GetObj(),
							ns.GetObj(), WX2XML(name), WX2XML(content)));
			} else {
				// this function can accept NULL as first argument
				m_refData = new wxXml2NodeRefData(xmlNewDocRawNode(doc.GetObj(), ns.GetObj(),
					  WX2XML(name), WX2XML(content)));
			}

		} else {

			// do not use libxml2 xmlNewTextChild or xmlNewDocRawNode functions:
			// they would create first a wxXML_ELEMENT_NODE with a wxXML_TEXT_NODE
			// as child; instead we want to create directly a wxXML_TEXT_NODE...
			if (wxXml2EmptyNode != parent) {
				m_refData = new wxXml2NodeRefData(xmlNewText(WX2XML(content)));
				parent.AddChild(*this);
			} else {

				// this function can accept NULL as first argument
				m_refData = new wxXml2NodeRefData(xmlNewDocText(doc.GetObj(), WX2XML(content)));
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
			m_refData = new wxXml2NodeRefData(xmlNewChild(parent.GetObj(), ns.GetObj(), WX2XML(name), NULL));

		} else {

			// this function can accept NULL as first argument
			m_refData = new wxXml2NodeRefData(xmlNewDocRawNode(doc.GetObj(), ns.GetObj(),
				WX2XML(name), NULL));
		}
		break;

	case wxXML_COMMENT_NODE:
		if (wxXml2EmptyNode != parent) {
			m_refData = new wxXml2NodeRefData(xmlNewComment(WX2XML(content)));

			// set this comment as child of the given parent
			parent.AddChild(*this);
		} else {

			// this function can accept NULL as first argument
			m_refData = new wxXml2NodeRefData(xmlNewDocComment(doc.GetObj(), WX2XML(content)));
		}
		break;

	case wxXML_CDATA_SECTION_NODE:
	case wxXML_ENTITY_REF_NODE:
	case wxXML_PI_NODE:
	case wxXML_DOCUMENT_NODE:
	case wxXML_HTML_DOCUMENT_NODE:
	case wxXML_DTD_NODE:
		wxASSERT_MSG(0, "You have to use another way to create such a node " \
						"(see the 'enum wxXml2NodeType' in xml.h, " \
						"or the Build() method for this nodetype is still " \
						"not implemented");
		break;
	}

	// now, set up:
	// - prop
	// - next, previous & children
	if (wxXml2EmptyProperty != prop) AddProperty(prop);
	if (wxXml2EmptyNode != previous) AddPrevious(previous);
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

wxXml2Node wxXml2Node::AddBreakLineChild(int breaklines)
{
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

void wxXml2Node::AddChild(wxXml2Node child) //const
{
	// child will be added at the end of the children list of this
	// object. Adding a child requires also to update "last" pointer
	// in the xmlNode structure and to set child->parent relation.
	// This function will do everything.
	xmlAddChild(GetObj(), child.GetObj());

	// doing something like:
	//    child.GetObj()->ns = GetObj()->ns
	//                    or
	//    xmlSetNs(child.GetObj(), GetObj()->ns);
	//
	// won't work because all the children's namespaces of 'child' must be
	// updated; use a recursive function !!!
	child.SetNamespace(GetNamespace());
}

void wxXml2Node::AddProperty(wxXml2Property prop)
{
	// this function will change the eventually preexisting property with
	// the same name of the given one, or will add it if this node hasn't
	// got any property with that name...
	xmlSetProp(GetObj(), WX2XML(prop.GetName()), WX2XML(prop.GetValue()));
}

void wxXml2Node::AddPrevious(wxXml2Node node)
{
	// unlinks given node and links it as previous sibling of this node
	xmlAddPrevSibling(GetObj(), node.GetObj());
}

//-----------------------------------------------------------------------------
//  wxXml2Node - set functions
//-----------------------------------------------------------------------------

void wxXml2Node::SetNext(const wxXml2Node &next)
{
	//GetNext().Destroy();
	if (wxXml2EmptyNode != next)
		xmlAddNextSibling(Node(), next.GetObj());
	else
		Node()->next = NULL;
}

void wxXml2Node::SetChildren(const wxXml2Node &)
{

}

bool wxXml2Node::operator==(const wxXml2Node &node)
{
	return Cmp(node);
}

bool wxXml2Node::Cmp(const wxXml2Node &node)
{
	// check contained data
	if (CmpNoNs(node)) {
		CHECK_NULL_POINTERS(node.GetObj(), GetObj());

		// also check namespace
		if (GetNamespace() == node.GetNamespace()) {
			// everything was identic (but we didn't check parents & children
			// nor properties)
			return TRUE;
		}
	}

	return FALSE;
}

bool wxXml2Node::CmpNoNs(const wxXml2Node &node)
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
	if (prop.GetName() == propName) return TRUE;
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

wxString wxXml2Node::GetPropVal(const wxString &propName, const wxString &defaultVal) const
{
	wxString tmp;
	if (GetPropVal(propName, &tmp))
		return tmp;
    else
		return defaultVal;
}

void wxXml2Node::SetProperties(const wxXml2Property &prop)
{
	Node()->properties = prop.GetObj();
	prop.GetObj()->parent = Node();
}

void wxXml2Node::SetNamespace(wxXml2Namespace ns)
{
	//ns.SetOwner(*this);
	Node()->ns = ns.GetObj();

	// set namespace for all the children of this node
	wxXml2Node p = this->GetChildren();
	while (p != wxXml2EmptyNode) {
		p.SetNamespace(ns);
		p = p.GetNext();
	}
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

wxXml2Node wxXml2Node::Find(const wxString &name, const wxString &content, int occ, bool bNS)
{
	wxXml2Node tmp;
	tmp.CreateTemp(wxXML_ELEMENT_NODE, GetDoc(), name, content);
	return Find(tmp, occ, bNS);
}

wxXml2Node wxXml2Node::Find(wxXml2Node &tofind, int occ, bool bNS)
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
	xmlReplaceNode(GetObj(), newnode.GetObj());
}

void wxXml2Node::Encapsulate(const wxString &nodename)
{
	wxXml2Node tmp, copy(*this);

	tmp.CreateTemp(wxXML_ELEMENT_NODE, wxXml2EmptyDoc, nodename);
	this->Replace(tmp);
	this->AddChild(copy);
}





//-----------------------------------------------------------------------------
//  wxXml2Document
//-----------------------------------------------------------------------------

// helper function for wxXml2Document::Load
static void XMLLoadError(void *ctx, const char *pszFormat, ...)
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
static int WriteXMLInStream(void *context, const char *buffer, int len)
{
	wxOutputStream *stream = (wxOutputStream *)context;
	stream->Write(buffer, len);
	return stream->LastWrite();
}

bool wxXml2Document::operator==(const wxXml2Document &doc)
{
	// check for null pointers
	if ((int)doc.GetObj() ^ (int)GetObj())
		return FALSE;
	if (GetRoot() == doc.GetRoot())
		return TRUE;
	return FALSE;
}

wxXml2Node wxXml2Document::GetRoot() const
{
	// wrap our root in a wxXml2Node object...
	return wxXml2Node(xmlDocGetRootElement(m_doc));
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
	h.error = XMLLoadError;
	h.warning = XMLLoadError;

	// xmlSAXHandler::fatalError is unused (in LibXML2 2.5.x),
	// however, we set it anyway to support future versions...
	h.fatalError = XMLLoadError;

	// parse from buffer
	wxString error;
	m_doc = xmlSAXParseMemoryWithData(&h, buf, l, 1, &error);

	// copy error string
	*pErr = error;
	return IsOk();
}

bool wxXml2Document::Save(wxOutputStream &stream,
						 const wxString &encoding, int /*indentstep*/) const
{
	xmlOutputBuffer *ob = xmlOutputBufferCreateIO(WriteXMLInStream, NULL,
								(void *)(&stream), NULL);

	// set up output options
	xmlKeepBlanksDefault(0);

	// save old indentation string
	//wxString old = xmlTreeIndentString;
	//xmlTreeIndentString = wxString(' ', indentstep);

	// save file & return (the output buffer will be deleted by libxml2)
	xmlSubstituteEntitiesDefault(0);
	bool res = (xmlSaveFormatFileTo(ob, m_doc, encoding, 1) != -1);

	// restore old indentation string
	//xmlTreeIndentString = old;

	return res;
}

