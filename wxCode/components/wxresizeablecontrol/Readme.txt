Documentation for wxResizeableControl and derived classes


Purpose:

Create a base class for controls that the user can resize, 
move, copy, paste and print and that has a simple programming 
interface. Derive controls for display of resizeable and moveable 
Metafiles and bitmaps.


Build:

Make sure you install and build wxWindows first (2.4.X,
probably works also with 2.5.x)


Windows:

BCC 5.5.1:

Execute

make -f Makefile.b32

to build the class and sample.

MinGW 2.0:

Execute

make -f Makefile.G95

to build the class and sample.

VC (untested):

Execute

nmake -f Makefile.vc

to build the class and sample.


LINUX:

You probably need to hand modify the Makefile.in to
change the path were you installed wxWindows.

The execute

make -f Makefile.in

to build the class and sample.


Run the sample resizecd(.exe).


Implementation:


wxResizeableControl:

Abstract base class. Defines function members to provide a common 
interface for printing, setting the with/height ratio and zoom.


wxPictureControl:

Abstract base class for pictures, derived from wxResizeableControl. 
Defines function members to provide a common interface for getting/setting 
picture data, picture type.


wxEnhancedMetafileControl:

Derived from wxPictureControl, implements resizeable Metafile controls. wxMSW only.


wxBitmapControl:

Derived from wxPictureControl, implements resizeable bitmap controls.


wxResizeableControlCanvas:

Parent window for wxResizeableControls. Responds to child window notifications by 
recalculation of scrollbars.
