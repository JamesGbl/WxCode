Result:

2 new DC's, that allows you to maintain a bitmap of all the is drawn on the canvas
window very easy.

Why:

Often people ask for a way to have a backup bitmap of what is shown on the canvas,
in order to reduce flickering, and to refresh/repaint a complex/large drawing quickly.
It is of course possible to draw on a bitmap using a memoryDC, and later blit this to the screen.
This i tried first, it has some drawback!
- a lot of house keeping is needed in a complex drawing program to keep everything up todate.
- the user will not see the progress in drawing, so for a long drawing process it is nasty.
- if the user wants to interrupt the drawing process at a certain moment, this is difficult.
- interactive drawing of polygons etc, still has to take place on the canvas itself,
  after that more house keeping is needed to update the right parts in the bitmap.
 
How:

I derived the already available DC classes at a lower level some extra DC's,
They allow you to select a bitmap into them, that will be used to draw upon at
the same time other things are drawn on the canvas.
Using the same clipping etc.
I had to do it at a lower level to be able to make them work along with the already available DC's.
No change is needed to drawing routines that are based on wxDC as parameter.
I did all this in a maximum stupid manner.
By that i mean no optimization whatsoever.
Although this could be possible to high extend, but not without duplicating much code.
The way i did now, will make it much easier to maintain this code, and the panelty is small i think.

Although it would be very easy to distribute this new code as part of the library,
i decided that it would be better at the moment to keep it at the application level.
Just include the files mentioned down here, and you should be able to use them.

See the drawing example on how to do it, it of course also depends on the application itself.

I think it should be possible to extend those new DC's with funtions to move/drag bitmaps/images
quicly across the canvas. Simply because the bitmap already contains many things
that can be used to speed up this process. 


The example:

I only updated the make files for UNIX wxGTK and makefile.b32 makefile.vc.
For the project files just include the files mentioned below.

dcclbuf.h  //top platform independent include file for new DC's

gtk/dcclbuf.h
gtk/dcclbuf.cpp

OR

msw/dcbuf.h
msw/dcbuf.cpp
msw/dcclbuf.h
msw/dcclbuf.cpp

Defines de following DC's that can be used to maintain a bitmap
that is part of the Canvas window (derived from a wxWindow)
class wxWindowDCbuf;
class wxPaintDCbuf;
class wxClientDCbuf;

The new DC's allow you to draw on a window canvas (same manner as this has to be done 
in the normal case using wxPaintDC & wxClientDC) 
The difference is that it is possible to select a bitmap into the DC's,
this bitmap will contain exactly the same drawings as all that
has been drawn on the canvas using the new DC's.
The principle is that everything that is drawn to the canvas 
will directly be drawn on the bitmap also.
Clipping region etc's work on the maintained bitmap also.

The bitmap can now be used in the OnPaint member function, 
to quickly update the canvas. 
This is very usefull if large amounts of data have to be redrawn every
time a dialog or other window is dragged accros the canvas.
And of course it takes care of flickering.


The modified drawing example shows how it needs to be used.
I think the modification are minimal to make it work for an application in general.

To see the difference clearly change this in the sample:

void MyCanvas::OnPaint(wxPaintEvent &WXUNUSED(event))
{

   if (m_valid_drawing)
   {

CHANGE TO:

void MyCanvas::OnPaint(wxPaintEvent &WXUNUSED(event))
{

   if (0)
   {

In both cases: 
-start the sample
-goto the polygon screen
-enlarge the window
-drag the about dialog across the screen (or any other window)
-and see the difference.


TODO:
-reorganize the sample to make it work for wxScrolledWindow also.
-moving bitmaps etc, using this type of DC







