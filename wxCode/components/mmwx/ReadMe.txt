
mmwx ReadMe
 ---------------------

 Website: http://wxcode.sourceforge.net/components/mmwx
 Author: Arne Morken, Gary Harris
 Version: 2.0
 Description:

 The mmwx library - brief description
-----------------------------------------------------

This is some useful classes for wxWindows.
They compile under wxMSW 2.3.2 with BCC 5.5, but shouldn't give
to much trouble for other configurations. Please submit patches,
makefiles, comments, etc. to arne.morken@mindmatters.no.
Visit www.mindmatters.no/mmwx for more information.


GUI classes:

o mmMultiButton - Bitmap/textlabel, toggle, autodraw.
o mmNavigator   - Simple navigation panel:  <<   <   Btn   >   >>
o mmMonthCtrl   - Calendar (month), with day/month/year selection.
o mmDropMonth   - mmMultiButton that can drop down a mmMonthCtrl.
o mmDropMenu    - mmMultiButton that can drop down a menu.
o mmDropWindow  - mmMultiButton that can drop down any window.
o mmHyperText   - 'Hyperlinked' text label.
o mmHyperBitmap - 'Hyperlinked' bitmap.
o mmTextStretch - Stretchable wxTextCtrl.
o mmLogonDialog - Simple logon dialog.

NON_GUI classes:

o mmDbase    - To simplify using wxODBC classes (wxDb, wxDbTable, etc.)
						Only tested with Interbase, but should work with any db that wxDb/wxDbTable supports.
						You can also create a data source with this class.
o mmMAPIMail - Send and receive email using MAPI (MS Windows only)


There is a test program, mmwxtest.cpp, for the GUI classes and a
separate test program for mmDbase, dbmm.cpp. There's also dbwx.cpp,
showing the difference by using mmDbase and wxDb/wxDbTable for some
simple database task. No test program for mmMAPIMail yet, but that's
very simple anyway.



 ChangeLog
 -----------------

 Version: 2.0

Updated for wxWidgets 2.8.10, April-May 2010.

Changes:
-Created Code:Blocks project.
-Fixed calls to missing or incorrect bitmap names.
-Changed the fixed grey button background to use the system button colour.
-Made the arrow and other images transparent so that they are background-independent.
-Changed code that checked bitmap validity to use IsOk().
-Made changes to reflect changes in mouse events since wx 2.4.
-Since http://www.mindmatters.no no longer exists, I have changed the URLs for the hyperlink controls to http://www.codeblocks.org/.
-Bumped version number to 1.1 since the changes are internal and should be backwards compatible.
-Reformatted the code using AStyle in Code:Blocks. Feel free to do the same for your own use.
-Reorganised files into src/, include/, etc. to meet wxCode's standards.
-Added doxygen docs.
-Cleaned up a bunch of unused variables and unnecessary code.
-Added DLL compilation definitions and reorganised things to suit.
-Decided that the version number should be increased to 2 as the changes are becoming significant.

Notes:
-Define __MMDEBUG__ if you want the debug window.
-The username and password for the logon box are:
	user:	mindmatters
	pw:	mmwx
-The demo uses mmMultiButtons for the button navigator and the Logon button. wxButton works just as well and provides the modern UI look.
-Since wxODBC has been removed from wxWidgets 2.9, mmDbase is considered deprecated and I have not tested it.
-I have not been able to test mmMAPIMail due to the lack of a MAPI server. Feedback is requested.
