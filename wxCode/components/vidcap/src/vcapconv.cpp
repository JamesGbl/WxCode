/////////////////////////////////////////////////////////////////////////////
// Name:        vcapconv.c, wxVideoCaptureWindow video converters
// Author:      John Labenski
// Created:     07/06/01
// Modified:    01/14/03
// Copyright:   John Labenski
// License: wxWidgets V2.0
/////////////////////////////////////////////////////////////////////////////

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "precomp.h"
#include "vcapconv.h"

#include <wx/mstream.h>

// this function is junk...
void Y8_to_RGB24(void *src, void *dst, int width, int height)
{
    int i, length = width*height;
    unsigned char *s = (unsigned char*)src;
    unsigned char *d = (unsigned char*)dst;

    for(i=0; i<length; i++)
    {
//        dst[i] = src[i]<<16 | src[i]<<8 | src[i];

        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
    }
}

bool wxVidCap_JPEG_to_wxImage(void* src, size_t src_length, wxImage& dstImage)
{
    wxMemoryInputStream is(src, src_length);

    return dstImage.LoadFile(is, wxBITMAP_TYPE_JPEG);
}
