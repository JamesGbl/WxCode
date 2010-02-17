/////////////////////////////////////////////////////////////////////////////
// Name:	propertypage.cpp
// Purpose: property page implementation
// Author:	Moskvichev Andrey V.
// Created:	06.11.2009
// Copyright:	(c) 2009-2010 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "propertypage.h"

#define PROPCTRL_ID_BASE 100

class PropCtrl_SELECT : public PropCtrl, public wxChoice
{
public:
	PropCtrl_SELECT(Property *prop, wxWindow *parent, int id)
	: PropCtrl(prop, id),
	wxChoice(parent, id, wxDefaultPosition, wxDefaultSize)
	{
		PropertySelect *propSel = (PropertySelect *) prop;

		int keyCount = propSel->GetKeyCount();
		const wxChar **keys = propSel->GetKeys();

		for (int n = 0; n < keyCount; n++)
			Append(wxString(keys[n]));
	}

	virtual wxEventType GetEventType()
	{
		return wxEVT_COMMAND_CHOICE_SELECTED;
	}

	virtual wxControl *GetControl()
	{
		return this;
	}

	virtual void UpdateProperty(PropertyObject *propObj)
	{
		int sel = GetSelection();
		if (sel != -1) {
			propObj->SetProperty(m_prop, GetStringSelection().c_str());
		}
	}

	virtual void UpdateCtrl(PropertyObject *propObj)
	{
		wxString value = propObj->GetProperty(m_prop);

		SetStringSelection(value);
	}
};

class PropCtrl_INT : public PropCtrl, public wxTextCtrl
{
public:
	PropCtrl_INT(Property *prop, wxWindow *parent, int id)
	: PropCtrl(prop, id),
	wxTextCtrl(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize,
			wxTE_RIGHT | wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_NUMERIC, &m_value))
			{
		m_value = wxT("0");
			}

	virtual ~PropCtrl_INT()
	{
	}

	virtual wxEventType GetEventType()
	{
		return wxEVT_COMMAND_TEXT_ENTER;
	}

	virtual wxControl *GetControl()
	{
		return this;
	}

	virtual void UpdateProperty(PropertyObject *propObj)
	{
		propObj->SetProperty(m_prop, GetValue());
	}

	virtual void UpdateCtrl(PropertyObject *propObj)
	{
		m_value = propObj->GetProperty(m_prop);
		SetValue(m_value);
	}

private:
	wxString m_value;
};

class PropCtrl_BOOL : public PropCtrl, public wxCheckBox
{
public:
	PropCtrl_BOOL(Property *prop, wxWindow *parent, int id)
	: PropCtrl(prop, id),
	wxCheckBox(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize)
	{
	}

	virtual ~PropCtrl_BOOL()
	{
	}

	virtual wxEventType GetEventType()
	{
		return wxEVT_COMMAND_CHECKBOX_CLICKED;
	}

	virtual wxControl *GetControl()
	{
		return this;
	}

	virtual void UpdateProperty(PropertyObject *propObj)
	{
		propObj->SetProperty(m_prop, m_prop->Format((const void *) GetValue()));
	}

	virtual void UpdateCtrl(PropertyObject *propObj)
	{
		wxString value = propObj->GetProperty(m_prop);
		SetValue(((PropertyBool *) m_prop)->Decode(value));
	}
};

/*
class PropCtrl_DOUBLE : public PropCtrl
{
public:
	PropCtrl_DOUBLE(Property *_prop) : PropCtrl(_prop, id)
	{
	}

	virtual wxControl *CreateControl(wxWindow *parent, int id)
	{
		ctrl = new wxTextCtrl(parent, id, wxT(""), wxDefaultPosition, wxDefaultSize,
				      wxTE_RIGHT | wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_NUMERIC));
		return (wxControl *) ctrl;
	}

	virtual wxEventType GetEventType()
	{
		return wxEVT_COMMAND_TEXT_ENTER;
	}

	virtual void SetProperty(PropertyObject *propObj, Property *prop, wxControl *ctrl)
	{
	}

	virtual void UpdateProperty(PropertyObject *propObj)
	{
	}

	virtual void UpdateCtrl(PropertyObject *propObj)
	{
	}

private:
	wxTextCtrl *ctrl;
};
 */

class PropCtrl_STRING : public PropCtrl, public wxTextCtrl
{
public:
	PropCtrl_STRING(Property *prop, wxWindow *parent, int id)
	: PropCtrl(prop, id),
	wxTextCtrl(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize,
			wxTE_RIGHT | wxTE_PROCESS_ENTER)
			{
			}

	virtual wxEventType GetEventType()
	{
		return wxEVT_COMMAND_TEXT_ENTER;
	}

	virtual wxControl *GetControl()
	{
		return this;
	}

	virtual void UpdateProperty(PropertyObject *propObj)
	{
		propObj->SetProperty(m_prop, GetValue());
	}

	virtual void UpdateCtrl(PropertyObject *propObj)
	{
		SetValue(propObj->GetProperty(m_prop));
	}
};

//
// PropertyPage
//

PropertyPage::PropertyPage(wxWindow *parent)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
	m_propCount = 0;
	m_propCtrls = NULL;
	m_propObj = NULL;
	m_ownPropObj = false;

	m_sizer = new wxFlexGridSizer(0, 2, 0, 0);

	SetSizer(m_sizer);
	m_sizer->SetSizeHints(this);
}

PropertyPage::~PropertyPage()
{
	RemovePropertyCtrls();

	if (m_ownPropObj) {
		wxDELETE(m_propObj);
	}
}

void PropertyPage::UpdatePropertyCtrls()
{
	RemovePropertyCtrls();

	if (m_propObj != NULL) {
		CreatePropertyCtrls(m_propObj->GetProperties());
	}
}

void PropertyPage::CreatePropertyCtrls(PropertyCollection *propColl)
{
	m_propCount = propColl->GetCount();

	int id = PROPCTRL_ID_BASE;

	m_propCtrls = new PropCtrl *[m_propCount];
	for (int n = 0; n < m_propCount; n++) {
		Property *prop = propColl->GetProperty(n);

		PropCtrl *propCtrl = CreatePropertyCtrl(prop, id);
		if (propCtrl == NULL) {
			wxLogError(wxT("PropertyPage::PropertyPage: unknown propType=%i"),
					prop->GetType());
			continue;
		}

		AddField(m_sizer, prop->GetName(), propCtrl);
		m_propCtrls[n] = propCtrl;

		Connect(id, propCtrl->GetEventType(),
				(wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction) &PropertyPage::OnPropChanged,
				NULL);

		id++;
	}

	m_sizer->Layout();
}

void PropertyPage::RemovePropertyCtrls()
{
	for (int n = 0; n < m_propCount; n++) {
		if (m_propCtrls[n] != NULL) {
			Disconnect(m_propCtrls[n]->GetId(), m_propCtrls[n]->GetEventType());

			//m_propCtrls[n]->GetControl()->Destroy();
		}
	}

	m_sizer->Clear(true);

	wxDELETE(m_propCtrls);
	m_propCount = 0;
}

PropCtrl *PropertyPage::CreatePropertyCtrl(Property *prop, int id)
{
#define CASE_PROPCTL(ptype)					\
	case PROP_##ptype:					\
	return new PropCtrl_##ptype(prop, this, id);	\
	break;

	switch (prop->GetType()) {
	CASE_PROPCTL(INT)
	CASE_PROPCTL(BOOL)
	// 		CASE_PROPCTL(DOUBLE)
	CASE_PROPCTL(STRING)
	CASE_PROPCTL(SELECT)
	default:
		return NULL;
	}

#undef CASE_PROPCTL
}

void PropertyPage::SetObject(PropertyObject *propObj, bool ownPropObj)
{
	if (m_ownPropObj) {
		wxDELETE(m_propObj);
	}

	m_propObj = propObj;
	m_ownPropObj = ownPropObj;

	UpdatePropertyCtrls();

	for (int n = 0; n < m_propCount; n++) {
		if (m_propCtrls[n] != NULL) {
			m_propCtrls[n]->UpdateCtrl(m_propObj);
		}
	}
}

void PropertyPage::AddField(wxSizer *sizer, const wxChar *label, PropCtrl *ctrl)
{
	sizer->Add(new wxStaticText(this, wxID_ANY, wxString(label), wxDefaultPosition),
			0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 5);
	sizer->Add(ctrl->GetControl(),
			0, wxALL | wxEXPAND, 5);
}

void PropertyPage::OnPropChanged(wxCommandEvent &ev)
{
	for (int n = 0; n < m_propCount; n++) {
		PropCtrl *propCtrl = m_propCtrls[n];

		if (propCtrl != NULL) {
			if (propCtrl->GetControl() == ev.GetEventObject()) {
				propCtrl->UpdateProperty(m_propObj);
				propCtrl->UpdateCtrl(m_propObj);
				break;
			}
		}
	}
}

