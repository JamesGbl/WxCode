A wxWindow with the Internet Explorer ActiveX Component embedded. 

IEHtmlWin.cpp
IEHtmlWin.h

This is a subclass of wxWindow, so you can use it in your wx application easily. It has a method to open urls, and you can subclass it to get a few events (before loading a url, finished loading a url, and progress while loading a url).

It is very rough. I just got it working 10 minutes before I wrote this. It has a variety of unnecessary stuff in the size and paint event handlers (from when I was trying to figure out why it wasn't working).

The bulk of the code is effectively an implementation of the the OLE site/frame interfaces (where an OLE object is embedded). This stuff is built into MFC and ATL, but since those are not compatible with wx, I reimplemented all of this crap. It might be worthwhile to generalize some of this stuff for general wx purposes.

You can also embed mozilla using this code thanks to the ActiveX mozilla component. You just need to change the the CoCreateInstance call in IEHtmlWin::CreateBrowser(), replacing "CLSID_WebBrowser" with "CLSID_MozillaBrowser". The mozilla CLSID is already in the code. You will need mozilla installed, of course. 

Anyway, feel free to use this and contribute improvements. I'll be cleaning it up over the next few days/weeks as I integrate it into the app I'm working on, too.

Justin Bradford
justin@maxwell.ucsf.edu 