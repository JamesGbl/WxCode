/////////////////////////////////////////////////////////////////////////////
// Name:        wxpptr.cpp
// Purpose:     Automatic pointer template class data
// Author:      George Yohng
// Modified by:
// Created:     13-Apr-2000
// RCS-ID:      $Id: wxpptr.cpp,v 1.1.1.1 2003-12-28 21:08:01 wyo Exp $
// Copyright:   (c) 2000 DosWare, corp. crew
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/*===========================================================================*\
    Copyright (C)2000 by George Yohng / DosWare, corp. crew.    
    DosWare is Digital Operation Software, Corp. Crew
\*===========================================================================*/
#include "wxpptr.h"

#include <stdio.h>
std::map<void *,int> __wxpPtr__data__;
