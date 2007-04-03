/////////////////////////////////////////////////////////////////////////////
// Name:        wx/avahi/pool.h
// Purpose:     Avahi message dispatchers.
// Author:      Kia Emilien
// Created:     2006/10/20
// RCS-ID:      $Id: pool.h,v 1.3 2005/10/21 16:42:58 frm Exp $
// Copyright:   (c) 2006-2007 Kia Emilien
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef POOL_H_
#define POOL_H_

#include "wx/avahi/avahidef.h"

#include <wx/wx.h>
#include <avahi-common/watch.h>
#include <avahi-common/simple-watch.h>
#include <avahi-common/thread-watch.h>

/**
 * Avahi abstract client pool.
 */
class WXDLLIMPEXP_AVAHI wxAvahiPool
{
public:
    virtual const AvahiPoll* GetPool()const=0;
    virtual ~wxAvahiPool(){}
};

/**
 * Avahi simple client pool.
 */
class WXDLLIMPEXP_AVAHI wxAvahiSimplePool : public wxAvahiPool
{
private:
    AvahiSimplePoll* m_pool;
public:
    wxAvahiSimplePool();
    virtual ~wxAvahiSimplePool();

    virtual const AvahiPoll* GetPool()const;
    
    int Iterate(int sleep_time);
    void Quit();
    void SetFunc(AvahiPollFunc func, void *userdata);
    int Prepare(int timeout);
    int Run();
    int Dispatch();
    int Loop();
    void WakeUp();
};

/**
 * Avahi threaded client pool.
 */
class WXDLLIMPEXP_AVAHI wxAvahiThreadPool : public wxAvahiPool
{
private:
    AvahiThreadedPoll* m_pool;
public:
    wxAvahiThreadPool();
    virtual ~wxAvahiThreadPool();
    
    virtual const AvahiPoll* GetPool()const;

    int Start();
    int Stop();
    void Quit();
    void Lock();
    void Unlock();
};

#endif /*POOL_H_*/
