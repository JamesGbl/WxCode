/////////////////////////////////////////////////////////////////////////////
// Name:	properties.cpp
// Purpose: properties implementation
// Author:	Moskvichev Andrey V.
// Created:	06.11.2009
// Copyright:	(c) 2009-2010 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "properties.h"

//
// Property
//
Property::Property(const wxChar *name, int uid, PROP_TYPE type)
{
	m_name = name;
	m_uid = uid;
	m_type = type;
}

Property::~Property()
{
}

PropertyObserver::~PropertyObserver()
{
}

PropertyObject::~PropertyObject()
{
}

//
// PropertyString
//

bool PropertyString::Check(const wxString &value)
{
	return true;
}

wxString PropertyString::Format(const void *value)
{
	return wxString((const wxString &) value);
}

//
// PropertyInt
//

bool PropertyInt::Check(const wxString &value)
{
	long l;
	return value.ToLong(&l);
}

wxString PropertyInt::Format(const void *value)
{
	int ival = (int) (long) value;
	return wxString::Format(wxT("%i"), ival);
}

int PropertyInt::Decode(const wxString &value)
{
	long l;
	wxCHECK(value.ToLong(&l), 0);
	return (int) l;
}

//
// PropertyBool
//
const wxString strTrue = wxT("true");
const wxString strFalse = wxT("false");

bool PropertyBool::Check(const wxString &value)
{
	return (value.Cmp(strTrue) == 0 || value.Cmp(strFalse) == 0);
}

wxString PropertyBool::Format(const void *value)
{
	wxString out;
	if ((bool) value) {
		out = strTrue;
	}
	else {
		out = strFalse;
	}
	return out;
}

bool PropertyBool::Decode(const wxString &value)
{
	return value.Cmp(strTrue) == 0;
}

//
// PropertyDouble
//

bool PropertyDouble::Check(const wxString &value)
{
	double d;
	return value.ToDouble(&d);
}

wxString PropertyDouble::Format(const void *value)
{
	return wxString::Format(wxT("%f"), (double) (long) value);
}

double PropertyDouble::Decode(const wxString &value)
{
	double d;
	wxCHECK(value.ToDouble(&d), 0);
	return d;
}

//
// PropertySelect
//
PropertySelect::PropertySelect(const wxChar *name, int uid, const wxChar **keys, int *values, int count)
: Property(name, uid, PROP_SELECT)
{
	m_keys = keys;
	m_values = values;
	m_count = count;
}

bool PropertySelect::Check(const wxString &value)
{
	return GetValue(value) != -1;
}

wxString PropertySelect::Format(const void *value)
{
	int ival = (int) (long) value;

	const wxChar *key = GetKey(ival);
	wxCHECK(key != NULL, wxEmptyString);
	return wxString(key);
}

int PropertySelect::GetValue(const wxString &key)
{
	for (int n = 0; n < m_count; n++) {
		if (key.Cmp(m_keys[n]) == 0)
			return m_values[n];
	}
	return -1; // XXx -1 may be used by application
}

const wxChar *PropertySelect::GetKey(int value)
{
	for (int n = 0; n < m_count; n++) {
		if (m_values[n] == value)
			return m_keys[n];
	}
	return NULL;
}

const wxChar **PropertySelect::GetKeys()
{
	return m_keys;
}

int PropertySelect::GetKeyCount()
{
	return m_count;
}

//
// PropertyColor
//

bool PropertyColor::Check(const wxString &value)
{
	return TRUE;
}

wxString PropertyColor::Format(const void *value)
{
	// XXX not implemented
	return wxEmptyString;
}

//
// PropertyFileName
//

bool PropertyFileName::Check(const wxString &value)
{
	return TRUE;
}

wxString PropertyFileName::Format(const void *value)
{
	return wxString((const wxString &) value);
}

//
// PropertyCollection
//

PropertyCollection::PropertyCollection(Property **props, int count)
{
	m_props = props;
	m_count = count;
}

PropertyCollection::~PropertyCollection()
{
}

Property *PropertyCollection::Find(const wxString &propName)
{
	for (int n = 0; n < m_count; n++) {
		if (propName.Cmp(m_props[n]->GetName()) == 0)
			return m_props[n];
	}
	return NULL;
}

int PropertyCollection::GetCount()
{
	return m_count;
}

Property *PropertyCollection::GetProperty(int n)
{
	if (n < m_count) {
		return m_props[n];
	}
	else {
		wxLogError(wxT("PropertyCollection::GetProperty: n=%i count=%i\n"), n, m_count);
		return NULL;
	}
}
