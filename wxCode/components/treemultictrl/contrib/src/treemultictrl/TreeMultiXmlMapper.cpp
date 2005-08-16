//---------------------------------------------------------------------------
// $RCSfile: TreeMultiXmlMapper.cpp,v $
// $Source: /home/ulrich/cvs-wxcode/wxCode/components/treemultictrl/contrib/src/treemultictrl/TreeMultiXmlMapper.cpp,v $
// $Revision: 1.2 $
// $Date: 2005-08-16 19:41:19 $
//---------------------------------------------------------------------------
// Author:      Jorgen Bodde
// Copyright:   (c) Jorgen Bodde
// License:     wxWidgets License
//---------------------------------------------------------------------------

#ifdef wxUSE_TMC_XMLMAPPER

#ifdef __GNUG__
    #pragma implementation "TreeMultiXmlMapper.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "TreeMultiXmlMapper.h"

//----------------------------------------------------------------------------
// wxTreeMultiXmlMapper
//----------------------------------------------------------------------------

wxTreeMultiXmlMapper::wxTreeMultiXmlMapper(wxTreeMultiCtrl *ctrl)
	: m_ctrl(ctrl)
    , m_tiDoc(0)
    , m_currentId(0)
{
	// TODO: Implement this
}

wxTreeMultiXmlMapper::~wxTreeMultiXmlMapper()
{
	// remove the created XML document
    if(m_tiDoc)
        delete m_tiDoc;
}

void wxTreeMultiXmlMapper::InitXML()
{
    // create a new TiXmlDocument object to load contents
    // the old one is destroyed for clarity
    if(m_tiDoc)
        delete m_tiDoc;

    m_tiDoc = new TiXmlDocument();
}

bool wxTreeMultiXmlMapper::InitWizard(const wxFileName &xmlfile, const wxString &start_tag)
{
    wxCHECK(m_ctrl, false);

    // create a new XML document
    InitXML();
    wxCHECK(m_tiDoc, false);

    // load the contents from disk
    if(m_tiDoc->LoadFile(xmlfile.GetFullPath().c_str()))
        return DoInitWizard(start_tag);

	return false;
}

bool wxTreeMultiXmlMapper::InitWizard(const wxString &xmlstring, const wxString &start_tag)
{
    wxCHECK(m_ctrl, false);

    InitXML();
    wxCHECK(m_tiDoc, false);

    // parse given XML string
    m_tiDoc->Parse(xmlstring.c_str());
    if(!m_tiDoc->Error())
        return DoInitWizard(start_tag);

    return false;
}

bool wxTreeMultiXmlMapper::DoInitWizard(const wxString &start_tag)
{
    bool allok = false;
    
    // clean up the control first
    m_ctrl->DeleteAllItems();

    // reset the ID counter
    m_currentId = XMLMAP_BASE_ID;
    
    // obtain handle to document
    TiXmlHandle h(m_tiDoc);
    
    // find our first wizard declaration
    TiXmlHandle wizard = h.FirstChildElement(start_tag.c_str());

    // we expect it to be an element, and we should go from there with
    // parsing categories and other stuff..

    if(wizard.Element())
    {
        // we skip all and look for the first category element
        TiXmlElement *cat = wizard.FirstChildElement(XMLMAP_TAG_CATEGORY).Element();

        // go through the loop of creating all categories
        allok = (cat != 0);
        while(cat && allok)
        {
            // ok create a category root name, but only when we 
            // have a name to give it
            if(cat->Attribute(XMLMAP_ATTR_CAPTION))
            {
                wxTreeMultiItem item = AddCategory(cat, wxTreeMultiItem(0));
                allok = DoCreateControls(cat, item, 0, 0);
            }
            
            // find a next one
            if(allok)
                cat = cat->NextSiblingElement(XMLMAP_TAG_CATEGORY);
        }
    }

    return allok;
}

bool wxTreeMultiXmlMapper::DoCreateControls(TiXmlElement *cat, wxTreeMultiItem &pitem, wxWindow *pwnd, wxSizer *szr)
{
    bool allOk = true;
    
    wxCHECK(cat, false);
    wxString value;
    wxWindow *wnd = 0;

    // go through all siblings of this 
    TiXmlNode *node = cat->FirstChild();
    while(node && allOk)
    {
        // cast to element. When it is an element, work this
        TiXmlElement *el = node->ToElement();
        if(el)
        {
            value = el->Value();
            
            // when we are dealing with a <row> element, we introduce a wxBoxSizer + panel
            // to add more on one row
            if(value.IsSameAs(XMLMAP_TAG_ROW, false))
            {
                // we are in a row. All elements inside need to be parent
                // of this element, and we use a sizer to layout
                wxPanel *pnl = new wxPanel((pwnd != 0 ? pwnd : m_ctrl), -1);
                wxBoxSizer *bsz = new wxBoxSizer(wxHORIZONTAL);
                allOk = DoCreateControls(el, pitem, pnl, bsz);
                if(allOk)
                {
                    // ok now we have all added elements, set the sizer and 
                    // add the item when we have no parent control nomore
                    pnl->SetSizerAndFit(bsz);
                    if(pwnd == 0)
                    {
                        // add the window 
                        m_ctrl->AppendWindow(pitem, pnl);
                    }
                }
                else
                    delete bsz;
            }
            else if(value.IsSameAs(XMLMAP_TAG_CATEGORY, false)) // is it an <category> element?
            {
                if(!pwnd)
                {
                    // only allow when we have no parent window
                    wxTreeMultiItem item = AddCategory(el, pitem);
                    allOk = DoCreateControls(el, item, 0, 0);
                }
                else
                {
                    // we do not allow a category inside another window!
                    // so we report this as an error
                    wxLogError(_("Cannot have a <" XMLMAP_TAG_CATEGORY "> inside <" XMLMAP_TAG_ROW "> tags!"));
                    allOk = false;
                }
            }
            else if(value.IsSameAs(XMLMAP_TAG_CHECKBOX, false)) 
                // is it a <checkbox> element?
                wnd = AddCheckBox(el, (pwnd != 0 ? pwnd : m_ctrl));
            else if(value.IsSameAs(XMLMAP_TAG_BUTTON, false)) 
                // is it a <button> element?
                wnd = AddButton(el, (pwnd != 0 ? pwnd : m_ctrl));
            else
                // we encountered a strange element, just ignore
                allOk = true;

            // if we have a window to add, do this.
            if(wnd)
            {
                // add to sizer when there is one, else
                // do not add but trust the parent will be added
                if(szr)
                    szr->Add(wnd, 0, wxALIGN_CENTER_VERTICAL);
                else
                    m_ctrl->AppendWindow(pitem, wnd);
                
                allOk = (wnd != 0);
                wnd = 0;
            }
        }

        // get next sibling on this level
        if(allOk)
            node = node->NextSibling();
    }

    return allOk;
}

wxTreeMultiItem wxTreeMultiXmlMapper::AddCategory(TiXmlElement *cat, wxTreeMultiItem &parent)
{
    wxTreeMultiItem item(0);
    
    // get caption
    wxString name = GetCaptionAttribute(cat);

    // add the caption to the control   
    if(!parent.IsOk())
        item = m_ctrl->AddRoot(name);
    else
        item = m_ctrl->AppendNode(parent, name);

    return item;
}

wxWindow *wxTreeMultiXmlMapper::AddCheckBox(TiXmlElement *cb, wxWindow *parent)
{
    wxWindow *wnd = 0;
    wxCHECK(parent, wnd);
            
    // get caption attrib
    wxString str = GetCaptionAttribute(cb);
    if(!str.IsEmpty())
    {
        // add the caption to the control   
        wxCheckBox *chkb = new wxCheckBox(parent, m_currentId, str);

        // set default checked state
        int checkstate = GetDefaultBoolAttribute(cb);
        if(checkstate != -1)
            chkb->SetValue(checkstate == 1);

        // increment ID
        m_currentId ++;
        wnd = chkb;
    }
    
    return wnd;
}

wxWindow *wxTreeMultiXmlMapper::AddButton(TiXmlElement *cb, wxWindow *parent)
{
    wxWindow *wnd = 0;
    wxCHECK(parent, wnd);
            
    // get caption attrib
    wxString str = GetCaptionAttribute(cb);
    if(!str.IsEmpty())
    {
        // add the caption to the control   
        wxButton *but = new wxButton(parent, m_currentId, str);

        // increment ID
        m_currentId ++;
        wnd = but;
    }
    
    return wnd;
}

int wxTreeMultiXmlMapper::GetDefaultBoolAttribute(TiXmlElement *el)
{
    wxString def(el->Attribute(XMLMAP_ATTR_DEFAULT));
    
    if(!def.IsEmpty())
    {
        wxString value = def.Strip(wxString::both);
        if(!value.CmpNoCase(_("1")))
            return 1;
        else if(!value.CmpNoCase(_("yes")))
            return 1;
        else if(!value.CmpNoCase(_("on")))
            return 1;
        else if(!value.CmpNoCase(_("true")))
            return 1;
        else if(!value.CmpNoCase(_("0")))
            return 0;
        else if(!value.CmpNoCase(_("no")))
            return 0;
        else if(!value.CmpNoCase(_("off")))
            return 0;
        else if(!value.CmpNoCase(_("false")))
            return 0;
        else
        {
            wxString str;
            str << _("<") << wxString(el->Value()) << _(">: invalid default value!");
            wxLogWarning(str);
        }
    }

    return -1;
}

wxString wxTreeMultiXmlMapper::GetCaptionAttribute(TiXmlElement *el, bool allowEmpty)
{
    wxCHECK(el, wxEmptyString);

    // get the attribute name
    wxString name(el->Attribute(XMLMAP_ATTR_CAPTION));
    
    // when empty and not allowed, log this
    if(name.IsEmpty() && !allowEmpty)
    {
        wxString errorstr;
        errorstr << _("<") << wxString(el->Value()) << _(">: has empty or no '" XMLMAP_ATTR_CAPTION "' attribute for title!");
        wxLogError( errorstr);
    }

    return name;
}

bool wxTreeMultiXmlMapper::Validate(wxArrayString *items)
{
	// clear log if we have one
	if(items)
		items->Clear();

	// TODO: Validate treemulti
	return false;
}
    
bool wxTreeMultiXmlMapper::Store()
{
	// we cannot continue when validate is false
	if(!Validate())
		return false;

	return true;
}

#endif
