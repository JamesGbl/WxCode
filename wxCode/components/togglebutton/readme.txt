wxCustomToggleCtrl is a "minimal" example of a custom control for
wxWindows version 2. It is a simple toggle button with a text label
which mirrors the functionality of the standard wxCheckBox control.

The code is heavily commented (perhaps too heavily for some tastes)
and includes notations of which class in the derivation sequence
the function calls are documented in - the derivation sequence is:
	wxCustomToggleCtrl
	wxControl
	wxWindow
	wxEvtHandler
	wxObject
	
The "interesting" class functions are :
	- Create() which notes important differences between wxGTK and
		wxMSW in terms of what you get when deriving a class
		from wxControl.
	- OnLeftClick() which handles sending out the control's single
		event.
	
The OnPaint() function is intentionally kept to a minimum to focus
on the steps needed for a custom control (rather than the "artwork"
of drawing) but does address the primary functionality of drawing
the different control states and altering the control when it is
disabled.

The event requirements are about as simple as possible for a non-static
control. The control handles EVT_PAINT and EVT_LEFT_DOWN events sent to
it and sends out a single custom event (handled by "user" classes with
EVT_CUSTOM_TOGGLED) when clicked.

The files included are :
	ReadMe				this file
	toggle.h				the toggle control's header file
	toggle.cpp			the toggle control's source file
	Makefile				make file for wxGTK
	makefile.b32			make file for Borland 32-bit
	mondrian.ico			the .ico icon (Windows)
	mondrian.xpm			the .xpm icon (GTK)
	minimal.cpp			the minimal sample w/ a single toggle
	minimal1.cpp			adds a "Toggle" menu item to test
						changing the control's characteristics
						(size, colour, font, enable, etc).
						Rename minimal.cpp to something else
						and this file to "minimal.cpp" to
						compile this extended version
	minimal.rc			the Windows .rc file
	minimal.def			the Windows .def file

As the list suggests, I have tested the control under
	Linux w/ wxGTK using gcc
	Windows w/ wxMSW using Borland 5.0
Please let me know if you have any problems with other enviornments (I'd
particularly be curious about how it behaves with wxMAC). For other
compilers/enviornments, you'll need to modify the make file from your
/sample/minimal to compile and link-in toggle.cpp (look at the make file
for the /sample/db in your setup to see how to alter a make file for
multiple .cpp files).

You should unpack the archive to your wxWindows directory and these files
should then apprear under "samples/custom".

Good luck and have fun!

Bruce Phillips
2001/06/03

Direct questions, comments, etc. to aphillips@erols.com