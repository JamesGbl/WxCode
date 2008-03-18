/////////////////////////////////////////////////////////////////////////////
// Name:        jsonval.cpp
// Purpose:     the wxJSON class that holds a JSON value
// Author:      Luciano Cattani
// Created:     2007/10/01
// RCS-ID:      $Id: jsonval.cpp,v 1.14 2008/03/12 10:48:19 luccat Exp $
// Copyright:   (c) 2007 Luciano Cattani
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "jsonval.cpp"
#endif

#include <wx/jsonval.h>

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include <wx/arrimpl.cpp>
#include <wx/log.h>
#include <wx/debug.h>


WX_DEFINE_OBJARRAY( wxJSONInternalArray );


// the trace mask used in ::wxLogTrace() function
static const wxChar* traceMask = _T("jsonval");
static const wxChar* compareTraceMask = _T("sameas");



/*! \class wxJSONValue
 \brief The JSON value class implementation.

JSON (JavaScript Object Notation) is a lightweight data-interchange format.
It is easy for humans to read and write. It is easy for machines to parse
and generate. It is based on a subset of the JavaScript Programming Language,
Standard ECMA-262 3rd Edition - December 1999. JSON is a text format that
is completely language independent but uses conventions that are familiar
to programmers of the C-family of languages, including C, C++, C#, Java,
JavaScript, Perl, Python, and many others. These properties make JSON an 
ideal data-interchange language.

This class holds a JSON value which may be of the following type:

\li wxJSONTYPE_EMPTY: an empty (not initialized) JSON value
\li wxJSONTYPE_NULL: a NULL value
\li wxJSONTYPE_INT: an integer value
\li wxJSONTYPE_UINT: an unsigned integer
\li wxJSONTYPE_DOUBLE: a double precision number
\li wxJSONTYPE_BOOL: a boolean
\li wxJSONTYPE_CSTRING: a C string
\li wxJSONTYPE_STRING: a wxString object
\li wxJSONTYPE_ARRAY: an array of wxJSONValue objects
\li wxJSONTYPE_OBJECT: a hashmap of key/value pairs where \e value is a wxJSONValue object

To know more about the internal representation of JSON values see
\ref pg_json_internals.
*/

#if defined( WXJSON_USE_VALUE_COUNTER )
// The progressive counter (used for debugging only)
int          wxJSONValue::sm_progr = 1;
#endif

//! Constructors.
/*!
 The overloaded constructors allow the user to construct a JSON value
 object that holds the specified value and type of value.
 The default ctor construct a valid JSON object that constains a \b null
 value.

 If you want to create an \b empty JSON value object you have to use the
 \c wxJSONValue( wxJSONType ) ctor.
 Note that this object is not a valid JSON value - to know more about this
 topic see the SetType function.

 \par The C-string JSON value object

 The wxJSONValue(const wxChar*) ctor allows you to create a JSON value
 object that contains a string value which is stored as a 
 \e pointer-to-statci-string.
 
 In fact, the ctor DOES NOT copy the string: it only stores the
 pointer in a data member and the pointed-to buffer is not deleted
 by the dtor.
 If the string is not static you have to use the wxJSONValue(const wxString&)
 constructor.

 Also note that this does NOT mean that the value stored in this JSON
 object cannot change: you can assign whatever other value you want,
 an integer, a double or an array of values.
 What I intended is that the pointed-to string must exist for the lifetime
 of the wxJSONValue object.
 The following code is perfectly legal:
 \code
   wxJSONvalue aString( "this is a static string" );
   aString = 10;
 \endcode
 To know more about this topic see \ref json_internals_cstring
*/
wxJSONValue::wxJSONValue()
{
  m_type = wxJSONTYPE_NULL;
  m_commentPos = wxJSONVALUE_COMMENT_BEFORE;
#if defined( WXJSON_USE_VALUE_CONTER )
  m_progr = sm_progr;
  ++sm_progr;
#endif
}

//! \overload wxJSONValue()
wxJSONValue::wxJSONValue( wxJSONType type )
{
  m_commentPos = wxJSONVALUE_COMMENT_BEFORE;
  m_type = wxJSONTYPE_EMPTY;    // avoid deleting old values in setType()
  SetType( type );

#if defined( WXJSON_USE_VALUE_CONTER )
  m_progr = sm_progr;
  ++sm_progr;
#endif
}

//! \overload wxJSONValue()
wxJSONValue::wxJSONValue( int i )
{
  m_commentPos = wxJSONVALUE_COMMENT_BEFORE;
  m_type = wxJSONTYPE_INT;
  m_value.m_valInt = i;
#if defined( WXJSON_USE_VALUE_CONTER )
  m_progr = sm_progr;
  ++sm_progr;
#endif
}


//! \overload wxJSONValue()
wxJSONValue::wxJSONValue( unsigned int ui )
{
  m_commentPos = wxJSONVALUE_COMMENT_BEFORE;
  m_type = wxJSONTYPE_UINT;
  m_value.m_valUInt = ui;
#if defined( WXJSON_USE_VALUE_COUNTER )
  m_progr = sm_progr;
  ++sm_progr;
#endif
}

//! \overload wxJSONValue()
wxJSONValue::wxJSONValue( bool b  )
{
  m_commentPos = wxJSONVALUE_COMMENT_BEFORE;
  m_type = wxJSONTYPE_BOOL;
  m_value.m_valBool = b;
#if defined( WXJSON_USE_VALUE_COUNTER )
  m_progr = sm_progr;
  ++sm_progr;
#endif
}

//! \overload wxJSONValue()
wxJSONValue::wxJSONValue( double d )
{
  m_commentPos = wxJSONVALUE_COMMENT_BEFORE;
  m_type = wxJSONTYPE_DOUBLE;
  m_value.m_valDouble = d;
#if defined( WXJSON_USE_VALUE_COUNTER )
  m_progr = sm_progr;
  ++sm_progr;
#endif
}

//! \overload wxJSONValue()
wxJSONValue::wxJSONValue( const wxChar* str )
{
  m_commentPos = wxJSONVALUE_COMMENT_BEFORE;
#if !defined( WXJSON_USE_CSTRING )
  m_type = wxJSONTYPE_STRING;
  m_value.m_valString = new wxString( str );
#else
  m_type = wxJSONTYPE_CSTRING;
  m_value.m_valCString = str;
#endif

#if defined( WXJSON_USE_VALUE_COUNTER )
  m_progr = sm_progr;
  ++sm_progr;
#endif
}

//! \overload wxJSONValue()
wxJSONValue::wxJSONValue( const wxString& str )
{
  m_commentPos = wxJSONVALUE_COMMENT_BEFORE;
  m_type = wxJSONTYPE_STRING;
  m_value.m_valString = new wxString( str );
#if defined( WXJSON_USE_VALUE_CONTER )
  m_progr = sm_progr;
  ++sm_progr;
#endif
}

//! Copy constructor.
wxJSONValue::wxJSONValue( const wxJSONValue& other )
{
  DeepCopy( other );
#if defined( WXJSON_USE_VALUE_COUNTER )
  m_progr = sm_progr;
  ++sm_progr;
#endif
}

//! Dtor.
wxJSONValue::~wxJSONValue()
{
  DeleteObj();
}

// functions for retreiving the value type


//! Return the type of the value stored in the object.
wxJSONType
wxJSONValue::GetType() const
{
  return m_type;
}

//! Set the type of the stored value.
/*!
 The function sets the type of the stored value as specified in
 the provided argument.
 If the actual type is equal to \c type, nothing happens and this
 JSON value object retains the original type and value.
 If the type differs, however, the original type and value are
 lost.
 The function just sets the type of the object and not the
 value itself. 
 This means that for primitive types, the content of the
 \c m_value data member is garbage.
 For complex types, the function also allocates a new,
 empty instance of the correct type (array, map or string).

 The \c type argument can be one of the following:

  \li wxJSONTYPE_EMPTY: an empty (not initialized) JSON value
  \li wxJSONTYPE_NULL: a NULL value
  \li wxJSONTYPE_INT: an integer value
  \li wxJSONTYPE_UINT: an unsigned integer
  \li wxJSONTYPE_DOUBLE: a double precision number
  \li wxJSONTYPE_BOOL: a boolean
  \li wxJSONTYPE_CSTRING: a C string
  \li wxJSONTYPE_STRING: a wxString object
  \li wxJSONTYPE_ARRAY: an array of wxJSONValue objects
  \li wxJSONTYPE_OBJECT: a hashmap of key/value pairs where \e value is a wxJSONValue object

 Note that there is no need to set a type for the object in order to assign
 a value to it.
 In other words, if you want to construct a JSON value which holds an integer
 value of 10, just use the specific constructor:
 \code
   wxJSONValue value( 10 );
 \endcode
 which sets the integer type and also the numeric value.
 Moreover, there is no need to set the type for none of the handled types,
 not only for pimitive types but for complex types, too.
 For example, if you want to construct an array of JSON values, just use
 the default ctor and call the Append() member function which will append the
 first element to the array and will set the array type:
 \code
   wxJSONValue value;
   value.Append( "a string" );
 \endcode

 The only exception is for empty (not initialized) values:
 \code
   // construct an empty value
   wxJSONValue v1( wxJSONTYPE_EMPTY );

   // this is the same
   wxJSONValue v2;
   v2.SetType( wxJSONTYPE_EMPTY );
 \endcode

 Maybe I have to spend some words about the (subtle) difference
 between an \b empty value, a \b null value and an \b empty 
 JSON \b object or \b array.

 A \b empty value is a JSONvalue object that was not initialized.
 Its type is \b wxJSONTYPE_EMPTY and it is used internally by the
 wxJSON library. You should never write empty values to JSON text
 because the output is not a valid JSON text.
 If you write an \e empty value to the wxJSONWriter class you 
 obtain the following text (which is not a JSON text):
 \code
   <empty>
 \endcode

 A \b null value is of type \b wxJSONTYPE_NULL and it is constructed
 using the default cosntructor.
 Its text output is valid JSON text:
 \code
   null
 \endcode

 An \b empty JSON \b object or an \b empty JSON \b array is a JSONvalue
 object of type wxJSONTYPE_OBJECT or wxJSONTYPE_ARRAY but they
 do not contain any element.
 Their output is valid JSON text:
 \code
   [ ]
   { }
 \endcode
*/
void
wxJSONValue::SetType( wxJSONType type )
{
  // only the type is set, the content is gargabe
  // for primitive types.
  // for complex types an object of the specified type
  // is created but it is empty

  // do nothing if the actual type is the same as 't'
  if ( m_type == type )  {
    return;
  }

  // check that type is within the allowed range
  wxASSERT((type >= wxJSONTYPE_EMPTY) && (type <= wxJSONTYPE_OBJECT));
  if ( (type < wxJSONTYPE_EMPTY) || (type > wxJSONTYPE_OBJECT) )  {
    type = wxJSONTYPE_EMPTY;
  }

  // first deletes actual content
  DeleteObj();
  m_type = type;

// if the WXJSON_USE_CSTRING macro is not defined, the class forces
// C-string to be stored as wxString objects
#if !defined( WXJSON_USE_CSTRING )
  if ( m_type == wxJSONTYPE_CSTRING )  {
    m_type = wxJSONTYPE_STRING;
  }
#endif

  switch ( m_type )  {
    case wxJSONTYPE_STRING :
      m_value.m_valString = new wxString();
      break;
    case wxJSONTYPE_ARRAY :
      m_value.m_valArray = new wxJSONInternalArray();
      break;
    case wxJSONTYPE_OBJECT :
      m_value.m_valMap = new wxJSONInternalMap();
      break;
    default :
      break;
  }
}

//! Return TRUE if the type of the value is wxJSONTYPE_NULL.
bool
wxJSONValue::IsNull() const
{
  bool r = false;
  if ( m_type == wxJSONTYPE_NULL )  {
    r = true;
  }
  return r;
}


//! Return TRUE if the value is of type wxJSONTYPE_EMPTY.
bool
wxJSONValue::IsEmpty() const
{
  bool r = false;
  if ( m_type == wxJSONTYPE_EMPTY )  {
    r = true;
  }
  return r;
}

//! Return TRUE if the type of the value stored is integer.
bool
wxJSONValue::IsInt() const
{
  bool r = false;
  if ( m_type == wxJSONTYPE_INT )  {
    r = true;
  }
  return r;
}

//! Return TRUE if the type of the value stored is a unsigned int.
bool
wxJSONValue::IsUInt() const
{
  bool r = false;
  if ( m_type == wxJSONTYPE_UINT )  {
    r = true;
  }
  return r;
}

//! Return TRUE if the type of the value stored is a boolean.
bool
wxJSONValue::IsBool() const
{
  bool r = false;
  if ( m_type == wxJSONTYPE_BOOL )  {
    r = true;
  }
  return r;
}

//! Return TRUE if the type of the value stored is a double.
bool
wxJSONValue::IsDouble() const
{
  bool r = false;
  if ( m_type == wxJSONTYPE_DOUBLE )  {
    r = true;
  }
  return r;
}

//! Return TRUE if the type of the value stored is a wxString object.
bool
wxJSONValue::IsString() const
{
  bool r = false;
  if ( m_type == wxJSONTYPE_STRING )  {
    r = true;
  }
  return r;
}

//! Return TRUE if the type of the value stored is a pointer to a static C string.
bool
wxJSONValue::IsCString() const
{
  bool r = false;
  if ( m_type == wxJSONTYPE_CSTRING )  {
    r = true;
  }
  return r;
}

//! Return TRUE if the type of the value stored is an array type.
bool
wxJSONValue::IsArray() const
{
  bool r = false;
  if ( m_type == wxJSONTYPE_ARRAY )  {
    r = true;
  }
  return r;
}

//! Return TRUE if the type of this value is a key/value map.
bool
wxJSONValue::IsObject() const
{
  bool r = false;
  if ( m_type == wxJSONTYPE_OBJECT )  {
    r = true;
  }
  return r;
}

// get the stored value

//! Return the stored value as an integer.
/*!
 The function returns the integer value stored in this JSON-value
 object.
 If the type of the stored value is not an integer, an unsigned int
 or a bool the function just returns the content of the stored value
 which may be garbage.
 In debug builds the function ASSERTs that the stored value is of
 compatible type: an \b int, an \b unsigned \b int or a \b boolean
*/
int
wxJSONValue::AsInt() const
{
  wxASSERT( m_type == wxJSONTYPE_INT || m_type == wxJSONTYPE_UINT
		|| m_type == wxJSONTYPE_BOOL );
  switch ( m_type )  {
    case wxJSONTYPE_BOOL :
    case wxJSONTYPE_INT :
      return m_value.m_valInt;
      break;
    case wxJSONTYPE_UINT :
      ::wxLogWarning( _T("wxJSONValue::AsInt() - value type is unsigned" ));
      return m_value.m_valInt;
      break;
    default :
      break;
  } 
  return m_value.m_valInt;
}

//! Return the stored value as a boolean.
/*!
 If the type of the stored value is not a boolean the function 
 just returns the content of the stored value which may be garbage.
 In debug builds the function ASSERTs that the stored value is of
 type \b boolean.
*/
bool
wxJSONValue::AsBool() const
{
  wxASSERT( m_type == wxJSONTYPE_BOOL );
  return m_value.m_valBool;
}

//! Return the stored value as a double.
/*!
 If the type of the stored value is not a double, an int or an
 unsigned int the function
 just returns the content of the stored value which may be garbage.
 In debug builds the function ASSERTs that the stored value is of
 compatible type: an \b int, an \b insigned \b int or a \b double.
*/
double
wxJSONValue::AsDouble() const
{
  wxASSERT( m_type == wxJSONTYPE_DOUBLE || m_type == wxJSONTYPE_INT || m_type == wxJSONTYPE_UINT );
  double d = m_value.m_valDouble;
  switch ( m_type )  {
    case wxJSONTYPE_INT :
      d = m_value.m_valInt;
      break;
    case wxJSONTYPE_UINT :
      d = m_value.m_valUInt;
      break;
    default :
      break;
  }
  return d;
}


//! Return the stored value as a wxWidget's string.
/*!
 If the type of the stored value is a wxString or a static
 C-string type the function returns a new string instance
 that contains the stored string.
 If the stored value is of a numeric type (int, unsigned int
 or double) the function returns the string representation
 of the numeric value.
 If the stored value is a boolean, the function returns the
 literal string \b true or \b false.
 If the value is a NULL value the \b null literal string is returned.

 If the value is of type wxJSONTYPE_EMPTY, the literal string \b &lt;empty&gt;
 is returned. Note that this is NOT a valid JSON text.
 If the value is an array or map, an empty string is returned.
*/
wxString
wxJSONValue::AsString() const
{
  wxString s;
  switch ( m_type )  {
    case wxJSONTYPE_STRING :
      s = *(m_value.m_valString);
      break;
    case wxJSONTYPE_CSTRING :
      s.assign( m_value.m_valCString);
      break;
    case wxJSONTYPE_INT :
      s.Printf( _T("%d"), m_value.m_valInt );
      break;
    case wxJSONTYPE_UINT :
      s.Printf( _T("%u"), m_value.m_valUInt );
      break;
    case wxJSONTYPE_DOUBLE :
      s.Printf( _T("%f"), m_value.m_valDouble );
      break;
    case wxJSONTYPE_BOOL :
      s.assign( (m_value.m_valBool ? _T("true") : _T("false") ));
      break;
    case wxJSONTYPE_NULL :
      s.assign( _T( "null"));
      break;
    case wxJSONTYPE_EMPTY :
      s.assign( _T( "<empty>"));
      break;
    default :
      break;
  }
  return s;
}

//! Return the stored value as a pointer to a static C string.
/*!
 If the type of the stored value is not a pointer to a C-string the function 
 returns a NULL pointer.
 Note that althrough this object was actually constructed using a static C-string
 pointer as the ctor's argument but the wxJSON_USE_CSTRING macro is not defined,
 the value is stored as a wxString object.
 For this reason, if wxJSON_USE_CSTRING is not defined, the function always
 returns a NULL pointer.

 See also \ref json_internals_cstring
*/
const wxChar*
wxJSONValue::AsCString() const
{
  const wxChar* s = 0;
  if ( m_type == wxJSONTYPE_CSTRING ) {
    s = m_value.m_valCString;
  }
  return s;
}

//! Return the stored value as a unsigned int.
/*!
 If the type of the stored value is not an unsigned int, an int or a bool
 the function  just returns the content of the stored value which may be garbage.
 Note that if the type is a signed int, the function returns the value
 converted to an unsigned integer but a warning is emitted because of
 the loss of the sign.
 In debug builds the function ASSERTs that the stored value is of
 compatible type: an \b int, an \b insigned \b int or a \b boolean
*/
unsigned int
wxJSONValue::AsUInt() const
{
  unsigned int ui;
  wxASSERT( m_type == wxJSONTYPE_INT || m_type == wxJSONTYPE_UINT
				|| m_type == wxJSONTYPE_BOOL );
  switch ( m_type )  {
    case wxJSONTYPE_BOOL :
    case wxJSONTYPE_UINT :
      ui = m_value.m_valUInt;
      break;
    case wxJSONTYPE_INT :
      ::wxLogWarning( _T("wxJSONValue::AsUInt() - value type is signed integer" ));
      ui = m_value.m_valInt;
      break;
    default :
      // for other types just return the content of the m_value union
      // but it will be really garbage
      ui = m_value.m_valUInt;
      break;
  } 
  return ui;
}

// internal use

//! Return the stored value as a map object.
/*!
 This function is for testing and debugging purposes and you shold never use it.
 To retreive values from an array or map JSON object use the \c Item()
 function or the subscript operator.
 If the stored value is not a map type, returns a NULL pointer.
*/
const wxJSONInternalMap*
wxJSONValue::AsMap() const
{
  const wxJSONInternalMap* v = 0;
  if ( m_type == wxJSONTYPE_OBJECT ) {
    v = m_value.m_valMap;
  }
  return v;
}

//! Return the stored value as an array object.
/*!
 This function is for testing and debugging purposes and you shold never use it.
 To retreive values from an array or map JSON object use the \c Item()
 function or the subscript operator.
 If the stored value is not an array type, returns a NULL pointer.
*/
const wxJSONInternalArray*
wxJSONValue::AsArray() const
{
  const wxJSONInternalArray* v = 0;
  if ( m_type == wxJSONTYPE_ARRAY ) {
    v = m_value.m_valArray;
  }
  return v;
}

// retrieve the members and other info


//! Return TRUE if the object contains an element at the specified index.
/*!
 If the stoerd value is not an array or a map, the function returns FALSE.
*/
bool
wxJSONValue::HasMember( unsigned index ) const
{
  bool r = false;
  int size = Size();
  if ( index < (unsigned) size )  {
    r = true;
  }
  return r;
}

//! Return TRUE if the object contains an element at the specified key.
/*!
 If the stored value is not a key/map map, the function returns FALSE.
*/
bool
wxJSONValue::HasMember( const wxString& key ) const
{
  bool r = false;
  if ( m_type == wxJSONTYPE_OBJECT )  {
    wxJSONInternalMap::iterator it = m_value.m_valMap->find( key );
    if ( it != m_value.m_valMap->end() )  {
      r = true;
    }
  }
  return r;
}

//! Return the size of the array or map stored in this value.
/*!
 Note that both the array and the key/value map may have a size of
 ZERO elements.
 If the stored value is not an array nor a key/value hashmap, the 
 function returns -1.
*/
int
wxJSONValue::Size() const
{
  int size = -1;
  if ( m_type == wxJSONTYPE_ARRAY )  {
    size = (int) m_value.m_valArray->GetCount();
  }
  if ( m_type == wxJSONTYPE_OBJECT )  {
    size = (int) m_value.m_valMap->size();
  }
  return size;
}

//! Return an array of keys
/*!
 If the stored value is an key/value map, the function returns an
 array of strings containing the \e key of all elements.
 Note that the returned array may be empty if the map has ZERO
 elements.
 An empty string array is also returned if the stored value is
 not a key/value map.
*/
wxArrayString
wxJSONValue::GetMemberNames() const
{
  wxArrayString arr;
  if ( m_type != wxJSONTYPE_OBJECT )   {
    return arr;
  }
  wxJSONInternalMap::iterator it;
  for ( it = m_value.m_valMap->begin(); it != m_value.m_valMap->end(); it++ )  {
    arr.Add( it->first );
  }
  return arr;
}


// appending items, resizing and deleting items

//! Append the specified value in the array.
/*!
 The function appends the JSON value \c value to the array contained
 in this object.
 If this object does not contain an array type, the actual content is
 deleted and a new array is created.
 Returns a reference to the appended object.
*/
wxJSONValue&
wxJSONValue::Append( const wxJSONValue& value )
{
  if ( m_type != wxJSONTYPE_ARRAY )  {
    DeleteObj();
    m_value.m_valArray = new wxJSONInternalArray();
    m_type = wxJSONTYPE_ARRAY;
  }
  m_value.m_valArray->Add( value );
  wxJSONValue& v = m_value.m_valArray->Last();
  return v;
}

//! \overload Append( const wxJSONValue& )
wxJSONValue&
wxJSONValue::Append( int i )
{
  wxJSONValue v( i );
  wxJSONValue& r = Append( v );
  return r;
}

//! \overload Append( const wxJSONValue& )
wxJSONValue&
wxJSONValue::Append( bool b )
{
  wxJSONValue v( b );
  wxJSONValue& r = Append( v );
  return r;
}

//! \overload Append( const wxJSONValue& )
wxJSONValue&
wxJSONValue::Append( unsigned int ui )
{
  wxJSONValue v( ui );
  wxJSONValue& r = Append( v );
  return r;
}

//! \overload Append( const wxJSONValue& )
wxJSONValue&
wxJSONValue::Append( double d )
{
  wxJSONValue v( d );
  wxJSONValue& r = Append( v );
  return r;
}

//! \overload Append( const wxJSONValue& )
wxJSONValue&
wxJSONValue::Append( const wxChar* str )
{
  wxJSONValue v( str );
  wxJSONValue& r = Append( v );
  return r;
}

//! \overload Append( const wxJSONValue& )
wxJSONValue&
wxJSONValue::Append( const wxString& str )
{
  wxJSONValue v( str );
  wxJSONValue& r = Append( v );
  return r;
}

//! Concatenate a string to this string object.
/*!
 The function concatenates \c str to the string contained
 in this object and returns TRUE if the operation is succefull.
 If the value stored in this value is not a string object
 the function does nothing and returns FALSE.
 Note that in order to be successfull, the value must contain
 a \b wxString object and not a pointer to C-string.
*/
bool
wxJSONValue::Cat( const wxString& str )
{
  bool r = false;
  if ( m_type == wxJSONTYPE_STRING )  { 
    m_value.m_valString->append( str );
    r = true;
  }
  return r;
}

//! \overload Cat( const wxString& )
bool
wxJSONValue::Cat( const wxChar* str )
{
  bool r = false;
  if ( m_type == wxJSONTYPE_STRING )  { 
    m_value.m_valString->append( str );
    r = true;
  }
  return r;
}


//! Remove the item at the specified index or key.
/*!
 The function removes the item at index \c index or at the specified
 key in the array or map.
 If this object does not contain an array (for a index parameter) or a map
 (for a key parameter), the function does nothing and returns FALSE.
 If the element does not exist, FALSE is returned.
*/
bool
wxJSONValue::Remove( int index )
{
  bool r = false;
  if ( m_type == wxJSONTYPE_ARRAY )  {
    m_value.m_valArray->RemoveAt( index );
    r = true;
  }
  return r;
}

//! \overload Remove( int )
bool
wxJSONValue::Remove( const wxString& key )
{
  bool r = false;
  if ( m_type == wxJSONTYPE_OBJECT )  {
    wxJSONInternalMap::size_type count = m_value.m_valMap->erase( key );
    if ( count > 0 )  {
      r = true;
    }
  }
  return r;
}


//! Clear the object value.
/*!
 This function causes the object to be empty.
 If it contained a map or an array, they will be deleted.
 The function sets the type of the object to \b wxJSONTYPE_EMPTY.
*/
void
wxJSONValue::Clear()
{
  DeleteObj();
  m_type = wxJSONTYPE_EMPTY;
}

// retrieve an item

//! Return the item at the specified index.
/*!
 The function returns a reference to the object at the specified
 index.
 If the element does not exist, the array is enlarged to \c index + 1
 elements and a reference to the last element will be returned.
 New elements will contain NULL values.
 If this object does not contain an array, the old value is
 replaced by an array object which will be enlarged to the needed
 dimension.
*/
wxJSONValue&
wxJSONValue::Item( unsigned index )
{
  if ( m_type != wxJSONTYPE_ARRAY )  {
    // deletes the contained value;
    DeleteObj();
    m_type = wxJSONTYPE_ARRAY;
    m_value.m_valArray = new wxJSONInternalArray();
  }
  int size = Size();
  wxASSERT( size >= 0 ); 
  // if the desired element does not yet exist, we create as many
  // elements as needed; the new values will be 'null' values
  if ( index >= (unsigned) size )  {
    wxJSONValue v( wxJSONTYPE_NULL);
    int missing = index - size + 1;
    m_value.m_valArray->Add( v, missing );
  }
  return m_value.m_valArray->Item( index );
}

//! Return the item at the specified key.
/*!
 The function returns a reference to the object in the map
 that has the specified key.
 If \c key does not exist, a new NULL value is created with
 the provided key and a reference to it is returned.
 If this object does not contain a map, the old value is
 replaced by a map object.
*/
wxJSONValue&
wxJSONValue::Item( const wxString& key )
{
  ::wxLogTrace( traceMask, _T("(%s) searched key=\'%s\'"), __PRETTY_FUNCTION__, key.c_str());
  ::wxLogTrace( traceMask, _T("(%s) actual object: %s"), __PRETTY_FUNCTION__, GetInfo().c_str());

  if ( m_type != wxJSONTYPE_OBJECT )  {
    // deletes the contained value;
    DeleteObj();
    m_type = wxJSONTYPE_OBJECT;
    m_value.m_valMap = new wxJSONInternalMap();
    // create a new entry in the map with the specified key
    return m_value.m_valMap->operator[]( key );
  }
  ::wxLogTrace( traceMask, _T("(%s) searching key \'%s' in the actual object"),
				 __PRETTY_FUNCTION__, key.c_str() ); 
  return m_value.m_valMap->operator[]( key );
}


//! Return the item at the specified index.
/*!
 The function returns a copy of the object at the specified
 index.
 If the element does not exist, the function returns an 'empty' value.
*/
wxJSONValue
wxJSONValue::ItemAt( unsigned index ) const
{
  wxJSONValue v;
  if ( m_type == wxJSONTYPE_ARRAY )  {
    int size = Size();
    wxASSERT( size >= 0 ); 
    if ( index < (unsigned) size )  {
      v = m_value.m_valArray->Item( index );
    }
  }
  return v;
}

//! Return the item at the specified key.
/*!
 The function returns a copy of the object in the map
 that has the specified key.
 If \c key does not exist, an 'empty' value is returned.
*/
wxJSONValue
wxJSONValue::ItemAt( const wxString& key ) const
{
  ::wxLogTrace( traceMask, _T("(%s) searched key=\'%s\'"), __PRETTY_FUNCTION__, key.c_str());
  ::wxLogTrace( traceMask, _T("(%s) actual object: %s"), __PRETTY_FUNCTION__, GetInfo().c_str());
  wxJSONValue v;
  if ( m_type == wxJSONTYPE_OBJECT )  {
    wxJSONInternalMap::const_iterator it = m_value.m_valMap->find( key );
    if ( it != m_value.m_valMap->end() )  {
      v = it->second;
    }
  }
  return v;
}


//! Return the item at the specified index.
/*!
 The function returns a reference to the object at the specified
 index.
 If the element does not exist, the array is enlarged to \c index + 1
 elements and a reference to the last element will be returned.
 New elements will contain NULL values.
 If this object does not contain an array, the old value is
 replaced by an array object.
*/
wxJSONValue&
wxJSONValue::operator [] ( unsigned index )
{
  wxJSONValue& v = Item( index );
  return v;
}

//! Return the item at the specified key.
/*!
 The function returns a reference to the object in the map
 that has the specified key.
 If \c key does not exist, a new NULL value is created with
 the provided key and a reference to it is returned.
 If this object does not contain a map, the old value is
 replaced by a map object.
*/
wxJSONValue&
wxJSONValue::operator [] ( const wxString& key )
{
  wxJSONValue& v = Item( key );
  return v;
}

//! Assign the specified value to this object replacing the old value.
/*!
 The function assigns to this object the value specified in the
 left operand of the assignment operator.
 Note that the old value is deleted.
*/
wxJSONValue&
wxJSONValue::operator = ( int i )
{
  DeleteObj();   // deletes actual content of this object
  ::wxLogTrace( traceMask, _T("(%s) actual object: %s"), __PRETTY_FUNCTION__, GetInfo().c_str());
  m_type = wxJSONTYPE_INT;
  m_value.m_valInt = i;
  return *this;
}

//! \overload operator = (int)
wxJSONValue&
wxJSONValue::operator = ( bool b )
{
  ::wxLogTrace( traceMask, _T("(%s) actual object: %s"), __PRETTY_FUNCTION__, GetInfo().c_str());
  DeleteObj();   // deletes actual content of this object
  m_type = wxJSONTYPE_BOOL;
  m_value.m_valBool = b;
  return *this;
}

//! \overload operator = (int)
wxJSONValue&
wxJSONValue::operator = ( unsigned int i )
{
  ::wxLogTrace( traceMask, _T("(%s) actual object: %s"), __PRETTY_FUNCTION__, GetInfo().c_str());
  DeleteObj();   // deletes actual content of this object
  m_type = wxJSONTYPE_UINT;
  m_value.m_valUInt = i;
  return *this;
}

//! \overload operator = (int)
wxJSONValue&
wxJSONValue::operator = ( double d )
{
  ::wxLogTrace( traceMask, _T("(%s) actual object: %s"), __PRETTY_FUNCTION__, GetInfo().c_str());
  DeleteObj();   // deletes actual content of this object
  m_type = wxJSONTYPE_DOUBLE;
  m_value.m_valDouble = d;
  return *this;
}


//! \overload operator = (int)
wxJSONValue&
wxJSONValue::operator = ( const wxChar* str )
{
  ::wxLogTrace( traceMask, _T("(%s) actual object: %s"), __PRETTY_FUNCTION__, GetInfo().c_str());
  DeleteObj();   // deletes actual content of this object
  m_type = wxJSONTYPE_CSTRING;
  m_value.m_valCString = str;

#if !defined( WXJSON_USE_CSTRING )
  m_type = wxJSONTYPE_STRING;
  m_value.m_valString = new wxString( str );
#endif

  return *this;
}

//! \overload operator = (int)
wxJSONValue&
wxJSONValue::operator = ( const wxString& str )
{
  ::wxLogTrace( traceMask, _T("(%s) actual object: %s"), __PRETTY_FUNCTION__, GetInfo().c_str());
  DeleteObj();   // deletes actual content of this object
  m_type = wxJSONTYPE_STRING;
  m_value.m_valString = new wxString( str );
  return *this;
}

//! \overload operator = (int)
wxJSONValue&
wxJSONValue::operator = ( const wxJSONValue& other )
{
  ::wxLogTrace( traceMask, _T("(%s) actual object: %s"), __PRETTY_FUNCTION__, GetInfo().c_str());
  DeleteObj();   // deletes actual content of this object
  DeepCopy( other );
  return *this;
}

//! Return a value or a default value.
/*!
 This function returns the value object for the specified key.
 If the key is not found, a copy of \c defaultValue is returned.
*/
wxJSONValue
wxJSONValue::Get( const wxString& key, const wxJSONValue& defaultValue ) const
{
  // NOTE: this function does many wxJSONValue copies.
  // Normally, the function is used for primitive types, but if used with
  // complex types, it is very slow - need to implement COW

  wxJSONValue v( defaultValue );

  if ( m_type == wxJSONTYPE_OBJECT )  {
    wxJSONInternalMap::iterator it = m_value.m_valMap->find( key );
    if ( it != m_value.m_valMap->end() )  {
      v = it->second;
    }
  }
  return v;
}


// protected functions

//! Find an element
/*!
 The function returns a pointer to the element at index \c index
 or a NULL pointer if \c index does not exist.
 A NULL pointer is also returned if the object does not contain an
 array nor a map.
*/
wxJSONValue*
wxJSONValue::Find( unsigned index ) const
{
  wxJSONValue* vp = 0;

  if ( m_type == wxJSONTYPE_ARRAY )  {
    size_t size = m_value.m_valArray->GetCount();
    if ( index < size )  {
      vp = &(m_value.m_valArray->Item( index ));
    }
  }
  return vp;
}

//! Find an element
/*!
 The function returns a pointer to the element with key \c key
 or a NULL pointer if \c key does not exist.
 A NULL pointer is also returned if the object does not contain a
 map.
*/
wxJSONValue*
wxJSONValue::Find( const wxString& key ) const
{
  wxJSONValue* vp = 0;

  if ( m_type == wxJSONTYPE_OBJECT )  {
    wxJSONInternalMap::iterator it = m_value.m_valMap->find( key );
    if ( it != m_value.m_valMap->end() )  {
      vp = &(it->second);
    }
  }
  return vp;
}

//! Do a deep copy of the other object.
/*!
 This function allocates a new structure object if the other class
 contains them.
 If the other value only holds primitive types, the function just
 copies the values in the data members.

 \todo JSON value objects may be very complex and a deep copy operation
	may be much time-consuming; in a next version of the wxJSON
	library, I will implement \e copy-on-write for this class by
	deriving from wxObject.
*/
void
wxJSONValue::DeepCopy( const wxJSONValue& other )
{
  // we assume that the actual object eventually contained
  // in this object was already deleted.
  // m_progr = other.m_progr;  NO: the progressive is not copied!
  m_type  = other.m_type;
  switch ( m_type )  {

    //  primitive types, just copy the content of the 'm_value' data member
    case wxJSONTYPE_EMPTY :
    case wxJSONTYPE_NULL :
      m_value.m_valInt = 0;   // the value is garbage!
      break;
    case wxJSONTYPE_INT :
      m_value.m_valInt = other.m_value.m_valInt;
      break;
    case wxJSONTYPE_UINT :
      m_value.m_valUInt = other.m_value.m_valUInt;
      break;
    case wxJSONTYPE_DOUBLE :
      m_value.m_valDouble = other.m_value.m_valDouble;
      break;
    case wxJSONTYPE_CSTRING :
      m_value.m_valCString = other.m_value.m_valCString;
      break;
    case wxJSONTYPE_BOOL :
      m_value.m_valBool = other.m_value.m_valBool;
      break;

   // now the copy must be a deep copy of pointed-to objects

    case wxJSONTYPE_STRING :
      m_value.m_valString = new wxString( *(other.m_value.m_valString));
      break;
    case wxJSONTYPE_ARRAY :
      m_value.m_valArray = new wxJSONInternalArray( *(other.m_value.m_valArray) );
      break;
    case wxJSONTYPE_OBJECT :
      m_value.m_valMap = new wxJSONInternalMap( *(other.m_value.m_valMap ));
      break;
    default :
      // should never happen
      // wxFAIL();  why does this code fail to compile?
      wxASSERT( 0 );
      break;
  }

  // finally copy the m_comment array
  m_commentPos = other.m_commentPos;
  m_comments   = other.m_comments;
  m_lineNo     = other.m_lineNo;
}

//! Delete the contained object.
/*!
 This function deletes the allocated objects if this class
 contains them.
 If the JSON value only holds primitive types, nothing happens.
 The function sets the value type as 'empty'
*/
void
wxJSONValue::DeleteObj()
{
  switch ( m_type )  {
    case wxJSONTYPE_ARRAY :
      if ( m_value.m_valArray != 0 )  {
        delete m_value.m_valArray;
        m_value.m_valArray = 0;
      }
      break;
    case wxJSONTYPE_OBJECT :
      break;
      if ( m_value.m_valMap != 0 )  {
        delete m_value.m_valMap;
        m_value.m_valMap = 0;
      }
    case wxJSONTYPE_STRING :
      if ( m_value.m_valString != 0 )  {
        delete m_value.m_valString;
        m_value.m_valString = 0;
      }
      break;
    default :
      break;
  }
  m_type = wxJSONTYPE_EMPTY;
  m_value.m_valInt = 0;
}

//! Return a string description of the type
/*!
 This static function is only usefull for debugging purposes and
 should not be used by users of this class.
 It simply returns a string representation of the JSON value
 type stored in a object.
 For example, if \c type is wxJSONTYPE_INT the function returns the
 string "wxJSONTYPE_INT".
 If \c type is out of range, an empty string is returned (should
 never happen).
*/
wxString
wxJSONValue::TypeToString( wxJSONType type )
{
  static const wxChar* str[] = {
    _T( "wxJSONTYPE_EMPTY" ),   // 0
    _T( "wxJSONTYPE_NULL" ),    // 1
    _T( "wxJSONTYPE_INT" ),     // 2
    _T( "wxJSONTYPE_UINT" ),    // 3
    _T( "wxJSONTYPE_DOUBLE" ),  // 4
    _T( "wxJSONTYPE_STRING" ),  // 5
    _T( "wxJSONTYPE_CSTRING" ), // 6
    _T( "wxJSONTYPE_BOOL" ),    // 7
    _T( "wxJSONTYPE_ARRAY" ),   // 8
    _T( "wxJSONTYPE_OBJECT" ),  // 9
  };

  wxString s;
  int idx = (int) type;
  if ( idx >= 0 && idx < 10 )  {
    s = str[idx];
  }
  return s;
}

//! Returns informations about the object
/*!
 The function is only usefull for debugging purposes and will probably
 be dropped in future versions.
 You should not rely on this function to exist.
*/
wxString
wxJSONValue::GetInfo() const
{
  wxString s;
#if defined( WXJSON_USE_VALUE_CONTER )
  s.Printf( _T("Object: Progr=%d Type=%s Size=%d comments=%d\n"),
			m_progr, TypeToString( m_type ).c_str(), Size(), m_comments.GetCount() );
#else
  s.Printf( _T("Object: Type=%s Size=%d comments=%d\n"),
			TypeToString( m_type ).c_str(), Size(), m_comments.GetCount() );
#endif
  if ( m_type == wxJSONTYPE_OBJECT ) {
    wxArrayString arr = GetMemberNames();
    for ( unsigned int i = 0; i < arr.size(); i++ )  {
      s.append( _T("    Member name: "));
      s.append( arr[i] );
      s.append( _T("\n") );
    }
  }
  return s;
}

//! The comparison function
/*!
 This function returns TRUE if this object looks like \c other.
 Note that this class does not define a comparison operator
 (the classical \b operator== function) because the notion
 of \b equal for JSON values objects is not applicable.
 The comment strings array are not compared: JSON value objects
 are \b the \b same if they contains the same values, regardless the
 comments strings.

 Note that the function does not return the element that cause the
 comparison to return FALSE. There is not a good structure to
 tell this information.
 If you need it for debugging purposes, you have to turn on
 \b jsonval tracing feature by setting the WXTRACE environment
 variable (you need a debug version of the application):

 \code
   export WXTRACE=jsonval
 \endcode

 For further info see \ref json_internals_compare.
*/
bool
wxJSONValue::IsSameAs( const wxJSONValue& other ) const
{
  // this is a recursive function: it calls itself
  // for every 'value' object in an array or map
  bool r = true;

  // some variables used in the switch statement
  int i; int size; wxJSONInternalMap::const_iterator it; int* usedElements;

  // if the type does not match the function compares the values if
  // they are of compatible types such as INT, UINT and DOUBLE
  if ( m_type != other.m_type )  {
    // if the types are not compatible, returns false
    double val, otherVal;
    switch ( m_type )  {
      case wxJSONTYPE_INT :
        val = m_value.m_valInt;
        break;
      case wxJSONTYPE_UINT :
        val = m_value.m_valUInt;
        break;
      case wxJSONTYPE_DOUBLE :
        val = m_value.m_valDouble;
        break;
      default:
        return false;
        break;
    }
    switch ( other.m_type )  {
      case wxJSONTYPE_INT :
        otherVal = other.m_value.m_valInt;
        break;
      case wxJSONTYPE_UINT :
        otherVal = other.m_value.m_valUInt;
        break;
      case wxJSONTYPE_DOUBLE :
        otherVal = other.m_value.m_valDouble;
        break;
      default:
        return false;
        break;
    }
    bool r = false;
    if ( val == otherVal )  {
      r = true;
    }
    return r;
  }

  // for comparing wxJSONTYPE_CSTRING we use two temporary wxString
  // objects: this is to avoid using strcmp() and wcscmp() which
  // may not be available on all platforms
  wxString s1, s2;

  switch ( m_type )  {
    case wxJSONTYPE_EMPTY :
    case wxJSONTYPE_NULL :
      // there is no need to compare the values
      break;
    case wxJSONTYPE_INT :
      if ( m_value.m_valInt != other.m_value.m_valInt )  {
        r = false;
      }
      break;
    case wxJSONTYPE_UINT :
      if ( m_value.m_valUInt != other.m_value.m_valUInt )  {
        r = false;
      }
      break;
    case wxJSONTYPE_DOUBLE :
      if ( m_value.m_valDouble != other.m_value.m_valDouble )  {
        r = false;
      }
      break;
    case wxJSONTYPE_CSTRING :
      s1 = wxString( m_value.m_valCString );
      s2 = wxString( other.m_value.m_valCString );
      // i = strcmp( m_value.m_valCString, other.m_value.m_valCString );
      // i = wcscmp( m_value.m_valCString, other.m_value.m_valCString );
      if ( s1 != s2 )  {
        r = false;
      }
      break;
    case wxJSONTYPE_BOOL :
      if ( m_value.m_valBool != other.m_value.m_valBool )  {
        r = false;
      }
      break;
    case wxJSONTYPE_STRING :
      if ( *(m_value.m_valString) != *(other.m_value.m_valString) )  {
        r = false;
      }
      break;
    case wxJSONTYPE_ARRAY :
      size = Size();
      ::wxLogTrace( compareTraceMask, _T("(%s) Comparing an array object - size=%d"),
			 __PRETTY_FUNCTION__, size );

      if ( size != other.Size() )  {
        ::wxLogTrace( compareTraceMask, _T("(%s) Sizes does not match"),
			 __PRETTY_FUNCTION__ );
        return false;
      }
      // for every element in this object iterates through all
      // items in the 'other' object searching for a matching
      // value. if not found, returns FALSE.
      // note that when an element is found in other it must be marked as
      // 'already used' (see 'test/testjson4.cpp')
      usedElements = new int[size];
      for ( int y = 0; y < size; y++ )  {
        usedElements[y] = 0;
      } 
      for ( int i = 0; i < size; i++ )  {
        ::wxLogTrace( compareTraceMask, _T("(%s) Comparing array element=%d"),
  			 __PRETTY_FUNCTION__, i );
        r = false;
        for ( int y = 0; y < size; y++ )  {
          if ( ItemAt( i ).IsSameAs( other.ItemAt( y )) && usedElements[y] == 0 )  {
            r = true;
            usedElements[y] = 1;
            break;
          } 
        }
        if ( r == false )  {
          delete [] usedElements;
          ::wxLogTrace( compareTraceMask, _T("(%s) Comparison failed"), __PRETTY_FUNCTION__ );
          return false;
        }
      }
      delete [] usedElements;
      break;
    case wxJSONTYPE_OBJECT :
      size = Size();
      ::wxLogTrace( compareTraceMask, _T("(%s) Comparing a map obejct - size=%d"),
			 __PRETTY_FUNCTION__, size );

      if ( size != other.Size() )  {
        ::wxLogTrace( compareTraceMask, _T("(%s) Comparison failed - sizes does not match"),
			 __PRETTY_FUNCTION__ );
        return false;
      }
      // for every key, calls itself on the value found in
      // the other object. if 'key' does no exist, returns FALSE
      // wxJSONInternalMap::const_iterator it;
      for ( it = m_value.m_valMap->begin(); it != m_value.m_valMap->end(); it++ )  {
        wxString key = it->first;
        ::wxLogTrace( compareTraceMask, _T("(%s) Comparing map object - key=%s"),
			 __PRETTY_FUNCTION__, key.c_str() );
        wxJSONValue otherVal = other.ItemAt( key );
        bool isSame = it->second.IsSameAs( otherVal );
        if ( !isSame )  {
          ::wxLogTrace( compareTraceMask, _T("(%s) Comparison failed for the last object"),
			 __PRETTY_FUNCTION__ );
          return false;
        }
      }
      break;
    default :
      // should never happen
      // wxFAIL();  why does this code fail to compile
      wxASSERT( 0 );
      break;
  }
  return r;
}

//! Add a comment to this JSON value object.
/*!
 The function adds a comment string to this JSON value object and returns
 the total number of comment strings belonging to this value.
 Note that the comment string must be a valid C/C++ comment because the
 wxJSONWriter does not modify it.
 In other words, a C++ comment string must start with '//' and must end with
 a new-line character. If not, the function automatically adds the final LF
 character.
 You can also add C-style comments which must be enclosed in the usual
 C-comment characters.
 For C-style comments, the function does not try to append the final comment
 characters but allows trailing whitespaces and new-line chars.
 The \c position parameter is one of:

 \li wxJSONVALUE_COMMENT_BEFORE: the comment will be written before the value
 \li wxJSONVALUE_COMMENT_INLINE: the comment will be written on the same line
 \li wxJSONVALUE_COMMENT_AFTER: the comment will be written after the value
 \li wxJSONVALUE_COMMENT_DEFAULT: the old value of comment's position is not
	changed; if no comments were added to the value object this is the
	same as wxJSONVALUE_COMMENT_BEFORE.

 To know more about comment's storage see \ref json_comment_add

*/
int
wxJSONValue::AddComment( const wxString& str, int position )
{
  ::wxLogTrace( traceMask, _T("(%s) comment=%s"), __PRETTY_FUNCTION__, str.c_str() );
  int r = -1;
  int len = str.length();
  if ( len < 2 )  {
    ::wxLogTrace( traceMask, _T("     error: len < 2") );
    return -1;
  }
  if ( str[0] != '/' )  {
    ::wxLogTrace( traceMask, _T("     error: does not start with\'/\'") );
    return -1;
  }
  if ( str[1] == '/' )  {       // a C++ comment: check that it ends with '\n'
    ::wxLogTrace( traceMask, _T("     C++ comment" ));
    if ( str.GetChar(len - 1) != '\n' )  {
      wxString temp( str );
      temp.append( 1, '\n' );
      m_comments.Add( temp );
      ::wxLogTrace( traceMask, _T("     C++ comment: LF added") );
    }
    else  {
      m_comments.Add( str );
    }
    r = m_comments.size();
  }
  else if ( str[1] == '*' )  {  // a C-style comment: check that it ends with '*/'
    ::wxLogTrace( traceMask, _T("     C-style comment") );
    int lastPos = len - 1;
    wxChar ch = str.GetChar( lastPos );
    // skip leading whitespaces
    while ( ch == ' ' || ch == '\n' || ch == '\t' )  {
      --lastPos;
      ch = str.GetChar( lastPos );
    }
    if ( str.GetChar( lastPos ) == '/' &&  str.GetChar( lastPos - 1 ) == '*' ) {
      m_comments.Add( str );
      r = m_comments.size();
    }  }
  else  {
    ::wxLogTrace( traceMask, _T("     error: is not a valid comment string") );
    r = -1;
  }
  // if the comment was stored, store the position
  if ( r >= 0 && position != wxJSONVALUE_COMMENT_DEFAULT )  {
    m_commentPos = position;
  }
  return r;
}

//! Add one or more comments to this JSON value object.
/*!
 The function adds the strings contained in \c comments to the comment's
 string array of this value object by calling the AddComment( const wxString&,int)
 function for every string in the \c comment array.
 Returns the number of strings correctly added.
*/
int
wxJSONValue::AddComment( const wxArrayString& comments, int position )
{
  int siz = comments.GetCount(); int r = 0;
  for ( int i = 0; i < siz; i++ ) {
    int r2 = AddComment( comments[i], position );
    if ( r2 >= 0 )  {
      ++r;
    }
  }
  return r;
}

//! Return a comment string.
/*!
 The function returns the comment string at index \c idx.
 If \c idx is out of range, an empty string is returned.
 If \c idx is equal to -1, then the function returns a string
 that contains all comment's strings stored in the array.
*/
wxString
wxJSONValue::GetComment( int idx ) const
{
  wxString s;
  int size = m_comments.GetCount();
  if ( idx < 0 )  {
    for ( int i = 0; i < size; i++ )  {
      s.append( m_comments[i] );
    }
  }
  else if ( idx < size )  {
    s = m_comments[idx];
  }
  return s;
}

//! Return the number of comment strings.
int
wxJSONValue::GetCommentCount() const
{
  int d = m_comments.GetCount();
  ::wxLogTrace( traceMask, _T("(%s) comment count=%d"), __PRETTY_FUNCTION__, d );
  return d;
}

//! Return the comment position.
int
wxJSONValue::GetCommentPos() const
{
  return m_commentPos;
}

//! Get the comment string's array.
const wxArrayString&
wxJSONValue::GetCommentArray() const
{
  return m_comments;
}

//! Clear all comment strings
void
wxJSONValue::ClearComments()
{
  m_comments.clear();
}


/*
{
}
*/

