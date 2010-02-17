/////////////////////////////////////////////////////////////////////////////
// Name:	properties.h
// Purpose: properties declarations
// Author:	Moskvichev Andrey V.
// Created:	06.11.2009
// Copyright:	(c) 2009-2010 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


enum PROP_TYPE {
	PROP_INT = 0,
	PROP_BOOL,
	PROP_DOUBLE,
	PROP_STRING,
	PROP_SELECT,
	PROP_COLOR,
	PROP_FILENAME,
};

class Property;
class PropertyCollection;

/**
 * Property base class.
 */
class Property
{
public:
	/**
	 * Constructs new property object.
	 * @param name property name
	 * @param uid property unique id
	 * @param type property type
	 */
	Property(const wxChar *name, int uid, PROP_TYPE type);
	virtual ~Property();

	/**
	 * Checks property string representation.
	 * @param value property string representation
	 * @return true if string representation is correct
	 */
	virtual bool Check(const wxString &value) = 0;

	/**
	 * Formats value to string representation.
	 * @param value in property own representation
	 * @return string representation
	 */
	virtual wxString Format(const void *value) = 0;

	/**
	 * Returns property name.
	 * @return property name
	 */
	const wxString &GetName()
	{
		return m_name;
	}

	/**
	 * Returns property unique id.
	 * @return property unique id
	 */
	int GetUID()
	{
		return m_uid;
	}

	/**
	 * Returns property type.
	 * @return property type
	 */
	PROP_TYPE GetType()
	{
		return m_type;
	}

private:
	wxString m_name;
	int m_uid;
	PROP_TYPE m_type;
};

/**
 * String property.
 */
class PropertyString : public Property
{
public:
	PropertyString(const wxChar *name, int uid)
	: Property(name, uid, PROP_STRING)
	{
	}

	virtual bool Check(const wxString &value);

	virtual wxString Format(const void *value);
};

/**
 * Integer property.
 */
class PropertyInt : public Property
{
public:
	PropertyInt(const wxChar *name, int uid)
	: Property(name, uid, PROP_INT)
	{
	}

	virtual bool Check(const wxString &value);

	virtual wxString Format(const void *value);

	int Decode(const wxString &value);
};

/**
 * Boolean property.
 */
class PropertyBool : public Property
{
public:
	PropertyBool(const wxChar *name, int uid)
	: Property(name, uid, PROP_BOOL)
	{
	}

	virtual bool Check(const wxString &value);

	virtual wxString Format(const void *value);

	bool Decode(const wxString &value);
};

/**
 * Double property.
 */
class PropertyDouble : public Property
{
public:
	PropertyDouble(const wxChar *name, int uid)
	: Property(name, uid, PROP_DOUBLE)
	{
	}

	virtual bool Check(const wxString &value);

	virtual wxString Format(const void *value);

	double Decode(const wxString &value);
};

/**
 * Select property.
 * This type of property maps string key to integer value.
 */
class PropertySelect : public Property
{
public:
	PropertySelect(const wxChar *name, int uid, const wxChar **keys, int *values, int count);

	virtual bool Check(const wxString &value);

	virtual wxString Format(const void *value);

	int GetValue(const wxString &key);

	const wxChar *GetKey(int value);

	const wxChar **GetKeys();

	int GetKeyCount();

private:
	const wxChar **m_keys;
	int *m_values;
	int m_count;
};

class PropertyColor : public Property
{
public:
	PropertyColor(const wxChar *name, int uid)
		: Property(name, uid, PROP_COLOR)
	{
	}

	virtual bool Check(const wxString &value);

	virtual wxString Format(const void *value);
};

class PropertyFileName : public Property
{
public:
	PropertyFileName(const wxChar *name, int uid) : Property(name, uid, PROP_FILENAME)
	{
	}

	virtual bool Check(const wxString &value);

	virtual wxString Format(const void *value);
private:
};

class PropertyObserver {
public:
	virtual ~PropertyObserver();

	virtual void PropertyChanged() = 0;
};

/**
 * Object that supports property get/set, enumeration.
 */
class PropertyObject
{
public:
	PropertyObject()
	{
	}

	virtual ~PropertyObject();

	/**
	 * @param property property
	 * @param value property value
	 */
	void SetProperty(Property *property, const wxString &value)
	{
		if (!property->Check(value)) {
			return ;
		}
		DoSetProperty(property, value);
	}

	/**
	 * Returns property value.
	 * @param property
	 * @return NULL if property is invalid
	 */
	virtual wxString GetProperty(Property *property) = 0;

	/**
	 * Returns property collection supported by this object.
	 * This collection must be static.
	 * @return property collection, can not be NULL
	 */
	virtual PropertyCollection *GetProperties() = 0;

protected:
	/**
	 * Actually sets property.
	 * @param property property
	 * @param value property value
	 */
	virtual void DoSetProperty(Property *property, const wxString &value) = 0;
};

/*
class ObservablePropertyObject : public PropertyObject, public Observable<PropertyObserver> {
protected:
	FIRE_VOID(PropertyChanged);
};
*/

class PropertyCollection {
public:
	PropertyCollection(Property **props, int count);

	virtual ~PropertyCollection();

	Property *Find(const wxString &propName);

	int GetCount();

	Property *GetProperty(int n);

private:
	Property **m_props;
	int m_count;
};

#endif /* PROPERTIES_H */
