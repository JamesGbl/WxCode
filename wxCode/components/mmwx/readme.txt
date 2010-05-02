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
               Only tested with Interbase, but should work with any db
	       that wxDb/wxDbTable supports. You can also create a data 
               source with this class.
o mmMAPIMail - Send and receive email using MAPI (MS Windows only)


There is a test program, mmwxtest.cpp, for the GUI classes and a
separate test program for mmDbase, dbmm.cpp. There's also dbwx.cpp,
showing the difference by using mmDbase and wxDb/wxDbTable for some
simple database task. No test program for mmMAPIMail yet, but that's
very simple anyway.

