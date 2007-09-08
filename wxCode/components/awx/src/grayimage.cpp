#include "wx/awx-0.3/grayimage.h"

wxGrayImage::wxGrayImage(const wxBitmap& bitmap) :
    wxImage(bitmap.ConvertToImage())
{
    DoGrey();
}

void wxGrayImage::DoGrey()
{
    int w = GetWidth();
    int h = GetHeight();

    // Returns the image data as an array. This is most often used when doing
    // direct image manipulation. The return value points to an array of
    // characters in RGBRGBRGB... format in the top-to-bottom, left-to-right
    // order, that is the first RGB triplet corresponds to the pixel first
    // pixel of the first row, the second one --- to the second pixel of the
    // first row and so on until the end of the first row, with second row
    // following after it and so on.
    unsigned char* rgb = GetData();

    for( int x = 0; x < w; x++ ) {
	   for( int y = 0; y < h; y++) {
		  unsigned char grey = GreyColour( *rgb, *(rgb+1), *(rgb+2) );
		  *rgb++ = grey;
		  *rgb++ = grey;
		  *rgb++ = grey;
	   }
    }
}

unsigned char wxGrayImage::GreyColour(unsigned int red,
							   unsigned int green,
							   unsigned int blue)
{
    // the grey value is calculated by: 
    // grey = ( red*0.30 + green*0.59 + blue*0.11 ) * greyColours
    return ( red * 30 ) / 100 + ( green * 59 ) / 100 + ( blue * 11 ) / 100;
}
