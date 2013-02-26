/*********************************************************************
 * Name:      main.cpp
 * Purpose:   SimpleListSample tutorial
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2009-03-13
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 *********************************************************************/

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include <wx/wx.h>
// wxXmlSerializer main header file
#include "wx/wxxmlserializer/XmlSerializer.h"

#define pause wxPrintf( wxT("Press ENTER to continue...\n") ); while( wxFgetc( stdin ) != '\n' );

/////////////////////////////////////////////////////////////////////////////////////
// SerializableObject class /////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

class SerializableObject : public xsSerializable
{
	// RTTI and Clone() function must be provided
	XS_DECLARE_CLONABLE_CLASS(SerializableObject);

	// constructor
	SerializableObject();
	// copy contructor must be provided for cloneability
	SerializableObject(const SerializableObject& obj);
	// destructor
	virtual ~SerializableObject() {;}
	
	// public data member
	wxString m_sTextData;
	
private:
	// private data member
	static int m_nCounter;
};

SerializableObject::SerializableObject()
{
	// initialize member data
	m_sTextData = wxString::Format( wxT("'SerializableObject' class instance No. %d"), m_nCounter++ );
	// mark the data members which should be serialized
	XS_SERIALIZE( m_sTextData, wxT("text") );
}

SerializableObject::SerializableObject(const SerializableObject& obj) : xsSerializable( obj )
{
	// initialize member data
	m_sTextData = obj.m_sTextData;
	// mark the data members which should be serialized
	XS_SERIALIZE( m_sTextData, wxT("text") );
}

// static data member
int SerializableObject::m_nCounter = 0;

// implementation of RTTI and Clone() function for serializable class
XS_IMPLEMENT_CLONABLE_CLASS( SerializableObject, xsSerializable );

/////////////////////////////////////////////////////////////////////////////////////
// Application entry point //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

int main( int argc, char ** argv )
{	
	// create instance of XML serializer
	wxXmlSerializer Serializer;
	
	// first, create set of serializable class objects and add them to the serializer
	for( int i = 0; i < 5; i++ )
	{
		// Add all new class objects to the serializer's root ( pointer to parent object is NULL )
		// so the instances will be arranged into a list. Note that each serializable object 
		// could be assigned as a child to another one so the objects could be arranged into a tree
		// structure as well.
		Serializer << new SerializableObject();
		
		// also member function of wxXmlSerializer could be used for this task as follows:
		//Serializer.AddItem( (xsSerializable*)NULL, new SerializableObject() );
	}
	
	// store the serializer's content to an XML file
	Serializer.SerializeToXml( wxT("data.xml") );
	
	// clear the serializer's content
	Serializer.RemoveAll();
	
	// now, re-create list of stored class instances from XML file (data.xml)
	Serializer.DeserializeFromXml( wxT("data.xml") );
	
	// if you declare serializable classes as clonable ones (using XS_DECLARE_CLONABLE_CLASS, etc), then you can
	// simply copy whole serializer's content in this way (or using its copy contructor or wxXmlSerializer::CopyItems() function):
	wxXmlSerializer Serializer2 = Serializer;
	
	// print out info about loaded class instances
	SerializableList::compatibility_iterator node = Serializer2.GetRootItem()->GetFirstChildNode();
	while( node )
	{
		SerializableObject *pObject = (SerializableObject*) node->GetData();
		
		// get object's member in a standard way:
		wxPrintf( wxT("1: ") + pObject->m_sTextData );
		
		// get object's member by using serializable properties (reference to data is returned):
		wxPrintf ( wxT("\n2: ") + pObject->GetProperty( wxT("text") )->AsString() );
		
		// get object's member encapsulated by a wxVariant object (if data type is supported):
		wxPrintf( wxT("\n3: ") + pObject->GetProperty( wxT("text") )->ToVariant().MakeString() );
		// NOTE: value of ANY data type SUPPORTED by wxXmlSerializer can be converted to/created from
		// its string representation via functions xsProperty::ToString() and xsProperty::FromString()
		
		wxPrintf( wxT("\n\n") );
		
		node = node->GetNext();
	}
	
	// alternativelly, all data managed by serializer can be inserted into serializable list
	SerializableList lst;
	Serializer2 >> lst;
	
	for( SerializableList::iterator it = lst.begin(); it != lst.end(); ++it )
	{
		wxPrintf( wxT("Data: %s\n"), ((SerializableObject*)*it)->m_sTextData.c_str() );
	}
	
	// wait at application's termination
	pause;
	
	return 0;
}
