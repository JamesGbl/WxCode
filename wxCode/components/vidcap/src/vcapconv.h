/////////////////////////////////////////////////////////////////////////////
// Name:        vcapconv.h - wxVideoCaptureWindow image conversions
// Author:      John Labenski
// Created:     7/06/2001
// Modified:    01/14/03
// Copyright:   John Labenski
// License:     wxWidgets V2.0
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_VCAPCONV_H__
#define __WX_VCAPCONV_H__

//#include <linux/videodev.h>

void Y8_to_RGB24(void *src, void *dst, int width, int height);


#endif //__WX_VCAPCONV_H__
