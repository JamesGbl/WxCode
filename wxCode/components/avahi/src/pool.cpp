/////////////////////////////////////////////////////////////////////////////
// Name:        pool.cpp
// Purpose:     Avahi message dispatchers.
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id: pool.cpp,v 1.4 2005/10/21 16:42:58 frm Exp $
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

#include "wx/avahi/pool.h"

//
// wxAvahiSimplePool
//

wxAvahiSimplePool::wxAvahiSimplePool():
wxAvahiPool()
{
    m_pool = avahi_simple_poll_new();
}

wxAvahiSimplePool::~wxAvahiSimplePool(){
    if(m_pool!=NULL)
        avahi_simple_poll_free(m_pool);
}

const AvahiPoll* wxAvahiSimplePool::GetPool()const{
    return avahi_simple_poll_get(m_pool);
}

int wxAvahiSimplePool::Iterate(int sleep_time){
    return avahi_simple_poll_iterate(m_pool, sleep_time);
}

void wxAvahiSimplePool::Quit(){
    avahi_simple_poll_quit(m_pool);
}

void wxAvahiSimplePool::SetFunc(AvahiPollFunc func, void *userdata){
    avahi_simple_poll_set_func(m_pool, func, userdata);
}

int wxAvahiSimplePool::Prepare(int timeout){
    return avahi_simple_poll_prepare(m_pool, timeout);
}

int wxAvahiSimplePool::Run(){
    return avahi_simple_poll_run(m_pool);
}

int wxAvahiSimplePool::Dispatch(){
    return avahi_simple_poll_dispatch(m_pool);
}

int wxAvahiSimplePool::Loop(){
    return avahi_simple_poll_loop(m_pool);
}

void wxAvahiSimplePool::WakeUp(){
    avahi_simple_poll_wakeup(m_pool);
}

//
// wxAvahiThreadPool
//
wxAvahiThreadPool::wxAvahiThreadPool():
wxAvahiPool(){
    m_pool = avahi_threaded_poll_new();
}

wxAvahiThreadPool::~wxAvahiThreadPool(){
    if(m_pool!=NULL)
        avahi_threaded_poll_free(m_pool);
}

const AvahiPoll* wxAvahiThreadPool::GetPool()const{
    return avahi_threaded_poll_get(m_pool);
}

int wxAvahiThreadPool::Start(){
    return avahi_threaded_poll_start(m_pool);
}

int wxAvahiThreadPool::Stop(){
    return avahi_threaded_poll_stop(m_pool);
}

void wxAvahiThreadPool::Quit(){
    avahi_threaded_poll_quit(m_pool);
}

void wxAvahiThreadPool::Lock(){
    avahi_threaded_poll_lock(m_pool);
}

void wxAvahiThreadPool::Unlock(){
    avahi_threaded_poll_unlock(m_pool);
}
