
 WebUpdate i18n support
 ======================

 This folder contains:
 -> a PO template file (webupdater.pot)
 -> .PO translation files
 -> this readme

 In order to update the .PO safely (i.e. without losing already translated
 strings), run the 'make i18n' command from a Unix box after ./configure-ing.



 Notes about translations
 ========================

 All the GUI messages can be translated and are listed in the PO files.
 The messages which will appear only in the LOG file do not use _() in order
 to keep the number of sentences to translate to an acceptable number.

 Those sentences which starts with 'class::function -' prefix must be translated
 since they are in wxLogUsrMsg() function calls which are shown to the user using
 the advanced GUI mask.
 (However the 'class::function -' prefix must be left untouched!).



 How to distribute WebUpdater with translation files
 ===================================================

 Just distribute the EXE file together with the 'lang' folder which you can find
 in the sample folders after compiling the project.

 WebUpdater will search at run-time for that LANG subfolder and eventually load
 from there the .MO files.


 $Id$