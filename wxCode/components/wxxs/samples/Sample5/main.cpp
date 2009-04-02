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
	// RTTI must be provided
	DECLARE_DYNAMIC_CLASS(SerializableObject);

	// constructor
	SerializableObject();
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

// static data member
int SerializableObject::m_nCounter = 0;

// implementation of RTTI for serializable class
IMPLEMENT_DYNAMIC_CLASS( SerializableObject, xsSerializable );

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
	
	// print out info about loaded class instances
	SerializableList::compatibility_iterator node = Serializer.GetRootItem()->GetFirstChildNode();
	while( node )
	{
		SerializableObject *pObject = (SerializableObject*) node->GetData();
		
		wxPrintf( pObject->m_sTextData << wxT("\n") );
		
		node = node->GetNext();
	}
	
	// wait at application's termination
	pause;
	
	return 0;
}
