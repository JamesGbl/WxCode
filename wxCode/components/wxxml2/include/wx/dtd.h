/////////////////////////////////////////////////////////////////////////////
// Name:        dtd.h
// Purpose:     wxXml2DTD and various DTD nodes wrappers
// Author:      Francesco Montorsi
// Created:     2005/1/1
// Last modify: 
// RCS-ID:      
// Copyright:   2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#ifndef _WX_DTD_H_
#define _WX_DTD_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "dtd.h"
#endif

// xml2 is absolutely required
#include "wx/xml2.h"


// defined later
class WXXMLDLLEXPORT wxXml2DTD;
class WXXMLDLLEXPORT wxXml2ElemDecl;
class WXXMLDLLEXPORT wxXml2AttrDecl;
class WXXMLDLLEXPORT wxXml2EntityDecl;
class WXXMLDLLEXPORT wxXml2ElemContent;
class WXXMLDLLEXPORT wxXml2Enumeration;


// global instances of empty objects
extern wxXml2DTD wxXml2EmptyDTD;
extern wxXml2ElemDecl wxXml2EmptyElemDecl;
extern wxXml2AttrDecl wxXml2EmptyAttrDecl;
extern wxXml2EntityDecl wxXml2EmptyEntityDecl;
extern wxXml2Enumeration wxXml2EmptyEnumeration;
extern wxXml2ElemContent wxXml2EmptyElemContent;



//! The possible types of a wxXml2ElemDecl object.
//! Synchronized with libxml2' xmlElementTypeVal.
enum wxXml2ElementTypeVal {
    wxXML_ELEMENT_TYPE_UNDEFINED = 0,
    wxXML_ELEMENT_TYPE_EMPTY = 1,
    wxXML_ELEMENT_TYPE_ANY,
    wxXML_ELEMENT_TYPE_MIXED,
    wxXML_ELEMENT_TYPE_ELEMENT
};


//! The possible types of a wxXml2ElemContent object.
//! Synchronized with libxml2' xmlElementContentType.
enum wxXml2ElementContentType {
	wxXML_ELEMENT_CONTENT_PCDATA = 1,
    wxXML_ELEMENT_CONTENT_ELEMENT,
    wxXML_ELEMENT_CONTENT_SEQ,
    wxXML_ELEMENT_CONTENT_OR,
};


//! The possible occurrence modes of a wxXml2ElemContent object.
//! Synchronized with libxml2' xmlElementContentOccur.
enum wxXml2ElementContentOccur {
    wxXML_ELEMENT_CONTENT_ONCE = 1,
    wxXML_ELEMENT_CONTENT_OPT,
    wxXML_ELEMENT_CONTENT_MULT,
    wxXML_ELEMENT_CONTENT_PLUS,
};


//! The possible types of a wxXml2AttrDecl.
enum wxXml2AttributeType {
    wxXML_ATTRIBUTE_CDATA = 1,
    wxXML_ATTRIBUTE_ID,
    wxXML_ATTRIBUTE_IDREF,
    wxXML_ATTRIBUTE_IDREFS,
    wxXML_ATTRIBUTE_ENTITY,
    wxXML_ATTRIBUTE_ENTITIES,
    wxXML_ATTRIBUTE_NMTOKEN,
    wxXML_ATTRIBUTE_NMTOKENS,
    wxXML_ATTRIBUTE_ENUMERATION,
    wxXML_ATTRIBUTE_NOTATION
};


//! The possible default modes for a wxXml2AttrDecl.
enum wxXml2AttributeDefault {
    wxXML_ATTRIBUTE_NONE = 1,
    wxXML_ATTRIBUTE_REQUIRED,
    wxXML_ATTRIBUTE_IMPLIED,
    wxXML_ATTRIBUTE_FIXED
};


//! The possible types of a wxXml2EntityDecl.
enum wxXml2EntityType {
    wxXML_INTERNAL_GENERAL_ENTITY = 1,
    wxXML_EXTERNAL_GENERAL_PARSED_ENTITY,
    wxXML_EXTERNAL_GENERAL_UNPARSED_ENTITY,
    wxXML_INTERNAL_PARAMETER_ENTITY,
    wxXML_EXTERNAL_PARAMETER_ENTITY,
    wxXML_INTERNAL_PREDEFINED_ENTITY,
};


//! The type of content of a wxXml2ElemDecl object.
class WXXMLDLLEXPORT wxXml2ElemContent : public wxXml2Wrapper
{
	DECLARE_DYNAMIC_CLASS(wxXml2ElemContent)

	//! The libxml2 structure which holds the data.
	xmlElementContent *m_cont;

protected:

	void Destroy() {
		if (m_cont) xmlFree(m_cont);
		SetAsEmpty();
	}

	void SetAsEmpty()
		{ m_cont = NULL; }

	void  Copy(const wxXml2ElemContent &n) {
		UnwrappingOld();
		m_cont = n.m_cont;
		JustWrappedNew();
	}

	int &GetPrivate() const
		{ return (int &)(m_cont); }

public:

	wxXml2ElemContent(xmlElementContent *cont = NULL) : m_cont(cont) {}
	wxXml2ElemContent(const wxString &name, wxXml2ElementContentType val)
		{ m_cont=NULL; Create(name, val); }

	virtual ~wxXml2ElemContent() { DestroyIfUnlinked(); }



	bool IsUnlinked() const
		{ return TRUE; }

	bool IsNonEmpty() const
		{ return m_cont != NULL; }


public:		// operators
	
	bool operator==(const wxXml2ElemContent &decl) const;
	bool operator!=(const wxXml2ElemContent &decl) const 	{ return !(*this == decl); }

	wxXml2ElemContent &operator=(const wxXml2ElemContent &decl)
		{ Copy(decl); return *this; }


public:		// miscellaneous

	void Create(const wxString &name, wxXml2ElementContentType val) {

		m_cont = xmlNewElementContent(WX2XML(name), (xmlElementContentType)val);
	}

	xmlElementContent *GetObj() const
		{ return m_cont; }
};



//! An XML attribute enumeration.
class WXXMLDLLEXPORT wxXml2Enumeration : public wxXml2Wrapper
{
	DECLARE_DYNAMIC_CLASS(wxXml2Enumeration)

	//! The libxml2 structure which holds the data.
	xmlEnumeration *m_enum;

protected:

	void Destroy() {
		if (m_enum) xmlFreeEnumeration(m_enum);
		SetAsEmpty();
	}

	void SetAsEmpty()
		{ m_enum = NULL; }

	void  Copy(const wxXml2Enumeration &n) {
		UnwrappingOld();
		m_enum = n.m_enum;
		JustWrappedNew();
	}

	int &GetPrivate() const
		{ return (int &)(m_enum); }

public:

	wxXml2Enumeration(xmlEnumeration *towrap = NULL) : m_enum(towrap) {}
	wxXml2Enumeration(const wxString &name, const wxXml2Enumeration &next)
		{ m_enum=NULL; Create(name, next); }
	wxXml2Enumeration(const wxString &list)
		{ m_enum=NULL; Create(list); }

	virtual ~wxXml2Enumeration() { DestroyIfUnlinked(); }



	bool IsUnlinked() const
		{ return TRUE; }

	bool IsNonEmpty() const
		{ return m_enum != NULL; }


public:		// operators
	
	bool operator==(const wxXml2Enumeration &decl) const;
	bool operator!=(const wxXml2Enumeration &decl) const 	{ return !(*this == decl); }

	wxXml2Enumeration &operator=(const wxXml2Enumeration &decl)
		{ Copy(decl); return *this; }


public:		// miscellaneous

	void Create(const wxString &name, const wxXml2Enumeration &next);
	void Create(const wxString &list);

	//! Appends another enumerated value to this one.
	void Append(const wxXml2Enumeration &e);

	xmlEnumeration *GetObj() const
		{ return m_enum; }
};




//! An element declaration.
//! This type of node is used only inside an inlined/external DTD.
class WXXMLDLLEXPORT wxXml2ElemDecl : public wxXml2BaseNode
{
	DECLARE_DYNAMIC_CLASS(wxXml2ElemDecl)

public:
	
	wxXml2ElemDecl() {}

	//! Wraps the given libxml2 structure.
	wxXml2ElemDecl(xmlElement *n) 
		{ m_obj = (wxXml2BaseNodeObj*)n; JustWrappedNew(); }

	//! Copies the given wrapper (does not imply the XML structure copy).
	wxXml2ElemDecl(const wxXml2ElemDecl &n)
		{ Copy(n); }
	
	//! Creates a new element declaration and automatically
	//! link it to the given parent.
	wxXml2ElemDecl(const wxXml2DTD &parent, const wxString &name, 
				wxXml2ElementTypeVal val = wxXML_ELEMENT_TYPE_ELEMENT, 
				const wxXml2ElemContent &content = wxXml2EmptyElemContent) 
		{ Create(parent, name, val, content); }

	virtual ~wxXml2ElemDecl() { DestroyIfUnlinked(); }


public:		// operators
	
	bool operator==(const wxXml2ElemDecl &decl) const;
	bool operator!=(const wxXml2ElemDecl &decl) const 	{ return !(*this == decl); }

	wxXml2ElemDecl &operator=(const wxXml2ElemDecl &decl)
		{ Copy(decl); return *this; }


public:		// miscellaneous

	void Create(const wxXml2DTD &parent, const wxString &name, 
				wxXml2ElementTypeVal val, const wxXml2ElemContent &content);

	xmlElement *GetObj() const
		{ return (xmlElement *)m_obj; }
};



//! An element declaration.
//! This type of node is used only inside an inlined/external DTD.
class WXXMLDLLEXPORT wxXml2AttrDecl : public wxXml2BaseNode
{
	DECLARE_DYNAMIC_CLASS(wxXml2AttrDecl)

	//! The libxml2 structure which holds the data.
	//xmlAttribute *m_attr;

public:

	wxXml2AttrDecl() {}

	//! Wraps the given libxml2 structure.
	wxXml2AttrDecl(xmlAttribute *n) 
		{ m_obj = (wxXml2BaseNodeObj*)n; JustWrappedNew(); }

	//! Copies the given wrapper (does not imply the XML structure copy).
	wxXml2AttrDecl(const wxXml2AttrDecl &n) 
		{ Copy(n); }
	
	//! Creates a new element declaration and automatically
	//! link it to the given parent.
	wxXml2AttrDecl(const wxXml2DTD &parent, const wxString &element,
				const wxString &name, 
				const wxXml2Namespace &ns = wxXml2EmptyNamespace,
				wxXml2AttributeType type = wxXML_ATTRIBUTE_CDATA, 
				wxXml2AttributeDefault def = wxXML_ATTRIBUTE_NONE,
				const wxString &defaultval = wxEmptyString, 
				const wxXml2Enumeration &e = wxXml2EmptyEnumeration) 
		{ Create(parent, element, name, ns, type, def, defaultval, e); }

	virtual ~wxXml2AttrDecl() { DestroyIfUnlinked(); }


public:		// operators
	
	bool operator==(const wxXml2AttrDecl &decl) const;
	bool operator!=(const wxXml2AttrDecl &decl) const 	{ return !(*this == decl); }

	wxXml2AttrDecl &operator=(const wxXml2AttrDecl &decl)
		{ Copy(decl); return *this; }


public:		// miscellaneous

	void Create(const wxXml2DTD &parent, const wxString &element,
				const wxString &name, const wxXml2Namespace &ns,
				wxXml2AttributeType type, wxXml2AttributeDefault def,
				const wxString &defaultval, const wxXml2Enumeration &e);

	xmlAttribute *GetObj() const
		{ return (xmlAttribute*)m_obj; }
};




//! An element declaration.
//! This type of node is used only inside an inlined/external DTD.
class WXXMLDLLEXPORT wxXml2EntityDecl : public wxXml2BaseNode
{
	DECLARE_DYNAMIC_CLASS(wxXml2EntityDecl)

	//! The libxml2 structure which holds the data.
	//xmlEntity *;

public:

	wxXml2EntityDecl() {}

	//! Wraps the given libxml2 structure.
	wxXml2EntityDecl(xmlEntity *n) 
		{ m_obj = (wxXml2BaseNodeObj*)n; JustWrappedNew(); }

	//! Copies the given wrapper (does not imply the XML structure copy).
	wxXml2EntityDecl(const wxXml2EntityDecl &n) 
		{ Copy(n); }
	

	wxXml2EntityDecl(const wxXml2DTD &parent, const wxString &name,
					wxXml2EntityType type = wxXML_INTERNAL_GENERAL_ENTITY, 
					const wxString &externalID = wxEmptyString,
					const wxString &systemID = wxEmptyString, 
					const wxString &content = wxEmptyString)
		{ Create(parent, name, type, externalID, systemID, content); }

	virtual ~wxXml2EntityDecl() { DestroyIfUnlinked(); }


public:		// operators
	
	bool operator==(const wxXml2EntityDecl &decl) const;
	bool operator!=(const wxXml2EntityDecl &decl) const 	{ return !(*this == decl); }

	wxXml2EntityDecl &operator=(const wxXml2EntityDecl &decl)
		{ Copy(decl); return *this; }


public:		// miscellaneous

	void Create(const wxXml2DTD &parent, const wxString &name,
				wxXml2EntityType type, const wxString &externalID,
				const wxString &systemID, const wxString &content);

	xmlEntity *GetObj() const
		{ return (xmlEntity *)m_obj; }
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
		SetAsEmpty();
	}

	void SetAsEmpty()
		{ m_dtd = NULL; }

	void Copy(const wxXml2DTD &dtd) {
		UnwrappingOld();
		m_dtd = dtd.m_dtd;
		JustWrappedNew();
	}

	int &GetPrivate() const
		{ return (int &)(m_dtd->_private); }

	//! This function is not public because FIXME
	void SetRoot(wxXml2BaseNode &node);

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
	wxXml2BaseNode GetRoot() const;

public:		// setters

    void SetName(const wxString &p);
	void SetSystemID(const wxString &u);
	void SetExternalID(const wxString &u);

	//! Sets the external URI for this DTD.
	//! \note The external URI is kept in the field named "SystemID" by libxml2.
	void SetExternalURI(const wxString &u)
		{ SetSystemID(u); }

	//! Sets the root node for this DTD.
	void SetRoot(wxXml2ElemDecl &node)
		{ SetRoot((wxXml2BaseNode &)node); }
	void SetRoot(wxXml2AttrDecl &node)
		{ SetRoot((wxXml2BaseNode &)node); }
	void SetRoot(wxXml2EntityDecl &node)
		{ SetRoot((wxXml2BaseNode &)node); }


public:		// DTD node creation made easy

	void AddElemDecl(const wxString &name, 
				wxXml2ElementTypeVal val = wxXML_ELEMENT_TYPE_ELEMENT, 
				const wxXml2ElemContent &content = wxXml2EmptyElemContent);

	void AddAttrDecl(const wxString &element,
				const wxString &name, 
				const wxXml2Namespace &ns = wxXml2EmptyNamespace,
				wxXml2AttributeType type = wxXML_ATTRIBUTE_CDATA, 
				wxXml2AttributeDefault def = wxXML_ATTRIBUTE_NONE,
				const wxString &defaultval = wxEmptyString, 
				const wxXml2Enumeration &e = wxXml2EmptyEnumeration);

	void AddEntityDecl(const wxString &name,
					wxXml2EntityType type = wxXML_INTERNAL_GENERAL_ENTITY, 
					const wxString &externalID = wxEmptyString,
					const wxString &systemID = wxEmptyString, 
					const wxString &content = wxEmptyString);
};


#endif		// _WX_DTD_H_