/////////////////////////////////////////////////////////////////////////////
// Name:        wxpptr.h
// Purpose:     Automatic pointer template class data
// Author:      George Yohng
// Modified by:
// Created:     13-Apr-2000
// RCS-ID:      $Id: wxpptr.h,v 1.1.1.1 2003-12-28 21:08:01 wyo Exp $
// Copyright:   (c) 2000 DosWare, corp. crew
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/*===========================================================================*\
    Copyright (C)2000 by George Yohng / DosWare, corp. crew.    
    DosWare is Digital Operation Software, Corp. Crew
\*===========================================================================*/

#ifndef __WXPPTR_H_INCLUDED__
#define __WXPPTR_H_INCLUDED__ 1
#include <map>

#ifndef NULL
#define NULL 0
#endif

extern std::map<void *,int> __wxpPtr__data__;

template <class baseclass>
class wxpPtr
{
    protected:
        baseclass *object;
    
    public:
        wxpPtr() 
            { object = NULL; }
        
        ~wxpPtr()
        {
            assign(NULL);
        }
        
        baseclass *getnew()
        {
            return new baseclass();
        }
        
        const wxpPtr &assign(baseclass *_object) 
        { 
	    baseclass *oldobject=release();
	    
	    if (oldobject)
	    {
	        if (oldobject!=_object) delete oldobject;
	    }else  
	    {
                object = _object;
	    }
            
            if (object)
                __wxpPtr__data__[(void *)object]++;

            return *this; 
        }
                
        baseclass &operator*() 
        { 
            if (!object) assign(getnew());
            return *get(); 
        }
        
        baseclass *operator->()  
        { 
            if (!object) assign(getnew());
            return get(); 
        }
        
        baseclass *get()
        {
            return object; 
        }
        
        baseclass *release()           
        { 
            baseclass *oldobject = object;
	    if (object) 
            {
                if ((__wxpPtr__data__[(void *)object]--)<=0)
                {
                    __wxpPtr__data__.erase((void *)object);
                }
            }            
            object = NULL;	    
            return oldobject; 
        }   
        
        const wxpPtr &operator =(baseclass *_object)
        {
            return assign(_object);
        }
        
        operator baseclass*()
        {
            return get();
        }                
};

#endif //__WXPPTR_H_INCLUDED__
