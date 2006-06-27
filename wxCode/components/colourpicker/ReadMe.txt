 
 wxColourPicker ReadMe
 ---------------------
 
 Website: http://wxcode.sourceforge.net/components/wxcolourpicker
 Author: Juan Antonio Ortega
 Version: 0.1
 Description:
 A control to choose colours. It is made up of a button and a drop-down menu, which shows some
 predefined colours. Also, it is possible to pick a custom colour.
 
 

 Installation - win32
 --------------------
 
 In order to compile the sample, there is a CodeBlocks project in the samples directory, which
 has two build targets: one for MSVC and another one for GCC-Mingw. For the project to work,
 the global variable "wx" must be defined within CodeBlocks. If you want to add the control to
 your projects, just add wx/colourpicker.h and colourpicker.cpp to them.


     
 Installation - unix
 -------------------

 The control has not been tested on Linux/Unix, but it *should* work. If there are problems,
 please let me know and I will try to fix them.

 
 Platform-specific issues
 ------------------------
	*The drop shadow effect on the drop-down menu requires Windows XP.
	*The button looks pressed when the drop-down menu is being shown. This feature is Windows specific.


 ChangeLog
 ---------
 
 0.1 - first version of wxColourPicker at wxCode
 
 
 Juan Antonio Ortega
 jortegalalmolda@gmail.com

