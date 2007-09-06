/////////////////////////////////////////////////////////////////////////////
// Name:        wx/avahi/strlst.h
// Purpose:     Avahi to wxWidgets strings and stringlist translations.
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id: strlst.h,v 1.3 2005/10/21 16:42:58 frm Exp $
// Copyright:   (c) 2006-2007 Kia Emilien
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef WXAVAHISTRINGLIST_H_
#define WXAVAHISTRINGLIST_H_

#include "wx/avahi/avahidef.h"

#include <wx/wx.h>
#include <avahi-common/strlst.h>


class WXDLLIMPEXP_AVAHI wxAvahiStringListIterator
{
private:
    AvahiStringList* m_stringlist;
public:
    wxAvahiStringListIterator():m_stringlist(NULL){}
    wxAvahiStringListIterator(AvahiStringList* strlst):m_stringlist(strlst){}
    ~wxAvahiStringListIterator(){}
    wxAvahiStringListIterator& operator=(const wxAvahiStringListIterator& strlst){m_stringlist = strlst.m_stringlist; return *this;}
    wxAvahiStringListIterator& operator=(AvahiStringList* strlst){m_stringlist = strlst; return *this;}
    
    AvahiStringList* GetAvahiStringList()const{return m_stringlist;}
    
    wxString operator*();
    wxAvahiStringListIterator operator++(int){
        wxAvahiStringListIterator strlst(m_stringlist);
        m_stringlist=avahi_string_list_get_next(m_stringlist);
        return strlst;
    }
    wxAvahiStringListIterator& operator++(){m_stringlist=avahi_string_list_get_next(m_stringlist);return *this;}
    wxAvahiStringListIterator GetNext(){return wxAvahiStringListIterator(avahi_string_list_get_next(m_stringlist));} 
    
    int GetPair(wxString& key, wxString& value, size_t* size);
    wxString GetText();
    void* GetContent();
    size_t GetSize();
};

/**
 * Avahi string list.
 * Primarily used for TXTRecords
 */
class WXDLLIMPEXP_AVAHI wxAvahiStringList
{
private:
	wxAvahiStringListIterator m_stringlist;
public:
	wxAvahiStringList();
    wxAvahiStringList(AvahiStringList* list);
	wxAvahiStringList(const wxAvahiStringList& list);
	wxAvahiStringList(const wxString* txt, ...);
	wxAvahiStringList(va_list va);
	wxAvahiStringList(const wxString *array, int length);
	wxAvahiStringList(const wxArrayString& array);
	virtual ~wxAvahiStringList();
	
	AvahiStringList* GetStringList(){return m_stringlist.GetAvahiStringList();}
    wxAvahiStringListIterator Find(const wxString& key);
    wxAvahiStringListIterator begin()const{return m_stringlist;}
    wxAvahiStringListIterator end(){return wxAvahiStringListIterator(NULL);}
	
	void Add(const wxString* txt);
	void Add(const wxString& format, va_list va);
	void Add(const wxString& format, ...);
	void AddArbitrary(const uint8_t *text, size_t size);
	wxAvahiStringListIterator AddAnonymous(size_t size);
	
	void AddMany(const wxString* txt, ...);
	void AddMany(va_list va);

	void AddPair(const wxString& name, const wxString& value);
	void AddPair(const wxString& name, const uint8_t *text, size_t size);
	
	wxString ListToString();
	wxString Serialize();
	
	bool Equal(const wxAvahiStringList& list)const;
	bool operator==(const wxAvahiStringList& list)const{return Equal(list);}
	bool operator!=(const wxAvahiStringList& list)const{return !Equal(list);}
	
	void Reverse();
	unsigned int GetCount()const;
	
	unsigned long GetServiceCookie();
};

#endif /*WXAVAHISTRINGLIST_H_*/
