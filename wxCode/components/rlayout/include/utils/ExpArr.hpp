/*
The file provides a template T array which grows
to required size.
 
Elements may be written/read through [] operator.
It may also be used as a stack through Push/Top/Pop
operators.
 
T         - The type of the elements stored in place 
INT       - The type of the index elements (can use short
            for an ExpArr that only uses 8 bytes an max 32767 elems)
            
ExpArr treats each T mostly plain memory, when copying
an array it uses memcpy/memmove rather than operator =. 
 
ExpArrObj will apply destructors as objects go out of array scope. 
It does not use constructor when including an object, instead it
copies raw mem from the 'null' elem (GetNullElem()). Invoking the
constructor can only happen through placement new and that is 
problematic (with custom new handlers). It will also use operator =
rather than memory copy for new members. When moving to a 
different memory area, it uses memcpy.
 
ExpArrP is designed to work with pointers.
 
The array does not allocate any memory until somethings is put into it.
*/

#ifndef EXPARR_HPP
#define EXPARR_HPP

#include "ea_shared.hpp" // Shared things for ExpArr and ExpArrP
#include "ArrBase.hpp"


// This is the base class for growable arrays. It can have index variable
// of te,plate type, to provide for compact 8-byte arrays. The base version
// treats all members as raw byte data and does not run any constructors
// or destructors. For memory allocation, it uses new char[].
template <class T, class INT=int >
class ExpArr {
public:
    ExpArr( const T *pt, INT n ) { Init(); Push(pt,n); }
    ExpArr( const ExpArr<T> &ea ) { Init(); Push( ea.Base(), ea.Size() ); };
    // Special constructor to move contents from other ExpArrr, breaking const, 
    // but that has to be due to C++ restriction in this case.
    ExpArr( const ExpArr<T> &ea, bool is_move ) { Init(); MoveFrom((ExpArr<T>&)ea); };
    ExpArr( ) { Init(); }
    ~ExpArr() {
#ifndef EA_REALLOC
        if(ta) delete [] (char*)ta;
#else
        if(ta) free(ta);
#endif
        memset(this,0,sizeof(*this));
    }

    // This sets length to 0 but keeps allocated buffer
ExpArr& Empty() { cur_size=0; return *this; }

    T*   Base( ) const { return ta; }
    T*   AddressToElems( ) const { return ta; }
    operator T*() { return ta; }

    INT  MaxSize( ) const { return max_size; }
    INT  Size( ) const { return cur_size; }

    // Returns a reference to an element
    T& operator [] ( int  ix ) const {
        if( ix<0 || ix>=cur_size )
            EA_THROW_RV( "ExpArr::operator[], Index out of range", GetNullElem() );
        return ta[ix];
    }

    // Returns a copy of the element
    T Elem( INT ix ) const {
        if( ix<0 || ix>=cur_size )
            EA_THROW_RV("ExpArr::Elem - Index out of range",GetNullElem());
        return ta[ix];
    }

    // Returns a copy of the element
    T& ElemRef( INT ix ) const {
        if( ix<0 || ix>=cur_size )
            EA_THROW_RV("ExpArr::Elem - Index out of range",GetNullElem());
        return ta[ix];
    }

    // Does range checking, no throw
    T ElemSafe( INT ix ) const {
        if( ix<0 || ix>=cur_size ) return GetNullElem();
        return ta[ix];
    }
    bool SetElem( INT ix, const T elem ) {
        if( ix<0 || ix>=cur_size ) return false;
        ta[ix] = elem;
        return true;
    }

    ExpArr& Push( T elem ) {
        MinFreeElem(1);
        ta[cur_size++] = elem;
        return *this;
    }
    T Pop( ) {
        if( !cur_size ) EA_THROW_RV( "ExpArr::Pop, Empty stack", GetNullElem() );
        return ta[--cur_size];
    }
    T &Top( ) const {
        if( !cur_size ) EA_THROW_RV( "ExpArr::Top, Empty stack", GetNullElem() );
        return ta[cur_size-1];
    }
    ExpArr& InsertOrdered( const T &elem, INT ix ) {
        // Accept inserting at given pos of array
        MakeSpaceAt(ix);
        ta[ix] = elem;
        return *this;
    }

    INT IndexOf( const T elem ) const {
        for(INT ix=0; ix<cur_size; ix++)
            if(ta[ix]==elem) return ix;
        return (INT)-1;
    }
    bool Has( const T elem ) const { return IndexOf(elem)!=-1; }

    bool HasItems( ) const { 
        for( int ix=0; ix<cur_size; ix++ )
            if( ta[ix]!=GetNullElem() )
                return true;
        return false;
    }

    // We should also have a Fit method to release memory when we have grown smaller
    void MinCurrentSize( INT size ) {
        // # This should be specialized to work better with ExpArrObj
        MinTotalSize(size);
        if( cur_size<size ) {
            memset( ta+cur_size, 0 , sizeof(T)*(size-cur_size) );
            cur_size = size;
        }
    }
    bool MinFreeElem( INT spc=1 ) {
        return MinTotalSize( cur_size+spc );
    }
    bool MinTotalSize( INT size ) {
        if( max_size>=size ) return true;
        INT new_size = max_size ? EA_GROW(max_size) : EA_START_SIZE;
        if( new_size<size ) new_size = size;
#ifndef EA_REALLOC
        T *nta = (T*) new char[sizeof(T)*new_size];
        if( !nta ) EA_THROW_RV( "ExpArr::MinTotalSize, Mem alloc failed - new", false );
        // Move old elements, leave no place in time where array is inconsistent
        if( cur_size )
            memmove(nta,ta,cur_size*sizeof(T));
        T *ota = ta;
        ta = nta;
        if(ota) delete [] (char*)ota;
        max_size = new_size;
#else
        // This leaves a gap in time, where realloc has freed the memory
        // but we don't yet point to the new memory. Hide the elements briefly.
        int _cur_size = cur_size;
        cur_size = 0;
        T *nta = (T*)realloc(ta,sizeof(T)*new_size);
        if( !nta ) EA_THROW_RV( "ExpArr::MinTotalSize, Mem alloc failed - realloc", false );
        ta = nta;
        cur_size = _cur_size;
        max_size = new_size;
#endif
        return true;
    }

    bool SetSize( INT size ) {
        if( size<0 ) return false;
        if( size>cur_size ) return MinTotalSize(size);
        else cur_size = size;
        return true;
    }

    // Use operator==: T t; U u; t==u
    template<class U>
    int FindIndexOf( const U &u, int ix=0, int dir=1 ) {
        for( ; ix>=0 && ix<cur_size; ix+=dir )
            if( ta[ix]==u )
                return ix;
        return -1;
    }
    // Use operator==: T t; U u; u==t
    template<class U>
    int FindIndexOfR( const U &u, int ix=0, int dir=1 ) {
        for( ; ix>=0 && ix<cur_size; ix+=dir )
            if( u==ta[ix] )
                return ix;
        return -1;
    }
    // Use operator==: T t; U u; t==u
    template<class U>
    T Find( const U &u ) {
        int ix = FindIndexOf(u);
        return ElemSafe(ix);
    }
    // Use operator==: T t; U u; u==t
    template<class U>
    T FindR( const U &u ) {
        int ix = FindIndexOfR(u);
        return ElemSafe(ix);
    }
    void Swap( INT ix1, INT ix2 ) {
        if( ix1<0 || ix1>=cur_size || ix2<0 || ix2>=cur_size )
            EA_THROW(  "ExpArr::Swap, Index out of range" );
        char buf[sizeof(T)];
        memcpy( buf, ta+ix1, sizeof(T) );
        memcpy( ta+ix1, ta+ix2, sizeof(T) );
        memcpy( ta+ix2, buf, sizeof(T) );
    }

    T RemoveIndexUnordered( INT ix ) {
        RangeCheck(ix);
        T t = ta[ix];
        memmove( ta+ix, ta+cur_size-1, sizeof(T) );
        cur_size--;
        return t;
    }
    T RemoveIndexOrdered( INT ix ) {
        RangeCheck(ix);
        T t = ta[ix];
        memmove( ta+ix, ta+ix+1, (cur_size-ix-1)*sizeof(T) );
        cur_size--;
        return t;
    }
    bool RemoveUnordered( const T &elem ) {
        INT ix = IndexOf(elem);
        if( ix==(INT)-1 ) return false;
        RemoveIndexUnordered(ix);
        return true;
    }
    bool RemoveOrdered( const T &elem ) {
        INT ix = IndexOf(elem);
        if( ix==-1 ) return false;
        RemoveIndexOrdered(ix);
        return true;
    }

    int Replace( const T find, const T repl ) {
        int n_found = 0;
        for(INT ix=0; ix<cur_size; ix++)
            if(ta[ix]==find) {
                ta[ix]=repl;
                n_found++;
            }
        return n_found;
    }

    ExpArr& Push( const ExpArr<T,INT>& other ) {  // At top of stack
        return Insert( other.ta, other.cur_size, cur_size );
    }
    ExpArr& Push( const T *elems, INT number ) {  // At top of stack
        return Insert( elems, number, cur_size );
    }
    ExpArr& Insert( const T *elems, INT number, int at_ix=0 ) {  // At arbitrary position
        if( !number ) return *this;
        if( !elems ) EA_THROW_RV( "ExpArr::Insert, NULL pointer received", *this );
    if( at_ix<0 ) { if(at_ix==EA_AT_END) at_ix=cur_size; else at_ix+=cur_size; }
        if( number<0 ) EA_THROW_RV( "ExpArr::Insert, Negative number", *this );
        if( at_ix<0 || at_ix>cur_size ) EA_THROW_RV( "ExpArr::Push, Index out of range", *this );
        MinFreeElem(number);
        MakeSpaceAt( at_ix, number );
        memmove( ta+at_ix, elems, number*sizeof(T) );
        return *this;
    }
    ExpArr& Remove( INT number, int at_ix ) {  // At arbitrary position
        if( at_ix<0 ) at_ix+=cur_size;
        if( number<0 || at_ix<0 || at_ix+number>cur_size ) EA_THROW_RV( "ExpArr::Remove, Invalid number and/or index", *this );
        INT to_ix = at_ix+number;
        memmove( ta+at_ix, ta+to_ix, (cur_size-to_ix)*sizeof(T) );
        cur_size -= number;
        return *this;
    }

    ExpArr& operator = ( const ExpArr& other ) {
        Empty();
        Push( other.ta, other.Size() );
        return *this;
    }

    // Take over array contents
    ExpArr& MoveFrom ( ExpArr<T,INT>& other ) {
        if( other.cur_size<cur_size )
            cur_size = other.cur_size;
        T *pt = ta;
        ta = other.ta;
#ifndef EA_REALLOC
        if(pt) delete [] pt;
#else
        if(pt) free(pt);
#endif
        cur_size = other.cur_size;
        max_size = other.max_size;
        other.Init();
    }
    bool operator == ( const ExpArr& other ) const {
        if( cur_size!=other.cur_size ) return false;
        return !memcmp(ta,other.ta,cur_size*sizeof(T));
    }

    
    // These are iterator supporting functions
    INT First( INT ix=-1 ){ return 0; }
    INT Last( INT ix=-1 ){ return cur_size-1; }
    INT Next( INT ix ){ return ix+1; }
    INT Prev( INT ix ){ return ix-1; }
    bool IndexValid( INT ix ){ return ix>=0 && ix<cur_size; }

    
    // Return reference to element used as non-element
    static T& GetNullElem() {
        static T st_null;
        return st_null;
    }

    // Helper function for inserting
    bool MakeSpaceAt( INT at_ix, int n_space=1 ) {
        // Accept inserting at given pos of array
        if( at_ix==EA_AT_END ) at_ix=cur_size;
        if( at_ix<0 || at_ix>cur_size )
            EA_THROW_RV( "ExpArr::RangeCheck, Index out of range", false );
        MinFreeElem(n_space);
        if( cur_size-at_ix>0 )
            memmove( ta+at_ix+n_space, ta+at_ix, (cur_size-at_ix)*sizeof(T) );
        cur_size += n_space;
        return true;
    }

    // All non-size increasing operations can be done through ArrBase structure
    ArrBase<T,INT>& GetArrBase( ) { return *reinterpret_cast<ArrBase<T,INT>*>(this); }

protected:
    // Helper function
    bool RangeCheck(int at_ix) {
        if( at_ix<0 || at_ix>=cur_size )
            EA_THROW_RV( "ExpArr::RangeCheck, Index out of range", false );
        return true;
    }

    void Init( ) {
        cur_size = max_size = 0;
        ta = NULL;
    }

    T     *ta;
    INT   cur_size;
    INT   max_size;
};

template <class T, class INT=int>
class EaIter {
public:
    EaIter( const ExpArr<T,INT> &ea ) : m_pea(&ea), m_ix(0) { }
    void Init( INT ix=0 ) { m_ix=ix; }
    T Get() {
        if( m_ix<0 || m_ix>=m_pea->Size() ) EA_THROW_RV( "EaIter::Get, Index out of range", m_pea->GetNullElem() );
        return m_pea->Elem(m_ix);
    }
T operator () () { return Get(); }
    int Index( ) { return m_ix; }
    void operator ++ () { m_ix++; }
    void operator ++ (int) { m_ix++; } // postfix
    void operator -- () { m_ix--; }
    void operator -- (int) { m_ix--; } // postfix
    bool AtEnd() { return m_ix>=m_pea->Size(); }

protected:
    const ExpArr<T> *m_pea;
    INT m_ix;
};



// ----- ExpArrP -----

// The pointer version
template<class T, class INT=int >
class ExpArrP : public ExpArr<T,INT> {
public:
    ExpArrP( const T *pt, INT n ) : ExpArr<T,INT>(pt,n) {  }
    ExpArrP( const ExpArrP<T,INT> &ea ) : ExpArr<T,INT>(ea) { }
    ExpArrP( const ExpArrP<T,INT> &ea, bool is_move ) : ExpArr<T,INT>(ea,is_move) { }
    ExpArrP( ) : ExpArr<T,INT>() { }
    //~ExpArrP();

    // # ExpArrP specific
    void DeleteAll( ) {
        for( int ix=0; ix<this->cur_size; ix++ )
            delete this->ta[ix];
        this->Empty();
    }

    // # ExpArrP specific, for efficiency
    T ElemSafe( INT ix ) const {
        if( ix<0 || ix>=this->cur_size ) return NULL;
        else return this->ta[ix];
    }

    // Use operator==: T t; U u; t==u
    template<class U>
    int FindIndexOf( const U &u, int ix=0, int dir=1 ) {
        for( ; ix>=0 && ix<this->cur_size; ix+=dir )
            if( this->ta[ix] && (*(this->ta[ix]))==u )
                return ix;
        return -1;
    }
    // Use operator==: T t; U u; u==t
    template<class U>
    int FindIndexOfR( const U &u, int ix=0, int dir=0 ) {
        for( ; ix>=0 && ix<this->cur_size; ix+=dir )
            if( this->ta[ix] && u==(*(this->ta[ix])) )
                return ix;
        return -1;
    }
    // Use operator==: T t; U u; t==u
    template<class U>
    T Find( const U &u ) {
        INT ix = FindIndexOf(u);
        return ElemSafe(ix);
    }
    // Use operator==: T t; U u; u==t
    template<class U>
    T FindR( const U &u ) {
        INT ix = FindIndexOfR(u);
        return ElemSafe(ix);
    }
};


template <class T, class INT=int>
class EapIter : public EaIter<T,INT> {
public:
    EapIter( const ExpArrP<T,INT> &ea ) : EaIter<T,INT>(ea) { }
    T Get() {
        if( this->m_ix<0 || this->m_ix >= this->m_pea->Size() )
            m_t = NULL;
        else
            m_t = this->m_pea->Elem(this->m_ix);
        return m_t;
    }
T operator () () { return Get(); }
    T m_t;
};


/*
template <class T,class INT=int>
class EapIter {
public:
   EapIter( const ExpArrP<T,INT> &ea ) : m_pea(&ea), m_ix(0) {} //{ EA_ASSERT(m_pea); }
   void Init( int ix=0 ) { m_ix=ix; }
   T Get() { return m_pea->ElemSafe(m_ix); }
   T operator () () { return m_pea->ElemSafe(m_ix); }
   int Index( ) { return m_ix; }
   void operator ++ () { m_ix++; }
   void operator ++ (int) { m_ix++; } // postfix
   void operator -- () { m_ix--; }
   void operator -- (int) { m_ix--; } // postfix
   bool AtEnd() { return m_ix>=m_pea->Size(); }
 
protected:
   const ExpArrP<T,INT> *m_pea;
   int m_ix;
};
*/



#include "ExpArr.hpp"

// We would want placement new below. However, together with debug new operators
// this becomes tricky. Instead choose to init each element with values from
// GetNullElem. (we could use placement new in release version, but then we get
// slightly different meanings to the operations).


/*
#ifdef new
	// Define way of replacing placement new
#else
	#include <new>
#endif
*/

// This version holds objects with constructors and/or destructors being applied to them
// When an object goes out of the array scope, its destructor is applied.
// When an object comes into the scope, its constructor is applied.
// Otherwise, we interpret things at 'object scope', not at raw memory level.
template <class T, class INT=int>
class ExpArrObj : public ExpArr<T,INT> {
public:
    ExpArrObj( const T *pt, INT n ) : ExpArr<T,INT>() { Push(pt,n); }
    ExpArrObj( const ExpArrObj<T> &ea ) : ExpArr<T,INT>() { Push(ea); }
    // Special constructor to move contents from other ExpArrr, breaking const, 
    // but that has to be due to C++ restriction in this case.
    ExpArrObj( const ExpArr<T> &ea, bool is_move ) : ExpArr<T,INT>(ea,is_move) { }
    ExpArrObj( ) : ExpArr<T,INT>() { }
    ~ExpArrObj( ) { DestroyAll(); }

    void Empty() { DestroyAll(); }
    ExpArrObj& DestroyAll( ) {
        // Reduce size first, then delete elems
        int sz = this->cur_size;
        this->cur_size = 0;
        for( int ix=0; ix<sz; ix++ )
            this->ta[ix].~T();
        return *this;
    }

    void MinCurrentSize( INT size ) {
        this->MinTotalSize(size);
        for( int ix=this->cur_size; ix<size; ix++ )
        	memcpy( this->ta+ix, &this->GetNullElem(), sizeof(T) );
            //new ((void*)(this->ta+ix)) T();
        this->cur_size = size;
    }


    ExpArrObj& Push( const T& elem ) {
        this->MinFreeElem(1);
        // Placement new to run constructor
    	memcpy( this->ta+this->cur_size, &this->GetNullElem(), sizeof(T) );
        //new ((void*)(this->ta+this->cur_size))T();
    	
        this->ta[this->cur_size++] = elem;
        return *this;
    }
    // Push an elemnt but don't set it to anything (meaning run constructor on it)
    ExpArrObj& Push( ) {
        this->MinFreeElem(1);
        // Placement new to run constructor
    	memcpy( this->ta+this->cur_size++, &this->GetNullElem(), sizeof(T) );
        //new ((void*)(this->ta+this->cur_size++))T();
        return *this;
    }
    ExpArrObj& PushVol( T& elem ) {  // Allows for operator = to modify elem
        this->MinFreeElem(1);
        // Placement new to run constructor
    	memcpy( this->ta+this->cur_size, &this->GetNullElem(), sizeof(T) );
        //new ((void*)(this->ta+this->cur_size))T();
        this->ta[this->cur_size++] = elem;  // Here!
        return *this;
    }
    ExpArrObj& PushElem( T elem ) {  // Copy element here (needed?)
        this->MinFreeElem(1);
        // Placement new to run constructor
    	memcpy( this->ta+this->cur_size, &this->GetNullElem(), sizeof(T) );
        //new ((void*)(this->ta+this->cur_size))T();
        this->ta[this->cur_size++] = elem;
        return *this;
    }

    void Pop( ) {
        if( !this->cur_size )
            EA_THROW_RV( "ExpArrObj::Pop, Empty stack", (ExpArr<T,INT>::GetNullElem()) );
        this->ta[this->cur_size].~T();
        // No return element
    }

    INT IndexOf( const T& elem, INT ix=0, INT dir=1 ) const {
        for(; ix>=0 && ix<this->cur_size; ix+=dir)
            if(this->ta[ix]==elem) 
                return ix;
        return (INT)-1;
    }
    
    bool Has( const T& elem ) const { return IndexOf(elem)!=-1; }

    void RemoveIndexUnordered( INT ix ) {
        this->RangeCheck(ix);
        this->ta[ix].~T();
        ExpArr<T,INT>::RemoveIndexUnordered(ix);
    }
    void RemoveIndexOrdered( INT ix ) {
        this->RangeCheck(ix);
        this->ta[ix].~T();
        ExpArr<T,INT>::RemoveIndexOrdered(ix);
    }
    bool RemoveUnordered( const T &elem ) {
        INT ix = this->IndexOf(elem);
        if( ix==(INT)-1 ) return false;
        RemoveIndexUnordered(ix);
        return true;
    }
    bool RemoveOrdered( const T &elem ) {
        INT ix = this->IndexOf(elem);
        if( ix==-1 ) return false;
        RemoveIndexOrdered(ix);
        return true;
    }
    ExpArrObj& InsertOrdered( const T &elem, INT ix ) {
        this->MakeSpaceAt( ix );
    	memcpy( this->ta+ix, &this->GetNullElem(), sizeof(T) );
        //new ((void*)(this->ta+ix)) T(); // If we have custom allocator, this might not have happened
        this->ta[this->ix] = elem;
        return *this;
    }

    int Replace( const T& find, const T& repl ) {
        int n_found = 0;
        for(INT ix=0; ix<this->cur_size; ix++)
            if( this->ta[this->ix]==find ) {
                this->ta[this->ix]=repl;
                n_found++;
            }
        return n_found;
    }
    ExpArrObj& Push( const ExpArrObj<T,INT>& other ) {  // At top of stack
        return Insert( other.ExpArr<T,INT>::ta, other.ExpArr<T,INT>::cur_size, this->cur_size );
    }
    ExpArrObj& Push( const T *elems, INT number ) {  // At top of stack
        if( !number ) return *this;
        return Insert( elems, number, this->cur_size );
    }
    ExpArrObj& Insert( const T *elems, INT number, INT at_ix=0 ) {  // At arbitrary position
        if( !number ) return *this;
        if( !elems ) EA_THROW_RV( "ExpArrObj::Insert, NULL pointer received", *this );
        if( at_ix<0 ) at_ix+=this->cur_size;
        if( number<0 ) EA_THROW_RV( "ExpArrObj::Insert, Negative number", *this );
        if( at_ix<0 || at_ix>this->cur_size ) EA_THROW_RV( "ExpArrObj::Push, Index out of reange", *this );
        //MinFreeElem(number);
        MakeSpaceAt( at_ix, number );
        for( int ix=0; ix<number; ix++ ) {
        	memcpy( this->ta+at_ix+ix, &this->GetNullElem(), sizeof(T) );
            //new ((void*)(this->ta+at_ix+ix)) T();
            this->ta[at_ix+ix] = elems[ix];
        }
        return *this;
    }

    ExpArrObj& Remove( INT number, INT at_ix ) {  // At arbitrary position
        if( number<0 || at_ix<0 || at_ix+number>this->cur_size ) EA_THROW_RV( "ExpArr::Remove, Invalid number and/or index", *this );
        INT to_ix = at_ix+number;
        // Destructors on elements to be removed
        for( INT ix=at_ix; ix<to_ix; ix++ )
            this->ta[this->ix].~T();
        ExpArr<T,INT>::Remove(number,at_ix);
        return *this;
    }

    ExpArrObj& operator = ( const ExpArrObj<T,INT>& other ) {
        // Same code as base case, but Push calls local func
        this->Empty();
        Push( other.ta, other.cur_size );
        return *this;
    }

    bool operator == ( const ExpArrObj<T,INT>& other ) const {
        if( this->cur_size!=other.cur_size ) return false;
        for( INT ix=0;ix<this->cur_size;ix++ )
            if ( !(this->ta[ix]==other.ta[ix]) )
                return false;
        return true;
    }

};

// Type registration, if typeof.h included before us
#ifdef TYPEOF_H
    #undef TYPE_REG_FILE
    #define TYPE_REG_FILE 100
    TYPE_REG_T2(ArrBase)
    TYPE_REG_T2(ExpArr)
    TYPE_REG_T2(ExpArrP)   // No need to redo the InnerType stuff
    TYPE_REG_T2(ExpArrObj) // for derived classes. Yes, we have to.
#endif

/*
// An ExpArr that has a custome allocator
template <class T, class INT=int, 
          AllocFunc AF=&NewArrAlloc<T>, FreeFunc FF=&DelArrFree<T> >
class ExpArrAlloc {
public:
};
*/

#endif // EXPARR_HPP


