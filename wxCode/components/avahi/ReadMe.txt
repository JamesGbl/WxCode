 
 wxAvahi ReadMe
 -------------
 
 Website: http://wxcode.sourceforge.net/components/wxavahi
 Author: Kia Emilien
 Version: 0.1.0 
 Description:
 wxAvahi is a wrapping of Avahi features for wxWidgets.
 It handles Zeroconf publishing and discovery with wxWidgets features (wxString and wxEvent-base).
 
 

 Installation - win32
 --------------------
 wxAvahi is not available on Windows yet.
     
 Installation - unix
 -------------------

 When building on a Linux/GNU-based system, type

  ./configure
  make
  make install

 to build against the default build of wxWidgets (the one listed as 'default' 
 by wx-config --list). Type "./configure --help" for more info.



 ChangeLog
 ---------
 
 0.2.0
     - Fix some bugs
     - Add service type database reader
     - Add service type icon provider
     - Add list widgets to easily browse services types and providers
 
 0.1.0
     - first version of wxAvahi at wxCode
