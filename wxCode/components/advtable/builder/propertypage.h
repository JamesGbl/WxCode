/////////////////////////////////////////////////////////////////////////////
// Name:	propertypage.h
// Purpose: property page declarations
// Author:	Moskvichev Andrey V.
// Created:	06.11.2009
// Copyright:	(c) 2009-2010 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef PROPERTYPAGE_H
#define PROPERTYPAGE_H

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "properties.h"

/**
 * Property control base class.
 */
class PropCtrl
{
public:
	PropCtrl(Property *prop, int id)
	{
		m_prop = prop;
		m_id = id;
	}

	virtual ~PropCtrl()
	{
	}

	Property *GetProperty()
	{
		return m_prop;
	}

	int GetId()
	{
		return m_id;
	}

	virtual wxControl *GetControl() = 0;

	virtual void UpdateProperty(PropertyObject *propObj) = 0;

	virtual void UpdateCtrl(PropertyObject *propObj) = 0;

	virtual wxEventType GetEventType() = 0;
protected:
	int m_id;
	Property *m_prop;
};

class PropertyPage : public wxPanel
{
public:
	PropertyPage(wxWindow *parent);
	virtual ~PropertyPage();

	void OnPropChanged(wxCommandEvent &ev);

	void SetObject(PropertyObject *propObj, bool ownPropObj = false);

private:
	void AddField(wxSizer *sizer, const wxChar *label, PropCtrl *ctrl);

	void UpdatePropertyCtrls();

	void CreatePropertyCtrls(PropertyCollection *propColl);
	void RemovePropertyCtrls();

	PropCtrl *CreatePropertyCtrl(Property *prop, int id);

	wxFlexGridSizer *m_sizer;

	PropertyObject *m_propObj;

	PropCtrl **m_propCtrls;
	int m_propCount;

	bool m_ownPropObj;
};

#endif /* PROPERTYPAGE_H */
