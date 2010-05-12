#ifndef MMWX_H_INCLUDED
#define MMWX_H_INCLUDED

//##########################################################################
//									Main page for doxygen docs.
//##########################################################################

/*! \mainpage mmwx
 *
 * \image html mmwx20.jpg "mmwx Test Application"
 * \author 	Gary Harris.
 * \version 	2.0 Beta
 * \date    	11 May, 2010
 *
 * \section intro Introduction
 * mmwx is a collection of controls and classes for wxWidgets. It was originally created by Arne Morken around 2000 but has been orphaned for a number of years and
 * not updated for recent versions of wxWidgets.
 *
 * I have updated it to work with wxWidgets 2.8.10 and done some cleaning up so that the package can be used again.
 * mmwx contains the following classes:
 *
 * GUI classes:
 * \li mmMultiButton - Bitmap/textlabel, toggle, autodraw.
 * \li mmNavigator   - Simple navigation panel:  <<   <   Btn   >   >>
 * \li mmMonthCtrl   - Calendar (month), with day/month/year selection.
 * \li mmDropMonth   - mmMultiButton that can drop down a mmMonthCtrl.
 * \li mmDropMenu    - mmMultiButton that can drop down a menu.
 * \li mmDropWindow  - mmMultiButton that can drop down any window.
 * \li mmHyperText   - 'Hyperlinked' text label.
  *\li mmHyperBitmap - 'Hyperlinked' bitmap.
 * \li mmTextStretch - Stretchable wxTextCtrl.
 * \li mmLogonDialog - Simple logon dialog.
 *
 * Other classes:
 * \li mmDbase    - To simplify using wxODBC classes (wxDb, wxDbTable, etc.)\n
 *  Only tested with Interbase, but should work with any db that wxDb/wxDbTable supports. You can also create a data source with this class.
 * \li mmMAPIMail - Send and receive email using MAPI (MS Windows only).
 *
 * You can download mmwx from http://wxcode.sourceforge.net/components/mmwx.
 *
 * \section changes Changes
 * \li Created Code:Blocks project.
 * \li Fixed calls to missing or incorrect bitmap names.
 * \li Changed the fixed grey button background to use the system button colour.
 * \li Made the arrow and other images transparent so that they are background-independent.
 * \li Changed code that checked bitmap validity to use IsOk().
 * \li Made changes to reflect changes in mouse events since wx 2.4.
 * \li Since http://www.mindmatters.no no longer exists, I have changed the URLs for the hyperlink controls to http://www.codeblocks.org/.
 * \li Bumped version number to 1.1 since the changes are internal and should be backwards compatible.
 * \li Reformatted the code using AStyle in Code:Blocks. Feel free to do the same for your own use.
 * \li Reorganised files into src/, include/, etc. to meet wxCode's standards.
 * \li Added doxygen docs.
 * \li Cleaned up a bunch of unused variables and unnecessary code.
 * \li Added DLL compilation definitions and reorganised things to suit.
 * \li Decided that the version number should be increased to 2.0 as the changes are becoming significant.
 *
 * \section notes Notes
 * \li Define __MMDEBUG__ in mmwxTest if you want the debug window.
 * \li The username and password for the logon box are:\n
 *	user:	<span class="keyword">mindmatters</span>\n
 *	pw:	<span class="keyword">mmwx</span>
 * \li The demo uses mmMultiButtons for the button navigator and the Logon button. wxButton works just as well and provides the modern UI look.
 * \li Since wxODBC has been removed from wxWidgets 2.9, mmDbase is considered deprecated and I have not tested it.
 * \li I have not been able to test mmMAPIMail due to the lack of a MAPI server. Feedback is requested.
 *
 * \section history History
 * Version 1.0 - 2000 - 2002.
 * \li Original package created and maintained by Arne up until wxWidgets 2.4.
 *
 */

#endif // MMWX_H_INCLUDED
