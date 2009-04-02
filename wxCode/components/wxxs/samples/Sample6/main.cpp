/*********************************************************************
 * Name:      main.cpp
 * Purpose:   SimpleTreeSample tutorial
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

xsSerializable* MakeTree( xsSerializable *parent, int levels )
{
	if( levels > 0 )
	{
		levels--;
		// add new instances of serializable object to given parent and pass these new instances
		// as parents for recursive call of this function ( operator << returns pointer to newly
		// added object ).
		MakeTree( *parent << new SerializableObject(), levels );
		MakeTree( *parent << new SerializableObject(), levels );
		
		// also member function of xsSerializable class can be used for this task as follows:
		//MakeTree( parent->AddChild( new SerializableObject() ), levels );
	}
	
	return parent;
}

void PrintTree( xsSerializable *parent, int level )
{
	level++;
	
	// iterate through children list of given parent
	SerializableList::compatibility_iterator node = parent->GetFirstChildNode();
	while( node )
	{
		SerializableObject *pObject = (SerializableObject*) node->GetData();
	
		// print info about current object...
		for( int i = 1; i < level; i++ ) wxPrintf( wxT(" ") );
		wxPrintf( pObject->m_sTextData << wxT("\n") );
		
		// .. and process the object's children if exist
		if( pObject->HasChildren() ) PrintTree( pObject, level );
		
		node = node->GetNext();
	}
}

int main( int argc, char ** argv )
{	
	// create instance of XML serializer
	wxXmlSerializer Serializer; 
	
	// first, create set of serializable class objects and add them to the serializer
	
	// add root item of a tree of serializable class objects to the serializer
	Serializer << MakeTree( new SerializableObject(), 3 );
	
	// also member function of wxXmlSerializer class can be used for this task as follows
	// (first NULL argument means that the object is added directly to the serializer's root):
	//Serializer.AddItem( (xsSerializable*)NULL, MakeTree( new SerializableObject(), 3 ) );
	
	// store the serializer's content to an XML file
	Serializer.SerializeToXml( wxT("data.xml") );
	
	// clear the serializer's content
	Serializer.RemoveAll();
	
	// now, re-create list of stored class instances from XML file (data.xml)
	Serializer.DeserializeFromXml( wxT("data.xml") );
	
	// print out info about loaded class instances
	PrintTree( Serializer.GetRootItem(), 0 );
	
	// wait at application's termination
	pause;
	
	return 0;
}
