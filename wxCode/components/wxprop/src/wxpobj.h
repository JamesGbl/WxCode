#ifndef __WXPOBJ_H_INCLUDED__
#define __WXPOBJ_H_INCLUDED__ 1

#include "wxpbasic.h"

#define thisclass wxpObject //**********************************************
class thisclass //**********************************************************
{
public:
    long tag;    
    
    virtual void alloc_data()  {} // for dynamic properties - allocate
    virtual void free_data()   {} // for dynamic properties - free
    virtual void bind_data()   {} // bind events and handlers
    virtual void unbind_data() {} // unbind something (for further extensions)
    virtual void init_data()   {} // alloc and initialize properties with default values
    virtual void deinit_data() {} // uninitialize properties (for further extensions)
            
    virtual void init_seq()       // class initialization sequence
        { alloc_data(); bind_data(); init_data(); }
    
    thisclass() { init_seq(); }
    
    thisclass(thisclass &data) { init_seq(); assign(data); }
    
    virtual ~thisclass() 
        { deinit_data(); unbind_data(); free_data(); }
    
    const thisclass &assign(thisclass &data)
    {
	tag = data.tag;
	return *this;
    }
    
    wxp_declareEq(thisclass,thisclass)

};//************************************************************************
#undef thisclass //*********************************************************


class wxpEventData
{
public:
    int tag;
    wxpObject *sender;
};

#endif //__WXPOBJ_H_INCLUDED__
