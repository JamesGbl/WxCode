#include <stdio.h>
#include "wxpobj.h"
#include "wxpptr.h"
#include "wxpfield.h"

#define PROPERTY_INT(name,rd,wr)\
    wxpPropertyBegin(name,thisclass,int)\
	wxpPropertyRead (name,thisclass,int,rd)\
	wxpPropertyWrite(name,thisclass,int,wr)\
    wxpPropertyEnd(name,thisclass,int)

#define thisclass cChild //************************************************
class thisclass : public wxpObject //***************************************
{
public:

// properties
    PROPERTY_INT(x,x_read,x_write)
    PROPERTY_INT(y,y_read,y_write)

// events
    wxpEvent(OnPosition, thisclass, int);


// fields for properties
    int f_x, f_y;
    

// bind everything    
    virtual void bind_data()
    {
	printf("cChild::bind_data() started.\n");
	
	wxpPropertyBind(x);
	wxpPropertyBind(y);
	wxpEventBind(OnPosition);
	
	printf("cChild::bind_data() finished.\n");
    } 

// initialize default data
    virtual void init_data()
    {
	f_x = -1; // default x value
	f_y = -1; // default y value
    }    

    
// handlers
    const int &x_read() 
	{ return f_x; }
	
    const int &x_write(const int &data)
	{ f_x = data; wxpEmitEvent(OnPosition,1); return data; }
    
    const int &y_read() 
	{ return f_y; }
	
    const int &y_write(const int &data)
	{ f_y = data; wxpEmitEvent(OnPosition,2); return data; }


// standard constructor/destructor for wxpObject derived class;
// you should always call init_seq() in constructor
    thisclass() : wxpObject() { init_seq(); }
    virtual ~thisclass() {}    
    
};//************************************************************************
#undef thisclass //*********************************************************


#define thisclass cDemo //**************************************************
class thisclass : public wxpObject //***************************************
{
public:

// members    
    wxpPtr<cChild> child1,child2;

// handlers    
    wxpHandler(OnChild1Position, thisclass, int);
    wxpHandler(OnChild2Position, thisclass, int);

// bind everything    
    virtual void bind_data()
    {
	printf("cDemo::bind_data() started.\n");
	
	wxpHandlerBind(OnChild1Position,  OnPosition, *child1);
	wxpHandlerBind(OnChild2Position,  OnPosition, *child2);
	
	printf("cDemo::bind_data() finished.\n");	
    } 
    
// allocate members
    virtual void alloc_data()  
    {
	*child1; // for wxpPtr it is equal to child1 = new cChild(child1),
	*child2; // but we really don't care about type here
    } 

// free members    
    virtual void free_data()
    {
	child1 = NULL; // for wxpPtr it is equal to delete child1,
	child2 = NULL; // but actually we need not to deallocate them 
	               // at all - wxpPtr does all automatically
    } 
    
// handlers    
    const void OnChild1Position(const int &tag)
    {
	printf("cDemo::OnChild1Position() Child 1 is positioned to %d %d (tag=%d)\n",child1->x.get(),*&child1->y.get(),tag);
    }
    
    const void OnChild2Position(const int &tag)
    {
	printf("cDemo::OnChild2Position() Child 2 is positioned to %d %d (tag=%d)\n",child1->x.get(),*&child1->y.get(),tag);
    }
    

// standard constructor/destructor for wxpObject derived class;
// you should always call init_seq() in constructor
    thisclass() : wxpObject() { init_seq(); }
    virtual ~thisclass() {}

};//************************************************************************
#undef thisclass //*********************************************************


void main()
{
    cDemo p;
    p.child1->x=7;
    p.child1->y=10;
}

