/////////////////////////////////////////////////////////////////////////////
// Name:        wxpfield.h
// Purpose:     Fields class implementation/header
// Author:      George Yohng <yohng@dosware.8m.com>
// Modified by:
// Created:     13-Apr-2000
// RCS-ID:      $Id: wxpfield.h,v 1.1.1.1 2003-12-28 21:08:01 wyo Exp $
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

#ifndef __WXPFIELD_H_INCLUDED__
#define __WXPFIELD_H_INCLUDED__ 1
#include <map>
#include <string>
#include <list>
#include <vector>
#include <typeinfo>
#include <stdio.h>

#include "wxpbasic.h"

extern char *wxpField_copyright;
extern char *wxpField_cyclicbind;

template <class basetype>
class wxpField
{
    protected:     
        basetype data;   
        wxpField<basetype> *bound_to;
        std::list<wxpField<basetype> *> bound;

// *** Interface code 1
    public:
        bool writelocal;
    
        wxpField() {  writelocal = false; bound_to = NULL; }
        
        wxpField(const basetype &a) 
            { writelocal = false; bound_to = NULL; _write(a); }
                
        wxpField(wxpField<basetype> &_bind_to, bool _writelocal = false) 
            { writelocal = _writelocal; bind(_bind_to); }
        
        wxpField<basetype> &bind(wxpField<basetype> &which)
        {
            if (!isbound(which))
            {
                unbind(which);
                bound.push_back(which.ptr());
                which.boundupdate();
            }else {fprintf(stderr,wxpField_cyclicbind);}
            return *this;
        }
        
        wxpField<basetype> &unbind(wxpField<basetype> &which)
        {
            wxpField<basetype> *which_bound_to = which.bound_to;
            if (which.isbound())
            {
                which.bound_to = NULL;
                which_bound_to->bound.remove(which.ptr());
                which._write(which_bound_to->_read());
                which.unboundupdate();
            }
            return *this;
        }
        
        wxpField<basetype> &bindto(wxpField<basetype> &where)
            { return *(bound_to = where.bind(*this).ptr()); }
        
        wxpField<basetype> &unbind()
            { return bound_to->unbind(*this); }
              
        wxpField<basetype> &inherit(wxpField<basetype> &from)
        {
            unbind();
            writelocal = from.writelocal;
            assign(from);
            if (from.isbound()) bindto(*from.bound_to);
        }
         
        virtual ~wxpField() 
        {
            if (isbound()) unbind();
            
            while(!bound.empty())
                bound.back()->unbind();
        }
        
// *** Hidden thing
    protected:
        const basetype &_read()
        {
            request();
            if (!isbound()) return read(); else 
                return b_read(bound_to->_read());
        };
        
        const basetype &_write(const basetype &what) 
        {        
            if ((!isbound())||(writelocal)) 
                { const basetype &old=write(what); _update(); return old; }
            else 
                { const basetype &old=bound_to->_write(b_write(what)); _update(); return old;}
        }
        
        void _update()
        {
            update();
            if (!bound.empty())
            {
                std::list<wxpField<basetype> *>::const_iterator t;
                
                for(t = bound.begin(); t != bound.end(); t++)
                {
                    (*t)->update();
                }
            }
        }
        
// *** Interface code 2
    public: 
	virtual const basetype &assign(const basetype &what)
	    { return _write(what);}                
	    
	virtual const basetype &assign(wxpField<basetype> &from)
	    { return _write(from._read());}                
	    
        wxp_declareEq(basetype,const basetype)	
	wxp_declareEq(basetype,wxpField<basetype>)
	
        operator const basetype()
            { return _read(); }
        
        wxpField<basetype> *ptr()
            { return this; }
	    
        const basetype &get()
            { return _read(); }
	    
        const basetype *operator &()
            { data = _read(); return &data; }
              
        bool isbound() const
            { return bound_to!=NULL; }
              
        bool isbound(wxpField<basetype> &where) const
        {           
            if (this == where.ptr()) return true; else
              if (bound_to == where.ptr()) return true; else	    
                if (!isbound()) return false; else            
                  return bound_to->isbound(where);              
        }

// *** Routines to overload
    public:
        virtual const basetype &read()
            { return data; };
        
        virtual const basetype &write(const basetype &what) 
            { return (data = what); }
        
        virtual const basetype &b_read(const basetype &token)
            { return token; };
        
        virtual const basetype &b_write(const basetype &token)
            { return token; };
        
        virtual void update()
            { }
              
        virtual void boundupdate()
            { }
        
        virtual void unboundupdate()
            { }
        
        virtual void request()
	    { }
};

#endif //__WXPFIELD_H_INCLUDED__

