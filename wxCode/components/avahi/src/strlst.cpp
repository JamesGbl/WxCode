/////////////////////////////////////////////////////////////////////////////
// Name:        strlst.cpp
// Purpose:     Avahi to wxWidgets strings and stringlist translations.
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id: strlst.cpp,v 1.4 2005/10/21 16:42:58 frm Exp $
// Copyright:   (c) 2006-2007 Kia Emilien
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
#endif

#include "wx/avahi/strlst.h"
#include <avahi-common/malloc.h>
#include <stdarg.h>

//
// wxAvahiStringListIterator
//
int wxAvahiStringListIterator::GetPair(wxString& key, wxString& value, size_t* size){
    char* _key;
    char* _value;
    int ret = avahi_string_list_get_pair(m_stringlist, &_key, &_value, size);
    key = wxString(_key, wxConvUTF8);
    value = wxString(_value, wxConvUTF8);
    avahi_free(_key);
    avahi_free(_value);
    return ret;
}

wxString wxAvahiStringListIterator::GetText(){
    return wxString((wxChar*)avahi_string_list_get_text(m_stringlist));
}

wxString wxAvahiStringListIterator::operator*(){
    return wxString((wxChar*)avahi_string_list_get_text(m_stringlist));
}


void* wxAvahiStringListIterator::GetContent(){
    return (void*) avahi_string_list_get_text(m_stringlist);
}

size_t wxAvahiStringListIterator::GetSize(){
    return avahi_string_list_get_size(m_stringlist);
}

//
// wxAvahiStringList
//

wxAvahiStringList::wxAvahiStringList():
m_stringlist(NULL)
{
}

wxAvahiStringList::wxAvahiStringList(AvahiStringList* list):
m_stringlist(NULL){
    m_stringlist = avahi_string_list_copy(list);
}


wxAvahiStringList::wxAvahiStringList(const wxAvahiStringList& list):
m_stringlist(NULL){
	m_stringlist = avahi_string_list_copy(list.begin().GetAvahiStringList());
}

wxAvahiStringList::wxAvahiStringList(const wxString* txt, ...):
m_stringlist(NULL)
{
	va_list va;
	m_stringlist = avahi_string_list_new((const char*)txt->mb_str(wxConvUTF8));
	AddMany(va);
}


wxAvahiStringList::wxAvahiStringList(va_list va):
m_stringlist(NULL){
	m_stringlist = avahi_string_list_new_va(va);
}

wxAvahiStringList::wxAvahiStringList(const wxString *array, int length):
m_stringlist(NULL){
	const char** arr = new const char*[length];
	for(int i=0; i< length; i++)
		arr[i] = (char*) array[i].GetData();
	m_stringlist = avahi_string_list_new_from_array(arr, length);
	delete arr;
}

wxAvahiStringList::wxAvahiStringList(const wxArrayString& array):
m_stringlist(NULL){
	int length = array.GetCount();
	const char** arr = new const char*[length];
	for(int i=0; i< length; i++)
		arr[i] = (char*) array[i].GetData();
	m_stringlist = avahi_string_list_new_from_array(arr, length);
	delete arr;
}

wxAvahiStringList::~wxAvahiStringList()
{
	if(m_stringlist.GetAvahiStringList()!=NULL)
		avahi_string_list_free(m_stringlist.GetAvahiStringList());
}

void wxAvahiStringList::Add(const wxString* txt){
	m_stringlist = avahi_string_list_add(m_stringlist.GetAvahiStringList(), (const char*)txt->mb_str(wxConvUTF8));
}

void wxAvahiStringList::Add(const wxString& format, va_list va){
	m_stringlist = avahi_string_list_add_vprintf(m_stringlist.GetAvahiStringList(), (const char*)format.GetData(), va); 
}

void wxAvahiStringList::Add(const wxString& format, ...){
	va_list varg;
	va_start(varg, format);	
	m_stringlist = avahi_string_list_add_vprintf(m_stringlist.GetAvahiStringList(), (const char*)format.GetData(), varg);
	va_end(varg);
}

void wxAvahiStringList::AddArbitrary(const uint8_t *text, size_t size){
	m_stringlist = avahi_string_list_add_arbitrary(m_stringlist.GetAvahiStringList(), text, size);
}

wxAvahiStringListIterator wxAvahiStringList::AddAnonymous(size_t size){
	return m_stringlist = avahi_string_list_add_anonymous(m_stringlist.GetAvahiStringList(), size);
}

void wxAvahiStringList::AddMany(const wxString* txt, ...){
	va_list va;
	Add(*txt);
	m_stringlist = avahi_string_list_add_many_va(m_stringlist.GetAvahiStringList(), va);
}

void wxAvahiStringList::AddMany(va_list va){
	m_stringlist = avahi_string_list_add_many_va(m_stringlist.GetAvahiStringList(), va);
}

void wxAvahiStringList::AddPair(const wxString& name, const wxString& value){
	m_stringlist = avahi_string_list_add_pair(m_stringlist.GetAvahiStringList(), (const char*)name.GetData(), (const char*)value.GetData());
}

void wxAvahiStringList::AddPair(const wxString& name, const uint8_t *text, size_t size){
	m_stringlist = avahi_string_list_add_pair_arbitrary(m_stringlist.GetAvahiStringList(), (const char*)name.GetData(), text, size);
}


wxString wxAvahiStringList::ListToString(){
	char* txt = avahi_string_list_to_string(m_stringlist.GetAvahiStringList());
	wxString str(txt, wxConvUTF8);
	avahi_free(txt);
	return str;
}

wxString wxAvahiStringList::Serialize(){
	char buff[256];
	int size = avahi_string_list_serialize(m_stringlist.GetAvahiStringList(), buff, 255);
	buff[255] = 0;
	return wxString(buff, wxConvUTF8, size); 
}

bool wxAvahiStringList::Equal(const wxAvahiStringList& list)const{
	return avahi_string_list_equal(m_stringlist.GetAvahiStringList(), list.m_stringlist.GetAvahiStringList())==0;
}

void wxAvahiStringList::Reverse(){
	m_stringlist = avahi_string_list_reverse(m_stringlist.GetAvahiStringList());
}

unsigned int wxAvahiStringList::GetCount()const{
	return avahi_string_list_length(m_stringlist.GetAvahiStringList());
}

unsigned long wxAvahiStringList::GetServiceCookie(){
	return avahi_string_list_get_service_cookie(m_stringlist.GetAvahiStringList());
}

wxAvahiStringListIterator wxAvahiStringList::Find(const wxString& key){
    return avahi_string_list_find(m_stringlist.GetAvahiStringList(), (const char*)key.GetData());
}

