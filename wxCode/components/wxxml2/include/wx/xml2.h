/////////////////////////////////////////////////////////////////////////////
// Name:        xml2.h
// Purpose:     libxml2 wrappers
// Author:      Francesco Montorsi (original design based on EXPAT by Vaclav Slavik)
// Created:     2000/03/05
// Last modify: 2004/12/18
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
class WXXMLDLLEXPORT wxXml2DTD;
class WXXMLDLLEXPORT wxXml2Document;


// this macro converts from wxStrings to xmlChars
#define WX2XML(str)		((xmlChar *)str.mb_str(wxConvUTF8))


//! Represents an XML node type. LibXML2 allows a lot of different element types,
//! but the following are the types you can use with the wxXml2 wrappers...
enum wxXml2NodeType
{
    // note: values are synchronized with xmlElementType from libxml2 2.5.x
	//       

	//! Container nodes; they are characterized by the NAME, the NAMESPACE
	//! and the CHILDRENs. They create something like:
	//!                  <NAME>children</NAME>  
	//! or (if namespace != wxXml2EmptyNamespace),
	//!                  <NS:NAME>children</NS:NAME>
	//! Examples:
	//!     wxXml2Node container(wxXML_ELEMENT_NODE, parentnode, "MYTAG");
	//!     container.AddChild(....);
	//!
    wxXML_ELEMENT_NODE =		1,

	//! Nodes containing text; they are characterized by VALUE (or CONTENT).
    wxXML_TEXT_NODE =			3,

	//! Creates something like: <![CDATA[content]]>
	//! To use this type of node, create a container node and then use the
	//! SetType(wxXML_CDATA_SECTION_NODE) function.
	//! To set content use wxXml2Node::SetContent.
	wxXML_CDATA_SECTION_NODE =	4,

	//! Like a text node, but this node contains only an "entity". Entities
	//! are strings like: &amp; or &quot; or &lt; ....
	//! To create them, use
	//!    wxXml2Node entityref(wxXML_TEXT_NODE, parentnode, "&amp;");
	//!    containernode.AddChild(entityref);
	wxXML_ENTITY_REF_NODE =		5,

	//! Creates a Processing Instruction (PI) node.
	//! Such type of nodes looks like: <?name content ?>
	//! To use this type of node, create a container node and then use SetType().
	//! To add 'properties', you cannot use wxXml2Property; instead use
	//! wxXml2Node::SetContent("property=\"value\"").
	wxXML_PI_NODE =				7,

	//! Creates a comment node: <!-- content -->.	
    wxXML_COMMENT_NODE =		8,


	//! The value of the "type" member of an xmlDoc structure.
	//! This value is used to identify a node as a wxXml2Document.
	//! Never use it directly: use wxXml2Document instead.
    wxXML_DOCUMENT_NODE =		9,
    wxXML_HTML_DOCUMENT_NODE =	13,		//!< Like #wxXML_DOCUMENT_NODE.

	//! The value of the "type" member of an xmlDtd structure.
	//! This value is used to identify a node as a wxXml2DTD.
	//! Never use it directly: use wxXml2DTD instead.
	wxXML_DTD_NODE =			14,


// these three element still needs to be wrapped:

	//! A DTD node which declares an element.
	//! Looks like:     <!ELEMENT mynode (#PCDATA)>
    wxXML_ELEMENT_DECL = 15,

	//! A DTD node which declares an attribute.
	//! Looks like:     <!ATTLIST myattr type #PCDATA "defaultvalue">
    wxXML_ATTRIBUTE_DECL = 16,

	//! A DTD node which declares an entity.
	//! Looks like:     <!ENTITY myentity "entity's replacement">
    wxXML_ENTITY_DECL = 17,


	//! The value of the "type" member of an xmlNs structure.
	//! This value is used to identify a node as a wxXml2Namespace.
	//! Never use it directly: use wxXml2Namespace instead.
    wxXML_NAMESPACE_DECL = 18

	// I don't even know when these one are used... sorry
    //wxXML_XINCLUDE_START = 19,
    //wxXML_XINCLUDE_END = 20
};



// global instances of empty objects
extern wxXml2Node wxXml2EmptyNode;
extern wxXml2Property wxXml2EmptyProperty;
extern wxXml2Namespace wxXml2EmptyNamespace;
extern wxXml2DTD wxXml2EmptyDTD;
extern wxXml2Document wxXml2EmptyDoc;




//! Provides some standard method for all wxXml2 wrappers.
class WXXMLDLLEXPORT wxXml2Wrapper : public wxObject
{
	DECLARE_ABSTRACT_CLASS(wxXml2Wrapper)

protected:

	//! Destroys the data wrapped by this class freeing its memory.
	//! This function should not try to update the reference count
	//! since it should have been already updated by the caller.
	virtual void Destroy() = 0;

	//! Copies the given object. This function should be used to implement
	//! copy constructor and assignment operators.
	/*virtual void Copy(const wxXml2Wrapper &tocopy) = 0;*/

	//! Returns the private member of the wrapped structure.
	virtual int &GetPrivate() const = 0;	

protected:		// reference counting utilities

	//! \page wxxml2 wxXml2Wrappers and reference counting.
	//! wxXml2Wrapper-derived classes does not use a full Copy-On-Write
	//! technique: when copying a wxXml2Wrapper you just create a new
	//! wxXml2Wrapper (which requires few bytes) which wraps the same
	//! libxml2 structure of the original wxXml2Wrapper.
	//! Consider the following code:
	//! \code
	//!     wxXml2Document doc;
	//!     wxXml2Node root(doc, "root_of_my_doc");
	//!     
	//!     // copy the root node
	//!     wxXml2Node copy(root);
	//!     copy.SetName("myroot");		// modify the copy
	//!
	//!     // save the document
	//!     doc.Save(....);
	//! \endcode
	//! The result won't be a document with a root named "root_of_my_doc"
	//! but a document with "myroot" as root's name.
	//! This is because deep copies are never performed, unlike, for example,
	//! in wxString's COW system where the string is shared until a non-const
	//! function is called.
	//! The libxml2 structure is always shared with wxXml2Wrapper.
	//! 
	//! However, a reference count is still required to avoid things like:
	//! \code
	//!     wxXml2Node mynode;
	//!     mynode.Create(...);
	//!     {
	//!         wxXml2Node copy(mynode);
	//!         [...]
	//!     } // "copy" is destroyed
	//!     mynode.AddChild(...);
	//! \endcode
	//! Without reference counting, in fact, the code above would create
	//! the "copy" node which would destroy its libxml2 node when being destroyed.
	//! This must be avoided because "mynode" is still wrapping that memory
	//! address: with reference counting, this is avoided.
	//! The reference count must be stored in the wrapped structure.
	//! How wxXml2Wrapper handle this problem ? Well, all libxml2 structure has
	//! a VOID* field called "_private" which can be safely used by external
	//! functions to hold user contents. The #GetPrivate() function casts that
	//! pointer to a reference to a integer so that it can be used as an int
	//! instead of a pointer to void.
	//!
	//! One last note about libxml2 structure destruction: if you read the
	//! wxXml2Wrapper::DestroyIfUnlinked function's sources, you'll notice
	//! that the wrapped libxml2 structure is not destroyed when the
	//! reference count reaches zero but the #IsUnlinked() function returns
	//! FALSE; why ? Because libxml2 implements "recursive destruction":
	//! when a node is destroyed, all its children are too.
	//! This means that we must also be careful not to break libxml2 memory
	//! representation destroying a node's child when it's still linked
	//! to its parent. To be exact, when a node (but also a property, a dtd,
	//! a namespace...) is part of a wider XML tree, we must *never* delete
	//! it when the relative wxXml2Wrapper is destroyed.
	//! This is because the wxXml2Wrappers are built on the fly by the getters
	//! of another wxXml2Wrapper: a libxml2 node does not have automatically
	//! associated a wxXml2Node and thus a tree composed of wxXml2Nodes does
	//! not exist in memory. If we would delete a libxml2 node in the wxXml2Node's
	//! destructor when it's still linked to a wider tree we would destroy

	void ResetRefCount()
		{ if (IsNonEmpty()) GetPrivate() = 0; }

	void IncRefCount()
		{ if (IsNonEmpty()) GetPrivate()++; }

	void DecRefCount()
		{ if (IsNonEmpty()) GetPrivate()--; }

	int GetRefCount() const
		{ if (IsNonEmpty()) return (int)(GetPrivate()); else return 0; }

	//! This function should be called each time the internal pointer
	//! of the class is going to be replaced by another one.
	void UnwrappingOld()
		{ DestroyIfUnlinked(); }

	//! This function should be called each time the internal pointer
	//! of the class has been replaced by another one.
	void JustWrappedNew()
		{ IncRefCount(); }

public:

	//! Constructs this wrapper but not its wrapped data.
	wxXml2Wrapper() {}

	//! The wxXml2Wrapper destructor will free the memory of the underlying
	//! libxml2 structure only if it is not linked to any XML tree.
	//! To perform this behaviour, the derived class MUST always call the
	//! #DestroyIfUnlinked function in its destructor (it cannot be called here).
	virtual ~wxXml2Wrapper() {}


	//! Returns TRUE if this object is unlinked from an XML tree.
	virtual bool IsUnlinked() const = 0;

	//! Returns TRUE if the wrapped element is not NULL.
	virtual bool IsNonEmpty() const = 0;

	//! Destroys the wrapped data if it is unlinked.
	virtual void DestroyIfUnlinked();
};



//! Represents a node property.
//! Example: in 
//!                    <img src="hello.gif" id="3"/>
//! "src" is property with value "hello.gif" and "id" is a property with value "3".
class WXXMLDLLEXPORT wxXml2Property : public wxXml2Wrapper
{
	DECLARE_DYNAMIC_CLASS(wxXml2Property)

	//! The XML property wrapped by this class.
	xmlAttr *m_attr;

protected:

	void Destroy() {
		if (m_attr) xmlRemoveProp(m_attr);
		m_attr = NULL;
	}

	void Copy(const wxXml2Property &prop)
		{ UnwrappingOld(); m_attr = prop.m_attr; JustWrappedNew(); }

	int &GetPrivate() const
		{ return (int &)(m_attr->_private); }

public:

	//! Constructs an empty property; it cannot be used until you #Create() it.
	wxXml2Property() : m_attr(NULL) {}

	//! Constructs a property associated with the given node.
    wxXml2Property(const wxString &name, 
					const wxString &value,
					wxXml2Node &owner = wxXml2EmptyNode) : m_attr(NULL)
		{ Create(name, value, owner); }

	//! Wraps the given XML structure.
	wxXml2Property(xmlAttr *pattr) : m_attr(pattr) 
		{ JustWrappedNew(); }

	//! Copies the given wrapper (does not imply the XML structure copy).
	wxXml2Property(const wxXml2Property &prop) : m_attr(NULL)
		{ Copy(prop); }

	//! Destructor.
	virtual ~wxXml2Property() { DestroyIfUnlinked(); }


public:		// operators

	//! Returns TRUE if the two wxXml2Properties have the same name (case insensitive
	//! comparison is performed) and same values (case sensitive comparison).
	bool operator==(const wxXml2Property &p) const;
	
	//! Returns NOT operator==.
	bool operator!=(const wxXml2Property &p) const		{ return !(*this == p); }

	//! Assignement operator.
	wxXml2Property &operator=(const wxXml2Property &p)
		{ Copy(p); return *this; }

public:		// miscellaneous

	//! Builds this XML property.
	void Create(const wxString &name, 
				const wxString &value,
				wxXml2Node &owner = wxXml2EmptyNode);
    
    //! Returns TRUE if this property is not linked to any wider XML tree.
	bool IsUnlinked() const;

	//! Returns TRUE if this object is wrapping a non-NULL object.
	bool IsNonEmpty() const
		{ return m_attr != NULL; }


public:		// getters

    wxString		GetName() const			{ return m_attr->name; }
    wxString		GetValue() const
		{ if (m_attr->children) return m_attr->children->content; return wxEmptyString; }

    wxXml2Property	GetNext() const			{ return wxXml2Property(m_attr->next); }
	wxXml2Property	GetPrevious() const		{ return wxXml2Property(m_attr->prev); }

	wxXml2Node		GetOwner() const;	// cannot be implemented here: wxXml2Node not defined yet

	//! Returns the XML structure wrapped by this class
	xmlAttr *GetObj() const					{ return m_attr; }
	

public:		// setters    

    void SetValue(const wxString &value);
    void SetName(const wxString &name);

    void SetNext(wxXml2Property &next);
	void SetPrevious(wxXml2Property &prev);

	//! Links this class with a wxXml2Node: it will have the duty of destroy this object.
	void SetOwner(wxXml2Node &owner);
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
class WXXMLDLLEXPORT wxXml2Namespace : public wxXml2Wrapper
{
	DECLARE_DYNAMIC_CLASS(wxXml2Namespace)

	//! The XML namespace wrapped by this class.
	xmlNs *m_ns;

	//! The node which owns this structure.
	//! This variable is requird since the xmlNs structure
	//! does not contain anything which can be used to determine
	//! if the namespace is linked or unlinked.
	xmlNode *m_owner;

protected:

	void Destroy() {
		if (m_ns) xmlFreeNs(m_ns);
		m_ns = NULL;
	}

	void Copy(const wxXml2Namespace &ns) {
		UnwrappingOld();
		m_ns = ns.m_ns;
		m_owner = ns.m_owner;
		JustWrappedNew();
	}

	int &GetPrivate() const
		{ return (int &)(m_ns->_private); }

public:

	//! Constructs an empty namespace. It cannot be used until you Create() it.
	wxXml2Namespace() : m_ns(NULL), m_owner(NULL) {}

	//! Constructs a namespaces and eventually set it as the namespace used
	//! by 'owner'. If 'owner' == wxXml2EmptyNode, then the namespace won't be
	//! attached to any XML tree. Use SetOwner to attach it to a specific node.
    wxXml2Namespace(const wxString &prefix, const wxString &uri,
					wxXml2Node &owner = wxXml2EmptyNode) : m_ns(NULL), m_owner(NULL)
		{ Create(prefix, uri, owner); }

	//! Wraps a libxml2 namespace structure.
	wxXml2Namespace(xmlNs *ns, wxXml2Node &owner);

	//! Copies the given wrapper (does not imply the XML structure copy).
	wxXml2Namespace(const wxXml2Namespace &ns) : m_ns(NULL), m_owner(NULL)
		{ Copy(ns); }

	//! Destroys this namespace if it is completely unlinked from a greater
	//! XML tree.
    virtual ~wxXml2Namespace() { DestroyIfUnlinked(); }


public:		// operators

	bool operator==(const wxXml2Namespace &ns) const;
	bool operator!=(const wxXml2Namespace &p) const		{ return !(*this == p); }
	
	wxXml2Namespace &operator=(const wxXml2Namespace &n)
		{ Copy(n); return *this; }

public:		// miscellaneous

	//! Creates a new namespace and eventually attach it to the given node.
	void Create(const wxString &prefix, const wxString &uri, wxXml2Node &owner);
	
	//! Returns TRUE if this element is unlinked.
	//! Since namespace structures does not contain a parent pointer by
	//! themselves, a wxXml2Namespace also keeps trace of the linked nodes
	//! using an additional variable #m_owner.
	bool IsUnlinked() const
		{ return (m_owner == NULL) || (m_ns == NULL); }

	//! Returns TRUE if this object is wrapping a non-NULL object.
	bool IsNonEmpty() const
		{ return m_ns != NULL; }


public:		// getters
    
	wxString	GetPrefix() const			{ return m_ns->prefix; }
    wxString	GetURI() const				{ return m_ns->href; }	

	//! Returns the associated XML structure
	xmlNs		*GetObj() const				{ return m_ns; }
	

public:		// setters

    void SetPrefix(const wxString &p);
    void SetURI(const wxString &u);
};




//! Represents an XML Document Type Definition (DTD): you can find the
//! W3C recommendation for DTDs at http://www.w3.org/TR/REC-he.
//!
//! The purpose of a Document Type Definition is to define the legal building
//! blocks of an XML document. It defines the document structure with a list of 
//! legal elements.
//! A DTD can be declared inline in your XML document, or as an external reference.
//! Inlined DTDs look like:
//! \code
//!    <?xml version="1.0"?>
//!    <!DOCTYPE note [
//!      <!ELEMENT note (to,from,heading,body)>
//!      <!ELEMENT to      (#PCDATA)>
//!      <!ELEMENT from    (#PCDATA)>
//!      <!ELEMENT heading (#PCDATA)>
//!      <!ELEMENT body    (#PCDATA)>
//!    ]>
//! \endcode
//!
//! External DTDs are linked this way:
//! \code
//!    <!DOCTYPE root-element SYSTEM "systemID = URI for the DTD">
//! or
//!    <!DOCTYPE root-element PUBLIC "externalID" "externalURI">
//! \endcode
//!
//! wxXml2DTD can represent both a full DTD (see #IsFullDTD()) or just an external
//! reference (see #IsExternalReference()); in this last case, you can use the
//! wxXml2DTD::LoadFullDTD() function to transform this DTD into a full DTD.
//!
//! Last, you can also use wxXml2DTD as a special type of wxXml2Document: you can
//! #Load() or #Save() a DTD in the same way you can do for wxXml2Document.
//! wxXml2DTD::GetRoot() lets you navigate through the DTD, even if DTD manipulation
//! using wxXml2Node is not completely supported yet.
class WXXMLDLLEXPORT wxXml2DTD : public wxXml2Wrapper
{
	DECLARE_DYNAMIC_CLASS(wxXml2DTD)

	//! The XML DTD wrapped by this class.
	xmlDtd *m_dtd;

protected:

	void Destroy() {
		if (m_dtd) xmlFreeDtd(m_dtd);
		m_dtd = NULL;
	}

	void Copy(const wxXml2DTD &dtd) {
		UnwrappingOld();
		m_dtd = dtd.m_dtd;
		JustWrappedNew();
	}

	int &GetPrivate() const
		{ return (int &)(m_dtd->_private); }

public:

	//! Constructs an empty DTD. It cannot be used until you Create() it.
	wxXml2DTD() : m_dtd(NULL) {}

	//! Loads a DTD standalone file which then can be used to validate
	//! a wxXml2Document.
	wxXml2DTD(const wxString &file) : m_dtd(NULL) 
		{ Load(file); }

	//! Constructs a DTD with the given name, ExternalID and SystemID.
    wxXml2DTD(const wxXml2Document &doc,
				const wxString &name, const wxString &externalid,
				const wxString &systemid) : m_dtd(NULL) 
		{ Create(doc, name, externalid, systemid); }

	//! Wraps a libxml2 DTD structure
	wxXml2DTD(xmlDtd *dtd) : m_dtd(dtd)
		{ JustWrappedNew(); }

	//! Copies the given wrapper (does not imply the XML structure copy).
	wxXml2DTD(const wxXml2DTD &dtd) : m_dtd(NULL)
		{ Copy(dtd); }

	//! Destroys this DTD if it is completely unlinked from a greater
	//! XML tree.
    virtual ~wxXml2DTD() { DestroyIfUnlinked(); }


public:		// operators

	bool operator==(const wxXml2DTD &ns) const;
	bool operator!=(const wxXml2DTD &p) const		{ return !(*this == p); }

	wxXml2DTD &operator=(const wxXml2DTD &dtd)
		{ Copy(dtd); return *this; }

public:		// miscellaneous

	//! Creates a new DTD and eventually attach it to the given document.
	void Create(const wxXml2Document &doc,
				const wxString &name, const wxString &externalid,
				const wxString &systemid);


	//! Loads an external DTD from the given wxInputStream.
	//! If a document is attached to this DTD and the DTD is loaded
	//! correctly, then the document is attached to the new DTD.
	bool Load(wxInputStream &, wxString *pErr = NULL);
	
	//! A specialized version of the generic #Load function for files.
	bool Load(const wxString &filename, wxString *pErr = NULL);

	//! Saves the DTD into the given wxOutputStream.
	int Save(wxOutputStream &) const;

	//! A specialized version of the generic #Save function for files.
	bool Save(const wxString &filename) const;

	//! Loads the DTD specified by current SystemID or ExternalID+ExternalURI.
	//! Returns FALSE if this is not a reference to an external DTD or if
	//! the DTD specified by the current SystemID/ExternalURI could not be loaded.
	bool LoadFullDTD(wxString *pErr = NULL);


public:		// checkers

	//! Returns TRUE if this DTD is valid.
	bool IsOk() const;

	//! Returns TRUE if this object is wrapping a non-NULL object.
	bool IsNonEmpty() const
		{ return m_dtd != NULL; }

	//! Returns TRUE if this DTD is not linked as internal/external
	//! subset of any wxXml2Document.
	bool IsUnlinked() const
		{ if (!m_dtd) return TRUE; return (m_dtd->parent == NULL); }

	//! Returns TRUE if this DTD is not an entire DTD but only a
	//! reference to another resource containing the full DTD.
	//! See wxXml2DTD's class description for more info about the
	//! difference between inlined/external DTD.
	bool IsExternalReference() const;

	//! Returns TRUE if this is an external reference
	//! (see #IsExternalReference) and it is a SYSTEM entity.
	//! SYSTEM subsets are declared as follows:
	//!           <!DOCTYPE Name SYSTEM "systemID">
	//! SYSTEM subsets can be loaded using #LoadFullDTD.
	//! SYSTEM subsets always have a non-empty SystemID (see #GetSystemID)
	//! and an empty ExternalID (see #GetExternalID) and an empty ExternalURI.
	bool IsSystemSubset() const;

	//! Returns TRUE if this is an external reference
	//! (see #IsExternalReference) and it is a PUBLIC entity.
	//! PUBLIC subsets are declared as follows:
	//!        <!DOCTYPE Name PUBLIC "externalID" "externalURI">
	//! PUBLIC subsets can be loaded using #LoadFullDTD.
	//! PUBLIC subsets always have an empty SystemID (see #GetSystemID)
	//! and a non-empty ExternalID (see #GetExternalID) and a non-empty
	//! ExternalURI (see #GetExternalURI).
	bool IsPublicSubset() const;

	//! Returns TRUE if this DTD is a "full" DTD: that is, it's not
	//! a *reference* to an external DTD but it contains at least
	//! one declaration of a node type.
	//! A "full" DTD can be an inlined DTD obtained using the
	//! wxXml2Document::GetDTD() function or it could be a standalone
	//! DTD loaded using wxXml2DTD::Load() function.
	//! See wxXml2DTD's class description for more info about the
	//! difference between inlined/external DTD.
	bool IsFullDTD() const
		{ return !IsExternalReference(); }

public:		// getters
    
	//! Returns the externalID if this is a PUBLIC entity otherwise
	//! returns wxEmptyString.
	//! See #IsPublicSubset().
	wxString GetExternalID() const
		{ if (IsPublicSubset()) return m_dtd->ExternalID; return wxEmptyString; }

	//! Returns the external URI (Uniform Resource Indicator)
	//! if this is a PUBLIC entity otherwise returns wxEmptyString.
	//! See #IsPublicSubset().
	//! \note Libxml2 holds the external URI into the SystemID field of the
	//!       xmlDtd structure when this is a PUBLIC entity.
	wxString GetExternalURI() const
		{ if (IsPublicSubset()) return m_dtd->SystemID; return wxEmptyString; }

	//! Returns the systemID if this is a SYSTEM entity otherwise
	//! returns wxEmptyString.
	//! See #IsSystemSubset().
    wxString GetSystemID() const
		{ if (IsSystemSubset()) return m_dtd->SystemID; return wxEmptyString; }

	//! Returns the name of this DTD.
	//! This is always non-empty: both when this is a reference to
	//! a full external DTD and when this is an inlined DTD.
    wxString GetName() const			{ return m_dtd->name; }	

	//! Returns the associated XML structure.
	xmlDtd *GetObj() const				{ return m_dtd; }
	
	//! Returns the root node for this DTD.
	//! If this is just a reference to an external DTD,
	//! then the result is wxXml2EmptyNode.
	wxXml2Node GetRoot() const;

public:		// setters

    void SetName(const wxString &p);
	void SetSystemID(const wxString &u);

	void SetExternalID(const wxString &u);

	//! The external URI is kept in the field named "SystemID" by libxml2.
	void SetExternalURI(const wxString &u)
		{ SetSystemID(u); }
};



//! This class holds XML data/document as parsed by the libxml2 parser.
//! Use #Load and #Save to perform IO on wxStream or on wxFile objects.
//! Use the #GetRoot() function to edit the contents of the XML
//! document through wxXml2Node functions.
class WXXMLDLLEXPORT wxXml2Document : public wxXml2Wrapper
{
	DECLARE_DYNAMIC_CLASS(wxXml2Document)

	//! The parsed document & the associated tree.
	xmlDoc *m_doc;

protected:

	void Destroy() {
		if (m_doc) xmlFreeDoc(m_doc);
		m_doc = NULL;
	}

	void Copy(const wxXml2Document &doc) {
		UnwrappingOld();
		m_doc = doc.m_doc;
		JustWrappedNew();
	}

	int &GetPrivate() const
		{ return (int &)(m_doc->_private); }

public:

	//! Constructs a new empty document ready to be used.
	//! Unlike almost all other wxXml2Wrapper this function creates
	//! a new document ready to be used and not an empty one.
    wxXml2Document() 
		{ m_doc = xmlNewDoc((xmlChar*)wxT("1.0")); JustWrappedNew(); }

	//! Constructs a document and sets its root to the given node.
	wxXml2Document(wxXml2Node &root);

	//! Loads the given filename and parse it.
    wxXml2Document(const wxString &filename) 
		{ m_doc = xmlParseFile(filename); /* WX2XML is not needed */ JustWrappedNew(); }

	//! Wraps the given libxml2 structure.
	wxXml2Document(xmlDoc *doc) : m_doc(doc) 
		{ JustWrappedNew(); }

	//! Copies the given wrapper (does not imply the XML structure copy).
	wxXml2Document(const wxXml2Document &doc) : m_doc(NULL)
		{ Copy(doc); }

	//! Destructor.
    virtual ~wxXml2Document() 
		{ /*DestroyIfUnlinked();*/ }


public:		// operators
	
	bool operator==(const wxXml2Document &doc) const;
	bool operator!=(const wxXml2Document &doc) const 	{ return !(*this == doc); }

	wxXml2Document &operator=(const wxXml2Document &doc)
		{ Copy(doc); return *this; }


public:		// load & save

    //! Parses the data from the given wxInputStream. See #Load().
    bool Load(wxInputStream &stream, wxString *pErr = NULL);

    //! Parses XML & XHTML file and loads data. Returns TRUE on success. 
    //! Returns FALSE otherwise & provide an error description if the given  
	//! pointer is not NULL.
    bool Load(const wxString &filename, wxString *pErr = NULL);
    
    //! Saves the XML data in the given stream with the given encoding.
	//! Returns the number of bytes written: -1 if there were errors.
    int Save(wxOutputStream &stream, const wxString &encoding = wxT("UTF-8"), 
		int indentstep = 1) const;

    //! Saves the document as XML or XHTML file in the given encoding format.
	//! Returns TRUE on success.
    bool Save(const wxString &filename, const wxString &encoding = wxT("UTF-8"), 
		int indentstep = 1) const;


public:		// miscellaneous

	//! Returns TRUE if this object is wrapping a non-NULL object.
	bool IsNonEmpty() const
		{ return m_doc != NULL; }

	//! Returns TRUE if everything is okay in the XML tree structure.
    bool IsOk() const 
		{ return xmlDocGetRootElement(m_doc) != NULL; }

	//! Returns always TRUE since documents are entities which contain
	//! links to other structures (wxXml2DTD, wxXml2Node...) but a doc
	//! is never linked with structures that are not own by itself.
	bool IsUnlinked() const
		{ return TRUE; }

	//! Checks if this document is conform to the DTD which it holds.
	//! \param bUseInternal If this parameter is set to 1, then the
	//!        internal subset (m_doc->intSubset) will be used.
	//!        If this parameter is set to 0, then the external subset
	//!        (m_doc->extSubset) will be used.
	//!        If this parameter is set to -1, then the subset will be
	//!        chosen automatically (the first valid one).
	//! \note Libxml2 does not specify if a document can hold both
	//!       an internal and an external subset. Anyway, the internal
	//!       one should be the preferred choice; besides, it is the
	//!       DTD returned by #GetDTD() function.
	bool IsDTDValid(wxString *err = NULL, int bUseInternal = 0) const;

public:		// getters

    //! Returns the root node of the document.
    wxXml2Node GetRoot() const;

	//! Returns the internal DTD associated with this document.
	//! If no internal DTDs are associated then wxXml2EmptyDTD is returned.
	//! \note The DTD associated with this document could be only a
	//!       simple line like:
	//!            <!DOCTYPE mydtd SYSTEM "mydtd.dtd">
	//!       to intercept this case, you should use the 
	//!       wxXml2DTD::IsExternalReference() function.	
	wxXml2DTD GetDTD() const;

    //! Returns version of document (may be empty).
    wxString GetVersion() const			{ return m_doc->version; }

    //! Returns encoding of document (may be empty).
    //! Note: this is the encoding original file was saved in, *not* the
    //! encoding of in-memory representation!
    wxString GetFileEncoding() const	{ return m_doc->encoding; }

	//! Returns the libxml2 underlying object.
	xmlDoc *GetObj() const				{ return m_doc; }


public:		// setters

    //! Changes the root of the document (deleting old one).
    void SetRoot(wxXml2Node &node);

	//! Sets the DTD for this document.
	void SetDTD(wxXml2DTD &dtd);

	//! Creates a PI node of the type:
	//!   <?xml-stylesheet type="text/xsl" href="xslfile"?>
	//! where "xslfile" is the given URI to an XSL file.
	void SetStyleSheet(const wxString &xslfile);

public:		// some common DTDs

	//! Sets the MathML DTD for this document.
	void SetMathMLDTD();

	//! Sets the XHTML DTD for this document.
	void SetXHTMLStrictDTD();
	void SetXHTMLTransitionalDTD();
	void SetXHTMLFrameSetDTD();
};




//! Represents node in XML document. Node has name and may have content
//! and properties. Most common node types are wxXML_TEXT_NODE (name and props
//! are irrelevant) and wxXML_ELEMENT_NODE (e.g. in <title>hi</title> there is
//! element with name="title" and with irrelevant content and one child 
//! (of type=wxXML_TEXT_NODE with content="hi").
//!
//! \todo Add better Unicode support.
//!
class WXXMLDLLEXPORT wxXml2Node : public wxXml2Wrapper
{
	DECLARE_DYNAMIC_CLASS(wxXml2Node)

	//! The libxml2 structure we are wrapping.
	xmlNode *m_node;

protected:

	//! Builds a new XML node settings ALL its possible attributes:
	//! the type (wxXml2NodeType), the parent, the children, the
	//! next & previous nodes (wxXml2Node), the properties (wxXml2Property),
	//! the parent document (wxXml2Document), the name & content (wxString).
	void Build(const wxXml2NodeType type,
				wxXml2Node &parent,
				wxXml2Document &document = wxXml2EmptyDoc, 
				wxXml2Namespace &ns = wxXml2EmptyNamespace,
				const wxString &name = wxEmptyString, 
				const wxString &content = wxEmptyString,
				wxXml2Property &props = wxXml2EmptyProperty, 
				wxXml2Node &next = wxXml2EmptyNode,
				wxXml2Node &previous = wxXml2EmptyNode,
				wxXml2Node &children = wxXml2EmptyNode);

	void Destroy() {
		xmlUnlinkNode(m_node);
		xmlFreeNode(m_node);
		m_node = NULL;
	}

	void Copy(const wxXml2Node &n) {
		UnwrappingOld();
		m_node = n.m_node;
		JustWrappedNew();
	}

	int &GetPrivate() const
		{ return (int &)(m_node->_private); }

public:

	//! Builds an empty node. If you need an empty node in a "read-only" operation
	//! (comparison, for example), you can use the global instance wxXml2EmptyNode.
	wxXml2Node() {
		// before this node can be used it must be constructed through
		// Create****() functions. See below.
		m_node = NULL;
	}

	//! Builds a new node as root element of the given document. The node is
	//! automatically set as wxXML_ELEMENT_NODE and will replace any other
	//! root node eventually set in the given document (the old node is
	//! destroyed). The root element of a document should have parent,
	//! next, previous == NULL.
    wxXml2Node(wxXml2Document &doc,
				const wxString &name = wxEmptyString, 
				wxXml2Namespace &ns = wxXml2EmptyNamespace) : m_node(NULL)
		{ CreateRoot(doc, name, ns); }

	//! Builds a new node as child of the given node.
    wxXml2Node(wxXml2NodeType type,
				wxXml2Node &parent,
				const wxString &name, 
				const wxString &content = wxEmptyString,
				wxXml2Namespace &ns = wxXml2EmptyNamespace) : m_node(NULL)
		{ CreateChild(type, parent, name, content, ns);	}

	//! Builds a new node unlinked from the main tree. This node must be considered
	//! temporary until it is linked to another node in the main tree.
    wxXml2Node(wxXml2NodeType type, wxXml2Document &doc,
				const wxString &name = wxEmptyString, 
				const wxString &content = wxEmptyString,
				wxXml2Namespace &ns = wxXml2EmptyNamespace) : m_node(NULL)
		{ CreateTemp(type, doc, name, content, ns);	}

	//! Wraps the given XML node. This function doesn't copy the given
	//! structure, it just attach it to this object.
	wxXml2Node(xmlNode *node) : m_node(NULL)
		{ m_node = node; JustWrappedNew(); }

	//! Copies the given wrapper (does not imply the XML structure copy).
	wxXml2Node(const wxXml2Node &n) : m_node(NULL)
		{ Copy(n); }

	virtual ~wxXml2Node() { DestroyIfUnlinked(); }


public:		// operators
	
	bool operator==(const wxXml2Node &node) const;
	bool operator!=(const wxXml2Node &p) const				{ return !(*this == p); }
	
	wxXml2Node &operator=(const wxXml2Node &n)
		{ Copy(n); return *this; }

	//! Returns TRUE if this object is wrapping a non-NULL object.
	bool IsNonEmpty() const
		{ return m_node != NULL; }

public:

	///////////////////////////////////////////////////////////////////////////////////////////////
    //! \name User-friendly creation.
	//! These functions are safe & powerful. Use these when possible. @{

	//! Creates this node as the ROOT of the given document.
	//! The XML root of a document is the node containing all other nodes.
	//! It can be preceded only by some special nodes. Consider the following:
	//! \code
	//! 
	//!   // this type of node is handled by wxXml2Document
	//!   <?xml version="1.0" encoding="UTF-8"?>
	//! 
	//!   // this type of node is a wxXML_PI_NODE
	//!   <?xml-stylesheet type="text/xsl" href="mathml.xsl"?>
	//!
	//!   // this type of node is handled by wxXml2DTD
	//!   <!DOCTYPE some PUBLIC "URI" "URI2">
	//! 
	//!   // our ROOT node
	//!   <root> .... </root>
	//! \endcode
	//!
	//! It's valid since the ROOT is preceded only by wxXML_PI_NODEs and a
	//! DTD declaration. If you see something like:
	//! \code
	//! 
	//!   <?xml version="1.0" encoding="UTF-8"?>
	//!   <root>....</root>
	//!   <anothernode></anothernode>
	//! \endcode
	//!
	//! Then it's wrong: the ROOT node must contain all other container nodes.	
	void CreateRoot(wxXml2Document &doc,
				const wxString &name = wxEmptyString, 
				wxXml2Namespace &ns = wxXml2EmptyNamespace,
				wxXml2Property &props = wxXml2EmptyProperty);
		
	//! Creates a node which is attached to the given parent as one of its children.	
	void CreateChild(wxXml2NodeType type,
				wxXml2Node &parent,
				const wxString &name, 
				const wxString &content = wxEmptyString,
				wxXml2Namespace &ns = wxXml2EmptyNamespace,
				wxXml2Property &props = wxXml2EmptyProperty, 
				wxXml2Node &next = wxXml2EmptyNode);

	//! Creates a temporary node unlinked from any XML tree (the wxXml2Document
	//! is required but it is not modified) which can be used to temporary
	//! store some data. Anyway, such node will be destroyed if you don't link
	//! it to a wider XML tree.
	void CreateTemp(wxXml2NodeType type, 
				wxXml2Document &doc,
				const wxString &name = wxEmptyString, 
				const wxString &content = wxEmptyString,
				wxXml2Namespace &ns = wxXml2EmptyNamespace,
				wxXml2Property &props = wxXml2EmptyProperty);	

	//! Creates a child which can contain other children.
	wxXml2Node AddContainerChild(const wxString &name);

	//! Creates a container child (named 'name'), which 
	//! contains a text child which contains the string 'content'.
	//! to avoid the creation of the container child and to create
	//! directly a text node, use wxEmptyString (or "") as name.
	wxXml2Node AddTextChild(const wxString &name, const wxString &content);

	//! Creates a comment node: <!-- content -->.
	wxXml2Node AddCommentChild(const wxString &content);

	//! Creates a CDATA node: <![CDATA[content]]>.
	wxXml2Node AddCDATAChild(const wxString &content);

	//! Creates a PI node: <?name content ?>.
	wxXml2Node AddPIChild(const wxString &name, const wxString &content);

	//! Creates a text node which contains one (or more) break line characters.
	//! In XML all characters in the content of the document are significant 
	//! including blanks and formatting line breaks, so if you want to add
	//! a break line you MUST create a text child with '\n' as content.
	//! The problem is that, if you use this function even once, libxml2 will
	//! automatically turn off its format algorithm for this node and all
	//! its children...
	wxXml2Node AddBreakLineChild(int breaklines = 1);

	//! Creates a new wxXml2Property with the given name & value (NAME="value")
	//! and attaches it to the properties of this node.
	wxXml2Property AddProperty(const wxString &name, const wxString &value);

	//! Creates a new wxXml2Namespace with the given prefic & href and sets
	//! it as the namespace of this node.
	wxXml2Namespace AddNamespace(const wxString &prefix, const wxString &href);
	
	//@}


	///////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Raw-creation functions 
	//! These functions should append the given objects at the end of the
	//! libxml2 node's list. Avoid them when possible. @{
    
	//! Links a new property to this node.
	void AddProperty(wxXml2Property &prop);

	//! Links a new child to this node.
	void AddChild(wxXml2Node &node);

	//! Links the given node as previous node.
	void AddPrevious(wxXml2Node &node);

	//! Links the given node as next node.
	void AddNext(wxXml2Node &node);
	
	//@}


public:		// getters

	///////////////////////////////////////////////////////////////////////////////////////////////
    //! \name Getters 
	//! These functions wrap on the fly the internal pointers of the libxml2
	//! structure and then return them (this is why they return objects and not
	//! references to objects). @{
    
    wxXml2NodeType	GetType() const			{ return (wxXml2NodeType)((int)m_node->type); }
    wxString		GetName() const			{ return m_node->name; }
    wxString		GetContent() const		{ return m_node->content; }

    wxXml2Node		GetParent() const 		{ return wxXml2Node(m_node->parent); }
    wxXml2Node		GetChildren() const		{ return wxXml2Node(m_node->children); }
    wxXml2Node		GetFirstChild() const	{ return GetChildren(); }
    wxXml2Node		GetNext() const			{ return wxXml2Node(m_node->next); }
	wxXml2Node		GetPrevious() const		{ return wxXml2Node(m_node->prev); }

	wxXml2Document  GetDoc() const			{ return wxXml2Document(m_node->doc); }
    wxXml2Property	GetProperties() const	{ return wxXml2Property(m_node->properties); }

    wxXml2Namespace GetNamespace() const		{ return wxXml2Namespace(m_node->ns, (wxXml2Node &)(*this)); }
	wxXml2Namespace GetNamespaceDecl() const	{ return wxXml2Namespace(m_node->nsDef, (wxXml2Node &)(*this)); }

	//! Returns the libxml2 node structure wrapped by this object. Use this function
	//! only if you know what to do with the returned structure.
	xmlNode *GetObj() const					{ return m_node; }

	//! Returns TRUE if a property named 'propName' exists in this node and, in this
	//! case, returns its value in 'value'.
	bool GetPropVal(const wxString &propName, wxString *value) const;

	//! Returns the value of the property named 'propName' if it exists, otherwise
	//! returns 'defaultVal'.
    wxString GetPropVal(const wxString &propName, const wxString &defaultVal) const;

	//! Returns the n-th child named 'name', or wxXml2EmptyNode if it doesn't exist.
	wxXml2Node Get(const wxString &name, int n = 0)	const
		{ return Find(name, wxT(""), n); }
	
	//@}


public:		// setters

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! \name Setters: still not tested all & unstable.
	//! These functions are dangereous since they can break the libxml2 internal
	//! tree if they are not used carefully. Try to avoid them. @{

    void SetType(wxXml2NodeType type)		{ m_node->type = (xmlElementType)((int)type); }
    void SetName(const wxString &name)		{ xmlNodeSetName(GetObj(), WX2XML(name)); }
    void SetContent(const wxString &cont)	{ xmlNodeSetContent(GetObj(), WX2XML(cont)); }

    void SetChildren(const wxXml2Node &child);
    void SetNext(const wxXml2Node &next);
	void SetPrevious(const wxXml2Node &prev);

    void SetProperties(const wxXml2Property &prop);
    void SetNamespace(wxXml2Namespace &ns);

	// these two would require the entire reconstruction of the node....
	//void SetDoc(const wxXml2Document &doc){ Build(GetType(), GetParent(), doc, GetNamespace(), GetName(), GetContent(), GetProperties(), GetNext()); }
    //void SetParent(wxXml2Node &parent){ Build(GetType(), parent, GetDoc(), GetNamespace(), GetName(), GetContent(), GetProperties(), GetNext()); }
    
    //@}
    

public:		// miscellaneous

	///////////////////////////////////////////////////////////////////////////////////////////////
	//! \name Miscellaneous
	//! Some miscellaneous functions which work on wxXml2Nodes. @{
	
	//! Returns TRUE if this node contains the given one.
	//! Obviously the function performs a recursive check.
	bool Contains(const wxXml2Node &tofind) const
		{ return (Find(tofind) != wxXml2EmptyNode); }

	//! Returns TRUE if this node has a property with the given name.
    bool HasProp(const wxString &propName) const;

	//! Is this node linked to a wider XML tree ?
	bool IsUnlinked() const;
	
	//! Makes the name of this tag all uppercase.
	//! Recursively updates the names of all children tags.
	void MakeUpper();

	//! Makes the name of this tag all lowercase.
	//! Recursively updates the names of all children tags.
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
