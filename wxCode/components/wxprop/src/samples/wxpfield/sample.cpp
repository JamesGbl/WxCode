/////////////////////////////////////////////////////////////////////////////
// Name:        sample.cpp
// Purpose:     Field class sample
// Author:      George Yohng <yohng@dosware.8m.com>
// Modified by:
// Created:     13-Apr-2000
// RCS-ID:      $Id: sample.cpp,v 1.1.1.1 2003-12-28 21:08:02 wyo Exp $
// Copyright:   (c) 1998,2000 by George Yohng
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wxpfield.h"

int main()
{
    wxpField<int> a=0,b=0,c=0;

    a.bindto(b);
    c.bindto(b);
    
    a = 777;
        
    printf("c = %d\n",*&a);
}
