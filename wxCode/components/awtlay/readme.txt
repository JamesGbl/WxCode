
               ----------------------------------
               Overview for AWT-Layout's contrib.
               ----------------------------------

Version: 0.1.1
Updated: 2000/01/20
Authors: Aleksandras Gluchovas
 E-mail: <alex@soften.ktu.lt>
    URL: http://soften.ktu.lt/~alex/awtlayouts/

Contents:

1. Interface-level compliance with Java's AWT
2. "Two-phase layouting" feature
3. Concept of "Component's Peer"
4. Building this contrib.
5. Problems ruining it with wxGtk
6. Tips for trying it
7. Tested on..

Abstract
--------

contrib. contains wxWindows based C++ classes
which try to mimic functionality and interfaces
of AWT layout managers in the corresponding classes
of JDK 1.0-1.2 ( in javax.awt and javax.swing packages)

Interface-level compliance with Java's AWT
------------------------------------------

The strength was to make C++ versions of these classes
similar to AWT ones in behaviour and also at the level
of their public-interfaces. Initially, it seamed
reasonable to derive these classes from wxSizer class, 
but later it appeared that derivation from wxSize makes
it hard to maintain interface-compliance with AWT, e.g.:

in wxWIndows we don't have containers which are not bound
to native window, instead we use nesting of wxSizers inside
each other, that is - one layout mgr. directly contains another,
whereas AWT has a logical separation between container and 
layout-manager, this makes "derivation from wxSizer" approach
tricky or even impossible.

The design was turned to strictly follow interface-compliance 
with AWT, which also honors  easier conversion of generated
java GUI-code designed using visual GUI-designers for Java, and
then manually converting resulting code into C++, e.g.

java code:

	Container panel = new java.awt.Container();
	panel.setLayout( new java.awt.FlowLayout() );
	
	panel.add( new JButton("Hello" );
	
could be converted to C++:

	Container* panel = new Container( this );
	panel->setLayout( new FlowLayout() );
	
	panel.add( new wxButton( this,-1, "Hello" ) );

Besides layout-managers there are other classes introduced 
for AWT-compliance, they form a tiny framework the purpose
of which is to wrap some native wxWindow classes the way 
"awt.dll/.so" of Java VM does for every platform.
Further this fmw. is named AWT-Emulation-layer (AWTEL).

For info on how to use these layouts, download docs. of 
JDK 1.1 or later, and see sections descirbing layout managers.
Also you may look at the layout_test.cpp which contains sections 
for testing each C++ based layout manager.


"Two-phase layouting" feature
-----------------------------

This feature is NOT present in AWT, in fact, it is notable shortcomming
of AWT that i've faced long ago, but now it is eliminated in this "C++ version".

The problem is, that when a composite component is requested to give
it's preferred size, at this point it does not know dimensions
of the parent which is asking it's pref. size... not knowing of the
parent's size causes e.g. FlowLayout to calc. it's pref.
size in the "unconstrained" way - not wrapping it's items around 
the edges of the parent, thus giving not really correct preferred-height. 

To obtain the necessary parent-size info, we lay out the container
twice, firstly to see how parent lays out it's children and what
sizes do they get, secondly - to get the actual pref. sizes (when 
sizes of all parent are already calculated form Phase-I) and
relayout the children with "awearness" of parent's size.


Concept of "Component's Peer"
-----------------------------

Each component in AWTEL (like in AWT) may have it's corresponding
native window, the class which wraps-up this native window is
called "peer", and is derived from java.awt.ComponentPeer. 
In the case of AWTEL, the ComponentPeer is simply typedef'ed
to wxWindow. 

Also, in the case when component's peer == NULL, the component
is considered light-weight (i.e. without it's native equivalent),
and is rendered using it's paint() method.

Building this contrib.
----------------------

Currently there is no "stand-alone" makefile for this contrib.
You'd have to copy the source of distribution into "awtlayouts"
directory in you "wxgtk/samples/" or "$(WXWIN)\samples\" (on Windows)

On unix, run the "configure" or any other script to process Makefile.in

then make it.

The makefiles may be buggy, please report problems to me (alex@soften.ktu.lt),
i'll try to fix them and post on wx-developers list.


Problems ruining it with wxGtk
-----------------------------

the library i am using is rather old one (perhaps 2.0.1). 
There are things like wxMenuBar slipping over main panel,
double-buffering not functioning (because of auto-erasing of
window background under X). wxGtk that i have, crashes 
when trying to create wxStaticText, so i #ifdef'ed code to
use wxPanel for non-windows platforms.

Also, if you run the demo from different directory then 
the one containing .cpp files (e.g. form ./Linux), then copy 
"layout_test.cpp" to that directory, otherwise the you'll
not be able to see the source code for each layout.


Tips for trying it
==================

Try to choosing different fonts (size/face/etc).

Settings dialog may be kept on top if you press
"Apply" button (not "Apply and Exit"), that way
you can see how different settings affect current
layout without leaving the settings-dialog. It's
the feature i've used very well myself, while 
"hunting" bugs... :)

Tested on..
===========

wxGtk 2.0.1 on RedHad 5.0 with gcc v.-dont-know

wxMSW 2.0.1 and 2.1.11 on Windows NT 4.0 with MSDev 4.0