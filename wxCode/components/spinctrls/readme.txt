Well, I've been away, but I'm back and here it is as I 
threatened. :) 
This is a GTK and MSW spinctrl that can handle floating 
point numbers. Unfortunately the native GTK control 
uses floats, :( and so there are some horrible roundoff 
errors for small values (large for my purposes). 
As Robert had suggested it is a copy of the wxSpinCtrl 
(for GTK), in fact these are copies of BOTH the GTK and 
MSW SpinCtrls from CVS with as little code 
added/removed as possible. It generates a regular 
wxSpinEvent and has been "compiled in" with both the 
GTK and MSW library with Linux Mandrake 7.2 and Win 
95&98 with bcc5.5. 

Included are 
spinreal.h -> include/wx/spinreal.h 
spinrealm.h -> include/wx/msw/spinreal.h 
spinrealm.cpp -> src/msw/spinreal.cpp 
spinrealg.h -> include/wx/gtk/spinreal.h 
spinrealg.cpp -> src/gtk/spinreal.cpp 
myminimal.cpp a generic test program 

Feel free to use/modify whatever to it as you will, I 
kept the date/version of the spinctrl's files I copied 
and only added my name to the header in case anyone 
wants to complain about them. :) 

I realize that there wasn't much interest for it in the 
message board, but I would imagine that any vaguely 
scientific application could have a use for it. I hope 
you can find a place for it in the library. 
In the least, the process of merely modifying a 
preexisting control taught me that both GTK and MSW are 
absurdly difficult (maybe obtuse is the right word) to 
program in, especially after using wxWindows for a 
short while. 

Thanks for your time and a great library, 
John Labenski 

ps. Vadim, if you look at the GTK version you can see 
how I made this GTK spinctrl stop updating every 
keypress, however in the MSW spinctrl (both this and 
the wxSpinCtrl) the RETURN key does the same thing as 
as TAB (naviagation) and I cannot figure out how to fix 
that. :( 