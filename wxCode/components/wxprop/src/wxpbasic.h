/////////////////////////////////////////////////////////////////////////////
// Name:        wxpbasic.h
// Purpose:     Property/Handler/Event header implementation
// Author:      George Yohng <yohng@dosware.8m.com>
// Modified by:
// Created:     13-Apr-2000
// RCS-ID:      $Id: wxpbasic.h,v 1.1.1.1 2003-12-28 21:08:01 wyo Exp $
// Copyright:   (c) 1998,2000 by George Yohng
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/*===========================================================================*\
    Code extracted from DWCS Library pre-release
    Copyright (C)1998,2000 by George Yohng <yohng@dosware.8m.com>
    
    Permission to use, copy, modify, distribute and sell this software
    for any purpose is hereby granted without fee, provided that the
    above copyright notice appear in all copies and that both that
    copyright notice and this permission notice appear in supporting
    documentation. The author makes no representations about the suitability
    of this software for any purpose.  It is provided "as is" without express
    or implied warranty.        
\*===========================================================================*/

#ifndef __WXPBASIC_H_INCLUDED__
#define __WXPBASIC_H_INCLUDED__ 1

#define wxp_assignArg(arg) do{arg=_##arg;}while(0)

#define wxp_declareEq(classname,fromclass)\
    const classname &operator =(fromclass &what)\
        {return assign(what);}



/////////////////////////////////////////////////////////////////////////
// VCL-like properties section
/////////////////////////////////////////////////////////////////////////

#define wxpPropertyBegin(name,typeclass,eventtype)\
    class t_##name : public wxpField<eventtype>\
    {\
    public:\
	typeclass *container;\
        wxp_declareEq(eventtype,const eventtype)\
	wxp_declareEq(eventtype,t_##name)
    
#define wxpPropertyEnd(name,typeclass,eventtype)\
    };\
    t_##name name;

#define wxpPropertyWriteReturn(name,typeclass,eventtype,classmember)\
        virtual const eventtype &write(const eventtype &data) \
        {\
            return container->classmember((eventtype &)data);\
        }
	
#define wxpPropertyWrite(name,typeclass,eventtype,classmember)\
        virtual const eventtype &write(const eventtype &data) \
        {\
            container->classmember((eventtype &)data);\
	    return data;\
        }    

#define wxpPropertyRead(name,typeclass,eventtype,classmember)\
        virtual const eventtype &read() \
        {\
            return container->classmember();\
        }

#define wxpPropertyBindClass(name,theclass)\
    do {\
        name.container = &(theclass);\
    }while(0)
    
#define wxpPropertyBind(name)\
    wxpPropertyBindClass(name, *this)

/////////////////////////////////////////////////////////////////////////
// Handlers section
/////////////////////////////////////////////////////////////////////////

#define wxpHANDLER_READ  1
#define wxpHANDLER_WRITE 2

#define wxpHandler wxpHandlerWrite

#define wxpHandlerWrite(classmember,typeclass,eventtype)\
    wxpPropertyBegin    (handler_##classmember,typeclass,eventtype)\
	wxpPropertyWrite(handler_##classmember,typeclass,eventtype,classmember)\
    wxpPropertyEnd      (handler_##classmember,typeclass,eventtype)

#define wxpHandlerRead(classmember,typeclass,eventtype)\
    wxpPropertyBegin    (handler_##classmember,typeclass,eventtype)\
	wxpPropertyRead (handler_##classmember,typeclass,eventtype,classmember)\
    wxpPropertyEnd      (handler_##classmember,typeclass,eventtype)

#define wxpHandlerReadWrite(classmember,typeclass,eventtype)\
    wxpPropertyBegin    (handler_##classmember,typeclass,eventtype)\
	wxpPropertyRead (handler_##classmember,typeclass,eventtype,handler_read_##classmember)\
	wxpPropertyWrite(handler_##classmember,typeclass,eventtype,handler_write_##classmember)\
    wxpPropertyEnd      (handler_##classmember,typeclass,eventtype)\
    const eventtype &handler_read_##classmember()\
        { return classmember(NULL); }\
    const eventtype &handler_write_##classmember(const eventtype &data)\
	{ classmember(data); return data;}

#define wxpHandlerBindClass(ct_handler, theclass, classevent, targetclass)\
    do{\
	wxpPropertyBindClass(handler_##ct_handler, theclass);\
        (targetclass).event_##classevent.bindto((theclass).handler_##ct_handler);\
        \
    }while(0)
    
#define wxpHandlerBind(ct_handler, classevent, targetclass)\
    wxpHandlerBindClass(ct_handler,*this,classevent,targetclass)
    
#define wxpEvent(eventname, typeclass, eventtype)\
    wxpField<eventtype> event_##eventname;
    
#define wxpEventBindClass(eventname, theclass)
    
#define wxpEventBind(eventname)\
    wxpEventBindClass(eventname, *this)

#define wxpEmitEventClass(eventname, targetclass, eventdata)\
    do { (targetclass).event_##eventname = eventdata; } while(0)

#define wxpEmitEvent(eventname, eventdata)\
    wxpEmitEventClass(eventname, *this, eventdata)

#endif //__WXPBASIC_H_INCLUDED__
