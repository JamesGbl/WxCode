#ifndef __AWX_GRAYIMAGE_H
#define __AWX_GRAYIMAGE_H

#include <wx/bitmap.h>
#include <wx/image.h>

class wxGrayImage : public wxImage
{
public:
    wxGrayImage(const wxBitmap& bitmap);
protected:
    void DoGrey();
    unsigned char GreyColour(unsigned int red,
					    unsigned int green,
					    unsigned int blue);
};

#endif
