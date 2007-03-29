/////////////////////////////////////////////////////////////////////////////
// Name:        fifo.h
// Purpose:
// Author:      Joachim Buermann, Michael Hungershausen
// Id:          $Id$
// Copyright:   (c) 2006 Joachim Buermann
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if !defined ( __FIFO_H )
#define __FIFO_H

#include <sys/types.h>

class fifo
{
protected:
    size_t m_size;
    char* m_begin;
    char* m_end;
    char* m_rdptr;
    char* m_wrptr;
public:
    fifo(size_t size);
    virtual ~fifo();
    virtual void clear();
    virtual int get(char* ch);
    size_t items();
    virtual int put(char ch);
    virtual int read(char* data,int n);
    virtual int write(char* data,int n);
};

#endif
