/////////////////////////////////////////////////////////////////////////////
// Name:        xml2.h
// Purpose:     libxml2 wrappers
// Author:      Francesco Montorsi (original design based on EXPAXT by Vaclav Slavik)
// Created:     2000/03/05
// RCS-ID:      
// Copyright:   (c) 2000 Vaclav Slavik and (c) 2003 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_XML2_H_
#define _WX_XML2_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "xml2.h"
#endif

// wxWidgets headers
#include "wx/string.h"
#include "wx/object.h"

class wxInputStream;
class wxOutputStream;


// Libxml2 headers: these classes wraps only libxml2 elements. You cannot
// use them without libxml2 !!!!!
//
// To add libxml2 to your project:
//   1) install libxml2 library binaries or compile them
//      (you can find all the info at http://xmlsoft.org)
//   2) add the include and library paths to the compiler
//   3) links your progrma with the 'libxml2.lib' 
//      (and 'iconv.lib' under Win32)
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>


#ifdef WXXMLISDLL
// if building DLL, then WX*XML*DLLEXPORT=WXDLLEXPORT...
#define WXXMLDLLEXPORT		WXDLLEXPORT
#else
// ...otherwise 
#define WXXMLDLLEXPORT
#endif

// these are required because the compiler must know about their existence
class WXXMLDLLEXPORT wxXml2Node;
class WXXMLDLLEXPORT wxXml2Property;
class WXXMLDLLEXPORT wxXml2Namespace;
class WXXMLDLLEXPORT wxXml2Document;


// this macro converts from wxStrings to xmlChars
#define WX2XML(str)		((xmlChar *)str.mb_str(wxConvUTF8))


//! Represents an XML node type. LibXML2 allows a lot of different element types,
//! but the following are the types you will need to use with the wxXml2
//! classes...
enum wxXml2NodeType
{
    // note: values are synchronized with xmlElementType from libxml2 2.5.x

	//! container nodes; they are characterized by the NAME, the NAMESPACE
	//! and the CHILDRENs. They create something like:
	//!                  <NAME>children</NAME>  
	//! or (if namespace != wxXml2EmptyNamespace),
	//!                  <NS:NAME>children</NS:NAME>
	//! Examples:
	//!     wxXml2Node container(wxXML_ELEMENT_NODE, parentnode, "MYTAG");
	//!     container.AddChild(....);
	//!
    wxXML_ELEMENT_NODE =		1,

	//! nodes containing text; they are characterized by VALUE (or CONTENT)
    wxXML_TEXT_NODE =			3,

	//! creates something like: <![CDATA[content]]>
	//! To use this type of node, create a container node and then use SetType().
	//! To set content use wxXml2Node::SetContent
	wxXML_CDATA_SECTION_NODE =	4,

	//! like a text node, but this node contains only an "entity". Entities
	//! are strings like: &amp; or &quot; or &lt; ....
	//! To create them, use
	//!    wxXml2Node entityref(wxXML_TEXT_NODE, parentnode, "&amp;");
	//!    containernode.AddChild(entityref);
	wxXML_ENTITY_REF_NODE =		5,

	//! creates something like: <?xml-stylesheet?>
	//! To use this type of node, create a container node and then use SetType().
	//! To add 'properties', you cannot use wxXml2Property; instead use
	//! wxXml2Node::SetContent("property=\"value\"");
	wxXML_PI_NODE =				7,

    wxXML_COMMENT_NODE =		8,		//! for nodes containing comments (see 
    wxXML_DOCUMENT_NODE =		9,		//! the "type" value of xmlDoc structure)
    wxXML_HTML_DOCUMENT_NODE =	13,	

	//! creates something like <!DOCTYPE name PUBLIC externalID SystemID>;
	//! this node still must be wrapped... wait for wxXml2DTD
	wxXML_DTD_NODE =			14,
};



// global instances of empty objects
extern wxXml2Node wxXml2EmptyNode;
extern wxXml2Property wxXml2EmptyProperty;
extern wxXml2Namespace wxXml2EmptyNamespace;
extern wxXml2Document wxXml2EmptyDoc;



//! Represents a node property.
//! Example: in <img src="hello.gif" id="3"/> "src" is property with value
//!          "hello.gif" and "id" is prop. with value "3".
class WXXMLDLLEXPORT wxXml2Property : public wxObject
{
	DECLARE_DYNAMIC_CLASS(wxXml2Property)

	//! the XML property wrapped by this class
	xmlAttr *m_attr;

	//! destroys this XML property
	void Destroy() {
		if (m_attr) xmlRemoveProp(m_attr);
		m_attr = NULL;
	}


public:

	//! constructs an empty property; it cannot be used until you #Create() it.
	wxXml2Property() : m_attr(NULL) {}

	//! constructs a property associated with the given node
    wxXml2Property(const wxString &name, 
					const wxString &value,
					wxXml2Node &owner = wxXml2EmptyNode) : m_attr(NULL) { 
		Create(name, value, owner); 
	}

	//! wraps the given XML structure
	wxXml2Property(xmlAttr *pattr) { 
		m_attr = pattr; 
	}

	//! destructor
	virtual ~wxXml2Property() {
		if (IsUnlinked()) {
			// if we do not free this property's memory here, nothing
			// will never do...
			Destroy();
		} else {
			// the memory associated with this property will be freed
			// by the node which owns this object
		}
	}


	//! Returns TRUE if the two wxXml2Properties have the same name (case insensitive
	//! comparison is performed) and same values (case sensitive comparison).
	bool operator==(const wxXml2Property &p) const;
	
	//! Returns NOT operator==.
	bool operator!=(const wxXml2Property &p) const		{ return !(*this == p); }

	//! builds this XML property
	void Create(const wxString &name, 
				const wxString &value,
				wxXml2Node &owner = wxXml2EmptyNode);


	///////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Getters
    wxString		GetName() const			{ return m_attr->name; }
    wxString		GetValue() const		{ return m_attr->children->content; }

    wxXml2Property	GetNext() const			{ return wxXml2Property(m_attr->next); }
	wxXml2Property	GetPrevious() const		{ return wxXml2Property(m_attr->prev); }

	wxXml2Node		GetOwner() const;	// cannot be implemented here: wxXml2Node not defined yet

	//! returns the XML structure wrapped by this class
	xmlAttr *GetObj() const					{ return m_attr; }
	
	//@}



	///////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Setters
    
    void SetValue(const wxString &value);
    void SetName(const wxString &name);

    void SetNext(wxXml2Property &next);
	void SetPrevious(wxXml2Property &prev);

	//! Links this class with a wxXml2Node: it will have the duty of destroy this object.
	void SetOwner(wxXml2Node &owner);
	
	//@}


	///////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Checkers
    
    //! Returns TRUE if this property is not linked to any wider XML tree.
	bool IsUnlinked();
	
	//@}
};



//! Represents an XML namespace. In this example:
//! \code
//!   <root xmlns="http://!www.w3.org/1999/xhtml">
//!     <e:child xmlns:e="http://!mysite.com/mynamespaces/e/">
//!       <e:subchild>content</e:subchild>
//!     </e:child>
//!     <child2 prop="value"/>
//!   </root>
//! \endcode
//! The 'e' declared on node 'child' is a namespace with an explicit prefix
//! (the 'e') and with hreference = http://!mysite.com/mynamespaces/e/.
//! The element 'root' instead defines a namespace, without any explicit prefix,
//! which has href==http://!www.w3.org/1999/xhtml.
//! The namespace of 'root' is then overridden by namespace 'e' in element
//! 'child' and all its subelements.
//!
class WXXMLDLLEXPORT wxXml2Namespace : public wxObject
{
	DECLARE_DYNAMIC_CLASS(wxXml2Namespace)

	//! the XML namespace wrapped by this class
	xmlNs *m_ns;

	//! the node which owns this structure
	wxXml2Node *m_owner;


	void Destroy() {
		if (m_ns) xmlFreeNs(m_ns);
		m_ns = NULL;
	}


public:

	//! constructs an empty namespace. It cannot be used until you Create() it.
	wxXml2Namespace() : m_ns(NULL), m_owner(NULL) {};

	//! constructs a namespaces and eventually set it as the namespace used
	//! by 'owner'. If 'owner' == wxXml2EmptyNode, then the namespace won't be
	//! attached to any XML tree. Use SetOwner to attach it to a specific node.
    wxXml2Namespace(const wxString &prefix, const wxString &uri,
		wxXml2Node &owner = wxXml2EmptyNode) : m_ns(NULL) {
		Create(prefix, uri, owner);
	}

	//! wraps a libxml2 namespace structure
	wxXml2Namespace(xmlNs *ns, wxXml2Node &owner) : m_ns(NULL), m_owner(NULL) {
		m_ns = ns;
		m_owner = &owner;
	}

	//! destroys this namespace if it is completely unlinked from a greater
	//! XML tree.
    virtual ~wxXml2Namespace() {}

	bool operator==(const wxXml2Namespace &ns) const;
	bool operator!=(const wxXml2Namespace &p) const		{ return !(*this == p); }

	//! creates a new namespace and eventually attach it to the given node.
	void Create(const wxString &prefix, const wxString &uri,
		wxXml2Node &owner);
		

	///////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Getters 
    
    wxString	GetPrefix() const			{ return m_ns->prefix; }
    wxString	GetURI() const				{ return m_ns->href; }
	//!wxXml2Node	GetOwner() const;

	//! returns the associated XML structure
	xmlNs		*GetObj() const				{ return m_ns; }
	
	//@}
	

	///////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Setters 
    
    void SetPrefix(const wxString &p);
    void SetURI(const wxString &u);
	//!void SetOwner(wxXml2Node &owner);
	
	//@}
};




//! This class holds XML data/document as parsed by the libxml2 parser.
//! Use #Load and #Save to perform IO on wxStream or on wxFile objects.
//! Use the #GetRoot() function to edit the contents of the XML
//! document through wxXml2Node functions.
class WXXMLDLLEXPORT wxXml2Document : public wxObject
{
	DECLARE_DYNAMIC_CLASS(wxXml2Document)

	//! the parsed document & the associated tree
	xmlDoc *m_doc;

public:

	//! constructs a new empty document ready to be used
    wxXml2Document() {
		m_doc = xmlNewDoc((xmlChar*)wxT("1.0"));
	};

	//! loads the given filename and parse it
    wxXml2Document(const wxString &filename) {
		m_doc = xmlParseFile(filename);		// WX2XML is not needed
	}

	//! wraps the given libxml2 structure
	wxXml2Document(xmlDoc *doc) {
		m_doc = doc;
	}

	//! destructor
    virtual ~wxXml2Document() {
		//! WARNING: this function does nothing !!!!
		//! If you want to delete the tree associated with
		//! this document you must use Destroy()
	}

	//! destroys this document & the associated tree
	void Destroy() {
		if (m_doc) xmlFreeDoc(m_doc);
		m_doc = NULL;
	}

	//! allows you to use == and != on this class.
	bool operator==(const wxXml2Document &doc) const;
	bool operator!=(const wxXml2Document &doc) const 	{ return !(*this == doc); }


    //! Parses the data from the given wxInputStream. See #Load().
    bool Load(wxInputStream& stream, wxString *pErr = NULL);

    //! Parses XML & XHTML file and loads data. Returns TRUE on success. 
    //! Returns FALSE otherwise & provide an error description if the given  
	//! pointer is not NULL.    
    bool Load(const wxString &filename, wxString *pErr = NULL);
    
    //! Saves the XML data in the given stream.
    bool Save(wxOutputStream& stream, const wxString &encoding = "UTF-8", 
		int indentstep = 1) const;

    //! Saves the document as XML or XHTML file in the given encoding format.
	//! Returns TRUE on success.
    bool Save(const wxString &filename, const wxString &encoding = wxT("UTF-8"), 
		int indentstep = 1) const;

	//! Returns TRUE if everything is okay in the XML tree structure.
    bool IsOk() const { return xmlDocGetRootElement(m_doc) != NULL; }

    //! Returns the root node of the document.
    wxXml2Node GetRoot() const;

    //! Returns version of document (may be empty).
    wxString GetVersion() const { return m_doc->version; }

    //! Returns encoding of document (may be empty).
    //! Note: this is the encoding original file was saved in, *not* the
    //! encoding of in-memory representation!
    wxString GetFileEncoding() const { return m_doc->encoding; }

    //! Changes the root of the document (deleting old one).
    void SetRoot(wxXml2Node &node);

	//! Returns the libxml2 underlying object.
	xmlDoc *GetObj() const { return m_doc; }
};





//! Represents the data owned by a wxXml2Node. wxXml2Node, in fact, uses reference
//! counting; in this way you can always use (and you should use) wxXml2Node directly
//! (without using pointers or references) like you would use wxString.
//! See wxXml2Node for more info.
//!
class WXXMLDLLEXPORT wxXml2NodeRefData : public wxObjectRefData
{
    friend class WXDLLEXPORT wxXml2Node;

public:
    wxXml2NodeRefData(xmlNode *p = NULL) {
		m_node = p;
	}
    wxXml2NodeRefData(const wxXml2NodeRefData& data) {
		m_node = xmlCopyNode(data.GetPtr(), 1);
	}
    ~wxXml2NodeRefData() {
		// check if this node is completely
		// unlinked from any other node
		if (IsUnlinked()) {
			//! completely unlinked; if we do not delete it,
			//! nothing will do.
			xmlUnlinkNode(m_node);
			xmlFreeNode(m_node);
			m_node = NULL;
		} else {
			// this node is still linked in some way to a greater XML tree;
			// something else (like the root node) will care about the
			// destruction of this node
		}
	}

	//! Returns the wrapped libxml2 structure.
	xmlNode *GetPtr() const		{ return m_node; }

	//! Is this node linked to a wider XML tree ?
	bool IsUnlinked() {
		if (m_node != NULL && m_node->parent == NULL && 
			m_node->prev == NULL &&	m_node->next == NULL)
			return TRUE;
		return FALSE;
	}

protected:
	xmlNode *m_node;
};


//! Represents node in XML document. Node has name and may have content
//! and properties. Most common node types are wxXML_TEXT_NODE (name and props
//! are irrelevant) and wxXML_ELEMENT_NODE (e.g. in <title>hi</title> there is
//! element with name="title", irrelevant content and one child (wxXML_TEXT_NODE
//! with content="hi").
//!
//! This class uses reference counting so you should always use it without using
//! pointers or references (except for const reference, which can be used for
//! a little performance gain):
//!
//! wxXml2Node MyXMLFunction(const wxXml2Node &myconstnode);
//! wxXml2Node MyXMLFunction2(wxXml2Node mynode);
//! 
//! Because of reference counting both these two functions are very quick: a
//! wxXml2Node requires just 8 bytes. If you want to know more about reference
//! counting, see wxWidgets's wxString overview in wxWidgets manual.
//! YOU CAN USE wxXml2Node JUST AS YOU WOULD USE wxString !!!!!
//!
//! If wxUSE_UNICODE is 0, all strings are encoded in the encoding given to Load
//! (default is UTF-8).
//!
class WXXMLDLLEXPORT wxXml2Node : public wxObject
{
	DECLARE_DYNAMIC_CLASS(wxXml2Node)

	//! builds a new XML node
	void Build(const wxXml2NodeType type,
				wxXml2Node &parent,
				wxXml2Document &document = wxXml2EmptyDoc, 
				wxXml2Namespace &ns = wxXml2EmptyNamespace,
				const wxString &name = wxEmptyString, 
				const wxString &content = wxEmptyString,
				const wxXml2Property &props = wxXml2EmptyProperty, 
				const wxXml2Node &next = wxXml2EmptyNode,
				const wxXml2Node &previous = wxXml2EmptyNode,
				const wxXml2Node &children = wxXml2EmptyNode);

	//! return the libxml2 structure associated with this element
	xmlNode *Node()			{ return m_refData ? ((wxXml2NodeRefData *)m_refData)->m_node : NULL; }
	xmlNode *Node()	const	{ return m_refData ? ((wxXml2NodeRefData *)m_refData)->m_node : NULL; }

public:

	//! Builds an empty node. If you need an empty node in a "read-only" operation
	//! (comparison, for example), you can use the global instance wxXml2EmptyNode.
	wxXml2Node() {
		// before this node can be used it must be constructed through
		// Create****() functions. See below.
	}

	//! Builds a new node as root element of the given document. The node is
	//! automatically set as wxXML_ELEMENT_NODE and will replace any other
	//! root node eventually set in the given document (the old node is
	//! destroyed). The root element of a document should have parent,
	//! next, previous == NULL.
    wxXml2Node(wxXml2Document doc,
				const wxString &name = wxEmptyString, 
				wxXml2Namespace ns = wxXml2EmptyNamespace) {
		CreateRoot(doc, name, ns);
	}

	//! Builds a new node as child of the given node.
    wxXml2Node(wxXml2NodeType type,
				wxXml2Node parent,
				const wxString &name, 
				const wxString &content = wxEmptyString,
				wxXml2Namespace ns = wxXml2EmptyNamespace) {
		CreateChild(type, parent, name, content, ns);
	}

	//! Builds a new node unlinked from the main tree. This node must be considered
	//! temporary until it is linked to another node in the main tree.
    wxXml2Node(wxXml2NodeType type, wxXml2Document doc,
				const wxString &name = wxEmptyString, 
				const wxString &content = wxEmptyString,
				wxXml2Namespace ns = wxXml2EmptyNamespace) {
		CreateTemp(type, doc, name, content, ns);
	}

	//! Wraps the given XML node. This function doesn't copy the given
	//! structure, it just attach it to this object.
	wxXml2Node(xmlNode *node) {
		m_refData = new wxXml2NodeRefData(node);
	}

	//! Clones the given XML node.
	wxXml2Node(const wxXml2Node &node) { 
		Ref(node);
	}

	virtual ~wxXml2Node() {}



	// overloaded operators
	wxXml2Node &operator=(const wxXml2Node &n);
	bool operator==(const wxXml2Node &node) const;
	bool operator!=(const wxXml2Node &p) const				{ return !(*this == p); }
	

	///////////////////////////////////////////////////////////////////////////////////////////////
    //! \name User-friendly creation. Use these when possible. 

	//! for two-step creation
	void CreateRoot(wxXml2Document doc,
				const wxString &name = wxEmptyString, 
				wxXml2Namespace ns = wxXml2EmptyNamespace,
				const wxXml2Property props = wxXml2EmptyProperty);
	
	void CreateChild(wxXml2NodeType type,
				wxXml2Node parent,
				const wxString &name, 
				const wxString &content = wxEmptyString,
				wxXml2Namespace ns = wxXml2EmptyNamespace,
				const wxXml2Property props = wxXml2EmptyProperty, 
				const wxXml2Node next = wxXml2EmptyNode);

	void CreateTemp(wxXml2NodeType type, 
				wxXml2Document doc,
				const wxString &name = wxEmptyString, 
				const wxString &content = wxEmptyString,
				wxXml2Namespace ns = wxXml2EmptyNamespace,
				const wxXml2Property props = wxXml2EmptyProperty);	

	//! creates a child which can contain other children
	wxXml2Node AddContainerChild(const wxString &name);

	//! this function creates a container child (named 'name'), which 
	//! contains a text child which contains the string 'content'.
	//! to avoid the creation of the container child and to create
	//! directly a text node, use wxEmptyString (or "") as name.
	wxXml2Node AddTextChild(const wxString &name, const wxString &content);

	//! creates a comment node: <!-- content -->
	wxXml2Node AddCommentChild(const wxString &content);

	//! creates a text node which contains one (or more) break line characters.
	//! In XML all characters in the content of the document are significant 
	//! including blanks and formatting line breaks, so if you want to add
	//! a break line you MUST create a text child with '\n' as content.
	//! The problem is that, if you use this function even once, libxml2 will
	//! automatically turn off its format algorithm for this node and all
	//! its children...
	wxXml2Node AddBreakLineChild(int breaklines = 1);

	//! to add properties & namespaces
	wxXml2Property AddProperty(const wxString &name, const wxString &value);
	wxXml2Namespace AddNamespace(const wxString &prefix, const wxString &href);
	
	//@}
	
	


	///////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Raw-creation functions 
    
	void AddProperty(wxXml2Property prop);
	void AddChild(wxXml2Node node);
	void AddPrevious(wxXml2Node node);
	
	//@}



	///////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Getters 
    
    wxXml2NodeType	GetType() const			{ return (wxXml2NodeType)((int)Node()->type); }
    wxString		GetName() const			{ return Node()->name; }
    wxString		GetContent() const		{ return Node()->content; }

    wxXml2Node		GetParent() const 		{ return wxXml2Node(Node()->parent); }
    wxXml2Node		GetChildren() const		{ return wxXml2Node(Node()->children); }
    wxXml2Node		GetFirstChild() const	{ return GetChildren(); }
    wxXml2Node		GetNext() const			{ return wxXml2Node(Node()->next); }
	wxXml2Node		GetPrevious() const		{ return wxXml2Node(Node()->prev); }

	wxXml2Document   GetDoc() const			{ return wxXml2Document(Node()->doc); }
    wxXml2Property	GetProperties() const	{ return wxXml2Property(Node()->properties); }
    wxXml2Namespace  GetNamespace() const	{ return wxXml2Namespace(Node()->ns, (wxXml2Node &)(*this)); }

	//! Returns the libxml2 node structure wrapped by this object. Use this function
	//! only if you know what to do with the returned structure.
	xmlNode *GetObj() const					{ return Node(); }

	//! returns TRUE if a property named 'propName' exists in this node and, in this
	//! case, returns its value in 'value'.
	bool GetPropVal(const wxString &propName, wxString *value) const;

	//! returns the value of the property named 'propName' if it exists, otherwise
	//! returns 'defaultVal'.
    wxString GetPropVal(const wxString &propName, const wxString &defaultVal) const;

	//! returns the n-th child named 'name', or wxXml2EmptyNode if it doesn't exist.
	wxXml2Node Get(const wxString &name, int n = 0)	{ return Find(name, wxT(""), n); }
	
	//@}



	///////////////////////////////////////////////////////////////////////////////////////////////
	//! \name Setters: still not tested all & unstable (many are not even coded yet)... 
	//! use Add***  functions instead !!!
    void SetType(wxXml2NodeType type)		{ Node()->type = (xmlElementType)((int)type); }
    void SetName(const wxString &name)		{ xmlNodeSetName(GetObj(), WX2XML(name)); }
    void SetContent(const wxString &cont)	{ xmlNodeSetContent(GetObj(), WX2XML(cont)); }
	//!void SetDoc(const wxXml2Document &doc)	{ Build(GetType(), GetParent(), doc, GetNamespace(), GetName(), GetContent(), GetProperties(), GetNext()); }

    //!void SetParent(wxXml2Node &parent)		{ Build(GetType(), parent, GetDoc(), GetNamespace(), GetName(), GetContent(), GetProperties(), GetNext()); }
    void SetChildren(const wxXml2Node &child);
    void SetNext(const wxXml2Node &next);
	void SetPrevious(const wxXml2Node &prev);

    void SetProperties(const wxXml2Property &prop);
    void SetNamespace(wxXml2Namespace ns);
    
    //@}
    


	///////////////////////////////////////////////////////////////////////////////////////////////
	//! \name Checkers 
	
	bool IsUnlinked() const					        { return ((wxXml2NodeRefData*)m_refData)->IsUnlinked(); }
	bool Contains(const wxXml2Node &tofind) const	{ return (Find(tofind) != wxXml2EmptyNode); }
    bool HasProp(const wxString &propName) const;
    
    //@}


	///////////////////////////////////////////////////////////////////////////////////////////////
	//! \name Miscellaneous 
	
	void MakeUpper();
	void MakeLower();

	//! Returns TRUE if the given node is identic to this one. Checks
	//! only content, name, namespace and type.
	bool Cmp(const wxXml2Node &node) const;

	//! Like Cmp() but this function checks only content, name and type.
	bool CmpNoNs(const wxXml2Node &node) const;

	//! Unlinks the old node from it's current context, prune the new one 
	//! at the same place. If cur was already inserted in a document it is 
	//! first unlinked from its existing context.
	void Replace(const wxXml2Node &newnode);

	//! Searches the occ-th occurence of a node with the given name & content, 
	//! among the children of this node. If bNS == TRUE, this function also checks
	//! the namespace, otherwise the namespace will be irrelevant:
	//!
	//!      <myns:mynode>[...]</myns:mynode>      and    <mynode>[...]</mynode>
	//!
	//! will both match this query:
	//!
	//!      Find("mynode", wxEmptyString, 0, FALSE);
	//!
	//! This function does not check neither properties, children (except for
	//! the first text child which is tested if content != wxEmptyString)
	//! nor siblings.
	wxXml2Node Find(const wxString &name, 
					const wxString &content = wxEmptyString,
					int occ = 0, bool bNS = TRUE) const;

	//! Searches the occ-th occurrence of a node like the given one among 
	//! all the children of this object. Also checks namespace if bNS == TRUE.
	wxXml2Node Find(const wxXml2Node &tofind, int occ = 0, bool bNS = TRUE) const;

	//! Encapsulates this node in another node with the given name; if the
	//! 
	//!      mynode.Encapsulate("newtag");
	//! 
	//! is called, then, mynode is
	//! 
	//! before:          <mynodetagname>[...]</mynodetagname>
	//! after:   <newtag><mynodetagname>[...]</mynodetagname></newtag>
	//!
	void Encapsulate(const wxString &nodename);
	
	//@}
};

#endif // _WX_XML2_H_
